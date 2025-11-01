#include "EngineAPI.h"

namespace
{
    constexpr const char kVersionString[] = ENGINE_VERSION_STRING;
}

std::uint32_t ENGINE_CALL Engine_GetVersionInt()
{
    return ENGINE_VERSION_INT;
}

const char* ENGINE_CALL Engine_GetVersionString()
{
    return kVersionString;
}

Engine::Status ENGINE_CALL Engine_Initialize()
{
    return Engine::Status::OK;
}

void ENGINE_CALL Engine_Shutdown()
{
}
