/* Verified from primary VA 00671DE8..00673776 by prepare-morph-two.py.
 * Qualifier and dispatch data stay in the original image. */
typedef struct { cm_lang_u32 qualifier, dispatch; } two_suffix_rule;
static const two_suffix_rule two_suffix_rules[] = {
    {0x9f12f8u, 0x6801a8u},
    {0x9f12b8u, 0x6801c0u},
    {0x99c35cu, 0x6801d8u},
    {0x9f128cu, 0x6801f0u},
    {0x9f1268u, 0x680208u},
    {0x9f122cu, 0x680220u},
    {0x9f1210u, 0x680238u},
    {0x9f11fcu, 0x680250u},
    {0x9f11e8u, 0x680268u},
    {0x9f11ccu, 0x680280u},
    {0x9f11a8u, 0x680298u},
    {0x9f117cu, 0x6802b0u},
    {0x9f115cu, 0x6802c8u},
    {0x9f1140u, 0x6802e0u},
    {0x9f1120u, 0x6802f8u},
    {0x9f10fcu, 0x680310u},
    {0x9f10d0u, 0x680328u},
    {0x9f10a8u, 0x680340u},
    {0x9f1084u, 0x680358u},
    {0x9f1060u, 0x680370u},
    {0x9f1040u, 0x680388u},
    {0x9f1004u, 0x6803a0u},
    {0x9f0fe4u, 0x6803b8u},
    {0x9f0fc8u, 0x6803d0u},
    {0x9f0fa8u, 0x6803e8u},
    {0x9f0f90u, 0x680400u},
    {0x9f0f6cu, 0x680418u},
    {0x9f0f3cu, 0x680430u},
    {0x9f0f20u, 0x680448u},
    {0x9f0f04u, 0x680460u},
    {0x9f0ee0u, 0x680478u},
    {0x9f0eb4u, 0x680490u},
};
static cm_lang_u32 two_suffix_literal(cm_lang_u32 target) {
    switch (target) {
    case 0x00672a19u: return 0x009f12e8u;
    case 0x00672a27u: return 0x009f12d4u;
    case 0x00672a35u: return 0x009f12c0u;
    case 0x00672a93u: return 0x009f12acu;
    case 0x00672aa1u: return 0x009f12a0u;
    case 0x00672affu: return 0x009f129cu;
    case 0x00672b0du: return 0x009f1294u;
    case 0x00672b6bu: return 0x009f1284u;
    case 0x00672b79u: return 0x009f1278u;
    case 0x00672bd7u: return 0x009f1258u;
    case 0x00672be5u: return 0x009f1248u;
    case 0x00672bf3u: return 0x009f1234u;
    case 0x00672c51u: return 0x009f1220u;
    case 0x00672c5fu: return 0x009f1214u;
    case 0x00672cbdu: return 0x009f1208u;
    case 0x00672ccbu: return 0x009f1200u;
    case 0x00672d29u: return 0x009f11f8u;
    case 0x00672d37u: return 0x009f11f0u;
    case 0x00672d95u: return 0x009f11e0u;
    case 0x00672da3u: return 0x009f11d4u;
    case 0x00672e01u: return 0x009f11c0u;
    case 0x00672e0fu: return 0x009f11b4u;
    case 0x00672e6du: return 0x009f1198u;
    case 0x00672e7bu: return 0x009f1188u;
    case 0x00672ed9u: return 0x009f1170u;
    case 0x00672ee7u: return 0x009f1164u;
    case 0x00672f45u: return 0x009f1154u;
    case 0x00672f53u: return 0x009f1148u;
    case 0x00672fb1u: return 0x009f1134u;
    case 0x00672fbfu: return 0x009f1128u;
    case 0x0067301du: return 0x009f1118u;
    case 0x0067302bu: return 0x009f110cu;
    case 0x00673089u: return 0x009f10ecu;
    case 0x00673097u: return 0x009f10dcu;
    case 0x006730f5u: return 0x009f10c4u;
    case 0x00673103u: return 0x009f10b4u;
    case 0x00673161u: return 0x009f109cu;
    case 0x0067316fu: return 0x009f1090u;
    case 0x006731cdu: return 0x009f1078u;
    case 0x006731dbu: return 0x009f1068u;
    case 0x00673239u: return 0x009f1058u;
    case 0x00673247u: return 0x009f104cu;
    case 0x006732a5u: return 0x009f1030u;
    case 0x006732b3u: return 0x009f1020u;
    case 0x006732c1u: return 0x009f1010u;
    case 0x0067331fu: return 0x009f0ff8u;
    case 0x0067332du: return 0x009f0fecu;
    case 0x0067338bu: return 0x009f0fdcu;
    case 0x00673399u: return 0x009f0fd0u;
    case 0x006733f7u: return 0x009f0fbcu;
    case 0x00673405u: return 0x009f0fb0u;
    case 0x00673463u: return 0x009f0fa0u;
    case 0x00673471u: return 0x009f0f94u;
    case 0x006734cfu: return 0x009f0f88u;
    case 0x006734ddu: return 0x009f0f80u;
    case 0x0067353bu: return 0x009f0f58u;
    case 0x00673549u: return 0x009f0f44u;
    case 0x006735a7u: return 0x009f0f34u;
    case 0x006735b5u: return 0x009f0f28u;
    case 0x00673613u: return 0x009f0f18u;
    case 0x00673621u: return 0x009f0f0cu;
    case 0x0067367fu: return 0x009f0ef8u;
    case 0x0067368du: return 0x009f0eecu;
    case 0x006736ebu: return 0x009f0ed0u;
    case 0x006736f9u: return 0x009f0ec0u;
    case 0x0067375bu: return 0x009f0ea8u;
    case 0x00673769u: return 0x009f0e9cu;
    default: return 0; /* Original target 0067FD31: no suffix copy. */
    }
}
