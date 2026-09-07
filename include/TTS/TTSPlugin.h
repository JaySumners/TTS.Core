#pragma once

#include <cstdint>

// Platform Binding
#ifdef TTS_PLUGIN_BUILD
    // Windows
    #ifdef _WIN32
        #define TTS_PLUGIN_API __declspec(dllexport)
    // Room for Expansion
    #else
        #define TTS_PLUGIN_API
    #endif
#else
    #define TTS_PLUGIN_API
#endif



// DLL ABI
// This is the DLL Contract.
// It defines the binary interface between an 
// executable and the TTS Plugin.

// C-Compatible Interface
// C-Style symbols only
extern "C"
{
    // Forward Declarations

    /// <summary>
    /// Forward declared handler.
    /// </summary>
    struct TTSPluginHandle;

    // Enums

    /// <summary>
    /// Result
    /// Provides a meaningful result to async synthesis operations.
    /// </summary>
    enum TTSResult : int32_t
    {
        TTS_SUCCESS = 0,
        TTS_FAILED = 1,
        TTS_CANCELLED = 2
    };

    // Structs and Classes

    /// <summary>
    /// Audio Buffer
    /// Translation of sythesized audio from plugin to host.
    /// </summary>
    struct TTSAudioBuffer
    {
        const float* samples;
        uint32_t sampleCount;
        uint32_t sampleRate;
        uint16_t channels;
    };

    // Usings

    using TTSVoiceId = uint32_t;
    using TTSJobId = uint64_t;

    /// <summary>
    /// Synthesis Callback
    /// A function signature for callbacks.
    /// </summary>
    using TTSPluginSynthesisCallback =
        void (*)(
            TTSPluginHandle* handle,
            TTSJobId jobId,
            TTSResult result,
            const TTSAudioBuffer* audio,
            const void* userData
        );

    // Exported Functions

    /// <summary>
    /// Create
    /// Creates internal TTS instance.
    /// </summary>
    /// <returns>A plugin handle reference.</returns>
    TTS_PLUGIN_API TTSPluginHandle* TTS_Create();

    /// <summary>
    /// Destroy
    /// Destroys whatever Create created.
    /// </summary>
    /// <param name="handle"></param>
    /// <returns></returns>
    TTS_PLUGIN_API void TTS_Destroy(
        TTSPluginHandle* handle
    );

    /// <summary>
    /// Synthesize Audio
    /// Sends out an async job that synthesizes audio.
    /// </summary>
    /// <param name="handle">A TTS Plugin Handle pointer</param>
    /// <param name="voiceId">A TTS Voice ID</param>
    /// <param name="text">The text to synthesize.</param>
    /// <param name="callback">A Plugin Synthesis Callback</param>
    /// <param name="userData">A user data void pointer used by the callback.</param>
    /// <returns></returns>
    TTS_PLUGIN_API TTSJobId TTS_SynthesizeAsync(
        TTSPluginHandle* handle,
        TTSVoiceId voiceId,
        const char* text,
        TTSPluginSynthesisCallback callback,
        void* userData
    );

    /// <summary>
    /// Cancel
    /// Cancels an async synthesize job.
    /// </summary>
    /// <param name="handle">A TTS Plugin Handle pointer</param>
    /// <param name="jobId">A TTS Job ID</param>
    /// <returns>Success or Failure</returns>
    TTS_PLUGIN_API bool TTS_Cancel(
        TTSPluginHandle* handle,
        TTSJobId jobId
    );

   
    /// <summary>
    /// Free Audio
    /// Releases audio returned by a synthesis callback
    /// The host must call this after it has finished consuming the
    /// audio buffer. The plugin may treat this as a no-op when the
    /// audio buffer is only valid for the duration of the callback.
    /// </summary>
    /// <param name="handle">A TTS Plugin Handle pointer</param>
    /// <param name="audio">A TTS Audio Buffer pointer to free.</param>
    /// <returns></returns>
    TTS_PLUGIN_API void TTS_FreeAudio(
        TTSPluginHandle* handle,
        const TTSAudioBuffer* audio
    );

    /// <summary>
    /// Get Voice Count
    /// </summary>
    /// <param name="handle">A TTS Plugin Handle pointer</param>
    /// <returns>The count of available voices in the TTS Engine.</returns>
    TTS_PLUGIN_API uint32_t TTS_GetVoiceCount(
        TTSPluginHandle* handle
    );
}