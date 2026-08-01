#include "AABB.hpp"
#include <limits>

AABB::AABB(const Point3& min, const Point3& max) : minPt(min), maxPt(max){}

AABB::AABB() {
	minPt = Point3(0, 0, 0);
	maxPt = Point3(0, 0, 0);
}

bool AABB::isIntersect(const Ray& ray) const {
	double tx1, tx2, ty1, ty2, tz1, tz2;

	tx1 = (minPt.x - ray.origin.x) / ray.direction.x;
	tx2 = (maxPt.x - ray.origin.x) / ray.direction.x;
	//in case the ray is in reverse direction, t1 > t2 
	if (tx2 < tx1) {
		std::swap(tx1, tx2);
	}

	ty1 = (minPt.y - ray.origin.y) / ray.direction.y;
	ty2 = (maxPt.y - ray.origin.y) / ray.direction.y;
	if (ty2 < ty1) {
		std::swap(ty1, ty2);
	}

	tz1 = (minPt.z - ray.origin.z) / ray.direction.z;
	tz2 = (maxPt.z - ray.origin.z) / ray.direction.z;
	if (tz2 < tz1) {
		std::swap(tz1, tz2);
	}


	if (ray.direction.x == 0 || ray.direction.y == 0 || ray.direction.z == 0) {
		if (ray.direction.x == 0) {
			if (ray.origin.x >= minPt.x && ray.origin.x <= maxPt.x) {
				tx1 = -std::numeric_limits<double>::infinity();
				tx2 = std::numeric_limits<double>::infinity();
			}
			else {
				return false;
			}
		}
		if (ray.direction.y == 0) {
			if (ray.origin.y >= minPt.y && ray.origin.y <= maxPt.y) {
				ty1 = -std::numeric_limits<double>::infinity();
				ty2 = std::numeric_limits<double>::infinity();
			}
			else {
				return false;
			}
		}
		if (ray.direction.z == 0) {
			if (ray.origin.z >= minPt.z && ray.origin.z <= maxPt.z) {
				tz1 = -std::numeric_limits<double>::infinity();
				tz2 = std::numeric_limits<double>::infinity();
			}
			else {
				return false;
			}
		}
	}

	double start = std::max({ tx1, ty1, tz1 });
	double end = std::min({ tx2, ty2, tz2 });

	if (start <= end && end >= 0) {
		return true;
	}
	else {
		return false;
	}
	
	
}

AABB AABB::combineBoxes(const AABB& box2) const {
	Point3 newMin(
		std::min(minPt.x, box2.minPt.x),
		std::min(minPt.y, box2.minPt.y),
		std::min(minPt.z, box2.minPt.z)
	);

	Point3 newMax(
		std::max(maxPt.x, box2.maxPt.x),
		std::max(maxPt.y, box2.maxPt.y),
		std::max(maxPt.z, box2.maxPt.z)
	);

	return AABB(newMin, newMax);
}


