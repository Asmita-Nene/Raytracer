#include"UtilityClasses.hpp"
#include"HitRecord.hpp"
#include"Primitive.hpp"
#include"Scene.hpp"
#include "BVHNode.hpp"

#include<vector>
#include<memory>
#include <random>
#include<numbers>
#include <iostream>

Scene::Scene() {
	isBVHInit = false;
}

void Scene::addPrimitiveObject(std::unique_ptr<Primitive> obj) {
	//implement the move semantic as unique pointer cannot be copied
	objects.push_back(std::move(obj));
}

double Scene::getSurfaceArea(const AABB& box) {
	double w = box.maxPt.x - box.minPt.x;
	double h = box.maxPt.y - box.minPt.y;
	double d = box.maxPt.z - box.minPt.z;
	return (w * h + h * d + d * w);
}

double Scene::getSplitCost(const AABB& parentBox, const AABB& leftBox, const AABB& rightBox, const size_t& leftChildren, const size_t& rightChildren) {
	//get the surface areas
	double parentSA = getSurfaceArea(parentBox);
	double leftSA = getSurfaceArea(leftBox);
	double rightSA = getSurfaceArea(rightBox);

	double Pl = leftSA / parentSA;
	double Pr = rightSA / parentSA;

	return (1 + (Pl * leftChildren) + (Pr * rightChildren));

}


double Scene::evaluateAxis(const size_t& startIdx, const size_t& stopIdx, size_t& outSplitIdx) {
	size_t n = stopIdx - startIdx;

	std::vector<AABB> prefixBox(n);
	std::vector<AABB> suffixBox(n);

	prefixBox[0] = objects[startIdx]->getBoundingBox();
	for (size_t k = 1; k < n; k++) {
		prefixBox[k] = prefixBox[k - 1].combineBoxes(objects[startIdx + k]->getBoundingBox());
	}

	suffixBox[n - 1] = objects[stopIdx - 1]->getBoundingBox();
	for (size_t k = n - 1; k-- > 0; ) {
		suffixBox[k] = suffixBox[k + 1].combineBoxes(objects[startIdx + k]->getBoundingBox());
	}

	AABB parentBox = prefixBox[n - 1]; // union of everything in this range

	double bestCost = std::numeric_limits<double>::infinity();
	size_t bestLocalIdx = 0;

	for (size_t j = 0; j < n - 1; j++) {
		AABB leftBox = prefixBox[j];
		AABB rightBox = suffixBox[j + 1];
		size_t leftChildren = j + 1;
		size_t rightChildren = n - (j + 1);

		double cost = getSplitCost(parentBox, leftBox, rightBox, leftChildren, rightChildren);

		if (cost < bestCost) {
			bestCost = cost;
			bestLocalIdx = j;
		}
	}

	outSplitIdx = startIdx + bestLocalIdx + 1; // convert to absolute "first index of right group"
	return bestCost;
}



std::unique_ptr<BVHNode> Scene::generateBVHTree(const size_t& startIdx, const size_t& stopIdx) {
	std::unique_ptr<BVHNode> node = std::make_unique<BVHNode>();

	//base case
	if (stopIdx - startIdx == 1) {
		node->box = objects[startIdx]->getBoundingBox();
		node->left = nullptr;
		node->right = nullptr;
		node->startIdx = startIdx;
		node->stopIdx = stopIdx;
		return node;
	}

	double bestCost = std::numeric_limits<double>::infinity();
	int bestAxis = -1;
	size_t bestSplitIdx = 0;

	for (int axis = 0; axis < 3; axis++) {//iterate across all three axes and get the min cost 
		std::sort(objects.begin() + startIdx, objects.begin() + stopIdx,
			[axis](const std::unique_ptr<Primitive>& a, const std::unique_ptr<Primitive>& b) {
				return a->getCenter()[axis] < b->getCenter()[axis];
			});

		size_t candidateSplitIdx;
		double cost = evaluateAxis(startIdx, stopIdx, candidateSplitIdx);

		if (cost < bestCost) {
			bestCost = cost;
			bestAxis = axis;
			bestSplitIdx = candidateSplitIdx;
		}
	}

	// re-sort by the winning axis, since the last loop iteration left it sorted by z
	std::sort(objects.begin() + startIdx, objects.begin() + stopIdx,
		[bestAxis](const std::unique_ptr<Primitive>& a, const std::unique_ptr<Primitive>& b) {
			return a->getCenter()[bestAxis] < b->getCenter()[bestAxis];
		});

	double leafCost = static_cast<double>(stopIdx - startIdx);
	if (bestCost >= leafCost) {
		// multi-object leaf here (needs BVHNode to support a range, not just one leafObj)
		node->startIdx = startIdx;
		node->stopIdx = stopIdx;
		node->left = nullptr;
		node->right = nullptr;
		
		AABB box = objects[startIdx]->getBoundingBox();
		for (size_t k = startIdx + 1; k < stopIdx; k++) {
			box = box.combineBoxes(objects[k]->getBoundingBox());
		}
		node->box = box;
		return node;

	}

	node->left = generateBVHTree(startIdx, bestSplitIdx);
	node->right = generateBVHTree(bestSplitIdx, stopIdx);
	node->box = node->left->box.combineBoxes(node->right->box);

	return node;
}


void Scene::initializeBVH() {
	this->BVHRoot = generateBVHTree(0, objects.size());
	isBVHInit = true;
}


bool Scene::traverseBVH(const std::unique_ptr<BVHNode>& node, const Ray& ray, HitRecord& record) {
	if (node == nullptr) {
		return false;
	}
	if (!node->box.isIntersect(ray)) {
		return false;
	}

	// Leaf node — both children null means leaf now, not leafObj != nullptr
	if (node->left == nullptr && node->right == nullptr) {
		bool hitAny = false;
		for (size_t i = node->startIdx; i < node->stopIdx; i++) {
			HitRecord tempRecord;
			if (objects[i]->intersect(ray, tempRecord)) {
				if (tempRecord.t < record.t) {
					record = tempRecord;
					hitAny = true;
				}
			}
		}
		return hitAny;
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