#pragma once
#include <vector>
#include <memory>
#include "PhysicsBody.h"

class Engine
{
private:
	bool checkAABB(const AxisBox& box1, const AxisBox& box2);
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> Entities;
	float gravity = -9.8f;
public:
	Engine();
	void Start();
	~Engine();
	void UpdateLoop(const float deltaTime);
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> getEntitiesPtr()const;
};

