#include"UtilityClasses.hpp"
#include"HitRecord.hpp"
#include"Primitive.hpp"
#include"Scene.hpp"

#include<vector>
#include<memory>
#include <random>
#include <iostream>

Scene::Scene() {
	isBVHInit = false;
}

void Scene::addPrimitiveObject(std::unique_ptr<Primitive> obj) {
	//implement the move semantic as unique pointer cannot be copied
	objects.push_back(std::move(obj));
}

std::unique_ptr<BVHNode> Scene::generateBVHTree(const size_t& startIdx, const size_t& stopIdx) {
	std::cout << "[" << startIdx << ", " << stopIdx << ")\n";

	std::unique_ptr<BVHNode> node = std::make_unique<BVHNode>();

	//base case
	if (stopIdx - startIdx == 1){
		node->box = objects[startIdx].get()->getBoundingBox();
		node->left = nullptr;
		node->right = nullptr;
		node->leafObj = objects[startIdx].get();
		return node;
	}


	int rand_ax = std::rand() % 3;		//to get random axis 0 = X, 1 = Y, 2 = Z
	switch (rand_ax) {
	case 0:
		//sort as per X axis
		std::sort(objects.begin() + startIdx, objects.begin() + stopIdx, 
			[](const std::unique_ptr<Primitive>& obj1, const std::unique_ptr<Primitive>& obj2) 
			{ return obj1.get()->getCenter().x < obj2.get()->getCenter().x; });

		break;
	case 1:
		//sort as per Y axis
		std::sort(objects.begin() + startIdx, objects.begin() + stopIdx,
			[](const std::unique_ptr<Primitive>& obj1, const std::unique_ptr<Primitive>& obj2)
			{ return obj1.get()->getCenter().y < obj2.get()->getCenter().y; });

		break;
	case 2:
		//sort as per axis
		std::sort(objects.begin() + startIdx, objects.begin() + stopIdx,
			[](const std::unique_ptr<Primitive>& obj1, const std::unique_ptr<Primitive>& obj2)
			{ return obj1.get()->getCenter().z < obj2.get()->getCenter().z; });
		break;
	}

	node->left = generateBVHTree(startIdx, startIdx + ((stopIdx - startIdx) / 2));
	node->right = generateBVHTree(((stopIdx - startIdx) / 2) + startIdx, stopIdx);
	node->box = node->left->box.combineBoxes(node->right->box);
	
	return node;
}


void Scene::initializeBVH() {
	this->BVHRoot = generateBVHTree(0, objects.size());
	isBVHInit = true;
}




bool Scene::traverseBVH(const std::unique_ptr<BVHNode>& node, const Ray& ray, HitRecord& record){
	if (node == nullptr){
		return false;
	}

	if (!node->box.isIntersect(ray)){
		return false;
	}

	// Leaf node
	if (node->leafObj != nullptr){

		HitRecord tempRecord;
		if (node->leafObj->intersect(ray, tempRecord)){

			if (tempRecord.t < record.t){

				record = tempRecord;
			}

			return true;
		}

		return false;
	}

	// Internal node
	bool hitLeft = traverseBVH(node->left, ray, record);
	bool hitRight = traverseBVH(node->right, ray, record);

	return hitLeft || hitRight;
}


bool Scene::intersect(const Ray& ray, HitRecord& record){
	record.t = std::numeric_limits<double>::infinity();

	return traverseBVH(BVHRoot, ray, record);
}