#ifndef BVHNODE_HPP
#define BVHNODE_HPP

#include "AABB.hpp"
#include "Primitive.hpp"

class BVHNode {
public:
	std::unique_ptr<BVHNode> left;
	std::unique_ptr<BVHNode> right;
	Primitive* leafObj;		//the actual object or primitive
	AABB box;

	BVHNode();

};

#endif // !BVHNODE_HPP