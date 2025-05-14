#pragma once
#include <vector>
#include <memory>
#include "PhysicsBody.h"

class Engine
{
private:
	std::shared_ptr<std::vector<Body>> Entities;
public:
	Engine();
	void Start();
	~Engine();
	void UpdateLoop(const float deltaTime);
	auto getEntitiesPtr()const;
};

