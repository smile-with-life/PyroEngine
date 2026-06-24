#pragma once

#include "Core.h"
#include "Runtime.h"
#include "String/String.h"
#include "Container/Array.h"
#include "Container/HashMap.h"
#include "Memory/RAII.h"
#include "World/Component/Component.h"

#include <typeindex>

#include <glm/glm.hpp>

struct Transform 
{
    glm::vec3 Position;
    glm::vec3 Rotation;   // 欧拉角
    glm::vec3 Scale;
};


class GameObject
{
public:
    GameObject() = default;

    ~GameObject() = default;

    explicit GameObject(const String& name);
public:
    template<class Type, class... Args>
    Type* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, Type>::value, "Type must inherit from Component");

        Type* ptr = new Type(std::forward<Args>(args)...);
        if (isProcessing)
        {
            // 延迟添加
            m_operations.Add({ DelayedOperation::Add, ptr });
            return ptr;
        }

        // 设置所属关系
        ptr->m_gameObject = this;

        // 存储
        auto type = std::type_index(typeid(Type));
        m_componentMap[type].Add(ptr);
        m_components.Emplace(ptr);

        // 立即调用Awake
        if (IsActive)
        {
            ptr->Awake();
        }

        return ptr;
    }

    template<class Type>
    Type* GetComponent()
    {
        auto iter = m_componentMap.Find(std::type_index(typeid(Type)));
        if (iter != m_componentMap.end() && !iter->second.empty())
        {
            return static_cast<Type*>(iter->second[0]);
        }
        return nullptr;
    }

    template<class Type>
    Array<Type*> GetComponents()
    {
        Array<Type*> result;
        auto iter = m_componentMap.Find(std::type_index(typeid(Type)));
        if (iter != m_componentMap.end())
        {
            for (auto* component : iter->second)
            {
                result.Add(static_cast<Type*>(component));
            }
        }
        return result;
    }

    template<class Type>
    void RemoveComponent()
    {
        if (isProcessing)
        {
            auto* component = GetComponent<Type>();
            if (component)
            {
                m_operations.Add({ DelayedOperation::Remove, component });
            }
            return;
        }
        auto type = std::type_index(typeid(Type));
        auto iter = m_componentMap.Find(type);
        if (iter != m_componentMap.end())
        {
            for (auto* component : iter->second)
            {
                component->OnDestroy();

                // 从components容器中移除
                auto it = m_components.Find(component);
                if (it != m_components.end())
                {
                    m_components.Erase(it);
                }
            }
            m_componentMap.Erase(iter);
        }

    }
public:
    void Update();

    GameObject* GetParent() const;

    const Array<GameObject*>& GetChildren() const;

    void SetParent(GameObject* parent);
private:
    void _DelayedProcess();
public:
    String Name = "GameObject";

    Transform Transform;

    bool IsActive = true;
private:
    struct DelayedOperation
    {
        enum Type { Add, Remove, Enable, Disable };
        Type type;
        Component* component = nullptr;
    };

    bool m_started = false;

    Array<ScopePtr<Component>> m_components;

    HashMap<std::type_index, Array<Component*>> m_componentMap;

    Array<DelayedOperation> m_operations;

    bool isProcessing = false;

    GameObject* m_parent = nullptr;

    Array<GameObject*> m_childrens;
};