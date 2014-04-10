#ifndef JL_RSTARTREE_HPP
#define JL_RSTARTREE_HPP

#include <vector>
#include <array>
#include <queue>
#include <cmath>
#include "BoundingBox.hpp"
#include "Math.hpp"

namespace jl
{
	/*
		RTree

		R*-tree implementation used for 3D spatial partitioning of entities
		Based on: http://dbs.mathematik.uni-marburg.de/publications/myPapers/1990/BKSS90.pdf
	*/
	template<typename TType, std::size_t minNodes = 16, std::size_t maxNodes = 32> class RTree
	{
	public:

		typedef std::pair<const TType*, BoundingBox> Entry;
		typedef std::vector<Entry> EntryList;

		struct Node
		{
			typedef std::array<Node*, maxNodes> NodeChildren;

			// A non-leaf node contains 'children'
			// A leaf node contains 'entries'

			// Whether or not this is a leaf node; the innermost node
			bool leafNode;

			// Minimum bounding box for the children
			BoundingBox bounds;

			// Data entries within the node
			EntryList entries;

			// Children nodes along with their bounds
			NodeChildren children;

			Node()
				:
				leafNode(false),
				bounds({0,0,0}, {0,0,0})
			{}
			~Node()
			{}


		};

		Node m_rootNode;





		// Calculate overlapping value of a new entry against an entry list
		float calculateOverlap(const Entry &newEntry, const EntryList &entries)
		{
			float overlapValue = 0;
			for(std::size_t i = 0; i < entries.size(); i++)
			{
				const Entry &otherEntry = entries[i];

				// The entry intersects with one of the other entries
				if(newEntry.second.intersects(otherEntry.second))
				{
					// Get the bounds of the area that is shared
					float width = std::fabs(newEntry.second.getSize().x - otherEntry.second.getSize().x);
					float height = std::fabs(newEntry.second.getSize().y - otherEntry.second.getSize().y);
					float depth = std::fabs(newEntry.second.getSize().z - otherEntry.second.getSize().z);

					// Add area of overlapping region
					overlapValue += width*height*depth;
				}
			}

			return overlapValue;
		};
		BoundingBox calculateMBR(const EntryList &entryList)
		{
			// Lowest point
			std::priority_queue<float, std::vector<float>, std::greater<float> > minX, minY, minZ;

			// Highest point
			std::priority_queue<float> maxX, maxY, maxZ;

			for(std::size_t i = 0; i < entryList.size(); i++)
			{
				const Entry &entry = entryList[i];
				minX.push(entry.second.getMinPoint().x);
				minY.push(entry.second.getMinPoint().y);
				minZ.push(entry.second.getMinPoint().z);

				maxX.push(entry.second.getMaxPoint().x);
				maxY.push(entry.second.getMaxPoint().y);
				maxZ.push(entry.second.getMaxPoint().z);
			}

			return BoundingBox(
				Vector3f(minX.top(), minY.top(), minZ.top()),
				Vector3f(maxX.top(), maxY.top(), maxZ.top()));
		}

		// Finds a node suitable for data entry insertion
		Node& findSuitableTree(Node &rootNode, Entry &newEntry)
		{
			// If root is a leaf, it is suitable for data insertion
			if(rootNode.leafNode)
				return rootNode;
			else
			{
				// We know 'rootNode' isn't a leaf node, thus we only access its children

				// If children are leaves
				if(rootNode.children[0]->leafNode)
				{
					// We know that the 'children' are leaves, thus we only access their entries

					// Sorted list of nodes and their overlap values
					std::priority_queue<std::pair<float, Node*> > overlaps;

					// Determine which child gives smallest overlap value
					for(std::size_t i = 0; i < rootNode.children.size(); i++)
					{
						Node *child = rootNode.children[i];
						float overlapValue = calculateOverlap(newEntry, child->entries);
						overlaps.push(std::make_pair(overlapValue, child));
					}

					// Determine which child gives the smallest change in MBR area
					std::priority_queue<std::pair<float, Node*> > tiedOverlaps;
					float topValue = overlaps.top().first;

					// Resolve ties in overlap values
					while(overlaps.top().first == topValue)
					{
						// Create a copy of the entries in the node and add the new data node to it
						EntryList entryList = overlaps.top().second->entries;

						float previousArea = calculateMBR(entryList).getArea();
						entryList.push_back(newEntry); // Add new data node
						float deltaArea = calculateMBR(entryList).getArea() - previousArea;

						tiedOverlaps.push(std::make_pair(deltaArea, overlaps.top().second));
						overlaps.pop();
					}

					// Return the node with the smallest area change
					return findSuitableTree(*tiedOverlaps.top().second, newEntry);
				}

				// If children aren't leaves
				else
				{
					// We know that the 'children' aren't leaves, thus we only access their children

					std::priority_queue<std::pair<float, Node*> > mbrChanges;

					// Determine which child gives the smallest MBR area change
					for(std::size_t i = 0; i < rootNode.children.size(); i++)
					{
						// Create a copy of the entries in the node and add the new data node to it
						Node *childNode = rootNode.children[i];
						EntryList entryList = childNode->entries;

						float previousArea = calculateMBR(entryList).getArea();
						entryList.push_back(newEntry); // Add new data node
						float deltaArea = calculateMBR(entryList).getArea() - previousArea;

						mbrChanges.push(std::make_pair(deltaArea, childNode));
					}

					// Solve ties by choosing the child with the smallest MBR
					std::priority_queue<std::pair<float, Node*> > smallestMbr;
					float topValue = mbrChanges.top().first;

					while(mbrChanges.top().first == topValue)
					{
						smallestMbr.push(
							std::make_pair(mbrChanges.top().second->bounds.getArea(), mbrChanges.top().second));
						mbrChanges.pop();
					};

					return findSuitableTree(*smallestMbr.top().second, newEntry);
				}

			}
		};
		Node& findSuitableTree(Entry &newEntry)
		{
			return findSuitableTree(m_rootNode, newEntry);
		};

		void splitTree(Node &node)
		{

		};

	public:

		RTree()
		{
			static_assert(minNodes >= 2, "R-trees cannot have a minimum node values less than 2");
			static_assert(minNodes <= maxNodes/2, "R-trees cannot have a minimum node above half the maximum value");
		}

	};
};

#endif