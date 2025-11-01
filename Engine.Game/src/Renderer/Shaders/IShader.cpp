#include "IShader.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;
using namespace Microsoft;
using namespace WRL;

void IShader::CheckShaderError(const HRESULT& hr, ID3DBlob* error)
{
	if (FAILED(hr))
	{
		if (error)
		{
			const std::string& str = (const char*)error->GetBufferPointer();
			MessageBoxA(nullptr, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}
}

void IShader::CompileShader(const wstring& path, const string& entryName, const string& profile, ID3DBlob** blob)
{
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	HRESULT hr = D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		nullptr,
		entryName.c_str(),
		profile.c_str(),
		0,
		0,
		blob,
		&error
	);
	CHECK(hr);
}