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

