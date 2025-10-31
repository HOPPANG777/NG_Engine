#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

namespace NG_Engine
{
	struct DXDevice
	{
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
	};

	class InputLayout
	{
	public:
		void Create(DXDevice dx, const size_t& count, ID3DBlob* blob);
		void SetIA(DXDevice dx);

	public:
		ID3D11InputLayout* GetResource() const { return m_inputLayout.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	};
} // namespace NG_Engine



