#include<iostream>
#include<memory>
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Primitive.hpp"
#include "Scene.hpp"
#include"ImageClasses.hpp"
#include"UtilityClasses.hpp"


int main() {
	Point3 camCenter(0, 0, 0);
	int imgHeight = 200;
	int imgWidth = 400;
	double viewportHeight = 2;
	double viewportWidth = 4;
	std::string opImgFile = "output.ppm";

	Camera camera(camCenter, 1, viewportHeight, viewportWidth, imgWidth, imgHeight);
	Image image(imgHeight, imgWidth);
	PPMImageWriter writer;


	Point3 sphereCen(0, 0, -2);
	double sphereRad = 0.50;

	std::unique_ptr<Primitive> obj = std::make_unique<Sphere>(sphereCen, sphereRad);
	Scene scene;
	scene.addPrimitiveObject(std::move(obj));


	Renderer renderer(camera, scene, image);
	renderer.renderImage();

	writer.writeData(opImgFile, image);


	return 0;
}

