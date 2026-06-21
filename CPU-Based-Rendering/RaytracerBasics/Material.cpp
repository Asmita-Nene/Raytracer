#include <iostream>
#include "Material.hpp"

#include"UtilityClasses.hpp"
#include "HitRecord.hpp"


Diffuse::Diffuse(Color albedo) : albedo(albedo) {}

Color Diffuse::getAlbedo() {
	return albedo;
}


Ray Diffuse::getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal) {
	//the scattering logic goes here for the diffuse material - using Lambert's law
	while(1){

		//here we are getting the randomly scattered ray using rejection sampling 
		// the following check ensures that the resulting ray is not going inside the object, and is actually being scattered on the outside
		
		double x = randomDouble(-1.00, 1.00);
		double y = randomDouble(-1.00, 1.00);	
		double z = randomDouble(-1.00, 1.00);
		Vector3 ray(x, y, z);
		if (ray.getMagnitude() > 1) {
			continue;
		}
		if (ray.getMagnitude() < 1e-8) {// near zero check
			continue;
		}  
		
		return Ray(intersectionPt, (normal + ray.getNormalized()));
	}

}

Metal::Metal(Color albedo,  double rx, double ry, bool isAnisotropic) : 
	albedo(albedo),  rx(rx), ry(ry), isAnisotropic(isAnisotropic) {}

Color Metal::getAlbedo() {
	return albedo;
}

Ray Metal::getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal) {
	int tries = 0;

	Vector3 reflected = incidentRay.direction.getNormalized() - (normal * (incidentRay.direction.getNormalized().dot(normal)) * 2);
	Vector3 alteredRay;

	if(!isAnisotropic){
		while (1) {
			tries++;
			if (tries > 1000) {
				std::cout << "Exceeded tries, returning...\n";
				return Ray(intersectionPt, reflected.getNormalized());
			}
			double x = randomDouble(-1.00, 1.00);
			double y = randomDouble(-1.00, 1.00);
			double z = randomDouble(-1.00, 1.00);
			Vector3 ray(x, y, z);
			if (ray.getMagnitude() > 1) {
				continue;
			}

			alteredRay = reflected + (ray * rx);

			if (alteredRay.dot(normal) < 0) {
				continue;
			}
			return Ray(intersectionPt, alteredRay.getNormalized());
		}
			
	}
	else {

		Vector3 up;
		if (std::fabs(normal.y) > 0.8) {
			up = Vector3(1, 0, 0);
		}
		else {
			up = Vector3(0, 1, 0);
		}

		Vector3 tangent = up.cross(normal);
		Vector3 bitangent = normal.cross(tangent);

		while(1){

			double x = randomDouble(-1.00, 1.00);
			double y = randomDouble(-1.00, 1.00);
			double z = randomDouble(-1.00, 1.00);

			alteredRay = reflected + ((tangent * x * rx) + (bitangent * y * ry) + (normal * 0.01 * z));

			if (alteredRay.dot(normal) < 0) {
				continue;
			}


			return Ray(intersectionPt, alteredRay.getNormalized());
		}
		
	}

}