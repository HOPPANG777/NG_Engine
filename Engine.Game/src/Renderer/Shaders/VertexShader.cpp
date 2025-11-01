#include "Renderer/Shaders/VertexShader.h"
#include "Renderer/Shaders/IShader.h"
#include "Engine.Graphics/include/DX/D3D11Device.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

void VertexShader::Create(Graphics::DXDevice dx, const std::wstring& path, const std::string& entryName)
{
	m_device = dx.device;
	m_context = dx.context;
	CHECK(m_device && m_context);

	this->m_path = path;
	this->m_entryName = entryName;

	CompileShader(path, entryName, "vs_5_0", m_blob.GetAddressOf());
	HRESULT hr = m_device->CreateVertexShader
	(
		m_blob->GetBufferPointer(),
		m_blob->GetBufferSize(),
		nullptr,
		m_shader.GetAddressOf()
	);
	CHECK(SUCCEEDED(hr));
}

void VertexShader::Clear()
{
	m_shader.Reset();
	m_blob.Reset();
	m_device = nullptr;
	m_context = nullptr;
}

void VertexShader::SetShader()
{
	m_context->VSSetShader(m_shader.Get(), nullptr, 0);
}