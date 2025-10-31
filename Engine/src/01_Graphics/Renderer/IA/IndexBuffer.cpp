#include "01_Graphics/DX/D3D11Device.h"
#include "IndexBuffer.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

void NG_Engine::IndexBuffer::Create(const std::vector<UINT>& indices, const D3D11_USAGE& usage)
{
	m_stride = sizeof(UINT);
	m_count = UINT(indices.size());

	D3D11_BUFFER_DESC desc = { 0 };

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = m_stride * m_count;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = indices.data();

	HRESULT hr = D3D11Device::GetDevice()->CreateBuffer(&desc, &subData, &m_buffer);
	CHECK(hr);
}

void NG_Engine::IndexBuffer::SetIA()
{
	D3D11Device::GetDC()->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
