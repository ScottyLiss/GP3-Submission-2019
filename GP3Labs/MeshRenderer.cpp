#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"


MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr < ShaderProgram> program, std::shared_ptr < Texture>
	texture)
{
	m_model = model;
	m_program = program;
	m_texture = texture;
}

void MeshRenderer::SetShaders(std::shared_ptr<ShaderProgram> lightPass, std::shared_ptr<ShaderProgram> geomPass, std::shared_ptr<ShaderProgram> lightBox)
{
	m_LightBox = lightBox;
	m_LightingPass = lightPass;
	m_GeometryPass = geomPass;
}

#pragma region Render Functions for MeshRenderer
void MeshRenderer::OnRender() // Default rendering for objects that do not use Deferred rendering
{
	// Previous code removed, more efficient to only call what is specific to the model to the shader
	// Instead of calling every (e.g LightPos) as those are unique to the shader, not to the model
	m_program->Use();
	m_program->setMat4("model", m_entity->GetTransform()->GetTransformationMatrix());
	
	m_texture->Bind();
	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		if (m_texture) m_texture->Bind();
		(glDrawElements(GL_TRIANGLES,
			mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}
}

void MeshRenderer::UpdateModel(std::shared_ptr<ShaderProgram> shader) // Used for rendering objects with deferred rendering
{
	shader->Use(); // Ensures that we use the shader passed down
	shader->setMat4("model", m_entity->GetTransform()->GetTransformationMatrix()); //Sets the Mat4 model in the shader
	m_texture->Bind();


	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		if (m_texture) m_texture->Bind();
		(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}
}
#pragma endregion

#pragma region Functions With No Use for the moment

//-----------------------------------------//
//		Function with no use for now	   //
//-----------------------------------------//
void MeshRenderer::OnUpdate(float deltaTime)
{

}

void MeshRenderer::OnAttatch()
{

}
#pragma endregion
