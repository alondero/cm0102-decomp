/* x87-register argument bridges for registered CRT intrinsics (ADR 0010).
   Each bridge loads its long double operands onto the x87 stack at full
   80-bit width, calls the original helper and returns ST0. It computes
   nothing itself; the helper stays the original code. */
#ifndef CMRE_X87_H
#define CMRE_X87_H

#include "cmre.h"

/* pow(x, y) through _CIpow 0x00945ff0: ST1 = x (base), ST0 = y (exponent). */
long double CM_CDECL cm_x87_CIpow(long double x, long double y);

__asm__(
    "\t.section .text$cm_x87_CIpow,\"x\"\n"
    "\t.linkonce discard\n"
    "\t.globl _cm_x87_CIpow\n"
    "_cm_x87_CIpow:\n"
    "\tfldt 4(%esp)\n"
    "\tfldt 16(%esp)\n"
    "\tcall _cm_fn_00945ff0\n"
    "\tret\n"
    "\t.text\n");

#endif
