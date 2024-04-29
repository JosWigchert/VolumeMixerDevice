#pragma once

#include <Arduino.h>

class ColorGradiant
{
public:
    ColorGradiant(uint8_t len);
    ~ColorGradiant();

    void SetGradiant(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    Color GetGradiant(uint8_t index);

    uint8_t GetLength();

private:
    Color *gradiants = nullptr;
    uint8_t pointer = 0;
    uint8_t len = 0;
};

ColorGradiant::ColorGradiant(uint8_t len)
{
    this->len = len;
    gradiants = new Color[len];
}

ColorGradiant::~ColorGradiant()
{
    if (gradiants != nullptr)
    {
        delete gradiants;
    }
}

void ColorGradiant::SetGradiant(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    Color color;
    color.a = a;
    color.r = r;
    color.g = g;
    color.b = b;
    gradiants[pointer] = color;
}

Color ColorGradiant::GetGradiant(uint8_t index)
{
    if (index > len)
    {
        index = len;
    }
    
    return gradiants[index];
}

uint8_t ColorGradiant::GetLength()
{
    return len;
}