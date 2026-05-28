#include<iostream>
#include<cmath>
#include"UtilityClasses.hpp"
#include"HitRecord.hpp"
#include"Primitive.hpp"

Sphere::Sphere(const Vector3& cen, const double rad) : center(cen), radius(rad) {}

bool Sphere::intersect(const Ray& ray, HitRecord& record) {
	double a = ray.direction.dot(ray.direction);
	Vector3 sub = (ray.origin - center);
	double b = 2.00 * (ray.direction.dot(sub));
	double c = (sub.dot(sub)) - (radius * radius);
	double discriminant = (b * b) - (4 * a * c);
	
	if (discriminant >= 0) {
	//Intersection is present
		double smallestT = (-b - sqrt(discriminant)) / (2 * a);
		if (smallestT < 0) {
			smallestT = (-b + sqrt(discriminant)) / (2 * a);
			if (smallestT < 0) {
				return false;
			}
		}

		record.IntersectionPoint = ray.pointAt(smallestT);
		record.Normal = (record.IntersectionPoint - center).getNormalized();
		record.t = smallestT;
		//TODO the face logic will be implemented later, in some time
		return true;
	}
	else {
		return false;
	}

}

