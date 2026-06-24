#pragma once

#include "Core.h"
#include "Runtime.h"
#include "String/String.h"
#include "Container/Array.h"
#include "Container/HashMap.h"
#include "Memory/RAII.h"
#include "FileSystem/File.h"
#include "Config/ConfigManager.h"
#include "World/GameObject.h"
#include "Import/ImportAsset.h"

#include "World/Component/Mesh/MeshComponent.h"

class Scene
{
public:

    ~Scene() = default;

    Scene(const String& name, const String& url)
        : Name(name)
        , Url(url)
    {

    }
public:
    void Update()
    {
        LoadScene();

        for (auto& gameObject : m_gameObjects)
        {
            gameObject->Update();
        }
    }

    void LoadScene()
    {
        if (m_isLoaded) return;

        auto gameObject = MakeShared<GameObject>("Cube");

        auto mesh = MakeShared<Mesh>();

        ImportObj("Asset/Mesh/cube.obj", *mesh.RawPtr());

        gameObject->AddComponent<MeshComponent>(mesh);

        m_gameObjects.Add(gameObject);


        m_isLoaded = true;
    }
private:
    String Name;

    String Url;

    bool m_isLoaded = false;

    Array<SharedPtr<GameObject>> m_gameObjects;
};