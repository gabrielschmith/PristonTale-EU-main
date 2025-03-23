#pragma once

#define ITEM_SERVER_MAX				1500
#define INVENTORYSERVER_MAX			100
#define	MAX_ITEMSINITEMBOX			100

#define UNIQUEID_QUEST_WEAPON		2

enum EWeaponClass
{
	WEAPONCLASS_None,
	WEAPONCLASS_Melee,
	WEAPONCLASS_Ranged,
	WEAPONCLASS_Magic,
};

/// <summary>
/// NOTE: once something is defined
/// DO NOT CHANGE. Always add to end
/// otherwise you break existing item's source
/// </summary>
enum class EItemSource : unsigned char
{
	Unknown            = 0,
	MonsterKill		   = 0x01,
	BossKill		   = 0x02,
	PlayerThrown       = 0x03,
	BellatraKill       = 0x04,
	BellatraReward     = 0x05,
	PlayerTrade		   = 0x06,
	NPCShop			   = 0x07,
	PlayerShop         = 0x08,
	T5Quest			   = 0x09,
	ChristmasEvent     = 0x10,
	WantedWolfEvent    = 0x11,
	WantedMoriphEvent  = 0x12,
	EasterEvent		   = 0x13,
	PerfectView        = 0x14,
	CoinShopPerfectItem = 0x15,
	FuryStone          = 0x16,
	HalloweenEvent     = 0x17,
	CleanStone	       = 0x18,
	ItemBox			   = 0x19,
	ManufactureSwap    = 0x20,
	Respec             = 0x21, //Jera, Gufu, Nerd
	QuestReward        = 0x22,
	QuestKill          = 0x23,
	GameMaster         = 0x24, //Created by GM
	MixedSocket        = 0x25,
	MimicKill          = 0x26,
	CoinShop           = 0x27,
};



enum ELootFilter : unsigned int
{
	LOOTFILTER_None				= 0x0,
	LOOTFILTER_HPPotion			= 0x0002,
	LOOTFILTER_MPPotion			= 0x0004,
	LOOTFILTER_SPPotion			= 0x0008,
	LOOTFILTER_Gold				= 0x0010,

	LOOTFILTER_Jewellery		= 0x0020,
	LOOTFILTER_Gear				= 0x0040,
	LOOTFILTER_Sheltoms			= 0x0080,
	LOOTFILTER_ForceOrbs		= 0x0100,
	LOOTFILTER_Crystals			= 0x0200,
	LOOTFILTER_NonSpecSheltoms	= 0x0400,
	LOOTFILTER_NonSpecJewellery = 0x0800,
	LOOTFILTER_NonSpecGear		= 0x1000,

	LOOTFILTER_All = LOOTFILTER_HPPotion | LOOTFILTER_MPPotion | LOOTFILTER_SPPotion | LOOTFILTER_Gold | LOOTFILTER_Jewellery |
					 LOOTFILTER_Gear | LOOTFILTER_Sheltoms | LOOTFILTER_ForceOrbs | LOOTFILTER_Crystals | LOOTFILTER_NonSpecSheltoms |
					 LOOTFILTER_NonSpecJewellery | LOOTFILTER_NonSpecGear
};

enum ELootFilterSpec : unsigned int
{
	LOOTFILTERSPEC_None			= 0x0,
	LOOTFILTERSPEC_NS			= 0x0002,
	LOOTFILTERSPEC_FS			= 0x0004,
	LOOTFILTERSPEC_MS			= 0x0008,
	LOOTFILTERSPEC_AS			= 0x0010,
	LOOTFILTERSPEC_PS			= 0x0020,
	LOOTFILTERSPEC_ATA			= 0x0040,
	LOOTFILTERSPEC_KS			= 0x0080,
	LOOTFILTERSPEC_MGS			= 0x0100,
	LOOTFILTERSPEC_PRS			= 0x0200,
	LOOTFILTERSPEC_ASS			= 0x0400,
	LOOTFILTERSPEC_SHA			= 0x0800,
};

enum EItemID
{
	ITEMID_None											= 0,

	ITEMID_RecipeCode									= 0x0A020000,
	ITEMID_RuneCodeManufacture1							= 0x03100000,

	ITEMID_HopyCrystal									= 0x08020100,
	ITEMID_HobgoblinCrystal								= 0x08020200,
	ITEMID_DecoyCrystal									= 0x08020300,
	ITEMID_BargonCrystal								= 0x08020400,
	ITEMID_HeadCutterCrystal							= 0x08020500,
	ITEMID_FigonCrystal									= 0x08020600,
	ITEMID_KingHopyCrystal								= 0x08020700,
	ITEMID_HulkCrystal									= 0x08020800,
	ITEMID_MysticCrystal								= 0x08020900,
	ITEMID_GuardianSaintCrystal							= 0x08020A00,
	ITEMID_WebCrystal									= 0x08020B00,
	ITEMID_DarkSpecterCrystal							= 0x08020C00,
	ITEMID_IronGuardCrystal								= 0x08020D00,
	ITEMID_RicartenMilitiaGuardCrystal					= 0x08020E00,
	ITEMID_RicartenGuardCrystal							= 0x08020F00,
	ITEMID_RoyalBlessGuardCrystal						= 0x08021000,
	ITEMID_OccultCrystal								= 0x08021100,
	ITEMID_DoomedCrystal								= 0x08021200,
	ITEMID_DuskyCrystal									= 0x08021300,
	ITEMID_MechanicCrystal								= 0x08021700, //Geek Event - GP120
	ITEMID_AbysmalCrystal								= 0x08021800, //GP124
	ITEMID_FieryCrystal									= 0x08021801, //GP126
	ITEMID_ArcticCrystal								= 0x08021900,
	ITEMID_MechanicKingCrystal							= 0x08022000, //Geek event - GP121

	ITEMID_MatureStoneA									= 0x080B3400,
	ITEMID_MatureStoneB									= 0x080B3500,
	ITEMID_MatureStoneC									= 0x080B3600,

	ITEMID_BlueStone									= 0x080B0100,
	ITEMID_RedStone										= 0x080B0200,
	ITEMID_GreenStone									= 0x080B0300,

	ITEMID_SapphireStone								= 0x080B3900,
	ITEMID_RubyStone									= 0x080B3700,

	//Cores
	ITEMID_RicartenCore									= 0x06010100,
	ITEMID_NaviskoCore									= 0x06010200,
	ITEMID_AtlantisCore									= 0x06010300,
	ITEMID_PillaiCore									= 0x06010400,
	ITEMID_UnionCore									= 0x06010500,

	ITEMID_Athanasia									= 0x080A0100, //BC101
	ITEMID_DeadlyEdge									= 0x080A0200, //BC102
	ITEMID_AvailOfEvade									= 0x080A0300, //BC103
	ITEMID_BolsterRecovery								= 0x080A0400, //BC104
	ITEMID_Restoration									= 0x080A0500, //BC105
	ITEMID_DefianceStoneFire							= 0x080A0600, //BC106
	ITEMID_DefianceStoneIce								= 0x080A0700, //BC107
	ITEMID_DefianceStoneLight							= 0x080A0800, //BC108
	ITEMID_MightPotionFighter							= 0x080A0900, //BC109
	ITEMID_MightPotionMechanician						= 0x080A0A00, //BC110
	ITEMID_MightPotionPikeman							= 0x080A0B00, //BC111
	ITEMID_MightPotionArcher							= 0x080A0C00, //BC112
	ITEMID_MightPotionKnight							= 0x080A0D00, //BC113
	ITEMID_MightPotionAtalanta							= 0x080A0E00, //BC114
	ITEMID_MightPotionMagician							= 0x080A0F00, //BC115
	ITEMID_MightPotionPriestess							= 0x080A1000, //BC116

	ITEMID_FatalEdge									= 0x080B0600, //BI106
	ITEMID_AvertScroll									= 0x080B0700, //BI107

	//Quest Items
	ITEMID_Roitra										= 0x07010700, //QT107
	ITEMID_TearsOfKalia									= 0x07010800, //QT108

	ITEMID_MagicRune									= 0x07011300, //QT119 (Frozen Quest)
	ITEMID_BlessOfKalia									= 0x07011900, //QT125 (Frozen Quest)
	ITEMID_GoldBag										= 0x07011700, //QT123
	ITEMID_ValentoEye									= 0x07013200, //QT134 (Tier5)
	ITEMID_PrinceEye									= 0x07013300, //QT135 (Tier5)
	ITEMID_DiamondTears									= 0x07013400, //QT136 (Tier5)
	ITEMID_GreenLiquidTree								= 0x07013500, //QT137 (Green Liquid Frozen Sanctuary Tree)

	//EXP Items
	ITEMID_RebirthScroll								= 0x080B0400, //BI104

	ITEMID_ExpUp1H										= 0x080B0F00,
	ITEMID_ExpUp24H										= 0x080B0D00,
	ITEMID_ExpUp48H										= 0x080B1800,
	ITEMID_ExpUp72H										= 0x080B1900,

	ITEMID_ThirdEye1H									= 0x080F1100,
	ITEMID_ThirdEye24H									= 0x080B0C00,
	ITEMID_ThirdEye48H									= 0x080B0E00,
	ITEMID_ThirdEye72H									= 0x080B5E00,

	ITEMID_SixthSense1H									= 0x080F1101,

	// SERVER WIDE BUFFS
	ITEMID_SHPRegen										= 0x080F1102,
	ITEMID_SSPRegen										= 0x080F1103,
	ITEMID_SMPRegen										= 0x080F1104,
	ITEMID_SMPDown										= 0x080F1105,
	ITEMID_SSPDown										= 0x080F1106,
	ITEMID_SBonusDamage									= 0x080F1107,
	ITEMID_SBonusAbsorb									= 0x080F1108,
	ITEMID_SMoveSpeed									= 0x080F1109,
	ITEMID_SExpBuff										= 0x080F1110,
	ITEMID_SDropBuff									= 0x080F1111,

	// Resistance Flasks
	ITEMID_IceResistance								= 0x080F1112,

	ITEMID_PhoenixEgg									= 0x080E0100,
	ITEMID_PhoenixFire									= 0x080B4800,
	ITEMID_PhoenixIce									= 0x080B4900,
	ITEMID_PhoenixLightning								= 0x080B4A00,
	ITEMID_PhoenixHealing								= 0x080B4B00,

	ITEMID_PhoenixFireSmall								= 0x080B4400,
	ITEMID_PhoenixIceSmall								= 0x080B4500,
	ITEMID_PhoenixLightningSmall						= 0x080B4600,
	ITEMID_PhoenixHealingSmall							= 0x080B4700,

	ITEMID_HPUp12H										= 0x080B1300,
	ITEMID_MPUp12H										= 0x080B1500,
	

	ITEMID_QuestVamp									= 0x07010600,
	ITEMID_QuestHoneyQuest								= 0x09020100,
	ITEMID_QuestRecommendationLetter					= 0x07010C00,
	ITEMID_QuestEmptyBottle								= 0x09010100,
	ITEMID_QuestRoyalJelly								= 0x07010400,

	ITEMID_QuestWolverineClaw							= 0x07010100,
	ITEMID_QuestWolverineTail							= 0x07010200,
	ITEMID_QuestWolverineHorn							= 0x07010300,


	ITEMID_QuestWeapon_DemonHunter						= 0x01010800, //WA108 (Quest) - Axe
	ITEMID_QuestWeapon_AncientRevenge					= 0x01020800, //WC108 (Quest) - Claw
	ITEMID_QuestWeapon_DarkSkull						= 0x01040900, //WM109 (Quest) - Wand
	ITEMID_QuestWeapon_DevilScythe						= 0x01050900, //WP109 (Quest) - Scythe
	ITEMID_QuestWeapon_Mist								= 0x01060A00, //WS110 (Quest) - Bow
	ITEMID_QuestWeapon_PlatinumSword					= 0x01070A00, //WS210 (Quest) - Sword
	ITEMID_QuestWeapon_Anaconda							= 0x01080900, //WT109 (Quest) - Javelin
	ITEMID_QuestWeapon_PlatinumDagger					= 0x010A0900, //WD109 (Quest) - Dagger
	ITEMID_QuestWeapon_HutanPhantom						= 0x01090900, //WN109 (Quest) - Plantom

	ITEMID_Wing_Metal									= 0x08030100,
	ITEMID_Wing_Silver									= 0x08030200,
	ITEMID_Wing_Gold									= 0x08030300,
	ITEMID_Wing_Diamond									= 0x08030400,
	ITEMID_Wing_Chaos									= 0x08030500,
	ITEMID_Wing_Imperial								= 0x08030600,

	ITEMID_HONEYDAMAGE									= 0x080B9000,
	ITEMID_HONEYFAST									= 0x080B9100,
	ITEMID_HONEYSMART									= 0x080B9200,
	ITEMID_HONEYDEFENSE									= 0x080B9300,

	ITEMID_MPDown12H									= 0x080B1C00,
	ITEMID_SPDown12H									= 0x080B1D00,

	ITEMID_GravityStone72H								= 0x080B6400,

	ITEMID_TeleportCore									= 0x080B0800,

	ITEMID_AgingStone									= 0x080B0A00,
	ITEMID_CopperOre									= 0x080B0B00,

	ITEMID_BigHeadHappyness								= 0x080B6F00,
	ITEMID_BigHeadLove									= 0x080B7000,
	ITEMID_BigHeadSadness								= 0x080B7100,
	ITEMID_BigHeadShyness								= 0x080B7200,
	ITEMID_BigHeadValentine								= 0x080B7300,
	ITEMID_BigHeadAngry									= 0x08110900,
	ITEMID_BigHeadSurprised								= 0x08110A00,
	ITEMID_BigHeadSensual								= 0x08110B00,

	ITEMID_HairPotionA									= 0x080B1000,
	ITEMID_HairPotionB									= 0x080B1100,
	ITEMID_HairPotionC									= 0x080B1200,
	ITEMID_HairPotionD									= 0x080C0D00,
	ITEMID_HairPotionE									= 0x080C0E00,
	ITEMID_HairPotionF									= 0x080C0F00,
	ITEMID_HairPotionG									= 0x080C1000,
	ITEMID_HairPotionH									= 0x080C1100,
	ITEMID_HairPotionI									= 0x080C1200,
	ITEMID_HairPotionJ									= 0x080C1300,
	ITEMID_HairPotionK									= 0x080C1400,
	ITEMID_HairPotionL									= 0x080C1500,
	ITEMID_HairPotionM									= 0x080C1600,
	ITEMID_HairPotionN									= 0x08110300,
	ITEMID_HairPotionO									= 0x08110400,
	ITEMID_HairPotionP									= 0x08110500,
	ITEMID_HairPotionQ									= 0x08110600,
	ITEMID_HairPotionR									= 0x08110700,
	ITEMID_HairPotionS									= 0x08110C00,
	ITEMID_HairPotionT									= 0x08110D00,

	ITEMID_SheepHat										= 0x080C3800,
//	ITEMID_EasterSheepHat								= 0x080B6700,
	ITEMID_GiraffeHat									= 0x080C3900,
	ITEMID_SoccerHat									= 0x080B6410,
	ITEMID_SoccerHatSpeed								= 0x080B6411,


	ITEMID_ChristmasGreenHat							= 0x080B6415,
	ITEMID_ChristmasRedHat								= 0x080B6420,
	ITEMID_BCBuff										= 0x080B6416,

	ITEMID_RomanticSuit									= 0x02111200,
	ITEMID_RomanticDress								= 0x02111300,

	ITEMID_CartolaHat									= 0x080B6500,
	ITEMID_WitchHat										= 0x080B6600,

	ITEMID_TurquoiseStone								= 0x080B5000,
	ITEMID_TopazStone									= 0x080B4F00,

	ITEMID_EmeraldStone									= 0x080B3800,

	ITEMID_VampireSuit									= 0x02100900,
	ITEMID_Succubus										= 0x02100B00,
	ITEMID_SkeletonSuit									= 0x02100C00, 	//ca646
	ITEMID_PumpkinSuit									= 0x02110100,	//ca647
	ITEMID_VampireSuit_NoTime							= 0x02110200,	//ca648
	ITEMID_Succubus_NoTime								= 0x02110300,	//ca649
	ITEMID_SkeletonDress								= 0x02110400, 	//ca650
	ITEMID_PumpkinDress									= 0x02110500, 	//ca651


	ITEMID_PermanentChristmasElegantCostumeM			= 0x02110600,
	ITEMID_PermanentChristmasElegantCostumeF			= 0x02110700,
	ITEMID_PermanentChristmasCloatCostumeTM				= 0x02110800,
	ITEMID_PermanentChristmasCloatCostumeTF				= 0x02110900,
	ITEMID_PermanentChristmasCloatCostumeMM				= 0x02111000,
	ITEMID_PermanentChristmasCloatCostumeMF				= 0x02111100,

	ITEMID_SummerTuxedo									= 0x02110400,
	ITEMID_SummerDress									= 0x02110500,

	ITEMID_Jera											= 0x03070100,
	ITEMID_Nied											= 0x03070200,
	ITEMID_Gyfu											= 0x03070300,

	ITEMID_ValentoRing									= 0x03050500,
	ITEMID_DevilShyRing									= 0x03050600,
	ITEMID_KelvezuAmulet								= 0x03012E00,
	ITEMID_TullaAmulet									= 0x03013C00,
	ITEMID_MokovaBoots									= 0x02022E00,
	ITEMID_AbyssRing									= 0x03054000, //OR210
	ITEMID_GuidanceAmulet								= 0x03011900, //OA125
	ITEMID_GuidanceAmuletPlus1							= 0x03011A00, //OA126
	ITEMID_DraxosBoots									= 0x02024200, //db150
	ITEMID_GreedyGauntlets								= 0x02034300, //dg130

	ITEMID_ManaMystic									= 0x04010400,
	ITEMID_HealthMystic									= 0x04020400,
	ITEMID_StaminaMystic								= 0x04030400,

	ITEMID_ManaGrand									= 0x04010500,	//67175680	pm105
	ITEMID_HealthGrand									= 0x04020500,	//67241216	ph105
	ITEMID_StaminaGrand									= 0x04030500,	//67306752	ps105

	//Halloween
	ITEMID_CoreHW										= 0x0A026001,
	ITEMID_RottenLantern								= 0x080B6001,
	ITEMID_EvilLantern									= 0x080B6002,
	ITEMID_InfernalLantern								= 0x080B6003,
	ITEMID_ToiletPaper									= 0x03106004,
	ITEMID_HopyToy										= 0x03106005,
	ITEMID_SpiderPlastic								= 0x03106006,
	ITEMID_GriffenEgg									= 0x03106007,

	//Sheltom
	ITEMID_Lucidy										= 0x02350100, //OS101
	ITEMID_Sereno										= 0x02350200, //OS102
	ITEMID_Fadeo										= 0x02350300, //OS103
	ITEMID_Sparky										= 0x02350400, //OS104
	ITEMID_Raident										= 0x02350500, //OS105
	ITEMID_Transparo									= 0x02350600, //OS106
	ITEMID_Murky										= 0x02350700, //OS107
	ITEMID_Devine										= 0x02350800, //OS108
	ITEMID_Celesto										= 0x02350900, //OS109
	ITEMID_Mirage										= 0x02350A00, //OS110
	ITEMID_Inferna										= 0x02350B00, //OS111
	ITEMID_Enigma										= 0x02350C00, //OS112
	ITEMID_Bellum										= 0x02350D00, //OS113
	ITEMID_Oredo										= 0x02350E00, //OS114
	ITEMID_FurySheltom									= 0x02351500, //OS121


	//ForceOrb
	ITEMID_LucidyForce									= 0x03060100,
	ITEMID_SerenoForce									= 0x03060200,
	ITEMID_FadeoForce									= 0x03060300,
	ITEMID_SparkyForce									= 0x03060400,
	ITEMID_RaidentForce									= 0x03060500,
	ITEMID_TransparoForce								= 0x03060600,
	ITEMID_MurkyForce									= 0x03060700,
	ITEMID_DevineForce									= 0x03060800,
	ITEMID_CelestoForce									= 0x03060900,
	ITEMID_MirageForce									= 0x03060A00,
	ITEMID_InfernaForce									= 0x03060B00,
	ITEMID_EnigmaForce									= 0x03060C00,
	ITEMID_BellumForce									= 0x03060D00,
	ITEMID_OredoForce									= 0x03060E00,

	//Cosmetic Forces
	ITEMID_RedForce										= 0x03061200,
	ITEMID_GreenForce									= 0x03061300,
	ITEMID_WhiteForce									= 0x03061400,
	ITEMID_PurpleForce									= 0x03061500,
	ITEMID_YellowForce									= 0x03061600,

	//Socket System
	ITEMID_RuneDrill									= 0x080C4F00, //BI263
	ITEMID_RedRuneDrill									= 0x080C5000, //BI264
	ITEMID_YellowRuneDrill								= 0x080C5100, //BI265
	ITEMID_GreenRuneDrill								= 0x080C5200, //BI266
	ITEMID_BlueRuneDrill								= 0x080C5300, //BI267
	ITEMID_PurpleRuneDrill1								= 0x080C5400, //BI268
	ITEMID_PurpleRuneDrill2								= 0x080C5500, //BI269
	ITEMID_PurpleRuneDrill3								= 0x080C5600, //BI270
	ITEMID_TitaniumDrill								= 0x080C4000, //BI248
	ITEMID_DiamondDrill									= 0x080C4100, //BI249
	ITEMID_IronDrill									= 0x080C4200, //BI250

	ITEMID_BronzeChest 								    = 0x080D0100, //sp239 - Bronze Chest  (100K)
	ITEMID_SilverChest 								    = 0x080D0200, //sp240 - Silver Chest  (200K)
	ITEMID_GreenChest								    = 0x080D0300, //sp241 - Green Chest	  (500K)
	ITEMID_BlueChest								    = 0x080D0400, //sp242 - Blue Chest	  (1KK)
	ITEMID_PurpleChest 								    = 0x080D0500, //sp243 - Purple Chest  (2KK)
	ITEMID_GoldChest								    = 0x080D0600, //sp244 - Gold Chest	  (5KK)
	ITEMID_GoldenChest 								    = 0x080D0700, //sp245 - Golden Chest  (10KK)
	ITEMID_RainbowChest								    = 0x080D0800, //sp246 - Rainbow Chest (20KK)
	ITEMID_GoldVaultChest								= 0x080D0900, //sp249 - Gold Vault Chest (200kk)

	ITEMID_RedTriangularGem								= 0x03130100, //PR501
	ITEMID_RedSquareGem									= 0x03130200, //PR502
	ITEMID_RedMarquiseGem								= 0x03130300, //PR503
	ITEMID_YellowTriangularGem							= 0x03130400, //PR504
	ITEMID_YellowSquareGem								= 0x03130500, //PR505
	ITEMID_YellowMarquiseGem							= 0x03130600, //PR506
	ITEMID_GreenTriangularGem							= 0x03130700, //PR507
	ITEMID_GreenSquareGem								= 0x03130800, //PR508
	ITEMID_GreenMarquiseGem								= 0x03130900, //PR509
	ITEMID_BlueTriangularGem							= 0x03130A00, //PR510
	ITEMID_BlueSquareGem								= 0x03130B00, //PR511
	ITEMID_BlueMarquiseGem								= 0x03130C00, //PR512
	ITEMID_PurpleTriangularGem							= 0x03130D00, //PR513
	ITEMID_PurpleSquareGem								= 0x03130E00, //PR514
	ITEMID_PurpleMarquiseGem							= 0x03130F00, //PR515

	ITEMID_GreenGemPiece								= 0x03131500, //PR521
	ITEMID_RedGemPiece									= 0x03131600, //PR522
	ITEMID_BlueGemPiece									= 0x03131700, //PR523
	ITEMID_PurpleGemPiece								= 0x03131800, //PR524
	ITEMID_YellowGemPiece								= 0x03131900, //PR525

	ITEMID_RedElaboratedGem								= 0x03131000, //PR516
	ITEMID_YellowElaboratedGem							= 0x03131100, //PR517
	ITEMID_GreenElaboratedGem							= 0x03131200, //PR518
	ITEMID_BlueElaboratedGem							= 0x03131300, //PR519
	ITEMID_PurpleElaboratedGem							= 0x03131400, //PR520

	ITEMID_PurpleDiamond								= 0x03090100,
	ITEMID_SilverDiamond								= 0x03090200,
	ITEMID_GoldDiamond									= 0x03090300,

	//Respec
	ITEMID_SpecStoneCode								= 0x03076000,
	ITEMID_SpecStoneFS									= 0x03076001,
	ITEMID_SpecStoneMS									= 0x03076002,
	ITEMID_SpecStoneAS									= 0x03076003,
	ITEMID_SpecStonePS									= 0x03076004,
	ITEMID_SpecStoneATA									= 0x03076005,
	ITEMID_SpecStoneKS									= 0x03076006,
	ITEMID_SpecStoneMG									= 0x03076007,
	ITEMID_SpecStonePRS									= 0x03076008,
	ITEMID_SpecStoneASS									= 0x03076009,
	ITEMID_SpecStoneSHA									= 0x0307600A,
	ITEMID_DeluxeSpecStoneFS							= 0x0307600B,
	ITEMID_DeluxeSpecStoneMS							= 0x0307600C,
	ITEMID_DeluxeSpecStoneAS							= 0x0307600D,
	ITEMID_DeluxeSpecStonePS							= 0x0307600E,
	ITEMID_DeluxeSpecStoneATA							= 0x0307600F,
	ITEMID_DeluxeSpecStoneKS							= 0x03076010,
	ITEMID_DeluxeSpecStoneMG							= 0x03076011,
	ITEMID_DeluxeSpecStonePRS							= 0x03076012,
	ITEMID_DeluxeSpecStoneASS							= 0x03076013,
	ITEMID_DeluxeSpecStoneSHA							= 0x03076014,

	ITEMID_CleanItem									= 0x080B6900,

	ITEMID_Fireworks									= 0x080B1700,

	//Costume old ids (to be deleted)
	ITEMID_SilverCostumeMale_OLD                        = 0x02013700, //DA139
	ITEMID_SilverCostumeFemale_OLD                      = 0x02013900, //DA141
	ITEMID_LunarCostume_OLD                             = 0x02014000, //DA148
	ITEMID_GaiaArmorMale_OLD                            = 0x02014100, //DA149
	ITEMID_GaiaArmorFemale_OLD                          = 0x02014200, //DA150
	ITEMID_DarkGaiaArmorMale_OLD                        = 0x02014300, //DA151
	ITEMID_DarkGaiaArmorFemale_OLD                      = 0x02014400, //DA152
	ITEMID_DarkSeasonArmorFemale_OLD                    = 0x02014500, //DA153
	ITEMID_DarkSeasonArmorMale_OLD                      = 0x02014600, //DA154
	ITEMID_WeddingSuit_OLD                              = 0x02014800, //DA156
	ITEMID_WeddingDress_OLD                             = 0x02014900, //DA157
	ITEMID_WarriorsHanbokM_OLD                          = 0x02014A00, //DA158
	ITEMID_WarriorsHanbokF_OLD                          = 0x02014B00, //DA159
	ITEMID_KimonoCostumeM_OLD                           = 0x02014C00, //DA160
	ITEMID_KimonoCostumeF_OLD                           = 0x02014D00, //DA161
	ITEMID_ChristmasCloatCostumeTM_OLD                  = 0x02014E00, //DA162
	ITEMID_ChristmasCloatCostumeTF_OLD                  = 0x02014F00, //DA163
	ITEMID_ChineseSuitM_OLD                             = 0x02015000, //DA164
	ITEMID_ChineseSuitF_OLD                             = 0x02015100, //DA165

	ITEMID_SoccerShirt_OLD								= 0x02053F00, //Da247
	ITEMID_IriaRobeMale_OLD								= 0x02054100, //DA249
	ITEMID_IriaRobeFemale_OLD							= 0x02054200, //DA250
	ITEMID_DarkIriaRobeMale_OLD							= 0x02054300, //DA251
	ITEMID_DarkIriaRobeFemale_OLD						= 0x02054400, //DA252

	//Suits
	ITEMID_SouthGraceHanbokM							= 0x02100100,
	ITEMID_SouthGraceHanbokF							= 0x02100200,
	ITEMID_MagnificentHanbokM							= 0x02100300,
	ITEMID_MagnificentHanbokF							= 0x02100500,
	ITEMID_GraySuitM									= 0x02100700,
	ITEMID_GraySuitF									= 0x02100800,
	ITEMID_RomanticSuitM								= 0x02101100,
	ITEMID_RomanticSuitF								= 0x02101200,
	ITEMID_MechanicCostumeM								= 0x02101500,
	ITEMID_MechanicTwitchCostumeM						= 0x02101600, // New Costume, Decimel = IDCode in Itemlist.dbo (CodeIMG=ca522 (Could be anything)
	ITEMID_MechanicCostumeF								= 0x02101700,
	ITEMID_MechanicTwitchCostumeF						= 0x02101800, // New Costume Female
	ITEMID_ChineseSuitM									= 0x02068001, //DA164
	ITEMID_ChineseSuitF									= 0x02068002, //DA165
	ITEMID_WarriorsHanbokM								= 0x02068003, //DA158
	ITEMID_WarriorsHanbokF								= 0x02068004, //DA159
	ITEMID_KimonoCostumeM								= 0x02068005, //DA160
	ITEMID_KimonoCostumeF								= 0x02068006, //DA161
	ITEMID_ChristmasCloatCostumeTM						= 0x02068007, //DA162
	ITEMID_ChristmasCloatCostumeTF						= 0x02068008, //DA163
	ITEMID_LunarCostume						            = 0x02068009, //DA148
	ITEMID_GaiaArmorMale								= 0x0206800A, //DA149
	ITEMID_GaiaArmorFemale								= 0x0206800B, //DA150
	ITEMID_SilverCostumeMale							= 0x0206800C, //DA139
	ITEMID_SilverCostumeFemale							= 0x0206800D, //DA141
	ITEMID_DarkGaiaArmorMale							= 0x0206800E, //DA151
	ITEMID_DarkGaiaArmorFemale							= 0x0206800F, //DA152
	ITEMID_DarkSeasonArmorFemale						= 0x02068010, //DA153
	ITEMID_DarkSeasonArmorMale							= 0x02068011, //DA154
	ITEMID_WeddingSuit									= 0x02068012, //DA156
	ITEMID_WeddingDress									= 0x02068013, //DA157
	ITEMID_SoccerShirt									= 0x02068014, //DA247
	ITEMID_IriaRobeMale									= 0x02068015, //DA249
	ITEMID_IriaRobeFemale								= 0x02068016, //DA250
	ITEMID_DarkIriaRobeMale								= 0x02068017, //DA251
	ITEMID_DarkIriaRobeFemale							= 0x02068018, //DA252


	ITEMID_FootballM									= 0x02101900,
	ITEMID_FootballF									= 0x02101B00,
	ITEMID_PeruvianSoccer								= 0x02111900,
	ITEMID_BrazilianSoccer								= 0x02111A00,
	ITEMID_ArgentineSoccer								= 0x02111B00,
	ITEMID_ChileanSoccer								= 0x02111C00,
	ITEMID_PinoySoccer									= 0x02111D00,
	ITEMID_VietnameseSoccer								= 0x02111E00,
	ITEMID_FrenchSoccer									= 0x02111F00,
	ITEMID_GermanySoccer								= 0x02112000,
	ITEMID_HollandSoccer								= 0x02112100,
	ITEMID_ItalianSoccer								= 0x02112200,
	ITEMID_MexicanSoccer								= 0x02112300,
	ITEMID_PortugueseSoccer								= 0x02112400,
	ITEMID_SpanishSoccer								= 0x02112500,
	ITEMID_TeamUSASoccer								= 0x02112600,
	ITEMID_ColombianSoccer								= 0x02112700,
	ITEMID_JapanSoccer									= 0x02112800,

	//Soccers timed Champions League Event
	ITEMID_ChelseaSoccer								= 0x02103100, //CA549 - 34615552
	ITEMID_ChelseaSoccerTimed							= 0x02103101, //CA549 - 34615553

	ITEMID_ManchesterSoccer								= 0x02103200, //CA550 - 34615808
	ITEMID_ManchesterSoccerTimed						= 0x02103201, //CA550 - 34615809

	//Armors
	ITEMID_ViperArmor									= 0x02011600,
	ITEMID_KronusArmor									= 0x02012B00,
	ITEMID_DestroyerArmor								= 0x02011A00,

	//New 100 set, Phantom
	ITEMID_PhantomArmor									= 0x02011C00,

	//Robes
	ITEMID_PhoenixRobe									= 0x02051600,
	ITEMID_KronusRobe									= 0x02052B00,

	ITEMID_Gold											= 0x05010100,
	ITEMID_Experience									= 0x05010200,

	ITEMID_PotionAgingQuestBee							= 0x09010100, //MA101

	//Christmas
	ITEMID_RecipeChristmas01							= 0x0A026101,
	ITEMID_RecipeEaster01								= 0x0A026103,
	ITEMID_RecipeChristmas02							= 0x0A026102,
	ITEMID_SnowManCrystal								= 0x08022900,
	ITEMID_Cotton										= 0x0310602E,
	ITEMID_RedButton									= 0x0310602F,
	ITEMID_GreenCloth									= 0x03106031,

	ITEMID_ChristmasElegantCostumeM						= 0x02100F00,
	ITEMID_ChristmasElegantCostumeF						= 0x02101000,
	ITEMID_ChristmasCloatCostumeMM						= 0x02054E00,
	ITEMID_ChristmasCloatCostumeMF						= 0x02054F00,

	ITEMID_Gingerbread									= 0x080E4200,
	ITEMID_GiftBox1										= 0x08010700, //SP107
	ITEMID_GiftBox2										= 0x08011100, //SP117
	ITEMID_SnowyChristmasTree							= 0x080B6100, //BI181
	ITEMID_ColorfulChristmasTree						= 0x080B6200, //BI182

	ITEMID_PetSystem_1H									= 0x080B7400, //to define
	ITEMID_PetSystem_24H								= 0x080B7500, //to define
	ITEMID_PetSystem_48H								= 0x080B7600, //to define	
	ITEMID_PetSystem_72H								= 0x080B7800, //to define

	//Easter
	ITEMID_RedEasterEgg									= 0x03106032, //SP254
	ITEMID_GreenEasterEgg								= 0x03106033, //SP255
	ITEMID_PinkEasterEgg								= 0x03106034, //SP256
	/*
	ITEMID_RedEasterEgg									= 0x080E4600,
	ITEMID_GreenEasterEgg								= 0x080E4700,
	ITEMID_PinkEasterEgg								= 0x080E4800,
	*/
	ITEMID_EasterHopyCostume							= 0x02112F00, //ca641
	ITEMID_EasterCuepyCostume							= 0x02113000, //ca642

	//Bellatra
	ITEMID_BrilliantStone								= 0x08017100,

	ITEMID_GrumpyArmor									= 0x0201EE01,
	ITEMID_GrumpyRobe									= 0x0205EE01,

	ITEMID_Season1CostumeArmor							= 0x0206EE01, //CA171
	ITEMID_Season2CostumeArmor							= 0x0206EE02, //CA173
	ITEMID_Season3CostumeArmor							= 0x0206EE03, //CA174
	ITEMID_Season4CostumeArmor							= 0x0206EE04, //CA175
	ITEMID_Season1CostumeRobe							= 0x0207EE01, //CA172 - Unused for now since ITEMID_Season1CostumeArmor is universal

	//TGF
	ITEMID_KingStone									= 0x07013100,

	//Runes
	ITEMID_LightBlueRune								= 0x03100400, // - doesn't exist in db
	ITEMID_DarkRedRune									= 0x03100700, //PR307 - doesn't exist in db
	ITEMID_LightGreenRune								= 0x03100800, //PR308 - doesn't exist in db


	//Beginner
	ITEMID_BeginnerAxe									= 0x01012E00, //WA130
	ITEMID_BeginnerClaw									= 0x01022D00, //WC129
	ITEMID_BeginnerHammer								= 0x01032F00, //WH131
	ITEMID_BeginnerSword								= 0x01073100, //WS233
	ITEMID_BeginnerStaff								= 0x01042F00, //WM131
	ITEMID_BeginnerJavelin								= 0x01082E00, //WT130
	ITEMID_BeginnerBow									= 0x01062F00, //WS131
	ITEMID_BeginnerScythe								= 0x01052E00, //WP130
	ITEMID_BeginnerPhantom								= 0x01092D00, //WN129
	ITEMID_BeginnerDagger								= 0x010A2D00, //WD129
	ITEMID_BeginnerGauntlet								= 0x02031B00, //DG127
	ITEMID_BeginnerBoots								= 0x02021B00, //DB127
	ITEMID_BeginnerBracelet								= 0x03021B00, //OA227

	//Wyvern 92 Subgear
	ITEMID_WyvernGauntlet								= 0x02031D00, //DG304
	ITEMID_WyvernBoots									= 0x02021D00, //DB304
	ITEMID_WyvernBracelet								= 0x03021D00, //OA304

	//Royal 95 Subgear
	//ITEMID_RoyalGauntlet								= 0x02031E00, //DG303
	//ITEMID_RoyalBoots									= 0x02021E00, //DB303
	//ITEMID_RoyalBracelet								= 0x03021E00, //OA303
	//
	////Phantom 98 Subgear
	//ITEMID_PhantomGauntlet								= 0x02031F00, //DG305
	//ITEMID_PhantomBoots									= 0x02021F00, //DB305
	//ITEMID_PhantomBracelet								= 0x03021F00, //OA305

	//Costumes
	ITEMID_PartyCostumeMale								= 0x02062F00, //CA131
	ITEMID_PartyCostumeFemale							= 0x02063100, //CA133
	ITEMID_IdasCostumeMale								= 0x02063300, //CA135
	ITEMID_IdasCostumeFemale							= 0x02063500, //CA137
	ITEMID_ThalesArmorMale								= 0x02063B00, //CA143
	ITEMID_ThalesArmorFemale							= 0x02063D00, //CA145
	ITEMID_Costume4ArmorMale							= 0x02064600, //CA154
	ITEMID_Costume4ArmorFemale							= 0x02064700, //CA155
	ITEMID_Costume5ArmorMale							= 0x02065200, //CA166
	ITEMID_Costume5ArmorFemale							= 0x02065400, //CA168
	//ITEMID_UNUSEDPLACEHOLDER							= 0x02065500, //Interesting fact is that using these hex values will attach wings to a costume.
	ITEMID_WingArmorMale								= 0x02066500, //CA185
	ITEMID_WingArmorFemale								= 0x02066700, //CA187
	ITEMID_DarkWingArmorMale							= 0x02066900, //CA189
	ITEMID_DarkWingArmorFemale							= 0x02066B00, //CA191
	ITEMID_SummerClotheMale								= 0x02066D00, //CA193
	ITEMID_SummerClotheFemale							= 0x02066F00, //CA195

	//Timed event costume
	ITEMID_SummerClotheTimedMale						= 0x02067A00, //CA644
	ITEMID_SummerClotheTimedFemale						= 0x02067B00, //CA645

	ITEMID_SpringCostumeMale							= 0x02100100, //CA501
	ITEMID_SpringCostumeFemale							= 0x02100200, //CA502
	ITEMID_YellowSpringCostumeMale						= 0x02100300, //CA503
	ITEMID_YellowSpringCostumeFemale					= 0x02100500, //CA505
	ITEMID_Costume11ArmorMale							= 0x02100700, //CA507
	ITEMID_Costume11ArmorFemale							= 0x02100800, //CA508
	ITEMID_BlackSuit									= 0x02101100, //CA517
	ITEMID_WhiteDress									= 0x02101300, //CA519
	ITEMID_Costume13ArmorMale							= 0x02101500, //CA521
	ITEMID_Costume13ArmorFemale							= 0x02101700, //CA523
	ITEMID_CasualSchoolUniformMale						= 0x02101F00, //CA531
	ITEMID_CasualSchoolUniformFemale					= 0x02102100, //CA533
	ITEMID_ElegantSchoolUniformMale						= 0x0207A900, //CA353
	ITEMID_ElegantSchoolUniformFemale					= 0x02102500, //CA537
	ITEMID_Costume16ArmorMale							= 0x02102700, //CA539
	ITEMID_Costume16ArmorFemale							= 0x02102900, //CA541
	ITEMID_MarineCostumeMale							= 0x02102B00, //CA543
	ITEMID_MarineCostumeFemale							= 0x02102D00, //CA545
	ITEMID_HopyCostume									= 0x02102F00, //CA547
	ITEMID_CuepyCostume									= 0x02103000, //CA548
	ITEMID_MartanosRobeMale								= 0x02073300, //CA235
	ITEMID_MartanosRobeFemale							= 0x02073500, //CA237
	ITEMID_MikalRobeMale								= 0x02073B00, //CA243
	ITEMID_MikalRobeFemale								= 0x02073D00, //CA245

	ITEMID_DarkSeasonRobeMale							= 0x02054500, //DA253
	ITEMID_DarkSeasonRobeFemale							= 0x02054600, //DA254

	ITEMID_WeddingSuit_Black_Gold						= 0x02106100, //DA166
	ITEMID_WeddingDress_Black_Gold						= 0x02106200, //DA167



	//Axes
	ITEMID_MythologyAxe									= 0x01012B00, //WA127
	ITEMID_DeathAxe										= 0x01012C00, //WA128
	ITEMID_AbyssAxe										= 0x01012D00, //WA129

	ITEMID_MeatCleaver									= 0x01012D01, //WA501

	//Claws
	ITEMID_MythologyTalon								= 0x01022A00, //WC126
	ITEMID_DeathTalon									= 0x01022B00, //WC127
	ITEMID_AbyssTalon									= 0x01022C00, //WC128

	//Hammers
	ITEMID_MythologyHammer								= 0x01032C00, //WH128
	ITEMID_DeathHammer									= 0x01032D00, //WH129
	ITEMID_AbyssHammer									= 0x01032E00, //WH130

	//Staffs
	ITEMID_MythologyStaff								= 0x01042C00, //WM128
	ITEMID_DeathStaff									= 0x01042D00, //WM129
	ITEMID_AbyssStaff									= 0x01042E00, //WM130

	//Scythes
	ITEMID_MythologySpear								= 0x01052B00, //WP127
	ITEMID_DeathSpear									= 0x01052C00, //WP128
	ITEMID_AbyssSpear									= 0x01052D00, //WP129

	//Bows
	ITEMID_MythologyBow									= 0x01062C00, //WS128
	ITEMID_DeathBow										= 0x01062D00, //WS129
	//Flame Bow ws130 - Unused
	//Verus Bow ws131 - Unused
	ITEMID_AbyssBow										= 0x01062E00, //WS132
	ITEMID_InfernalBow									= 0x01063A00, //WS133
	ITEMID_LunaticBow									= 0x01063B00, //WS134
	ITEMID_AmentiaBow									= 0x01063C00, //WS135

	//Swords
	ITEMID_MythologySword								= 0x01072E00, //WS230
	ITEMID_DeathSword									= 0x01072F00, //WS231
	ITEMID_AbyssSword									= 0x01072D00, //WS232
	ITEMID_LunaticSword									= 0x01073000, //WS236
	ITEMID_AmentiaSword									= 0x01073E00, //WS237

	ITEMID_StarWarsShirt								= 0x0211A100, //DA444
	ITEMID_StarWarsStaff								= 0x0104A100, //WM430
	ITEMID_StarWarsStaff90								= 0x0104A200, //WM432
	ITEMID_StarWarsStaff100								= 0x0104A300, //WM433
	ITEMID_StarWarsStaff110								= 0x0104A400, //WM434
	ITEMID_StarWarsScythe								= 0x0105A100, //WP435
	ITEMID_StarWarsScythe90								= 0x0105A200, //WP436
	ITEMID_StarWarsScythe100							= 0x0105A300, //WP437
	ITEMID_StarWarsScythe110							= 0x0105A400, //WP438
	ITEMID_StarWarsBow									= 0x0106A100, //WS427
	ITEMID_StarWarsBow90								= 0x0106A200, //WS433
	ITEMID_StarWarsBow100								= 0x0106A300, //WS434
	ITEMID_StarWarsBow110								= 0x0106A400, //WS435
	ITEMID_StarWarsLightSaber1H							= 0x0107A100, //WS433
	ITEMID_StarWarsLightSaber1H90						= 0x0107A200, //WS437
	ITEMID_StarWarsLightSaber1H100						= 0x0107A300, //WS438
	ITEMID_StarWarsLightSaber1H110						= 0x0107A400, //WS439
	ITEMID_StarWarsLightSaber2H							= 0x0107AA00, //WS432
	ITEMID_StarWarsLightSaber2H90						= 0x0107AB00, //WS440
	ITEMID_StarWarsLightSaber2H100						= 0x0107AC00, //WS441
	ITEMID_StarWarsLightSaber2H110						= 0x0107AD00, //WS442
	ITEMID_StarWarsPhantom								= 0x0109A100, //WN435
	ITEMID_StarWarsPhantom90							= 0x0109A200, //WN436
	ITEMID_StarWarsPhantom100							= 0x0109A300, //WN437
	ITEMID_StarWarsPhantom110							= 0x0109A400, //WN438
	ITEMID_StarWarsDagger								= 0x010AA100, //WD428
	ITEMID_StarWarsDagger90								= 0x010AA200, //WD430
	ITEMID_StarWarsDagger100							= 0x010AA300, //WD431
	ITEMID_StarWarsDagger110							= 0x010AA400, //WD432
	ITEMID_StarWarsJavelin								= 0x0108A100, //WT432
	ITEMID_StarWarsJavelin90							= 0x0108A200, //WT433
	ITEMID_StarWarsJavelin100							= 0x0108A300, //WT434
	ITEMID_StarWarsJavelin110							= 0x0108A400, //WT435

	//Javelins
	ITEMID_MythologyJavelin								= 0x01082B00, //WT127
	ITEMID_DeathJavelin									= 0x01082C00, //WT128
	ITEMID_AbyssJavelin									= 0x01082D00, //WT129

	//Phantoms
	ITEMID_MythologyPhantom								= 0x01092A00, //WN126
	ITEMID_DeathPhantom									= 0x01092B00, //WN127
	ITEMID_AbyssPhantom									= 0x01092C00, //WN128
	ITEMID_LunaticPhantom								= 0x01092E00, //WN129
	ITEMID_AmentiaPhantom								= 0x01092F00, //WN130

	//Daggers
	ITEMID_MythologyDagger								= 0x010A2A00, //WD126
	ITEMID_DeathDagger									= 0x010A2B00, //WD127
	ITEMID_AbyssDagger									= 0x010A2C00, //WD128

	//Armors
	ITEMID_MythologyArmor								= 0x02012C00, //DA128
	ITEMID_DeathArmor									= 0x02012D00, //DA129
	ITEMID_AbyssArmor									= 0x02012E00, //DA130
	ITEMID_InfernalArmor								= 0x02012F00, //DA131
	ITEMID_LunaticArmor									= 0x02013A00, //DA132
	ITEMID_AmentiaArmor									= 0x02013A00, //DA132

	//Shields
	ITEMID_MythologyShield								= 0x02041900, //DS125
	ITEMID_DeathShield									= 0x02041A00, //DS126
	ITEMID_AbyssShield									= 0x02041B00, //DS128
	ITEMID_LunaticShield								= 0x02041C00, //DS130
	ITEMID_AmentiaShield								= 0x02041D00, //DS135

	//Orbs
	ITEMID_MythologyOrb									= 0x03031A00, //OM126
	ITEMID_DeathOrb										= 0x03031B00, //OM127
	ITEMID_AbyssOrb										= 0x03031C00, //OM128

	//Robes
	ITEMID_MythologyRobe								= 0x02052C00, //DA228
	ITEMID_DeathRobe									= 0x02052D00, //DA229
	ITEMID_AbyssRobe									= 0x02052E00, //DA230
	ITEMID_InfernalRobe									= 0x02052F00, //DA231

	//LUNATIC
	ITEMID_SandurrRedCostumeMale						= 0x02104100, //CA565
	ITEMID_SandurrRedCostumeFemale						= 0x02104200, //CA566

	//AMENTIA
	ITEMID_AmentiaMale									= 0x02104300, //CA565
	ITEMID_AmentiaFemale								= 0x02104400, //CA566

	//Boots
	ITEMID_DeathBoots									= 0x02021A00, //DB126
	ITEMID_AbyssBoots									= 0x02021C00, //DB127

	//Gauntlets
	ITEMID_DeathGauntlets								= 0x02031A00, //DG126
	ITEMID_AbyssGauntlets								= 0x02031C00, //DG127

	//Bracelets
	ITEMID_DeathBracelets								= 0x03021A00, //OA226
	ITEMID_AbyssBracelets								= 0x03021C00, //OA229
	ITEMID_IgnisBracelets								= 0x03021C01, //OA231

	//Toys
	ITEMID_LoveSword									= 0x01073600, //LI101
	ITEMID_Rose											= 0x01023200, //LI102
	ITEMID_PickAxe										= 0x01013300, //LI103
	ITEMID_Guitar										= 0x01053300, //LI104
	ITEMID_XmasCane										= 0x01013400, //LI105
	ITEMID_TigersClaw									= 0x01023201, //LI106
	ITEMID_PanFun										= 0x01033600, //LI107
	ITEMID_GoldenPanFun									= 0x01033601, //LI108
	ITEMID_GoldenTigersClaw								= 0x01023202, //LI109
};

enum EItemType : unsigned int
{
	ITEMTYPE_Any			= 0xFFFFFFFF,
	ITEMTYPE_None			= 0x00000000,

	ITEMTYPE_Recipe			= 0x0A020000, //RECIPE

	/* Weapons */
	ITEMTYPE_Axe			= 0x01010000, //WA
	ITEMTYPE_Claw			= 0x01020000, //WC
	ITEMTYPE_Hammer			= 0x01030000, //WH
	ITEMTYPE_Wand			= 0x01040000, //WM
	ITEMTYPE_Scythe			= 0x01050000, //WP
	ITEMTYPE_Bow			= 0x01060000, //WS1
	ITEMTYPE_Sword			= 0x01070000, //WS2
	ITEMTYPE_Javelin		= 0x01080000, //WT
	ITEMTYPE_Phantom		= 0x01090000, //WN
	ITEMTYPE_Dagger			= 0x010A0000, //WD

	/* Defenses */
	ITEMTYPE_Armor			= 0x02010000, //DA1
	ITEMTYPE_Costume		= 0x02100000, //CA
	ITEMTYPE_Costume2		= 0x02110000, //CA
	ITEMTYPE_Costume3		= 0x02060000, //CA
	ITEMTYPE_Costume4		= 0x02070000, //CA
	ITEMTYPE_Boots			= 0x02020000, //DB
	ITEMTYPE_Gauntlets		= 0x02030000, //DG
	ITEMTYPE_Shield			= 0x02040000, //DS
	ITEMTYPE_Robe			= 0x02050000, //DA2
	ITEMTYPE_Sheltom		= 0x02350000, //OS

	/* Accessories */
	ITEMTYPE_Amulet			= 0x03010000, //OA1
	ITEMTYPE_Bracelets		= 0x03020000, //OA2
	ITEMTYPE_Orb			= 0x03030000, //OM
	ITEMTYPE_Ring			= 0x03040000, //OR1
	ITEMTYPE_ForceOrb		= 0x03060000, //FO
	ITEMTYPE_Respec			= 0x03070000, //SE or JV
	ITEMTYPE_Smelting		= 0x03090000, //PR2
	ITEMTYPE_Crafting		= 0x03100000, //PR3
	ITEMTYPE_EventPR5		= 0x03130000, //PR5
	ITEMTYPE_Ring2			= 0x03050000, //OR2



	/* Potions */
	ITEMTYPE_ManaPotion		= 0x04010000, //PM sinPM1
	ITEMTYPE_HealthPotion	= 0x04020000, //PL sinPL1
	ITEMTYPE_StaminaPotion	= 0x04030000, //PS sinPS1

	/* Special */
	ITEMTYPE_GoldAndExp		= 0x05010000, //GG sinGG1
	ITEMTYPE_BellatoStone	= 0x05020000, //BS sinBS1

	/* Other */
	ITEMTYPE_EtherCore		= 0x06010000, //EC

	/* Quest */
	ITEMTYPE_Quest			= 0x07010000, //QT
	ITEMTYPE_MorionT2Quest	= 0x09020000,

	/* Event */
	ITEMTYPE_Cookie			= 0x08010000,
	ITEMTYPE_MonsterCrystal	= 0x08020000, //GP
	ITEMTYPE_Wing			= 0x08030000, //QW
	ITEMTYPE_Gift			= 0x08040000,
	ITEMTYPE_Puzzle1		= 0x08060000,
	ITEMTYPE_Puzzle2		= 0x08070000,
	ITEMTYPE_Chocolate		= 0x08080000,
	ITEMTYPE_Bellatra		= 0x08090000, //SD2
	ITEMTYPE_BlessCastle	= 0x080A0000, //BC
	ITEMTYPE_Premium1		= 0x080B0000, //BI1
	ITEMTYPE_Premium2		= 0x080C0000, //BI2
	ITEMTYPE_MoneyChest		= 0x080D0000,
	ITEMTYPE_Puzzle4		= 0x080E0000,
	ITEMTYPE_HairPotionCode2	= 0x08110000, //135331840

	/* Custom */
	ITEMTYPE_MixStone		= 0x08A00000, //RS
	ITEMTYPE_MatureStone	= 0x08B00000, //MS
	ITEMTYPE_Crystal		= 0x08C00000, //SC
	ITEMTYPE_Bars			= 0x08D00000, //GB
	ITEMTYPE_PetEggs		= 0x08E00000, //PI
	ITEMTYPE_Event			= 0x08F00000, //EV
};

enum EItemBase : unsigned int
{
	ITEMBASE_Any			= 0xFFFFFFFF,
	ITEMBASE_None			= 0x00000000,

	ITEMBASE_Weapon			= 0x01000000, //W
	ITEMBASE_Defense		= 0x02000000, //D
	ITEMBASE_Sheltom		= 0x03000000, //O
	ITEMBASE_Potion			= 0x04000000, //P
	ITEMBASE_Crystal		= 0x05000000, //G
	ITEMBASE_Core			= 0x06000000, //E
	ITEMBASE_Quest1			= 0x07000000, //Q
	ITEMBASE_Premium		= 0x08000000, //BI
	ITEMBASE_Quest2			= 0x09000000,
	ITEMBASE_Recipe			= 0x0A000000, //SD
};

enum EItemCraftType : int
{
	ITEMCRAFTTYPE_NPC				= -1,
	ITEMCRAFTTYPE_None				= 0,    // ITEM_KIND_NORMAL
	ITEMCRAFTTYPE_Mixing			= 1,	// ITEM_KIND_CRAFT
	ITEMCRAFTTYPE_Aging				= 2,	// ITEM_KIND_AGING
	ITEMCRAFTTYPE_Quest				= 3,	// ITEM_KIND_QUEST
	ITEMCRAFTTYPE_Event				= 4,	// ITEM_KIND_EVENT
	ITEMCRAFTTYPE_Bottle			= 5,	// ITEM_KIND_MAKE_MAIN
	ITEMCRAFTTYPE_BottleElement		= 6,	// ITEM_KIND_MAKE_ELEMENT
	ITEMCRAFTTYPE_QuestWeapon		= 7,	// ITEM_KIND_QUEST_WEAPON
	ITEMCRAFTTYPE_Special			= 8,	// ITEM_KIND_SPECIAL
	ITEMCRAFTTYPE_TGFKingFury		= 9,
	ITEMCRAFTTYPE_TGFLordBabel		= 10,
};


enum class EMixType : int
{
	MIXTYPE_Unknown			= 0,

	MIXTYPE_Weapons			= 1,
	MIXTYPE_ArmourAndRobe	= 2,
	MIXTYPE_Sheilds			= 3,
	MIXTYPE_Orbs			= 4,
	MIXTYPE_Bracelet		= 5,
	MIXTYPE_Gauntlets		= 6,
	MIXTYPE_Boots			= 7,
	MIXTYPE_Amulets			= 8,
	MIXTYPE_Rings			= 9,
	//MIXTYPE_DaggerPhantom	= 10
};

/**
MIX COLOR
See CraftBlinkColor[6][6] (leaked)
and see AgingColorStructure sMixingColor[10]
*/
enum class EMixColor : unsigned short
{
	MIXCOLOR_None		= 0,
	MIXCOLOR_Classic	= 5,
	MIXCOLOR_Transparo	= 6,
	MIXCOLOR_Murky		= 7,
	MIXCOLOR_Devine		= 8,
	MIXCOLOR_Celesto	= 9,
	MIXCOLOR_Mirage		= 10,
	MIXCOLOR_Inferna	= 11,
	MIXCOLOR_Enigma		= 12,
	MIXCOLOR_Bellum		= 13,
	MIXCOLOR_Oredo		= 14
};

/**
MIX ID
NOTE - EVERY MIX IN MIX TYPE NEED TO HAVE A UNIQUE
MIX NUMBER SO THAT MIXES CAN BE HANDLED CORRECTLY
*/
enum class EMixTypeName : unsigned int
{
	MIXTYPENAME_None = 0,		//0

	MIXTYPENAME_LegacyDoNotUse,		//1 (ignore this mix, because since launch all mix id was 1 (in the database), so all mix was 'Devine mix'

	MIXTYPENAME_Lucidy1 = 101,
	MIXTYPENAME_Sereneo1,
	MIXTYPENAME_Fadeo1,
	MIXTYPENAME_Sparky1,
	MIXTYPENAME_Raident1,
	MIXTYPENAME_Transparo1,
	MIXTYPENAME_Murky1,
	MIXTYPENAME_Devine1,
	MIXTYPENAME_Celesto1,
	MIXTYPENAME_Mirage1,
	MIXTYPENAME_Inferna1,
	MIXTYPENAME_Enigma1,
	MIXTYPENAME_Bellum1,
	MIXTYPENAME_Oredo1,

	MIXTYPENAME_Lucidy2 = 201,
	MIXTYPENAME_Sereneo2,
	MIXTYPENAME_Fadeo2,
	MIXTYPENAME_Sparky2,
	MIXTYPENAME_Raident2,
	MIXTYPENAME_Transparo2,
	MIXTYPENAME_Murky2,
	MIXTYPENAME_Devine2,
	MIXTYPENAME_Celesto2,
	MIXTYPENAME_Mirage2,
	MIXTYPENAME_Inferna2,
	MIXTYPENAME_Enigma2,
	MIXTYPENAME_Bellum2,
	MIXTYPENAME_Oredo2,

	MIXTYPENAME_Lucidy3 = 301,
	MIXTYPENAME_Sereneo3,
	MIXTYPENAME_Fadeo3,
	MIXTYPENAME_Sparky3,
	MIXTYPENAME_Raident3,
	MIXTYPENAME_Transparo3,
	MIXTYPENAME_Murky3,
	MIXTYPENAME_Devine3,
	MIXTYPENAME_Celesto3,
	MIXTYPENAME_Mirage3,
	MIXTYPENAME_Inferna3,
	MIXTYPENAME_Enigma3,
	MIXTYPENAME_Bellum3,
	MIXTYPENAME_Oredo3,

	MIXTYPENAME_Lucidy4 = 401,
	MIXTYPENAME_Sereneo4,
	MIXTYPENAME_Fadeo4,
	MIXTYPENAME_Sparky4,
	MIXTYPENAME_Raident4,
	MIXTYPENAME_Transparo4,
	MIXTYPENAME_Murky4,
	MIXTYPENAME_Devine4,
	MIXTYPENAME_Celesto4,
	MIXTYPENAME_Mirage4,
	MIXTYPENAME_Inferna4,
	MIXTYPENAME_Enigma4,
	MIXTYPENAME_Bellum4,
	MIXTYPENAME_Oredo4,

	MIXTYPENAME_Lucidy5 = 501,
	MIXTYPENAME_Sereneo5,
	MIXTYPENAME_Fadeo5,
	MIXTYPENAME_Sparky5,
	MIXTYPENAME_Raident5,
	MIXTYPENAME_Transparo5,
	MIXTYPENAME_Murky5,
	MIXTYPENAME_Devine5,
	MIXTYPENAME_Celesto5,
	MIXTYPENAME_Mirage5,
	MIXTYPENAME_Inferna5,
	MIXTYPENAME_Enigma5,
	MIXTYPENAME_Bellum5,
	MIXTYPENAME_Oredo5,

	MIXTYPENAME_Lucidy6 = 601,
	MIXTYPENAME_Sereneo6,
	MIXTYPENAME_Fadeo6,
	MIXTYPENAME_Sparky6,
	MIXTYPENAME_Raident6,
	MIXTYPENAME_Transparo6,
	MIXTYPENAME_Murky6,
	MIXTYPENAME_Devine6,
	MIXTYPENAME_Celesto6,
	MIXTYPENAME_Mirage6,
	MIXTYPENAME_Inferna6,
	MIXTYPENAME_Enigma6,
	MIXTYPENAME_Bellum6,
	MIXTYPENAME_Oredo6,

	MIXTYPENAME_Lucidy7 = 701,
	MIXTYPENAME_Sereneo7,
	MIXTYPENAME_Fadeo7,
	MIXTYPENAME_Sparky7,
	MIXTYPENAME_Raident7,
	MIXTYPENAME_Transparo7,
	MIXTYPENAME_Murky7,
	MIXTYPENAME_Devine7,
	MIXTYPENAME_Celesto7,
	MIXTYPENAME_Mirage7,
	MIXTYPENAME_Inferna7,
	MIXTYPENAME_Enigma7,
	MIXTYPENAME_Bellum7,
	MIXTYPENAME_Oredo7,

	MIXTYPENAME_Lucidy8 = 801,
	MIXTYPENAME_Sereneo8,
	MIXTYPENAME_Fadeo8,
	MIXTYPENAME_Sparky8,
	MIXTYPENAME_Raident8,
	MIXTYPENAME_Transparo8,
	MIXTYPENAME_Murky8,
	MIXTYPENAME_Devine8,
	MIXTYPENAME_Celesto8,
	MIXTYPENAME_Mirage8,
	MIXTYPENAME_Inferna8,
	MIXTYPENAME_Enigma8,
	MIXTYPENAME_Bellum8,
	MIXTYPENAME_Oredo8,

	MIXTYPENAME_Lucidy9 = 901,
	MIXTYPENAME_Sereneo9,
	MIXTYPENAME_Fadeo9,
	MIXTYPENAME_Sparky9,
	MIXTYPENAME_Raident9,
	MIXTYPENAME_Transparo9,
	MIXTYPENAME_Murky9,
	MIXTYPENAME_Devine9,
	MIXTYPENAME_Celesto9,
	MIXTYPENAME_Mirage9,
	MIXTYPENAME_Inferna9,
	MIXTYPENAME_Enigma9,
	MIXTYPENAME_Bellum9,
	MIXTYPENAME_Oredo9
};

enum EMixAttributeType
{
	MIXATTRTYPE_Fire			= 1,
	MIXATTRTYPE_Ice				= 2,
	MIXATTRTYPE_Lightning		= 4,
	MIXATTRTYPE_Poison			= 8,
	MIXATTRTYPE_Organic			= 16,
	MIXATTRTYPE_Critical		= 32,
	MIXATTRTYPE_AttackRating	= 64,
	MIXATTRTYPE_DamageMin		= 128,
	MIXATTRTYPE_DamageMax		= 256,
	MIXATTRTYPE_AttackSpeed		= 512,
	MIXATTRTYPE_Absorption		= 1024,
	MIXATTRTYPE_Defense			= 2048,
	MIXATTRTYPE_BlockRating		= 4096,
	MIXATTRTYPE_WalkSpeed		= 8192,
	MIXATTRTYPE_HP				= 16384,
	MIXATTRTYPE_MP				= 32768,
	MIXATTRTYPE_SP				= 65536,
	MIXATTRTYPE_HPRegen			= 131072,
	MIXATTRTYPE_MPRegen			= 262144,
	MIXATTRTYPE_SPRegen			= 524288,
	MIXATTRTYPE_PotionStorage	= 1048576,

};

enum EItemSlotFlag : unsigned int
{
	ITEMSLOTFLAG_None			= 0,
	ITEMSLOTFLAG_Inventory		= 1 << 0,				//1
	ITEMSLOTFLAG_LeftHand		= 1 << 1,				//2
	ITEMSLOTFLAG_RightHand		= 1 << 2,				//4
	ITEMSLOTFLAG_LeftRightHand	= (1 << 2) | (1 << 1),	//6
	ITEMSLOTFLAG_Armor			= 1 << 3,				//8
	ITEMSLOTFLAG_Boots			= 1 << 4,				//16
	ITEMSLOTFLAG_Gauntlets		= 1 << 5,				//32
	ITEMSLOTFLAG_RingLeft		= 1 << 6,				//64
	ITEMSLOTFLAG_RingRight		= 1 << 7,				//128
	ITEMSLOTFLAG_Sheltom		= 1 << 8,				//256
	ITEMSLOTFLAG_Amulet			= 1 << 9,				//512
	ITEMSLOTFLAG_Bracelets		= 1 << 11,				//2048
	ITEMSLOTFLAG_Potion			= 1 << 13,				//8192
	ITEMSLOTFLAG_Costume		= 1 << 14,				//16384
};

enum EMixEffect : unsigned int
{
	MIXEFFECT_None				= 0x00000000,
	MIXEFFECT_Flame				= 0x00000001,
	MIXEFFECT_Frost				= 0x00000002,
	MIXEFFECT_Light				= 0x00000004,
	MIXEFFECT_Poison			= 0x00000008,
	MIXEFFECT_Organic			= 0x00000010,
	MIXEFFECT_Critical			= 0x00000020,
	MIXEFFECT_AttackRating		= 0x00000040,
	MIXEFFECT_MinAttackPower	= 0x00000080,
	MIXEFFECT_MaxAttackPower	= 0x00000100,
	MIXEFFECT_WeaponSpeed		= 0x00000200,
	MIXEFFECT_AbsorbRating		= 0x00000400,
	MIXEFFECT_DefenseRating		= 0x00000800,
	MIXEFFECT_Block				= 0x00001000,
	MIXEFFECT_WalkSpeed			= 0x00002000,
	MIXEFFECT_HP				= 0x00004000,
	MIXEFFECT_MP				= 0x00008000,
	MIXEFFECT_SP				= 0x00010000,
	MIXEFFECT_HPRegen			= 0x00020000,
	MIXEFFECT_MPRegen			= 0x00040000,
	MIXEFFECT_SPRegen			= 0x00080000,
	MIXEFFECT_PotionStorage		= 0x00100000,
};

enum EItemInvSound
{
	ITEMINVSOUND_None			= 0,
	ITEMINVSOUND_Axe			= 1,
	ITEMINVSOUND_Claw			= 2,
	ITEMINVSOUND_Armor			= 9,
};

enum EItemSlot : int
{
	ITEMSLOT_None				= 0,

	ITEMSLOT_RightHand			= 1,
	ITEMSLOT_LeftHand			= 2,
	ITEMSLOT_Armor				= 3,
	ITEMSLOT_Amulet				= 4,
	ITEMSLOT_RingRight			= 5,
	ITEMSLOT_RingLeft			= 6,
	ITEMSLOT_Sheltom			= 7,
	ITEMSLOT_Bracelets			= 8,
	ITEMSLOT_Gauntlets			= 9,
	ITEMSLOT_Boots				= 10,
	ITEMSLOT_PotionOne			= 11,
	ITEMSLOT_PotionTwo			= 12,
	ITEMSLOT_PotionThree		= 13,
	ITEMSLOT_Costume			= 14,
};

enum ESocketType : unsigned char
{
	SOCKETTYPE_None,

	SOCKETTYPE_Prismatic,
	SOCKETTYPE_Emerald,
	SOCKETTYPE_Ruby,
	SOCKETTYPE_Aquamarine,
	SOCKETTYPE_Amethyst,
	SOCKETTYPE_Topaz,

	SOCKETTYPE_Count = 7,
};

enum EGemType : unsigned char
{
	GEMTYPE_None,

	GEMTYPE_Triangular,
	GEMTYPE_Square,
	GEMTYPE_Marquise,
};

enum EGemItem : unsigned char
{
	GEMITEM_None,

	/*Normal Gems*/
	GEMITEM_Emerald,
	GEMITEM_Ruby,
	GEMITEM_Aquamarine,
	GEMITEM_Amethyst,
	GEMITEM_Topaz,

	/*Special Gems*/
	GEMITEM_RainbowOpal,
	GEMITEM_DemonSlayer,
	GEMITEM_BloodyPerl,
	GEMITEM_NavyPerl,
	GEMITEM_TempskronMartyr,
	GEMITEM_MorionMartyr,
	GEMITEM_RoyalDiamond,
	GEMITEM_PolishedCitrine,
	GEMITEM_ElementalTourmaline,
	GEMITEM_DivineHeart,
	GEMITEM_LuckyCirclet,
};

enum EGemSpecial
{
	GEMSPECIAL_None,
	GEMSPECIAL_WPNMaxAtkPowAdd1,				//+1 Max Atk Pow on Weapon
	GEMSPECIAL_WPNMaxAtkPowAdd2,				//+2 Max Atk Pow on Weapon
	GEMSPECIAL_WPNReqDec5PercentStrenght,		//-5% Strenght on Weapon
	GEMSPECIAL_WPNAtkPowAdd1,					//+1 Min & Max Atk Pow on Weapon
	GEMSPECIAL_HPAdd10,							//+10 HP
	GEMSPECIAL_WeightAdd100,					//+100 iWeight
	GEMSPECIAL_AbsorbAdd5,						//+5 fAbsorbRating
	GEMSPECIAL_BlockAdd10Percent,				//+10% Block
	GEMSPECIAL_WPNReqDec5PercentSpirit,			//-5% Spirit
	GEMSPECIAL_HPRegen1,						//1.0 Regen HP
	GEMSPECIAL_FireResistance10,				//+10 Fire saElementalDef
	GEMSPECIAL_MPAdd100,						//+100 MP
	GEMSPECIAL_MPRegen2,						//2.0 Regen MP
	GEMSPECIAL_SPAdd200,						//+200 SP
	GEMSPECIAL_IceResistance10,					//+10 Ice saElementalDef
	GEMSPECIAL_WPNReqDec5PercentTalent,			//-5% Talent
	GEMSPECIAL_AtkRtgAdd20,						//+20 Attack Rating
	GEMSPECIAL_DefenseAdd30,					//+30 Defense
	GEMSPECIAL_WPNRangeAdd10,					//+10 Range on Weapon
	GEMSPECIAL_OrganicResistanceAdd10,			//+10 Organic saElementalDef
	GEMSPECIAL_WPNReqDec5PercentAgility,		//-5% Agility on Weapon
	GEMSPECIAL_AtkRtgAdd30,						//+30 Attack Rating
	GEMSPECIAL_DefenseAdd40,					//+40 Defense
	GEMSPECIAL_RunSpeedAdd1,					//+1 Run fMovementSpeed
	GEMSPECIAL_LightningResistanceAdd10,		//+10 Lightning saElementalDef
	GEMSPECIAL_BlockAdd1Percent,				//+1% Block
	GEMSPECIAL_HPAdd25,							//+25 HP
	GEMSPECIAL_AbsorbAdd8,						//+8 fAbsorbRating
	GEMSPECIAL_AllStatsDec2Percent,				//-2% All Stats
	GEMSPECIAL_HPRegen2,						//2.0 HP Regen
	GEMSPECIAL_DefenseAdd50,					//+50 Defense
	GEMSPECIAL_PoisonResistanceAdd10,			//+10 Poison saElementalDef
};

struct SocketItem
{
	ESocketType 	eSocketType;
	EGemType		eGemType;
	EGemItem		eGemItem;
};

#ifdef ITEMFIXES
struct ItemFix
{

	short									iID = 0;
	char									sName[12];
	short									iMinLevel = 0;
	short									iMaxLevel = 0;
	short									iMinRarity = 0;
	short									iMaxRarity = 0;
	short									isPrefix = 0;
	short									isSuffix = 0;
	short									iChance = 0;
	short									iAxes = 0;
	short									iBows = 0;
	short									iJavelins = 0;
	short									iDaggers = 0;
	short									iWands = 0;
	short									iScythes = 0;
	short									iSwords = 0;
	short									iClaws = 0;
	short									iHammers = 0;
	short									iPhantoms = 0;
	short									iArmors = 0;
	short									iRobes = 0;
	short									iBoots = 0;
	short									iGauntlets = 0;
	short									iBracelets = 0;
	short									iAmulets = 0;
	short									iRings = 0;
	short									iShields = 0;
	short									iOrbs = 0;
	short									iOrganicMin = 0;
	short									iOrganicMax = 0;
	short									iFireMin = 0;
	short									iFireMax = 0;
	short									iFrostMin = 0;
	short									iFrostMax = 0;
	short									iLightningMin = 0;
	short									iLightningMax = 0;
	short									iPoisonMin = 0;
	short									iPoisonMax = 0;
	short									iATKPowMin = 0;
	short									iATKPowMax = 0;
	short									iRangeMin = 0;
	short									iRangeMax = 0;
	short									iATKRatingMin = 0;
	short									iATKRatingMax = 0;
	float									fCriticalMin = 0;
	float									fCriticalMax = 0;
	float									fBlockMin = 0;
	float									fBlockMax = 0;
	float									fAbsorbMin = 0;
	float									fAbsorbMax = 0;
	short									iDefenseMin = 0;
	short									iDefenseMax = 0;
	float									fRegenerationHPMin = 0;
	float									fRegenerationHPMax = 0;
	float									fRegenerationMPMin = 0;
	float									fRegenerationMPMax = 0;
	short									iAddHPMin = 0;
	short									iAddHPMax = 0;
	short									iAddMPMin = 0;
	short									iAddMPMax = 0;
	float									fRunSpeedMin = 0;
	float									fRunSpeedMax = 0;
};
#endif

#ifdef ITEMFIXES
struct ItemFixData
{
	//struct has a 64 bit max size

	short									iID = 0;
	char									sName[12];
	short									iATKPow = 0;
	short									iATKRating = 0;
	short									iRange = 0;
	float									fBlock = 0;
	float									fAbsorb = 0;
	short									iDefense = 0;
	float									fRegenerationHP = 0;
	float									fRegenerationMP = 0;
	short									iAddHP = 0;
	short									iAddMP = 0;
	float									fRunSpeed = 0;
	float									fCritical = 0;
	short									iOrganic = 0;
	short									iFire = 0;
	short									iFrost = 0;
	short									iLightning = 0;
	short									iPoison = 0;

	//char									sPadding[0];
};
#endif

struct SpecItemData
{
	float									fSpecAbsorbRating;
	int										iSpecDefenseRating;
	float									fSpecMovementSpeed;
	float									fSpecBlockRating;
	int										iSpecAttackSpeed;
	int										iSpecCritical;
	int										iSpecAttackRange; // shooting range
	float									fSpecMagicMastery;
	short									saSpecElementalDef[8];
	short									saSpecElementalAtkDiv[8];
	int										iSpecAddMPDiv;
	int										iSpecAddHPDiv;
	int										iSpecAttackRatingDiv;
	short									iSpecAttackPowerDivMin;
	short									iSpecAttackPowerDiv;
	float									fSpecMPRegen;
	float									fSpecHPRegen;
	float									fSpecSPRegen;

#ifdef ITEMFIXES
	//char									iPadding99[0];   //Removed all adding for Item Fixes
#else
	int										iPadding99[32];
#endif
};

inline EItemFlag operator | (EItemFlag lhs, EItemFlag rhs)
{
	using T = std::underlying_type_t <EItemFlag>;
	return static_cast<EItemFlag>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline EItemFlag operator & (EItemFlag lhs, EItemFlag rhs)
{
	using T = std::underlying_type_t <EItemFlag>;
	return static_cast<EItemFlag>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline EItemFlag operator ^ (EItemFlag lhs, EItemFlag rhs)
{
	using T = std::underlying_type_t <EItemFlag>;
	return static_cast<EItemFlag>(static_cast<T>(lhs) ^ static_cast<T>(rhs));
}

inline EItemFlag& operator |= (EItemFlag& lhs, EItemFlag rhs)
{
	lhs = lhs | rhs;
	return lhs;
}

inline EItemFlag& operator ^= (EItemFlag& lhs, EItemFlag rhs)
{
	lhs = lhs ^ rhs;
	return lhs;
}

//sITEMINFO
class ItemID
{
public:
	ItemID() {};
	ItemID( int i ) { *(int *)(this) = i; }

	char									iIDHigh;	//always zero
	char									iIDLow;		//++ counter
	short									sItemType;

	inline unsigned int						ToInt() { return *(unsigned int *)(this); }
	inline EItemID							ToItemID() { return *(EItemID*)(this); }
	inline EItemType						ToItemType() { return (EItemType)(*(unsigned int *)(this) & 0xFFFF0000); }
	inline EItemBase						ToItemBase() { return (EItemBase)(*(unsigned int *)(this) & 0xFF000000); }

	inline void								SetItemID( EItemID iItemID ) { *(EItemID*)this = iItemID; }

	static inline ItemID					* ToItemID( EItemID * i ) { return (ItemID *)i; }
};


enum EItemRarity : char
{
	NONE = 0,
	COMMON = 1,
	UNCOMMON = 2,
	RARE = 3,
	EPIC = 4,
	LEGENDARY = 5
};

//struct sITEMINFO
struct Item
{
	//Attributes
	DWORD									dwSize;		//Item structure size
	ITEMCHECKSUM							iChk1;		//Header
	DWORD									dwVersion;	//Generated version
	DWORD									dwTime;		//Creation Time
	ITEMCHECKSUM							iChk2;		//Items checksum

	//Statistics
	CurMax									sIntegrity;			//Durability
	ItemID									sItemID;			//CODE
	char									szItemName[32];		//ItemName
	int										iWeight;			//Weight
	int										iSalePrice;			//Price
	int										iIndex;				//Index
	int										iPotionCount;		//PotionCount
	short									saElementalDef[8];	//Resistance
	int										iSight;				//Sight
	DWORD									iUnknown02;			//Temp0
	MinMax									sDamage;			//Damage
	int										iAttackRange;		//Shooting_Range
	int										iAttackSpeed;		//Attack_Speed
	int										iAttackRating;		//Attack_Rating
	int										iCritical;			//Critical_Hit
	float									fAbsorbRating;		//fAbsorb
	int										iDefenseRating;		//Defence
	float									fBlockRating;		//fBlock_Rating
	float									fMovementSpeed;		//fSpeed
	int										iPotionStorage;		//Potion_Space
	float									fMagicMastery;		//fMagic_Mastery
	float									fMPRegen;			//fMana_Regen
	float									fHPRegen;			//fLife_Regen
	float									fSPRegen;			//fStamina_Regen
	float									fAddHP;				//fIncrease_Life
	float									fAddMP;				//fIncrease_Mana
	float									fAddSP;				//fIncrease_Stamina
	int										iLevel;				//Level
	int										iStrength;			//Strength
	int										iSpirit;			//Spirit
	int										iTalent;			//Talent
	int										iAgility;			//Dexterity
	int										iHealth;			//Health
	MinMax									sMPRecovery;		//Mana
	MinMax									sHPRecovery;		//Life
	MinMax									sSPRecovery;		//Stamina
	int										iGold;				//Money
	BOOL									bCanNotUse;			//NotUseFlag

	DWORD									iBackupKey;			//BackUpKey	   (Backup header). Once set, it does not get changed. See ReformItem in leaked code
	DWORD									iBackupChk;			//BackUpChkSum (Backup item checksum). Once set, it does not get changed. See ReformItem in leaked code

	short									iScaleBlink[2];		//ScaleBlink
	int										iItemUniqueID;		//UniqueItem (0 = normal, 2 = quest weapon)
	short									sEffectBlink[2];	//EffectBlink
	short									sEffectColor[4];	//EffectColor
	BOOL									bDisallowDrop;		//DispEffect -- this determines if the item can be dropped or not..

	int										eSpecialization;	//JobCodeMask
	SpecItemData							sSpecData;			//JobItem

#ifdef ITEMFIXES
	ItemFixData								sPrefixData;		//Item Fix Data
	ItemFixData								sSuffixData;		//Item Fix Data
#endif

	EItemCraftType							eCraftType;			//ItemKindCode
	int										eMixEffect;			//ItemKindMask
	union
	{
		USHORT								sAgeLevel;			//ItemAgingNum[0]
		USHORT								sMixColorId;		//ItemAgingNum[0]
	};
	union
	{
		USHORT								bMaturing;				//ItemAgingNum[1]
		USHORT								uQuestMonId;			//ItemAgingNum[1]
	};
	UCurMax									sMatureBar;				//ItemAgingCount[2]

	short									sItemAgingProtect[2];	//Aging protection

	short									sSpecialItemType[2];	//SpecialItemFlag - 0 is a curse item Flex / 1 is a special item used for quests

	DWORD									tTime;					//dwCreateTime

	SocketItem								sSocketData[2];

	EItemFlag								eItemFlags;				//8 bits item flags
	EItemSource								eItemLastSource;		//Last source where the item came from (could be useful for quests etc)
	EItemSource								eItemOriginalSource;	//Original source of item

	char									cPadding[6];			//spare

	EItemRarity								eRarity;				//rarity of the item.

	int										sMixUniqueID1;			//unique mix id
	CurMax									sQuestMonsters;

	EMixTypeName							eMixTypeName;
	int										iItemSpecialType;		//1 = perfect, 2 = quest reward

	int										iPadding666;
};

//struct sITEM
class ItemData
{
public:
	ItemID									sBaseItemID;
	char									szBaseName[32];
	char									szInventoryName[16];
	int										iWidth;
	int										iHeight;
	char									szCategory[64];
	EItemSlotFlag							iItemSlotFlag;		//Class - The type of item
	char									szModelName[64];	//Model name for when item is on floor
	EItemSlotFlag							iVisibleItemType;	//SetModelPosi - Position the item to be set
	EItemInvSound							iInvItemSound;		//SoundIndex - Sound of item
	int										iItemWpnDmg;
	BOOL									bValid;				//Flags items
	Point2D									sPosition;
	Point2D									sPlacePosition;
	void									* pcInventoryTexture;
	int										iItemSlot;
	int										iPotionCount;		//Only when the potion counts
	int										iNotUse;			//NotUseFlag
	int										iSalePrice;
	Point2D									sOldPosition;
	void									* pcTempInventoryTexture;
	Item									sItem;

	BOOL									IsOneHandedWeapon();
	BOOL									IsTwoHandedWeapon();


};

struct DefinitionItem
{
	struct Item								sItem; // 0x0
	short									IntegrityMin; // 0x214
	short									IntegrityMax; // 0x216
	short									OrganicMin; // 0x218
	short									OrganicMax; // 0x21A
	short									UnknownResistanseMin; // 0x21C
	short									UnknownResistanseMax; // 0x21E
	short									FireMin; // 0x220
	short									FireMax; // 0x222
	short									FrostMin; // 0x224
	short									FrostMax; // 0x226
	short									LightningMin; // 0x228
	short									LightningMax; // 0x22A
	short									PoisonMin; // 0x22C
	short									PoisonMax; // 0x22E
	short									sUnknownResistance[4]; // 0x230
	short									AttackPower1Min; // 0x238 (Min of Min value) X -
	short									AttackPower2Min; // 0x23A (Min of Max value)   - X
	short									AttackPower1Max; // 0x23C (Max of Min value) X -
	short									AttackPower2Max; // 0x23E (Max of Max value)   - X
	short									AttackRatingMin; // 0x240
	short									AttackRatingMax; // 0x242
	short									DefenseMin; // 0x244
	short									DefenseMax; // 0x246
	float									BlockRatingMin; // 0x248
	float									BlockRatingMax; // 0x24C
	float									AbsorbMin; // 0x250
	float									AbsorbMax; // 0x254
	float									RunSpeedMin; // 0x258
	float									RunSpeedMax; // 0x25C
	int										AddHPMin; // 0x260
	int										AddHPMax; // 0x264
	int										AddMPMin; // 0x268
	int										AddMPMax; // 0x26C
	int										AddSTMMin; // 0x270
	int										AddSTMMax; // 0x274
	float									MPRegenMin; // 0x278
	float									MPRegenMax; // 0x27C
	float									HPRegenMin; // 0x280
	float									HPRegenMax; // 0x284
	float									STMRegenMin; // 0x288
	float									STMRegenMax; // 0x28C
	short									AddSpecDefenseMin; // 0x290
	short									AddSpecDefenseMax; // 0x292
	float									AddSpecAbsorbMin; // 0x294
	float									AddSpecAbsorbMax; // 0x298
	float									AddSpecRunSpeedMin; // 0x29C
	float									AddSpecRunSpeedMax; // 0x2A0
	float									SpecialMagicMastery[0x2]; // 0x2A4
	float									AddSpecMPRegenMin; // 0x2AC
	float									AddSpecMPRegenMax; // 0x2B0
	int										SpecAttackRatingMin; // 0x2B4
	int										SpecAttackRatingMax; // 0x2B8
	DWORD									JobBitCodeRandom[0xC]; // 0x2BC
	int										JobBitCodeRandomCount; // 0x2EC
	short									sGenDay[0x2]; // 0x2F0
	unsigned char *							DefCompressData[0x214]; // 0x2F4
	int										DefCompressDataLen; // 0xB44
};

struct DropSettingsData
{
	int					iItemID;	//0x00
	int					iPercent;	//0x04
	short				sGoldMin;	//0x08
	short				sGoldMax;	//0x0A
};


struct ItemBoxItem
{
	int										iID;
	BOOL									bValid;
	int										iItemID;
	int										iCount;
	int										iSpecialization;
	BOOL									bCoinShop;
	BOOL									bHasItem;
};

struct ItemBoxSlot
{
	int										iNumItems;
	ItemBoxItem								saItemBox[MAX_ITEMSINITEMBOX];
};

struct PlayerTitle
{
	unsigned short sTitleID;
	short sRarity;
	char  szTitle[20];
};

struct RecordItem
{
	int			iItemCount;

	Point2D		sItemPosition;
	int			iItemPosition;

	Item	sItem;
};

struct InventoryItemData
{
	int				iPosition;
	Rectangle2D		sBox;
	int				iItemIndex;
	int				iWeaponClass;
};

struct SellItemShopData
{
	EItemID				eItemID;
	DWORD				dwHead;
	DWORD				dwChecksum;
	int					iPrice;
};

struct ItemLoadData
{
	BOOL				bState;
	DWORD				dwSerial;
	Item				sItem;
};

struct RespecStone
{
	EItemID				eItemID;
	int					iMinLevel;
	int					iMaxLevel;
	int					iRequiredStones;
	int					iPrice;
};

struct PriceItem
{
	int					iRealPrice;
	int					iPrice;
	int					iRepairPrice;
};



struct SocketItemData
{
	BOOL					bOpenNPC;
	ItemData				sItemData;
	ItemData				sRune[5];
	ItemData				sStone;
	int						iSocketWorkType;
	char					cPadding[32];
	int						iItemCounter;
};

struct ItemLogBox
{
	int										iSize;
	int										iID;
	struct SPlayer
	{
		char								szAccountName[32];
		char								szCharacterName[32];
		IN_ADDR								iIP;
	};

	struct SItem
	{
		int									iItemID;
		int									iCount;
		int									iChecksum1;
		int									iChecksum2;
	};

	int										iActionCode;

	SPlayer									sPlayer;
	SItem									sItem;
};

struct CompressedItem
{
	ItemID									sItemID;
	char									szBaseName[32];
	char									szInventoryName[16];
	int										iWidth;
	int										iHeight;
	char									szDropFolder[16];
	UINT									iClass;
	char									szDropName[16];
	UINT									iModelPosition;
	UINT									iSound;
	UINT									iWeaponClass;
};


struct RGB
{
	int red;
	int green;
	int blue;
};

struct ARGBNew
{
	int red;
	int green;
	int blue;
	int alpha;
};

const RGB rarityColors[6] = {
	{51, 51, 51},
	{51, 51, 51},
	{24, 63, 28},
	{23, 29, 62},
	{54, 29, 60},
	{72, 40, 23}
};


const ARGBNew rarityColorsGround[6] = {
	{200, 255, 255, 255},			//No Rarity
	{200, 255, 255, 255},			//Common
	{255, 184, 255, 190},				//Uncommon
	{255, 180, 192, 255},			//Rare
	{255, 238, 165, 255},			//Epic
	{255, 255, 166, 121}				//Legendary
};

const ARGBNew rarityColorsGroundHover[6] = {
	{255, 255, 255, 255},			//No Rarity
	{255, 255, 255, 255},			//Common
	{255, 85, 253, 52},				//Uncommon
	{255, 127, 149, 250},			//Rare
	{255, 221, 126, 240},			//Epic
	{255, 255, 108, 35}				//Legendary
};


/// <summary>
/// class scITEM
/// </summary>
struct DropItemView
{
	BYTE									bFormatted;
	EItemFlag								eItemFlags;

	EItemRarity								eRarity;
	BYTE									bSpare3;

	BOOL									bVisible;		//DisplayFlag
	BOOL									bActive;		//Flag

	Point3D									sPosition;

	char									szModelName[64];
	char									szName[64];

	int										iClass;			//lpStgArea

	class EXEModel							* pcModel;		//Pattern
	void									* pcMesh;		//lpDinaPattern

	BOOL									bHidden;		//PatLoading

	ItemID									iItemID;		//ItemCode

	DWORD									dwLastUpdate;	//dwLastTransTime;		- Last transmitted time

	int										iServerID;		//ServerCode;			- Applicable character server code

	Point3D									sRenderPosition;	//RendPoint
	Point3D									sSelfPosition;		//Posi
	Point3D									sAngle;				//Angle
};

struct ItemInventoryData
{
	int										iItemSlot;		//Position
	Rectangle2D								sBox;			//Rect
	int										iItemIndex;		//ItemIndex
	int										iWeaponClass;	//WeaponClass
};

#define ITEM_PERSONALSHOP_MAX 32

struct PacketPersonalBotShopData : Packet
{
	char									szMessage[256];
	ItemData								cItemData[32];
	int										iPrice[ITEM_PERSONALSHOP_MAX];
	int										iaItems[3];
};

enum ESlotTypeHandle
{
	ITEMSLOTHANDLE_Get,
	ITEMSLOTHANDLE_Put,
};
enum ESlotTypeHandleWhere
{
	ITEMSLOTHANDLEWHERE_Perfectize,
	ITEMSLOTHANDLEWHERE_Swap,
};

struct PacketSlotItemHandle : Packet
{

	ESlotTypeHandle			iTypeHandle;
	ESlotTypeHandleWhere	iTypeWhere;

	int					iItemID;

	int					iChk1;
	int					iChk2;
};

struct NPCItemBoxMessageBox
{
	BOOL				bActive;
	ItemData			cItemData;
	char				szMessage[128];
	char				szItemName[64];

	BOOL				bYesButtonHover;
	BOOL				bNoButtonHover;

	DWORD				dwReserved[4];
};

struct PacketNPCItemBox : Packet
{
	char									szItemName[32];
	char									szMessage[128];
	int										iItemID;
	int										iSpecialization;
	DWORD									dwReserved[3];

	short									sWeight;
	short									sUnknown;

	BOOL									bItem;

	Item									sItem;
};

struct PacketMixServer : Packet
{
	int										iDocIndex;
	int										iResult;
	ItemData								sItemData;
	DWORD									dwCode[12];
	DWORD									dwHead[12];
	DWORD									dwCheckSum[12];
	int										iIndex;
	int										iGold;

	DWORD									dwStoneItemCode;
	DWORD									dwStoneHead;
	DWORD									dwStoneCheckSum;
};

struct PacketRecoverItem : Packet
{
	int							iStatus;
	struct Item					sItem;
};

struct PacketItemData : Packet
{
	int										iItemIndex;
	ItemData								sItem;
};

struct PacketUseUnionCore : Packet
{
	Item								sItem;
	Point3D									sPosition;
};

struct PacketWingItem : Packet
{
    int DocIndex;
    int Result;
    ItemData DesCraftItem;
    DWORD SheltomCode[12];
    DWORD Head[12];
    DWORD CheckSum[12];
    int  Index;
    int  Money;

    DWORD A_StoneItemCode;
    DWORD A_StoneHead;
    DWORD A_StoneCheckSum;
};

struct PacketThrowItem : Packet
{
	Item								sItem;
	Point3D									sPosition;
	DWORD									dwSecCode[4];
};

struct PacketResetItem : Packet
{
	int										iDocIndex;
	int										iResult;
	int										iIndex;

	ItemData								sItemData;

	DWORD									dwCode;
	DWORD									dwHead;
	DWORD									dwCheckSum;
};

struct PacketQuestNPCItemList : Packet
{
	int										iID;

	int										iIndex;

	class ItemData							cItemData;

	int										iQuantity;

	BOOL									bLast;
};

struct PacketFuryArenaItemData : Packet
{
	ItemData								sItemData;
};

struct PacketSocketSystem : Packet
{
	ItemData								sItemData;
	ItemData								sRune[5];
	ItemData								sItemStone;
	int										iSocketWorkType; //0x0E = Drill, 0x0F = Stone
}; //sSize = 0x1598

struct PacketItemPerfectView : Packet
{
	ItemData								cItemData;
	int										iDefenseOverride;
};

//smTRANS_ACITON_ITEM
struct PacketItem : Packet
{
	struct Item							sItem;
	int										iX;
	int										iY;
	int										iZ;
	DWORD									dwSecCode[4];
};


struct PacketManufactureItem : Packet
{
	int								iUunknown;
	int								iResult;
	ItemData						sItemData;
	EItemID							iRecipeID;
	EItemID							iaRuneID[3];
	int								iRecipeChk1;
	int								iaChk1[3];
	int								iRecipeChk2;
	int								iaChk2[3];
	int								iIndex;
	int								iGold;

	DWORD							iExStoneItemCode;
	DWORD							iExStoneHead;
	DWORD							iExStoneCheckSum;
};

//Item structure to be sent to the server
//sCRAFTITEM_SERVER
struct PacketAgingItem : Packet
{
	int						DocIndex;
	int						Result;
	ItemData				sItem;
	DWORD					SheltomCode[12];
	DWORD					Head[12];
	DWORD					CheckSum[12];
	int						Index;
	int						Money;

	DWORD					StoneItemCode;
	DWORD					StoneHead;
	DWORD					StoneCheckSum;
};

struct PacketBuyNPCShop : Packet
{
	ItemData				sItemData;
	int						iCount;

	int						iUniqueID;
	int						iNPCID;
};

struct PacketItemShopList : Packet
{
	int						iShopType;
	int						iItemCount;
	int						iUniqueID;
	int						iNPCID;
	int						iUnk001[2];
	char					szBuffer[0x1FE0];
};

struct PacketNetSendItemData : Packet
{
	ID				dwUserID;
	int				iItemID;
	EItemSource     eItemSource;
};

struct PacketSellNPCShop : Packet
{
	Item					sItem;
	int Count, Money, z;

	DWORD	dwSeCode[4];		//���� �ڵ�
};

struct PacketRepairNPCShop : Packet
{
    int         Money;
    ItemData    sItemData;
};

struct PacketGetItem : Packet
{
	int						iState;
	Point3D					sPosition;

	class Map			  * pcMap;
};

struct PacketItemLinkChat : Packet
{
	char					szCharacterName[32];

	char					szMessage[256];

	int						eChatColor;

	ItemData				cItemData;
};

