#include "Engine.hpp"

#include <iostream>

bool Engine::checkAABB(const AxisBox& box1,const AxisBox& box2 )
{
    static bool fst = true;
    /*if (fst) {

    std::cout << "Box1 min: " << box1.vMin << " max: " << box1.vMax << "\n";
    std::cout << "Box2 min: " << box2.vMin << " max: " << box2.vMax << "\n";
    fst = false;
    }*/

    bool isColliding =
        box1.vMax.x() >= box2.vMin.x() && box1.vMin.x() <= box2.vMax.x() &&
        box1.vMax.y() >= box2.vMin.y() && box1.vMin.y() <= box2.vMax.y() &&
        box1.vMax.z() >= box2.vMin.z() && box1.vMin.z() <= box2.vMax.z();

    return isColliding;
}

Engine::Engine():Entities(std::make_shared<std::vector<std::unique_ptr<Body>>>())
{
}

void Engine::Start()
{
    Entities->emplace_back(std::make_unique<Body>(CUBE));
    //Entities->emplace_back(std::make_unique<Body>(SPHERE));
    Entities->emplace_back(std::make_unique<PhysicsBody>(true, 1.0, SPHERE));
    (*Entities)[1]->setPosition(Eigen::Vector3f(0.0f, 10.0f, 0.0f));
}

Engine::~Engine(){}

void Engine::UpdateLoop(const float deltaTime)
{
    for (int i = 0; i < Entities->size() - 1; i++) {
        for (int j = i + 1; j < Entities->size(); j++) {
            bool res = checkAABB(Entities->at(i)->getAABBbox(), Entities->at(j)->getAABBbox());
            if (res)std::cout << "tap\n";
        }
    }
    for (auto& entity : *Entities) {
        entity->Update(deltaTime, gravity);
    }
}

std::shared_ptr<std::vector<std::unique_ptr<Body>>> Engine::getEntitiesPtr() const
{
    return Entities;
}


