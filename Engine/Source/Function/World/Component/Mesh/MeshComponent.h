#pragma once

#include "Core.h"
#include "Memory/RAII.h"
#include "World/Resource/Mesh.h"

#include "World/Component/Component.h"

class MeshComponent : public Component
{
public:
    ~MeshComponent() = default;

    MeshComponent(SharedPtr<Mesh> mesh);
public:
    // 生命周期
    virtual void Awake() override;

    virtual void OnEnable() override;

    virtual void Start() override;

    virtual void Update() override;

    virtual void LateUpdate() override;

    virtual void OnDisable() override;

    virtual void OnDestroy() override;

    std::type_index GetTypeIndex() const override;
public:

private:
    SharedPtr<Mesh> m_mesh;
};