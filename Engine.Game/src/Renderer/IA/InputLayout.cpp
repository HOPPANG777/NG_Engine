#include "Renderer/IA/InputLayout.h"
#include "Engine.Graphics/include/DX/D3D11Device.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

void InputLayout::Create(Graphics::DXDevice dx, const D3D11_INPUT_ELEMENT_DESC* desc, const size_t& count, ID3DBlob* blob)
{
	m_device = dx.device;
	m_context = dx.context;
	CHECK(m_device && m_context);

	HRESULT hr = m_device->CreateInputLayout(
		desc,
		UINT(count),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&m_inputLayout
	);
	CHECK(SUCCEEDED(hr));
}

void InputLayout::SetIA()
{
	m_context->IASetInputLayout(m_inputLayout.Get());
}

void InputLayout::Destroy()
{
	//m_inputLayout.Reset();      // ComPtr이 참조 해제
	m_device = nullptr;    // 비소유 포인터 초기화
	m_context = nullptr;
}