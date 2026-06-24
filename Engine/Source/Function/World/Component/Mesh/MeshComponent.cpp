#include "pch.h"

#include "MeshComponent.h"

#include "Render/Render.h"

MeshComponent::MeshComponent(SharedPtr<Mesh> mesh)
    : m_mesh(mesh)
{

}

void MeshComponent::Awake()
{

}

void MeshComponent::OnEnable()
{

}

void MeshComponent::Start()
{

}

void MeshComponent::Update()
{

}

void MeshComponent::LateUpdate()
{

}

void MeshComponent::OnDisable()
{

}

void MeshComponent::OnDestroy()
{

}

std::type_index MeshComponent::GetTypeIndex() const
{
    return typeid(MeshComponent);
}
