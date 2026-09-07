#pragma once

#include <filesystem>

// Generic configuration pass to TTS Implementation.
struct TTSConfig
{
    std::filesystem::path resourceDirectory;
};