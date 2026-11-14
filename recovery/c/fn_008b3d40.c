/* fn_008b3d40 -- training manager initialiser (preferred-image VA 0x008b3d40),
   recovered from the pinned CM 01/02 3.9.68 image; see `cmre brief fn_008b3d40`.

   The listing keeps its own frame: `sub esp,0xc0c` with ebx/ebp/esi/edi
   saved below it.  Let L be the address just after `sub esp,0xc0c`; the loop
   body then runs with esp = L-16 and the locals are

     L+0x000  dword  outer counter
     L+0x004  byte   inner counter (byte-written, read back as a dword)
     L+0x008  dword  outer limit (16)
     L+0x00c  twelve 256-byte scratch buffers, one pair per range check

   Every one of the six range checks can fall into the same diagnostic.
   cm_fn_00944cff splits the module path 0x00a8ccd0 into a file stem and an
   extension, cm_fn_0090d130 returns the version string (it ignores its three
   arguments and returns 0x00ad4394), and the variadic formatter
   cm_fn_00944c9f writes "v%s %s %s %d" (format 0x009870e8) with the version,
   the stem, the extension and 0x68f into the message buffer 0x00ae24d0, which
   cm_fn_005e8290 then shows.  The three words the cm_fn_0090d130 call leaves
   on the stack are the formatter's last three varargs, so they are passed
   here as the stem, the extension and 0x68f.

   The range-check error path ends in cm_fn_008b4e50 with a null receiver,
   which faults writing the receiver's first dword at address zero. */
#include "cmre.h"

#define DAT_00ae2364 CM_GLOBAL(volatile int, 0x00ae2364)
#define DAT_00ae23bc CM_GLOBAL(volatile int, 0x00ae23bc)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern void CM_THISCALL cm_fn_008b2230(volatile u8 *self);
extern u32 CM_THISCALL cm_fn_008b3930(u32 self, u32 value, u32 kind);
extern void CM_THISCALL cm_fn_008b4e50(u8 *self, s8 arg1, s8 arg2);
extern char * cm_fn_0090d130(...);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* The original frame, kept in one allocation so the related scratch buffers
   stay adjacent exactly as the listing places them. */
struct cm_scratch {
  u32 counter;            /* L+0x000 */
  volatile u32 loop_word; /* L+0x004: low byte written, whole word read back */
  u32 limit;              /* L+0x008 */
  u8 buf[12][256];        /* L+0x00c .. L+0xc0b */
};

/* The out-of-range branch of one range check (008b3dff .. 008b3e5b and its
   five siblings).  `stem` receives the file stem and `ext` the extension of
   the module path; the formatter's varargs are the version string, the stem,
   the extension and 0x68f. */
static void cm_range_error(u8 *stem, u8 *ext)
{
  u32 version;

  cm_fn_00944cff((u32)0x00a8ccd0, 0u, 0u, (void *)stem, (void *)ext);
  version = (u32)cm_fn_0090d130((void *)stem, (void *)ext, (u32)0x68f);
  cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, version,
                 (u32)stem, (u32)ext, (u32)0x68f);
  cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
  DAT_00b67a34 = 0;
}

void CM_THISCALL cm_fn_008b3d40(u8 *self)
{
  struct cm_scratch sc;
  u32 count;
  u32 idx;
  u32 elem;
  u32 value;
  u32 item;
  u8 *target;

  /* 008b3d50 .. 008b3daa: the seven initialisers, receiver and arguments. */
  cm_fn_008b4e50(self, 0, 0);
  cm_fn_008b4e50(self + 0x9b, 1, 1);
  cm_fn_008b4e50(self + 0x136, 2, 2);
  cm_fn_008b4e50(self + 0x1d1, 3, 3);
  cm_fn_008b4e50(self + 0x26c, 4, 4);
  cm_fn_008b4e50(self + 0x307, 5, 5);
  cm_fn_008b4e50(self + 0x3a2, 6, 6);

  sc.counter = 0;
  sc.limit = 0x10;

  do {
    /* 008b3dbb .. 008b3ddc: the element is taken from the tail of the
       array, its value selects the index used by the six checks. */
    count = (u32)DAT_00ae2364;
    idx = count - sc.counter - 1u;
    elem = (u32)DAT_00ae23bc + idx * 110u;

    /* 008b3ddf .. 008b3e66: first check, receiver self+0x43d+1550*index. */
    count = (u32)DAT_00ae2364;
    value = (u32)*(volatile int *)elem;
    idx = count - value - 1u;
    if ((s32)idx < 0 || (s32)idx >= 16) {
      cm_range_error(sc.buf[2], sc.buf[8]);
      target = (u8 *)0;
    } else {
      target = self + 0x43d + idx * 1550u;
    }
    cm_fn_008b4e50(target, 0, -1);

    /* 008b3e6b .. 008b3ef9: second check, receiver self+0x4d8+1550*index. */
    count = (u32)DAT_00ae2364;
    value = (u32)*(volatile int *)elem;
    idx = count - value - 1u;
    if ((s32)idx < 0 || (s32)idx >= 16) {
      cm_range_error(sc.buf[4], sc.buf[10]);
      target = (u8 *)0;
    } else {
      target = self + 0x4d8 + idx * 1550u;
    }
    cm_fn_008b4e50(target, 1, 0);

    /* 008b3efe .. 008b3f87: third check, receiver self+0x573+1550*index. */
    count = (u32)DAT_00ae2364;
    value = (u32)*(volatile int *)elem;
    idx = count - value - 1u;
    if ((s32)idx < 0 || (s32)idx >= 16) {
      cm_range_error(sc.buf[6], sc.buf[0]);
      target = (u8 *)0;
    } else {
      target = self + 0x573 + idx * 1550u;
    }
    cm_fn_008b4e50(target, 2, 1);

    /* 008b3f8c .. 008b401a: fourth check, receiver self+1550*(index+1). */
    count = (u32)DAT_00ae2364;
    value = (u32)*(volatile int *)elem;
    idx = count - value - 1u;
    if ((s32)idx < 0 || (s32)idx >= 16) {
      cm_range_error(sc.buf[3], sc.buf[1]);
      target = (u8 *)0;
    } else {
      target = self + (idx + 1u) * 1550u;
    }
    cm_fn_008b4e50(target, 3, 2);

    /* 008b401f .. 008b40ae: fifth check, receiver self+0x6a9+1550*index. */
    count = (u32)DAT_00ae2364;
    value = (u32)*(volatile int *)elem;
    idx = count - value - 1u;
    if ((s32)idx < 0 || (s32)idx >= 16) {
      cm_range_error(sc.buf[7], sc.buf[5]);
      target = (u8 *)0;
    } else {
      target = self + 0x6a9 + idx * 1550u;
    }
    cm_fn_008b4e50(target, 4, 3);

    /* 008b40b3 .. 008b4142: sixth check, receiver self+0x744+1550*index. */
    count = (u32)DAT_00ae2364;
    value = (u32)*(volatile int *)elem;
    idx = count - value - 1u;
    if ((s32)idx < 0 || (s32)idx >= 16) {
      cm_range_error(sc.buf[11], sc.buf[9]);
      target = (u8 *)0;
    } else {
      target = self + 0x744 + idx * 1550u;
    }
    cm_fn_008b4e50(target, 5, 4);

    /* 008b4147 .. 008b416c: two more entries built from the element. */
    item = cm_fn_008b3930((u32)self, elem, 6u);
    cm_fn_008b4e50((u8 *)item, 6, 5);
    item = cm_fn_008b3930((u32)self, elem, 7u);
    cm_fn_008b4e50((u8 *)item, 7, 6);

    /* 008b4171 .. 008b4197: the inner loop over the mirrored byte counter. */
    {
      u8 bl = 8;

      ((volatile u8 *)&sc.loop_word)[0] = bl;
      for (;;) {
        u32 word = sc.loop_word;

        item = cm_fn_008b3930((u32)self, elem, word);
        cm_fn_008b4e50((u8 *)item, (s8)word, 7);
        bl = (u8)(bl + 1);
        ((volatile u8 *)&sc.loop_word)[0] = bl;
        if (!((s8)bl < 10))
          break;
      }
    }

    /* 008b4199 .. 008b41ab */
    sc.counter++;
    sc.limit--;
  } while (sc.limit != 0);

  /* 008b41b1 .. 008b41b3 */
  cm_fn_008b2230(self);
}
