#pragma once
#include <Eigen/dense>
#include <Eigen/geometry>
#include <memory>
//for poointing the vertex data

enum BoundingType {CUBE , SPHERE,CUBOID ,CAPSULE};

struct AxisBox {
	Eigen::Vector3f vMax , vMin;
};

//MERE ENGINE ME ACTUAL SHAPE SE HI COLLISON DETECT HOGA NOT ARTIFICAL SHAPE [ UNTILL I CHANGE MY MIND]
//for non Collidable body
class Body
{
protected:
	Eigen::Matrix4f Transform;
	Eigen::Vector3f Position;
	Eigen::Vector3f Scale;
	Eigen::Vector4f Rotation;
	//for cuboid ,capcule , sphere

public:
	BoundingType Collider;
	//in C++ the constructor of Parent is called when we create an Child Object
	Body( BoundingType type);//required so we know whats the bb type
	~Body();

	void setPosition(const Eigen::Vector3f&);
	void Translate(const Eigen::Vector3f&);
	Eigen::Vector3f getPosition()const;

	//syncs with Collider also
	void setScale(const Eigen::Vector3f);
	//only Updates Entity Scale
	Eigen::Vector3f getScale()const;
	//for accessing the data in collision detect for specially cuboid ,capsule,sphere and for renderer raw data
	float getScale_X()const {
		return Scale.x();
	}
	float getScale_Y()const {
		return Scale.y();
	}
	float getScale_Z()const {
		return Scale.z();
	}


	AxisBox getAABBbox()const;

	float* ConstructTransformMat();
	virtual void Update(float deltaTime, const float gravity){}
	virtual Eigen::Vector3f getVelocity()const { return Eigen::Vector3f::Zero(); }//yaha mene null vector send kiya keep that in mind
	virtual float getInverseMass()const { return 0.0f; }
	virtual void ApplyImpulse(const Eigen::Vector3f, const float deltaTime){}
};
//for body which is Collidable
class PhysicsBody :public Body 
{
protected:
	float Mass;
	//Eigen::Vector3f Point_of_mass;  future me 
	Eigen::Vector3f Velocity;
	Eigen::Vector3f Force_accumulator;
	Eigen::Vector3f Angular_velocity;
	Eigen::Vector3f Torque_accumulator;
	bool Influence_gravity;
	//bool Indestructible;  ye mene moving object which will just exerts force not experience itself

public:
	//In C++, if a parent class (Base) requires arguments in its constructor, the child class (Derived) must explicitly call that constructor using an initializer list.
	PhysicsBody(bool gravity_influnce, float mass, BoundingType type);
	~PhysicsBody();

	void Update(float deltaTime, const float gravity) override;//abhi ke liye gravity only y axis me hogi
	void ApplyImpulse(const Eigen::Vector3f, const float deltaTime) override;
	float getInverseMass()const override { return 1/Mass; };

	Eigen::Vector3f getVelocity()const override{ return Velocity; }
	void ApplyForceAtPoint(const Eigen::Vector3f, const Eigen::Vector3f);

};


