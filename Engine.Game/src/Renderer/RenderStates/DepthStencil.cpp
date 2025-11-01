#include "DepthStencil.h"
#include "Common.h"
#include "Graphics/DX/D3D11Device.h"

#include <cstring>

using namespace NG_Engine;
using namespace Common;

D3D11_DEPTH_STENCIL_DESC DepthStencil::s_desc{};

void DepthStencil::Init()
{
    CreateDesc();
}

void DepthStencil::GetDesc(D3D11_DEPTH_STENCIL_DESC* desc)
{
    CHECK(desc);
    std::memcpy(desc, &s_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
}

void DepthStencil::Create(const Graphics::DXDevice& dx, const D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state)
{
    CHECK(dx.device && state);
    const D3D11_DEPTH_STENCIL_DESC* source = desc ? desc : &s_desc;
    HRESULT hr = dx.device->CreateDepthStencilState(source, state);
    CHECK(SUCCEEDED(hr));
}

void DepthStencil::CreateDesc()
{
    std::memset(&s_desc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

    s_desc.DepthEnable = TRUE;                              // 깊이 테스트 활성화
    s_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;     // 깊이 버퍼에 기록
    s_desc.DepthFunc = D3D11_COMPARISON_LESS;               // Less 비교

    s_desc.StencilEnable = TRUE;                            // 스텐실 사용
    s_desc.StencilReadMask = 0xFF;                          // 읽기 마스크
    s_desc.StencilWriteMask = 0xFF;                         // 쓰기 마스크

    s_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    s_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    s_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    s_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    s_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    s_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    s_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    s_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}
