#pragma once

#include <Encoder.h>

#define ENCODER_DO_NOT_USE_INTERRUPTS

class EncoderWrapper
{
private:
    /* data */
public:
    EncoderWrapper();
    EncoderWrapper(int16_t pinA, int16_t pinB, int16_t pinButton);
    ~EncoderWrapper();

    void Run();
    void UpdateMute();
    void UpdateEncoder();

    void SetEncoder(int16_t pinA, int16_t pinB, int16_t pinButton);
    Encoder GetEncoder();

    void SetValue(int16_t value);
    int16_t GetValue();

    void SetMaxValue(int16_t value);
    int16_t GetMaxValue();

    void SetMute(bool mute);
    bool GetMute();

    void SetUpdateDelay(uint32_t value);
    uint32_t GetUpdateDelay();

private:
    Encoder *encoder = nullptr;
    uint8_t encoderButton;
    uint8_t buttonState;

    bool mute;

    int16_t value;
    int16_t maxValue = 255;

    uint32_t updateDelay = 25;
};

EncoderWrapper::EncoderWrapper()
{
}

EncoderWrapper::EncoderWrapper(int16_t pinA, int16_t pinB, int16_t pinButton)
{
    encoder = new Encoder(pinA, pinB);
    encoderButton = pinButton;
    pinMode(pinButton, INPUT_PULLUP);
}

EncoderWrapper::~EncoderWrapper()
{
    if (encoder != nullptr)
    {
        delete encoder;
    }
}

void EncoderWrapper::Run()
{
    encoder->read();

    static unsigned long time = 0;
    unsigned long milliseconds = millis();
    if (time + updateDelay < milliseconds)
    {
        UpdateMute();
        UpdateEncoder();
        time = milliseconds;
    }
}

void EncoderWrapper::UpdateMute()
{
    static bool previousState = false;
    bool state = digitalRead(encoderButton);

    if (state == false && previousState == true)
    {
        mute = !mute;
    }

    previousState = state;
}

void EncoderWrapper::UpdateEncoder()
{
    value = encoder->read();

    if (value < 0)
    {
        encoder->write(0);
        value = 0;
    }
    else if (value > maxValue)
    {
        encoder->write(maxValue);
        value = maxValue;
    }
}

void EncoderWrapper::SetEncoder(int16_t pinA, int16_t pinB, int16_t pinButton)
{
    encoder = new Encoder(pinA, pinB);
    pinMode(pinButton, INPUT_PULLUP);

    encoder->write(51);
}

Encoder EncoderWrapper::GetEncoder()
{
    return *encoder;
}

void EncoderWrapper::SetValue(int16_t value)
{
    encoder->write(value);
    this->value = value;
}

int16_t EncoderWrapper::GetValue()
{
    return value;
}

void EncoderWrapper::SetMaxValue(int16_t value)
{
    this->maxValue = value;
}

int16_t EncoderWrapper::GetMaxValue()
{
    return maxValue;
}

void EncoderWrapper::SetMute(bool mute)
{
    this->mute = mute;
}

bool EncoderWrapper::GetMute()
{
    return mute;
}

void EncoderWrapper::SetUpdateDelay(uint32_t updateDelay)
{
    this->updateDelay = updateDelay;
}

uint32_t EncoderWrapper::GetUpdateDelay()
{
    return updateDelay;
}