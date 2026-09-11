#pragma once

#include <cstdint>
#include <functional>
#include <string_view>

#include "AudioBuffer.h"

// Application-facing Abstraction
using TTSVoiceId = uint32_t;
using TTSJobId = uint64_t;

enum class TTSSynthesisResult
{
    Success,
    Failed,
    Cancelled
};

using TTSSynthesisCallback = 
    std::function<
        void(
            TTSJobId jobId,
            TTSSynthesisResult result,
            AudioBuffer audio
        )
    >;

class ITTSEngine
{
public:
    // Destructor
    virtual ~ITTSEngine() = default;

    // Methods
    virtual bool Initialize() = 0;

    virtual TTSJobId SynthesizeAsync(
        TTSVoiceId voiceId,
        std::string_view text,
        TTSSynthesisCallback callback
    ) = 0;

    virtual bool Cancel(TTSJobId jobId) = 0;
};