#ifndef SCENE_HPP
#define SCENE_HPP
#include"UtilityClasses.hpp"
#include"HitRecord.hpp"
#include"Primitive.hpp"
#include "AABB.hpp"
#include "BVHNode.hpp"

#include<vector>
#include<memory>


class Scene {
	std::vector<std::unique_ptr<Primitive>> objects;
	//TODO Save the lights like this as well

	std::unique_ptr<BVHNode> BVHRoot;
	bool isBVHInit;


public:
	Scene();

	void addPrimitiveObject(std::unique_ptr<Primitive> obj);

	std::unique_ptr<BVHNode> generateBVHTree(const size_t& startIdx, const size_t& stopIdx);		//recursive function that generates the BVH tree uing random axes for splitting the boxes

	void initializeBVH();

	bool traverseBVH(const std::unique_ptr<BVHNode>& node, const Ray& ray, HitRecord& record);

	bool intersect(const Ray& ray, HitRecord& record);		//actually uses the BVH tree traversal to reduce the number of intersection calculations
};


#endif // !SCENE_HPP