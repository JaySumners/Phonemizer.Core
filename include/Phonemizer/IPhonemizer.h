#pragma once

#include <string>

class IPhonemizer
{
public:
    virtual ~IPhonemizer() = default;

    virtual bool Initialize() = 0;

    virtual std::string Phonemize(
        const std::string& text
    ) const = 0;
};