#pragma once
#include "ShaderProgram.h"
//forward declaring to avoid circular includes
class Entity;
class Component
{
protected:
	Entity* m_entity = nullptr; 
	friend class Entity;
public:
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnAttatch() = 0;
	virtual void UpdateModel(std::shared_ptr<ShaderProgram> shader) = 0;

};
