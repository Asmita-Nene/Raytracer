#include <iostream>
#include "Material.hpp"

#include"UtilityClasses.hpp"
#include "HitRecord.hpp"

//--Diffuse Implementation--//
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



//--Metal Implementation--//
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


//--Dielectric Implementation--//

Dielectric::Dielectric(double refract_idx) : refract_idx(refract_idx) {}

Color Dielectric::getAlbedo() {
	return Color(1.00, 1.00, 1.00);
}

Ray Dielectric::getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal) {
	//We assume the refractive index of the environment as 1.00
	double env_refract_idx = 1.00;
	double epsilon = 0.001;			//this is to avoid self intersection of the ray, and avoid indefinite TIR inside the sphere
	double idx_ratio = env_refract_idx / refract_idx;
	bool inside = false;

	if (incidentRay.direction.dot(normal) > 0) {
		//flip the normal direction
		inside = true;
		normal.x = -normal.x;	
		normal.y = -normal.y;
		normal.z = -normal.z;
		idx_ratio = refract_idx / env_refract_idx;
	}


	//Getting the incident ray angle - using dot product
	double cos_theta = -incidentRay.direction.dot(normal);		//as both are unit vectors, no need to divide by magnitude

	double sin_theta = sqrt(1 - std::pow(cos_theta, 2));



	if (sin_theta * (idx_ratio) > 1.00) {
		//this is the Total Interanal Reflection condition, return the reflected ray
		Vector3 reflected = incidentRay.direction.getNormalized() - (normal * (incidentRay.direction.getNormalized().dot(normal)) * 2);
		return Ray(intersectionPt + (reflected * epsilon), reflected.getNormalized());
	}

	//Now get the R_theta based on schlick's approximation
	//check for the inner ray condition
	double Rs;
	if (inside) {
		//intersects from inside, hence goes from dielectric - to - env
		Rs = std::pow(((refract_idx - env_refract_idx) / (env_refract_idx + refract_idx)), 2);
	}
	else {
		Rs = std::pow(((env_refract_idx - refract_idx) / (env_refract_idx + refract_idx)), 2);
	}

	double R_theta = Rs + ((1 - Rs) * std::pow((1 - cos_theta), 5));

	//Get a random sample and compare
	double rand_sample = randomDouble(0.00, 1.00);
	if (R_theta > rand_sample) {
		//return the reflected ray
		Vector3 reflected = incidentRay.direction.getNormalized() - (normal * (incidentRay.direction.getNormalized().dot(normal)) * 2);
		return Ray(intersectionPt + (reflected * epsilon), reflected.getNormalized());
	}
	else {
		//return the refraced ray as per snell's law
		Vector3 refracted_tang = (incidentRay.direction + (normal * cos_theta)) * (idx_ratio);
		Vector3 refracted_norm = normal * sqrt(1 - std::pow(refracted_tang.getMagnitude(), 2)) * (-1);
		Vector3 refracted_dir = refracted_tang + refracted_norm;
		return Ray(intersectionPt + (refracted_dir * epsilon), refracted_dir);
	}

}