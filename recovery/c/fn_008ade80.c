/* Draft for preferred-image VA 0x008ade80, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

unsigned long long cm_fn_008ade80(char param_1)

{
  if (param_1 == '\0') {
    return 1;
  }
  if (param_1 == '\x01') {
    return 2;
  }
  if (param_1 == '\x02') {
    return 4;
  }
  if (param_1 == '\x03') {
    return 8;
  }
  if (param_1 == '\x04') {
    return 0x10;
  }
  if (param_1 == '\x05') {
    return 0x20;
  }
  if (param_1 == '\x06') {
    return 0x40;
  }
  if (param_1 == '\a') {
    return 0x80;
  }
  if (param_1 == '\b') {
    return 0x100;
  }
  if (param_1 == '\t') {
    return 0x200;
  }
  if (param_1 == '\n') {
    return 0x400;
  }
  if (param_1 == '\v') {
    return 0x800;
  }
  if (param_1 == '\f') {
    return 0x1000;
  }
  if (param_1 == '\r') {
    return 0x2000;
  }
  if (param_1 == '\x0e') {
    return 0x4000;
  }
  if (param_1 == '\x0f') {
    return 0x8000;
  }
  if (param_1 == '\x10') {
    return 0x10000;
  }
  if (param_1 == '\x11') {
    return 0x20000;
  }
  if (param_1 == '\x12') {
    return 0x40000;
  }
  if (param_1 == '\x13') {
    return 0x80000;
  }
  if (param_1 == '\x14') {
    return 0x100000;
  }
  if (param_1 == '\x15') {
    return 0x200000;
  }
  if (param_1 == '\x16') {
    return 0x400000;
  }
  if (param_1 == '\x17') {
    return 0x800000;
  }
  if (param_1 == '\x18') {
    return 0x1000000;
  }
  if (param_1 == '\x19') {
    return 0x2000000;
  }
  if (param_1 == '\x1a') {
    return 0x4000000;
  }
  if (param_1 == '\x1b') {
    return 0x8000000;
  }
  if (param_1 == '\x1c') {
    return 0x10000000;
  }
  if (param_1 == '\x1d') {
    return 0x20000000;
  }
  if (param_1 == '\x1e') {
    return 0x40000000;
  }
  if (param_1 == '\x1f') {
    return 0x80000000;
  }
  if (param_1 == ' ') {
    return 0x100000000;
  }
  if (param_1 == '!') {
    return 0x200000000;
  }
  if (param_1 == '\"') {
    return 0x400000000;
  }
  if (param_1 == '#') {
    return 0x800000000;
  }
  if (param_1 == '$') {
    return 0x1000000000;
  }
  if (param_1 == '%') {
    return 0x2000000000;
  }
  if (param_1 == '&') {
    return 0x4000000000;
  }
  if (param_1 == '\'') {
    return 0x8000000000;
  }
  if (param_1 == '(') {
    return 0x10000000000;
  }
  if (param_1 == ')') {
    return 0x20000000000;
  }
  if (param_1 == '*') {
    return 0x40000000000;
  }
  if (param_1 == '+') {
    return 0x80000000000;
  }
  if (param_1 == ',') {
    return 0x100000000000;
  }
  if (param_1 == '-') {
    return 0x200000000000;
  }
  if (param_1 == '.') {
    return 0x400000000000;
  }
  if (param_1 == '/') {
    return 0x800000000000;
  }
  return 0;
}
