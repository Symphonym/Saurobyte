#ifndef JL_RSTARTREE_HPP
#define JL_RSTARTREE_HPP

#include <vector>
#include <array>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include "BoundingBox.hpp"
#include "Math.hpp"

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

		Node m_rootNode;


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
		Node& findSuitableNode(Node &rootNode, const BoundingBox &newBox)
		{
			// If root is a leaf, it is suitable for data insertion
			if(rootNode.leafNode)
				return rootNode;
			else
			{
				// We know 'rootNode' isn't a leaf node, thus we only access its children

				// If children are leaves
				if(rootNode.data.children[0]->leafNode)
				{
					// We know that the 'children' are leaves, thus we only access their entries

					// Most suitable node to insert into, as of now
					std::pair<float, Node*> lowestOverlapNode = std::make_pair(
						std::numeric_limits<float>::max(),
						nullptr);

					// Determine which child gives smallest overlap value
					for(std::size_t i = 0; i < rootNode.childrenCount; i++)
					{
						Node *child = rootNode.data.children[i];
						float overlapValue = calculateOverlap(newBox, child->bounds, child->childrenCount);

						// Check if overlap value when inserting into this child is smaller
						if(overlapValue < lowestOverlapNode.first)
							lowestOverlapNode = std::make_pair(overlapValue, child);

						// Solve tied values
						else if(overlapValue == lowestOverlapNode.first)
						{
							float currentMBRDelta =
								calculateMBR(lowestOverlapNode.second->bounds, lowestOverlapNode.second->childrenCount).enlarge(newBox).getArea() -
								calculateMBR(lowestOverlapNode.second->bounds, lowestOverlapNode.second->childrenCount).getArea();

							float newMBRDelta =
								calculateMBR(child->bounds, child->childrenCount).enlarge(newBox).getArea() -
								calculateMBR(child->bounds, child->childrenCount).getArea();

							// Inserting into this child yields a smaller MBR change
							if(newMBRDelta < currentMBRDelta)
								lowestOverlapNode = std::make_pair(overlapValue, child);
						}
					}

					// Venture down this node until we find a leaf node
					return findSuitableNode(*lowestOverlapNode.second, newBox);
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
					for(std::size_t i = 0; i < rootNode.childrenCount; i++)
					{
						// Create a copy of the entries in the node and add the new data node to it
						Node *child = rootNode.data.children[i];

						float newMBRDelta =
								calculateMBR(child->bounds, child->childrenCount).enlarge(newBox).getArea() -
								calculateMBR(child->bounds, child->childrenCount).getArea();

						// Check if MBR change when inserting into this child is smaller
						if(newMBRDelta < lowestMBRNode.first)
							lowestMBRNode = std::make_pair(newMBRDelta, i);

						// Solve tied values
						else if(newMBRDelta == lowestMBRNode.first)
						{
							float currentArea = rootNode.bounds[lowestMBRNode.second].getArea();
							float newArea = rootNode.bounds[i].getArea();

							// Inserting into this child yields a smaller area change
							if(newArea < currentArea)
								lowestMBRNode = std::make_pair(newMBRDelta, i);
						}
					}

					return findSuitableNode(*rootNode.data.children[lowestMBRNode.second], newBox);
				}

			}
		};
		Node& findSuitableNode(const BoundingBox &newBox)
		{
			return findSuitableNode(m_rootNode, newBox);
		};

		// Boundingbox axis comparator
		template<int AxisIndex> struct AxisComparator
		{
			bool operator () (const BoundingBox &lhs, const BoundingBox &rhs) const
			{
				if(lhs.getMinPoint()[AxisIndex] == rhs.getMinPoint()[AxisIndex])
					return lhs.getMaxPoint()[AxisIndex] < rhs.getMaxPoint()[AxisIndex];
				else
					return lhs.getMinPoint()[AxisIndex] < rhs.getMinPoint()[AxisIndex];
			}
		};

		// Returns the margin value of an axis to split, and the value referenced by 'optimalDistribution_K' will
		// hold the 'k' value of the optimal distribution on this axis.
		template<int AxisIndex> float calculateOptimalSplit(
			std::priority_queue<AxisEntryPair, std::vector<AxisEntryPair>, AxisComparator<AxisIndex> > &sortedAxisList,
			unsigned int &optimalDistribution_K)
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

				BoundingBox firstMbr = sortedAxisList.first[0];
				BoundingBox secondMbr = sortedAxisList.first[firstEntries];

				// TODO are indexes right here?
				// meaning that should i start at 0, and i start at firstEntries, and end at maxNodes+1

				// The first half of the split
				for(unsigned int i = 0; i < firstEntries; i++)
					firstMbr.enlarge(sortedAxisList.first[i]);

				// The second half of the split
				for(unsigned int i = firstEntries; i < maxNodes+1; i++)
					secondMbr.enlarge(sortedAxisList.first[i]);

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
		void splitNode(Node &nodeToSplit, const TType *newEntry, const BoundingBox &newBox)
		{
			// TODO newEntry might not be needed as arg
			// TODO does this sort by 'upper' and 'lower' values? Or just one of them? Paper says to sort by both.
			std::priority_queue<AxisEntryPair, std::vector<AxisEntryPair>, AxisComparator<0> > sortedX;
			std::priority_queue<AxisEntryPair, std::vector<AxisEntryPair>, AxisComparator<1> > sortedY;
			std::priority_queue<AxisEntryPair, std::vector<AxisEntryPair>, AxisComparator<2> > sortedZ;

			// First add the new entry (+1)
			sortedX.push(std::make_pair(newBox, newEntry));
			sortedY.push(std::make_pair(newBox, newEntry));
			sortedZ.push(std::make_pair(newBox, newEntry));

			// Then add the other entries (M)
			for(std::size_t i = 0; i < nodeToSplit.entries.size(); i++)
			{
				const TType *entry = nodeToSplit.entries[i];
				BoundingBox box = nodeToSplit.bounds[i];

				sortedX.push(std::make_pair(box, entry));
				sortedY.push(std::make_pair(box, entry));
				sortedZ.push(std::make_pair(box, entry));
			}

			// Optimal distribution on each axis, 'k' value
			unsigned int optimalDistribution_X = 0;
			unsigned int optimalDistribution_Y = 0;
			unsigned int optimalDistribution_Z = 0;

			float marginValue_X = calculateOptimalSplit<0>(sortedX, optimalDistribution_X);
			float marginValue_Y = calculateOptimalSplit<1>(sortedY, optimalDistribution_Y);
			float marginValue_Z = calculateOptimalSplit<2>(sortedZ, optimalDistribution_Z);

			float smallestMargin = std::min({marginValue_X, marginValue_Y, marginValue_Z});

			// Axis to split, and distribution to use
			int splitAxis = -1;
			unsigned int optimalDistribution_K = 0;

			// Determine split axis
			if(smallestMargin == marginValue_X)
			{
				splitAxis = 0;
				optimalDistribution_K = optimalDistribution_X;
			}
			else if(smallestMargin == marginValue_Y)
			{
				splitAxis = 1;
				optimalDistribution_K = optimalDistribution_Y;
			}
			else if(smallestMargin == marginValue_Z)
			{
				splitAxis = 2;
				optimalDistribution_Z = optimalDistribution_Z;
			}
			
			// TODO we know who the axis to split, and distribution of split, so do some vodoo split below here and all is
			// good
		};

	public:

		RTree()
			:
			m_rootNode()
		{
			// Root node will always be the leaf node at startup, as the tree is empty
			m_rootNode.leafNode = true;

			static_assert(minNodes >= 2, "R-trees cannot have a minimum node values less than 2");
			static_assert(minNodes <= maxNodes/2, "R-trees cannot have a minimum node value above half the maximum value");
		};
		~RTree()
		{

		};

	};
};

#endif