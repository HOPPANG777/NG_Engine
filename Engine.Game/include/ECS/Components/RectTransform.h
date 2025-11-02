#pragma once

#include <directxtk/SimpleMath.h>
#include <imgui.h>

#include "Component.h"
#include "ConstantVector.h"

namespace NG_Engine
{
    class RectTransform : public Component
    {
    public:
        RectTransform(const std::string& name = "RectTransform") : Component(name) {}
        virtual ~RectTransform() override = default;

    public:
        // Component을(를) 통해 상속됨
        void Awake() override;
        void Start() override;

        void Destroy() override;

        void FixedUpdate() override;
        void Update() override;
        void LateUpdate() override;

        void Render() override;
        void PostRender() override;

        void GUI() override;

        const DirectX::SimpleMath::Matrix& GetWorld() const;
        RECT GetRect() const;

    public:
        DirectX::SimpleMath::Vector3 GetPosition() const;
        DirectX::SimpleMath::Vector2 GetAnchoredPosition() const;
        DirectX::SimpleMath::Vector3 GetScale() const;
        float GetRotation() const;

    public:
        DirectX::SimpleMath::Vector2 GetAnchor() const;
        DirectX::SimpleMath::Vector2 GetPivot() const;
        DirectX::SimpleMath::Vector2 GetSize() const;
        DirectX::SimpleMath::Vector2 GetScaledSize() const;

    public:
        void SetPosition(const DirectX::SimpleMath::Vector3& pos);
        void SetAnchoredPosition(const DirectX::SimpleMath::Vector2& pos);
        void SetScale(const DirectX::SimpleMath::Vector3& scale);
        void SetRotation(float degrees);

    public:
        void SetAnchor(const DirectX::SimpleMath::Vector2& anchor);
        void SetPivot(const DirectX::SimpleMath::Vector2& pivot);
        void SetSize(const DirectX::SimpleMath::Vector2& size);

    public:
        void Move(const float& dt, const DirectX::SimpleMath::Vector3& speed);
        void Scale(const float& dt, const DirectX::SimpleMath::Vector3& scale);
        void Rotate(const float& dt, float degrees);

    private:
        DirectX::SimpleMath::Vector3 m_position = ConstantVector::ZeroVector3;
        DirectX::SimpleMath::Vector3 m_scale = ConstantVector::OneVector3;
        float m_rotation = 0.0f;

        // Normalized anchor relative to the screen (0.0f ~ 1.0f)
        DirectX::SimpleMath::Vector2 m_anchor = DirectX::SimpleMath::Vector2(0.5f, 0.5f);
        // Pivot inside the rectangle (0.0f ~ 1.0f)
        DirectX::SimpleMath::Vector2 m_pivot = DirectX::SimpleMath::Vector2(0.5f, 0.5f);
        // Pixel size of the rectangle at the reference resolution used for pivot offset
        DirectX::SimpleMath::Vector2 m_size = ConstantVector::ZeroVector2;

        DirectX::SimpleMath::Matrix m_world = DirectX::XMMatrixIdentity();

        bool bOpen = false;
    };
}




