#pragma once
#include "Component.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Model.h"


class MeshRenderer : public Component
{
private:
	// Made into shader pointers, for performance 
	std::shared_ptr<Model> m_model;
	std::shared_ptr<ShaderProgram> m_program;

	std::shared_ptr<ShaderProgram> m_GeometryPass;
	std::shared_ptr<ShaderProgram> m_LightingPass;
	std::shared_ptr<ShaderProgram> m_LightBox;

	std::shared_ptr<Texture> m_texture;

public:
	MeshRenderer(std::shared_ptr < Model> model, std::shared_ptr < ShaderProgram> program, std::shared_ptr < Texture> texture);
	// Inherited via Component
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnAttatch() override;
	virtual void SetShaders(std::shared_ptr<ShaderProgram> lightPass, std::shared_ptr<ShaderProgram> geomPass, std::shared_ptr<ShaderProgram> lightBox);
	virtual void UpdateModel(std::shared_ptr<ShaderProgram> shader) override;
	
};
