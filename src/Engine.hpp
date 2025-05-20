#pragma once
#include <vector>
#include <memory>
#include "PhysicsBody.h"

class Engine
{
private:
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> Entities;
	float gravity = -9.8f;
private:
	bool detectAABB(const AxisBox& box1, const AxisBox& box2);
	void CollisionResolution(std::unique_ptr<Body>& ent_A, std::unique_ptr<Body>& ent_B,const Eigen::Vector3f& Collision_normal, const float deltaTime);
	void PenetrationResolution(std::unique_ptr<Body>& ent_A, std::unique_ptr<Body>& ent_B, const Eigen::Vector3f& Collision_normal);
public:
	Engine();
	void Start();
	~Engine();
	void UpdateLoop(const float deltaTime);
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> getEntitiesPtr()const;
};

