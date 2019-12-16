#pragma once
#include "Component.h"
#include "Camera.h"
class CameraComp : public Component
{
private:
	Camera* m_camera;
	// Inherited via Component
public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnAttatch() override;
	virtual void UpdateModel(std::shared_ptr<ShaderProgram> shader) override;
};