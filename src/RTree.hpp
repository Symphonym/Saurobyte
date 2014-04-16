#ifndef JL_RSTARTREE_HPP
#define JL_RSTARTREE_HPP

#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <algorithm>
#include <cassert>
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
	private:

		typedef const TType* UserdataPtr;

		// Array of bounding boxes, which parents use to store children bounds
		typedef std::array<BoundingBox, maxNodes> BoundsArray; 
		typedef std::vector<UserdataPtr> QueryResult;
		typedef std::unordered_set<unsigned int> OverflowMap;


		// Defined as 'p' in the paper
		const int ReinsertFactor = maxNodes * 0.3;


		struct RTreeChild
		{
			BoundingBox bounds;
			virtual ~RTreeChild() {};
		};
		typedef std::vector<RTreeChild*> ChildArray;

		struct Entry : public RTreeChild
		{
			UserdataPtr data;
			Entry(UserdataPtr newData)
				:
				data(newData)
			{};
		};

		struct Node : public RTreeChild
		{
			// A non-leaf node contains 'children'
			// A leaf node contains 'entries'

			// List of bounds for entry/children at the same index
			//BoundsArray bounds;

			// Amount of entries/childs in this node
			//unsigned int childrenCount;

			// R-tree level index, 0 is always the leaf level
			unsigned int level;

			ChildArray children;

			/*union
			{
				// Data entries within the node
				std::array<UserdataPtr, maxNodes> entries;

				// Children nodess
				std::array<Node*, maxNodes> children;

			} data;*/

			// Whether or not this is a leaf node; the innermost node
			bool leafNode;

			UserdataPtr entry(unsigned int index)
			{
				return static_cast<Entry*>(children.at(index))->data;
			};
			Node* child(unsigned int index)
			{
				return static_cast<Node*>(children.at(index));
			};

			const UserdataPtr entry(unsigned int index) const
			{
				return static_cast<Entry*>(children.at(index))->data;
			};
			const Node* child(unsigned int index) const
			{
				return static_cast<Node*>(children.at(index));
			};

			Node()
				:
				//childrenCount(0),
				level(0),
				leafNode(false)
			{
				children.reserve(minNodes);
			}
			~Node()
			{}


		};

		typedef std::unique_ptr<Node> NodePtr;

		// Search path trace, nodes and their parent node
		typedef std::vector<std::pair<Node*, Node*> > NodeSearchPath;

		// Used throughout the algorithms to clamp together data more easily
		typedef std::pair<const BoundingBox*, UserdataPtr>  BoxEntryPair;
		typedef std::pair<BoundingBox*, Node*>  BoxNodePair;
		typedef std::pair<BoundingBox*, int>  BoxIndexPair;


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
		float calculateOverlap(const BoundingBox &newBox, const ChildArray &children)
		{
			float overlapValue = 0;
			for(std::size_t i = 0; i < children.size(); i++)
			{
				const BoundingBox &otherBox = children.at(i)->bounds;

				// Add area of overlapping region
				overlapValue += calculateOverlap(newBox, otherBox);
			}

			return overlapValue;
		};
		BoundingBox calculateMBR(const ChildArray &children)
		{
			// Lowest point
			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float minZ = std::numeric_limits<float>::max();

			// Highest point
			float maxX = std::numeric_limits<float>::lowest();
			float maxY = std::numeric_limits<float>::lowest();
			float maxZ = std::numeric_limits<float>::lowest();

			for(std::size_t i = 0; i < children.size(); i++)
			{
				const BoundingBox &boundingBox = children.at(i)->bounds;

				if(boundingBox.getMinPoint().x < minX)
					minX = boundingBox.getMinPoint().x;
				if(boundingBox.getMinPoint().y < minY)
					minY = boundingBox.getMinPoint().y;
				if(boundingBox.getMinPoint().z < minZ)
					minZ = boundingBox.getMinPoint().z;

				if(boundingBox.getMaxPoint().x > maxX)
					maxX = boundingBox.getMaxPoint().x;
				if(boundingBox.getMaxPoint().y > maxY)
					maxY = boundingBox.getMaxPoint().y;
				if(boundingBox.getMaxPoint().z > maxZ)
					maxZ = boundingBox.getMaxPoint().z;
			}

			return BoundingBox(
				Vector3f(minX, minY, minZ),
				Vector3f(maxX, maxY, maxZ));
		}

		void invariant(Node &node, int tabCount)
		{
			JL_INFO_LOG("Child count: %i", node.childrenCount);
			std::string indent = "";
			for(int i = 0; i < tabCount; i++)
				indent += "    ";

			if(node.level != m_rootNode->level)
				assert(node.childrenCount >= minNodes && node.childrenCount <= maxNodes);
			JL_INFO_LOG("%sChildren count is valid!", indent.c_str());


			if(!node.leafNode && node.level == m_rootNode->level)
				assert(node.childrenCount >= 2);
			JL_INFO_LOG("%sRoot children count is valid!", indent.c_str());

			if(node.leafNode)
				assert(node.level == 0);
			JL_INFO_LOG("%sSame level leaf is valid!", indent.c_str());


			for(std::size_t i = 0; i < node.childrenCount; i++)
			{

				// Recurse into children and get their bounds
				if(!node.leafNode)
				{
					JL_INFO_LOG("%sNEW CHILD (Level %i, Child children %i)", indent.c_str(), node.data.children.at(i)->level,node.data.children.at(i)->childrenCount);
					invariant(*node.data.children.at(i), tabCount+1);
				}
			}
		}
		void invariant(const std::string &note, Node* node = nullptr)
		{
			if(node == nullptr)
				node = m_rootNode;
			//JL_INFO_LOG("                         R* Tree invariant test: %s", note.c_str());
			//invariant(*node, 0);
		}

		// Finds a node suitable for inserting an object with the 'newBox' bounding box
		// Returns a vector of all nodes in the search path, with the 'suitableNode' at the end of the vector
		NodeSearchPath findSuitableNode(const BoundingBox &newBox, NodeSearchPath &searchPath)
		{
			invariant("Find suitable node start");
			Node& currentNode = *searchPath.back().first;

			// If root is a leaf, it is suitable for data insertion
			if(currentNode.leafNode)
				return searchPath;
			else
			{
				// We know 'currentNode' isn't a leaf node, thus we only access its children

				// If children are leaves
				if(currentNode.level == 1)
				{
					// We know that the 'children' are leaves, thus we only access their entries

					// Most suitable node index to insert into, as of now
					std::pair<float, int> lowestOverlapNode = std::make_pair(
						std::numeric_limits<float>::max(),
						0);

					// Determine which child gives smallest overlap value
					for(std::size_t i = 0; i < currentNode.children.size(); i++)
					{
						Node *child = currentNode.child(i);
						BoundingBox mbr = child->bounds;
						float overlapValue = calculateOverlap(newBox, child->children);

						// Check if overlap value when inserting into this child is smaller
						if(overlapValue < lowestOverlapNode.first)
							lowestOverlapNode = std::make_pair(overlapValue, i);

						// Solve tied values
						else if(overlapValue == lowestOverlapNode.first)
						{
							// Change of MBR area of the currently most suitable node
							BoundingBox currentMBR = currentNode.children.at(lowestOverlapNode.second)->bounds;
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
						std::make_pair(currentNode.child(lowestOverlapNode.second), &currentNode));

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
					for(std::size_t i = 0; i < currentNode.children.size(); i++)
					{
						// Create a copy of the entries in the node and add the new data node to it
						Node *child = currentNode.child(i);
						BoundingBox mbr = child->bounds;

						BoundingBox biggerMbr = mbr;
						biggerMbr.enlarge(newBox); // MBR if we insert the new data

						float newMBRDelta = biggerMbr.getArea() - mbr.getArea();

						// Check if MBR change when inserting into this child is smaller
						if(newMBRDelta < lowestMBRNode.first)
							lowestMBRNode = std::make_pair(newMBRDelta, i);

						// Solve tied values
						else if(newMBRDelta == lowestMBRNode.first)
						{
							float currentArea = currentNode.children.at(lowestMBRNode.second)->bounds.getArea();
							float newArea = mbr.getArea();

							// Inserting into this child yields a smaller area change
							if(newArea < currentArea)
								lowestMBRNode = std::make_pair(newMBRDelta, i);
						}
					}

					searchPath.push_back(
						std::make_pair(currentNode.child(lowestMBRNode.second), &currentNode));

					// Venture down this node until we find a leaf node
					return findSuitableNode(newBox, searchPath);
				}

			}
			invariant("Find suitable node end");
		};
		NodeSearchPath findSuitableNode(const BoundingBox &newBox)
		{
			NodeSearchPath searchPath;
			searchPath.push_back(std::make_pair(m_rootNode, nullptr)); // nullptr since root has no parent
			return findSuitableNode(newBox, searchPath);
		};

		// Boundingbox axis comparator
		template<int AxisIndex> struct AxisComparator
		{
			bool operator () (const RTreeChild  *lhs, const RTreeChild *rhs) const
			{
				if(lhs->bounds.getMinPoint()[AxisIndex] == rhs->bounds.getMinPoint()[AxisIndex])
					return lhs->bounds.getMaxPoint()[AxisIndex] < rhs->bounds.getMaxPoint()[AxisIndex];
				else
					return lhs->bounds.getMinPoint()[AxisIndex] < rhs->bounds.getMinPoint()[AxisIndex];
			}
		};

		// Returns the margin value of an axis to split, and the value referenced by 'optimalDistribution_K' will
		// hold the 'k' value of the optimal distribution on this axis.
		float calculateOptimalSplit(std::vector<RTreeChild*> &sortedAxisList, unsigned int &optimalDistribution_K)
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

				BoundingBox firstMbr = sortedAxisList.at(0)->bounds;
				BoundingBox secondMbr = sortedAxisList.at(firstEntries)->bounds;

				// TODO are indexes right here?
				// meaning that should i start at 0, and i start at firstEntries, and end at maxNodes+1

				// In the loops below at start at the starting index + 1 since the first element is
				// used to initialize the MBR

				// The first half of the split
				for(unsigned int i = 1; i < firstEntries; i++)
					firstMbr.enlarge(sortedAxisList.at(i)->bounds);

				// The second half of the split
				for(unsigned int i = firstEntries+1; i < maxNodes+1; i++)
					secondMbr.enlarge(sortedAxisList.at(i)->bounds);

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
		// 'nodeToSplit' is the node that we want to split
		// returns the second half of the split
		Node* splitNode(Node *nodeToSplit)
		{
			invariant("Split node start");
			// TODO newEntry might not be needed as arg
			// TODO does this sort by 'upper' and 'lower' values? Or just one of them? Paper says to sort by both.

			std::vector<RTreeChild*> sortedX;
			std::vector<RTreeChild*> sortedY;
			std::vector<RTreeChild*> sortedZ;

			for(std::size_t i = 0; i < nodeToSplit->children.size(); i++)
			{
				sortedX.push_back(nodeToSplit->children.at(i));
				sortedY.push_back(nodeToSplit->children.at(i));
				sortedZ.push_back(nodeToSplit->children.at(i));
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
			std::vector<RTreeChild*> *splitAxisVector = nullptr;
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

			optimalDistribution_K = (minNodes - 1) + optimalDistribution_K;

			if(splitAxisVector == nullptr)
			{
				JL_ERROR_LOG("R* tree splitting error, no optimal split could be found!");
				return nullptr;
			}

			std::vector<RTreeChild*> splitAxisRef = *splitAxisVector;



			// Put entries of first group into the first node
			nodeToSplit->children.clear();

			nodeToSplit->bounds = splitAxisRef.at(0)->bounds;
			nodeToSplit->children.push_back(splitAxisRef.at(0));

			JL_INFO_LOG("SPLITTED NODE IS LEAF %i", nodeToSplit->leafNode);

			for(std::size_t i = 1; i < optimalDistribution_K; i++)
			{
				nodeToSplit->children.push_back(splitAxisRef.at(i));
				nodeToSplit->children.back()->bounds.enlarge(splitAxisRef.at(i)->bounds);
				//nodeToSplit.children.at(i)->bounds.enlarge(*splitAxisRef.at(i).first);
				//nodeToSplit.second->data.entries.at(nodeToSplit.second->childrenCount++) = 
				//	entryCopies.at(splitAxisRef.at(i).second);
			}

			invariant("First split", nodeToSplit);

			Node *newNode = new Node();

			// Put entries of second group in the second node
			newNode->bounds = splitAxisRef.at(optimalDistribution_K)->bounds;
			newNode->children.push_back(splitAxisRef.at(optimalDistribution_K));

			newNode->leafNode = nodeToSplit->leafNode; // Make sure they're at the same level
			newNode->level = nodeToSplit->level;

			for(std::size_t i = optimalDistribution_K+1; i < maxNodes+1; i++)
			{
				newNode->children.push_back(splitAxisRef.at(i));
				newNode->children.back()->bounds.enlarge(splitAxisRef.at(i)->bounds);
			}
			invariant("Split node end");

			return newNode;
			//}
			/*else
			{
				std::array<Node*, maxNodes> childrenCopies = nodeToSplit.second->data.children;

				nodeToSplit.second->childrenCount = 1; // Initialize with first element
				*nodeToSplit.first = *splitAxisRef.at(0).first;
				nodeToSplit.second->data.children.at(0) = childrenCopies.at(splitAxisRef.at(0).second);

				JL_INFO_LOG("SPLITTED NODE IS LEAF %i", nodeToSplit.second->leafNode);

				for(std::size_t i = 1; i < optimalDistribution_K; i++)
				{
					nodeToSplit.first->enlarge(*splitAxisRef.at(i).first);
					nodeToSplit.second->data.children.at(nodeToSplit.second->childrenCount++) = 
						childrenCopies.at(splitAxisRef.at(i).second);
				}

				invariant("First split", nodeToSplit.second);

				// Put children of second group in the second node
				newNode.second->childrenCount = 1; // Initialize with first element
				*newNode.first = *splitAxisRef.at(optimalDistribution_K).first;
				newNode.second->data.children.at(0) = childrenCopies.at(splitAxisRef.at(optimalDistribution_K).second);

				newNode.second->leafNode = nodeToSplit.second->leafNode; // Make sure they're at the same level
				newNode.second->level = nodeToSplit.second->level;

				for(std::size_t i = optimalDistribution_K+1; i < maxNodes; i++)
				{
					newNode.first->enlarge(*splitAxisRef.at(i).first);
					newNode.second->data.children.at(newNode.second->childrenCount++) =
						childrenCopies.at(splitAxisRef.at(i).second);
				}
			}*/
			// Put entries of first group into the first node
			/*nodeToSplit.second->childrenCount = 1; // Initialize with first element
			*nodeToSplit.first = *splitAxisRef.at(0).first;
			nodeToSplit.second->data.entries.at(0) = splitAxisRef.at(0).second;

			JL_INFO_LOG("SPLITTED NODE IS LEAF %i", nodeToSplit.second->leafNode);

			for(std::size_t i = 1; i < optimalDistribution_K; i++)
			{
				nodeToSplit.first->enlarge(*splitAxisRef.at(i).first);
				nodeToSplit.second->data.entries.at(nodeToSplit.second->childrenCount++) = splitAxisRef.at(i).second;
			}

			invariant("First split", nodeToSplit.second);

			// Put entries of second group in the second node
			newNode.second->childrenCount = 1; // Initialize with first element
			*newNode.first = *splitAxisRef.at(optimalDistribution_K).first;
			newNode.second->data.entries.at(0) = splitAxisRef.at(optimalDistribution_K).second;
			newNode.second->leafNode = nodeToSplit.second->leafNode; // Make sure they're at the same level
			newNode.second->level = nodeToSplit.second->level;

			for(std::size_t i = optimalDistribution_K+1; i < maxNodes+1; i++)
			{
				newNode.first->enlarge(*splitAxisRef.at(i).first);
				newNode.second->data.entries.at(newNode.second->childrenCount++) = splitAxisRef.at(i).second;
			}*/
			//invariant("Second split", newNode.second);

		};

		void reinsert(Node *node, OverflowMap &overflowMap)
		{
			// This allows us to sort by distance, and then update the Node data members
			typedef std::pair<float, RTreeChild*>  DistanceChildPair;

			JL_INFO_LOG("REINSERTING INTO LEAFNODE %i", node->leafNode);

			// This should always hold true
			assert(node->children.size() == maxNodes+1);

			std::vector<DistanceChildPair> sortedByDistance;
			sortedByDistance.reserve(maxNodes+1);

			Vector3f nodeCenter = node->bounds.getCenter();
			for(std::size_t i = 0; i < node->children.size(); i++)
			{
				Vector3f entryCenter = node->child(i)->bounds.getCenter();
				sortedByDistance.push_back(
					DistanceChildPair(glm::distance(entryCenter, nodeCenter), node->children.at(i)));
						//BoxEntryPair(&node->bounds.at(i), node->data.entries.at(i))));
			}

			// Add the new entry
			//sortedByDistance.push_back(
			//	DistanceChildPair(glm::distance(newEntry.first->getCenter(), nodeCenter),
			//		newEntry));


			// Sort by distance
			std::partial_sort(sortedByDistance.begin(), sortedByDistance.end() - ReinsertFactor, sortedByDistance.end(),
				[] (const DistanceChildPair &lhs, const DistanceChildPair &rhs) -> bool
				{
					return lhs.first < rhs.first;
				});



			// Remove first 'ReinsertFactor' entries and move them to a separate vector
			std::vector<DistanceChildPair> toRemove(sortedByDistance.end() - ReinsertFactor, sortedByDistance.end());
			sortedByDistance.erase(sortedByDistance.end() - ReinsertFactor, sortedByDistance.end());

			node->children.clear();
			node->children.reserve(sortedByDistance.size());
			for(std::size_t i = 0; i < sortedByDistance.size(); i++)
				node->children.push_back(sortedByDistance[i].second);

			node->bounds = calculateMBR(node->children);



			// Update node data according to this distance sort
			//for(std::size_t i = 0; i < sortedByDistance.size(); i++)
			//{
			//	node.bounds.at(i) = *sortedByDistance.at(i).second.first;
			//	node.data.entries.at(i) = sortedByDistance.at(i).second.second;
			//}

			//node.childrenCount -= ReinsertFactor;

			// Reinsert the last 'ReinsertFactor' entries
			//parentNode.bounds.at(positionInParent) = calculateMBR(node.bounds, node.childrenCount); // Recalculate MBR
			for(std::size_t i = 0; i < toRemove.size(); i++)
			{
				insert(toRemove.at(i).second, overflowMap);
					//toRemove.at(i).second.second,
					//*toRemove.at(i).second.first, overflowMap);
			}


			// Decrease childrenCount after we reinsert, otherwise we'll just end up inesrting into
			// ourselves again.

			/*JL_INFO_LOG("Count -1: %i, Factor %i, Total %i", (node.childrenCount - 1), ReinsertFactor, (node.childrenCount - 1) + ReinsertFactor);
			int startingIndex = node.childrenCount;
			int endIndex = node.childrenCount + ReinsertFactor;
			JL_INFO_LOG("START %i, END %i", startingIndex, endIndex);
			for(std::size_t i = startingIndex; i < endIndex; i++)
			{
				JL_INFO_LOG("REINSERT %i, TOTAL %i", i, ReinsertFactor);
				insert(node.data.entries.at(i), node.bounds.at(i), false);
			}*/
		}

		void overflowTreatment(int pathIndex, NodeSearchPath &searchPath, OverflowMap &overflowMap)
		{
			invariant("Overflow treatment start");
			Node *overflowNode = searchPath.at(pathIndex).first;
			Node *parent = searchPath.at(pathIndex).second;

			if(overflowNode->children.size() > maxNodes)
			{
				// Make sure we aren't the root node and haven't already invoked OverflowTreatment
				if(overflowNode->level != m_rootNode->level && overflowMap.find(overflowNode->level) == overflowMap.end())
				{
					overflowMap.insert(overflowNode->level);
					reinsert(overflowNode, overflowMap);
				}

				// Propagate upwards
				//else if(overflowMap.find(overflowNode->level) != overflowMap.end())
				//{
				//	JL_INFO_LOG("Propagate");
			//	/	overflowTreatment(desiredLevel+1,newEntry, pathIndex-1, searchPath, overflowMap);
				//}
				else
				{

					invariant("Pre overflow split");

					// If we're splitting root, use a temporary bounding box to store the bounds since root doesn't
					// have a parent that stores its bounds.
					//BoundingBox &bounds = overflowNode ==  m_rootNode ?
					//	newRootBounds : parent->bounds.at(parentIndex);

					Node* newSplitNode = splitNode(overflowNode);
						//BoxNodePair(&overflowNode->bounds, overflowNode),
						//BoxNodePair(&splitNodeBounds, splitSecondHalf));



					// If we split the root, create a new root
					if(overflowNode->level == m_rootNode->level)
					{
						invariant("Pre root split");
						JL_INFO_LOG("ROOT SPLIT");
						Node *newRoot = new Node();

						newRoot->children.push_back(newSplitNode);
						newRoot->children.push_back(m_rootNode);

						newRoot->bounds = calculateMBR(newRoot->children);
						newRoot->level = overflowNode->level + 1;

						m_rootNode = newRoot;
						invariant("Post root split");
					}

					// Else just insert the new element into the parent
					else
					{
						invariant("Pre normal split", parent);
						//JL_INFO_LOG("uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuNORMAL SPLIT, INSERT TO PARENTERU %i", parent->level);
						//int newIndex = parent->childrenCount++;
						parent->children.push_back(newSplitNode);
						parent->bounds = calculateMBR(parent->children);
						//parent->bounds.at(newIndex) = splitNodeBounds;
						invariant("Post normal split", parent);
					}

					invariant("Post overflow split");

					if(pathIndex-1 >= 0)
						overflowTreatment(pathIndex-1, searchPath, overflowMap);
				}

				
			}
			invariant("Overflow treatment end");
		}


		void insert(RTreeChild *entry, int desiredLevel, OverflowMap &overflowMap)
		{
			NodeSearchPath searchPath = findSuitableNode(entry->bounds);

			Node *suitableNode = searchPath.back().first;
			Node *parent = searchPath.back().second;

			// Push another entry into the node
			JL_INFO_LOG("INSERT NORMAL into level %i", suitableNode->level);
			suitableNode->children.push_back(entry);

			// Run overflow treatment if we have M+1 entries
			if(suitableNode->children.size() > maxNodes)
				overflowTreatment(searchPath.size()-1, searchPath, overflowMap);

			// Shit, the most suitable node is full! Invoke OverflowTreatment
			/*int overflowIndex = searchPath.size() - 1;
			while(overflowIndex >= 0)
			{
				Node *overflowNode = searchPath[overflowIndex].first;
				int parentIndex = searchPath[overflowIndex].second;

				--overflowIndex;
				if(overflowNode->childrenCount == maxNodes)
				{
					JL_INFO_LOG("INSERT WAS FULL");

					// Make sure we aren't the root node and haven't already invoked OverflowTreatment
					if(overflowNode->level != m_rootNode->level && overflowMap.find(overflowNode->level) == overflowMap.end())
					{
						JL_INFO_LOG("REINERT");
						overflowMap.insert(overflowNode->level);
						reinsert(*searchPath.at(overflowIndex).first, *overflowNode, parentIndex, overflowMap);
					}
					else
					{
						BoundingBox splitNodeBounds, newRootBounds;
						Node *splitSecondHalf = new Node();

						// If we're splitting root, use a temporary bounding box to store the bounds since root doesn't
						// have a parent that stores its bounds.
						BoundingBox &bounds = overflowNode ==  m_rootNode ?
							newRootBounds : searchPath.at(overflowIndex).first->bounds.at(parentIndex);

						splitNode(
							BoxNodePair(&bounds, overflowNode),
							BoxNodePair(&splitNodeBounds, splitSecondHalf),
							BoxEntryPair(&entryBounds, newEntry));

						JL_INFO_LOG("INSERT SPLIT");


						// If we split the root, create a new root
						if(overflowNode == m_rootNode)
						{
							JL_INFO_LOG("ROOT SPLIT");
							Node *newRoot = new Node();
							newRoot->childrenCount = 2;

							newRoot->data.children.at(0) = splitSecondHalf;
							newRoot->bounds.at(0) = splitNodeBounds;

							newRoot->data.children.at(1) = m_rootNode;
							newRoot->bounds.at(1) = newRootBounds;

							newRoot->level = overflowNode->level + 1;

							m_rootNode = newRoot;
						}

						// Else just insert the new element into the parent
						else
						{
							JL_INFO_LOG("uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuNORMAL SPLIT, INSERT TO PARENTERU");
							Node *parent = searchPath.at(overflowIndex).first;

							int newIndex = parent->childrenCount++;
							parent->data.children.at(newIndex) = splitSecondHalf;
							parent->bounds.at(newIndex) = splitNodeBounds;
						}
					}
				}

			}*/

			/*else if(suitableNode.childrenCount == maxNodes)
			{
				JL_INFO_LOG("INSERT WAS FULL");
				Node *betterSuitedNode = &suitableNode;
				int parentIndex = searchPath.back().second;

				if(&suitableNode != m_rootNode && overflowMap.find(suitableNode.level) != overflowMap.end())
				{
					JL_INFO_LOG("SIZERU %i", (int)searchPath.size()-2);
					betterSuitedNode = searchPath.at(searchPath.size()-2).first;
					parentIndex = searchPath.at(searchPath.size()-2).second;
					JL_INFO_LOG("PSTSERURE");
				}

				// Make sure we aren't the root node and haven't already invoked OverflowTreatment
				if(betterSuitedNode != m_rootNode && overflowMap.find(betterSuitedNode->level) == overflowMap.end())
				{
					JL_INFO_LOG("REINERT");
					overflowMap.insert(betterSuitedNode->level);
					reinsert(*parent, *betterSuitedNode, parentIndex, overflowMap);
				
				}
				//else if(parent != nullptr && parent != m_rootNode && overflowMap.find(betterSuitedNode->level) != overflowMap.end())
				//{
				//	JL_INFO_LOG("OVERFLOWERdddddddddddddddddddddddddddddddddddddddddddddU");
				//	reinsert(*searchPath.at(searchPath.size()-3).first, *parent, searchPath.at(searchPath.size()-2).second, overflowMap);
				//}
				else
				{
					BoundingBox splitNodeBounds, newRootBounds;
					Node *splitSecondHalf = new Node();

					// If we're splitting root, use a temporary bounding box to store the bounds since root doesn't
					// have a parent that stores its bounds.
					BoundingBox &bounds = betterSuitedNode ==  m_rootNode ?
						newRootBounds : searchPath.at(searchPath.size()-2).first->bounds.at(parentIndex);

					splitNode(
						BoxNodePair(&bounds, betterSuitedNode),
						BoxNodePair(&splitNodeBounds, splitSecondHalf),
						BoxEntryPair(&entryBounds, newEntry));

					JL_INFO_LOG("INSERT SPLIT");


					// If we split the root, create a new root
					if(betterSuitedNode == m_rootNode)
					{
						JL_INFO_LOG("ROOT SPLIT");
						Node *newRoot = new Node();
						newRoot->childrenCount = 2;

						newRoot->data.children.at(0) = splitSecondHalf;
						newRoot->bounds.at(0) = splitNodeBounds;

						newRoot->data.children.at(1) = m_rootNode;
						newRoot->bounds.at(1) = newRootBounds;

						newRoot->level = betterSuitedNode->level + 1;

						m_rootNode = newRoot;
					}

					// Else just insert the new element into the parent
					else
					{
						JL_INFO_LOG("NORMAL SPLIT, INSERT TO PARENTERU");
						Node *parent = searchPath.at(searchPath.size()-2).first;

						int newIndex = parent->childrenCount++;
						parent->data.children.at(newIndex) = splitSecondHalf;
						parent->bounds.at(newIndex) = splitNodeBounds;
					}
				}
			}*/

			// Go through search path and update MBR's, start at the 'back' which is the leafNode
			unsigned int parentIndex = std::numeric_limits<unsigned int>::max();
			for(std::size_t i = searchPath.size() - 1; i > 0; i--)
			{
				Node& nodeInPath = *searchPath.at(i).first;

				// We have no interest in updating the bounds of the entries in the leafNode
				if(nodeInPath.leafNode)
				{
					//parentIndex = searchPath.at(i).second;
					continue;
				}

				nodeInPath.bounds = calculateMBR(nodeInPath.children);
				//nodeInPath.bounds.at(parentIndex) = calculateMBR(
				//	nodeInPath.data.children.at(parentIndex)->bounds, nodeInPath.data.children.at(parentIndex)->childrenCount);

				//parentIndex = searchPath.at(i).second;
			}
		};

		void query(const Node &nodeToQuery, const BoundingBox &queryBounds, QueryResult &queryResult)
		{
			for(std::size_t i = 0; i < nodeToQuery.children.size(); i++)
			{
				RTreeChild *child = nodeToQuery.children.at(i);

				// Check if bounds of node/entry intersects our query bounds
				if(child->bounds.intersects(queryBounds))
				{
					// Add intersecting entries from leaf node
					if(nodeToQuery.leafNode)
						queryResult.push_back(nodeToQuery.entry(i));

					// Recurse into non-leaf nodes
					else
						query(*nodeToQuery.child(i), queryBounds, queryResult);
				}
				
			}
		};

		void getAllBounds(const Node &nodeToQuery, std::vector<BoundingBox> &boundList) const
		{
			for(std::size_t i = 0; i < nodeToQuery.children.size(); i++)
			{
				boundList.push_back(nodeToQuery.children.at(i)->bounds);

				// Recurse into children and get their bounds
				if(!nodeToQuery.leafNode)
					getAllBounds(*nodeToQuery.child(i), boundList);
			}
		};


		void printTree(const Node &node, int tabCount)
		{
			std::string indent = "";
				for(int i = 0; i < tabCount; i++)
					indent += "  ";

			for(std::size_t i = 0; i < node.children.size(); i++)
			{

				// Recurse into children and get their bounds
				if(!node.leafNode)
				{
					JL_INFO_LOG("%sCHILD (Level %i)", indent.c_str(), node.child(i)->level);
					printTree(*node.child(i), tabCount+1);
				}
				else
					JL_INFO_LOG("%sENTRY (Level %i)", indent.c_str(), node.level);
			}
		};

		void deleteTree(Node *node)
		{
			for(std::size_t i = 0; i < node->children.size(); i++)
			{

				// Recurse into children to delete them as well
				if(!node->leafNode)
					deleteTree(node->child(i));
				else
					delete node->children.at(i);

				// The memory of entries are not managed by the rtree 
			}

			delete node;
		};


		void deleteTree()
		{
			deleteTree(m_rootNode);
		}

	public:

		void insert(const TType *newEntry, const BoundingBox &entryBounds)
		{
			OverflowMap overflowMap;

			Entry *ent = new Entry(newEntry);
			ent->bounds = entryBounds;
			insert(ent, overflowMap);
		};

		QueryResult query(const BoundingBox &queryBounds)
		{
			// Store result in temporary list
			QueryResult result;
			query(*m_rootNode, queryBounds, result);

			// Return a copy of the results
			return result;
		};

		// This iterates the whole R-tree, grabbing all entry and node bounds which can later be
		// used for rendering. This is not a fast operation.
		std::vector<BoundingBox> getAllBounds() const
		{
			std::vector<BoundingBox> result;
			getAllBounds(*m_rootNode, result);
			return result;
		};

		void printTree()
		{
			JL_INFO_LOG("ROOT (Level %i)", m_rootNode->level);
			printTree(*m_rootNode, 1);
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
			deleteTree();
		};

	};
};

#endif