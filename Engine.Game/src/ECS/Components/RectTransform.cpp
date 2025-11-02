#include "ECS/Components/RectTransform.h"
#include "ConstantVector.h"
#include "Common.h"
#include "Screen.h"
#include "UI.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace NG_Engine;
using namespace Common;
using namespace Screen;
using namespace UI;

namespace
{
    constexpr float kMinAnchor = 0.0f;
    constexpr float kMaxAnchor = 1.0f;
}

void RectTransform::Update()
{
    using namespace DirectX::SimpleMath;

    const Vector2 screenSize(static_cast<float>(GetWinWidth()), static_cast<float>(GetWinHeight()));
    const Vector2 uiScale = GetUIScale();
    Vector3 translation(m_position.x * uiScale.x, m_position.y * uiScale.y, m_position.z);

    const Vector3 anchorOffset(m_anchor.x * screenSize.x, m_anchor.y * screenSize.y, 0.0f);
    translation += anchorOffset;

    const Vector2 scaledSize = GetScaledSize();
    const Vector2 pivotOffset((m_pivot.x - 0.5f) * scaledSize.x, (m_pivot.y - 0.5f) * scaledSize.y);
    translation.x -= pivotOffset.x;
    translation.y -= pivotOffset.y;

    translation += ConstantVector::ZeroVector3; //나중에 카메라 컴포넌트를 추가해라 //CameraManager::Get()->GetPosition();

    const Vector3 scaledScale(m_scale.x * uiScale.x, m_scale.y * uiScale.y, m_scale.z);
    const Matrix S = DirectX::XMMatrixScalingFromVector(scaledScale);
    const Matrix R = DirectX::XMMatrixRotationZ(m_rotation);
    const Matrix T = DirectX::XMMatrixTranslationFromVector(translation);

    m_world = S * R * T;
}

void RectTransform::Render()
{
    // RectTransform does not render anything by itself.
}

void RectTransform::GUI()
{
    // ImGui를 이용해 UI를 만들어내는 기능과 코드들

    // ImGui 코드들을 이용해 UI를 만들기위해 먼저 실행해야하는 함수
    ImGui::Begin("RectTransform", &bOpen);
    {
        ImGui::Text("RectTrasnform");

        ImGui::SliderFloat3(
            "Position",
            (float*)&m_position,
            0,
            GetWinWidth()
        );

        ImGui::SliderFloat3(
            "Scale",
            (float*)&m_scale,
            0,
            GetWinWidth()
        );

        ImGui::SliderAngle(
            "Rotation",
            &m_rotation,
            0,
            360.0f
        );
    }
    ImGui::End();
}

const DirectX::SimpleMath::Matrix& RectTransform::GetWorld() const
{
    return m_world;
}

RECT RectTransform::GetRect() const
{
    using namespace DirectX::SimpleMath;

    const Vector2 screenSize(static_cast<float>(GetWinWidth()), static_cast<float>(GetWinHeight()));
    const Vector2 uiScale = GetUIScale();
    const Vector2 scaledSize = GetScaledSize();

    Vector2 position2D(m_position.x * uiScale.x, m_position.y * uiScale.y);
    position2D += Vector2(m_anchor.x * screenSize.x, m_anchor.y * screenSize.y);

    const Vector2 pivotOffset(m_pivot.x * scaledSize.x, m_pivot.y * scaledSize.y);
    Vector2 topLeft = position2D - pivotOffset;

    const DirectX::SimpleMath::Vector3 cameraPos = ConstantVector::ZeroVector3; //나중에 카메라 컴포넌트를 추가해라 //CameraManager::Get()->GetPosition();
    topLeft += Vector2(cameraPos.x, cameraPos.y);

    const Vector2 bottomRight = topLeft + scaledSize;

    RECT rect{};
    rect.left = static_cast<LONG>(topLeft.x);
    rect.top = static_cast<LONG>(topLeft.y);
    rect.right = static_cast<LONG>(bottomRight.x);
    rect.bottom = static_cast<LONG>(bottomRight.y);

    return rect;
}

DirectX::SimpleMath::Vector3 RectTransform::GetPosition() const
{
    return m_position;
}

DirectX::SimpleMath::Vector2 RectTransform::GetAnchoredPosition() const
{
    return DirectX::SimpleMath::Vector2(m_position.x, m_position.y);
}

DirectX::SimpleMath::Vector3 RectTransform::GetScale() const
{
    return m_scale;
}

float RectTransform::GetRotation() const
{
    return DirectX::XMConvertToDegrees(m_rotation);
}

DirectX::SimpleMath::Vector2 RectTransform::GetAnchor() const
{
    return m_anchor;
}

DirectX::SimpleMath::Vector2 RectTransform::GetPivot() const
{
    return m_pivot;
}

DirectX::SimpleMath::Vector2 RectTransform::GetSize() const
{
    return m_size;
}

DirectX::SimpleMath::Vector2 RectTransform::GetScaledSize() const
{
    const DirectX::SimpleMath::Vector2 uiScale = GetUIScale();
    return DirectX::SimpleMath::Vector2(m_size.x * uiScale.x, m_size.y * uiScale.y);
}

void RectTransform::SetPosition(const DirectX::SimpleMath::Vector3& pos)
{
    m_position = pos;
}

void RectTransform::SetAnchoredPosition(const DirectX::SimpleMath::Vector2& pos)
{
    m_position.x = pos.x;
    m_position.y = pos.y;
}

void RectTransform::SetScale(const DirectX::SimpleMath::Vector3& sca)
{
    m_scale = sca;
}

void RectTransform::SetRotation(float degrees)
{
    m_rotation = DirectX::XMConvertToRadians(degrees);
}

void RectTransform::SetAnchor(const DirectX::SimpleMath::Vector2& value)
{
    m_anchor.x = clamp(value.x, kMinAnchor, kMaxAnchor);
    m_anchor.y = clamp(value.y, kMinAnchor, kMaxAnchor);
}

void RectTransform::SetPivot(const DirectX::SimpleMath::Vector2& value)
{
    m_pivot.x = clamp(value.x, kMinAnchor, kMaxAnchor);
    m_pivot.y = clamp(value.y, kMinAnchor, kMaxAnchor);
}

void RectTransform::SetSize(const DirectX::SimpleMath::Vector2& value)
{
    m_size.x = max(0.0f, value.x);
    m_size.y = max(0.0f, value.y);
}

void RectTransform::Move(const float& dt, const DirectX::SimpleMath::Vector3& speed)
{
    m_position += speed * dt;
}

void RectTransform::Scale(const float& dt, const DirectX::SimpleMath::Vector3& value)
{
    m_scale += value * dt;
}

void RectTransform::Rotate(const float& dt, float degrees)
{
    m_rotation += DirectX::XMConvertToRadians(degrees) * dt;
}