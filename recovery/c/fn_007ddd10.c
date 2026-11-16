/* fn_007ddd10, preferred-image VA 0x007ddd10, image base 0x00400000, body 1159B.
   Convention: ret 8 (two stack arguments) plus ECX read before written -> thiscall.

   Stack-frame reconstruction (L = esp immediately after `sub esp,0xac`; the SEH
   record occupies L+0xac..L+0xb7, so the saved return address is L+0xb8):
     L+0x02/L+0x03  local bytes set to 0/0xff and later to 1/2
     L+0x04         object C, 8 bytes initialised by cm_fn_0054a110
     L+0x14         the incoming ECX (stored by `mov [esp+0x2c],edi`, reloaded as
                    `mov ecx,[esp+0x24]` before both cm_fn_0068d0d0 calls)
     L+0x18         record handed to cm_fn_00556940 / cm_fn_00557280:
                    +0x00 byte, +0x01 byte, +0x02 word, +0x04 object D,
                    +0x10 byte, +0x11 word, +0x13 flags byte
     L+0x1c         object D (8 bytes initialised by cm_fn_0054a110, then
                    cm_fn_00417c10 writes 8 bytes to it)
     L+0x24..L+0x27 header bytes 0x0c,0xff,0xff,0xff of object D
     L+0x2c         cm_fn_00417c10 output (8 bytes), key object for cm_fn_0054c5b0
     L+0x34         cm_fn_00417c10 output (8 bytes)
     L+0x3c         payload record copied by cm_fn_0068d0d0 (0x70 bytes):
                    +0x00 byte, +0x01 byte, +0x02 word, +0x04 object A (16 bytes),
                    +0x14 object B (8 bytes), +0x18 dword
     L+0x40         object A, L+0x50 object B
     L+0x5c..L+0xaa 0x4f bytes copied from the second argument
     L+0x59 etc.    untouched by this routine

   The flags byte at L+0x2b (record +0x13) is READ at 0x007de0ff before any store
   of that byte on any reachable path: cm_fn_0054a110 initialises only the 8 bytes
   at L+0x1c..L+0x23, the 0x0c/0xff/0xff/0xff reload covers L+0x24..L+0x27, and
   L+0x28..L+0x2a are written after that read.  The original therefore transports
   the indeterminate stack byte; this translation performs the same load
   (volatile, so the load is not folded away) and keeps every bit it does not
   clear: 0x7de10f clears bit 1, 0x7de119 clears bit 2, 0x7de123 ORs bit 2 from
   `*((u8*)param_1+0x48) == 0`, and 0x7de12b stores the result.  Bits 0 and 3..7
   of the unknown byte reach cm_fn_00556940 and cm_fn_00557280 unchanged; nothing
   is zero-filled or narrowed.  The byte is likewise never initialised by any
   callee: cm_fn_00417c10 writes 8 bytes to its receiver and 8 bytes to its first
   argument, and cm_fn_0054a110 writes 8 bytes to its receiver.
   The same is true of L+0xab, the last byte of the cm_fn_0068d0d0 source range
   (0x70 bytes from L+0x3c), which is copied without being written here. */
#include "cmre.h"

/* Callee boundaries stay explicit; every one of these binds to original code. */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
/* The listing uses EAX after both non-initial call sites (0x7dde7a, 0x7ddeb4,
   0x7de034 push the result as the third argument of cm_fn_00417c10), so the
   revived record returns that dword. */
extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1, u32 *param_2, int param_3);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern int CM_FASTCALL cm_fn_0054a3f0(short *param_1);
extern u32 CM_THISCALL cm_fn_00556940(u32 self, volatile u32 key_arg, volatile u32 record_arg);
/* CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   preferred-image VAs 0x007de159/0x007de161/0x007de170 push the three
   stack arguments; 0x007de171 reloads ECX from [0x00ae28f0] immediately
   before call 0x00557280 at 0x007de177. Preserve that separate ECX input. */
extern void CM_THISCALL cm_fn_00557280(u32 self, int param_1, char *param_2, int param_3);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);
extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, volatile u32 create);
extern u32 CM_THISCALL cm_fn_007e8e10(volatile u8 *input, volatile u8 *out, volatile u8 *mode_data, u32 use_mode);

/* `mov ecx,[ecx+7]` is an unaligned dword load; keep it as one 32-bit access. */
typedef struct __attribute__((packed)) cm_packed_u32 {
  u32 v;
} cm_packed_u32;

static u32 cm_load_u32(const u8 *p)
{
  return ((const cm_packed_u32 *)(const void *)p)->v;
}

void CM_THISCALL cm_fn_007ddd10(u32 self, int *param_1, u32 *param_2)
{
  union cm_scratch {
    u8 b[0xac];
    u16 w[0x56];
    u32 d[0x2b];
  } s;
  volatile struct cm_seh {
    void *prev;
    void *handler;
    volatile u32 state;
  } frame;
  union cm_word {
    u16 w;
    u8 b[2];
  } tmp;
  u8 *S;
  u8 *rec;
  u8 al, cl, dl, bl;
  u16 ax, dx;
  u32 v, p, u, i;

  S = s.b;

  /* 0x7ddd10 push -1 / push 0x00960092 / push fs:[0] / mov fs:[0],esp */
  frame.state = 0xffffffffu;
  frame.handler = (void *)&CM_GLOBAL(u8, 0x00960092);
  frame.prev = CM_FS(void *, 0);
  CM_FS(void *, 0) = (void *)&frame;

  /* 0x7ddd38 `mov eax,[esi]`, 0x7ddd3d store of ECX, 0x7ddd41/0x7ddd46 bytes,
     0x7ddd4b cm_fn_007e82e0(this, *param_1, 1). */
  s.d[0x05] = self;
  S[0x02] = 0;
  S[0x03] = 0xff;
  rec = (u8 *)(void *)cm_fn_007e82e0(self, (u32)*param_1, 1);

  /* 0x7ddd52..0x7dde0b: bit moves over rec[0xa]/rec[0xb] with a byte counter in
     bl.  Every `add bl,..` is an 8-bit add (bl wraps). */
  al = rec[0x0a];
  cl = al;
  al >>= 1;
  al &= 1;
  cl &= 0xfe;
  al |= cl;                     /* 0x7ddd5e */
  cl = al;
  bl = al;
  cl >>= 1;
  cl &= 2;
  al &= 0xfd;
  cl |= al;                     /* 0x7ddd6b */
  bl &= 1;
  al = cl;
  al >>= 1;
  dl = al;
  al ^= cl;
  dl &= 1;
  al &= 4;
  bl = (u8)(bl + dl);           /* 0x7ddd7d */
  dl = al;
  dl ^= cl;
  al = dl;
  al >>= 2;
  al &= 1;
  bl = (u8)(bl + al);           /* 0x7ddd8a */
  al = dl;
  al >>= 1;
  al &= 8;
  dl &= 0xf7;
  al |= dl;
  cl = al;
  cl >>= 3;
  cl &= 1;
  bl = (u8)(bl + cl);           /* 0x7ddd9f */
  cl = al;
  cl >>= 1;
  cl &= 0x10;
  al &= 0xef;
  cl |= al;
  al = cl;
  dl = cl;
  al >>= 1;
  al &= 0x20;
  cl &= 0xdf;
  al |= cl;
  dl >>= 4;
  cl = al;
  dl &= 1;
  cl >>= 5;
  bl = (u8)(bl + dl);           /* 0x7dddc4 */
  cl &= 1;
  bl = (u8)(bl + cl);           /* 0x7dddc9 */
  cl = al;
  cl >>= 1;
  cl &= 0x40;
  al &= 0xbf;
  cl |= al;
  dl = cl;
  cl &= 0x7f;
  dl >>= 6;
  dl &= 1;
  bl = (u8)(bl + dl);           /* 0x7ddde1 */
  dl = rec[0x0b];
  al = dl;
  al <<= 7;
  cl |= al;
  rec[0x0a] = cl;               /* 0x7ddded */
  cl = rec[0x0c];
  al = dl;
  al ^= cl;
  cl = rec[0x0a];
  al &= 1;
  al ^= dl;
  rec[0x0b] = al;               /* 0x7dddfe */
  al &= 1;
  cl >>= 7;
  cl = (u8)(cl + al);
  al = *((u8 *)param_1 + 0x2d); /* 0x7dde08 */
  bl = (u8)(bl + cl);           /* 0x7dde0b */

  if (al == 1) {
    /* 0x7dde15..0x7dde41: three cm_fn_0054a110 initialisers at L+0x40, L+0x50
       and L+0x04, interleaved with the SEH scope-state writes. */
    cm_fn_0054a110((u16 *)(void *)(S + 0x40));
    frame.state = 0;
    cm_fn_0054a110((u16 *)(void *)(S + 0x50));
    frame.state = 1;
    cm_fn_0054a110((u16 *)(void *)(S + 0x04));
    *(volatile u8 *)&frame.state = 2;
    /* 0x7dde58 cm_fn_007e8e10(ecx=this, arg1=L+0x3c, arg2=param_1, arg3=1) */
    v = cm_fn_007e8e10((volatile u8 *)(void *)self, (volatile u8 *)(void *)(S + 0x3c),
                       (volatile u8 *)param_1, 1);
    if (v != 0) {
      /* 0x7dde65..0x7dde71 */
      p = cm_fn_0054c5b0((short *)(void *)&CM_GLOBAL(u32, 0x00ae2c90),
                         (u32 *)(void *)(S + 0x0c), 1);
      *(volatile u8 *)&frame.state = 3;
      /* 0x7dde88 cm_fn_00417c10(ecx=L+0x04, arg1=L+0x2c, arg2=p) */
      cm_fn_00417c10((u32 *)(void *)(S + 0x04), (u32 *)(void *)(S + 0x2c),
                     (u16 *)(void *)p);
      *(volatile u8 *)&frame.state = 2;
      ax = (u16)cm_fn_0054a3f0((short *)(void *)(S + 0x04));
      while (ax != 3) {
        p = cm_fn_0054c5b0((short *)(void *)(S + 0x04),
                           (u32 *)(void *)(S + 0x2c), 1);
        *(volatile u8 *)&frame.state = 4;
        cm_fn_00417c10((u32 *)(void *)(S + 0x04), (u32 *)(void *)(S + 0x0c),
                       (u16 *)(void *)p);
        *(volatile u8 *)&frame.state = 2;
        ax = (u16)cm_fn_0054a3f0((short *)(void *)(S + 0x04));
      }
      /* 0x7ddee2..0x7ddef0 cm_fn_00417c10(ecx=L+0x50, arg1=L+0x2c, arg2=L+0x04) */
      cm_fn_00417c10((u32 *)(void *)(S + 0x50), (u32 *)(void *)(S + 0x2c),
                     (u16 *)(void *)(S + 0x04));
      /* 0x7ddf05..0x7ddf14 are the header bytes of object A (L+0x48..L+0x4b). */
      S[0x48] = 0x0c;
      S[0x49] = 0xff;
      S[0x4a] = 0xff;
      S[0x4b] = 0xff;
      /* 0x7ddf1d rep movsd 0x13, 0x7ddf25 movsw, 0x7ddf2b movsb */
      for (i = 0; i < 0x13; i++) {
        s.d[0x17 + i] = param_2[i];
      }
      s.w[0x54] = *(u16 *)(void *)((u8 *)param_2 + 0x4c);
      S[0xaa] = *((u8 *)param_2 + 0x4e);
      u = s.d[0x05];                                   /* 0x7ddf1f */
      u = cm_load_u32((const u8 *)((u8 *)(void *)u) + 7); /* 0x7ddf27 */
      /* 0x7ddf2c: dst=[this+7], src=L+0x3c, count=0x70.  The original leaves EDX
         undefined at this call site (the preceding call clobbers it and no
         instruction loads it); the recovered second parameter is therefore not
         transportable and 0 is passed. */
      cm_fn_0068d0d0((volatile u8 *)(void *)u, 0, (const u8 *)(void *)(S + 0x3c), 0x70);
    }
    /* 0x7ddf38..0x7ddf4a */
    S[0x02] = 1;
    S[0x03] = 2;
    s.d[0x01] = 1;
    frame.state = 0xffffffffu;
  }

  /* 0x7ddf55 `mov al,[esi+0x2c]; test al,al; jbe` */
  al = *((u8 *)param_1 + 0x2c);
  if (al == 0) {
    rec[0x0c] &= 0xfe;           /* 0x7de0b0 */
  }
  else {
    rec[0x0c] |= 1;              /* 0x7ddf60/0x7ddf6b */
    bl = (u8)(bl + 1);           /* 0x7ddf66 */
    if (bl == 3) {               /* 0x7ddf68 */
      cm_fn_0054a110((u16 *)(void *)(S + 0x40));
      frame.state = 5;
      cm_fn_0054a110((u16 *)(void *)(S + 0x50));
      v = *(volatile u32 *)(void *)param_1;       /* 0x7ddf91 mov edx,[esi] */
      frame.state = 6;
      s.d[0x16] = v;                              /* 0x7ddfac L+0x58 */
      S[0x3c] = 1;                                /* 0x7ddfb0 */
      S[0x3d] = 0;                                /* 0x7ddfb5 */
      s.w[0x1f] = 0xffff;                         /* 0x7ddfba L+0x3e */
      /* 0x7ddfc1 cm_fn_00417c10(ecx=L+0x40, arg1=L+0x2c, arg2=0x00ae2c90) */
      cm_fn_00417c10((u32 *)(void *)(S + 0x40), (u32 *)(void *)(S + 0x2c),
                     (u16 *)(void *)&CM_GLOBAL(u32, 0x00ae2c90));
      S[0x48] = 0x0c;
      S[0x49] = 0xff;
      S[0x4a] = 0xff;
      S[0x4b] = 0xff;
      S[0x4c] = 1;
      tmp.w = 3;
      S[0x4d] = tmp.b[0];                         /* 0x7ddfef word L+0x4d */
      S[0x4e] = tmp.b[1];
      for (i = 0; i < 0x13; i++) {
        s.d[0x17 + i] = param_2[i];
      }
      s.w[0x54] = *(u16 *)(void *)((u8 *)param_2 + 0x4c);
      S[0xaa] = *((u8 *)param_2 + 0x4e);
      p = cm_fn_0054c5b0((short *)(void *)&CM_GLOBAL(u32, 0x00ae2c90),
                         (u32 *)(void *)(S + 0x0c), 1);
      bl = 7;                                     /* 0x7de00c */
      *(volatile u8 *)&frame.state = 7;
      ax = (u16)cm_fn_0054a3f0((short *)(void *)(S + 0x0c));
      while (ax != 3) {
        p = cm_fn_0054c5b0((short *)(void *)(S + 0x0c),
                           (u32 *)(void *)(S + 0x2c), 1);
        *(volatile u8 *)&frame.state = 8;
        cm_fn_00417c10((u32 *)(void *)(S + 0x0c), (u32 *)(void *)(S + 0x34),
                       (u16 *)(void *)p);
        *(volatile u8 *)&frame.state = bl;
        ax = (u16)cm_fn_0054a3f0((short *)(void *)(S + 0x0c));
      }
      /* 0x7de061..0x7de06f cm_fn_00417c10(ecx=L+0x50, arg1=L+0x34, arg2=L+0x0c) */
      cm_fn_00417c10((u32 *)(void *)(S + 0x50), (u32 *)(void *)(S + 0x34),
                     (u16 *)(void *)(S + 0x0c));
      u = s.d[0x05];                                   /* 0x7de074 */
      u = cm_load_u32((const u8 *)((u8 *)(void *)u) + 7);
      cm_fn_0068d0d0((volatile u8 *)(void *)u, 0, (const u8 *)(void *)(S + 0x3c), 0x70);
      cl = rec[0x0b];                              /* 0x7de087 */
      al = rec[0x0c];
      cl &= 0xfe;
      al &= 0xfe;
      rec[0x0a] = 0;
      rec[0x0b] = cl;
      rec[0x0c] = al;
      frame.state = 0xffffffffu;
    }
  }

  /* 0x7de0b4: only the `param_1[0x2d]==1` path sets L+0x02. */
  if (S[0x02] != 0) {
    cm_fn_0054a110((u16 *)(void *)(S + 0x1c));     /* 0x7de0c0 */
    dl = S[0x02];                                  /* 0x7de0c9 */
    frame.state = 9;
    S[0x18] = dl;                                  /* 0x7de0e6 */
    s.w[0x0d] = 0xffff;                            /* 0x7de0ea L+0x1a */
    S[0x19] = 0;                                   /* 0x7de0f1 */
    /* 0x7de0f6 cm_fn_00417c10(ecx=L+0x1c, arg1=L+0x34, arg2=0x00ae2c90) */
    cm_fn_00417c10((u32 *)(void *)(S + 0x1c), (u32 *)(void *)(S + 0x34),
                   (u16 *)(void *)&CM_GLOBAL(u32, 0x00ae2c90));
    cl = S[0x03];                                  /* 0x7de0fb */
    /* 0x7de0ff: load of the scratch flags byte that no reachable path stores
       before this point (see header comment).  Volatile keeps the original load
       and its indeterminate value instead of folding it to a constant. */
    al = *(volatile u8 *)(void *)(S + 0x2b);
    S[0x28] = cl;                                  /* 0x7de103 */
    cl = *((u8 *)param_1 + 0x48);                  /* 0x7de107 */
    dx = s.w[0x02];                                /* 0x7de10a word L+0x04 */
    al &= 0xfd;                                    /* 0x7de10f */
    cl = (u8)((cl == 0) ? 1 : 0);                  /* 0x7de111/0x7de113 */
    cl &= 1;                                       /* 0x7de116 */
    al &= 0xfb;                                    /* 0x7de119 */
    cl <<= 2;                                      /* 0x7de11b */
    tmp.w = dx;
    S[0x29] = tmp.b[0];                            /* 0x7de11e word L+0x29 */
    S[0x2a] = tmp.b[1];
    cl |= al;                                      /* 0x7de123 */
    v = *(volatile u32 *)(void *)param_1;          /* 0x7de125 mov eax,[esi] */
    S[0x2b] = cl;                                  /* 0x7de12b */
    u = CM_GLOBAL(u32, 0x00ae28f0);                /* 0x7de12f */
    S[0x24] = 0x0c;                                /* 0x7de137..0x7de146 */
    S[0x25] = 0xff;
    S[0x26] = 0xff;
    S[0x27] = 0xff;
    /* 0x7de14b cm_fn_00556940(ecx=[0xae28f0], arg1=*param_1, arg2=L+0x18) */
    cm_fn_00556940(u, v, (u32)(void *)(S + 0x18));
    v = *(volatile u32 *)(void *)param_1;          /* 0x7de157 */
    /* 0x7de15e..0x7de16d: base + 110 * *param_1, 32-bit wrapping arithmetic. */
    v = CM_GLOBAL(volatile u32, 0x00ae23bc) + v * 110u; /* 0x7de168..0x7de16d */
    /* 0x7de171: reload after cm_fn_00556940 and after the base read.
       The earlier receiver from 0x7de12f is not this call's input. */
    u = CM_GLOBAL(volatile u32, 0x00ae28f0);
    /* 0x7de177: ECX=u, stack=(base+110*x, L+0x18, param_2). */
    cm_fn_00557280(u, (int)v,
                   (char *)(void *)(S + 0x18), (int)(void *)param_2);
  }

  /* 0x7de17c..0x7de194 epilogue: unlink the SEH record. */
  CM_FS(void *, 0) = frame.prev;
}
