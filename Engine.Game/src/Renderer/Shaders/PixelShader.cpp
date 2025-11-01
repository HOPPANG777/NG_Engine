#include "Graphics/DX/D3D11Device.h"
#include "PixelShader.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

	void PixelShader::Create(DXDevice dx, const std::wstring& path, const std::string& entryName) override
	{
		m_device = dx.device;
		m_context = dx.context;
		CHECK(m_device && m_context);

		this->m_path = path;
		this->m_entryName = entryName;

		CompileShader(path, entryName, "ps_5_0", m_blob.GetAddressOf());
		HRESULT hr = m_device->CreatePixelShader
		(
			m_blob->GetBufferPointer(),
			m_blob->GetBufferSize(),
			nullptr,
			&m_shader
		);
		CHECK(hr);
	}

	void PixelShader::Clear() override
	{
		m_shader.Reset();
		m_blob.Reset();
		m_device = nullptr;
		m_context = nullptr;
	}

	void PixelShader::SetShader() override
	{
		m_context->PSSetShader(m_shader.Get(), nullptr, 0);
	}