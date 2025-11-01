#include "Renderer/RenderStates/Rasterizer.h"
#include "Common.h"
#include "Engine.Graphics/include/DX/D3D11Device.h"

#include <cstring>

using namespace NG_Engine;
using namespace Common;

D3D11_RASTERIZER_DESC Rasterizer::s_desc{};

void Rasterizer::Init()
{
    CreateDesc();
}

void Rasterizer::GetDesc(D3D11_RASTERIZER_DESC* desc)
{
    CHECK(desc);
    std::memcpy(desc, &s_desc, sizeof(D3D11_RASTERIZER_DESC));
}

void Rasterizer::Create(const Graphics::DXDevice& dx, const D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state)
{
    CHECK(dx.device && state);
    const D3D11_RASTERIZER_DESC* source = desc ? desc : &s_desc;
    HRESULT hr = dx.device->CreateRasterizerState(source, state);
    CHECK(SUCCEEDED(hr));
}

void Rasterizer::CreateDesc()
{
    std::memset(&s_desc, 0, sizeof(D3D11_RASTERIZER_DESC));
    s_desc.AntialiasedLineEnable = FALSE;       // 안티알리아싱
    s_desc.CullMode = D3D11_CULL_BACK;          // 뒷면 컬링
    s_desc.DepthBias = 0;                       // 깊이 바이어스
    s_desc.DepthBiasClamp = 0.0f;               // 깊이 바이어스 클램프
    s_desc.FillMode = D3D11_FILL_SOLID;         // 채우기 모드
    s_desc.FrontCounterClockwise = FALSE;       // 정면 시계 반대 여부
    s_desc.MultisampleEnable = FALSE;           // 멀티샘플링
    s_desc.ScissorEnable = FALSE;               // 시저
    s_desc.SlopeScaledDepthBias = 0.0f;         // 기울기 기반 깊이 바이어스
}
