#pragma once

#include <Arduino.h>
#include <Color.h>

void UpdateEncodersLoop();
void UpdateLightsLoop();

void InitColorGradiant();
uint8_t GetValueAlong(uint8_t start, uint8_t end, float percentageAlong);
Color GetColorGradiant(uint16_t value, uint16_t maxValue);
void UpdateLights();
void PrintEncoderValues();
void SendMessage(uint8_t messageType, uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4, uint8_t value5);
