#pragma once
#include <vector>
#include <memory>
#include "PhysicsBody.h"

class Engine
{
private:
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> Entities;
	float gravity;
public:
	Engine();
	void Start();
	~Engine();
	void UpdateLoop(const float deltaTime);
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> getEntitiesPtr()const;
};

