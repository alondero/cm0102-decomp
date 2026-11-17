/* Translation of fn_008c39f0 (VA 0x008c39f0, preferred image base 0x00400000).
   Follows the original listing instruction by instruction.

   The 50-DWORD roster buffer at [esp+0x110] is deliberately left
   uninitialized on the nation-selected (mask == 4) path, exactly as the
   original: the same 50 DWORDs are walked at 0x008c3ac7 with no zero-fill, no
   narrowing and no "empty roster" assumption.  The full 32-bit EAX tail value
   (AL = outer+1, upper bits zero) and the signed 16-bit delta/counters are
   preserved. */
#include "cmre.h"

#define G_AE2364 CM_GLOBAL(int, 0x00ae2364)
#define G_AE23B4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_AE23BC CM_GLOBAL(u32, 0x00ae23bc)
#define G_AE1080 CM_GLOBAL(u32, 0x00ae1080)
#define G_AE2C74 CM_GLOBAL(u32, 0x00ae2c74)
#define G_AE1050 CM_GLOBAL(u32, 0x00ae1050)
#define G_AE28F0 CM_GLOBAL(u32, 0x00ae28f0)
#define G_B63D28 CM_GLOBAL(u32, 0x00b63d28)
#define G_B74318 CM_GLOBAL(u32, 0x00b74318)

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32 self, u32 argument);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_00541f90(u32 club, void *output, u32 extended);
extern u32 CM_THISCALL cm_fn_0055d270(u32 self, volatile u32 person, u32 club_key,
                                      u32 amount_arg, u32 match_arg);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_007757d0(u32 self, volatile u32 person_argument,
                                      volatile u32 date_argument,
                                      volatile u32 club_filter_argument,
                                      volatile u32 text_argument,
                                      volatile u32 match_argument);
extern u32 CM_THISCALL cm_fn_008cb340(u32 self, u32 *param_2, int param_3,
                                      u32 param_4, u32 param_5);
extern void CM_THISCALL cm_fn_008cb590(void *self, u32 *arg);
extern void CM_THISCALL cm_fn_008cb630(void *self, u32 *arg);
extern u32 CM_THISCALL cm_fn_008cf5e0(int self, u32 param_2, int param_3);
extern u32 CM_CDECL cm_fn_00946464(void *param_1, u32 param_2, u32 param_3,
                                   int param_4, void *param_5);

u32 CM_THISCALL cm_fn_008c39f0(u32 self, u32 *param_2, u32 param_3)
{
  /* One scratch allocation for the original frame at [esp+0x00..0x1d8).
     Bytes 0x110..0x1d8 are the 50-DWORD roster and are intentionally not
     initialized here: the listing only writes them in the mask==2 path. */
  union {
    u8 b[0x1d8];
    u32 w[0x76];
  } L;

  L.b[0x11] = 0; /* outer selector 0..1 */

  for (;;) {
    u8 bl = L.b[0x11];
    u32 flag = 0; /* [esp+0x14] special flag */
    u32 sel;      /* [esp+0x18] selected group/context */
    u8 count;     /* [esp+0x10] collected roster count */
    u8 inner;

    /* 0x008c3a0c */
    if (bl == 0) {
      sel = param_2[7];
    }
    else {
      sel = param_2[8];
    }

    /* 0x008c3a3a..0x008c3a79 */
    if (param_2[5] != 0) {
      if (cm_fn_0053b900((u32 *)(unsigned long)param_2[7]) != 0) {
        goto check_minus2;
      }
      if ((u32)cm_fn_005c6710((void *)(unsigned long)G_AE2C74, param_2[5]) != 0) {
        goto check_minus2;
      }
      if (bl == 0) {
        if (*(u8 *)((unsigned long)param_2 + 0x40) != 0) {
          flag = 1;
          goto flag_done;
        }
      }
      else {
        if (*(u8 *)((unsigned long)param_2 + 0x41) != 0) {
          flag = 1;
          goto flag_done;
        }
      }
    }
  check_minus2:
    if (param_2[0] == 0xfffffffeu) {
      flag = 1;
    }
  flag_done:

    /* 0x008c3a81..0x008c3aaa */
    if (cm_fn_0053b900((u32 *)(unsigned long)sel) != 0) {
      L.b[0x12] = 4;
    }
    else {
      L.b[0x12] = 2;
      cm_fn_00541f90(sel, (void *)&L.b[0x110], 0);
    }

    /* 0x008c3aad..0x008c3af5: walk the 50 roster DWORDs. */
    L.b[0x10] = 0;
    count = 0;
    if (flag != 0) {
      u32 *rb = (u32 *)(void *)&L.b[0x110];
      int remaining = 50;
      do {
        u32 v = *rb;
        if (v != 0) {
          u32 id = *(u32 *)(unsigned long)v;
          if ((int)id < G_AE2364) {
            u8 *map = (u8 *)(unsigned long)*(u32 *)((u32)self + 0x86e);
            u32 ea = (u32)(unsigned long)map + (u32)id * 8u;
            if ((*(u8 *)(unsigned long)ea & 0x20) != 0) {
              L.w[0x12 + (s8)count] = v;
              count = (u8)(count + 1);
            }
          }
        }
        rb = rb + 1;
        remaining = remaining - 1;
      } while (remaining != 0);
    }
    L.b[0x10] = count;

    /* 0x008c3af9..0x008c3ce0 */
    inner = 0;
    do {
      u32 obase = (u32)(s8)L.b[0x11] * 1573u;
      u32 rec = param_3 + obase + (u32)inner * 73u;
      u32 id0 = *(u32 *)(unsigned long)(rec + 0x190);

      if (id0 != 0xffffffffu) {
        if (*(u8 *)(unsigned long)(rec + 0x1ad) != 0xfb) {
          if (!((s8)inner >= 0x0b &&
                *(u8 *)(unsigned long)(rec + 0x1c2) == 0xff)) {
            u32 base = G_AE23BC;
            u8 *entry = (u8 *)(unsigned long)(base + id0 * 110u);
            u32 fld = *(u32 *)(unsigned long)(entry + 0x61u);

            if (fld == 0) {
              break;
            }

            {
              u32 *p = (u32 *)(void *)entry;
              int ok = 1;
              if (p != 0) {
                if ((int)(*p) >= G_AE2364) {
                  ok = 0;
                }
              }

              if (ok != 0) {
                u8 m = L.b[0x12];

                if (m == 2) {
                  if (cm_fn_004cf6d0(G_AE1050, (u32)(unsigned long)p) != 0) {
                    ok = 0;
                  }
                  else {
                    m = L.b[0x12];
                  }
                }

                if (ok != 0) {
                  u32 id = *p;
                  u8 *map = (u8 *)(unsigned long)*(u32 *)((u32)self + 0x86e);
                  u32 ea = (u32)(unsigned long)map + id * 8u;

                  if ((*(u8 *)(unsigned long)ea & m) != 0) {
                    s16 sv;
                    u8 savemask;

                    if (m == 4) {
                      u16 b1 = (u16)*(u8 *)(unsigned long)(ea + 7u);
                      u16 b2 = (u16)*(u8 *)((unsigned long)p + 0x22);
                      sv = (s16)(u16)(b2 - b1);
                      savemask = 5;
                    }
                    else {
                      savemask = 2;
                      sv = 0;
                      {
                        u32 r = cm_fn_004cf5a0(G_AE1050,
                                               (u32)(unsigned long)p);
                        if (r != 0) {
                          r = cm_fn_004cf5a0(G_AE1050,
                                             (u32)(unsigned long)p);
                          sv = *(s16 *)(unsigned long)(r + 0x3b);
                        }
                      }
                    }

                    L.b[0x1c] = savemask;

                    {
                      int rem = (int)sv % 10;
                      int do_it = ((rem == 0 && sv <= 0x32) || sv == 1);
                      if (do_it != 0) {
                        /* CM0102 3.9.68, SHA-256
                           0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
                           preferred-image VA 0x008c3c09 writes only the byte;
                           0x008c3c1d reloads the DWORD, pushed at 0x008c3c27
                           for 0x008c3c2c. Keep the unwritten upper bytes.
                           0x008c3aa0 is after three pushes and writes 0x12,
                           not 0x1e, relative to this scratch base. */
                        u32 rr = cm_fn_008cb340(self, p, (int)sv,
                                               L.w[0x1c / 4], 0);
                        if (rr != 0) {
                          if (L.b[0x1c] == 5) {
                            cm_fn_008cb630((void *)(unsigned long)self, p);
                          }
                          else {
                            cm_fn_008cb590((void *)(unsigned long)self, p);
                          }
                        }
                      }
                    }
                  }

                  /* 0x008c3c53 */
                  if (flag != 0) {
                    u32 id2 = *p;
                    u8 *map2 = (u8 *)(unsigned long)*(u32 *)((u32)self + 0x86e);
                    u32 ea2 = (u32)(unsigned long)map2 + id2 * 8u;

                    if ((*(u8 *)(unsigned long)ea2 & 0x20) != 0) {
                      u32 e;

                      /* 008c3c6e test esi,esi / 008c3c70 je 008c3c7e: a null
                         receiver skips the 008c3c72 bound check but still runs
                         the 008c3c7e map-bit test and the 008c3ca5 lookup. */
                      if (p == 0 || (int)id2 < G_AE2364) {
                        if ((*(u8 *)(unsigned long)ea2 & 0x20) == 0) {
                          e = 0;
                        }
                        else {
                          L.w[0x28 / 4] = id2;
                          e = cm_fn_00946464((void *)&L.b[0x28],
                                             *(u32 *)((u32)self + 0x93a),
                                             *(u32 *)((u32)self + 0x93e),
                                             0xe,
                                             (void *)(unsigned long)0x008e55c0u);
                        }
                      }
                      else {
                        e = 0;
                      }

                      {
                        u32 a4 = e + 4u;
                        *(u16 *)(unsigned long)a4 =
                          (u16)(*(u16 *)(unsigned long)a4 + 1u);
                      }

                      {
                        u8 k = 0;
                        if ((s8)count > 0) {
                          do {
                            if (L.w[0x12 + (s8)k] ==
                                (u32)(unsigned long)p) {
                              L.w[0x12 + (s8)k] = 0;
                              break;
                            }
                            k = (u8)(k + 1);
                          } while ((s8)k < (s8)count);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      inner = (u8)(inner + 1);
    } while (inner < 0x14);

    /* 0x008c3ce6..0x008c3e26 */
    if (flag != 0 && (s8)count > 0) {
      u32 remaining = (u32)(s8)count;
      u32 *slotp = (u32 *)(void *)&L.w[0x12];

      do {
        u32 v = *slotp;

        if (v != 0) {
          u8 r = cm_fn_0062c400(G_B63D28, v);

          if (r == 0) {
            int cont = 0;

            if (param_2[5] == 0) {
              cont = 1;
            }
            else if (cm_fn_0055d270(G_AE28F0, v, (u32)param_2[5], 1, 0) == 0) {
              cont = 1;
            }

            if (cont != 0) {
              if (cm_fn_007757d0(G_B74318, v, 0x00ae2c90u, (u32)sel,
                                 0, 0) == 0) {
                cont = 0;
                if (param_2[5] == 0) {
                  cont = 1;
                }
                else if ((u8)cm_fn_008cf5e0((int)self, v,
                                             (int)(unsigned long)param_2) == 0) {
                  cont = 1;
                }

                if (cont != 0) {
                  int skip = 0;

                  if (cm_fn_004cf6d0(G_AE1050, v) != 0) {
                    u32 p2 = cm_fn_004cf5a0(G_AE1050, v);
                    u32 e;

                    G_AE1080 = p2;
                    if (p2 == 0) {
                      e = 0;
                    }
                    else {
                      e = G_AE23B4 +
                          (*(u32 *)(unsigned long)(p2 + 4)) * 0x245u;
                    }
                    if (e == sel) {
                      skip = 1;
                    }
                  }

                  if (skip == 0) {
                    u32 id = *(u32 *)(unsigned long)v;
                    u32 e2;

                    if ((int)id < G_AE2364) {
                      u8 *map = (u8 *)(unsigned long)*(u32 *)((u32)self + 0x86e);
                      u32 ea = (u32)(unsigned long)map + id * 8u;

                      if ((*(u8 *)(unsigned long)ea & 0x20) != 0) {
                        L.w[0x38 / 4] = id;
                        e2 = cm_fn_00946464(
                          (void *)&L.b[0x38],
                          *(u32 *)((u32)self + 0x93a),
                          *(u32 *)((u32)self + 0x93e),
                          0xe,
                          (void *)(unsigned long)0x008e55c0u);
                      }
                      else {
                        e2 = 0;
                      }
                    }
                    else {
                      e2 = 0;
                    }

                    {
                      u32 a6 = e2 + 6u;
                      *(u16 *)(unsigned long)a6 =
                        (u16)(*(u16 *)(unsigned long)a6 + 1u);
                    }
                  }
                }
              }
            }
          }
        }

        slotp = slotp + 1;
        remaining = remaining - 1;
      } while (remaining != 0);
    }

    /* 0x008c3e2c */
    L.b[0x11] = (u8)(L.b[0x11] + 1);
    if ((s8)L.b[0x11] < 2) {
      continue;
    }
    return (u32)L.b[0x11];
  }
}
