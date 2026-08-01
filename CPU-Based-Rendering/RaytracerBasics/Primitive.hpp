#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "UtilityClasses.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"
#include "AABB.hpp"

class Primitive {
public:
	virtual bool intersect(const Ray& ray, HitRecord& record) = 0;
	virtual AABB getBoundingBox() = 0;
	virtual Point3 getCenter() = 0;
	virtual ~Primitive() = default;

};


class Sphere : public Primitive {
	Point3 center;
	double radius;
	Material* material;

public:
	Sphere(const Vector3& cen, const double rad, Material* material);

	bool intersect(const Ray& ray, HitRecord& record);

	Point3 getCenter();

	AABB getBoundingBox();

};

#endif // !PRIMITIVE_HPP
