# Phonemizer.Core

`Phonemizer.Core` is a generic plugin contract for phonemization engines.

It defines the binary interface between an application or host library and a phonemizer implementation. The project contains **no phonemization engine itself** and has no dependency on a specific phonemizer implementation.

The goal is to allow different phonemization engines to be developed, distributed, and replaced independently while exposing a stable interface to consumers.

## Architecture

```text
                    Phonemizer.Core
                   Plugin Contract
                         ▲
                         │
             ┌───────────┴───────────┐
             │                       │
             │                       │
   Phonemizer.eSpeakNG       Phonemizer.Misaki
        implementation          implementation
```

A consumer depends only on `Phonemizer.Core` and does not need to know how a particular phonemizer is implemented.

For example:

```text
TTS.Kokoro
     │
     │ Phonemizer.Core
     ▼
Phonemizer.eSpeakNG
```

Additional implementations can be introduced without changing the Core contract.

## Design Goals

`Phonemizer.Core` is intentionally small.

The interface is designed around the following principles:

* Implementation-independent API
* Stable DLL/plugin boundary
* C linkage for exported functions
* Opaque plugin handles
* No C++ standard-library types across the DLL boundary
* Explicit buffer ownership
* UTF-8 text input and output
* Runtime API version identification
* Independent phonemizer implementations
* No dependency on TTS engines, applications, GUI frameworks, or simulators

The Core library should remain usable by any project that needs a phonemization plugin, not only by a specific TTS engine.

## Public API

The primary public header is:

```text
include/
└── Phonemizer/
    ├── PhonemizerPlugin.h
    └── PhonemizerVersion.h
```

### `PhonemizerPlugin.h`

This header defines the plugin ABI.

The interface provides:

* Plugin creation and destruction
* Initialization
* API version identification
* Language capability queries
* Phonemization
* Output buffer management

The plugin is represented by an opaque handle:

```cpp
struct PhonemizerHandle;
```

The implementation owns the internal state. Consumers interact with the implementation only through the exported API functions.

## API Versioning

`PhonemizerVersion.h` defines the API compatibility version:

```cpp
#define PHONEMIZER_CORE_API_VERSION_MAJOR 1
#define PHONEMIZER_CORE_API_VERSION_MINOR 0
```

The version describes the **Core API contract**, not the version of an individual phonemizer implementation.

The intended compatibility model is:

* **Major** — breaking API or ABI change
* **Minor** — backward-compatible API addition or extension
* **Patch** — implementation/release versioning handled separately by the individual project

A phonemizer implementation may therefore have its own release version while implementing a particular `Phonemizer.Core` API version.

## Phoneme Buffers

Phonemization output is returned through a simple buffer:

```cpp
struct PhonemizerBuffer
{
    const char* data;
    uint32_t length;
};
```

The Core interface does not expose `std::string` or other C++ standard-library types across the DLL boundary.

The implementation is responsible for allocating the returned buffer. The consumer releases it using:

```cpp
Phonemizer_FreeBuffer(...)
```

This keeps memory ownership within the plugin and avoids crossing incompatible C++ runtime boundaries.

## Language Support

Consumers can query whether an implementation supports a particular language:

```cpp
Phonemizer_SupportsLanguage(...)
```

The Core interface does not define implementation-specific language identifiers or phonemization rules.

Individual implementations determine which languages and dialects they support.

## Plugin Implementations

`Phonemizer.Core` does not contain an implementation.

Implementations are separate projects and repositories.

For example:

```text
Phonemizer.Core
        ▲
        │
        ├── Phonemizer.eSpeakNG
        │
        ├── Phonemizer.Misaki
        │
        └── Other implementations
```

This allows consumers to select the implementation appropriate for their requirements.

A host may also maintain an ordered list of implementations and use a fallback strategy when more than one phonemizer is available.

For example:

```text
1. Phonemizer.Misaki
2. Phonemizer.eSpeakNG
```

The fallback policy belongs to the **consumer**, not to `Phonemizer.Core`.

## Example Integration

A consumer can load a phonemizer plugin dynamically and communicate with it exclusively through the Core API.

Conceptually:

```text
Consumer
   │
   │ Load plugin
   ▼
Phonemizer Plugin
   │
   │ implements
   ▼
Phonemizer.Core ABI
```

The consumer does not need to link directly against the implementation's internal classes or libraries.

## Repository Structure

```text
Phonemizer.Core/
├── Phonemizer.Core.sln
├── Phonemizer.Core/
│   └── Phonemizer.Core.vcxproj
├── include/
│   └── Phonemizer/
│       ├── PhonemizerPlugin.h
│       └── PhonemizerVersion.h
├── README.md
├── LICENSE
├── .gitignore
├── .gitattributes
└── vcpkg.json
```

`Phonemizer.Core` is intentionally header-only. It does not build a static library or DLL.

## Dependencies

`Phonemizer.Core` has no runtime dependency on a phonemization engine.

It does not depend on:

* eSpeak NG
* Misaki
* Kokoro
* ONNX Runtime
* Qt
* Microsoft Flight Simulator
* SimConnect
* Any particular application

Implementation-specific dependencies belong in the corresponding implementation repository.

## Consumers

Potential consumers include:

* TTS engines
* TTS applications
* Command-line tools
* GUI applications
* Accessibility software
* Speech-processing tools
* Other software requiring phonemization

A consumer should depend on `Phonemizer.Core`, rather than depending directly on an implementation when runtime plugin selection is desired.

## Relationship to TTS.Core

`Phonemizer.Core` and `TTS.Core` define separate plugin contracts.

```text
TTS.Core
   ▲
   │
TTS.Kokoro
   │
   │ Phonemizer.Core
   ▼
Phonemizer.eSpeakNG
```

`TTS.Core` defines the interface between a TTS host and a TTS engine.

`Phonemizer.Core` defines the interface between a phonemization consumer and a phonemizer.

Neither Core library contains the implementation of the other.

## License

`Phonemizer.Core` is licensed under the Apache License, Version 2.0.

See [`LICENSE`](LICENSE) for the complete license text.

Copyright and attribution requirements for individual phonemizer implementations are the responsibility of those implementations and their respective repositories.

## Scope

This repository intentionally contains only the generic phonemizer contract.

It does **not** provide:

* A phonemization engine
* Language models
* Pronunciation dictionaries
* Language-specific rules
* Voice data
* TTS functionality
* Audio generation
* Tokenization for a particular TTS model

Those concerns belong to implementations or consuming projects.

## Status

`Phonemizer.Core` is intended to provide a stable foundation for independently developed phonemizer implementations.

The API should remain deliberately small. New functionality should only be added to the Core contract when there is a demonstrated need across multiple implementations or consumers.
