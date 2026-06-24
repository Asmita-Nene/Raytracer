#include<iostream>
#include<memory>
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Primitive.hpp"
#include "Scene.hpp"
#include"ImageClasses.hpp"
#include"UtilityClasses.hpp"
#include "Material.hpp"


int main() {
	Point3 camCenter(0, 0, 0);
	int imgHeight = 200;
	int imgWidth = 400;
	double viewportHeight = 2;
	double viewportWidth = 4;
	std::string opImgFile = "dielectric_material_output.ppm";
	int samplesPerPixel = 100;
	int depthReflections = 50;
	int colorDepth = 8;				//goes from 0 to 255 for representing intensity level of a pixel

	Camera camera(camCenter, 1, viewportHeight, viewportWidth, imgWidth, imgHeight);
	Image image(imgHeight, imgWidth);
	std::unique_ptr<ImageWriter> writer = std::make_unique<PPMImageWriter>();

	//the diffuse sphere
	Point3 sphereCen(-1, 0, -2);
	double sphereRad = 0.75;
	Color base1(1.00, 0.00, 0.00);			//the colors will be represented in normalized form from 0-1, so it becomes easy to change the colorDepth as needed
	
	//the glass sphere
	Point3 sphereCen3(1, 0, -2);
	double sphereRad3 = 0.75;
	Color base3(0.80, 0.80, 0.80);

	Point3 biggerSphCen(0, 20.75, -2);
	double biggerSphRad = 20;
	Color base2(0.20, 0.80, 0.80);
	
	auto mat1 = std::make_unique<Diffuse>(base1);
	auto mat3 = std::make_unique<Dielectric>(1.30);
	auto mat2 = std::make_unique<Diffuse>(base2);


	std::unique_ptr<Primitive> obj = std::make_unique<Sphere>(sphereCen, sphereRad, mat1.get());
	std::unique_ptr<Primitive> obj3 = std::make_unique<Sphere>(sphereCen3, sphereRad3, mat3.get());
	std::unique_ptr<Primitive> obj2 = std::make_unique<Sphere>(biggerSphCen, biggerSphRad, mat2.get());
	Scene scene;
	scene.addPrimitiveObject(std::move(obj));
	scene.addPrimitiveObject(std::move(obj3));
	scene.addPrimitiveObject(std::move(obj2));


	Renderer renderer(camera, scene, image, samplesPerPixel, depthReflections);
	renderer.renderImage();

	writer->writeData(opImgFile, image, colorDepth, 2.2f);


	return 0;
}

