#include "Engine.hpp"

#include <iostream>

bool Engine::detectAABB(const AxisBox& box1,const AxisBox& box2 )
{
    return (box1.vMax.x() >= box2.vMin.x() && box1.vMin.x() <= box2.vMax.x() &&
        box1.vMax.y() >= box2.vMin.y() && box1.vMin.y() <= box2.vMax.y() &&
        box1.vMax.z() >= box2.vMin.z() && box1.vMin.z() <= box2.vMax.z());
}

Engine::Engine():Entities(std::make_shared<std::vector<std::unique_ptr<Body>>>())
{
}

void Engine::Start()
{
    Entities->emplace_back(std::make_unique<Body>(CUBE));
    //Entities->emplace_back(std::make_unique<Body>(SPHERE));
    Entities->emplace_back(std::make_unique<PhysicsBody>(false, 1.0, SPHERE));
    Entities->emplace_back(std::make_unique<PhysicsBody>(true, 1.0, SPHERE));
    (*Entities)[0]->setPosition(Eigen::Vector3f(0.0f, -2.0f, 0.0f));

    (*Entities)[2]->setPosition(Eigen::Vector3f(0.0f, 10.0f, 0.0f));
}

Engine::~Engine(){}

void Engine::UpdateLoop(const float deltaTime)
{
    //wese har frame me collision check karna expensive hota hai for game engine buts since i m making physics engine i will check
    for (int i = 0; i < Entities->size() - 1; i++) {
        for (int j = i + 1; j < Entities->size(); j++) {
            //agar collision detect huwa toh resolve karo
            //abhi mene sirf aabb hi add kiya hai baad me sphere aabb then sphere -sphere bhi karna hai
            if (detectAABB(Entities->at(i)->getAABBbox(), Entities->at(j)->getAABBbox())) {
                //two ways to resolve collision
                //1. for impulse mostly moving objects
                Eigen::Vector3f normal = (Entities->at(i)->getPosition() - Entities->at(j)->getPosition()).normalized();
                Eigen::Vector3f relVel = Entities->at(i)->getVelocity() - Entities->at(j)->getVelocity();

                float velAlongNormal = relVel.dot(normal);
                if (velAlongNormal > 0) return; // already separating

                float restitution = 0.5f; // elasticity (0 = inelastic, 1 = perfect elastic)

                // optional: get masses
                float inv_m1 = Entities->at(i)->getMass();
                float inv_m2 = Entities->at(j)->getMass();

                inv_m1 = (inv_m1 == 0.0f) ? 0.0f : 1 / inv_m1; //idhar 0 se rerpresnet kiya tha mass whic was infinity but sinec i cant represent in code
                inv_m2 = (inv_m2 == 0.0f) ? 0.0f : 1 / inv_m2;


                float impulseMag = -(1 + restitution) * velAlongNormal / (inv_m1 + inv_m2);
                Eigen::Vector3f impulse = impulseMag * normal;

                Entities->at(i)->ApplyImpulse(impulse, deltaTime);
                Entities->at(j)->ApplyImpulse(-impulse, deltaTime);

                
                //2.face intersect
            }
            
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


