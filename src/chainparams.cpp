// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2019 The ReviewBase_Coin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include <limits>


struct SeedSpec6 {
	uint8_t addr[16];
	uint16_t port;
};

#include "chainparamsseeds.h"

/**
* Main network
*/

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
	// It'll only connect to one or two seed nodes because once it connects,
	// it'll get a pile of addresses with newer timestamps.
	// Seed nodes are given a random 'last seen time' of between one and two
	// weeks ago.
	const int64_t nOneWeek = 7 * 24 * 60 * 60;
	for (unsigned int i = 0; i < count; i++) {
		struct in6_addr ip;
		memcpy(&ip, data[i].addr, sizeof(ip));
		CAddress addr(CService(ip, data[i].port));
		addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
		vSeedsOut.push_back(addr);
	}
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
boost::assign::map_list_of
(0, uint256("0x0000059b5bfa4d46f2fe779ef269feafe9114541c64deb7bd8440d85a6df4c04"))
(1, uint256("0x00000e87c82b4de51a03ddecca5cad5a4371223904baeceaef4a89570f306402"))
(871, uint256("0x5400e3334717a0925ee856fc703955734c27f23adfed0b2c85b2bd06ce3c58c3"))
(1452, uint256("0xf58efaf84b4761e336da45636d22f2063caba4c9d565c78205fc1f7307f4a746"))
(2318, uint256("0xa667e866914182ee40eaea7dc93b50386c80a73cecb08aeb5e9b91fc0aef457f"))
(3031, uint256("0x9949bfa5d45654fb527bf3cd1ed3cde507218842759f853ab9857d419ef69c7a"))
(5937, uint256("0x20b22309654da3a1082e120147e323045e451cfd059faecf57928f96462ea59d"))
;
static const Checkpoints::CCheckpointData data = {
	&mapCheckpoints,
	1569233228, // * UNIX timestamp of last checkpoint block
	11879,    // * total number of transactions between genesis and last checkpoint
				//   (the tx=... number in the SetBestChain debug.log lines)
	2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
boost::assign::map_list_of
(0, uint256("0x001"))
;
static const Checkpoints::CCheckpointData dataTestnet = {
	&mapCheckpointsTestnet,
	1560843157,
	2501682,
	250 };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
	&mapCheckpointsRegtest,
	1568872800,
	0,
	100 };

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
	assert(this);

	static CBigNum bnDecModulus = 0;
	if (!bnDecModulus)
		bnDecModulus.SetDec(zerocoinModulus);
	static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

	return &ZCParamsDec;
}

bool CChainParams::HasStakeMinAgeOrDepth(const int contextHeight, const uint32_t contextTime,
	const int utxoFromBlockHeight, const uint32_t utxoFromBlockTime) const
{
	// before stake modifier V2, the age required was 60 * 60 (1 hour) / not required on regtest
	if (!IsStakeModifierV2(contextHeight))
		return (NetworkID() == CBaseChainParams::REGTEST || (utxoFromBlockTime + 3600 <= contextTime));

	// after stake modifier V2, we require the utxo to be nStakeMinDepth deep in the chain
	return (contextHeight - utxoFromBlockHeight >= nStakeMinDepth);
}

class CMainParams : public CChainParams
{
public:
	CMainParams()
	{
		networkID = CBaseChainParams::MAIN;
		strNetworkID = "main";
		/**
		* The message start string is designed to be unlikely to occur in normal data.
		* The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		* a large 4-byte int at any alignment.
		*/
		pchMessageStart[0] = 0x17;
		pchMessageStart[1] = 0xb2;
		pchMessageStart[2] = 0xae;
		pchMessageStart[3] = 0xc1;
		vAlertPubKey = ParseHex("0000098d3bc6ba6e7143ba5cbd6a85e0a9a5313f18d332b44c5ab1a8b7ed6b1eaa335fc8dc4f012cb8a41cc0bdafd6ca70a5f54d2916e4e6f511bcd746ed57dc50");
		nDefaultPort = 11915;
		bnProofOfWorkLimit = ~uint256(0) >> 20; // ReviewBase_Coin starting difficulty is 1 / 2^12
		nSubsidyHalvingInterval = 210000;
		nMaxReorganizationDepth = 100;
		nEnforceBlockUpgradeMajority = 8100; // 75%
		nRejectBlockOutdatedMajority = 10260; // 95%
		nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
		nMinerThreads = 0;
		nTargetSpacing = 1 * 60;        // 1 minute
		nMaturity = 61;
		nStakeMinDepth = 600;
		nFutureTimeDriftPoW = 7200;
		nFutureTimeDriftPoS = 180;
		nMasternodeCountDrift = 20;
		nMaxMoneyOut = 60000000 * COIN;

		/** Height or Time Based Activations **/
		nLastPOWBlock = 225;
		nReviewbase_CoinBadBlockTime = 1471401614; // Skip nBit validation of Block 259201 per PR #915
		nReviewbase_CoinBadBlocknBits = 0x1c056dac; // Skip nBit validation of Block 259201 per PR #915
		nModifierUpdateBlock = 0;
		nZerocoinStartHeight = 235;
		nZerocoinStartTime = 1568872800; // Thursday, September 19, 2019 6:00:00 AM
		nBlockZerocoinV2 = 235;
		nBlockDoubleAccumulated = 0;
		nBlockStakeModifierlV2 = 1000;
		// Public coin spend enforcement
		nPublicZCSpends = 300;

		/**
		* Build the genesis block. Note that the output of the genesis coinbase cannot
		* be spent as it did not originally exist in the database.
		*
		* CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
		*   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
		*     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
		*     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
		*   vMerkleTree: e0028e
		*/
		const char* pszTimestamp = "Google Maps 10.26 hints at a new “Eyes Free” Walking Navigation Mode.";
		CMutableTransaction txNew;
		txNew.vin.resize(1);
		txNew.vout.resize(1);
		txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
		txNew.vout[0].SetEmpty();
		txNew.vout[0].scriptPubKey = CScript() << ParseHex("04c10e83b27031cf322f7dad62dd5813ac7c10bd0a5814ce121ba32607d573b8810c02c0682aed05b4deb9c4b77b26dd2418c61256cd42774babeb0a073b2ed0e9") << OP_CHECKSIG;
		genesis.vtx.push_back(txNew);
		genesis.hashPrevBlock = 0;
		genesis.hashMerkleRoot = genesis.BuildMerkleTree();
		genesis.nVersion = 1;
		genesis.nTime = 1568872800;
		genesis.nBits = 0x1e0ffff0;
		genesis.nNonce = 705267;

		hashGenesisBlock = genesis.GetHash();
		assert(hashGenesisBlock == uint256("0x0000059b5bfa4d46f2fe779ef269feafe9114541c64deb7bd8440d85a6df4c04"));
		assert(genesis.hashMerkleRoot == uint256("0xe5f16f9f7bb0787b439c07b887e5479799a7b73a9159aabcab3ad6bc24fef582"));

		vSeeds.push_back(CDNSSeedData("0", "157.245.164.19"));
		vSeeds.push_back(CDNSSeedData("1", "2604:a880:2:d1::6b:c001"));
		vSeeds.push_back(CDNSSeedData("2", "167.71.117.89"));
		vSeeds.push_back(CDNSSeedData("3", "2604:a880:2:d1::6d:2001"));
		vSeeds.push_back(CDNSSeedData("4", "167.71.117.112"));
		vSeeds.push_back(CDNSSeedData("5", "2604:a880:2:d1::6a:f001"));
		vSeeds.push_back(CDNSSeedData("6", "167.71.125.170"));
		vSeeds.push_back(CDNSSeedData("7", "2604:a880:2:d1::78:8001"));
		vSeeds.push_back(CDNSSeedData("8", "167.71.115.18"));
		vSeeds.push_back(CDNSSeedData("9", "2604:a880:2:d1::5a:c001"));
		vSeeds.push_back(CDNSSeedData("10", "167.71.126.74"));
		vSeeds.push_back(CDNSSeedData("11", "2604:a880:2:d1::44:f001"));
		vSeeds.push_back(CDNSSeedData("12", "157.245.173.128"));
		vSeeds.push_back(CDNSSeedData("13", "2604:a880:2:d1::78:a001"));

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 60);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
		// 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
		base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

		convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

		fMiningRequiresPeers = true;
		fAllowMinDifficultyBlocks = false;
		fDefaultConsistencyChecks = false;
		fRequireStandard = true;
		fMineBlocksOnDemand = false;
		fSkipProofOfWorkCheck = false;
		fTestnetToBeDeprecatedFieldRPC = false;
		fHeadersFirstSyncingActive = false;

		nPoolMaxTransactions = 3;
		nBudgetCycleBlocks = 43200; //!< Amount of blocks in a months period of time (using 1 minutes per) = (60*24*30)
		strSporkKey = "04d578e235bddbc57756ea20c61ad1f0923eb1ec7fd89d4d5dd27f3197f84f1c2deb87ac1b5e3e3ad7eaefe52f6ae24ab222c73a6cedcbb9f3be7772f05b70ac99";
		strObfuscationPoolDummyAddress = "RAH1oBdS66oYtmaKYdKshjXfqBb5XwEhT9";
		nStartMasternodePayments = 1568962800; //Friday, September 20, 2019 7:00:00 AM GMT

											   /** Zerocoin */
		zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
			"4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
			"6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
			"7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
			"8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
			"31438167899885040445364023527381951378636564391212010397122822120720357";
		nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
		nMaxZerocoinPublicSpendsPerTransaction = 637; // Assume about 220 bytes each input
		nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
		nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
		nRequiredAccumulation = 1;
		nDefaultSecurityLevel = 100; //full security level for accumulators
		nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
		nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zrview to be stakable

		nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
		nProposalEstablishmentTime = 60 * 60 * 24; // Proposals must be at least a day old to make it into a budget
	}

	const Checkpoints::CCheckpointData& Checkpoints() const
	{
		return data;
	}

};
static CMainParams mainParams;

/**
* Testnet (v3)
*/
class CTestNetParams : public CMainParams
{
public:
	CTestNetParams()
	{
		networkID = CBaseChainParams::TESTNET;
		strNetworkID = "test";
		pchMessageStart[0] = 0x45;
		pchMessageStart[1] = 0x76;
		pchMessageStart[2] = 0x65;
		pchMessageStart[3] = 0xba;
		vAlertPubKey = ParseHex("000010e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9");
		nDefaultPort = 51474;
		nEnforceBlockUpgradeMajority = 4320; // 75%
		nRejectBlockOutdatedMajority = 5472; // 95%
		nToCheckBlockUpgradeMajority = 5760; // 4 days
		nMinerThreads = 0;
		nTargetSpacing = 1 * 60;  // ReviewBase_Coin: 1 minute
		nLastPOWBlock = 200;
		nReviewbase_CoinBadBlockTime = 1489001494; // Skip nBit validation of Block 259201 per PR #915
		nReviewbase_CoinBadBlocknBits = 0x1e0a20bd; // Skip nBit validation of Block 201 per PR #915
		nMaturity = 15;
		nStakeMinDepth = 100;
		nMasternodeCountDrift = 4;
		nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
		nMaxMoneyOut = 43199500 * COIN;
		nZerocoinStartHeight = 201576;
		nZerocoinStartTime = 1501776000;
		nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
		nBlockStakeModifierlV2 = 1214000;
		// Public coin spend enforcement
		nPublicZCSpends = 1106100;

		//! Modify the testnet genesis block so the timestamp is valid for a later start.
		genesis.nTime = 1568872800;
		genesis.nNonce = 2402015;

		//hashGenesisBlock = genesis.GetHash();
		//assert(hashGenesisBlock == uint256("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));

		vFixedSeeds.clear();
		vSeeds.clear();
		vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "reviewbase_coin-testnet.seed.fuzzbawls.pw"));
		vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "reviewbase_coin-testnet.seed2.fuzzbawls.pw"));
		vSeeds.push_back(CDNSSeedData("warrows.dev", "testnet.dnsseed.reviewbase_coin.warrows.dev"));

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet reviewbase_coin addresses start with 'x' or 'y'
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet reviewbase_coin script addresses start with '8' or '9'
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
																			 // Testnet reviewbase_coin BIP32 pubkeys start with 'DRKV'
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
		// Testnet reviewbase_coin BIP32 prvkeys start with 'DRKP'
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
		// Testnet reviewbase_coin BIP44 coin type is '1' (All coin's testnet default)
		base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

		convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

		fMiningRequiresPeers = true;
		fAllowMinDifficultyBlocks = true;
		fDefaultConsistencyChecks = false;
		fRequireStandard = true;
		fMineBlocksOnDemand = false;
		fTestnetToBeDeprecatedFieldRPC = true;

		nPoolMaxTransactions = 2;
		nBudgetCycleBlocks = 144; //!< Ten cycles per day on testnet
		strSporkKey = "04E88BB455E2A04E65FCC41D88CD367E9CCE1F5A409BE94D8C2B4B35D223DED9C8E2F4E061349BA3A38839282508066B6DC4DB72DD432AC4067991E6BF20176127";
		strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
		nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
		nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
									   // here because we only have a 8 block finalization window on testnet

		nProposalEstablishmentTime = 60 * 5; // Proposals must be at least 5 mns old to make it into a test budget
	}
	const Checkpoints::CCheckpointData& Checkpoints() const
	{
		return dataTestnet;
	}
};
static CTestNetParams testNetParams;

/**
* Regression test
*/
class CRegTestParams : public CTestNetParams
{
public:
	CRegTestParams()
	{
		networkID = CBaseChainParams::REGTEST;
		strNetworkID = "regtest";
		pchMessageStart[0] = 0xa1;
		pchMessageStart[1] = 0xcf;
		pchMessageStart[2] = 0x7e;
		pchMessageStart[3] = 0xac;
		nDefaultPort = 51476;
		nSubsidyHalvingInterval = 150;
		nEnforceBlockUpgradeMajority = 750;
		nRejectBlockOutdatedMajority = 950;
		nToCheckBlockUpgradeMajority = 1000;
		nMinerThreads = 1;
		nTargetSpacing = 1 * 60;        // ReviewBase_Coin: 1 minutes
		bnProofOfWorkLimit = ~uint256(0) >> 1;
		nLastPOWBlock = 250;
		nMaturity = 100;
		nStakeMinDepth = 0;
		nMasternodeCountDrift = 4;
		nModifierUpdateBlock = 0; //approx Mon, 17 Apr 2017 04:00:00 GMT
		nMaxMoneyOut = 43199500 * COIN;
		nZerocoinStartHeight = 300;
		nBlockZerocoinV2 = 300;
		nZerocoinStartTime = 1501776000;
		nBlockStakeModifierlV2 = std::numeric_limits<int>::max(); // max integer value (never switch on regtest)
																  // Public coin spend enforcement
		nPublicZCSpends = 350;


		//! Modify the regtest genesis block so the timestamp is valid for a later start.
		genesis.nTime = 1568872800;
		genesis.nNonce = 2402015;

		//hashGenesisBlock = genesis.GetHash();
		//assert(hashGenesisBlock == uint256("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));
		//assert(hashGenesisBlock == uint256("0x4f023a2120d9127b21bbad01724fdb79b519f593f2a85b60d3d79160ec5f29df"));

		vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
		vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

		fMiningRequiresPeers = false;
		fAllowMinDifficultyBlocks = true;
		fDefaultConsistencyChecks = true;
		fRequireStandard = false;
		fMineBlocksOnDemand = true;
		fSkipProofOfWorkCheck = true;
		fTestnetToBeDeprecatedFieldRPC = false;

		/* Spork Key for RegTest:
		WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
		private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
		Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
		*/
		strSporkKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
	}
	const Checkpoints::CCheckpointData& Checkpoints() const
	{
		return dataRegtest;
	}
};
static CRegTestParams regTestParams;

/**
* Unit test
*/
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
	CUnitTestParams()
	{
		networkID = CBaseChainParams::UNITTEST;
		strNetworkID = "unittest";
		nDefaultPort = 51478;
		vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
		vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

		fMiningRequiresPeers = false;
		fDefaultConsistencyChecks = true;
		fAllowMinDifficultyBlocks = false;
		fMineBlocksOnDemand = true;
	}

	const Checkpoints::CCheckpointData& Checkpoints() const
	{
		// UnitTest share the same checkpoints as MAIN
		return data;
	}

	//! Published setters to allow changing values in unit test cases
	virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
	virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
	virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
	virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
	virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
	virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
	virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;

static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
	assert(pCurrentParams);
	assert(pCurrentParams == &unitTestParams);
	return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
	assert(pCurrentParams);
	return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
	switch (network) {
	case CBaseChainParams::MAIN:
		return mainParams;
	case CBaseChainParams::TESTNET:
		return testNetParams;
	case CBaseChainParams::REGTEST:
		return regTestParams;
	case CBaseChainParams::UNITTEST:
		return unitTestParams;
	default:
		assert(false && "Unimplemented network");
		return mainParams;
	}
}

void SelectParams(CBaseChainParams::Network network)
{
	SelectBaseParams(network);
	pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
	CBaseChainParams::Network network = NetworkIdFromCommandLine();
	if (network == CBaseChainParams::MAX_NETWORK_TYPES)
		return false;

	SelectParams(network);
	return true;
}
