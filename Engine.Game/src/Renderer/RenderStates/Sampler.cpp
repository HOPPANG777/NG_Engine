#include "Renderer/RenderStates/Sampler.h"
#include "Common.h"
#include "Engine.Graphics/include/DX/D3D11Device.h"

#include <cfloat>
#include <cstring>

using namespace NG_Engine;
using namespace Common;

D3D11_SAMPLER_DESC Sampler::s_desc{};

void Sampler::Init()
{
    CreateDesc();
}

void Sampler::GetDesc(D3D11_SAMPLER_DESC* desc)
{
    CHECK(desc);
    std::memcpy(desc, &s_desc, sizeof(D3D11_SAMPLER_DESC));
}

void Sampler::Create(const Graphics::DXDevice& dx, const D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state)
{
    CHECK(dx.device && state);
    const D3D11_SAMPLER_DESC* source = desc ? desc : &s_desc;
    HRESULT hr = dx.device->CreateSamplerState(source, state);
    CHECK(SUCCEEDED(hr));
}

void Sampler::CreateDesc()
{
    std::memset(&s_desc, 0, sizeof(D3D11_SAMPLER_DESC));
    s_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 기본 선형 필터
    s_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    s_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    s_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    s_desc.MipLODBias = 0.0f;
    s_desc.MaxAnisotropy = 1;
    s_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    s_desc.MinLOD = FLT_MIN;
    s_desc.MaxLOD = FLT_MAX;
}
