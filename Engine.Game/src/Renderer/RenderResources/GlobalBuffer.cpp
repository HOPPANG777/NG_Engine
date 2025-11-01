#include "GlobalBuffer.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace NG_Engine;

WorldBuffer::WorldBuffer()
    : ConstantBuffer(&_data, sizeof(_data))
{
}

void WorldBuffer::SetWorld(const DirectX::SimpleMath::Matrix& world)
{
    _data._world = DirectX::XMMatrixTranspose(world);
}

ViewProjectionBuffer::ViewProjectionBuffer()
    : ConstantBuffer(&_data, sizeof(_data))
{
}

void ViewProjectionBuffer::SetView(const DirectX::SimpleMath::Matrix& view)
{
    _data._view = DirectX::XMMatrixTranspose(view);
}

void ViewProjectionBuffer::SetProjection(const DirectX::SimpleMath::Matrix& proj)
{
    _data._proj = DirectX::XMMatrixTranspose(proj);
}

ColorBuffer::ColorBuffer()
    : ConstantBuffer(&_data, sizeof(_data))
{
    _data._color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
}

void ColorBuffer::SetColor(const DirectX::SimpleMath::Color& color)
{
    _data._color = color;
}

DirectX::SimpleMath::Color ColorBuffer::GetColor() const
{
    return _data._color;
}

IndexNumBuffer::IndexNumBuffer()
    : ConstantBuffer(&_data, sizeof(_data))
{
    _data._index = 0;
}

void IndexNumBuffer::SetIndex(UINT index)
{
    _data._index = index;
}
