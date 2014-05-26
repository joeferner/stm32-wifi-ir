#ifndef IR_CODE_H
#define	IR_CODE_H

#define BRAND_TIVO         0x0001
#define BRAND_WESTINGHOUSE 0x0002

typedef struct {
  uint16_t brand;
  uint16_t key;
  uint16_t codeLength;
  uint16_t* code;
} IrCode;

void ir_code_setup();
void ir_code_decode(uint16_t* buffer, uint16_t bufferLen);

#endif // IR_CODE_H
