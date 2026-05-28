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

public:
	Renderer(Camera& cam, Scene& scene, Image& img);
	
	void renderImage();
};

#endif // !RENDERER_HPP
