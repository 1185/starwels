// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Starwels developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "January/20/2017 45th President of the United States of America Donald Trump";
    const CScript genesisOutputScript = CScript() << ParseHex("04ea1e6e7cace7b63b88949a3f43f0144b0032eaa9ee6c9627fc58bfb51163a262542fd8cdd3cc40186a1aeeb4857c15954e6f15f789bfdcf9cec59863cdfc6e14") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Height = 173805; // 0x00000000cf1f156ba3611b74c85475d4feca9cee39ceeec1460a02feff479106
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x00000000ac6a39893714f4240301f40abff1afcdcaf51d2f40ce6675a73f0961");
        consensus.BIP65Height = 388381; // 0x000000003bad725a4d305cacb92e5854159238c6899b8241b8c7398b7da47336
        consensus.BIP66Height = 363725; // 0x0000000013d266812076495e2f406e6840745b000b8c28dc94d20d4b910e5802
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2 * 24 * 84;
        consensus.nPowTargetSpacing = 2; // 2 second
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1498867200; //  July 1 Saturday, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1530403200; // July 1 Sunday, 2018

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1501545600; // August 1 Tuesday, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533081600; // August 1 Wednesday, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1504224000; // September 1 Friday, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1535760000; // September 1 Saturday, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000e6ea97648e7ab");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000000b53cc9d25e756bbabeb427632dd36339d268695d3ddf884317a1af7"); // 524288

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;
        nDefaultPort = 43110;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1484870400, 2121032621, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000003d69a915e9da53348c5c272978bb743442e3a6341c11061c125811a2"));
        assert(genesis.hashMerkleRoot == uint256S("0x7e2c59b1404833991962e7e6d95a1d4f81f03fffeaf79c0d25d97d24182db485"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they dont support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("127.0.0.1:43110/139H5wy8H3tgXwJ33mXpwmm4oCfjKUo7dY"); // ZeroNet
        /**
        "zeroid.bit": {
          "auth_address": "139H5wy8H3tgXwJ33mXpwmm4oCfjKUo7dY",
          "auth_privatekey": "5JTywMxQgDZyFQEGoBLbSHazrLKhfZP3f4W8qQxtA1r3JXKCega",
          "cert_sign": "HICD7py5jbnkZRRDSG8eavs6wI1MzHQHKd8MXczaMMZTUqFaYEA3i3aU/Ucgm7Epafj46gKopsj6YW1OGgw0Rv8="
        }
        */

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "ai";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                // 1517320634 1556464671 1517409431 704136802 1529914513 1155211149
                {1024, uint256S("0x00000000fb559851169e0d915093533d31af0d963e06a7d80a20496f9cfd2b9f")},
                {2048, uint256S("0x00000000a753b360bb5d54908378d999132fbeb415279ef530b397ae249cbef4")},
                {4096, uint256S("0x00000000d7674ad1e7703e7ba02b7611c12fe74de27b6a414cf6e906cc3599e9")},
                // 1530100737 3507130640
                {8192, uint256S("0x000000007039ae6930925c9c6297835642ea5a4d6f32cef2f6b84867e049fff8")},
                                                                                                          {11111, uint256S("0x00000000f293b24e32076da179e26117328f717b5788ed6be61ac094ebfcc848")},
                // 1530115458 2127567142 1530157844 3201332445
                {16384, uint256S("0x00000000b6b20859ba118746cb4bbe4a52adbe504fceb2105092fd1b4234afc9")},
                {32768, uint256S("0x00000000cc19cff47d676ad56594e695ac96a1d2781a83a5fd1a7702b98a9a65")},
                                                                                                          {33333, uint256S("0x000000006e6325f0574ed633e957c054a68cf3cd31b65a5c63e3e76481f325a4")},
                // 1530240588 1199797262
                {65536, uint256S("0x00000000c6814c6715cda6f76b08b967abc0ea70aa3f8db8e8c62206477cb749")},
                                                                                                          {74000, uint256S("0x00000000d19c2aac56911372d964a9e89845459869ffd1ee3910ab048d9faf5e")},
                                                                                                          {105000, uint256S("0x00000000eca71f0c3abf5745144fd17e88593f36f433257b2a35a11f8561d7ce")},
                // 1530390152 555394614
                {131072, uint256S("0x00000000dbff9cae44489b3c02bbd556b67db5e3d21318c35cca0b6c361e6944")},
                                                                                                          {134444, uint256S("0x000000001022357320b444edb994a291a02d0831bffcaefaf2ead58a2716f563")},
                                                                                                          {168000, uint256S("0x0000000048908a1d941c28992d79deeff14e7312ac3969cf4950bd6129d577d6")},
                                                                                                          {193000, uint256S("0x000000000c66a5ddbb94e334b6ade2cc01e1e27f676760df8bcde2b5195c2f2e")},
                                                                                                          {210000, uint256S("0x0000000047a4fa17266bf33f228fcf370206fb1948096521efd1baf00f1e94d9")},
                                                                                                          {216116, uint256S("0x000000005d7781fc151d19ead1757a69233f9cae16de1ee1ff97058617d7bf1f")},
                                                                                                          {225430, uint256S("0x000000000b6ca8ef743c7681c3858dc1497e32530a1b9e319100c8dc46af233f")},
                                                                                                          {250000, uint256S("0x0000000050818b5dde86d7fd8ffcd45e4cdc6e8b8fc49ec638629840d455c68f")},
                // 1530692464 763374868
                {262144, uint256S("0x00000000764077b29d13e1cb2484f028a24ef2a44486b6e5d3cb17eb4716c465")},
                                                                                                          {279000, uint256S("0x000000002ef9de43b58ca79936d1f60021c18eb7b64f06662ebdeb5881b44652")},
                                                                                                          {295000, uint256S("0x0000000095c429e9d8b947c3c25267adc31f98484b6b6034da600fa55802f41e")},
                // 1531254946 2429180701 1532461464 3251947141 1535645986 3994357250
                {524288, uint256S("0x000000000b53cc9d25e756bbabeb427632dd36339d268695d3ddf884317a1af7")},
                {1048576, uint256S("0x0000000000005f2135fde2a7944711b3a4c94117737aef500618395226ea98f3")},
                {2097152, uint256S("0x00000000001a462b7ab372ba813ef5bbbc84185d32bcc6d506ad749630f3187d")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 0x000000000006b154980f738a5d8359344f1c3c43fe0209da7b4033f379c3a413 (height 4194304).
            1546793987, // * UNIX timestamp of last known number of transactions
            4243099,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            3.5         // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Starwels Ai Network
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Height = 173805; // 0x00000000cf1f156ba3611b74c85475d4feca9cee39ceeec1460a02feff479106
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x00000000ac6a39893714f4240301f40abff1afcdcaf51d2f40ce6675a73f0961");
        consensus.BIP65Height = 388381; // 0x000000003bad725a4d305cacb92e5854159238c6899b8241b8c7398b7da47336
        consensus.BIP66Height = 363725; // 0x0000000013d266812076495e2f406e6840745b000b8c28dc94d20d4b910e5802
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2 * 24 * 84;
        consensus.nPowTargetSpacing = 2; // 2 second
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1498867200; //  July 1 Saturday, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1530403200; // July 1 Sunday, 2018

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1501545600; // August 1 Tuesday, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533081600; // August 1 Wednesday, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1504224000; // September 1 Friday, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1535760000; // September 1 Saturday, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000100110011001");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000d7674ad1e7703e7ba02b7611c12fe74de27b6a414cf6e906cc3599e9"); // 4096

        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;
        nDefaultPort = 8333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1484870400, 2121032621, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000003d69a915e9da53348c5c272978bb743442e3a6341c11061c125811a2"));
        assert(genesis.hashMerkleRoot == uint256S("0x7e2c59b1404833991962e7e6d95a1d4f81f03fffeaf79c0d25d97d24182db485"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("127.0.0.1:43110/139H5wy8H3tgXwJ33mXpwmm4oCfjKUo7dY");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "ai";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
                // 1517320634 1556464671 1517409431 704136802
                {1024, uint256S("0x00000000fb559851169e0d915093533d31af0d963e06a7d80a20496f9cfd2b9f")},
                {2048, uint256S("0x00000000a753b360bb5d54908378d999132fbeb415279ef530b397ae249cbef4")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 0x00000000d7674ad1e7703e7ba02b7611c12fe74de27b6a414cf6e906cc3599e9 (height 4096)
            1529914513, // * UNIX timestamp of last known number of transactions
            4172,       // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            3.5         // * estimated number of transactions per second after that timestamp
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2 * 24 * 84;
        consensus.nPowTargetSpacing = 2; // 2 second
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1484956800, 0, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x79a6a4d5e19d4e5c6783691ba9ad75c7c352f906275b93dcad27ea0c3017ec80"));
        assert(genesis.hashMerkleRoot == uint256S("0x7e2c59b1404833991962e7e6d95a1d4f81f03fffeaf79c0d25d97d24182db485"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("79a6a4d5e19d4e5c6783691ba9ad75c7c352f906275b93dcad27ea0c3017ec80")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "airt";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::AI)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
