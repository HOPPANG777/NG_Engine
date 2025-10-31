#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

namespace NG_Engine
{
	class D3D11Device;

	class IndexBuffer
	{
	public:
		void Create(const std::vector<UINT>& indices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);
		void SetIA();

	public:
		ID3D11Buffer* GetResource() const { return m_buffer.Get(); }
		UINT GetStride() const { return m_stride; }
		UINT GetOffset() const { return m_offset; }
		UINT GetCount() const { return m_count; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		UINT m_stride = 0;
		UINT m_offset = 0;
		UINT m_count = 0;

	};
} // namespace NG_Engine



