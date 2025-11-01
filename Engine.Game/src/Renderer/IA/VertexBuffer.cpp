#include "Graphics/DX/D3D11Device.h"
#include "VertexBuffer.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

void VertexBuffer::SetIA(DXDevice dx)
{
    ID3D11DeviceContext* context = dx.context ? dx.context : m_context;
    CHECK(context);

    ID3D11Buffer* buffer = m_buffer.Get();
    CHECK(buffer);

    context->IASetVertexBuffers(
        0,
        1,
        &buffer,
        &m_stride,
        &m_offset);
}

void VertexBuffer::Destroy()
{
    m_buffer.Reset();
    m_device = nullptr;
    m_context = nullptr;
    m_stride = 0;
    m_offset = 0;
    m_count = 0;
}
