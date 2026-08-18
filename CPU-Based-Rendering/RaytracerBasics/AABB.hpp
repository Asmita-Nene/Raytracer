#ifndef AABB_HPP
#define AABB_HPP

#include "UtilityClasses.hpp"

class AABB {
public:
	Point3 minPt;
	Point3 maxPt;

	AABB(const Point3& min, const Point3& max);
	AABB();
	bool isIntersect(const Ray& ray) const ;
	AABB combineBoxes(const AABB& box2) const ;
};

#endif // !AABB_HPP
