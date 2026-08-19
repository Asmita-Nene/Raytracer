#ifndef OBJFILEHANDLE
#define OBJFILEHANDLE

#include <fstream>
#include <vector>
#include<string>

#include "UtilityClasses.hpp"
#include "Scene.hpp"

class ObjFileHandle {
	std::string filename;
	std::vector<Point3> verts;
	std::vector<Vector3> norms;

public:
	ObjFileHandle(const std::string& filename);
	
	bool constructScene(Scene& scene, std::vector<std::unique_ptr<Material>>& materialPool);

};






#endif // !OBJFILEHANDLE
