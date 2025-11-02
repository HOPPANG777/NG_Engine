#pragma once

#include <Windows.h>

namespace NG_Engine
{
    namespace Graphics
    {
        struct GraphicsInitDesc
        {
            HWND window = nullptr;
            UINT width = 0;
            UINT height = 0;
            bool vsync = false;
            int adapterIndex = -1;
        };

        class IGraphicsEngine
        {
        public:
            virtual ~IGraphicsEngine() = default;

            virtual bool Init(const GraphicsInitDesc& desc) = 0;
            virtual void Shutdown() = 0;

            virtual void BeginFrame(const float clearColor[4]) = 0;
            virtual void EndFrame(bool vsync) = 0;

            virtual void Resize(UINT width, UINT height) = 0;
        };
    }
}