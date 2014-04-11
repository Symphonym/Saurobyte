#ifndef JL_RSTARTREE_HPP
#define JL_RSTARTREE_HPP

#include <vector>
#include <array>
#include <queue>
#include <cmath>
#include <limits>
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
		float calculateOverlap(const BoundingBox &newBox, const BoundsArray &bounds, std::size_t maxIndex)
		{
			float overlapValue = 0;
			for(std::size_t i = 0; i < maxIndex; i++)
			{
				const BoundingBox &otherBox = bounds[i];

				// The newBox intersects with one of the other bounds
				if(newBox.intersects(otherBox))
				{
					// Get the bounds of the area that is shared
					float width = std::fabs(newBox.getSize().x - otherBox.getSize().x);
					float height = std::fabs(newBox.getSize().y - otherBox.getSize().y);
					float depth = std::fabs(newBox.getSize().z - otherBox.getSize().z);

					// Add area of overlapping region
					overlapValue += width*height*depth;
				}
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
		Node& findSuitableTree(Node &rootNode, const BoundingBox &newBox)
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
					return findSuitableTree(*lowestOverlapNode.second, newBox);
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

					return findSuitableTree(*rootNode.data.children[lowestMBRNode.second], newBox);
				}

			}
		};
		Node& findSuitableTree(const BoundingBox &newBox)
		{
			return findSuitableTree(m_rootNode, newBox);
		};

		void splitTree(Node &node)
		{
			unsigned int splitDistribution = maxNodes - (2*minNodes) + 2; 

			std::priority_queue<float> sortedX, sortedY, sortedZ;
			for(unsigned int k = 1; k < splitDistribution; k++)
			{
				// TODO I don't know what I'm doing, research more on splitting
			}
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