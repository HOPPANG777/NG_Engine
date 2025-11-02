#pragma once

#include <directxtk/SimpleMath.h>

namespace ConstantVector
{
	// ------------------ Vector2 ------------------
	constexpr DirectX::SimpleMath::Vector2 ZeroVector2{ 0.0f, 0.0f};
	constexpr DirectX::SimpleMath::Vector2 OneVector2{ 1.0f, 1.0f};
	constexpr DirectX::SimpleMath::Vector2 uvLT{ 0.0f, 0.0f};
	constexpr DirectX::SimpleMath::Vector2 uvLB{ 0.0f, 1.0f};
	constexpr DirectX::SimpleMath::Vector2 uvRT{ 1.0f, 0.0f};
	constexpr DirectX::SimpleMath::Vector2 uvRB{ 1.0f, 1.0f};

	// ------------------ Vector3 ------------------
	constexpr DirectX::SimpleMath::Vector3 ZeroVector3{ 0.0f, 0.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 OneVector3{ 1.0f, 1.0f, 1.0f };
	constexpr DirectX::SimpleMath::Vector3 UnitX{ 1.0f, 0.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 UnitY{ 0.0f, 1.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 UnitZ{ 0.0f, 0.0f, 1.0f };
	constexpr DirectX::SimpleMath::Vector3 Up{ 0.0f, 1.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Down{ 0.0f, -1.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Right{ 1.0f, 0.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Left{ -1.0f, 0.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Forward{ 0.0f, 0.0f, 1.0f };
	constexpr DirectX::SimpleMath::Vector3 Backward{ 0.0f, 0.0f, -1.0f };

}