#include "Camera.hpp"
#include "ImageClasses.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"

#include"UtilityClasses.hpp"
#include "HitRecord.hpp"

Renderer::Renderer(Camera& cam, Scene& scene, Image& img, int samplesPerPixel, int depthReflections):
	camera(cam), scene(scene), image(img), samplesPerPixel(samplesPerPixel), depthReflections(depthReflections) {}




Color Renderer::getColor(const Ray& ray, int depth) {
	//get where the ray intersects in the given scene
	if (depth == 0) {
		return Color(0.00, 0.00, 0.00);	
	}

	HitRecord record;
	if (!scene.intersect(ray, record)) {
		// doesn't intersect anything so simple sky gradient, white to blue as background
		//TODO Change this to support different backgrounds

		double t = 0.5 * (ray.direction.getNormalized().y + 1.0);
		return  (Color(1.0, 1.0, 1.0) * Color(0.5, 0.7, 1.0) * (1.0 - t)) * t;
	}

	// get the new ray from the material
	Ray reflRay = record.material->getRay(record.intersectionPoint, record.normal);

	//then get the base color and recursively call the function
	return record.material->getAlbedo() * getColor(reflRay, depth - 1);
		
}


void Renderer::renderImage() {
	int imgHeight = camera.getImgHeight();
	int imgWidth = camera.getImgWidth();
	HitRecord record;

	Vector3 normal;
	double offsetX, offsetY;
	

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			//these above neted loops for accessing each pixel of the image 
			
			HitRecord record;
			Color color(0, 0, 0);

			
			for (int k = 0; k < samplesPerPixel; k++) {
				//this is the loop for anti-aliasing 


				offsetX = randomDouble(0.00, 1.00);
				offsetY = randomDouble(0.00, 1.00);
				Ray ray = camera.generateRay(j + offsetX, i + offsetY);		

				color = color + getColor(ray, depthReflections);		//for the anti-aliasing
				
			}

			color = color / samplesPerPixel;

			image.setPixel(j, i, color);

		}
	}
}