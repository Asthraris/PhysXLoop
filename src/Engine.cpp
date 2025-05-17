#include "Engine.hpp"


Engine::Engine():Entities(std::shared_ptr<std::vector<std::unique_ptr<Body>>>())
{
    gravity = -9.8;
}

void Engine::Start()
{
    Entities->push_back(std::make_unique<Body>(CUBE));
    Entities->push_back(std::make_unique<PhysicsBody>(true, 1.0, SPHERE, true));
}

Engine::~Engine()
{
}

void Engine::UpdateLoop(const float deltaTime)
{
    for (auto& entity : *Entities) {
        entity->Update(deltaTime, gravity);
    }
}

std::shared_ptr<std::vector<std::unique_ptr<Body>>> Engine::getEntitiesPtr() const
{
    return Entities;
}


