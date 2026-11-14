/* fn_0058e340 -- Championship Manager 01/02 3.9.68, module file_llist.cpp
   Preferred-image VA 0x0058e340, image base 0x00400000.

   Faithful plain-C translation of the recovered listing (1633 bytes).
   Convention: thiscall, `this` in ECX, three stack arguments, `ret 0xc`.

   Frame (original): `sub esp, 0x208` gives L = entry_esp - 0x208; four
   saved registers (ebx/ebp/esi/edi) live above it, so L is the base of the
   0x208 byte scratch area:
       L + 0x000  local_208   (dword)
       L + 0x004  local_204   (dword)
       L + 0x008  local_200   (256 bytes)
       L + 0x108  local_100   (256 bytes)
   The two 256 byte scratch buffers and the two dwords are kept in one
   allocation with explicit offsets so their adjacency matches the original.
*/

#include "cmre.h"

/* ---------------- original globals ---------------- */
#define STR_009a8120 ((char *)0x009a8120)
#define STR_009c7540 ((char *)0x009c7540)
#define STR_009c756c ((char *)0x009c756c)
#define STR_009c75ac ((char *)0x009c75ac)
#define STR_009c75e4 ((char *)0x009c75e4)
#define STR_009c7614 ((char *)0x009c7614)
#define STR_009c764c ((char *)0x009c764c)
#define STR_009c7680 ((char *)0x009c7680)
#define STR_009c7774 ((char *)0x009c7774)
#define STR_009c78b0 ((char *)0x009c78b0)
#define STR_009c7970 ((char *)0x009c7970)
#define STR_009c7788 ((char *)0x009c7788)
#define STR_009c77bc ((char *)0x009c77bc)
#define STR_009c77e8 ((char *)0x009c77e8)
#define STR_009c781c ((char *)0x009c781c)
#define STR_009c784c ((char *)0x009c784c)
#define STR_009c7880 ((char *)0x009c7880)
#define STR_009c7980 ((char *)0x009c7980)
#define DAT_009c7848 CM_GLOBAL(u8, 0x009c7848)
#define DAT_009c79b0 CM_GLOBAL(u8, 0x009c79b0)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae26c4 CM_GLOBAL(u8, 0x00ae26c4)
#define DAT_00ae28bc CM_GLOBAL(u8, 0x00ae28bc)

/* ---------------- original callees ---------------- */
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern char *CM_CDECL cm_fn_0090d130(char *param_1, char *param_2, u32 param_3, char *param_4);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);
extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);
extern s32 CM_CDECL cm_fn_00946a22(volatile u8 *file, u32 second, u32 mode);
extern s32 CM_CDECL cm_fn_00946aae(volatile u8 *file);

/* ---------------- original scratch frame ---------------- */
typedef struct {
  s32 local_208;      /* L + 0x000 */
  s32 local_204;      /* L + 0x004 */
  u8 local_200[256];  /* L + 0x008 */
  u8 local_100[256];  /* L + 0x108 */
} cm_llist_frame;

/* Failure tail shared by every error site of the original routine.
   Call order (listing order): 0x00944cff, 0x0090d130, 0x00944c9f,
   0x0066f4e0 (title), 0x0066f4e0 (detail), 0x005e8290, and, only on the
   paths that own an open file, 0x00945a09 (close).

   0x00944c9f takes seven words: the three it is given here plus the four
   arguments of the 0x0090d130 call, which are still live on the stack when
   it is entered (the original cleans all seven with `add esp, 0x1c`).  The
   variadic arguments are therefore (result, first, second, line, text) in
   that order. */
static void cm_llist_fail(u32 line, char *text, u8 *first, u8 *second,
                          char *title, char *detail, volatile u32 *file, int close_file)
{
  u32 r;

  cm_fn_00944cff((u32)STR_009c7540, 0, 0, (void *)first, (void *)second);
  r = (u32)cm_fn_0090d130((char *)first, (char *)second, line, text);
  cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)STR_009a8120, r,
                 (void *)first, (void *)second, line, (void *)text);
  cm_fn_0066f4e0((int)&DAT_00ae28bc, title);
  cm_fn_0066f4e0((int)&DAT_00ae26c4, detail, (char *)&DAT_00ae24d0);
  cm_fn_005e8290((const char *)&DAT_00ae28bc, (const char *)&DAT_00ae26c4, 0);
  if (close_file) {
    cm_fn_00945a09((u32)file);
  }
}

/* ---------------- fn_0058e340 ---------------- */
void CM_THISCALL cm_fn_0058e340(u32 *param_1, int *param_2, int *param_3, u32 param_4)
{
  cm_llist_frame fr;
  volatile u32 *file;
  s32 p2, p3, v;

  p2 = *param_2;                 /* 0x0058e356: mov eax, [ebx] */
  p3 = *param_3;                 /* 0x0058e365 / 0x0058e4b2: mov eax, [ebp] */

  if (p2 == -1 && p3 == -1) {
    /* ===== 0x0058e36f: new file, append the record at the end ===== */
    file = cm_fn_00945b00(*param_1, (volatile s8 *)&DAT_009c79b0);
    if (file == 0) {
      /* 0x0058e385: buffer order is swapped on this path */
      cm_llist_fail(0x58, STR_009c7980, fr.local_100, fr.local_200,
                    STR_009c7970, STR_009c78b0, file, 0);
      return;
    }
    /* 0x0058e3ee */
    cm_fn_00946a22((volatile u8 *)file, 0, 2);
    v = cm_fn_00946aae((volatile u8 *)file);
    *param_3 = v;                /* 0x0058e3fe */
    *param_2 = v;                /* 0x0058e401 */
    if (cm_fn_00945ce6(param_4, param_1[1], 1, (u32)file) != 1) {
      /* 0x0058e41f */
      cm_llist_fail(0x65, STR_009c7880, fr.local_200, fr.local_100,
                    STR_009c7774, STR_009c7680, file, 1);
      return;
    }
    /* 0x0058e455 */
    fr.local_208 = -1;
    if (cm_fn_00945ce6((u32)&fr.local_208, 4, 1, (u32)file) != 1) {
      /* 0x0058e478 */
      cm_llist_fail(0x6e, STR_009c784c, fr.local_200, fr.local_100,
                    STR_009c7774, STR_009c7680, file, 1);
      return;
    }
    /* 0x0058e98b */
    cm_fn_00945a09((u32)file);
    return;
  }

  if (p3 != -1) {
    /* ===== 0x0058e5bf: existing file, read the next record ===== */
    file = cm_fn_00945b00(*param_1, (volatile s8 *)&DAT_009c7848);
    if (file == 0) {
      /* 0x0058e5d5 */
      cm_llist_fail(0xa1, STR_009c781c, fr.local_200, fr.local_100,
                    STR_009c7774, STR_009c7680, file, 0);
      return;
    }
    /* 0x0058e60a */
    if (cm_fn_00946a22((volatile u8 *)file, (u32)*param_3, 0) != 0) {
      /* 0x0058e621 */
      cm_llist_fail(0xa8, STR_009c7788, fr.local_200, fr.local_100,
                    STR_009c7774, STR_009c7680, file, 0);
      return;
    }
    /* 0x0058e6b5 */
    cm_fn_00946a22((volatile u8 *)file, param_1[1], 1);
    if (cm_fn_00945bfe((u32)&fr.local_208, 4, 1, (u32)file) != 1) {
      /* 0x0058e6d8 */
      cm_llist_fail(0xb1, STR_009c764c, fr.local_200, fr.local_100,
                    STR_009c7970, STR_009c78b0, file, 1);
      return;
    }
    v = fr.local_208;            /* 0x0058e746: mov eax, [esp+0x10] */
    if (v > -1) {
      /* 0x0058e755 */
      if (cm_fn_00946a22((volatile u8 *)file, (u32)v, 0) != 0) {
        /* 0x0058e765 */
        cm_llist_fail(0xbc, STR_009c7614, fr.local_200, fr.local_100,
                      STR_009c7774, STR_009c7680, file, 1);
        return;
      }
      /* 0x0058e79e */
      if (cm_fn_00945ce6(param_4, param_1[1], 1, (u32)file) != 1) {
        /* 0x0058e7ba */
        cm_llist_fail(0xc4, STR_009c75e4, fr.local_200, fr.local_100,
                      STR_009c7774, STR_009c7680, file, 1);
        return;
      }
      /* 0x0058e7f3 */
      v = fr.local_208;
      fr.local_208 = -1;
      *param_3 = v;
      /* 0x0058e98b */
      cm_fn_00945a09((u32)file);
      return;
    }
    /* 0x0058e803: no pointer stored, append at the end of the file */
    cm_fn_00946a22((volatile u8 *)file, 0, 2);
    fr.local_204 = cm_fn_00946aae((volatile u8 *)file);   /* 0x0058e81a */
    fr.local_208 = -1;                                    /* 0x0058e826 */
    if (cm_fn_00945ce6(param_4, param_1[1], 1, (u32)file) != 1) {
      /* 0x0058e837 */
      cm_llist_fail(0xdb, STR_009c75e4, fr.local_200, fr.local_100,
                    STR_009c7774, STR_009c7680, file, 1);
      return;
    }
    /* 0x0058e871: writes the [-1] next-pointer sentinel stored at L+0 by
       0x0058e826 (`mov dword ptr [esp+0x30], ebx`, ebx = -1).  The source
       slot is L+0 (local_208), not the 256 byte buffer at L+0x008. */
    if (cm_fn_00945ce6((u32)&fr.local_208, 4, 1, (u32)file) != 1) {
      /* 0x0058e888 */
      cm_llist_fail(0xe2, STR_009c75ac, fr.local_200, fr.local_100,
                    STR_009c7774, STR_009c7680, file, 1);
      return;
    }
    /* 0x0058e8be: seek past the record just written */
    {
      u32 off = (u32)*param_3 + param_1[1];
      cm_fn_00946a22((volatile u8 *)file, off, 0);
    }
    if (cm_fn_00945ce6((u32)&fr.local_204, 4, 1, (u32)file) != 1) {
      /* 0x0058e8ea */
      cm_llist_fail(0xec, STR_009c756c, fr.local_200, fr.local_100,
                    STR_009c7774, STR_009c7680, file, 1);
      return;
    }
    /* 0x0058e984 */
    *param_3 = fr.local_204;
    cm_fn_00945a09((u32)file);
    return;
  }

  /* ===== 0x0058e4c4: existing file, overwrite the record at *param_2 ===== */
  file = cm_fn_00945b00(*param_1, (volatile s8 *)&DAT_009c7848);
  if (file == 0) {
    /* 0x0058e4da */
    cm_llist_fail(0x7f, STR_009c781c, fr.local_200, fr.local_100,
                  STR_009c7774, STR_009c7680, file, 0);
    return;
  }
  /* 0x0058e50f */
  if (cm_fn_00946a22((volatile u8 *)file, (u32)*param_2, 0) != 0) {
    /* 0x0058e521 */
    cm_llist_fail(0x86, STR_009c77e8, fr.local_200, fr.local_100,
                  STR_009c7774, STR_009c7680, file, 1);
    return;
  }
  /* 0x0058e55a */
  if (cm_fn_00945ce6(param_4, param_1[1], 1, (u32)file) != 1) {
    /* 0x0058e576 */
    cm_llist_fail(0x8e, STR_009c77bc, fr.local_200, fr.local_100,
                  STR_009c7774, STR_009c7680, file, 1);
    return;
  }
  /* 0x0058e5af -> 0x0058e988 */
  *param_3 = *param_2;
  cm_fn_00945a09((u32)file);
  return;
}
