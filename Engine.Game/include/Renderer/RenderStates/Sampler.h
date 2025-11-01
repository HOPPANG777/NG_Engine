#pragma once

#include <d3d11.h>

namespace NG_Engine
{
    namespace Graphics
    {
        struct DXDevice;
    }

    class Sampler
    {
    public:
        static void Init();
        static void GetDesc(D3D11_SAMPLER_DESC* desc);
        static void Create(const Graphics::DXDevice& dx, const D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state);

    private:
        static void CreateDesc();

    private:
        static D3D11_SAMPLER_DESC s_desc;
    };
}
