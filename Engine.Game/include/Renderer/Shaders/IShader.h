#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <algorithm>
#include <vector>


namespace NG_Engine
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Create(DXDevice dx, const std::wstring& path, const std::string& entryName) = 0;

		virtual void Clear() = 0;
		virtual void SetShader() = 0;

	public:
		ID3DBlob* GetBlob() const { return m_blob.Get(); }

	private:
		void CheckShaderError(const HRESULT& hr, ID3DBlob* error);

	protected:
		void CompileShader(const std::wstring& path, const std::string& entryName, const std::string& profile, ID3DBlob** blob);

	protected:
		std::wstring m_path = L"";
		std::string m_entryName = "";
		Microsoft::WRL::ComPtr<ID3DBlob> m_blob;

	};
}


