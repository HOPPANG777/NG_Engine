#include "Renderer/IA/VertexBuffer.h"
#include "Engine.Graphics/include/DX/D3D11Device.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

template<typename T>
inline void VertexBuffer::Create(Graphics::DXDevice dx, const std::vector<T>& vertices, const D3D11_USAGE& usage)
{
	m_device = dx.device;
	m_context = dx.context;
	CHECK(m_device && m_context);

	_stride = sizeof(T);
	_count = UINT(vertices.size());

	D3D11_BUFFER_DESC desc = { 0 };

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _stride * _count;

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
	subData.pSysMem = vertices.data();

	HRESULT hr = m_device->CreateBuffer(&desc, &subData, &_buffer);
	CHECK(hr);
}

void VertexBuffer::SetIA() 
{
	m_context->IASetVertexBuffers(
		0, 
		1, 
		m_buffer.GetAddressOf(), 
		&m_stride, 
		&m_offset
	);
}

void VertexBuffer::Destroy()
{
	//m_buffer.Reset();      // ComPtr이 참조 해제
	m_device = nullptr;    // 비소유 포인터 초기화
	m_context = nullptr;
	//m_stride = 0;
	//m_offset = 0;
	//m_count = 0;
}
