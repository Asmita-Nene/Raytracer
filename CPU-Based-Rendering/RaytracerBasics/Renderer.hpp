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

public:
	Renderer(Camera& cam, Scene& scene, Image& img, int samplesPerPixel);
	
	void renderImage();
};

#endif // !RENDERER_HPP
