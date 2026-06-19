#include <iostream>
#include "Material.hpp"

#include"UtilityClasses.hpp"
#include "HitRecord.hpp"


Diffuse::Diffuse(Color albedo) : albedo(albedo) {}

Color Diffuse::getAlbedo() {
	return albedo;
}


Ray Diffuse::getRay(Point3 intersectionPt, Vector3 normal) {
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