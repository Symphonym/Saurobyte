#ifndef JL_RSTARTREE_HPP
#define JL_RSTARTREE_HPP

#include <vector>
#include <array>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include "BoundingBox.hpp"
#include "Math.hpp"
#include "Logger.hpp"

namespace jl
{
	/*
		RTree

		R*-tree implementation used for 3D spatial partitioning of entities
		Based on: http://dbs.mathematik.uni-marburg.de/publications/myPapers/1990/BKSS90.pdf
	*/
	template<typename TType, unsigned int minNodes = 4, unsigned int maxNodes = 8> class RTree
	{
	public:

		typedef std::array<BoundingBox, maxNodes> BoundsArray; 
		typedef std::pair<BoundingBox, const TType*> AxisEntryPair;
		typedef std::unordered_set<unsigned int> OverflowMap;

		const int ReinsertFactor = maxNodes * 0.3;


		struct Node
		{
			// A non-leaf node contains 'children'
			// A leaf node contains 'entries'

			// List of bounds for entry/children at the same index
			BoundsArray bounds;

			// Amount of entries/childs in this node
			unsigned int childrenCount;

			union
			{
				// Data entries within the node
				std::array<const TType*, maxNodes> entries;

				// Children nodess
				std::array<Node*, maxNodes> children;

			} data;

			// Whether or not this is a leaf node; the innermost node
			bool leafNode;

			Node()
				:
				childrenCount(0),
				leafNode(false)
			{
			}
			~Node()
			{}


		};

		// Search path trace, nodes and their index in the parent array
		typedef std::vector<std::pair<Node*, int> > NodeSearchPath;


		Node *m_rootNode;


		// Calculate overlapping value of a new boundingbox against an existing array of boundingboxes
		float calculateOverlap(const BoundingBox &box1, const BoundingBox &box2)
		{
			// The newBox intersects with one of the other bounds
			if(box1.intersects(box2))
			{
				// Get the bounds of the area that is shared
				float width = std::fabs(box1.getSize().x - box2.getSize().x);
				float height = std::fabs(box1.getSize().y - box2.getSize().y);
				float depth = std::fabs(box1.getSize().z - box2.getSize().z);

				return width*height*depth;
			}

			return 0;
		}
		float calculateOverlap(const BoundingBox &newBox, const BoundsArray &bounds, std::size_t maxIndex)
		{
			float overlapValue = 0;
			for(std::size_t i = 0; i < maxIndex; i++)
			{
				const BoundingBox &otherBox = bounds[i];

				// Add area of overlapping region
				overlapValue += calculateOverlap(newBox, otherBox);
			}

			return overlapValue;
		};
		BoundingBox calculateMBR(const BoundsArray &bounds, std::size_t maxIndex)
		{
			// Lowest point
			std::priority_queue<float, std::vector<float>, std::greater<float> > minX, minY, minZ;

			// Highest point
			std::priority_queue<float> maxX, maxY, maxZ;

			for(std::size_t i = 0; i < maxIndex; i++)
			{
				const BoundingBox &boundingBox = bounds[i];
				minX.push(boundingBox.getMinPoint().x);
				minY.push(boundingBox.getMinPoint().y);
				minZ.push(boundingBox.getMinPoint().z);

				maxX.push(boundingBox.getMaxPoint().x);
				maxY.push(boundingBox.getMaxPoint().y);
				maxZ.push(boundingBox.getMaxPoint().z);
			}

			return BoundingBox(
				Vector3f(minX.top(), minY.top(), minZ.top()),
				Vector3f(maxX.top(), maxY.top(), maxZ.top()));
		}

		// Finds a node suitable for inserting an object with the 'newBox' bounding box
		// Returns a vector of all nodes in the search path, with the 'suitableNode' at the end of the vector
		NodeSearchPath findSuitableNode(const BoundingBox &newBox, NodeSearchPath &searchPath)
		{
			Node& currentNode = *searchPath.back().first;

			// If root is a leaf, it is suitable for data insertion
			if(currentNode.leafNode)
				return searchPath;
			else
			{
				// We know 'currentNode' isn't a leaf node, thus we only access its children

				// If children are leaves
				if(currentNode.data.children[0]->leafNode)
				{
					// We know that the 'children' are leaves, thus we only access their entries

					// Most suitable node index to insert into, as of now
					std::pair<float, int> lowestOverlapNode = std::make_pair(
						std::numeric_limits<float>::max(),
						0);

					// Determine which child gives smallest overlap value
					for(std::size_t i = 0; i < currentNode.childrenCount; i++)
					{
						Node *child = currentNode.data.children[i];
						BoundingBox mbr = currentNode.bounds[i];
						float overlapValue = calculateOverlap(newBox, child->bounds, child->childrenCount);

						// Check if overlap value when inserting into this child is smaller
						if(overlapValue < lowestOverlapNode.first)
							lowestOverlapNode = std::make_pair(overlapValue, i);

						// Solve tied values
						else if(overlapValue == lowestOverlapNode.first)
						{
							// Change of MBR area of the currently most suitable node
							BoundingBox currentMBR = currentNode.bounds[lowestOverlapNode.second];
							float currentMBRArea = currentMBR.getArea();
							float currentMBRDelta = currentMBR.enlarge(newBox).getArea() - currentMBRArea;

							// Change of MBR area with the newly considered node
							float newMBRCurrentArea = mbr.getArea();
							float newMBRDelta = mbr.enlarge(newBox).getArea() - newMBRCurrentArea;

							// Inserting into this child yields a smaller MBR change
							if(newMBRDelta < currentMBRDelta)
								lowestOverlapNode = std::make_pair(overlapValue, i);
						}
					}

					searchPath.push_back(
						std::make_pair(currentNode.data.children[lowestOverlapNode.second], lowestOverlapNode.second));

					// Venture down this node until we find a leaf node
					return findSuitableNode(newBox, searchPath);
				}

				// If children aren't leaves
				else
				{
					// We know that the 'children' aren't leaves, thus we only access their children


					// Most suitable node index to insert into, as of now
					std::pair<float, int> lowestMBRNode = std::make_pair(
						std::numeric_limits<float>::max(),
						0);

					// Determine which child gives the smallest MBR area change
					for(std::size_t i = 0; i < currentNode.childrenCount; i++)
					{
						// Create a copy of the entries in the node and add the new data node to it
						Node *child = currentNode.data.children[i];
						BoundingBox mbr = currentNode.bounds[i];

						BoundingBox biggerMbr = mbr;
						biggerMbr.enlarge(newBox); // MBR if we insert the new data

						float newMBRDelta = biggerMbr.getArea() - mbr.getArea();

						// Check if MBR change when inserting into this child is smaller
						if(newMBRDelta < lowestMBRNode.first)
							lowestMBRNode = std::make_pair(newMBRDelta, i);

						// Solve tied values
						else if(newMBRDelta == lowestMBRNode.first)
						{
							float currentArea = currentNode.bounds[lowestMBRNode.second].getArea();
							float newArea = mbr.getArea();

							// Inserting into this child yields a smaller area change
							if(newArea < currentArea)
								lowestMBRNode = std::make_pair(newMBRDelta, i);
						}
					}

					searchPath.push_back(
						std::make_pair(currentNode.data.children[lowestMBRNode.second], lowestMBRNode.second));

					// Venture down this node until we find a leaf node
					return findSuitableNode(newBox, searchPath);
				}

			}
		};
		NodeSearchPath findSuitableNode(const BoundingBox &newBox)
		{
			NodeSearchPath searchPath;
			searchPath.push_back(std::make_pair(m_rootNode, -1)); // Index is -1 since root has no parent
			return findSuitableNode(newBox, searchPath);
		};

		// Boundingbox axis comparator
		template<int AxisIndex> struct AxisComparator
		{
			bool operator () (const AxisEntryPair &lhs, const AxisEntryPair &rhs) const
			{
				if(lhs.first.getMinPoint()[AxisIndex] == rhs.first.getMinPoint()[AxisIndex])
					return lhs.first.getMaxPoint()[AxisIndex] < rhs.first.getMaxPoint()[AxisIndex];
				else
					return lhs.first.getMinPoint()[AxisIndex] < rhs.first.getMinPoint()[AxisIndex];
			}
		};

		// Returns the margin value of an axis to split, and the value referenced by 'optimalDistribution_K' will
		// hold the 'k' value of the optimal distribution on this axis.
		float calculateOptimalSplit(std::vector<AxisEntryPair> &sortedAxisList, unsigned int &optimalDistribution_K)
		{
			// This is how many ways we can split the axis
			unsigned int splitDistributions = maxNodes - (2*minNodes) + 2;

			float marginValue = 0;

			float lowestOverlap = std::numeric_limits<float>::max();
			float lowestArea = std::numeric_limits<float>::max();

			// Now lets test the 'goodness' of each split
			for(unsigned int k = 1; k < splitDistributions; k++)
			{
				unsigned int firstEntries = (minNodes - 1) + k;

				BoundingBox firstMbr = sortedAxisList[0].first;
				BoundingBox secondMbr = sortedAxisList[firstEntries].first;

				// TODO are indexes right here?
				// meaning that should i start at 0, and i start at firstEntries, and end at maxNodes+1

				// In the loops below at start at the starting index + 1 since the first element is
				// used to initialize the MBR

				// The first half of the split
				for(unsigned int i = 1; i < firstEntries; i++)
					firstMbr.enlarge(sortedAxisList[i].first);

				// The second half of the split
				for(unsigned int i = firstEntries+1; i < maxNodes+1; i++)
					secondMbr.enlarge(sortedAxisList[i].first);

				marginValue += firstMbr.getPerimeter() + secondMbr.getPerimeter();

				float overlapValue = calculateOverlap(firstMbr, secondMbr);
				float areaValue = firstMbr.getArea() + secondMbr.getArea();

				// Aha, this distribution is the best yet!
				if(overlapValue < lowestOverlap ||
					(overlapValue == lowestOverlap && areaValue < lowestArea))
				{
					lowestOverlap = overlapValue;
					lowestArea = areaValue;
					optimalDistribution_K = k;
				}
			}

			return marginValue;
		};

		// Split the node along the optimal axis and use the best distribution in which 'newBox'
		// can be inserted.
		// 'nodeToSplit' is the node, and it's bounding box that's currently full
		// 'newNode' is the node that was created by the parent for the splitting, that is, the second half of the split
		// 'newEntry' is the new entry that we want to fit into the 'nodeToSplit' leaf node
		void splitNode(
			const std::pair<BoundingBox&, Node&> &nodeToSplit,
			const std::pair<BoundingBox&, Node&> &newNode,
			const std::pair<const BoundingBox&, const TType*> &newEntry)
		{
			// TODO newEntry might not be needed as arg
			// TODO does this sort by 'upper' and 'lower' values? Or just one of them? Paper says to sort by both.
			std::vector<AxisEntryPair> sortedX;
			std::vector<AxisEntryPair> sortedY;
			std::vector<AxisEntryPair> sortedZ;

			// First add the new entry (+1)
			sortedX.push_back(newEntry);
			sortedY.push_back(newEntry);
			sortedZ.push_back(newEntry);

			// Then add the other entries (M)
			for(std::size_t i = 0; i < nodeToSplit.second.childrenCount; i++)
			{
				const TType *entry = nodeToSplit.second.data.entries[i];
				BoundingBox box = nodeToSplit.second.bounds[i];

				sortedX.push_back(std::make_pair(box, entry));
				sortedY.push_back(std::make_pair(box, entry));
				sortedZ.push_back(std::make_pair(box, entry));
			}

			// Sort the axis vectors
			std::sort(sortedX.begin(), sortedX.end(), AxisComparator<0>());
			std::sort(sortedY.begin(), sortedY.end(), AxisComparator<1>());
			std::sort(sortedZ.begin(), sortedZ.end(), AxisComparator<2>());


			// Optimal distribution on each axis, 'k' value
			unsigned int optimalDistribution_X = 0;
			unsigned int optimalDistribution_Y = 0;
			unsigned int optimalDistribution_Z = 0;

			float marginValue_X = calculateOptimalSplit(sortedX, optimalDistribution_X);
			float marginValue_Y = calculateOptimalSplit(sortedY, optimalDistribution_Y);
			float marginValue_Z = calculateOptimalSplit(sortedZ, optimalDistribution_Z);

			float smallestMargin = std::min({marginValue_X, marginValue_Y, marginValue_Z});

			// Axis to split, and distribution to use
			std::vector<AxisEntryPair> *splitAxisVector = nullptr;
			unsigned int optimalDistribution_K = 0;

			// Determine split axis
			if(smallestMargin == marginValue_X)
			{
				splitAxisVector = &sortedX;
				optimalDistribution_K = optimalDistribution_X;
			}
			else if(smallestMargin == marginValue_Y)
			{
				splitAxisVector = &sortedY;
				optimalDistribution_K = optimalDistribution_Y;
			}
			else if(smallestMargin == marginValue_Z)
			{
				splitAxisVector = &sortedZ;
				optimalDistribution_Z = optimalDistribution_Z;
			}

			if(splitAxisVector == nullptr)
			{
				JL_ERROR_LOG("R* tree error, no optimal split could be found!");
				return;
			}


			std::vector<AxisEntryPair> splitAxisRef = *splitAxisVector;

			// Put entries of first group into the first node
			nodeToSplit.second.childrenCount = 1; // Initialize with first element
			nodeToSplit.first = splitAxisRef[0].first;
			nodeToSplit.second.data.entries[0] = splitAxisRef[0].second;

			int realIndex = 1;
			for(std::size_t i = 1; i < optimalDistribution_K; i++)
			{
				nodeToSplit.first.enlarge(splitAxisRef[i].first);
				nodeToSplit.second.data.entries[realIndex] = splitAxisRef[i].second;
				nodeToSplit.second.childrenCount++;
				realIndex++;
			}

			// Put entries of second group in the second node
			newNode.second.childrenCount = 1; // Initialize with first element
			newNode.first = splitAxisRef[optimalDistribution_K].first;
			newNode.second.data.entries[0] = splitAxisRef[optimalDistribution_K].second;

			realIndex = 1;
			for(std::size_t i = optimalDistribution_K+1; i < maxNodes+1; i++)
			{
				newNode.first.enlarge(splitAxisRef[i].first);
				newNode.second.data.entries[realIndex] = splitAxisRef[i].second;
				newNode.second.childrenCount++;
				realIndex++;
			}

		};

		void reinsert(Node &parentNode, Node& node, int positionInParent)
		{
			// This allows us to sort by distance, and then update the Node data members
			std::vector<std::pair<float, std::pair<BoundingBox, const TType*> > > sortedByDistance(node.childrenCount);
			Vector3f nodeCenter = parentNode.bounds[positionInParent].getCenter();

			for(std::size_t i = 0; i < node.childrenCount; i++)
			{
				Vector3f entryCenter = node.bounds[i];
				sortedByDistance.push_back(
					std::make_pair(glm::distance(entryCenter, nodeCenter),
						std::make_pair(node.bounds[i], node.data.entries[i])));
			}
			std::sort(sortedByDistance.begin(), sortedByDistance.end());

			// Update node data according to this distance sort
			for(std::size_t i = 0; i < node.childrenCount; i++)
			{
				node.bounds[i] = sortedByDistance[i].second.first;
				node.entries[i] = sortedByDistance[i].second.second;
			}

			// Reinsert the last 'ReinsertFactor' entries
			node.childrenCount -= ReinsertFactor;
			for(std::size_t i = node.childrenCount + ReinsertFactor; i > node.childrenCount; i--)
				insert(node.data.entries[i], node.bounds[i], false);
		}


		void insert(const TType *newEntry, const BoundingBox &entryBounds, bool firstInsert = true)
		{
			NodeSearchPath searchPath = findSuitableNode(entryBounds);
			Node &suitableNode = *searchPath.back().first;

			// The node has room for another entry, so just add it
			if(suitableNode.childrenCount < maxNodes)
			{
				int newIndex = suitableNode.childrenCount++;
				suitableNode.data.entries[newIndex] = newEntry;
				suitableNode.bounds[newIndex] = entryBounds;
			}

			// Shit, the most suitable node is full! Invoke OverflowTreatment
			else if(suitableNode.childrenCount == maxNodes)
			{
				// Make sure we aren't the root node and haven't already invoked OverflowTreatment
				if(suitableNode != &m_rootNode && firstInsert)
					reinsert(searchPath[searchPath.size()-2], suitableNode, searchPath.back().second);
				else
				{
					BoundingBox splitNodeBounds;
					Node *splitNode = new Node();
					splitNode(
						std::make_pair(std::ref(suitableNode), searchPath[searchPath.size()-2].bounds[searchPath.back().second]),
						std::make_pair(std::ref(splitNodeBounds), splitNode),
						std::make_pair(std::ref(entryBounds), newEntry));

					// If we split the root, create a new root
					if(suitableNode.level == 0)
					{
						Node *newRoot = new Node();
						newRoot->childrenCount = 2;

						newRoot.data.children[0] = splitNode;
						newRoot.bounds[0] = splitNodeBounds;

						newRoot.data.children[1] = m_rootNode;
						newRoot.bounds[1] = calculateMBR(m_rootNode->bounds, m_rootNode->childrenCount);

						m_rootNode = newRoot;
					}

					// Else just insert the new element into the parent
					else
					{
						Node *parent = searchPath[searchPath.size()-2];

						int newIndex = parent->childrenCount++;
						parent->children[newIndex] = splitNode;
						parent->bounds[newIndex] = splitNodeBounds;
					}
					//std::pair<BoundingBox, const TType*> secondSplitHalf = std::make_pair()
				}
				//if(suitableNode.parent != nullptr)
					//reinsert(*suitableNode.parent, suitableNode,)
				// reinsert(suitableNode)
				// TODO how do we get parent in here? D:
			}
			// TODO handle overflowing etc else if

			// Go through search path and update MBR's, start at the 'back' which is the leafNode
			unsigned int parentIndex = std::numeric_limits<unsigned int>::max();
			for(std::size_t i = searchPath.size() - 1; i > 0; i--)
			{
				Node& nodeInPath = *searchPath[i].first;

				// We have no interest in updating the bounds of the entries in the leafNode
				if(nodeInPath.leafNode)
				{
					parentIndex = searchPath[i].second;
					continue;
				}

				nodeInPath.bounds[parentIndex] = calculateMBR(
					nodeInPath.children[parentIndex].bounds, nodeInPath.children[parentIndex].childrenCount);

				parentIndex = searchPath[i].second;
			}
		};

	public:

		void insert(const TType *newEntry, const BoundingBox &entryBounds)
		{
			insert(newEntry, entryBounds);
		};


		RTree()
			:
			m_rootNode()
		{
			m_rootNode = new Node();

			// Root node will always be the leaf node at startup, as the tree is empty
			m_rootNode->leafNode = true;

			static_assert(minNodes >= 2, "R-trees cannot have a minimum node values less than 2");
			static_assert(minNodes <= maxNodes/2, "R-trees cannot have a minimum node value above half the maximum value");
		};
		~RTree()
		{

		};

	};
};

#endif