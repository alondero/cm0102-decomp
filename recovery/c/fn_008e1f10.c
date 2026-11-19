/* Reconstruction of FUN_008e1f10, preferred-image VA 0x008e1f10,
   CM0102 3.9.68 primary executable (SHA-256 0c78e7da...f86, base 0x00400000).

   ABI findings (all cited to the pinned listing / image):
     008e1f10-008e1f1e : SEH frame {prev(esp0-0xc),handler(esp0-8),state(esp0-4)}
                          linked through fs:[0].
     008e1f25          : sub esp,0x158 -> scratch base B = esp0-0x164.
     008e1f2e          : param_1 at [esp+0x174] = [esp0+4].
     008e2029          : param_2 at [esp+0x178] = [esp0+8].
     008e1f57          : cm_fn_008d1130 receives ECX unchanged since entry;
                          forwarded explicitly as the thiscall receiver.
     008e1f71/8a/9c/ae/c0/cf/2017/2024/203b/204d : ECX = lea [esp+0x14] or
                          lea [esp+0xc]; both resolve to scratch base B.
     008e1fdb          : cm_fn_004cf5a0 ECX = [0xae1050].
     008e206e          : cm_fn_007861e0 ECX = [0xb7431c].
     008e1f4c vs 008e2056 : output buffer at B+0xe0 (lea [esp+0xf0] then
                          lea [esp+0xec]).
     008e1f78          : dword 0 written to SEH state ([esp+0x16c]=esp0-4).
   All global/object reads are volatile dword loads to preserve width and
   fault order. The L7 (index 7) path keeps the original order and object
   pointers, so the original-only probes fault identically; no object-state
   initialization was invented. */
#include "cmre.h"

#define DAT_0096474b 0x0096474bu
#define EXCEPTION_LIST CM_FS(u32, 0)
#define G_ae1080 CM_GLOBAL(volatile u32, 0x00ae1080)
#define G_ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_ae1050 CM_GLOBAL(volatile u32, 0x00ae1050)
#define G_b7431c CM_GLOBAL(volatile u32, 0x00b7431c)

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_008d1130(u32 self, u32 offer, u32 flags, u32 output, s8 capacity);

struct local_block {
    u8 scratch[0x158];      /* B .. B+0x157 */
    volatile u32 prev;      /* B+0x158 (esp0-0xc) */
    volatile u32 handler;   /* B+0x15c */
    volatile u32 state;     /* B+0x160 (esp0-0x4) */
};

void CM_THISCALL cm_fn_008e1f10(u32 self, int param_1, int param_2)
{
    struct local_block lb;
    u32 esi_val;
    u32 *ptr;
    u8 *out;
    s8 bl;
    u32 res;
    u32 *pu;
    u32 idx;
    u32 v;
    u32 *loop_ptr;
    int i;

    /* mov eax,fs:[0]; push -1; push 0x96474b; push eax; mov fs:[0],esp */
    lb.prev = EXCEPTION_LIST;
    lb.handler = DAT_0096474b;
    lb.state = 0xffffffffu;
    EXCEPTION_LIST = (u32)&lb.prev;

    /* 008e1f37-49 : esi = [0xae23bc] + *(u32*)(param_1+0xc) * 0x6e */
    esi_val = *(volatile u32 *)((u8 *)param_1 + 0xc);
    ptr = (u32 *)(G_ae23bc + esi_val * 0x6eu);
    out = lb.scratch + 0xe0;

    /* 008e1f35/4c/53/54/56/57 : cm_fn_008d1130(self,param_1,0x5f,out,30) */
    bl = (s8)cm_fn_008d1130(self, (u32)param_1, 0x5fu, (u32)out, 0x1e);
    if (bl != 0) {
        /* 008e1f66-71 : cm_fn_0077bbe0(B,0xfb6,0) */
        cm_fn_0077bbe0((void *)lb.scratch, 0xfb6u, 0u);

        /* 008e1f76 reads *ptr before 008e1f78 updates the SEH state. */
        v = *(volatile u32 *)ptr;
        lb.state = 0u;

        /* 008e1f83-8a : index 0, value captured above */
        cm_fn_00785790((void *)lb.scratch, 0u, v);
        /* 008e1f96-9c : index 1, value = *(u32*)(ptr[1]+0x33) */
        cm_fn_00785790((void *)lb.scratch, 1u,
                       *(volatile u32 *)((u32)ptr[1] + 0x33u));
        /* 008e1fab-ae : index 2, value = *(u32*)(ptr[2]+0x33) */
        cm_fn_00785790((void *)lb.scratch, 2u,
                       *(volatile u32 *)((u32)ptr[2] + 0x33u));
        /* 008e1fb9-c0 : index 3, value = *(u32*)(ptr[3]+0x33) */
        cm_fn_00785790((void *)lb.scratch, 3u,
                       *(volatile u32 *)((u32)ptr[3] + 0x33u));
        /* 008e1fcc-cf : index 4, value = *(u32*)(param_1+8) */
        cm_fn_00785790((void *)lb.scratch, 4u,
                       *(volatile u32 *)((u8 *)param_1 + 8));

        /* 008e1fd4-db : cm_fn_004cf5a0([0xae1050], ptr) */
        res = cm_fn_004cf5a0(G_ae1050, (u32)ptr);
        G_ae1080 = res;                 /* 008e1fe2 : dword store */

        /* 008e1fe7-200a : byte [res+0x35]&0x3f != 5 -> table lookup */
        pu = (u32 *)0;
        if (res != 0 &&
            ((*(volatile u8 *)((u32)res + 0x35u)) & 0x3fu) != 5u) {
            idx = *(volatile u32 *)((u32)res + 4u);
            pu = (u32 *)(G_ae23b4 + idx * 0x245u);
        }
        /* 008e200e-17 : index 5, value = *pu (original reads [0] when pu==0) */
        cm_fn_00785790((void *)lb.scratch, 5u, *(volatile u32 *)pu);
        /* 008e201c-24 : index 7, value 0 */
        cm_fn_00785790((void *)lb.scratch, 7u, 0u);
        /* 008e2034-3b : index 8, movsx word [param_2+0x2d] */
        cm_fn_00785790((void *)lb.scratch, 8u,
                       (u32)(s32)*(volatile s16 *)((u8 *)param_2 + 0x2d));
        /* 008e2040-4d : index 9, zero-extended word [param_2+0x2f] */
        cm_fn_00785790((void *)lb.scratch, 9u,
                       (u32)*(volatile u16 *)((u8 *)param_2 + 0x2f));

        /* 008e2052-77 : if bl>0 loop over output dwords */
        if (bl > 0) {
            loop_ptr = (u32 *)out;
            i = (int)bl;
            do {
                v = *(volatile u32 *)loop_ptr;
                cm_fn_007861e0((void *)G_b7431c, (u32)lb.scratch, v);
                loop_ptr++;
                i--;
            } while (i != 0);
        }
    }

    /* mov ecx,[esp+0x164]; ...; mov fs:[0],ecx */
    EXCEPTION_LIST = lb.prev;
}
