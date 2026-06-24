#include "pch.h"

#include "Component.h"

#include "World/GameObject.h"

GameObject* Component::GetGameObject() const 
{ 
    return m_gameObject; 
}

Transform* Component::GetTransform() const 
{ 
    return m_gameObject ? &m_gameObject->Transform : nullptr; 
}