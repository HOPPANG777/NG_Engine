#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <directxtk/SimpleMath.h>

#include "Renderer/RenderResources/ConstantBuffer.h"

namespace NG_Engine
{
    class WorldBuffer : public ConstantBuffer
    {
        struct WorldStruct
        {
            DirectX::SimpleMath::Matrix _world;
        };

        WorldStruct _data{};

    public:
        WorldBuffer();

        void SetWorld(const DirectX::SimpleMath::Matrix& world);
    };

    class ViewProjectionBuffer : public ConstantBuffer
    {
        struct ViewProjStruct
        {
            DirectX::SimpleMath::Matrix _view;
            DirectX::SimpleMath::Matrix _proj;
        };

        ViewProjStruct _data{};

    public:
        ViewProjectionBuffer();

        void SetView(const DirectX::SimpleMath::Matrix& view);
        void SetProjection(const DirectX::SimpleMath::Matrix& proj);
    };

    class ColorBuffer : public ConstantBuffer
    {
        struct ColorStruct
        {
            DirectX::SimpleMath::Color _color;
        };

        ColorStruct _data{};

    public:
        ColorBuffer();

        void SetColor(const DirectX::SimpleMath::Color& color);
        DirectX::SimpleMath::Color GetColor() const;
    };

    class IndexNumBuffer : public ConstantBuffer
    {
        struct IndexStruct
        {
            UINT _index = 0;
            DirectX::SimpleMath::Vector3 _dummy = DirectX::SimpleMath::Vector3::Zero;
        };

        IndexStruct _data{};

    public:
        IndexNumBuffer();

        void SetIndex(UINT index);
    };
}
