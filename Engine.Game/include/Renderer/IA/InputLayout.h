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

	class InputLayout
	{
	public:
		~InputLayout() {
			Destroy();
		}

		void Create(Graphics::DXDevice dx, const D3D11_INPUT_ELEMENT_DESC* desc, const size_t& count, ID3DBlob* blob);
		void SetIA();

		void Destroy();

	public:
		ID3D11InputLayout* GetResource() const { return m_inputLayout.Get(); }

	private:
		ID3D11Device* m_device = nullptr;           // 비소유
		ID3D11DeviceContext* m_context = nullptr;   // 비소유

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	};
} // namespace NG_Engine



