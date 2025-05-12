#pragma once
#include <memory>
#include "Scene.hpp"
class Engine
{
private:
	std::shared_ptr<Scene> MAIN_SCENE;
public:
	Engine();
	void Start();
	~Engine();
	void UpdateLoop(const float deltaTime);
	std::shared_ptr<Scene> giveAccess2Scene()const;
};

