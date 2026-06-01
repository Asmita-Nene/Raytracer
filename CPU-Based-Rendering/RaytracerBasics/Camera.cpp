#include<iostream>
#include"UtilityClasses.hpp"
#include"Camera.hpp"


Camera::Camera(const Point3& center, double focalLength, double viewportHeight, double viewportWidth, int width, int height) : 
	center(center), focalLength(focalLength), viewportHeight(viewportHeight), viewportWidth(viewportWidth), imageWidth(width), imageHeight(height)
{
	//TODO these will also chenge in future, as right now we are assuming camera at 0 0, 0 facing the negative z axis
	horizDimen = Vector3(viewportWidth, 0, 0);
	vertDimen = Vector3(0, viewportHeight, 0);
	lowerLeftCorner = Point3(-viewportWidth / 2, -viewportHeight / 2, -focalLength);	//present in negative z axis

}

int Camera::getImgHeight() const {
	return imageHeight;
}
int Camera::getImgWidth() const {
	return imageWidth;
}

Ray Camera::generateRay(double x, double y) const {
	//x = related to width, horizontal and y = related to height, vertical
	
	//add 0.5 to get the pixel center
	double u = x / imageWidth;
	double v = y / imageHeight;

	Ray ray(center, ((lowerLeftCorner + (horizDimen * u) + (vertDimen * v)) - center));
	return ray;
}