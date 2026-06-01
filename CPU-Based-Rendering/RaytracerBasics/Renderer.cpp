#include "Camera.hpp"
#include "ImageClasses.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"

#include"UtilityClasses.hpp"
#include "HitRecord.hpp"

#include <random>

double randomDouble()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<double> dist(0.0, 1.0);

	return dist(gen);
}


Renderer::Renderer(Camera& cam, Scene& scene, Image& img, int samplesPerPixel): camera(cam), scene(scene), image(img), samplesPerPixel(samplesPerPixel){}

void Renderer::renderImage() {
	int imgHeight = camera.getImgHeight();
	int imgWidth = camera.getImgWidth();
	HitRecord record;

	Vector3 normal;
	double offsetX, offsetY;
	

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			
			HitRecord record;
			Color color(0, 0, 0);

			//add loop for anti-aliasing here
			for (int k = 0; k < samplesPerPixel; k++) {
				Color tempColor(0, 0, 0);
				offsetX = randomDouble();
				offsetY = randomDouble();

				Ray ray = camera.generateRay(j + offsetX, i + offsetY);		
				if (scene.intersect(ray, record)) {

					//this part of for temporary color calculations
					normal = record.Normal;
					tempColor.x = ((normal.x + 1) / 2) * 255;
					tempColor.y = ((normal.y + 1) / 2) * 255;
					tempColor.z = ((normal.z + 1) / 2) * 255;
				}
				color = color +  tempColor;
				
			}

			color = color / samplesPerPixel;
			image.setPixel(j, i, color);

		}
	}
}