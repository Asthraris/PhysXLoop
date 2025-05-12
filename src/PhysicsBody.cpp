#include "PhysicsBody.h"

Body::Body(std::shared_ptr<Mesh> mesh, BoundingType type) :
	Position(Eigen::Vector3f::Zero()),
	Scale(Eigen::Vector3f::Ones()),
	Rotation(Eigen::Vector4f(0.0f, 1.0f, 0.0f, 0.0f)),
	Collider(type,mesh)
{
	//this way of initiatization is faster than Block init and aslo gets called over child creation
	
}

Body::~Body()
{
}

void Body::setPosition(const Eigen::Vector3f pos)
{
	Position = pos;
}

void Body::Translate(const Eigen::Vector3f change)
{
	Position += change;
}

Eigen::Vector3f Body::getPosition() const
{
	return Position;
}

void Body::setScale(const Eigen::Vector3f sca)
{
	Collider.size = sca;
	Scale = sca;
}

void Body::setScaleOnly(const Eigen::Vector3f sca)
{
	Scale = sca;

}

float* Body::ConstructTransformMat()
{
	//"Affine" just means it supports linear transformations plus translation (like a mat4 in OpenGL).
	Eigen::Affine3f model = Eigen::Affine3f::Identity();
	model.translate(Position);
	//isline me mene rotation ko break kiya  === {x,y,z,angle} me and then normalized it afterwards
	model.rotate(Eigen::AngleAxisf(Rotation.w(), (Eigen::Vector3f)Rotation.head<3>().normalized()));
	model.scale(Scale);

	Transform = model.matrix();
	return Transform.data();
		
}
//====================================================================
PhysicsBody::PhysicsBody(bool gravity_influnce, float mass ,std::shared_ptr<Mesh> mesh, BoundingType type, bool indes = false):
	Body(mesh,type),
	Influence_gravity(gravity_influnce),
	Mass(mass),
	Indestructible(indes),
	Force_accumulator(Eigen::Vector3f::Zero()),
	Torque_accumulator(Eigen::Vector3f::Zero())
{
	
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::Update(const float deltaTime,const float gravity)
{
	if (!Indestructible) {
		if (Influence_gravity) 
			Force_accumulator += Mass * Eigen::Vector3f( 0.0f, gravity, 0.0f) * deltaTime;
		//linear
		Velocity += Force_accumulator/Mass * deltaTime;
		//Angular
		//Angular_velocity += Torque_accumulator/ * deltaTime;
		Force_accumulator.setZero();
		Torque_accumulator.setZero();
	}

	Position += Velocity * deltaTime;
	//update rotation also acc to ang_velo

}

void PhysicsBody::ApplyForce(const Eigen::Vector3f new_force)
{
	Force_accumulator += new_force;
}

void PhysicsBody::ApplyForceAtPoint(const Eigen::Vector3f new_force, const Eigen::Vector3f point)
{
	Force_accumulator += new_force;
	Torque_accumulator += (point - Position).cross(new_force);
}


