#include "cmre.h"

u8 CM_STDCALL cm_fn_0083bd00(s16 value)
{
    if (value <= 0x140) {
        if (value == 0x140)
            return 0x0e;
        if (value <= 0x84) {
            switch (value) {
            case 0x84: return 2;
            case 1: return 0;
            case 2: return 1;
            case 4: return 0x11;
            case 8: return 0x13;
            case 0x10: return 0x12;
            case 0x20: return 0x15;
            case 0x40: return 0x14;
            default: return 0x0e;
            }
        }
        switch (value) {
        case 0x88: return 5;
        case 0x90: return 8;
        case 0xa0: return 0x0b;
        case 0x104: return 4;
        case 0x108: return 6;
        case 0x110: return 9;
        case 0x120: return 0x0c;
        default: return 0x0e;
        }
    }

    if (value <= 0x410) {
        if (value == 0x410) return 9;
        if (value > 0x220) {
            if (value == 0x404) return 4;
            if (value == 0x408) return 6;
            return 0x0e;
        }
        if (value == 0x220) return 0x0c;
        if (value == 0x204) return 4;
        if (value == 0x208) return 6;
        if (value == 0x210) return 9;
        return 0x0e;
    }

    if (value <= 0x808) {
        if (value == 0x808) return 7;
        if (value == 0x420) return 0x0c;
        if (value == 0x804) return 3;
        return 0x0e;
    }
    if (value == 0x810) return 0x0a;
    if (value == 0x820) return 0x0d;
    return 0x0e;
}