#ifndef SCENE_HPP
#define SCENE_HPP
#include"UtilityClasses.hpp"
#include"HitRecord.hpp"
#include"Primitive.hpp"

#include<vector>
#include<memory>


class Scene {
	std::vector<std::unique_ptr<Primitive>> objects;
	//TODO Save the lights like this as well

public:
	Scene();

	void addPrimitiveObject(std::unique_ptr<Primitive> obj);

	bool intersect(const Ray& ray, HitRecord& record);
};


#endif // !SCENE_HPP