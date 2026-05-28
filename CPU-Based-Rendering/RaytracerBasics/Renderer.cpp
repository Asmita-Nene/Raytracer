#include "Camera.hpp"
#include "ImageClasses.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"

#include"UtilityClasses.hpp"
#include "HitRecord.hpp"


Renderer::Renderer(Camera& cam, Scene& scene, Image& img): camera(cam), scene(scene), image(img){}

void Renderer::renderImage() {
	int imgHeight = camera.getImgHeight();
	int imgWidth = camera.getImgWidth();
	HitRecord record;

	Vector3 normal;
	

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			HitRecord record;
			Color color(50, 50, 50);
			Ray ray = camera.generateRay(j, i);		
			if (scene.intersect(ray, record)) {

				//this part of for temporary color calculations
				normal = record.Normal;
				color.x = ((normal.x + 1) / 2) * 255;
				color.y = ((normal.y + 1) / 2) * 255;
				color.z = ((normal.z + 1) / 2) * 255;
			}
			image.setPixel(j, i, color);

		}
	}
}