#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <directxtk/SimpleMath.h>

#include "ECS/Components/Component.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/RectTransform.h"

namespace NG_Engine
{
    class WorldBuffer;

    class Object
    {
    public:
        Object(const std::string& name, bool rectTransformType = false);
        Object(
            const std::string& name,
            const DirectX::SimpleMath::Vector3& initPos,
            const DirectX::SimpleMath::Vector3& initScale,
            float initRotation,
            bool rectTransformType = false
        );

        virtual ~Object() = default;

        Object(const Object&) = delete;
        Object& operator=(const Object&) = delete;
        Object(Object&&) noexcept = default;
        Object& operator=(Object&&) noexcept = default;

    public:
        virtual void Awake();
        virtual void Start();
        virtual void Destroy();

        virtual void FixedUpdate();
        virtual void Update();
        virtual void LateUpdate();

        virtual void Render();
        virtual void PostRender();

        virtual void GUI();

    public:
        void AddComponent(const std::shared_ptr<Component>& component);
        void RemoveComponent(const std::string& compName);

        template<typename T>
        std::shared_ptr<T> GetComponent(const std::string& compName) const;

        template<typename T>
        T* FindComponentByInterface() const;

        template<typename T>
        std::vector<T*> GetAllComponentsByInterface() const;

    public:
        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& newName) { m_name = newName; }

        std::shared_ptr<Transform> GetTransform() const;
        std::shared_ptr<RectTransform> GetRectTransform() const;

        bool UsesRectTransform() const { return bRectTransformType; }

        void SetWorldBuffer(std::shared_ptr<WorldBuffer> buffer);
        std::shared_ptr<WorldBuffer> GetWorldBuffer() const { return m_worldBuffer; }

    protected:
        void UpdateWorldBuffer();

    protected:
        std::string m_name;
        std::map<std::string, std::shared_ptr<Component>> m_components;
        std::shared_ptr<WorldBuffer> m_worldBuffer;
        bool bRectTransformType = false;
        bool bAwakeCalled = false;
        bool bStartCalled = false;
    };
}

#include "Object.inl"