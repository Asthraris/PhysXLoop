#pragma once
#include <vector>
#include <memory>
#include "PhysicsBody.h"

struct Engine_Ui {
	bool Gravity =true;
	bool Collision = true;
};
class Engine
{
private:
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> Entities;
	float gravity = -9.8f;
	Engine_Ui ui;
private:
	bool detectAABB(const AxisBox& box1, const AxisBox& box2);
	bool detectABSphere(float rad1, const AxisBox& box2, const Eigen::Vector3f& Sphere_ent_pos );
	bool detect2Spheres(float rad1,float rad2 , const Eigen::Vector3f& centerDiff);
	void CollisionResolution(std::unique_ptr<Body>& ent_A, std::unique_ptr<Body>& ent_B,const Eigen::Vector3f& Collision_normal, const float deltaTime);
	void PenetrationResolution(std::unique_ptr<Body>& ent_A, std::unique_ptr<Body>& ent_B, const Eigen::Vector3f& Collision_normal);
public:
	Engine();
	void Start();
	~Engine();
	void UpdateLoop(const float deltaTime);
	void imGuiDraw_eng();
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> getEntitiesPtr()const;
};

