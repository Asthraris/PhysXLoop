#include "Engine.hpp"

Engine::Engine():MAIN_SCENE()
{
}

void Engine::Start()
{
}

Engine::~Engine()
{
}

void Engine::UpdateLoop(const float deltaTime)
{

}

std::shared_ptr<Scene> Engine::giveAccess2Scene() const
{
    return MAIN_SCENE;
}
