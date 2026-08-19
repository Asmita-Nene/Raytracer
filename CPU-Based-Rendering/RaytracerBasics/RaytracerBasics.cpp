#include<iostream>
#include<memory>
#include<random>
#include <chrono>
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Primitive.hpp"
#include "Scene.hpp"
#include"ImageClasses.hpp"
#include"UtilityClasses.hpp"
#include "Material.hpp"
#include"ObjFileHandle.hpp"



int main() {
	//--------------------------------Camera Setup parameters and object creation---------------------------------------------------------------------------//

    Point3 camCenter(0, 0, 0);
    Point3 camTarget(0, 0, -3);

	int imgHeight = 200;
	int imgWidth = 400;
	Vector3 worldUp(0, 1, 0);		//Y axis(as per convention in majority Graphics engines)
	double aspectRatio = static_cast<double>(imgWidth) / imgHeight;
	double vertFOV = 45.00;
	double focalLength = 1.00;
	Camera camera(imgHeight, imgWidth, camCenter, camTarget, worldUp, vertFOV, focalLength, aspectRatio);


	//------------------------------------------------Output file setup--------------------------------------------------------//
	std::string opImgFile = "SuzanneObj.ppm";	
	Image image(imgHeight, imgWidth);
	std::unique_ptr<ImageWriter> writer = std::make_unique<PPMImageWriter>();

	//---------------------------------scene and renderer object setup------------------------------------------------------//
	Scene scene;
	int samplesPerPixel = 100;
	int depthReflections = 50;
	int colorDepth = 8;					//8 bits to represent color depth
	Renderer renderer(camera, scene, image, samplesPerPixel, depthReflections);

	//-----------------------------------------------------------------Scene Setup----------------------------------------------//

	std::vector<std::unique_ptr<Material>> materials;
	ObjFileHandle handle(".\\objFiles\\blender_suzanne.obj");
	if (!handle.constructScene(scene, materials)) {}
	
	//--------------------------------------------------scene rendering and writing to file---------------------------------------------------//

    //BVH Tree construction shoudl be seperate from the render pipeline
    scene.initializeBVH();
    std::cout << "\n\n Tree Built\n\n";
    renderer.renderImage();
    std::cout << "after render\n";
    writer->writeData(opImgFile, image, colorDepth, 2.2f);

	return 0;
}

