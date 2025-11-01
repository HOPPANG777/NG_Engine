#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

namespace NG_Engine
{
	namespace Graphics
	{
		struct DXDevice;
	}

	class ConstantBuffer
	{
	public:
		void SetVSBuffer(const UINT slot);
		void SetPSBuffer(const UINT slot);

	protected:
		ConstantBuffer(Graphics::DXDevice dx, void* data, const UINT dataSize);
		~ConstantBuffer() {
			Destroy();
		}
		
		void Destroy();

	private:
		void MapData(void* data, const UINT dataSize);

	private:
		ID3D11Device* m_device = nullptr;           // 비소유
		ID3D11DeviceContext* m_context = nullptr;   // 비소유

		D3D11_BUFFER_DESC m_desc = { 0 };
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

		void* m_data;
		UINT m_dataSize;

	};
}


