/* Preferred-image VA 0x0062ef30, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 0x10: four stack DWORDs. Caller 0062ef1f forwards EAX unchanged,
   so retain the original helper/error results and unsupported-mode EAX.
   Each table has 82 entries. Preserve the guarded original target DWORD
   read, and dispatch to its original block rather than replacing that read
   with a switch on the injury byte. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0066f4d0(void);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_0062ef30(u32 injury, u32 destination,
                               s32 size, u32 mode)
{
    u8 scratch[0x200];
    u8 *name, *extension;
    u32 line, language, index, target, text, result;

    if (size < 0x1e) {
        /* Original L+0 and L+0x100 buffers; no initialization is added. */
        name = scratch + 0x100;
        extension = scratch;
        line = 0xe8du;
        goto diagnostic;
    }
    if ((u8)mode == 0) {
        goto plain;
    }
    language = cm_fn_0066f4d0();
    if (language == 3u) {
        goto plain;
    }
    if ((u8)mode != 1u)
        return language;

    index = (u32)(s32)(s8)injury;
    if (index > 0x51u) {
        name = scratch;
        extension = scratch + 0x100;
        line = 0xf49u;
        goto diagnostic;
    }
    target = CM_GLOBAL(volatile u32, 0x0063044cu + index * 4u);
    goto dispatch;

plain:
    index = (u32)(s32)(s8)injury;
    if (index > 0x51u) {
        name = scratch;
        extension = scratch + 0x100;
        line = 0xeecu;
        goto diagnostic;
    }
    target = CM_GLOBAL(volatile u32, 0x00630594u + index * 4u);

dispatch:
    switch (target) {
    case 0x0062efb3u:
        return cm_fn_0066f4e0(destination, 0x009e88fcu); /* call VA 0062efc0 */
    case 0x0062efd2u:
        return cm_fn_0066f4e0(destination, 0x009e88f8u); /* call VA 0062efdf */
    case 0x0062eff1u:
        return cm_fn_0066f4e0(destination, 0x009e88f0u); /* call VA 0062effe */
    case 0x0062f010u:
        return cm_fn_0066f4e0(destination, 0x009e88e0u); /* call VA 0062f01d */
    case 0x0062f02fu:
        return cm_fn_0066f4e0(destination, 0x009e88c4u); /* call VA 0062f03c */
    case 0x0062f04eu:
        return cm_fn_0066f4e0(destination, 0x009e88b4u); /* call VA 0062f05b */
    case 0x0062f06du:
        return cm_fn_0066f4e0(destination, 0x009e88a0u); /* call VA 0062f07a */
    case 0x0062f08cu:
        return cm_fn_0066f4e0(destination, 0x009e8890u); /* call VA 0062f099 */
    case 0x0062f0abu:
        return cm_fn_0066f4e0(destination, 0x009e8880u); /* call VA 0062f0b8 */
    case 0x0062f0cau:
        return cm_fn_0066f4e0(destination, 0x009e8870u); /* call VA 0062f0d7 */
    case 0x0062f0e9u:
        return cm_fn_0066f4e0(destination, 0x009e8860u); /* call VA 0062f0f6 */
    case 0x0062f108u:
        return cm_fn_0066f4e0(destination, 0x009e8844u); /* call VA 0062f115 */
    case 0x0062f127u:
        return cm_fn_0066f4e0(destination, 0x009e8828u); /* call VA 0062f134 */
    case 0x0062f146u:
        return cm_fn_0066f4e0(destination, 0x009e8810u); /* call VA 0062f153 */
    case 0x0062f165u:
        return cm_fn_0066f4e0(destination, 0x009e8800u); /* call VA 0062f172 */
    case 0x0062f184u:
        return cm_fn_0066f4e0(destination, 0x009e87f0u); /* call VA 0062f191 */
    case 0x0062f1a3u:
        return cm_fn_0066f4e0(destination, 0x009e87e0u); /* call VA 0062f1b0 */
    case 0x0062f1c2u:
        return cm_fn_0066f4e0(destination, 0x009e87d0u); /* call VA 0062f1cf */
    case 0x0062f1e1u:
        return cm_fn_0066f4e0(destination, 0x009e87bcu); /* call VA 0062f1ee */
    case 0x0062f200u:
        return cm_fn_0066f4e0(destination, 0x009e87acu); /* call VA 0062f20d */
    case 0x0062f21fu:
        return cm_fn_0066f4e0(destination, 0x009e879cu); /* call VA 0062f22c */
    case 0x0062f23eu:
        return cm_fn_0066f4e0(destination, 0x009e878cu); /* call VA 0062f24b */
    case 0x0062f25du:
        return cm_fn_0066f4e0(destination, 0x009e877cu); /* call VA 0062f26a */
    case 0x0062f27cu:
        return cm_fn_0066f4e0(destination, 0x009e8764u); /* call VA 0062f289 */
    case 0x0062f29bu:
        return cm_fn_0066f4e0(destination, 0x009e8750u); /* call VA 0062f2a8 */
    case 0x0062f2bau:
        return cm_fn_0066f4e0(destination, 0x009e8738u); /* call VA 0062f2c7 */
    case 0x0062f2d9u:
        return cm_fn_0066f4e0(destination, 0x009e8724u); /* call VA 0062f2e6 */
    case 0x0062f2f8u:
        return cm_fn_0066f4e0(destination, 0x009e8708u); /* call VA 0062f305 */
    case 0x0062f317u:
        return cm_fn_0066f4e0(destination, 0x009e86f4u); /* call VA 0062f324 */
    case 0x0062f336u:
        return cm_fn_0066f4e0(destination, 0x009e86e4u); /* call VA 0062f343 */
    case 0x0062f355u:
        return cm_fn_0066f4e0(destination, 0x009e86d4u); /* call VA 0062f362 */
    case 0x0062f374u:
        return cm_fn_0066f4e0(destination, 0x009e86c0u); /* call VA 0062f381 */
    case 0x0062f393u:
        return cm_fn_0066f4e0(destination, 0x009e86b4u); /* call VA 0062f3a0 */
    case 0x0062f3b2u:
        return cm_fn_0066f4e0(destination, 0x009e87acu); /* call VA 0062f3bf */
    case 0x0062f3d1u:
        return cm_fn_0066f4e0(destination, 0x009e878cu); /* call VA 0062f3de */
    case 0x0062f3f0u:
        return cm_fn_0066f4e0(destination, 0x009e86a4u); /* call VA 0062f3fd */
    case 0x0062f40fu:
        return cm_fn_0066f4e0(destination, 0x009e8690u); /* call VA 0062f41c */
    case 0x0062f42eu:
        return cm_fn_0066f4e0(destination, 0x009e8680u); /* call VA 0062f43b */
    case 0x0062f44du:
        return cm_fn_0066f4e0(destination, 0x009e8670u); /* call VA 0062f45a */
    case 0x0062f46cu:
        return cm_fn_0066f4e0(destination, 0x009e8660u); /* call VA 0062f479 */
    case 0x0062f48bu:
        return cm_fn_0066f4e0(destination, 0x009e8650u); /* call VA 0062f498 */
    case 0x0062f4aau:
        return cm_fn_0066f4e0(destination, 0x009e8640u); /* call VA 0062f4b7 */
    case 0x0062f4c9u:
        return cm_fn_0066f4e0(destination, 0x009e8630u); /* call VA 0062f4d6 */
    case 0x0062f4e8u:
        return cm_fn_0066f4e0(destination, 0x009e8624u); /* call VA 0062f4f5 */
    case 0x0062f507u:
        return cm_fn_0066f4e0(destination, 0x009e8614u); /* call VA 0062f514 */
    case 0x0062f526u:
        return cm_fn_0066f4e0(destination, 0x009e8604u); /* call VA 0062f533 */
    case 0x0062f545u:
        return cm_fn_0066f4e0(destination, 0x009e85f0u); /* call VA 0062f552 */
    case 0x0062f564u:
        return cm_fn_0066f4e0(destination, 0x009e85e4u); /* call VA 0062f571 */
    case 0x0062f583u:
        return cm_fn_0066f4e0(destination, 0x009e85d4u); /* call VA 0062f590 */
    case 0x0062f5a2u:
        return cm_fn_0066f4e0(destination, 0x009e85c4u); /* call VA 0062f5af */
    case 0x0062f5c1u:
        return cm_fn_0066f4e0(destination, 0x009e85b0u); /* call VA 0062f5ce */
    case 0x0062f5e0u:
        return cm_fn_0066f4e0(destination, 0x009e859cu); /* call VA 0062f5ed */
    case 0x0062f5ffu:
        return cm_fn_0066f4e0(destination, 0x009e858cu); /* call VA 0062f60c */
    case 0x0062f61eu:
        return cm_fn_0066f4e0(destination, 0x009e8578u); /* call VA 0062f62b */
    case 0x0062f63du:
        return cm_fn_0066f4e0(destination, 0x009e8568u); /* call VA 0062f64a */
    case 0x0062f65cu:
        return cm_fn_0066f4e0(destination, 0x009e8558u); /* call VA 0062f669 */
    case 0x0062f67bu:
        return cm_fn_0066f4e0(destination, 0x009e8548u); /* call VA 0062f688 */
    case 0x0062f69au:
        return cm_fn_0066f4e0(destination, 0x009e8538u); /* call VA 0062f6a7 */
    case 0x0062f6b9u:
        return cm_fn_0066f4e0(destination, 0x009e8524u); /* call VA 0062f6c6 */
    case 0x0062f6d8u:
        return cm_fn_0066f4e0(destination, 0x009e850cu); /* call VA 0062f6e5 */
    case 0x0062f6f7u:
        return cm_fn_0066f4e0(destination, 0x009e84f8u); /* call VA 0062f704 */
    case 0x0062f716u:
        return cm_fn_0066f4e0(destination, 0x009e84e8u); /* call VA 0062f723 */
    case 0x0062f735u:
        return cm_fn_0066f4e0(destination, 0x009e84d8u); /* call VA 0062f742 */
    case 0x0062f754u:
        return cm_fn_0066f4e0(destination, 0x009e84c4u); /* call VA 0062f761 */
    case 0x0062f773u:
        return cm_fn_0066f4e0(destination, 0x009e84b4u); /* call VA 0062f780 */
    case 0x0062f792u:
        return cm_fn_0066f4e0(destination, 0x009e84a4u); /* call VA 0062f79f */
    case 0x0062f7b1u:
        return cm_fn_0066f4e0(destination, 0x009e8494u); /* call VA 0062f7be */
    case 0x0062f7d0u:
        return cm_fn_0066f4e0(destination, 0x009e8484u); /* call VA 0062f7dd */
    case 0x0062f7efu:
        return cm_fn_0066f4e0(destination, 0x009e8478u); /* call VA 0062f7fc */
    case 0x0062f80eu:
        return cm_fn_0066f4e0(destination, 0x009e8468u); /* call VA 0062f81b */
    case 0x0062f82du:
        return cm_fn_0066f4e0(destination, 0x009e8458u); /* call VA 0062f83a */
    case 0x0062f84cu:
        return cm_fn_0066f4e0(destination, 0x009e8444u); /* call VA 0062f859 */
    case 0x0062f86bu:
        return cm_fn_0066f4e0(destination, 0x009e842cu); /* call VA 0062f878 */
    case 0x0062f88au:
        return cm_fn_0066f4e0(destination, 0x009e8418u); /* call VA 0062f897 */
    case 0x0062f8a9u:
        return cm_fn_0066f4e0(destination, 0x009e8408u); /* call VA 0062f8b6 */
    case 0x0062f8c8u:
        return cm_fn_0066f4e0(destination, 0x009e83f4u); /* call VA 0062f8d5 */
    case 0x0062f8e7u:
        return cm_fn_0066f4e0(destination, 0x009e83e8u); /* call VA 0062f8f4 */
    case 0x0062f906u:
        return cm_fn_0066f4e0(destination, 0x009e83d8u); /* call VA 0062f913 */
    case 0x0062f925u:
        return cm_fn_0066f4e0(destination, 0x009e83c8u); /* call VA 0062f932 */
    case 0x0062f944u:
        return cm_fn_0066f4e0(destination, 0x009e83c0u); /* call VA 0062f951 */
    case 0x0062f963u:
        return cm_fn_0066f4e0(destination, 0x009e83b0u); /* call VA 0062f970 */
    case 0x0062f982u:
        return cm_fn_0066f4e0(destination, 0x009e83a8u); /* call VA 0062f98f */
    case 0x0062f9eeu:
        return cm_fn_0066f4e0(destination, 0x009e83a0u); /* call VA 0062f9fb */
    case 0x0062fa0du:
        return cm_fn_0066f4e0(destination, 0x009e88f8u); /* call VA 0062fa1a */
    case 0x0062fa2cu:
        return cm_fn_0066f4e0(destination, 0x009e88f0u); /* call VA 0062fa39 */
    case 0x0062fa4bu:
        return cm_fn_0066f4e0(destination, 0x009e88e0u); /* call VA 0062fa58 */
    case 0x0062fa6au:
        return cm_fn_0066f4e0(destination, 0x009e8388u); /* call VA 0062fa77 */
    case 0x0062fa89u:
        return cm_fn_0066f4e0(destination, 0x009e8378u); /* call VA 0062fa96 */
    case 0x0062faa8u:
        return cm_fn_0066f4e0(destination, 0x009e8368u); /* call VA 0062fab5 */
    case 0x0062fac7u:
        return cm_fn_0066f4e0(destination, 0x009e8358u); /* call VA 0062fad4 */
    case 0x0062fae6u:
        return cm_fn_0066f4e0(destination, 0x009e834cu); /* call VA 0062faf3 */
    case 0x0062fb05u:
        return cm_fn_0066f4e0(destination, 0x009e833cu); /* call VA 0062fb12 */
    case 0x0062fb24u:
        return cm_fn_0066f4e0(destination, 0x009e8330u); /* call VA 0062fb31 */
    case 0x0062fb43u:
        return cm_fn_0066f4e0(destination, 0x009e8844u); /* call VA 0062fb50 */
    case 0x0062fb62u:
        return cm_fn_0066f4e0(destination, 0x009e8318u); /* call VA 0062fb6f */
    case 0x0062fb81u:
        return cm_fn_0066f4e0(destination, 0x009e8810u); /* call VA 0062fb8e */
    case 0x0062fba0u:
        return cm_fn_0066f4e0(destination, 0x009e8308u); /* call VA 0062fbad */
    case 0x0062fbbfu:
        return cm_fn_0066f4e0(destination, 0x009e82fcu); /* call VA 0062fbcc */
    case 0x0062fbdeu:
        return cm_fn_0066f4e0(destination, 0x009e82ecu); /* call VA 0062fbeb */
    case 0x0062fbfdu:
        return cm_fn_0066f4e0(destination, 0x009e82e0u); /* call VA 0062fc0a */
    case 0x0062fc1cu:
        return cm_fn_0066f4e0(destination, 0x009e82ccu); /* call VA 0062fc29 */
    case 0x0062fc3bu:
        return cm_fn_0066f4e0(destination, 0x009e82c0u); /* call VA 0062fc48 */
    case 0x0062fc5au:
        return cm_fn_0066f4e0(destination, 0x009e879cu); /* call VA 0062fc67 */
    case 0x0062fc79u:
        return cm_fn_0066f4e0(destination, 0x009e82b4u); /* call VA 0062fc86 */
    case 0x0062fc98u:
        return cm_fn_0066f4e0(destination, 0x009e82a4u); /* call VA 0062fca5 */
    case 0x0062fcb7u:
        return cm_fn_0066f4e0(destination, 0x009e8764u); /* call VA 0062fcc4 */
    case 0x0062fcd6u:
        return cm_fn_0066f4e0(destination, 0x009e8290u); /* call VA 0062fce3 */
    case 0x0062fcf5u:
        return cm_fn_0066f4e0(destination, 0x009e8738u); /* call VA 0062fd02 */
    case 0x0062fd14u:
        return cm_fn_0066f4e0(destination, 0x009e8724u); /* call VA 0062fd21 */
    case 0x0062fd33u:
        return cm_fn_0066f4e0(destination, 0x009e8708u); /* call VA 0062fd40 */
    case 0x0062fd52u:
        return cm_fn_0066f4e0(destination, 0x009e827cu); /* call VA 0062fd5f */
    case 0x0062fd71u:
        return cm_fn_0066f4e0(destination, 0x009e826cu); /* call VA 0062fd7e */
    case 0x0062fd90u:
        return cm_fn_0066f4e0(destination, 0x009e825cu); /* call VA 0062fd9d */
    case 0x0062fdafu:
        return cm_fn_0066f4e0(destination, 0x009e824cu); /* call VA 0062fdbc */
    case 0x0062fdceu:
        return cm_fn_0066f4e0(destination, 0x009e8240u); /* call VA 0062fddb */
    case 0x0062fdedu:
        return cm_fn_0066f4e0(destination, 0x009e82c0u); /* call VA 0062fdfa */
    case 0x0062fe0cu:
        return cm_fn_0066f4e0(destination, 0x009e82b4u); /* call VA 0062fe19 */
    case 0x0062fe2bu:
        return cm_fn_0066f4e0(destination, 0x009e8230u); /* call VA 0062fe38 */
    case 0x0062fe4au:
        return cm_fn_0066f4e0(destination, 0x009e821cu); /* call VA 0062fe57 */
    case 0x0062fe69u:
        return cm_fn_0066f4e0(destination, 0x009e8210u); /* call VA 0062fe76 */
    case 0x0062fe88u:
        return cm_fn_0066f4e0(destination, 0x009e8200u); /* call VA 0062fe95 */
    case 0x0062fea7u:
        return cm_fn_0066f4e0(destination, 0x009e81f4u); /* call VA 0062feb4 */
    case 0x0062fec6u:
        return cm_fn_0066f4e0(destination, 0x009e81e4u); /* call VA 0062fed3 */
    case 0x0062fee5u:
        return cm_fn_0066f4e0(destination, 0x009e81d8u); /* call VA 0062fef2 */
    case 0x0062ff04u:
        return cm_fn_0066f4e0(destination, 0x009e8630u); /* call VA 0062ff11 */
    case 0x0062ff23u:
        return cm_fn_0066f4e0(destination, 0x009e8624u); /* call VA 0062ff30 */
    case 0x0062ff42u:
        return cm_fn_0066f4e0(destination, 0x009e81c8u); /* call VA 0062ff4f */
    case 0x0062ff61u:
        return cm_fn_0066f4e0(destination, 0x009e81b8u); /* call VA 0062ff6e */
    case 0x0062ff80u:
        return cm_fn_0066f4e0(destination, 0x009e81a8u); /* call VA 0062ff8d */
    case 0x0062ff9fu:
        return cm_fn_0066f4e0(destination, 0x009e819cu); /* call VA 0062ffac */
    case 0x0062ffbeu:
        return cm_fn_0066f4e0(destination, 0x009e818cu); /* call VA 0062ffcb */
    case 0x0062ffddu:
        return cm_fn_0066f4e0(destination, 0x009e8180u); /* call VA 0062ffea */
    case 0x0062fffcu:
        return cm_fn_0066f4e0(destination, 0x009e8170u); /* call VA 00630009 */
    case 0x0063001bu:
        return cm_fn_0066f4e0(destination, 0x009e8160u); /* call VA 00630028 */
    case 0x0063003au:
        return cm_fn_0066f4e0(destination, 0x009e8150u); /* call VA 00630047 */
    case 0x00630059u:
        return cm_fn_0066f4e0(destination, 0x009e8140u); /* call VA 00630066 */
    case 0x00630078u:
        return cm_fn_0066f4e0(destination, 0x009e8134u); /* call VA 00630085 */
    case 0x00630097u:
        return cm_fn_0066f4e0(destination, 0x009e8124u); /* call VA 006300a4 */
    case 0x006300b6u:
        return cm_fn_0066f4e0(destination, 0x009e8114u); /* call VA 006300c3 */
    case 0x006300d5u:
        return cm_fn_0066f4e0(destination, 0x009e8108u); /* call VA 006300e2 */
    case 0x006300f4u:
        return cm_fn_0066f4e0(destination, 0x009e80f4u); /* call VA 00630101 */
    case 0x00630113u:
        return cm_fn_0066f4e0(destination, 0x009e80e0u); /* call VA 00630120 */
    case 0x00630132u:
        return cm_fn_0066f4e0(destination, 0x009e80d0u); /* call VA 0063013f */
    case 0x00630151u:
        return cm_fn_0066f4e0(destination, 0x009e80c0u); /* call VA 0063015e */
    case 0x00630170u:
        return cm_fn_0066f4e0(destination, 0x009e80b0u); /* call VA 0063017d */
    case 0x0063018fu:
        return cm_fn_0066f4e0(destination, 0x009e809cu); /* call VA 0063019c */
    case 0x006301aeu:
        return cm_fn_0066f4e0(destination, 0x009e808cu); /* call VA 006301bb */
    case 0x006301cdu:
        return cm_fn_0066f4e0(destination, 0x009e807cu); /* call VA 006301da */
    case 0x006301ecu:
        return cm_fn_0066f4e0(destination, 0x009e8070u); /* call VA 006301f9 */
    case 0x0063020bu:
        return cm_fn_0066f4e0(destination, 0x009e8064u); /* call VA 00630218 */
    case 0x0063022au:
        return cm_fn_0066f4e0(destination, 0x009e8478u); /* call VA 00630237 */
    case 0x00630249u:
        return cm_fn_0066f4e0(destination, 0x009e8058u); /* call VA 00630256 */
    case 0x00630268u:
        return cm_fn_0066f4e0(destination, 0x009e8048u); /* call VA 00630275 */
    case 0x00630287u:
        return cm_fn_0066f4e0(destination, 0x009e8038u); /* call VA 00630294 */
    case 0x006302a6u:
        return cm_fn_0066f4e0(destination, 0x009e8024u); /* call VA 006302b3 */
    case 0x006302c5u:
        return cm_fn_0066f4e0(destination, 0x009e8010u); /* call VA 006302d2 */
    case 0x006302e4u:
        return cm_fn_0066f4e0(destination, 0x009e8004u); /* call VA 006302f1 */
    case 0x00630303u:
        return cm_fn_0066f4e0(destination, 0x009e7ff4u); /* call VA 00630310 */
    case 0x00630322u:
        return cm_fn_0066f4e0(destination, 0x009e83e8u); /* call VA 0063032f */
    case 0x00630341u:
        return cm_fn_0066f4e0(destination, 0x009e83d8u); /* call VA 0063034e */
    case 0x00630360u:
        return cm_fn_0066f4e0(destination, 0x009e83c8u); /* call VA 0063036d */
    case 0x0063037fu:
        return cm_fn_0066f4e0(destination, 0x009e83c0u); /* call VA 0063038c */
    case 0x0063039eu:
        return cm_fn_0066f4e0(destination, 0x009e83b0u); /* call VA 006303ab */
    case 0x006303bdu:
        return cm_fn_0066f4e0(destination, 0x009e83a8u); /* call VA 006303ca */
    }
    /* Both tables are bounded above and immutable in the pinned image;
       every target is represented by an original block above. */
    __builtin_unreachable();

diagnostic:
    cm_fn_00944cff(0x009e7f28u, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                   name, extension, line);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    return result;
}
