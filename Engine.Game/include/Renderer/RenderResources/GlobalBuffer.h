#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <directxtk/SimpleMath.h>

namespace NG_Engine
{
	class WorldBuffer : public ConstantBuffer
	{
		// 16바이트 단위로 데이터 패딩을 해줘야 한다.
		struct WorldStruct
		{
			DirectX::SimpleMath::Matrix _world;
		};

		WorldStruct _data;

	public:
		WorldBuffer();

		void SetWorld(const DirectX::SimpleMath::Matrix& world);
	};

	class ViewProjectionBuffer : public ConstantBuffer
	{
		struct ViewProjStruct
		{
			DirectX::SimpleMath::Matrix _view;
			DirectX::SimpleMath::Matrix _proj;
		};

		ViewProjStruct _data;

	public:
		ViewProjectionBuffer();

		void SetView(const DirectX::SimpleMath::Matrix& view);
		void SetProjection(const DirectX::SimpleMath::Matrix& proj);
	};

	class ColorBuffer : public ConstantBuffer
	{
		struct ColorStruct
		{
			DirectX::SimpleMath::Color _color;
		};

		ColorStruct _data;

	public:
		ColorBuffer();

		// 색상 변환 값 설정 (예: 텍스처 색상에 곱해질 값)
		void SetColor(const DirectX::SimpleMath::Color& color);

		// 현재 설정된 색상 변환 값을 반환 (읽기 전용)
		DirectX::SimpleMath::Color GetColor() const;
	};

	class IndexNumBuffer : public ConstantBuffer
	{
		struct IndexStruct
		{
			UINT _index = 0;
			DirectX::SimpleMath::Vector3 _dummy = Values::ZeroVector3;
		};

		IndexStruct _data;

	public:
		IndexNumBuffer();

		void SetIndex(UINT index);
	};
}



