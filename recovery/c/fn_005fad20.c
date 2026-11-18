/* fn_005fad20 - CM 01/02 3.9.68, comp\host_country.cpp
 *
 * Preferred-image VA 0x005fad20 (image base 0x00400000).
 * Plain C translation of the original instruction listing; acceptance comes
 * only from `cmre test --c` and `cmre accept --c`.
 */
#include "cmre.h"

#define DAT_009cf6e8 CM_GLOBAL(int, 0x009cf6e8)
#define DAT_009cf6ec CM_GLOBAL(int, 0x009cf6ec)
#define DAT_009cf6f0 CM_GLOBAL(int, 0x009cf6f0)
#define DAT_009cfa10 CM_GLOBAL(int, 0x009cfa10)
#define DAT_00ae2350 CM_GLOBAL(int, 0x00ae2350)
#define DAT_00ae23a8 CM_GLOBAL(u32, 0x00ae23a8)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

#define s_009cf0d8 ((u32)0x009cf0d8)
#define s_009870e0 ((u32)0x009870e0)
#define s_009870e8 ((u32)0x009870e8)

/* The direct caller (0x005fa8d0) reads EAX after the call, so the EAX result
 * of this routine and of its last callee is live. */
extern u32 cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_FASTCALL cm_fn_005fad20(u32 *param_1)
{
    /* 005fad20 sub esp,0x204 : the frame L..L+0x203.
     *   L+0x000  saved `this` (mov dword ptr [esp+0x10], ebp)
     *   L+0x004  accepted-country list, 50 dword slots (rep stosd zeroed)
     *   L+0x104  message buffer (256 bytes), left uninitialized */
    u32 scratch[0x204 / 4];
    u8 *frame = (u8 *)scratch;
    u32 *arr = (u32 *)(frame + 0x04);
    u8 *buf = frame + 0x104;
    u32 thisp = (u32)param_1;
    u32 n = (u32)DAT_00ae2350;    /* 005fad27 mov ebx,[0xae2350] */
    u32 base;                    /* 005fad41 mov ecx,[0xae23a8] */
    u8 dl = 0;                   /* accepted-country counter (DL) */
    int i;
    u32 z;

    /* 005fad32 mov ecx,0x32 / xor eax,eax / lea edi,[esp+0x14] / rep stosd :
     * 50 dwords (0xc8 bytes) zeroed from L+4. */
    for (z = 0; z < 0x32u; z++)
        scratch[1 + z] = 0;

    /* 005fad4b mov dword ptr [esp+0x10], ebp : save `this` at L+0. */
    *(u32 *)frame = thisp;

    base = DAT_00ae23a8;

    /* 005fad49 test ebx,ebx / jle 0x5fadcc : no countries -> error path. */
    i = 0;
    if ((s32)n > 0) {
        for (;;) {
            u32 c;

            /* 005fad51 cmp dl,0x32 / jge 0x5fadc7 */
            if (dl >= 0x32)
                break;

            c = base + (u32)i * 0x122u;   /* 005fadbd add ecx,0x122 */

            /* 005fad56 mov eax,[ecx+0x71] / test eax,eax / je */
            {
                u32 p71 = *(u32 *)(c + 0x71u);
                if (p71 != 0) {
                    /* 005fad5d mov eax,[eax] / mov esi,[0x9cfa10] / cmp eax,esi */
                    if (*(u32 *)p71 == (u32)DAT_009cfa10 &&
                        /* 005fad69 cmp byte ptr [ecx+0x7e],1 / jne */
                        *(u8 *)(c + 0x7eu) == 1u &&
                        /* 005fad6f cmp byte ptr [ecx+0x85],7 / jle (signed) */
                        *(s8 *)(c + 0x85u) > 7) {
                        /* 005fad78 mov eax,[ecx+0x80] / test eax,eax / je */
                        u32 p80 = *(u32 *)(c + 0x80u);
                        if (p80 != 0 &&
                            /* 005fad82 cmp dword ptr [eax+0x3c],0x9c40 / jle (signed) */
                            *(s32 *)(p80 + 0x3cu) > 0x9c40) {
                            /* duplicate scan over the accepted list */
                            u8 al = 0;
                            u8 found = 0;

                            /* 005fad8f test dl,dl / jle 0x5fadaf */
                            if (dl > 0) {
                                do {
                                    /* 005fad93 test esi,esi / jne 0x5fadb8 */
                                    if (found)
                                        break;
                                    /* 005fad9a cmp dword ptr [esp+ebp*4+0x14],ecx */
                                    if (arr[al] == c)
                                        found = 1;
                                    al++;               /* 005fada5 inc al */
                                } while (al < dl);     /* 005fada7 cmp al,dl / jl */
                            }
                            /* 005fadab test esi,esi / jne 0x5fadb8 */
                            if (!found) {
                                u8 slot = dl;
                                dl++;                    /* 005fadb2 inc dl */
                                arr[slot] = c;           /* 005fadb4 mov [esp+eax*4+0x14],ecx */
                            }
                        }
                    }
                }
            }

            /* 005fadbc inc edi / 005fadc3 cmp edi,ebx / jl */
            i++;
            if ((u32)i >= n)
                break;
        }
    }

    /* 005fadc7 cmp dl,3 / jge 0x5fae3a */
    if (dl >= 3) {
        int count = (int)(s8)dl;   /* 005fae3a movsx edi,dl */

        /* 005fae3d test edi,edi / jle 0x5fae66 */
        if (count > 0) {
            u32 *p = arr;          /* 005fae41 lea esi,[esp+0x14] */
            int k = count;         /* 005fae45 mov ebx,edi */

            do {
                /* 005fae47 push edi / 005fae48 call 0x90cfc0 */
                unsigned long long r = cm_fn_0090cfc0((u32)count, 0u, (u32)count);
                /* 005fae4d movsx edx,al */
                int j = (int)(s8)(u8)(r & 0xffu);
                u32 *slot;
                u32 cur;
                u32 tmp;

                /* 005fae50 mov ecx,[esi] */
                cur = *p;
                /* 005fae55 lea eax,[esp+edx*4+0x14] */
                slot = (u32 *)(frame + 4 + 4 * j);
                /* 005fae5d mov edx,[eax] / 005fae5f mov [esi-4],edx */
                tmp = *slot;
                *p = tmp;
                /* 005fae62 mov [eax],ecx */
                *slot = cur;
                p++;                  /* 005fae59 add esi,4 */
                k--;                 /* 005fae5c dec ebx */
            } while (k != 0);        /* 005fae64 jne 0x5fae47 */
        }

        /* 005fae66 mov eax,[ebp] */
        {
            u32 recs = *(volatile u32 *)thisp;
            u32 idx = 0;             /* esi */
            u8 dl2 = 0;              /* 005fae6b xor dl,dl */
            u32 eaxv;               /* EAX at the ret */

            /* 005fae6d cmp word ptr [ebp+4],0 / jle 0x5faeaf */
            if ((s16)*(volatile u16 *)(thisp + 4) > 0) {
                for (;;) {
                    /* 005fae74 cmp dl,3 / jge 0x5faea3 */
                    if (dl2 < 3) {
                        u32 rec = recs + idx * 0x22u;
                        /* 005fae79 mov ecx,[eax] */
                        u32 id = *(u32 *)rec;

                        /* 005fae7b..005fae93 : three special ids */
                        if (id == (u32)DAT_009cf6e8 ||
                            id == (u32)DAT_009cf6f0 ||
                            id == (u32)DAT_009cf6ec) {
                            u8 slot = dl2;
                            u32 cp;
                            u32 v;

                            dl2++;                          /* 005fae98 inc dl */
                            cp = *(u32 *)(frame + 4 + 4 * (int)slot);
                            v = *(u32 *)cp;                 /* 005fae9e mov ecx,[ecx] */
                            *(volatile u32 *)(rec + 6) = v;           /* 005faea0 mov [eax+6],ecx */
                        }
                    }
                    /* 005faea3 movsx ecx,word ptr [ebp+4] */
                    s32 record_count = (s16)*(volatile u16 *)(thisp + 4);
                    idx++;                                /* 005fae7a inc esi */
                    if (!((s32)idx < record_count))
                        break;                             /* 005feab cmp esi,ecx / jl */
                }
            }
            /* 005faea8 add eax,0x22 : EAX is the walked record pointer */
            eaxv = recs + idx * 0x22u;
            return eaxv;                 /* 005faeaf pops, add esp,0x204, ret */
        }
    }

    /* 005fadcc error path */
    /* 005fadd7 push ecx(L+4) / push edx(L+0x104) / push 0 / push 0 / push 0x9cf0d8 */
    cm_fn_00944cff(s_009cf0d8, 0u, 0u, (void *)buf, (void *)arr);

    /* 005fadf5 push 0x3b5 / push eax(L+4) / push ecx(L+0x104) / call 0x90d130 ;
     * the three pushed words belong to the next call (right-to-left). */
    {
        char *msg = cm_fn_0090d130();

        /* 005fae01 push eax / push 0x9870e8 / push 0xae24d0 / call 0x944c9f */
        cm_fn_00944c9f((u32)&DAT_00ae24d0, s_009870e8, (u32)msg,
                       (u32)buf, (u32)arr, 0x3b5u);
    }

    /* 005fae11 push 0 / push 0xae24d0 / push 0x9870e0 / call 0x5e8290 */
    {
        /* EAX at the ret is the value left by this last call. */
        u32 eaxv = cm_fn_005e8290((const char *)s_009870e0,
                                  (const char *)&DAT_00ae24d0, 0);

        /* 005fae25 mov dword ptr [0xb67a34],0 */
        DAT_00b67a34 = 0;
        return eaxv;
    }
}
