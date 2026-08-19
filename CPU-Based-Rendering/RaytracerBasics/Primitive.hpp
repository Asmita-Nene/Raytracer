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


class Triangle : public Primitive {
	Point3 V0, V1, V2;
	Vector3 normal;
	Point3 center;		//actually the centroid, but keepin it center for simplicity
	Material* material;
	bool enableBackfaceCulling;		//when enabled, the triangle whose normal faces away from the camera is not rendered(optimization for opaque), should be disabled for glass and transparent things

public:

	Triangle(const Point3& V0, const Point3& V1, const Point3& V2, const Vector3& normal, Material* material, bool enableBC = true);	//backface culing enabled by default

	bool intersect(const Ray& ray, HitRecord& record);

	Point3 getCenter();

	AABB getBoundingBox();

};

#endif // !PRIMITIVE_HPP
