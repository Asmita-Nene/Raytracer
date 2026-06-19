#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "UtilityClasses.hpp"
#include "Material.hpp"

//Stores the information related to one intersection of ray and object, basically to color one pixel, we need this data
class HitRecord {
public:
	double t;
	Point3 intersectionPoint;
	Vector3 normal;
	bool isFrontFace;

	Material* material = nullptr;
};

#endif // !HITRECORD_HPP
