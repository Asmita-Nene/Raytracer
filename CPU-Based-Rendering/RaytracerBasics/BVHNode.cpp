#include "BVHNode.hpp"

BVHNode::BVHNode() {
	this->left = nullptr;
	this->right = nullptr;
	this->leafObj = nullptr;
}