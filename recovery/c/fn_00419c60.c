/* fn_00419c60 - Championship Manager 01/02 3.9.68 (cm0102.exe, SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86),
 * preferred image base 0x00400000.
 *
 * Original instruction range: VA 0x00419c60 .. 0x0041a321; dispatch tables
 * at VA 0x0041a324, 0x0041a338 and 0x0041a34c.
 *
 * Entry ABI, taken from the listing only:
 *   - `ret 0xc` (0x0041a125, 0x0041a321) pops 12 bytes, so the routine
 *     takes three stack arguments and cleans them up (stdcall).
 *   - arg1 (entry esp + 4) is read at 0x00419c78 (`mov edi, [esp + 0x1c0]`)
 *     and is the receiver of cm_fn_00785830.
 *   - arg2 (entry esp + 8) is read at 0x00419c60 (`mov eax, [esp + 8]`) and
 *     selects the branch (`test eax, eax` / `je 0x0041a128`).
 *   - arg3 (entry esp + 0xc) is read at 0x0041a063 (`mov ebp, [esp + 0x1c4]`)
 *     and 0x0041a28d (`mov ecx, [esp + 0x1c4]`) and is the receiver of
 *     cm_fn_004ae660 and cm_fn_004ae8a0.
 *   - No register is read before it is written, so there is no thiscall or
 *     fastcall receiver of its own.
 *
 * The frame of the original holds one 0x190-byte scratch buffer (frame
 * offset +0x18, the address handed to cm_fn_0053e140 and to
 * cm_fn_0066f4e0); it is kept as a single scratch array here.
 *
 * The three DWORD dispatch tables are read from the original image at their
 * own VAs, each behind its own guard (0x00419d43 / 0x00419f0a / 0x0041a1a5,
 * all `cmp eax, 4` + `ja`), and the table entry that is read selects the
 * case body, so the table access itself is kept.
 *
 * Validated: `cmre test fn_00419c60 --c` passes at -O0 and -O2 against the
 * pinned 3.9.68 binary (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
 * Acceptance of the record is the farm/controller step (`cmre accept --c`).
 */
#include "cmre.h"

/* Original globals (preferred-image VAs). */
#define G_AE23A8 CM_GLOBAL(volatile u32, 0x00ae23a8)
#define G_AE23B4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_AE23BC CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_B74C26 CM_GLOBAL(volatile u32, 0x00b74c26)
#define G_B77C7A CM_GLOBAL(volatile s16, 0x00b77c7a)

/* Format strings in the original image (immediate addresses in the listing). */
#define S_0098811c ((char *)0x0098811c)
#define S_00988170 ((char *)0x00988170)
#define S_009881a0 ((char *)0x009881a0)
#define S_009881d4 ((char *)0x009881d4)
#define S_00988208 ((char *)0x00988208)
#define S_00988248 ((char *)0x00988248)
#define S_00988310 ((char *)0x00988310)
#define S_009883e0 ((char *)0x009883e0)
#define S_009884cc ((char *)0x009884cc)
#define S_009885a4 ((char *)0x009885a4)
#define S_00988698 ((char *)0x00988698)
#define S_009887bc ((char *)0x009887bc)
#define S_009888b4 ((char *)0x009888b4)
#define S_00988990 ((char *)0x00988990)
#define S_00988a88 ((char *)0x00988a88)
#define S_00988b9c ((char *)0x00988b9c)
#define S_00988ccc ((char *)0x00988ccc)

/* Destination address pushed by the original (0x00419da1 and friends). */
#define DST_00DE1F64 0x00de1f64u

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
/* The original pushes three, six or nine arguments at 0x00419da6 and the
 * other call sites, so the callee is variadic (format plus operands). */
extern char * CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

/* Wrapping 32-bit address arithmetic, as the original instructions do. */
/* 0x00419ed1 and friends: base + index + 0x244 * index (581 records). */
#define A_B4(idx) (G_AE23B4 + (u32)(idx) + 0x244u * (u32)(idx))
/* 0x00419d64 and friends: base + 0x122 * index (290 records). */
#define A_A8(idx) (G_AE23A8 + 0x122u * (u32)(idx))
/* Signed byte load (movsx) at a computed 32-bit address. */
#define LD_S8(a) (*(s8 *)(void *)(unsigned long)(u32)(a))
/* DWORD load at a computed 32-bit address. */
#define LD_U32(a) (*(volatile u32 *)(void *)(unsigned long)(u32)(a))

void CM_STDCALL cm_fn_00419c60(void *arg1, u32 arg2, u32 arg3)
{
  u8 buf[0x190];                /* frame slot +0x18 of the original */
  u32 r0, r1, r2, r3, r4, r5, r6, r7, r8;
  u32 tgt;

  if (arg2 != 0) {
    /* --- 0x00419c78 .. 0x00419ce7: nine receiver calls (index 0..8) --- */
    r0 = (u32)cm_fn_00785830(arg1, 0);
    r1 = (u32)cm_fn_00785830(arg1, 1);
    r2 = (u32)cm_fn_00785830(arg1, 2);
    r3 = (u32)cm_fn_00785830(arg1, 3);
    r4 = (u32)cm_fn_00785830(arg1, 4);
    r5 = (u32)cm_fn_00785830(arg1, 5);
    r6 = (u32)cm_fn_00785830(arg1, 6);
    r7 = (u32)cm_fn_00785830(arg1, 7);
    r8 = (u32)cm_fn_00785830(arg1, 8);

    /* --- 0x00419cf6: cm_fn_0053e140(r1, r2, r3, buf, 0x190, 1, r0) --- */
    cm_fn_0053e140(r1, r2, r3, (u32)(void *)buf, 0x190u, 1u, r0);

    /* --- 0x00419d0f .. 0x00419d3d: table probe and equality test --- */
    {
      u32 e = (u32)(s32)G_B77C7A;   /* movsx eax, word ptr [0x00b77c7a] */
      u32 p;
      int same = 0;
      e = (e + e + e) << 8;          /* lea ecx,[eax+eax*2] / shl ecx,8 */
      /* 0x00419d1f: mov eax, dword ptr [ecx + 0x00b74c26] - the indexed
       * element of the table at 0x00b74c26, not the table's first entry. */
      p = LD_U32(0x00b74c26u + e);
      if (p != 0) {
        u32 p2 = LD_U32(p + 0x39);   /* mov eax, [eax + 0x39] */
        if (p2 != 0) {
          u32 v = LD_U32(p2);        /* mov ebp, [eax] */
          same = (r5 == v);          /* cmp esi, ebp / sete dl */
        }
      }
      if (same) {
        /* --- 0x00419f0a: second dispatch, table at 0x0041a338 --- */
        if (r6 <= 4) {
          tgt = CM_GLOBAL(u32, 0x0041a338 + 4u * r6);
          switch (tgt) {
          case 0x0041a04c:          /* index 0 */
            /* 0x0041a04c: three arguments */
            cm_fn_0066f4e0((int)DST_00DE1F64, S_00988248, (u32)(void *)buf);
            break;
          case 0x0041a015:          /* index 1 */
            {
              /* 0x0041a015: six arguments */
              u32 a = A_B4(r5);
              s8 c1 = LD_S8(a + 0x52);
              u32 p5 = a + 0x38;
              /* 0x0041a030 pushes the record pointer first, the buffer second:
               * the operand order is (buffer, record). */
              cm_fn_0066f4e0((int)DST_00DE1F64, S_00988310, (int)c1, (int)c1,
                             (u32)(void *)buf, p5);
            }
            break;
          case 0x00419fc6:          /* index 2 */
            {
              /* 0x00419fc6: nine arguments */
              u32 base = G_AE23B4;
              u32 a5 = base + r5 + 0x244u * r5;
              u32 a4 = base + r4 + 0x244u * r4;
              s8 c1 = LD_S8(a4 + 0x52);
              u32 p5 = a5 + 0x38;
              s8 c2 = LD_S8(p5 + 0x1a);
              u32 p4 = a4 + 0x38;
              cm_fn_0066f4e0((int)DST_00DE1F64, S_009883e0, (int)c1, (int)c1,
                             (int)c2, (int)c2, (u32)(void *)buf, p4, p5);
            }
            break;
          case 0x0041a063:          /* index 3: falls into the tail */
            break;
          case 0x00419f1e:          /* index 4 */
            {
              /* 0x00419f1e: cmp [esp + 0x20], 7 / jge 0x00419f7a */
              if ((s32)r7 < 7) {
                /* 0x00419f25: nine arguments */
                u32 a5 = A_B4(r5);
                u32 a8 = A_A8(r8);
                s8 c1 = LD_S8(a8 + 0x52);
                u32 p5 = a5 + 0x38;
                s8 c2 = LD_S8(p5 + 0x1a);
                u32 p8 = a8 + 0x38;
                cm_fn_0066f4e0((int)DST_00DE1F64, S_00988698, (int)c1, (int)c1,
                               (int)c2, (int)c2, (u32)(void *)buf, p8, p5);
              } else if (r8 == 0xffffffffu) {
                /* 0x00419fb7: three arguments */
                cm_fn_0066f4e0((int)DST_00DE1F64, S_009884cc, (u32)(void *)buf);
              } else {
                /* 0x00419f7f: six arguments */
                u32 a8 = A_A8(r8);
                s8 c1 = LD_S8(a8 + 0x52);
                u32 p8 = a8 + 0x38;
                cm_fn_0066f4e0((int)DST_00DE1F64, S_009885a4, (int)c1, (int)c1,
                               p8, (u32)(void *)buf);
              }
            }
            break;
          default:
            break;
          }
        }
      } else {
        /* --- 0x00419d43: first dispatch, table at 0x0041a324 --- */
        if (r6 <= 4) {
          tgt = CM_GLOBAL(u32, 0x0041a324 + 4u * r6);
          switch (tgt) {
          case 0x00419ed1:          /* index 0 */
            {
              /* 0x00419ed1: six arguments */
              u32 a = A_B4(r5);
              s8 c1 = LD_S8(a + 0x52);
              u32 p5 = a + 0x38;
              cm_fn_0066f4e0((int)DST_00DE1F64, S_009887bc, (int)c1, (int)c1,
                             p5, (u32)(void *)buf);
            }
            break;
          case 0x00419e98:          /* index 1 */
            {
              /* 0x00419e98: six arguments */
              u32 a = A_B4(r5);
              s8 c1 = LD_S8(a + 0x52);
              u32 p5 = a + 0x38;
              cm_fn_0066f4e0((int)DST_00DE1F64, S_009888b4, (int)c1, (int)c1,
                             p5, (u32)(void *)buf);
            }
            break;
          case 0x00419e46:          /* index 2 */
            {
              /* 0x00419e46: nine arguments */
              u32 base = G_AE23B4;
              u32 a4 = base + r4 + 0x244u * r4;
              u32 a5 = base + r5 + 0x244u * r5;
              s8 c1 = LD_S8(a5 + 0x52);
              u32 p4 = a4 + 0x38;
              s8 c2 = LD_S8(p4 + 0x1a);
              u32 p5 = a5 + 0x38;
              cm_fn_0066f4e0((int)DST_00DE1F64, S_00988990, (int)c1, (int)c1,
                             (int)c2, (int)c2, p5, (u32)(void *)buf, p4);
            }
            break;
          case 0x0041a063:          /* index 3: falls into the tail */
            break;
          case 0x00419d57:          /* index 4 */
            {
              /* 0x00419d57: cmp [esp + 0x20], 7 / jge 0x00419db3 */
              if ((s32)r7 < 7) {
                /* 0x00419d5e: nine arguments */
                u32 a8 = A_A8(r8);
                u32 a5 = A_B4(r5);
                s8 c1 = LD_S8(a5 + 0x52);
                u32 p8 = a8 + 0x38;
                s8 c2 = LD_S8(p8 + 0x1a);
                u32 p5 = a5 + 0x38;
                cm_fn_0066f4e0((int)DST_00DE1F64, S_00988ccc, (int)c1, (int)c1,
                               (int)c2, (int)c2, p5, (u32)(void *)buf, p8);
              } else if (r8 == 0xffffffffu) {
                /* 0x00419e0d: six arguments */
                u32 a5 = A_B4(r5);
                s8 c1 = LD_S8(a5 + 0x52);
                u32 p5 = a5 + 0x38;
                cm_fn_0066f4e0((int)DST_00DE1F64, S_00988a88, (int)c1, (int)c1,
                               p5, (u32)(void *)buf);
              } else {
                /* 0x00419db8: nine arguments */
                u32 a8 = A_A8(r8);
                u32 a5 = A_B4(r5);
                s8 c1 = LD_S8(a5 + 0x52);
                u32 p8 = a8 + 0x38;
                s8 c2 = LD_S8(p8 + 0x1a);
                u32 p5 = a5 + 0x38;
                cm_fn_0066f4e0((int)DST_00DE1F64, S_00988b9c, (int)c1, (int)c1,
                               (int)c2, (int)c2, p5, (u32)(void *)buf, p8);
              }
            }
            break;
          default:
            break;
          }
        }
      }
    }

    /* --- 0x0041a063: common tail of the non-zero branch --- */
    cm_fn_004ae660((void *)arg3, DST_00DE1F64);
    {
      u32 person = 110u * r0 + G_AE23BC;   /* lea ecx, [edi + eax] */
      u32 res = cm_fn_0053e990(person, r1, r2, r3, 0);
      if (res != 0) {
        u32 v = 110u * r0 + G_AE23BC;       /* add edi, edx (fresh read) */
        cm_fn_004ae8a0(arg3, (const char *)(void *)buf, 0x7da, v);
      }
    }
    if ((s32)r4 > -1) {                    /* cmp ebx, -1 / jle */
      u32 a = A_B4(r4);
      cm_fn_004ae8a0(arg3, (const char *)(void *)(a + 4), 0x7d5, a);
    }
    if ((s32)r5 > -1) {                    /* cmp esi, -1 / jle */
      u32 a = A_B4(r5);
      cm_fn_004ae8a0(arg3, (const char *)(void *)(a + 4), 0x7d5, a);
    }
  } else {
    u32 q0, q1, q2, q3, q5, q6;

    /* --- 0x0041a128 .. 0x0041a17e: eight receiver calls --- */
    q0 = (u32)cm_fn_00785830(arg1, 0);
    q1 = (u32)cm_fn_00785830(arg1, 1);
    q2 = (u32)cm_fn_00785830(arg1, 2);
    q3 = (u32)cm_fn_00785830(arg1, 3);
    (void)cm_fn_00785830(arg1, 4);
    q5 = (u32)cm_fn_00785830(arg1, 5);
    (void)cm_fn_00785830(arg1, 6);
    q6 = (u32)cm_fn_00785830(arg1, 6);

    /* --- 0x0041a183: cm_fn_0053e140(q1, q2, q3, buf, 0x190, 5, q0) --- */
    cm_fn_0053e140(q1, q2, q3, (u32)(void *)buf, 0x190u, 5u, q0);

    /* --- 0x0041a1a5: dispatch, table at 0x0041a34c --- */
    if (q6 <= 4) {
      tgt = CM_GLOBAL(u32, 0x0041a34c + 4u * q6);
      switch (tgt) {
      case 0x0041a258:              /* index 0 */
        {
          /* 0x0041a258: six arguments */
          u32 a = A_B4(q5);
          s8 c1 = LD_S8(a + 0x52);
          u32 p5 = a + 0x38;
          /* 0x0041a273 pushes the record pointer first, the buffer second. */
          cm_fn_0066f4e0((int)DST_00DE1F64, S_0098811c, (int)c1, (int)c1,
                         (u32)(void *)buf, p5);
        }
        break;
      case 0x0041a218:              /* index 1 */
        {
          /* 0x0041a218: idiv by 3 of the first receiver result */
          s32 rem = (s32)q0 % 3;
          if (rem == 0) {
            /* 0x0041a23f: three arguments */
            cm_fn_0066f4e0((int)DST_00DE1F64, S_009881d4, (u32)(void *)buf);
          } else {
            /* 0x0041a226: three arguments */
            cm_fn_0066f4e0((int)DST_00DE1F64, S_00988170, (u32)(void *)buf);
          }
        }
        break;
      case 0x0041a1d1:              /* index 2 */
        {
          /* 0x0041a1d1: idiv by 3 of the first receiver result */
          s32 rem = (s32)q0 % 3;
          if (rem == 0) {
            /* 0x0041a1ff: three arguments */
            cm_fn_0066f4e0((int)DST_00DE1F64, S_009881a0, (u32)(void *)buf);
          } else if (rem == 1) {
            /* 0x0041a1e3: three arguments */
            cm_fn_0066f4e0((int)DST_00DE1F64, S_009881d4, (u32)(void *)buf);
          } else {
            /* 0x0041a226: three arguments */
            cm_fn_0066f4e0((int)DST_00DE1F64, S_00988170, (u32)(void *)buf);
          }
        }
        break;
      case 0x0041a28d:              /* index 3: falls into the tail */
        break;
      case 0x0041a1b5:              /* index 4 */
        /* 0x0041a1b5: three arguments */
        cm_fn_0066f4e0((int)DST_00DE1F64, S_00988208, (u32)(void *)buf);
        break;
      default:
        break;
      }
    }

    /* --- 0x0041a28d: common tail of the zero branch --- */
    cm_fn_004ae660((void *)arg3, DST_00DE1F64);
    {
      u32 person = 110u * q0 + G_AE23BC;   /* lea edx, [esi + ecx] */
      u32 res = cm_fn_0053e990(person, q1, q2, q3, 0);
      if (res != 0) {
        u32 v = 110u * q0 + G_AE23BC;       /* add esi, eax (fresh read) */
        cm_fn_004ae8a0(arg3, (const char *)(void *)buf, 0x7da, v);
      }
    }
    if ((s32)q5 > -1) {                    /* cmp ebx, -1 / jle */
      u32 a = A_B4(q5);
      cm_fn_004ae8a0(arg3, (const char *)(void *)(a + 4), 0x7d5, a);
    }
  }
}
