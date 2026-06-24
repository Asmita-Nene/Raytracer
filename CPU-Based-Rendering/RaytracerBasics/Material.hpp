#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "UtilityClasses.hpp"

class Material {
public:
	virtual Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal) = 0;
	virtual Color getAlbedo() = 0;
};


class Diffuse : public Material {
	//albedo is the fraction of incoming light that a surface reflects, so Color(1.0, 0.0, 0.0) is red, as it reflects all red, and absorbs the rest
	//albedo equivalent to base color for artists
	Color albedo;		

public:
	Diffuse(Color albedo);
	Color getAlbedo();	
	Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal);
};



class Metal : public Material {
	Color albedo;
	double rx;
	double ry;
	bool isAnisotropic;

public:
	Metal(Color albedo, double rx, double ry, bool isAnisotropic = false);
	Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal);
	Color getAlbedo();
};


class Dielectric :public Material {
	//albedo is always (1, 1, 1) here
	double refract_idx;
public:
	Dielectric(double refract_idx);
	Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal);
	Color getAlbedo();
};


#endif // !MATERIAL_HPP
