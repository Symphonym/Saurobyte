#ifndef JL_RSTARTREE_HPP
#define JL_RSTARTREE_HPP

#include <vector>
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

		typedef std::vector<TType> QueryResult; 
		typedef std::unordered_set<unsigned int> OverflowMap; // Set of level numbers
		typedef std::size_t IdentifierType;


		// Defined as 'p' in the paper, it's how many children we remove when reinserting
		const int ReinsertFactor = maxNodes * 0.3;

		// Common base for children
		struct RTreeChild
		{
			BoundingBox bounds;
			virtual ~RTreeChild() {};
		};
		typedef std::vector<RTreeChild*> ChildArray;

		struct Entry : public RTreeChild
		{
			TType data;
			IdentifierType id;

			Entry(TType newData)
				:
				data(newData)
			{};
		};

		struct Node : public RTreeChild
		{
			// R-tree level index, 0 is always the leaf level
			unsigned int level;

			// Children of this node
			// A non-leaf node will ONLY contain 'Node' children
			// A leaf node will ONLY contain 'Entry' children
			ChildArray children;

			// Whether or not this is a leaf node; the innermost node at level 0
			bool leafNode;

			// Utility functions for easily grabbing a child or an entry in the node
			TType entry(unsigned int index)
			{
				return static_cast<Entry*>(children.at(index))->data;
			};
			Node* child(unsigned int index)
			{
				return static_cast<Node*>(children.at(index));
			};

			const TType entry(unsigned int index) const
			{
				return static_cast<Entry*>(children.at(index))->data;
			};
			const Node* child(unsigned int index) const
			{
				return static_cast<Node*>(children.at(index));
			};

			Node()
				:
				level(0),
				leafNode(false)
			{
				// So we don't have to allocate more than we need, thus reducing performance
				children.reserve(minNodes);
			}
			~Node()
			{
				//for(std::size_t i = 0; i < children.size(); i++)
				//	delete children.at(i);
			}


		};

		// Search path trace for the findSuitableNode algorithm
		// Pairs of nodes and their parent nodes
		typedef std::vector<std::pair<Node*, Node*> > NodeSearchPath;

		// List of entries to reinsert when underflowTreatment deletes underflowed nodes
		// Children together with the level of their parent
		typedef std::vector<std::pair<RTreeChild*, int> > ChildReinsertQueue;

		Node *m_rootNode;

		// ID's available for re-use
		std::vector<IdentifierType> m_spareIDs;
		IdentifierType m_idIndex;

		// Calculate overlapping value of a new boundingbox against an existing array of boundingboxes
		float calculateOverlap(const BoundingBox &box1, const BoundingBox &box2) const
		{
			// The newBox intersects with one of the other bounds
			if(box1.intersects(box2))
			{
				// Get the bounds of the area that is shared
				float width = 
					std::min(box2.getMaxPoint().x, box1.getMaxPoint().x) - std::max(box2.getMinPoint().x, box1.getMinPoint().x);
				float height = 
					std::min(box2.getMaxPoint().y, box1.getMaxPoint().y) - std::max(box2.getMinPoint().y, box1.getMinPoint().y);
				float depth = 
					std::min(box2.getMaxPoint().z, box1.getMaxPoint().z) - std::max(box2.getMinPoint().z, box1.getMinPoint().z);

				return width*height*depth;
			}

			return 0;
		}
		float calculateOverlap(const BoundingBox &newBox, const ChildArray &children) const
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

		// Calculate minimum bounding rectangle for an array of children
		BoundingBox calculateMBR(const ChildArray &children) const
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

		// Expensive debugging function that will validate the internal R-tree structure making
		// sure all the rules of an R-tree is followed.
		void invariant(Node *node, int tabCount)
		{
			// Indenting for neater output
			std::string indent = "";
			for(int i = 0; i < tabCount; i++)
				indent += "    ";

			if(node->level != m_rootNode->level)
				assert(node->children.size() >= minNodes && node->children.size() <= maxNodes+1);
			JL_INFO_LOG("%sChildren count is valid!", indent.c_str());


			if(!node->leafNode && node->level == m_rootNode->level)
				assert(node->children.size() >= 2);
			JL_INFO_LOG("%sRoot children count is valid!", indent.c_str());

			if(node->leafNode)
				assert(node->level == 0);
			JL_INFO_LOG("%sSame level leaf is valid!", indent.c_str());

			assert(node->bounds == calculateMBR(node->children));
			JL_INFO_LOG("%sMBR size is correct and up-to-date!");


			for(std::size_t i = 0; i < node->children.size(); i++)
			{
				// Recurse into children and validate them as well
				if(!node->leafNode)
					invariant(node->child(i), tabCount+1);
			}
		}
		void invariant(const std::string &note, Node* node = nullptr)
		{
			if(node == nullptr)
				node = m_rootNode;
			JL_INFO_LOG("R* Tree invariant test: %s", note.c_str());
			invariant(node, 0);
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
				// We know 'currentNode' isn't a leaf node, thus it only contains children nodes

				// If children are leaves
				if(currentNode.level == 1)
				{
					// We know that the children of 'currentNode' are leaf nodes, thus they only contain entries

					// Most suitable node index to insert into, as of now
					std::pair<float, int> lowestOverlapNode = std::make_pair(
						std::numeric_limits<float>::max(),
						0);

					// Determine which child gives smallest overlap value
					for(std::size_t i = 0; i < currentNode.children.size(); i++)
					{
						Node *child = currentNode.child(i);
						BoundingBox childBounds = child->bounds;
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
							float newMBRCurrentArea = childBounds.getArea();
							float newMBRDelta = childBounds.enlarge(newBox).getArea() - newMBRCurrentArea;

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
						BoundingBox childBounds = child->bounds;

						BoundingBox biggerMbr = childBounds;
						biggerMbr.enlarge(newBox); // MBR if we insert the new data

						float newMBRDelta = biggerMbr.getArea() - childBounds.getArea();

						// Check if MBR change when inserting into this child is smaller
						if(newMBRDelta < lowestMBRNode.first)
							lowestMBRNode = std::make_pair(newMBRDelta, i);

						// Solve tied values
						else if(newMBRDelta == lowestMBRNode.first)
						{
							float currentArea = currentNode.children.at(lowestMBRNode.second)->bounds.getArea();
							float newArea = childBounds.getArea();

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
		};
		NodeSearchPath findSuitableNode(const BoundingBox &newBox)
		{
			NodeSearchPath searchPath;
			searchPath.push_back(std::make_pair(m_rootNode, nullptr)); // nullptr since root has no parent
			return findSuitableNode(newBox, searchPath);
		};

		// Boundingbox axis comparator
		struct AxisComparator
		{
			const int AxisIndex;
			const int LowerOrUpper;

			explicit AxisComparator(const int axis, const int lowerOrUpper)
				:
				AxisIndex(axis),
				LowerOrUpper(lowerOrUpper)
			{};
			bool operator () (const RTreeChild  *lhs, const RTreeChild *rhs) const
			{
				if(LowerOrUpper == 0)
					return lhs->bounds.getMinPoint()[AxisIndex] < rhs->bounds.getMinPoint()[AxisIndex];
				else
					return lhs->bounds.getMaxPoint()[AxisIndex] < rhs->bounds.getMaxPoint()[AxisIndex];
				//if(lhs->bounds.getMinPoint()[AxisIndex] == rhs->bounds.getMinPoint()[AxisIndex])
				//	return lhs->bounds.getMaxPoint()[AxisIndex] < rhs->bounds.getMaxPoint()[AxisIndex];
				//else
				//	return lhs->bounds.getMinPoint()[AxisIndex] < rhs->bounds.getMinPoint()[AxisIndex];
			}
		};

		// Returns the margin value of an axis to split, and the value referenced by 'optimalDistribution_K' will
		// hold the 'k' value of the optimal distribution on this axis.
		float calculateOptimalSplit(std::vector<RTreeChild*> &sortedAxisList, unsigned int &optimalDistribution_K)
		{
			// This is how many ways we can split the axis
			unsigned int splitDistributions = maxNodes - (2*minNodes) + 2;


		

			// 0 is sorting by lower
			// 1 is sorting by upper
			int split_sort = 0;
			int split_axis = 0;

			int split_k = 1;


			float lowestMargin = std::numeric_limits<float>::max();

			for(int axis = 0; axis < 3; axis++)
			{
				float marginValue = 0;

				float lowestOverlap = std::numeric_limits<float>::max();
				float lowestArea = std::numeric_limits<float>::max();

				 // Best value for K on this axis
				int best_KValue = 0;

				for(int sorting = 0; sorting < 2; sorting++)
				{
					std::sort(sortedAxisList.begin(), sortedAxisList.end(), AxisComparator(axis, sorting));

					// Now lets test the 'goodness' of each split
					for(unsigned int k = 1; k < splitDistributions; k++)
					{
						unsigned int firstEntries = (minNodes - 1) + k;

						BoundingBox firstMbr = sortedAxisList.at(0)->bounds;
						BoundingBox secondMbr = sortedAxisList.at(firstEntries)->bounds;

						// We append 1 to the starting indexes since the first element is used to
						// initialize the MBR

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
							(overlapValue == lowestOverlap && areaValue < lowestArea)) // Solve ties by area
						{
							split_sort = sorting;
							lowestOverlap = overlapValue;
							lowestArea = areaValue;
							best_KValue = k;
						}
					}

					if(marginValue < lowestMargin)
					{
						split_axis = axis;
						split_k = best_KValue;
						lowestMargin = marginValue;
					}
				}
			}

			optimalDistribution_K = split_k;
			std::sort(sortedAxisList.begin(), sortedAxisList.end(), AxisComparator(split_axis, split_sort));
			

			return 0;
		};

		// Split the node along the optimal axis and use the best distribution of children
		// 'nodeToSplit' is the node that we want to split
		// returns the second half of the split
		Node* splitNode(Node *nodeToSplit)
		{
			std::vector<RTreeChild*> sortedX = nodeToSplit->children;
			std::vector<RTreeChild*> sortedY = nodeToSplit->children;
			std::vector<RTreeChild*> sortedZ = nodeToSplit->children;


			// Make a reference from the pointer, for convenience
			std::vector<RTreeChild*> sortedList = nodeToSplit->children;
			unsigned int optimalDistribution_K = 0;

			calculateOptimalSplit(sortedList, optimalDistribution_K);

			// Calculate number of entries in the first split with the value of 'k' we determined to be the best
			optimalDistribution_K = (minNodes - 1) + optimalDistribution_K;

			

			// Put entries of first group into the first node
			nodeToSplit->children.clear();
			nodeToSplit->children.reserve(optimalDistribution_K);

			for(std::size_t i = 0; i < optimalDistribution_K; i++)
				nodeToSplit->children.push_back(sortedList.at(i));

			// Recalculate MBR
			nodeToSplit->bounds = calculateMBR(nodeToSplit->children);


			// Put entries of second group in the second node
			Node *newNode = new Node();

			// Make sure both halves of the split are at the same level
			newNode->leafNode = nodeToSplit->leafNode; 
			newNode->level = nodeToSplit->level;

			for(std::size_t i = optimalDistribution_K; i < maxNodes+1; i++)
				newNode->children.push_back(sortedList.at(i));

			// Recalculate MBR
			newNode->bounds = calculateMBR(newNode->children);
			return newNode;
			
		};

		void reinsert(Node *node, OverflowMap &overflowMap)
		{
			// This allows us to sort by distance, and then update the Node data members
			typedef std::pair<float, RTreeChild*>  DistanceChildPair;

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
			}

			// Sort by distance
			std::partial_sort(sortedByDistance.begin(), sortedByDistance.end() - ReinsertFactor, sortedByDistance.end(),
				[] (const DistanceChildPair &lhs, const DistanceChildPair &rhs) -> bool
				{
					return lhs.first < rhs.first;
				});



			// Remove last 'ReinsertFactor' entries and move them to a separate vector
			std::vector<DistanceChildPair> toRemove(sortedByDistance.end() - ReinsertFactor, sortedByDistance.end());
			sortedByDistance.erase(sortedByDistance.end() - ReinsertFactor, sortedByDistance.end());

			node->children.clear();
			node->children.reserve(sortedByDistance.size());
			for(std::size_t i = 0; i < sortedByDistance.size(); i++)
				node->children.push_back(sortedByDistance[i].second);

			node->bounds = calculateMBR(node->children);

			// Reinsert the items, entries goes to the leaf level and children to their respective levels
			for(std::size_t i = 0; i < toRemove.size(); i++)
			{
				if(node->leafNode)
					insert(toRemove.at(i).second, 0, overflowMap);
				else
					insert(toRemove.at(i).second, node->level, overflowMap);
			}

		}

		void overflowTreatment(int pathIndex, NodeSearchPath &searchPath, OverflowMap &overflowMap)
		{
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
				else
				{

					Node* newSplitNode = splitNode(overflowNode);

					// If we split the root, create a new root
					if(overflowNode->level == m_rootNode->level)
					{
						Node *newRoot = new Node();


						newRoot->children.push_back(newSplitNode);
						newRoot->children.push_back(m_rootNode);


						newRoot->bounds = calculateMBR(newRoot->children);
						newRoot->level = overflowNode->level + 1;

						m_rootNode = newRoot;
					}

					// Else just insert the new element into the parent
					else
					{
						parent->children.push_back(newSplitNode);
						parent->bounds = calculateMBR(parent->children);
					}

					// Propagate upwards
					if(pathIndex-1 >= 0)
						overflowTreatment(pathIndex-1, searchPath, overflowMap);
				}

				
			}
		}

		void underflowTreatment(int pathIndex, NodeSearchPath &searchPath, ChildReinsertQueue &nodesToReinsert)
		{
			Node *underflowNode = searchPath.at(pathIndex).first;
			Node *parent = searchPath.at(pathIndex).second;

			if(underflowNode->level == m_rootNode->level)
				return;

			if(underflowNode->children.size() < minNodes)
			{


				// Flags the children of the underflow node for reinsertion
				for(std::size_t i = 0; i < underflowNode->children.size(); i++)
					nodesToReinsert.push_back(std::make_pair(underflowNode->children.at(i), underflowNode->level));

				underflowNode->children.clear();

				// Find the underflowNode in its parent, and remove it from the parent
				for(std::size_t i = 0; i < parent->children.size(); i++)
				{
					if(parent->child(i) == underflowNode)
					{
						delete parent->children.at(i);
						parent->children.erase(parent->children.begin() + i);
						break;
					}
				}

			}
			else
				underflowNode->bounds = calculateMBR(underflowNode->children);

			// Propagate underflowTreatment up
			underflowTreatment(pathIndex-1, searchPath, nodesToReinsert);
		}
		void underflowTreatment(int pathIndex, NodeSearchPath &searchPath)
		{
			ChildReinsertQueue nodesToReinsert;
			underflowTreatment(pathIndex, searchPath, nodesToReinsert);

			// Reinsert all the children whose parent were deleted
			for(std::size_t i = 0; i < nodesToReinsert.size(); i++)
			{
				OverflowMap overflowMap;
				insert(nodesToReinsert.at(i).first, nodesToReinsert.at(i).second, overflowMap);
			}

			// If the root node has only 1 child, the child becomes the new root
			if(m_rootNode->children.size() == 1)
			{
				Node *tmpNode = m_rootNode;
				m_rootNode = tmpNode->child(0);
				delete tmpNode;
			}
		}

		void insert(RTreeChild *entry, int desiredLevel, OverflowMap &overflowMap)
		{
			NodeSearchPath searchPath = findSuitableNode(entry->bounds);

			int pathIndex = searchPath.size() - 1;
			while(searchPath.at(pathIndex).first->level != desiredLevel)
				--pathIndex;


			Node *suitableNode = searchPath.at(pathIndex).first;
			Node *parent = searchPath.at(pathIndex).second;

			// Push another entry into the node
			suitableNode->children.push_back(entry);
			suitableNode->bounds = calculateMBR(suitableNode->children);

			// Run overflow treatment if we have M+1 entries
			if(suitableNode->children.size() > maxNodes)
				overflowTreatment(pathIndex, searchPath, overflowMap);

			// Go through search path and update MBR's, start at the 'back' which is the leafNode
			while(pathIndex >= 0)
			{
				Node *nodeInPath = searchPath.at(pathIndex).first;
				nodeInPath->bounds = calculateMBR(nodeInPath->children);

				--pathIndex;
			}
	
		};
		bool remove(Node* node, IdentifierType id, const BoundingBox &entryBounds, NodeSearchPath &searchPath)
		{
			//TODO add underflow treatment, remove single matching entry or ALL matching entries?
			for(std::size_t i = 0; i < node->children.size(); i++)
			{
				RTreeChild *child = node->children.at(i);
				if(entryBounds.intersects(child->bounds))
				{
					// Check all the entries against the one we want to remove if we're at a
					// leaf node.
					if(node->leafNode)
					{
						if(id == static_cast<Entry*>(child)->id)
						{
							node->children.erase(node->children.begin() + i);
							m_spareIDs.push_back(id);
							delete child;

							underflowTreatment(searchPath.size()-1, searchPath);

							return true;
						}
					}

					// Recurse into other intersecting nodes
					else
					{
						searchPath.push_back(std::make_pair(static_cast<Node*>(child), node));
						return remove(static_cast<Node*>(child), id, entryBounds, searchPath);
					}
				}
			}

			return false;
		};

		TType* get(Node* node, IdentifierType id, const BoundingBox &queryBounds)
		{
			for(std::size_t i = 0; i < node->children.size(); i++)
			{
				RTreeChild *child = node->children.at(i);
				if(queryBounds.intersects(child->bounds))
				{
					// Check all the entries against the one we want to retrieve if we're at a
					// leaf node.
					if(node->leafNode)
					{
						Entry *ent = static_cast<Entry*>(child);
						if(id == ent->id)
							return &ent->data;
					}

					// Recurse into other intersecting nodes
					else
						return get(static_cast<Node*>(child), id, queryBounds);
				}
			}

			return nullptr;
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
				// Recurse into children and get their bounds
				if(!nodeToQuery.leafNode)
				{
					if(nodeToQuery.level == 1)
						boundList.push_back(nodeToQuery.children.at(i)->bounds);


					getAllBounds(*nodeToQuery.child(i), boundList);
				}
				else
					boundList.push_back(nodeToQuery.children.at(i)->bounds);
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


	public:

		IdentifierType insert(TType newEntry, const BoundingBox &entryBounds)
		{
			OverflowMap overflowMap;

			Entry *ent = new Entry(newEntry);
			ent->bounds = entryBounds;

			// Check for reuseable ID's
			if(!m_spareIDs.empty())
			{
				ent->id = m_spareIDs.back();
				m_spareIDs.pop_back();
			}
			else
				ent->id = m_idIndex++;
			
			insert(ent, 0, overflowMap);
			return ent->id;
		};

		bool remove(IdentifierType id, const BoundingBox &entryBounds)
		{
			NodeSearchPath searchPath;
			searchPath.push_back(std::make_pair(m_rootNode, nullptr));
			return remove(m_rootNode, id, entryBounds, searchPath);
		};

		// Queries for an entry of id 'id' within the specified query bounds
		TType* get(IdentifierType id, const BoundingBox &queryBounds)
		{
			return get(m_rootNode, id, queryBounds);
		};

		// Queries a specific region of the R-tree, returning all entries within the query bounds
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

		// Prints a the R-tree hierarchy to the console
		void printTree()
		{
			JL_INFO_LOG("ROOT (Level %i)", m_rootNode->level);
			printTree(*m_rootNode, 1);
		};



		RTree()
			:
			m_rootNode(),
			m_idIndex(0)
		{
			m_rootNode = new Node();

			// Root node will always be the leaf node at startup, as the tree is empty
			m_rootNode->leafNode = true;

			static_assert(minNodes >= 2, "R-trees cannot have a minimum node values less than 2");
			static_assert(minNodes <= maxNodes/2, "R-trees cannot have a minimum node value above half the maximum value");
		};
		~RTree()
		{
			delete m_rootNode;
		};

	};
};

#endif