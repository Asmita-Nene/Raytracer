#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "UtilityClasses.hpp"

//Stores the information related to one intersection of ray and object, basically to color one pixel, we need this data
class HitRecord {
public:
	double t;
	Point3 IntersectionPoint;
	Vector3 Normal;
	bool isFrontFace;	//look into this logic a bit, setFaceNormal method required?

	//will also contain the material info in future
};

#endif // !HITRECORD_HPP
