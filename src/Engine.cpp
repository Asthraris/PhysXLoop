#include "Engine.hpp"
#include <imGui/imgui.h>
#include <iostream>

bool Engine::detectAABB(const AxisBox& box1,const AxisBox& box2 )
{
    return (box1.vMax.x() >= box2.vMin.x() && box1.vMin.x() <= box2.vMax.x() &&
        box1.vMax.y() >= box2.vMin.y() && box1.vMin.y() <= box2.vMax.y() &&
        box1.vMax.z() >= box2.vMin.z() && box1.vMin.z() <= box2.vMax.z());
}

bool Engine::detectABSphere(float rad1, const AxisBox& box2,const Eigen::Vector3f& Sphere_ent_pos)
{
    
    // Clamp the sphere center to the AABB to find the closest point
    Eigen::Vector3f closestPoint;
    closestPoint.x() = std::max(box2.vMin.x(), std::min(Sphere_ent_pos.x(), box2.vMax.x()));
    closestPoint.y() = std::max(box2.vMin.y(), std::min(Sphere_ent_pos.y(), box2.vMax.y()));
    closestPoint.z() = std::max(box2.vMin.z(), std::min(Sphere_ent_pos.z(), box2.vMax.z()));

    // Vector from closest point on AABB to sphere center
    Eigen::Vector3f diff = Sphere_ent_pos - closestPoint;
    float distSq = diff.squaredNorm();

    if (distSq < rad1 * rad1) {
        return true;
    }

    return false;
    

}

bool Engine::detect2Spheres(float rad1, float rad2, const Eigen::Vector3f& normal)
{
    return (rad1+rad2 >= normal.norm());
}


void Engine::CollisionResolution(std::unique_ptr<Body>& ent_A, std::unique_ptr<Body>& ent_B , const Eigen::Vector3f& Collision_normal, const float deltaTime)
{
    //find normal of collided bodies
    Eigen::Vector3f relative_velo_upon_collison = ent_B->getVelocity() - ent_A->getVelocity();
    //find total velocity of system
    float Magnitude_of_Collision = relative_velo_upon_collison.dot(Collision_normal);
    if (Magnitude_of_Collision > 1.0f)return;
    //find Impact of collison less if angle between normal and velocities is more cos(theta)

    float Restitution_e = 0.0f;
    //collison type 0 for inelastic 1 for elastic  calc by min of two object under collision having their restitution but for now its defined
    Eigen::Vector3f Impulse = -(1 + Restitution_e) * (Magnitude_of_Collision *Collision_normal)/(ent_A->getInverseMass() + ent_B->getInverseMass());

    ent_A->ApplyImpulse(-Impulse, deltaTime);
    ent_B->ApplyImpulse(Impulse, deltaTime);

}

void Engine::PenetrationResolution(std::unique_ptr<Body>& ent_A, std::unique_ptr<Body>& ent_B, const Eigen::Vector3f& Collision_normal)
{
    // Cache inverse masses
    float invMassA = ent_A->getInverseMass();
    float invMassB = ent_B->getInverseMass();
    float netInvMass = invMassA + invMassB;

    // Skip if both objects are static
    if (netInvMass == 0.0f) return;

    // Get penetration depth (magnitude of collision normal)
    float penetration = Collision_normal.norm();

    // Apply slop: ignore micro penetrations
    const float slop = 0.05f;
    if (penetration < slop) return;

    // Amount of correction to apply (not full to avoid jitter)
    const float percent = 0.004f;

    // Compute depth to correct
    float depth = penetration - slop;
    if (depth < 0.0f)return;
    Eigen::Vector3f normal = Collision_normal.normalized();
    Eigen::Vector3f correction = (depth / netInvMass) * percent * normal;

    // Apply asymmetric correction based on static/dynamic
    if (invMassA == 0.0f) {
        ent_B->Translate(-correction); // Only B moves
    }
    else if (invMassB == 0.0f) {
        ent_A->Translate(correction);  // Only A moves
    }
    else {
        // Both move according to inverse mass
        ent_A->Translate(correction * invMassA);
        ent_B->Translate(-correction * invMassB);
    }

}

Engine::Engine():Entities(std::make_shared<std::vector<std::unique_ptr<Body>>>())
{
}

void Engine::Start()
{
    Entities->emplace_back(std::make_unique<Body>(CUBE));
    //Entities->emplace_back(std::make_unique<Body>(SPHERE));
    Entities->emplace_back(std::make_unique<PhysicsBody>(false, 1.0, CUBE));
    Entities->emplace_back(std::make_unique<PhysicsBody>(true, 1.0, CUBE));
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
            auto entityA = Entities->at(i).get();
            auto entityB = Entities->at(j).get();

            if (entityA->getInverseMass() +entityB->getInverseMass() == 0.0f)continue;
           
            Eigen::Vector3f Collision_normal = (entityA->getPosition() - entityB->getPosition()).normalized();

            //DETECTION
            bool colides = false;

            bool isASphere = (entityA->Collider == SPHERE);
            bool isBSphere = (entityB->Collider == SPHERE);

            if (isASphere && isBSphere) {
                colides = detect2Spheres(entityA->getScale_X(), entityB->getScale_X(),(entityA->getPosition() - entityB->getPosition()));
            }
            else if (isASphere || isBSphere) {
                // Ensure the sphere is passed first if needed
                if (isASphere)
                {
                    colides = detectABSphere(entityA->getScale_X(),entityB->getAABBbox() ,entityA->getPosition());
                }
                else
                {
                    colides = detectABSphere(entityB->getScale_X(), entityA->getAABBbox(), entityB->getPosition());
                }
            }
            else {
                colides = detectAABB(entityA->getAABBbox(), entityB->getAABBbox());
            }



            //RESOLUTION
            if (colides) {
                //two ways to resolve collision

                //1. for impulse mostly moving objects
                CollisionResolution(Entities->at(i), Entities->at(j), -Collision_normal, deltaTime);

                //2.face intersect
                PenetrationResolution(Entities->at(i), Entities->at(j), Collision_normal);
            }
            
        }
    }

    for (auto& entity : *Entities) {
        entity->Update(deltaTime, gravity);
    }
}

void Engine::imGuiDraw_eng()
{
    ImGui::Begin("UI");
    ImGui::Checkbox("Collision",&ui.Collision);
    ImGui::SameLine();
    ImGui::Checkbox("Gravity", &ui.Gravity);
    ImGui::End();
}

std::shared_ptr<std::vector<std::unique_ptr<Body>>> Engine::getEntitiesPtr() const
{
    return Entities;
}


