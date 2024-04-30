# UART Communication Protocol Messages

This document describes the UART communication protocol used to communicate with the volume controller.
Each messages starts with a start character from the ascii table and ends with a stop character. 
The first byte in the message is the message code. 
This is used to identify the type of message. 
A response to a message will have the same message code as the request, with the most significant bit set to 1. 
For example if the request is 0x01, the response will be 0x81.
After this is the payload of the message, the payload can be fixed length or variable length. 
If the payload is variable length, the first byte in the payload should contain some information about the length of the payload.

## 1. Get Encoder Values

### Description

Request to retrieve the values of the encoders from the volume controller.

### Message Structure

- **Code**: 0x01

### Response

- **Code**: 0x81
- **Number of encoders**: 1 byte 
- **Encoder values**:
  - Encoder 1: x byte
  - Encoder 2: x byte
  - Encoder ...: x bytes
  - Encoder n: x bytes
