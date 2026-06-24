#pragma once

#include "Core.h"
#include "World/Resource/Mesh.h"

#include <typeindex>

class GameObject;
class Transform;

class Component
{
public:
    friend class GameObject;
public:
    virtual ~Component() = default;
public:
    // 生命周期函数
    virtual void Awake() = 0;

    virtual void OnEnable() = 0;

    virtual void Start() = 0;

    virtual void Update() = 0;

    virtual void LateUpdate() = 0;

    virtual void OnDisable() = 0;

    virtual void OnDestroy() = 0;

    virtual std::type_index GetTypeIndex() const = 0;
public:
    GameObject* GetGameObject() const;

    Transform* GetTransform() const;
public:
    bool Enabled = true;
protected:
    GameObject* m_gameObject = nullptr;

    bool m_started = false;
};




