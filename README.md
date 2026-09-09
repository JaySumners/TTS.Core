# TTS.Core

A generic, implementation-independent C ABI-compatible plugin contract for text-to-speech engines.

## Overview

TTS.Core defines the binary interface between an application and a dynamically loaded text-to-speech (TTS) implementation.

It provides a small, stable API for:

- Creating and destroying a TTS plugin instance
- Submitting asynchronous synthesis requests
- Cancelling synthesis jobs
- Receiving synthesized audio through callbacks
- Managing audio buffer ownership
- Querying available voices

TTS.Core contains **no TTS engine implementation**. It is intended to be implemented by independent TTS plugins and consumed by applications that want to support multiple TTS engines without linking directly against a specific implementation.

## Architecture

```text
                 TTS.Core
              Plugin Contract
                    ▲
                    │
        ┌───────────┼───────────┐
        │           │           │
   TTS.Kokoro   TTS.Piper   Other TTS
        │           │           │
        └───────────┴───────────┘
                    │
                    ▼
                Host App