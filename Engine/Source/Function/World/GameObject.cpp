#include "pch.h"

#include "GameObject.h"

GameObject::GameObject(const String& name)
    : Name(name)
{

}

void GameObject::Update()
{
    if (!IsActive)
        return;

    isProcessing = true;

    // 第一次 Update 前调用 Start
    if (!m_started)
    {
        m_started = true;

        for (auto& component : m_components)
        {
            if (component->Enabled)
            {
                component->Start();
                component->m_started = true;
            }
        }
    }

    // Update 所有启用的组件
    for (auto& component : m_components)
    {
        if (component->Enabled)
        {
            component->Update();
        }
    }

    isProcessing = false;

    _DelayedProcess();
}

GameObject* GameObject::GetParent() const
{
    return m_parent;
}

const Array<GameObject*>& GameObject::GetChildren() const
{
    return m_childrens;
}

void GameObject::SetParent(GameObject* parent)
{
        if (parent == this)
        {
            return;
        }
    
        // 从当前父对象移除
        if (m_parent)
        {
            auto& siblings = m_parent->m_childrens;
            for (int64 i = 0; i < siblings.Size(); i++)
            {
                if (siblings[i] == this)
                {
                    siblings.Erase(i);
                    break;
                }
            }
        }
    
        m_parent = parent;
    
        // 添加到新父对象的子对象列表
        if (m_parent)
        {
            m_parent->m_childrens.Add(this);
        }
}

void GameObject::_DelayedProcess()
{
    for (auto& op : m_operations)
    {
        switch (op.type)
        {
        case DelayedOperation::Add:
        {
            auto* comp = op.component;
            comp->m_gameObject = this;

            auto type = comp->GetTypeIndex();
            m_componentMap[type].Add(comp);
            m_components.Add(ScopePtr<Component>(comp));

            if (IsActive)
            {
                comp->Awake();
            }
            break;
        }

        case DelayedOperation::Remove:
        {
            auto* component = op.component;
            component->OnDestroy();

            auto type = component->GetTypeIndex();
            auto iter = m_componentMap.Find(type);
            if (iter != m_componentMap.end())
            {
                iter->second.Remove(component);
            }

            for (auto& value : m_components)
            {
                if (value.RawPtr() == component)
                {
                    m_components.Remove(value);
                    break;
                }
            }
            break;
        }

        case DelayedOperation::Enable:
            op.component->OnEnable();
            break;

        case DelayedOperation::Disable:
            op.component->OnDisable();
            break;
        }
    }

    m_operations.Clear();
}
