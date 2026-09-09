# Phonemizer.Core

A generic, implementation-independent C ABI-compatible plugin contract for phonemization engines.

## Overview

Phonemizer.Core defines the binary interface between an application or host library and a dynamically loaded phonemization implementation.

It provides a small, stable API for:

* Creating and destroying a phonemizer plugin instance
* Initializing a plugin
* Identifying the API version
* Querying language capabilities
* Phonemizing UTF-8 text
* Managing output buffers

Phonemizer.Core contains **no phonemization engine implementation**. It is intended to be implemented by independent phonemizer plugins and consumed by applications or libraries that want to support multiple phonemization engines without linking directly against a specific implementation.

## Architecture

```text
                 Phonemizer.Core
                  Plugin Contract
                        ▲
                        │
            ┌───────────┴───────────┐
            │                       │
   Phonemizer.eSpeakNG       Phonemizer.Misaki
      implementation           implementation
            │                       │
            └───────────┬───────────┘
                        │
                        ▼
                     Consumer
```

A consumer depends only on Phonemizer.Core and does not need to know how a particular phonemizer is implemented.

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

Phonemizer.Core is intentionally small.

The interface is designed around the following principles.

### Implementation independent

The contract does not depend on a particular phonemizer, TTS engine, model, application, GUI framework, or simulator.

### C ABI-compatible

The plugin boundary uses C linkage, fixed-width integer types, pointers, opaque handles, and C-style structures to avoid exposing C++ ABI details across the DLL boundary.

The interface intentionally avoids passing types such as:

* `std::string`
* `std::vector`
* C++ classes
* C++ exceptions

across the plugin boundary.

The public headers themselves are C++ headers and the project uses C++20, but the DLL boundary is designed to remain independent of C++ ABI details.

### Explicit ownership

Phonemization output has an explicit lifetime and release mechanism. The implementation owns memory returned through the plugin interface, and the consumer releases it using the appropriate Core API function.

### UTF-8 text

Text input and phonemization-related text data use UTF-8 encoded strings.

### Runtime API identification

The API exposes its compatibility version so that consumers can identify the Core contract implemented by a plugin.

### Independent implementations

Phonemizer implementations are separate projects and repositories. Core functionality should remain independent of implementation-specific dependencies.

## Public API

The primary public headers are:

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
* Plugin initialization
* API version identification
* Language capability queries
* Phonemization
* Output buffer management

The plugin is represented by an opaque handle:

```cpp
struct PhonemizerHandle;
```

This allows each implementation to maintain its own internal state without exposing implementation details to the consumer.

## API Versioning

`PhonemizerVersion.h` defines the Core API compatibility version:

```cpp
#define PHONEMIZER_CORE_API_VERSION_MAJOR 1
#define PHONEMIZER_CORE_API_VERSION_MINOR 0
```

The API version describes the **Core API contract**, not the version of an individual phonemizer implementation.

The intended compatibility model is:

* **Major** — breaking API or ABI change
* **Minor** — backward-compatible API addition or extension

An implementation may therefore have its own release version while implementing a particular Phonemizer.Core API version.

The Core API version is independent of the project release version.

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

The Core interface does not define implementation-specific phonemization rules.

Individual implementations determine which languages, dialects, and pronunciation systems they support.

## Plugin Implementations

Phonemizer.Core does not contain a phonemization implementation.

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

A consumer may maintain an ordered list of available implementations and use a fallback strategy when more than one phonemizer is available.

For example:

```text
1. Phonemizer.Misaki
2. Phonemizer.eSpeakNG
```

The fallback policy belongs to the **consumer**, not to Phonemizer.Core.

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

## Building

Phonemizer.Core uses CMake and requires C++20.

The project is a header-only interface library and does not produce a DLL or static library.

Configure the project with CMake:

```text
cmake -S . -B build
```

Build:

```text
cmake --build build
```

Install:

```text
cmake --install build
```

The installed package provides the CMake target:

```text
Phonemizer::Core
```

## Using Phonemizer.Core with CMake

After installing Phonemizer.Core, a consuming CMake project can use:

```cmake
find_package(Phonemizer CONFIG REQUIRED)

target_link_libraries(MyApplication
    PRIVATE
        Phonemizer::Core
)
```

The public headers can then be included with:

```cpp
#include <Phonemizer/PhonemizerPlugin.h>
```

The installed package provides the headers and CMake package configuration required by consumers.

## Repository Structure

```text
Phonemizer.Core/

├── cmake/
│   └── PhonemizerConfig.cmake.in
├── include/
│   └── Phonemizer/
│       ├── PhonemizerPlugin.h
│       └── PhonemizerVersion.h
├── .gitattributes
├── .gitignore
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE.txt
└── README.md
```

Phonemizer.Core is currently a header-only contract. It does not produce a DLL or static library.

## Dependencies

Phonemizer.Core has no third-party runtime dependencies.

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

A consumer should depend on Phonemizer.Core rather than directly depending on an implementation when runtime plugin selection is desired.

## Relationship to TTS.Core

Phonemizer.Core and TTS.Core define separate plugin contracts.

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

TTS.Core defines the interface between a TTS host and a TTS engine.

Phonemizer.Core defines the interface between a phonemization consumer and a phonemizer.

Neither Core library contains the implementation of the other.

The two contracts can therefore be used independently or together.

## Scope

This repository intentionally contains only the generic phonemizer contract.

It does **not** provide:

* A phonemization engine
* Language models
* Pronunciation dictionaries
* Language-specific phonemization rules
* Voice data
* TTS functionality
* Audio generation
* Tokenization for a particular TTS model

Those concerns belong to implementations or consuming projects.

## Status

Phonemizer.Core is intended to provide a stable foundation for independently developed phonemizer implementations.

The API should remain deliberately small. New functionality should only be added to the Core contract when there is a demonstrated need across multiple implementations or consumers.

## License

Phonemizer.Core is licensed under the Apache License 2.0.

See `LICENSE.txt` for the complete license text.

Copyright © 2026
