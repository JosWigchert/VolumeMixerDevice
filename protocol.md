# UART Communication Protocol Messages

## 1. Get Encoder Values

### Description

Request to retrieve the values of the encoders from the volume controller.

### Message Structure

- **Code**: 0x01
- **Payload Length**: 1 byte (Number of encoders)
- **Payload**:
  - Byte 1: Number of Encoders (N)
- **Response**:
  - Byte 1-2: Encoder 1 Value (16-bit unsigned integer)
  - Byte 3-4: Encoder 2 Value (16-bit unsigned integer)
  - ...
  - Byte (2N-1)-(2N): Encoder N Value (16-bit unsigned integer)

### Example

#### Request
