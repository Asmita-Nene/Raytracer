#include"UtilityClasses.hpp"
#include"HitRecord.hpp"
#include"Primitive.hpp"
#include"Scene.hpp"

#include<vector>
#include<memory>

Scene::Scene() {}

void Scene::addPrimitiveObject(std::unique_ptr<Primitive> obj) {
	//implement the move semantic as unique pointer cannot be copied
	objects.push_back(std::move(obj));
}

bool Scene::intersect(const Ray& ray, HitRecord& record) {
	//Loop through all objects to get which object it intersects and the value of t

	HitRecord tempRecord;
	record.t = std::numeric_limits<double>::infinity();		//init to infinity, not taking -1 here, as it geometrically signifies something, and will be better this way in future implementations


	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i].get()->intersect(ray, tempRecord)) {
			//the ray intersects the object given

			if (record.t > tempRecord.t) {
				record = tempRecord;	//for storing the smallest possible t
			}
		}
	}

	if (record.t == std::numeric_limits<double>::infinity()) {
		//no intersections are present
		return false;
	}
	else {
		return true;
	}


}