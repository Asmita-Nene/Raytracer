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


int main() {
	//--------------------------------Camera Setup parameters and object creation---------------------------------------------------------------------------//

    Point3 camCenter(0, 2, 5);
    Point3 camTarget(0, 0, -3);

	int imgHeight = 200;
	int imgWidth = 400;
	Vector3 worldUp(0, 1, 0);		//Y axis(as per convention in majority Graphics engines)
	double aspectRatio = static_cast<double>(imgWidth) / imgHeight;
	double vertFOV = 45.00;
	double focalLength = 1.00;
	Camera camera(imgHeight, imgWidth, camCenter, camTarget, worldUp, vertFOV, focalLength, aspectRatio);


	//------------------------------------------------Output file setup--------------------------------------------------------//
	std::string opImgFile = "complexSceneWithBVHSAH.ppm";	
	Image image(imgHeight, imgWidth);
	std::unique_ptr<ImageWriter> writer = std::make_unique<PPMImageWriter>();

	//---------------------------------scene and renderer object setup------------------------------------------------------//
	Scene scene;
	int samplesPerPixel = 100;
	int depthReflections = 50;
	int colorDepth = 8;					//8 bits to represent color depth
	Renderer renderer(camera, scene, image, samplesPerPixel, depthReflections);

	//-----------------------------------------------------------------Scene Setup----------------------------------------------//

    // Stores all materials so that Sphere can store the Material*
    std::vector<std::unique_ptr<Material>> materials;

    // Ground

    materials.push_back(
        std::make_unique<Diffuse>(Color(0.1, 0.1, 0.1))
    );

    scene.addPrimitiveObject(
        std::make_unique<Sphere>(
            Point3(0, -1000.5, 0),
            1000,
            materials.back().get()
        )
    );


    // Main Spheres
   
    materials.push_back(
        std::make_unique<Diffuse>(Color(0.99, 0.2, 0.2))
    );

    scene.addPrimitiveObject(
        std::make_unique<Sphere>(
            Point3(0, 0.7, -2.5),
            1.0,
            materials.back().get()
        )
    );

    
    materials.push_back(
        std::make_unique<Dielectric>(
            1.5,
            Color(1, 1, 1)
        )
    );

    scene.addPrimitiveObject(
        std::make_unique<Sphere>(
            Point3(-2.5, 0.5, -3.5),
            0.8,
            materials.back().get()
        )
    );


    materials.push_back(
        std::make_unique<Metal>(
            Color(0.4, 0.5, 0.5),
            0.5,
            0.5
        )
    );

    scene.addPrimitiveObject(
        std::make_unique<Sphere>(
            Point3(2.5, 0.5, -3.5),
            0.8,
            materials.back().get()
        )
    );

// Sphere 1
    materials.push_back(std::make_unique<Diffuse>(Color(0.9, 0.2, 0.2)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(-4.5, 0.25, -1.5), 0.25, materials.back().get()));;

    // Sphere 3
    materials.push_back(std::make_unique<Diffuse>(Color(0.2, 0.2, 0.9)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(-1.5, 0.25, -6.0), 0.25, materials.back().get()));

    // Sphere 4
    materials.push_back(std::make_unique<Diffuse>(Color(0.9, 0.9, 0.2)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(1.0, 0.25, -5.5), 0.25, materials.back().get()));

    // Sphere 5
    materials.push_back(std::make_unique<Diffuse>(Color(0.9, 0.4, 0.1)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(3.5, 0.25, -6.5), 0.25, materials.back().get()));

    // Sphere 6
    materials.push_back(std::make_unique<Diffuse>(Color(0.2, 0.8, 0.8)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(4.5, 0.25, -2.0), 0.25, materials.back().get()));

    // Sphere 7
    materials.push_back(std::make_unique<Diffuse>(Color(0.8, 0.2, 0.8)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(-1.0, 0.25, -1.5), 0.25, materials.back().get()));

    // Sphere 8
    materials.push_back(std::make_unique<Diffuse>(Color(0.5, 0.8, 0.3)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(2.0, 0.25, -1.0), 0.25, materials.back().get()));

    // Sphere 9
    materials.push_back(std::make_unique<Diffuse>(Color(0.3, 0.5, 0.9)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(-5.0, 0.25, -7.0), 0.25, materials.back().get()));

    // Sphere 10
    materials.push_back(std::make_unique<Diffuse>(Color(0.8, 0.8, 0.8)));
    scene.addPrimitiveObject(std::make_unique<Sphere>(Point3(5.0, 0.25, -7.5), 0.25, materials.back().get()));


	//--------------------------------------------------scene rendering and writing to file---------------------------------------------------//

    //BVH Tree construction shoudl be seperate from the render pipeline
    scene.initializeBVH();
    //renderer.renderImage();
    //writer->writeData(opImgFile, image, colorDepth, 2.2f);

//for checking the average time to render the scene with 14 objects(to be kept same for time comparisons)
   double time_agg = 0;
    for (int i = 0; i < 10; i++) {
        auto start = std::chrono::steady_clock::now();
        renderer.renderImage();
        auto end = std::chrono::steady_clock::now();
        auto elapsed = end - start;
        time_agg += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    }
    std::cout << "Average time to render the scene with SAH in BVH : ";
    std::cout << time_agg/10.0 << " ms\n";

    writer->writeData(opImgFile, image, colorDepth, 2.2f);

	return 0;
}

