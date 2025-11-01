#include "Renderer/RenderStates/Blend.h"
#include "Common.h"
#include "Engine.Graphics/include/DX/D3D11Device.h"

#include <cstring>

using namespace NG_Engine;
using namespace Common;

D3D11_BLEND_DESC Blend::s_desc{};

void Blend::Init()
{
    CreateDesc();
}

void Blend::GetDesc(D3D11_BLEND_DESC* desc)
{
    CHECK(desc);
    std::memcpy(desc, &s_desc, sizeof(D3D11_BLEND_DESC));
}

void Blend::Create(const Graphics::DXDevice& dx, const D3D11_BLEND_DESC* desc, ID3D11BlendState** state)
{
    CHECK(dx.device && state);
    const D3D11_BLEND_DESC* source = desc ? desc : &s_desc;
    HRESULT hr = dx.device->CreateBlendState(source, state);
    CHECK(SUCCEEDED(hr));
}

void Blend::CreateDesc()
{
    std::memset(&s_desc, 0, sizeof(D3D11_BLEND_DESC));
    s_desc.AlphaToCoverageEnable = FALSE;         // Alpha To Coverage
    s_desc.IndependentBlendEnable = FALSE;        // Independent Blend

    auto& target = s_desc.RenderTarget[0];
    target.BlendEnable = FALSE;                   // 기본 비활성화
    target.SrcBlend = D3D11_BLEND_SRC_ALPHA;      // SRC 알파
    target.DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 역 SRC 알파
    target.BlendOp = D3D11_BLEND_OP_ADD;

    target.SrcBlendAlpha = D3D11_BLEND_ZERO;      // SRC 알파 채널
    target.DestBlendAlpha = D3D11_BLEND_ONE;      // DST 알파 채널
    target.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    target.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}
