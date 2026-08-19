#include<iostream>
#include<cmath>
#include"UtilityClasses.hpp"
#include"HitRecord.hpp"
#include"Primitive.hpp"
#include "Material.hpp"

Sphere::Sphere(const Vector3& cen, const double rad, Material* material) : center(cen), radius(rad), material(material) {}

bool Sphere::intersect(const Ray& ray, HitRecord& record) {
	double a = ray.direction.dot(ray.direction);
	Vector3 sub = (ray.origin - center);
	double b = 2.00 * (ray.direction.dot(sub));
	double c = (sub.dot(sub)) - (radius * radius);
	double discriminant = (b * b) - (4 * a * c);
	
	if (discriminant >= 0) {
	//Intersection is present
		double smallestT = (-b - sqrt(discriminant)) / (2 * a);
		if (smallestT < 1e-3) {
			smallestT = (-b + sqrt(discriminant)) / (2 * a);
			if (smallestT < 1e-3) {
				return false;
			}
		}

		record.intersectionPoint = ray.pointAt(smallestT);
		record.normal = (record.intersectionPoint - center).getNormalized();
		record.t = smallestT;
		record.material = material;
		//TODO the face logic will be implemented later
		return true;
	}

	else {
		return false;
	}
}

Point3 Sphere::getCenter() {
	return center;
}

AABB Sphere::getBoundingBox() {
	Point3 minPt = center - Point3(radius, radius, radius);
	Point3 maxPt = center + Point3(radius, radius, radius);
	return AABB(minPt, maxPt);
}



Triangle::Triangle(const Point3& V0, const Point3& V1, const Point3& V2, const Vector3& normal, Material* material, bool enableBC) :
	V0(V0), V1(V1), V2(V2), normal(normal), material(material), enableBackfaceCulling(enableBC) {

		this->center = (V0 + V1 + V2) / 3;		//centroid calc
}



bool Triangle::intersect(const Ray& ray, HitRecord& record) {
	Vector3 edge1 = V0 - V2;
	Vector3 edge2 = V1 - V2;
	Vector3 temp = ray.origin - V2;

	Vector3 dCrossE2 = ray.direction.cross(edge2);
	Vector3 tempCrossE1 = temp.cross(edge1);

	double denom = dCrossE2.dot(edge1);
	if (fabs(denom) <= 1e-8) {
		//the ray is almost parallel to triangle, no intersection
		return false;
	}
	double invDenom = 1 / denom;
	
	double u = dCrossE2.dot(temp) * invDenom;
	double v = tempCrossE1.dot(ray.direction) * invDenom;

	if (u + v <= 1 && u >= 0 && v >= 0) {
		//intersection exists, as barycentric co-ord are valid

		//check for backface cullng
		if (enableBackfaceCulling && (ray.direction.dot(this->normal)) > 0) {
			return false;
		}
		
		double t = tempCrossE1.dot(edge2) * invDenom;
		if (t <= 1e-3) {
			//reject very small t
			return false;
		}
		record.intersectionPoint = ray.pointAt(t);	//there will be no check for max or min t, as triangle is very thin

		record.normal = normal;
		record.t = t;
		record.material = material;
		return true;
	}

	return false;
}


Point3 Triangle::getCenter() {
	return center;
}

AABB Triangle::getBoundingBox() {
	AABB box;
	box.minPt = Point3(std::min({ V0.x, V1.x, V2.x }), std::min({ V0.y, V1.y, V2.y }), std::min({ V0.z, V1.z, V2.z }));
	box.maxPt = Point3(std::max({ V0.x, V1.x, V2.x }), std::max({ V0.y, V1.y, V2.y }), std::max({ V0.z, V1.z, V2.z }));
	return box;
}