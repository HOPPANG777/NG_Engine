#include "ECS/Objects/Object.h"

#include <utility>

#include "Renderer/RenderResources/GlobalBuffer.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace NG_Engine;

namespace
{
    constexpr char kTransformName[] = "Transform";
    constexpr char kRectTransformName[] = "RectTransform";
}

Object::Object(const std::string& name, bool rectTransformType)
    : m_name(name), bRectTransformType(rectTransformType)
{
    if (bRectTransformType)
    {
        const std::shared_ptr<RectTransform>& rectTransform = std::make_shared<RectTransform>();
        this->AddComponent(rectTransform);
    }
    else
    {
        const std::shared_ptr<Transform>& transform = std::make_shared<Transform>();
        this->AddComponent(transform);
    }
}

Object::Object(
    const std::string& name,
    const Vector3& initPos,
    const Vector3& initScale,
    float initRotation,
    bool rectTransformType
)
    : Object(name, rectTransformType)
{
    if (bRectTransformType)
    {
        const std::shared_ptr<RectTransform>& rectTransform = GetComponent<RectTransform>(kRectTransformName);
        if (rectTransform)
        {
            rectTransform->SetPosition(initPos);
            rectTransform->SetScale(initScale);
            rectTransform->SetRotation(initRotation);
        }
    }
    else
    {
        const std::shared_ptr<Transform>& transform = GetComponent<Transform>(kTransformName);
        if (transform)
        {
            transform->SetPosition(initPos);
            transform->SetScale(initScale);
            transform->SetRotation(initRotation);
        }
    }
}

void Object::Awake()
{
    if (bAwakeCalled)
    {
        return;
    }

    for (auto& element : m_components)
    {
        element.second->Awake();
    }

    bAwakeCalled = true;
}

void Object::Start()
{
    if (bStartCalled)
    {
        return;
    }

    for (auto& element : m_components)
    {
        element.second->Start();
    }

    bStartCalled = true;
}

void Object::Destroy()
{
    if (!bAwakeCalled && !bStartCalled)
    {
        return;
    }

    for (auto& element : m_components)
    {
        element.second->Destroy();
    }

    bAwakeCalled = false;
    bStartCalled = false;
}

void Object::FixedUpdate()
{
    for (auto& element : m_components)
    {
        element.second->FixedUpdate();
    }
}

void Object::Update()
{
    for (auto& element : m_components)
    {
        element.second->Update();
    }

    UpdateWorldBuffer();
}

void Object::LateUpdate()
{
    for (auto& element : m_components)
    {
        element.second->LateUpdate();
    }
}

void Object::Render()
{
    UpdateWorldBuffer();

    if (m_worldBuffer)
    {
        m_worldBuffer->SetVSBuffer(0);
    }

    for (auto& element : m_components)
    {
        element.second->Render();
    }
}

void Object::PostRender()
{
    for (auto& element : m_components)
    {
        element.second->PostRender();
    }
}

void Object::GUI()
{
    for (auto& element : m_components)
    {
        element.second->GUI();
    }
}

void Object::AddComponent(const std::shared_ptr<Component>& component)
{
    if (!component)
    {
        return;
    }

    if (m_components.count(component->GetName()) != 0)
    {
        return;
    }

    component->SetOwner(this);
    m_components.emplace(component->GetName(), component);

    if (bAwakeCalled)
    {
        component->Awake();
    }

    if (bStartCalled)
    {
        component->Start();
    }
}

void Object::RemoveComponent(const std::string& compName)
{
    m_components.erase(compName);
}

std::shared_ptr<Transform> Object::GetTransform() const
{
    return GetComponent<Transform>(kTransformName);
}

std::shared_ptr<RectTransform> Object::GetRectTransform() const
{
    return GetComponent<RectTransform>(kRectTransformName);
}

void Object::SetWorldBuffer(std::shared_ptr<WorldBuffer> buffer)
{
    m_worldBuffer = std::move(buffer);
}

void Object::UpdateWorldBuffer()
{
    if (!m_worldBuffer)
    {
        return;
    }

    if (bRectTransformType)
    {
        const std::shared_ptr<RectTransform>& rectTransform = GetRectTransform();
        if (rectTransform)
        {
            m_worldBuffer->SetWorld(rectTransform->GetWorld());
            return;
        }
    }

    const std::shared_ptr<Transform>& transform = GetTransform();
    if (transform)
    {
        m_worldBuffer->SetWorld(transform->GetWorld());
    }
}