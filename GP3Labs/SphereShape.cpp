#include "SphereShape.h"
#include "pch.h"

SphereShape::SphereShape(float radius)
{
	m_radius = radius;
	m_shape = new btSphereShape(btScalar(m_radius));
	m_type = CShapeType::SPHERE;
}