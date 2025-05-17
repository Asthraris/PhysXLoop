#include "Engine.hpp"

Engine::Engine():Entities(std::make_shared<std::vector<Body>>())
{
}

void Engine::Start()
{
    Body box(CUBE);
    Entities->push_back(box);
}

Engine::~Engine()
{
}

void Engine::UpdateLoop(const float deltaTime)
{
    
}

std::shared_ptr<std::vector<Body>> Engine::getEntitiesPtr() const
{
    return Entities;
}


