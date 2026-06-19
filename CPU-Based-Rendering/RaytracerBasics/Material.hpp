#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "UtilityClasses.hpp"

class Material {
public:
	virtual Ray getRay(Point3 intersectionPt, Vector3 normal) = 0;
	virtual Color getAlbedo() = 0;

};


class Diffuse : public Material {
	//albedo is the fraction of incoming light that a surface reflects, so Color(1.0, 0.0, 0.0) is red, as it reflects all red, and absorbs the rest
	//albedo equivalent to base color for artists
	Color albedo;		

public:
	Diffuse(Color baseColor);

	Color getAlbedo();	
	Ray getRay(Point3 intersectionPt, Vector3 normal);

};

#endif // !MATERIAL_HPP
