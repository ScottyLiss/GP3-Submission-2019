#include "pch.h"
#include "CameraComp.h"
#include "Camera.h"
//#include "Input.h"
void CameraComp::OnUpdate(float deltaTime)
{
}
void CameraComp::OnRender()
{
}
void CameraComp::OnAttatch()
{
	m_camera = new Camera(m_entity->GetTransform());

	if (Application::GetInstance()->GetCamera() == nullptr)
	{
		Application::GetInstance()->SetCamera(m_camera);
	}
}

void CameraComp::UpdateModel(std::shared_ptr<ShaderProgram> shader)
{

}
