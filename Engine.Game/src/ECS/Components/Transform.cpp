#include "ECS/Components/Transform.h"
#include "ConstantVector.h"
#include "Screen.h"
#include "UI.h"
#include "Common.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace NG_Engine;
using namespace Common;
using namespace Screen;
using namespace UI;

void Transform::Update()
{
    // S, R, T 행렬 계산
    DirectX::SimpleMath::Matrix S = DirectX::XMMatrixScalingFromVector(m_scale);
    DirectX::SimpleMath::Matrix R = DirectX::XMMatrixRotationZ(m_rotation);
    DirectX::SimpleMath::Matrix T = DirectX::XMMatrixTranslationFromVector(m_position);

    m_world = S * R * T;
}

void Transform::Render()
{
    // Transform은 시각적으로 그릴 요소가 없으므로 보통 비웁니다.
    // (필요 시, 디버그용 기즈모 등으로 확장 가능)
}

void Transform::GUI()
{
    // ImGui를 이용해 UI를 만들어내는 기능과 코드들

    // ImGui 코드들을 이용해 UI를 만들기위해 먼저 실행해야하는 함수
    ImGui::Begin("Transform", &bOpen);
    {
        ImGui::Text("Transform");

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
    // ImGui의 UI만드는게 끝났다는것을 알려줄 함수
    ImGui::End();
}

DirectX::SimpleMath::Vector3 Transform::GetPosition() const
{
    return m_position;
}

DirectX::SimpleMath::Vector3 Transform::GetScale() const
{
    return m_scale;
}

float Transform::GetRotation() const
{
    // 내부는 라디안이지만, 외부엔 각도(도)로 반환
    return DirectX::XMConvertToDegrees(m_rotation);
}

const DirectX::SimpleMath::Matrix& Transform::GetWorld() const
{
    return m_world;
}

void Transform::SetPosition(const DirectX::SimpleMath::Vector3& pos)
{
    m_position = pos;
}

void Transform::SetScale(const DirectX::SimpleMath::Vector3& sca)
{
    m_scale = sca;
}

void Transform::SetRotation(float degrees)
{
    m_rotation = DirectX::XMConvertToRadians(degrees);
}

void Transform::Move(const float& dt, const DirectX::SimpleMath::Vector3& speed)
{
    m_position += speed * dt;
}

void Transform::Rotate(const float& dt, float degrees)
{
    m_rotation += DirectX::XMConvertToRadians(degrees) * dt;
}

void Transform::Scale(const float& dt, const DirectX::SimpleMath::Vector3& m_scale)
{
    this->m_scale += m_scale * dt;
}
