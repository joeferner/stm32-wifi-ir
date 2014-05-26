
#include <stdlib.h>
#include <stdint.h>
#include "debug.h"
#include "ir_code.h"

int _ir_code_match(IrCode* code, uint16_t* buffer, uint16_t bufferLen);

IrCode codes[20];

uint16_t code0[] = { 2440, 516, 1232, 541, 616, 541, 615, 542, 1231, 542, 615, 542, 614, 543, 614, 542, 1230, 543, 615, 542, 613, 543, 614, 543, 614 };
uint16_t code1[] = { 2440, 515, 640, 517, 615, 541, 615, 541, 616, 542, 615, 542, 615, 541, 614, 542, 1230, 542, 614, 542, 615, 542, 614, 543, 612 };
uint16_t code2[] = { 2416, 539, 1231, 541, 615, 541, 614, 542, 614, 543, 614, 542, 614, 542, 613, 543, 1230, 543, 614, 542, 614, 544, 613, 544, 613 };
uint16_t code3[] = { 2438, 518, 616, 540, 1231, 541, 615, 541, 615, 541, 615, 542, 615, 542, 615, 541, 1230, 542, 614, 543, 614, 543, 614, 542, 613 };
uint16_t code4[] = { 2439, 517, 1232, 541, 1231, 541, 615, 542, 614, 542, 615, 542, 614, 543, 614, 543, 1229, 543, 614, 543, 614, 543, 614, 543, 614 };
uint16_t code5[] = { 2416, 540, 616, 541, 616, 541, 1231, 541, 615, 542, 614, 542, 613, 542, 614, 543, 1231, 542, 614, 543, 615, 542, 614, 543, 613 };
uint16_t code6[] = { 2416, 540, 1232, 541, 616, 541, 1231, 541, 615, 542, 615, 542, 614, 542, 615, 543, 1230, 542, 614, 542, 614, 542, 614, 542, 614 };
uint16_t code7[] = { 2415, 539, 616, 541, 1231, 541, 1231, 542, 615, 542, 614, 542, 615, 542, 615, 542, 1230, 542, 614, 543, 614, 542, 615, 542, 614 };
uint16_t code8[] = { 2440, 516, 1253, 520, 1231, 541, 1230, 542, 614, 543, 614, 542, 615, 542, 613, 543, 1230, 543, 614, 543, 614, 543, 613, 544, 613 };
uint16_t code9[] = { 2417, 538, 616, 541, 616, 541, 615, 542, 1231, 542, 615, 542, 614, 542, 614, 542, 1231, 542, 614, 542, 615, 542, 614, 543, 614 };

void ir_code_setup() {
  for(int i=0; i<10; i++) {
    codes[i].brand = BRAND_WESTINGHOUSE;
    codes[i].key = i;
    codes[i].codeLength = 25;
    switch(i) {
      case 0: codes[i].code = code0; break;
      case 1: codes[i].code = code1; break;
      case 2: codes[i].code = code2; break;
      case 3: codes[i].code = code3; break;
      case 4: codes[i].code = code4; break;
      case 5: codes[i].code = code5; break;
      case 6: codes[i].code = code6; break;
      case 7: codes[i].code = code7; break;
      case 8: codes[i].code = code8; break;
      case 9: codes[i].code = code9; break;
    }
  }
}

void ir_code_decode(uint16_t* buffer, uint16_t bufferLen) {
  debug_write("signal: ");
  for(int i = 0; i < bufferLen; i++) {
    debug_write_u16(buffer[i], 10);
    debug_write(", ");
  }
  debug_write_line("");

  int bestMatch = -1;
  int bestValue = 0;
  for(int i=0; i<10; i++) {
    int matchValue = _ir_code_match(&codes[i], buffer, bufferLen);
    if(matchValue > bestValue) {
      bestMatch = i;
      bestValue = matchValue;
    }
  }
  if(bestMatch >= 0) {
    debug_write("match: ");
    debug_write_u16(codes[bestMatch].brand, 16);
    debug_write_u16(codes[bestMatch].key, 16);
    debug_write_line("");
  }
}

int _ir_code_match(IrCode* code, uint16_t* buffer, uint16_t bufferLen) {
  int minLen = bufferLen < code->codeLength ? bufferLen : code->codeLength;
  uint32_t diff = 0;
  for(int i=0; i<minLen; i++) {
    diff += abs((int)buffer[i] - (int)code->code[i]);
  }
  debug_write("diff: ");
  debug_write_u16(code->brand, 16);
  debug_write_u16(code->key, 16);
  debug_write(" ");
  debug_write_u16(0xffff - diff, 10);
  debug_write_line("");
  return 0xffff - diff;
}
