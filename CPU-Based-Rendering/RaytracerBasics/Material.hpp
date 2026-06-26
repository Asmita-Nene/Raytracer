#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "UtilityClasses.hpp"

class Material {
public:
	virtual Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal) = 0;
	virtual Color getAttenuation(double distance, bool isInnerFace) = 0;
};


class Diffuse : public Material {
	//albedo is the fraction of incoming light that a surface reflects, so Color(1.0, 0.0, 0.0) is red, as it reflects all red, and absorbs the rest
	//albedo equivalent to base color for artists
	Color albedo;		

public:
	Diffuse(Color albedo);
	Color getAttenuation(double distance, bool isInnerFace) override;
	Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal)override;
};



class Metal : public Material {
	Color albedo;
	double rx;
	double ry;
	bool isAnisotropic;

public:
	Metal(Color albedo, double rx, double ry, bool isAnisotropic = false);
	Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal)override;
	Color getAttenuation(double distance, bool isInnerFace)override;
};


class Dielectric :public Material {
	//albedo is always (1, 1, 1) here
	Color albedo;
	double refract_idx;
	//the tint shows the color of the light absorbed by the material.So if tint = (2, 0.1, 0.1) red is absorbed, and color of glass obtained is close to cyan
	Color tint;			
public:
	Dielectric(double refract_idx, Color tint);
	Ray getRay(const Ray& incidentRay, Point3 intersectionPt, Vector3 normal)override;
	Color getAttenuation(double distance, bool isInnerFace)override;
};


#endif // !MATERIAL_HPP
