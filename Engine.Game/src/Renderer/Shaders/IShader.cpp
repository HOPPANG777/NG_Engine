#include <Windows.h>
#include <d3dcompiler.h>

#include "IShader.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

void IShader::CheckShaderError(const HRESULT& hr, ID3DBlob* error)
{
        if (FAILED(hr))
        {
                if (error)
                {
                        std::string message(
                                static_cast<const char*>(error->GetBufferPointer()),
                                error->GetBufferSize());
                        MessageBoxA(nullptr, message.c_str(), "Shader Error", MB_OK);
                }
                CHECK(false);
        }
}

void IShader::CompileShader(const std::wstring& path, const std::string& entryName, const std::string& profile, ID3DBlob** blob)
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
        CheckShaderError(hr, error.Get());
}