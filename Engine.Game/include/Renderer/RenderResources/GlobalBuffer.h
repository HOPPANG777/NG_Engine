#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <directxtk/SimpleMath.h>
#include "Renderer/RenderResources/ConstantBuffer.h"

namespace NG_Engine
{
	namespace Graphics
	{
		struct DXDevice;
	}

	class WorldBuffer : public ConstantBuffer
	{
		// 16바이트 단위로 데이터 패딩을 해줘야 한다.
		struct WorldStruct
		{
			DirectX::SimpleMath::Matrix m_world;
		};

		WorldStruct m_data;

	public:
		WorldBuffer(Graphics::DXDevice dx);

		void SetWorld(const DirectX::SimpleMath::Matrix& world);
	};

	class ViewProjectionBuffer : public ConstantBuffer
	{
		struct ViewProjStruct
		{
			DirectX::SimpleMath::Matrix m_view;
			DirectX::SimpleMath::Matrix m_proj;
		};

		ViewProjStruct m_data;

	public:
		ViewProjectionBuffer(Graphics::DXDevice dx);

		void SetView(const DirectX::SimpleMath::Matrix& view);
		void SetProjection(const DirectX::SimpleMath::Matrix& proj);
	};

	class ColorBuffer : public ConstantBuffer
	{
		struct ColorStruct
		{
			DirectX::SimpleMath::Color m_color;
		};

		ColorStruct m_data;

	public:
		ColorBuffer(Graphics::DXDevice dx);

		// 색상 변환 값 설정 (예: 텍스처 색상에 곱해질 값)
		void SetColor(const DirectX::SimpleMath::Color& color);

		// 현재 설정된 색상 변환 값을 반환 (읽기 전용)
		DirectX::SimpleMath::Color GetColor() const;
	};

	class IndexNumBuffer : public ConstantBuffer
	{
		struct IndexStruct
		{
			UINT m_index = 0;
			DirectX::SimpleMath::Vector3 m_dummy = DirectX::SimpleMath::Vector3::Zero;
		};

		IndexStruct m_data;

	public:
		IndexNumBuffer(Graphics::DXDevice dx);

		void SetIndex(UINT index);
	};
}



