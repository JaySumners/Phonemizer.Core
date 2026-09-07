#pragma once

#include <cstdint>

#ifdef PHONEMIZER_PLUGIN_BUILD
    #ifdef _WIN32
        #define PHONEMIZER_PLUGIN_API __declspec(dllexport)
    #else
        #define PHONEMIZER_PLUGIN_API
    #endif
#else
    #ifdef _WIN32
        #define PHONEMIZER_PLUGIN_API __declspec(dllimport)
    #else
        #define PHONEMIZER_PLUGIN_API
    #endif
#endif

#include "PhonemizerVersion.h"

extern "C"
{
    // -------------------------------------------------------------------------
    // Opaque handle
    // -------------------------------------------------------------------------

    struct PhonemizerHandle;

    // -------------------------------------------------------------------------
    // Result
    // -------------------------------------------------------------------------

    enum PhonemizerResult : int32_t
    {
        PHONEMIZER_SUCCESS = 0,
        PHONEMIZER_FAILED = 1,
        PHONEMIZER_UNSUPPORTED = 2
    };

    // -------------------------------------------------------------------------
    // Phoneme buffer
    // -------------------------------------------------------------------------

    struct PhonemizerBuffer
    {
        const char* data;
        uint32_t length;
    };

    // -------------------------------------------------------------------------
    // API version
    // -------------------------------------------------------------------------

    PHONEMIZER_PLUGIN_API void Phonemizer_GetApiVersion(
        uint32_t* major,
        uint32_t* minor
    );

    // -------------------------------------------------------------------------
    // Lifecycle
    // -------------------------------------------------------------------------

    PHONEMIZER_PLUGIN_API PhonemizerHandle* Phonemizer_Create();

    PHONEMIZER_PLUGIN_API void Phonemizer_Destroy(
        PhonemizerHandle* handle
    );

    // -------------------------------------------------------------------------
    // Initialization
    // -------------------------------------------------------------------------

    PHONEMIZER_PLUGIN_API PhonemizerResult Phonemizer_Initialize(
        PhonemizerHandle* handle,
        const char* dataDirectory
    );

    // -------------------------------------------------------------------------
    // Language support
    // -------------------------------------------------------------------------

    PHONEMIZER_PLUGIN_API bool Phonemizer_SupportsLanguage(
        PhonemizerHandle* handle,
        const char* language
    );

    // -------------------------------------------------------------------------
    // Phonemization
    // -------------------------------------------------------------------------

    PHONEMIZER_PLUGIN_API PhonemizerResult Phonemizer_Phonemize(
        PhonemizerHandle* handle,
        const char* text,
        PhonemizerBuffer* output
    );

    // -------------------------------------------------------------------------
    // Memory management
    // -------------------------------------------------------------------------

    PHONEMIZER_PLUGIN_API void Phonemizer_FreeBuffer(
        PhonemizerHandle* handle,
        PhonemizerBuffer* buffer
    );
}