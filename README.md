# TTS.Core

A generic, implementation-independent C-compatible plugin contract for text-to-speech engines.

## Overview

TTS.Core defines the binary interface between an application and a dynamically loaded text-to-speech (TTS) implementation.

It provides a small, stable API for:

* Creating and destroying a TTS plugin instance
* Submitting asynchronous synthesis requests
* Cancelling synthesis jobs
* Receiving synthesized audio through callbacks
* Managing audio buffer ownership
* Querying available voices

TTS.Core contains **no TTS engine implementation**. It is intended to be implemented by independent TTS plugins and consumed by applications that want to support multiple TTS engines without linking directly against a specific implementation.

## Architecture

```text
                 TTS.Core
              Plugin Contract
                    ▲
                    │
        ┌───────────┼───────────┐
        │           │           │
        │           │           │
   TTS.Kokoro   TTS.Piper   Other TTS
        │           │           │
        └───────────┴───────────┘
                    │
                    ▼
                 Host App
```

A host application loads a TTS implementation at runtime and communicates with it exclusively through the TTS.Core contract.

The host does not need to know which TTS engine is being used.

## Design Goals

TTS.Core is designed around a few principles:

### Implementation independent

The contract does not depend on a particular TTS engine, model, phonemizer, UI framework, simulator, or application.

### C-compatible ABI

The plugin boundary uses C-compatible types and exported functions to avoid exposing C++ ABI details across the DLL boundary.

The interface intentionally avoids passing types such as:

* `std::string`
* `std::vector`
* `std::filesystem::path`
* C++ classes
* C++ exceptions

across the plugin boundary.

### Asynchronous synthesis

Speech synthesis is exposed as an asynchronous operation. A synthesis request returns a job identifier, and completion is reported through a callback.

### Explicit ownership

Audio returned by a plugin has an explicit lifetime and release mechanism. The host is responsible for following the ownership rules defined by the API.

### Small contract

TTS.Core is intended to remain a contract rather than becoming a general-purpose TTS framework.

Engine-specific functionality should remain inside the implementation.

## Public API

The primary public header is:

```text
include/TTS/TTSPlugin.h
```

The API currently provides:

```cpp
TTS_Create()
TTS_Destroy()

TTS_SynthesizeAsync()
TTS_Cancel()

TTS_FreeAudio()

TTS_GetVoiceCount()
```

The API uses opaque plugin handles:

```cpp
struct TTSPluginHandle;
```

This allows each implementation to maintain its own internal state without exposing implementation details to the host.

## Audio

The DLL boundary uses the C-compatible `TTSAudioBuffer` structure:

```cpp
struct TTSAudioBuffer
{
    const float* samples;
    uint32_t sampleCount;
    uint32_t sampleRate;
    uint16_t channels;
};
```

This structure is intentionally different from the C++ convenience representation used internally by applications or implementations.

No C++ standard-library containers cross the DLL boundary.

## Error Handling

Synthesis operations report their result through `TTSResult`:

```cpp
enum TTSResult : int32_t
{
    TTS_SUCCESS = 0,
    TTS_FAILED = 1,
    TTS_CANCELLED = 2
};
```

The API is designed so that implementations can report completion without requiring exceptions or other C++-specific mechanisms across the DLL boundary.

## Repository Structure

```text
TTS.Core/
├── TTS.Core.sln
├── TTS.Core/
│   ├── TTS.Core.vcxproj
│   └── TTS.Core.vcxproj.filters
├── include/
│   └── TTS/
│       ├── AudioBuffer.h
│       ├── TTSConfig.h
│       └── TTSPlugin.h
├── .gitattributes
├── .gitignore
├── LICENSE
├── README.md
└── vcpkg.json
```

`TTS.Core` is currently a header-only contract. It does not produce a DLL or static library.

## Using TTS.Core

A TTS implementation includes the public headers and implements the exported functions defined by the contract.

A host application includes the same headers and dynamically loads the implementation DLL.

Conceptually:

```text
Host Application
       │
       │ TTS.Core headers
       ▼
TTS Plugin DLL
       │
       │ TTS engine implementation
       ▼
Speech synthesis engine
```

The host and plugin therefore share the contract without requiring the host to link directly against the implementation.

## Dependencies

TTS.Core has no third-party runtime dependencies.

The repository includes a `vcpkg.json` manifest so that the project can participate in a vcpkg-based dependency workflow as the ecosystem develops.

## Scope

TTS.Core intentionally does not define:

* TTS model formats
* phonemization
* audio playback
* audio device management
* GUI functionality
* configuration file formats
* voice model storage
* engine-specific features
* application-specific behavior

Those concerns belong to implementations or higher-level applications.

## Versioning

The plugin contract is intended to evolve conservatively.

Changes to the binary interface must consider compatibility between independently built hosts and plugins.

Future versions of the API may expose an explicit API version so that a host can validate plugin compatibility before using the plugin.

## License

TTS.Core is licensed under the Apache License 2.0.

See [`LICENSE`](LICENSE) for the complete license text.

Copyright © 2026
