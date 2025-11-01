#include "Engine.Graphics/include/DX/D3D11Device.h"
#include "Renderer/RenderResources/GlobalBuffer.h"
#include "Renderer/RenderResources/ConstantBuffer.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace NG_Engine;

WorldBuffer::WorldBuffer(Graphics::DXDevice dx)
	: ConstantBuffer(dx, &m_data, sizeof(m_data))
{

}

void WorldBuffer::SetWorld(const DirectX::SimpleMath::Matrix& world)
{
	m_data.m_world = DirectX::XMMatrixTranspose(world);
}



ViewProjectionBuffer::ViewProjectionBuffer(Graphics::DXDevice dx)
	: ConstantBuffer(dx, &m_data, sizeof(m_data))
{

}

void ViewProjectionBuffer::SetView(const DirectX::SimpleMath::Matrix& view)
{
	m_data.m_view = DirectX::XMMatrixTranspose(view);
}

void ViewProjectionBuffer::SetProjection(const DirectX::SimpleMath::Matrix& proj)
{
	m_data.m_proj = DirectX::XMMatrixTranspose(proj);
}


ColorBuffer::ColorBuffer(Graphics::DXDevice dx)
	: ConstantBuffer(dx, &m_data, sizeof(m_data))
{
	// 기본 변환 색상은 흰색으로 설정 (즉, 색상 변화 없음)
	m_data.m_color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
}

// 색상 변환 값 설정 (예: 텍스처 색상에 곱해질 값)
void ColorBuffer::SetColor(const DirectX::SimpleMath::Color& color)
{
	m_data.m_color = color;
}

// 현재 설정된 색상 변환 값을 반환 (읽기 전용)
DirectX::SimpleMath::Color ColorBuffer::GetColor() const
{
	return m_data.m_color;
}



IndexNumBuffer::IndexNumBuffer(Graphics::DXDevice dx)
	: ConstantBuffer(dx, &m_data, sizeof(m_data))
{
	m_data.m_index = 0;
}

void IndexNumBuffer::SetIndex(UINT index)
{
	m_data.m_index = index;
}

