#ifndef CAMERA_HPP
#define CAMERA_HPP
#include"UtilityClasses.hpp"


class Camera {
	int imageHeight;
	int imageWidth;
	double focalLength;
	Point3 center;
	double viewportHeight;
	double viewportWidth;
	Vector3 horizDimen;
	Vector3 vertDimen;
	Point3  lowerLeftCorner;



public:
	Camera(const Point3& center, double focalLength, double viewportHeight, double viewportWidth, int width, int height);

	int getImgHeight()const;
	int getImgWidth() const ;

	Ray generateRay(int x, int y) const ;
};

#endif // !CAMERA_HPP
