#include "Graphics/DX/D3D11Device.h"
#include "ConstantBuffer.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

ConstantBuffer::ConstantBuffer(DXDevice dx, void* data, const UINT dataSize)
	: m_data(data), m_dataSize(dataSize)
{
	m_device = dx.device;
	m_context = dx.context;
	CHECK(m_device && m_context);

	m_desc.Usage = D3D11_USAGE_DYNAMIC;
	m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_desc.ByteWidth = dataSize;
	m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&desc, nullptr, &m_buffer);
	CHECK(hr);
}

void ConstantBuffer::Destroy()
{
	//m_buffer.Reset();
	m_device = nullptr;           // 비소유
	m_context = nullptr;		  // 비소유
	//m_desc = { 0 };
	//m_data = nullptr;
	//m_dataSize = 0;
}

void ConstantBuffer::SetVSBuffer(DXDevice dx, const UINT slot)
{
	MapData(m_data, m_dataSize);

	m_context->VSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
}

void ConstantBuffer::SetPSBuffer(DXDevice dx, const UINT slot)
{
	MapData(m_data, m_dataSize);

	m_context->PSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
}

void ConstantBuffer::MapData(DXDevice dx, void* data, const UINT dataSize)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	m_context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	{
		memcpy(mappedSubResource.pData, data, dataSize);
	}
	m_context->Unmap(m_buffer.Get(), 0);
}