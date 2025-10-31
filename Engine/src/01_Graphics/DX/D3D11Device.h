#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace NG_Engine
{
    struct DXDevice
    {
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;
    };

    class D3D11Device
    {
    public:
        D3D11Device() = default;
        ~D3D11Device() { Destroy(); }

    public:
        bool Create();     // ±âº» ¾î´ðÅÍ, D3D11_CREATE_DEVICE_BGRA_SUPPORT(+DEBUG)
        void Destroy();

    public:
        ID3D11Device* GetDevice()  const { return m_device.Get(); }
        ID3D11DeviceContext* GetDC() const { return m_context.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Device>        m_device = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context = nullptr;
    };
} // namespace NG_Engine
