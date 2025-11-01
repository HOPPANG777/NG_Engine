#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <algorithm>
#include <vector>

namespace NG_Engine
{
	namespace Graphics
	{
		struct DXDevice;
	}

	class PixelShader : public IShader
	{
	public:
		void Create(Graphics::DXDevice dx, const std::wstring& path, const std::string& entryName) override;
		void Clear() override;

		void SetShader() override;

	public:
		ID3D11PixelShader* GetResource() const { return m_shader.Get(); }

	private:
		ID3D11Device* m_device = nullptr;           // 비소유
		ID3D11DeviceContext* m_context = nullptr;   // 비소유

		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;

	};
}


