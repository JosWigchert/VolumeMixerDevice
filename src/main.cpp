#include <EEPROM.h>
/* Uses EEPROM bytes 0 to 68 (68 not included)
 * to store the color gradiant information for
 * the volume colors and the off color
 */

#include <pins.h>
#include <config.h>

#include <main.h>
#include <ColorGradiant.h>
#include <EncoderWrapper.h>
#include <Adafruit_NeoPixel.h>
const int NUM_SLIDERS = 5;
const int LedUpdateFrequency = 20; // frequency of how many times per seconds the leds need to be updated

Adafruit_NeoPixel pixels(NUMLEDS, LEDPIN, NEO_GRB + NEO_KHZ800);
EncoderWrapper **encoders;

uint32_t ColorMuted;
ColorGradiant *gradiant;

void setup()
{
    Serial.begin(BAUDRATE);

    InitColorGradiant();

    encoders = new EncoderWrapper *[ENCODERCOUNT];

    encoders[0] = new EncoderWrapper(SW1A, SW1B, SW1BUTTON);
    encoders[1] = new EncoderWrapper(SW2A, SW2B, SW2BUTTON);
    encoders[2] = new EncoderWrapper(SW3A, SW3B, SW3BUTTON);
    encoders[3] = new EncoderWrapper(SW4A, SW4B, SW4BUTTON);
    encoders[4] = new EncoderWrapper(SW5A, SW5B, SW5BUTTON);

    pixels.begin();
    pixels.clear();
}

void loop()
{
    UpdateEncodersLoop();
    UpdateLightsLoop();

    static unsigned long time = 0;
    unsigned long milliseconds = millis();
    if (time + 1000 < milliseconds)
    {
        PrintEncoderValues();
        time = milliseconds;
    }
}

void UpdateEncodersLoop()
{
    for (size_t i = 0; i < ENCODERCOUNT; i++)
    {
        encoders[i]->Run();
    }
}

void UpdateLightsLoop()
{
    static unsigned long time = 0;
    unsigned long milliseconds = millis();
    if (time + LedUpdateFrequency < milliseconds)
    {
        UpdateLights();
        time = milliseconds;
        pixels.show();
    }
}

void UpdateSerialLoop()
{
    if (Serial.available() >= MessageLength)
    {
        uint8_t buffer[MessageLength];
        Serial.readBytes(buffer, MessageLength);
        MessageType type = (MessageType)buffer[0];
        switch (type)
        {
        case RequestValue:
            break;
        case ReceiveValue:
            break;
        case SendValue:
            break;
        default:
            break;
        }
    }
}

void InitColorGradiant()
{
    uint8_t red = EEPROM.read(0);
    uint8_t green = EEPROM.read(1);
    uint8_t blue = EEPROM.read(2);

    ColorMuted = pixels.Color(red, green, blue);

    uint8_t numColors = EEPROM.read(3);
    if (numColors > 16)
    {
        numColors = 16;
    }

    gradiant = new ColorGradiant(numColors);

    for (uint8_t i = 0; i < numColors; i++)
    {
        uint8_t offset = (i * 4) + 4;
        uint8_t gradiantPoint = EEPROM.read(offset);
        red = EEPROM.read(offset + 1);
        green = EEPROM.read(offset + 2);
        blue = EEPROM.read(offset + 3);

        gradiant->SetGradiant(gradiantPoint, red, green, blue);
    }
}

uint8_t GetValueAlong(uint8_t start, uint8_t end, float percentageAlong)
{
    uint8_t normalized = end - start;
    uint8_t value = normalized * percentageAlong;
    return value + start;
}

Color GetColorGradiant(uint16_t value, uint16_t maxValue)
{
    uint8_t percentage = value / maxValue;

    Color prevCol = gradiant->GetGradiant(0);

    for (size_t i = 0; i < gradiant->GetLength(); i++)
    {
        Color col = gradiant->GetGradiant(i);
        if (col.a >= percentage)
        {
            float percentageAlong = (percentage - prevCol.a) / (col.a - prevCol.a);
            uint8_t r = GetValueAlong(prevCol.r, col.r, percentageAlong);
            uint8_t g = GetValueAlong(prevCol.g, col.g, percentageAlong);
            uint8_t b = GetValueAlong(prevCol.b, col.b, percentageAlong);
        }
        prevCol = col;
    }
}

void UpdateLights()
{
    for (size_t i = 0; i < ENCODERCOUNT; i++)
    {
        if (encoders[i]->GetMute())
        {
            pixels.setPixelColor(i, 150, 0, 0);
        }
        else
        {
            Color c = GetColorGradiant(encoders[i]->GetValue(), encoders[i]->GetMaxValue());
            pixels.setPixelColor(i, c.r, c.g, c.b);
        }
    }
}

void PrintEncoderValues()
{
    for (int i = 0; i < ENCODERCOUNT; i++)
    {
        String printedString = String("Encoder #") + String(i + 1) + String(": ") + String(encoders[i]->GetValue());
        Serial.write(printedString.c_str());

        if (i < NUM_SLIDERS - 1)
        {
            Serial.write(" | ");
        }
        else
        {
            Serial.write("\n");
        }
    }
}

void SendMessage(uint8_t messageType, uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4, uint8_t value5)
{
    const size_t messageSize = 6;
    uint8_t *messageBytes = new uint8_t[messageSize];

    messageBytes[0] = messageType;
    messageBytes[1] = value1;
    messageBytes[2] = value2;
    messageBytes[3] = value3;
    messageBytes[4] = value4;
    messageBytes[5] = value5;

    Serial.write(messageBytes, messageSize);
    delete[] messageBytes;
}