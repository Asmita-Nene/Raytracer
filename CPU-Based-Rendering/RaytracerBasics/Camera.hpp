#ifndef CAMERA_HPP
#define CAMERA_HPP
#include"UtilityClasses.hpp"


class Camera {
	//configurable by the user
	int imageHeight;
	int imageWidth;
	Point3 position;
	Point3 target;
	Vector3 worldUp;
	double vertFOV;
	double focalLength;
	double aspectRatio;

	//camera axes
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	//viewport params
	Point3 viewportCenter;
	double viewportHeight;
	double viewportWidth;
	Vector3 horizontal;
	Vector3 vertical;
	Point3  lowerLeftCorner;


public:
	Camera(int imageHeight, int imageWidth, const Point3& position, const Point3& target, const Vector3& worldUp, double vertFOV, double focalLength, double aspect);

	int getImgHeight()const;
	int getImgWidth() const ;

	Ray generateRay(double x, double y) const ;
};

#endif // !CAMERA_HPP
