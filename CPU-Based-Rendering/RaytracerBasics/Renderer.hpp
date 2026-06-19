#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Camera.hpp"
#include "ImageClasses.hpp"
#include "Scene.hpp"
#include"UtilityClasses.hpp"

class Renderer {
	Camera& camera;
	Image& image;
	Scene& scene;
	int samplesPerPixel;		//may need to change the datatype later
	int depthReflections;

public:
	Renderer(Camera& cam, Scene& scene, Image& img, int samplesPerPixel, int depthReflections);

	Color getColor(const Ray& ray, int depth);
	
	void renderImage();
};

#endif // !RENDERER_HPP
