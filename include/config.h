#pragma once

#define BAUDRATE 115200

// Message info
#define MessageLength 5

// Message types
#define RequestValueMessage 0x01
#define ReceiveValueMessage 0x02
#define SendValueMessage 0x03

enum MessageType { RequestValue = RequestValueMessage,
                    ReceiveValue = ReceiveValueMessage,
                    SendValue = SendValueMessage};