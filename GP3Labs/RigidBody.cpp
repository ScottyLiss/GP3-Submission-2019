#include "pch.h"
#include "RigidBody.h"
#include "Physics.h"
#include "Entity.h"
void RigidBody::OnUpdate(float deltaTime)
{
}
void RigidBody::OnRender()
{
}
void RigidBody::Init(CollisionShape* shape, float mass, const glm::vec3
	localInertia)
{
	btTransform bT = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_mState = new btDefaultMotionState(bT);
	m_shape = shape;
	btVector3 li = btVector3(localInertia.x, localInertia.y,localInertia.z);
	m_shape->Get()->calculateLocalInertia(mass, li);
	m_rigidBody = new btRigidBody(btScalar(mass), m_mState,
		m_shape->Get(), li);
	Physics::GetInstance()->AddRigidbody(this);
	m_rigidBody->setSleepingThresholds(0, 0);
	m_rigidBody->setFriction(1);
}
void RigidBody::UpdateParent()
{
	//TODO: Get the transform from the rigidbody's motionstate and apply

	
	btTransform bt;
	m_rigidBody->getMotionState()->getWorldTransform(bt);

	Physics::ConvertBtTransformToTransform(bt, m_entity->GetTransform());


}
void RigidBody::UpdateRigidBody()
{
	btTransform t =	Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_rigidBody->setWorldTransform(t);
	m_rigidBody->getMotionState()->setWorldTransform(t);
}

void RigidBody::OnAttatch()
{

}

void RigidBody::UpdateModel(std::shared_ptr<ShaderProgram> shader)
{
}

void RigidBody::ApplyForce(btVector3 force)
{
	btTransform bt;
	m_rigidBody->getMotionState()->getWorldTransform(bt);

	m_rigidBody->applyForce(force, m_rigidBody->getCenterOfMassPosition());

	Physics::ConvertBtTransformToTransform(bt, m_entity->GetTransform());
}
void RigidBody::ApplyTorque(btVector3 torque)
{
	btTransform bt;
	m_rigidBody->getMotionState()->getWorldTransform(bt);

	m_rigidBody->applyTorqueImpulse(torque);

	Physics::ConvertBtTransformToTransform(bt, m_entity->GetTransform());
}
