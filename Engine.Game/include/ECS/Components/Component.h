#pragma once

#include <string>

namespace NG_Engine
{
    class Object;

    class Component
    {
    protected:
        explicit Component(const std::string& compName);

    public:
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component&&) = delete;

    public:
        Object* GetOwner() const { return m_owner; }
        const std::string& GetName() const { return m_name; }

        void SetOwner(Object* obj) { m_owner = obj; }

    public:
        virtual void Awake() = 0;
        virtual void Start() = 0;

        virtual void Destroy() = 0;

        virtual void FixedUpdate() = 0;
        virtual void Update() = 0;
        virtual void LateUpdate() = 0;

        virtual void Render() = 0;
        virtual void PostRender() = 0;

        virtual void GUI() = 0;

    protected:
        std::string m_name;
        Object* m_owner = nullptr;
    };
}