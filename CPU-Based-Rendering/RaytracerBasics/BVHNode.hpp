#ifndef BVHNODE_HPP
#define BVHNODE_HPP

#include "AABB.hpp"
#include "Primitive.hpp"

class BVHNode {
public:
	std::unique_ptr<BVHNode> left;
	std::unique_ptr<BVHNode> right;

	//the objets are not copied here, as we access them in the Scene class itself, so no need of storing even the pointer
	size_t startIdx, stopIdx;  // the range of objecs in the scene present in the node(leaf can support multiple primitives for SAH)
	AABB box;

	BVHNode();

};

#endif // !BVHNODE_HPP