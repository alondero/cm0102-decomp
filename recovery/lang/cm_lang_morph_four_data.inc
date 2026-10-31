/* Original language-four dispatch descriptors, prepare-morph-four.py.
 * Qualifiers, pointer tables and literals remain in the original image. */
typedef struct { cm_lang_u32 qualifier, dispatch; } four_rule;
static const four_rule four_prefix_rules[] = {
    {0x9f14bcu, 0x6804a8u},
    {0x9f14b4u, 0x6804ccu},
    {0x9f0e6cu, 0x680610u},
    {0x9f0e64u, 0x680610u},
    {0x9f0e5cu, 0x6804f0u},
    {0x9f0e18u, 0x680514u},
    {0x9f0dd4u, 0x680538u},
    {0x9f0d90u, 0x6805ecu},
    {0x99c35cu, 0x6805ecu},
    {0x9f0d88u, 0x68055cu},
    {0x9f0d4cu, 0x680580u},
    {0x9f0d24u, 0x6805a4u},
    {0x9f0d08u, 0x6805c8u},
};
static const four_rule four_suffix_rules[] = {
    {0x9f0c78u, 0x680634u},
    {0x9f0c3cu, 0x680658u},
    {0x9f0c00u, 0x68067cu},
    {0x9f0d4cu, 0x6806a0u},
    {0x9f0b74u, 0x6806c4u},
    {0x9f0b30u, 0x6806e8u},
    {0x9f0accu, 0x68070cu},
    {0x9f0a8cu, 0x680730u},
    {0x9f0a34u, 0x680754u},
    {0x9f09dcu, 0x680778u},
    {0x9f09a8u, 0x68079cu},
    {0x9f096cu, 0x6807c0u},
    {0x9f0928u, 0x6807e4u},
    {0x9f08c0u, 0x680808u},
    {0x9f086cu, 0x68082cu},
    {0x9f0834u, 0x680850u},
};
static cm_lang_u32 four_literal(cm_lang_u32 target) {
    switch (target) {
    case 0x00673899u: return 0x009f0e98u;
    case 0x006738aau: return 0x009f0e94u;
    case 0x006738bbu: return 0x009f130cu;
    case 0x006738ccu: return 0x009f0e90u;
    case 0x006738ddu: return 0x009f0e88u;
    case 0x006738eeu: return 0x009f14d4u;
    case 0x006738ffu: return 0x009f14d8u;
    case 0x00673964u: return 0x00ad9c64u;
    case 0x00673975u: return 0x009f0e84u;
    case 0x00673986u: return 0x009f0e80u;
    case 0x00673997u: return 0x009f14b0u;
    case 0x006739a8u: return 0x009f0e7cu;
    case 0x006739b9u: return 0x009f0e74u;
    case 0x006739cau: return 0x009f14e8u;
    case 0x006739dbu: return 0x009f14ecu;
    case 0x00673ab8u: return 0x009f0e58u;
    case 0x00673ac9u: return 0x009f0e50u;
    case 0x00673adau: return 0x009f0e48u;
    case 0x00673aebu: return 0x009f0e40u;
    case 0x00673afcu: return 0x009f0e38u;
    case 0x00673b0du: return 0x009f0e30u;
    case 0x00673b1eu: return 0x009f0e28u;
    case 0x00673b2fu: return 0x009f0e20u;
    case 0x00673b94u: return 0x009f0e14u;
    case 0x00673ba5u: return 0x009f0e0cu;
    case 0x00673bb6u: return 0x009f0e04u;
    case 0x00673bc7u: return 0x009f0dfcu;
    case 0x00673bd8u: return 0x009f0df4u;
    case 0x00673be9u: return 0x009f0decu;
    case 0x00673bfau: return 0x009f0de4u;
    case 0x00673c0bu: return 0x009f0ddcu;
    case 0x00673c70u: return 0x009f0dd0u;
    case 0x00673c81u: return 0x009f0dc8u;
    case 0x00673c92u: return 0x009f0dc0u;
    case 0x00673ca3u: return 0x009f0db8u;
    case 0x00673cb4u: return 0x009f0db0u;
    case 0x00673cc5u: return 0x009f0da8u;
    case 0x00673cd6u: return 0x009f0da0u;
    case 0x00673ce7u: return 0x009f0d98u;
    case 0x00673dc4u: return 0x009f15c8u;
    case 0x00673dd5u: return 0x009f0d84u;
    case 0x00673de6u: return 0x009f0d7cu;
    case 0x00673df7u: return 0x009f0d74u;
    case 0x00673e08u: return 0x009f0d70u;
    case 0x00673e19u: return 0x009f0d68u;
    case 0x00673e2au: return 0x009f0d60u;
    case 0x00673e3bu: return 0x009f0d58u;
    case 0x00673e9cu: return 0x009f0d40u;
    case 0x00673eadu: return 0x009f0d4cu;
    case 0x00673ebeu: return 0x009f0d34u;
    case 0x00673ecfu: return 0x009f0d28u;
    case 0x00673f30u: return 0x009f0d20u;
    case 0x00673f41u: return 0x009f0d18u;
    case 0x00673f52u: return 0x009f0d10u;
    case 0x00673f63u: return 0x009f0d0cu;
    case 0x00673fc4u: return 0x009f0d04u;
    case 0x00673fd5u: return 0x009f0cfcu;
    case 0x00673fe6u: return 0x009f0cf4u;
    case 0x00673ff7u: return 0x009f0cf0u;
    case 0x0067404cu: return 0x009f15d4u;
    case 0x0067405du: return 0x009f0cecu;
    case 0x0067406eu: return 0x009f0ce4u;
    case 0x0067407fu: return 0x009f0cdcu;
    case 0x00674090u: return 0x009f0cd8u;
    case 0x006740a1u: return 0x009f0cd0u;
    case 0x006740b2u: return 0x009f0cc8u;
    case 0x006740c3u: return 0x009f0cc0u;
    case 0x006740e8u: return 0x009f0cbcu;
    case 0x006740f6u: return 0x009f0cb4u;
    case 0x00674104u: return 0x009f0cacu;
    case 0x00674112u: return 0x009f0ca4u;
    case 0x00674120u: return 0x009f0c9cu;
    case 0x0067412eu: return 0x009f0c94u;
    case 0x0067413cu: return 0x009f0c8cu;
    case 0x0067414au: return 0x009f0c84u;
    case 0x0067421eu: return 0x009f0c6cu;
    case 0x0067422cu: return 0x009f0c60u;
    case 0x0067423au: return 0x009f0c54u;
    case 0x00674248u: return 0x009f0c48u;
    case 0x00674256u: return 0x009f0c6cu;
    case 0x006742b4u: return 0x009f0c30u;
    case 0x006742c2u: return 0x009f0c24u;
    case 0x006742d0u: return 0x009f0c18u;
    case 0x006742deu: return 0x009f0c0cu;
    case 0x006742ecu: return 0x009f0c30u;
    case 0x0067434au: return 0x009f0bf0u;
    case 0x00674358u: return 0x009f0be0u;
    case 0x00674366u: return 0x009f0bd0u;
    case 0x00674374u: return 0x009f0bc0u;
    case 0x00674382u: return 0x009f0bf0u;
    case 0x006743e0u: return 0x009f0bb0u;
    case 0x006743eeu: return 0x009f0ba0u;
    case 0x006743fcu: return 0x009f0b90u;
    case 0x0067440au: return 0x009f0b80u;
    case 0x00674418u: return 0x009f0bb0u;
    case 0x00674476u: return 0x009f0b68u;
    case 0x00674484u: return 0x009f0b5cu;
    case 0x00674492u: return 0x009f0b50u;
    case 0x006744a0u: return 0x009f0b44u;
    case 0x006744aeu: return 0x009f0b68u;
    case 0x0067450cu: return 0x009f0b1cu;
    case 0x0067451au: return 0x009f0b08u;
    case 0x00674528u: return 0x009f0af0u;
    case 0x00674536u: return 0x009f0ad8u;
    case 0x00674544u: return 0x009f0b08u;
    case 0x006745a2u: return 0x009f0ac0u;
    case 0x006745b0u: return 0x009f0ab4u;
    case 0x006745beu: return 0x009f0aa8u;
    case 0x006745ccu: return 0x009f0a9cu;
    case 0x006745dau: return 0x009f0ac0u;
    case 0x00674638u: return 0x009f0a7cu;
    case 0x00674646u: return 0x009f0a6cu;
    case 0x00674654u: return 0x009f0a58u;
    case 0x00674662u: return 0x009f0a44u;
    case 0x00674670u: return 0x009f0a7cu;
    case 0x006746ceu: return 0x009f0a20u;
    case 0x006746dcu: return 0x009f0a0cu;
    case 0x006746eau: return 0x009f09f8u;
    case 0x006746f8u: return 0x009f09e4u;
    case 0x00674706u: return 0x009f0a20u;
    case 0x00674764u: return 0x009f09d0u;
    case 0x00674772u: return 0x009f09c4u;
    case 0x00674780u: return 0x009f09d0u;
    case 0x0067482fu: return 0x009f099cu;
    case 0x0067483du: return 0x009f0990u;
    case 0x0067484bu: return 0x009f0984u;
    case 0x00674859u: return 0x009f0978u;
    case 0x00674867u: return 0x009f099cu;
    case 0x006748c5u: return 0x009f0960u;
    case 0x006748d3u: return 0x009f0954u;
    case 0x006748e1u: return 0x009f0948u;
    case 0x006748efu: return 0x009f093cu;
    case 0x006748fdu: return 0x009f0960u;
    case 0x0067495bu: return 0x009f0914u;
    case 0x00674969u: return 0x009f0900u;
    case 0x00674977u: return 0x009f08e8u;
    case 0x00674985u: return 0x009f08d0u;
    case 0x00674993u: return 0x009f0914u;
    case 0x006749f1u: return 0x009f08b0u;
    case 0x006749ffu: return 0x009f08a0u;
    case 0x00674a0du: return 0x009f088cu;
    case 0x00674a1bu: return 0x009f0878u;
    case 0x00674a29u: return 0x009f08b0u;
    case 0x00674a87u: return 0x009f0860u;
    case 0x00674a95u: return 0x009f0854u;
    case 0x00674aa3u: return 0x009f0848u;
    case 0x00674ab1u: return 0x009f083cu;
    case 0x00674abfu: return 0x009f0860u;
    case 0x00674b1du: return 0x009f082cu;
    case 0x00674b2bu: return 0x009f0824u;
    case 0x00674b39u: return 0x009f082cu;
    default: return 0; /* Verified 0067FD31 skips a copy. */
    }
}
