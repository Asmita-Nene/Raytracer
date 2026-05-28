#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "UtilityClasses.hpp"
#include "HitRecord.hpp"


class Primitive {
public:
	virtual bool intersect(const Ray& ray, HitRecord& record) = 0;
	virtual ~Primitive() = default;

};


class Sphere : public Primitive {
	Point3 center;
	double radius;

public:
	
	Sphere(const Vector3& cen, const double rad);

	bool intersect(const Ray& ray, HitRecord& record);


};

#endif // !PRIMITIVE_HPP
