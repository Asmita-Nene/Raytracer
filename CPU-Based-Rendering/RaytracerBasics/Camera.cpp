#include<iostream>
#include <numbers>
#include"UtilityClasses.hpp"
#include"Camera.hpp"

//TODO fix the bug where scene is not rendered correctly for y > 1 (y = camCenter co-ord)

Camera::Camera(int imageHeight, int imageWidth, const Point3& position, const Point3& target, const Vector3& worldUp, double vertFOV, double focalLength, double aspect) :
	imageHeight(imageHeight), imageWidth(imageWidth), position(position), target(target), worldUp(worldUp), vertFOV(vertFOV), focalLength(focalLength), aspectRatio(aspect)
{
	forward = (target - position).getNormalized();
	right = worldUp.cross(forward).getNormalized();
	up = forward.cross(right).getNormalized();

	viewportCenter = position + (forward * focalLength);
	double vertFOV_rad = degreeToRadian(vertFOV);
	viewportHeight = 2 * focalLength * tan(vertFOV_rad / 2);
	viewportWidth = aspectRatio * viewportHeight;

	horizontal = right * viewportWidth;
	vertical = up * viewportHeight;

	lowerLeftCorner = viewportCenter - horizontal * 0.5 - vertical * 0.5;
}

int Camera::getImgHeight() const {
	return imageHeight;
}
int Camera::getImgWidth() const {
	return imageWidth;
}

Ray Camera::generateRay(double x, double y) const {
	//x = related to width, horizontal and y = related to height, vertical

	double u = x / (imageWidth - 1.0);
	double v = 1.0 - (y / (imageHeight - 1.0));

	Ray ray(position, ((lowerLeftCorner + (horizontal * u) + (vertical * v)) - position));
	return ray;
}