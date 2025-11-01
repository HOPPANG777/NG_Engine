#pragma once
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>

#include <cstdint>
#include <vector>

namespace NG_Engine
{
    namespace Graphics
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
            bool Create(UINT width, UINT height, int preferredAdapter = -1);     // ⺻ , D3D11_CREATE_DEVICE_BGRA_SUPPORT(+DEBUG)
            void Destroy();

        public:
            ID3D11Device* GetDevice()  const { return m_device.Get(); }
            ID3D11DeviceContext* GetDC() const { return m_context.Get(); }

        public:
            struct OutputInfo
            {
                Microsoft::WRL::ComPtr<IDXGIOutput> output = nullptr;
                DXGI_OUTPUT_DESC desc{};
                UINT refreshNumerator = 60;
                UINT refreshDenominator = 1;
            };

            struct AdapterInfo
            {
                UINT ordinal = 0;
                Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = nullptr;
                DXGI_ADAPTER_DESC1 desc{};
                OutputInfo output{};
            };

            const std::vector<AdapterInfo>& GetAdapterInfos() const { return m_adapterInfos; }
            int GetSelectedAdapterIndex() const { return m_selectedAdapterIndex; }
            UINT GetSelectedRefreshNumerator() const { return m_selectedRefreshNumerator; }
            UINT GetSelectedRefreshDenominator() const { return m_selectedRefreshDenominator; }

        public:
            DXDevice GetDXDevice() const
            {
                return {
                    m_device.Get(),
                    m_context.Get()
                };
            }

        private:
            Microsoft::WRL::ComPtr<ID3D11Device>        m_device = nullptr;
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context = nullptr;

            std::vector<AdapterInfo> m_adapterInfos;
            int m_selectedAdapterIndex = -1;
            UINT m_selectedRefreshNumerator = 60;
            UINT m_selectedRefreshDenominator = 1;

        private:
            void EnumerateAdapters(UINT width, UINT height);
            void EnumerateAdapterOutput(AdapterInfo& info, UINT width, UINT height);
            int ChooseAdapterIndex(int preferred) const;
            void PrintAdapterSummary() const;
        };
    }
} // namespace NG_Engine
