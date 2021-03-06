#include "Component.h"
#include "Physics.h"
#include "CollisionShape.h"
class RigidBody : public Component
{
private:
	CollisionShape* m_shape;
	btRigidBody* m_rigidBody;
	bool isStatic = false;
	btMotionState* m_mState = nullptr;
	float mass;
public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnAttatch() override;
	virtual void UpdateModel(std::shared_ptr<ShaderProgram> shader) override;
	void Init(CollisionShape* shape, float mass = 1.f, const glm::vec3
		localInertia = glm::vec3(1));
	virtual void UpdateParent();
	virtual void UpdateRigidBody();
	btRigidBody* Get() { return m_rigidBody; }

	virtual void ApplyForce(btVector3 force);
	virtual void ApplyTorque(btVector3 torque);
};