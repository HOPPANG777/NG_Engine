#pragma once

#include <directxtk/SimpleMath.h>
#include <imgui.h>

#include "Component.h"
#include "ConstantVector.h"

class Transform : public Component
{
public:
	Transform(const std::string& name = "Transform") : Component(name) {}
	virtual ~Transform() override = default;

public:
	// Component을(를) 통해 상속됨
	void Awake() override {}
	void Start() override {}

	void Destroy() override {}

	void FixedUpdate() override {}
	void Update() override;
	void LateUpdate() override {}

	void Render() override;
	void PostRender() override {}

	void GUI() override;

	const DirectX::SimpleMath::Matrix& GetWorld() const;

public:
	// Getter
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetScale() const;
	float GetRotation() const;

public:
	// Setter
	void SetPosition(const DirectX::SimpleMath::Vector3& pos);
	void SetScale(const DirectX::SimpleMath::Vector3& scale);
	void SetRotation(float degrees);

public:
	// 이동/회전/스케일 변화를 DELTA 기반으로 적용할 수 있는 함수들
	void Move(const float& dt, const DirectX::SimpleMath::Vector3& speed);
	void Scale(const float& dt, const DirectX::SimpleMath::Vector3& scale);
	void Rotate(const float& dt, float degrees);

private:
	DirectX::SimpleMath::Vector3 m_position = ConstantVector::ZeroVector3;
	DirectX::SimpleMath::Vector3 m_scale = ConstantVector::ZeroVector3;
	float m_rotation = 0.0f;	// 라디안(radian)으로 보관

	DirectX::SimpleMath::Matrix m_world = DirectX::XMMatrixIdentity();	// S * R * T 결과

	bool bOpen = false;
};

