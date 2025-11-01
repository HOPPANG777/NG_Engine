#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

namespace NG_Engine
{
	class ConstantBuffer
	{
	public:


	private:
		ID3D11Device* m_device = nullptr;           // 비소유
		ID3D11DeviceContext* m_context = nullptr;   // 비소유

		D3D11_BUFFER_DESC m_desc = { 0 };

	};
}


