#pragma once

#include <cstdint>
#include <vector>

// C++ representation used inside injected project.
struct AudioBuffer
{
    std::vector<float> samples;
    uint32_t sampleRate = 0;
    uint16_t channels = 1;
};