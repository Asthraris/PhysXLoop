#pragma once

#include "rend/Renderer_Data.hpp"
#include "PhysicsBody.h"
#include <vector>

class Scene{
	MeshLibrary s_meshLib;
public:
	std::vector<Body> Static_entities;
	std::vector<PhysicsBody> Dynamic_entities;
	Scene() :s_meshLib(){

	}
	void MainSetup(){}
	
	
};

