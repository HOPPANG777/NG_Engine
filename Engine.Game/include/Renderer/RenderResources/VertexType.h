#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <directxtk/SimpleMath.h>

// C++의 struct는 기본 접근 제어가 public이라는 차이점이 C의 struct와의 유일한 차이점이다
struct VertexColor
{
	DirectX::SimpleMath::Vector2 _position;
	DirectX::SimpleMath::Color _color;

	VertexColor() = default;
	VertexColor(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Color& color)
		: _position(position), _color(color)
	{

	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> ieDescs;
};



struct VertexTexture
{
	DirectX::SimpleMath::Vector3 _position;
	DirectX::SimpleMath::Vector2 _uv;

	VertexTexture() = default;
	VertexTexture(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector2& uv)
		: _position(position), _uv(uv)
	{

	}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> ieDescs;
};

