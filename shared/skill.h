#pragma once

#include "PSkill.h"

#include "item.h"

#define SKILLCONTAINER_LENGTH	0x1FF4
#define SKILLBUFF_MAX_COUNT	100
#define MAX_SKILLS				228
#define MAX_MASTERY				200

#define TINYSKILLID_ENCHANT_WEAPON_LIGHTING       100

#define	SKILL_SOUND_LEARN					0x1000
#define	SKILL_SOUND_EXTREME_SHIELD			0x1010
#define	SKILL_SOUND_MECHANIC_BOMB1			0x1020
#define	SKILL_SOUND_MECHANIC_BOMB2			0x1021
#define	SKILL_SOUND_PHYSICAL_ABSORB1		0x1030
#define	SKILL_SOUND_PHYSICAL_ABSORB2		0x1031
#define	SKILL_SOUND_PHYSICAL_ABSORB3		0x1032
#define	SKILL_SOUND_JUMPING_CRASH1			0x1130
#define	SKILL_SOUND_JUMPING_CRASH2			0x1131
#define	SKILL_SOUND_PIKE_WIND				0x1200
#define	SKILL_SOUND_SCOUT_HAWK1				0x1310
#define	SKILL_SOUND_SCOUT_HAWK2				0x1311
#define	SKILL_SOUND_WIND_ARROW				0x1320
#define	SKILL_SOUND_PERFECT_AIM				0x1330

#define	SKILL_SOUND_ARROW_HIT1				0x1301
#define	SKILL_SOUND_ARROW_HIT2				0x1302

#define	SKILL_SOUND_SKILL_SWING1			0x2000
#define	SKILL_SOUND_SKILL_SWING2			0x2001

#define	SKILL_SOUND_SKILL_AUTOMATION1		0x2010
#define	SKILL_SOUND_SKILL_AUTOMATION2		0x2011
#define	SKILL_SOUND_SKILL_AUTOMATION3		0x2012
#define	SKILL_SOUND_SKILL_AUTOMATION4		0x2013
#define	SKILL_SOUND_SKILL_AUTOMATION5		0x2014

#define	SKILL_SOUND_SKILL_MAXIMIZE1			0x2020
#define	SKILL_SOUND_SKILL_MAXIMIZE2			0x2021
#define	SKILL_SOUND_SKILL_MAXIMIZE3			0x2022

#define	SKILL_SOUND_SKILL_SPARK1			0x2030
#define	SKILL_SOUND_SKILL_SPARK2			0x2031
#define	SKILL_SOUND_SKILL_SPARK3			0x2032

#define	SKILL_SOUND_SKILL_GROUND_PIKE		0x2110

#define	SKILL_SOUND_SKILL_TORNADO1			0x2120

#define	SKILL_SOUND_SKILL_EXPANSION1		0x2130
#define	SKILL_SOUND_SKILL_EXPANSION2		0x2131

#define	SKILL_SOUND_SKILL_ROAR1				0x2201
#define	SKILL_SOUND_SKILL_ROAR2				0x2202

#define	SKILL_SOUND_SKILL_RAGE_OF_ZECRAM1	0x2211
#define	SKILL_SOUND_SKILL_RAGE_OF_ZECRAM2	0x2212
#define	SKILL_SOUND_SKILL_RAGE_OF_ZECRAM3	0x2213

#define	SKILL_SOUND_SKILL_FALCON			0x2310
#define	SKILL_SOUND_SKILL_FALCON_ATTACK1	0x2311
#define	SKILL_SOUND_SKILL_FALCON_ATTACK2	0x2312

#define	SKILL_SOUND_SKILL_AVALANCHE1		0x2322
#define	SKILL_SOUND_SKILL_AVALANCHE2		0x2322
#define	SKILL_SOUND_SKILL_AVALANCHE3		0x2322

#define	SKILL_SOUND_SKILL_ARROW_OF_RANGE	0x2332



#define	SKILL_SOUND_SKILL_CASTING_MAGICIAN	0x1800
#define	SKILL_SOUND_SKILL_CASTING_PRIEST	0x1700

#define	SKILL_SOUND_SKILL_SWORDBLAST1		0x1510
#define	SKILL_SOUND_SKILL_SWORDBLAST2		0x1511
#define	SKILL_SOUND_SKILL_HOLYBODY			0x1520

#define	SKILL_SOUND_SKILL_SHIELDSTRIKE		0x1610
#define	SKILL_SOUND_SKILL_FARINA			0x1620
#define	SKILL_SOUND_SKILL_VIGORSPEAR		0x1640

#define	SKILL_SOUND_SKILL_HEALING			0x1710
#define	SKILL_SOUND_SKILL_HOLYBOLT			0x1720
#define	SKILL_SOUND_SKILL_MULTISPARK		0x1730
#define	SKILL_SOUND_SKILL_HOLYMIND			0x1740

#define	SKILL_SOUND_SKILL_AGONY				0x1810
#define	SKILL_SOUND_SKILL_FIREBOLT1			0x1820
#define	SKILL_SOUND_SKILL_FIREBOLT2			0x1821
#define	SKILL_SOUND_SKILL_ZENITH			0x1830
#define	SKILL_SOUND_SKILL_FIREBALL1			0x1840
#define	SKILL_SOUND_SKILL_FIREBALL2			0x1841



#define	SKILL_SOUND_SKILL_HOLY_VALOR1		0x2510
#define	SKILL_SOUND_SKILL_HOLY_VALOR3		0x2513
#define	SKILL_SOUND_SKILL_BLANDISH1			0x2520
#define	SKILL_SOUND_SKILL_BLANDISH2			0x2521
#define	SKILL_SOUND_SKILL_PIERCING			0x2530
#define	SKILL_SOUND_SKILL_DRASTICSPIRIT1	0x2540
#define	SKILL_SOUND_SKILL_DRASTICSPIRIT3	0x2543


#define	SKILL_SOUND_SKILL_WINDY1			0x2610
#define	SKILL_SOUND_SKILL_WINDY3			0x2613
#define	SKILL_SOUND_SKILL_TWISTJAVELIN		0x2620
#define	SKILL_SOUND_SKILL_SOULSUCKER2		0x2622
#define	SKILL_SOUND_SKILL_SOULSUCKER3		0x2623
#define	SKILL_SOUND_SKILL_SOULSUCKER4		0x2624
#define	SKILL_SOUND_SKILL_FIREJAVELIN		0x2630



#define	SKILL_SOUND_SKILL_DIVINELIGHTNING1	0x2710
#define	SKILL_SOUND_SKILL_DIVINELIGHTNING2	0x2711
#define	SKILL_SOUND_SKILL_DIVINELIGHTNING3	0x2712
#define	SKILL_SOUND_SKILL_HOLYREFLECTION1	0x2720
#define	SKILL_SOUND_SKILL_HOLYREFLECTION2	0x2722
#define	SKILL_SOUND_SKILL_GRANDHEALING		0x2730


#define	SKILL_SOUND_SKILL_WATORNADO1						0x2810
#define	SKILL_SOUND_SKILL_WATORNADO2						0x2811
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_CAST				0x2820
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE1				0x2821
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE2				0x2822
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE1				0x2823
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE2				0x2824
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING1			0x2825
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING2			0x2826
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING1	0x2827
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING2	0x2828
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_ICE1			0x2840
#define	SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_ICE2			0x2841

#define	SKILL_SOUND_SKILL_DEATHRAY							0x2830

//
#define	SKILL_SOUND_SKILL_METAL_ARMOR						0x3010


#define	SKILL_SOUND_SKILL_SPARK_SHIELD						0x3040

//
#define	SKILL_SOUND_SKILL_CONCENTRATION1					0x3110
#define	SKILL_SOUND_SKILL_CONCENTRATION2					0x3111
#define	SKILL_SOUND_SKILL_AVANGING_CRASH					0x3120
#define	SKILL_SOUND_SKILL_SWIFT_AXE1						0x3130
#define	SKILL_SOUND_SKILL_SWIFT_AXE2						0x3131
#define	SKILL_SOUND_SKILL_BONE_CRASH1						0x3140
//
#define	SKILL_SOUND_SKILL_VENOM_SPEAR1						0x3210
#define	SKILL_SOUND_SKILL_VENOM_SPEAR2						0x3211
#define	SKILL_SOUND_SKILL_VANISH1							0x3220
#define	SKILL_SOUND_SKILL_VANISH2							0x3221
#define	SKILL_SOUND_SKILL_CHAIN_LANCE1						0x3240
#define	SKILL_SOUND_SKILL_CHAIN_LANCE2						0x3241
#define	SKILL_SOUND_SKILL_CHAIN_LANCE3						0x3242

//
#define	SKILL_SOUND_SKILL_ELEMENTAL_SHOT					0x3310
#define	SKILL_SOUND_SKILL_GOLDEN_FALCON						0x3320
#define	SKILL_SOUND_SKILL_PERFORATION						0x3330
#define	SKILL_SOUND_SKILL_BOMB_SHOT1						0x3340
#define	SKILL_SOUND_SKILL_BOMB_SHOT2						0x3341


///
#define	SKILL_SOUND_SKILL_FLAME_BRANDISH					0x3410
#define	SKILL_SOUND_SKILL_DIVINE_INHALATION					0x3420
#define	SKILL_SOUND_SKILL_HOLY_INCANTATION					0x3430
#define	SKILL_SOUND_SKILL_GRAND_CROSS						0x3440
//
#define	SKILL_SOUND_SKILL_SPLIT_JAVELIN1					0x3510
#define	SKILL_SOUND_SKILL_SPLIT_JAVELIN2					0x3511
#define	SKILL_SOUND_SKILL_TRIUMPH_OF_VALHALLA1				0x3520
#define	SKILL_SOUND_SKILL_TRIUMPH_OF_VALHALLA2				0x3521
#define	SKILL_SOUND_SKILL_LIGHTNING_JAVELIN1				0x3530
#define	SKILL_SOUND_SKILL_LIGHTNING_JAVELIN2				0x3531
#define	SKILL_SOUND_SKILL_STORM_JAVELIN1					0x3540
#define	SKILL_SOUND_SKILL_STORM_JAVELIN2					0x3541
//
#define	SKILL_SOUND_SKILL_VIGOR_BALL1						0x3610
#define	SKILL_SOUND_SKILL_VIGOR_BALL2						0x3611
#define	SKILL_SOUND_SKILL_RESURRECTION						0x3620
#define	SKILL_SOUND_SKILL_EXTINTION							0x3630
#define	SKILL_SOUND_SKILL_VIRTUAL_LIFE						0x3640
//
#define	SKILL_SOUND_SKILL_ENERGY_SHIELD						0x3710
#define	SKILL_SOUND_SKILL_DIASTROPHISM						0x3720
#define	SKILL_SOUND_SKILL_SPIRIT_ELEMENTAL					0x3730
#define	SKILL_SOUND_SKILL_DANCING_SWORD						0x3740
#define	SKILL_SOUND_SKILL_DANCING_SWORD_ATK1				0x3741
#define	SKILL_SOUND_SKILL_DANCING_SWORD_ATK2				0x3742


///////////////
#define	SKILL_SOUND_SKILL_IMPULSION							0x4010
#define	SKILL_SOUND_SKILL_COMPULSION1						0x4021
#define	SKILL_SOUND_SKILL_COMPULSION2						0x4022
#define	SKILL_SOUND_SKILL_COMPULSION3						0x4023
#define	SKILL_SOUND_SKILL_METAL_GOLEM						0x4040

#define	SKILL_SOUND_SKILL_BERSERKER							0x4120
#define	SKILL_SOUND_SKILL_CYCLONE_STRIKE					0x4130

#define	SKILL_SOUND_SKILL_ASSASSIN_EYE						0x4210
#define	SKILL_SOUND_SKILL_CHARGING							0x4220
#define	SKILL_SOUND_SKILL_CHARGING_STRIKE					0x4221
#define	SKILL_SOUND_SKILL_VAGUE								0x4230
#define	SKILL_SOUND_SKILL_SHADOW_STRIKE1					0x4241
#define	SKILL_SOUND_SKILL_SHADOW_STRIKE2					0x4242
#define	SKILL_SOUND_SKILL_SHADOW_STRIKE3					0x4243
#define	SKILL_SOUND_SKILL_SHADOW_STRIKE4					0x4244

#define	SKILL_SOUND_SKILL_RECALL_WOLVERIN					0x4310
#define	SKILL_SOUND_SKILL_PHOENIX_SHOT						0x4330
#define	SKILL_SOUND_SKILL_FORCE_OF_NATURE					0x4340

#define	SKILL_SOUND_SKILL_DVINE_PIERCING					0x4410
#define	SKILL_SOUND_SKILL_GODLY_SHIELD						0x4420
#define	SKILL_SOUND_SKILL_GODS_BLESS						0x4430
#define	SKILL_SOUND_SKILL_SWORD_OF_JUSTICE					0x4440

#define	SKILL_SOUND_SKILL_HALL_OF_VALHALLA					0x4510
#define	SKILL_SOUND_SKILL_EXTREM_RAGE1						0x4521
#define	SKILL_SOUND_SKILL_EXTREM_RAGE2						0x4522
#define	SKILL_SOUND_SKILL_EXTREM_RAGE3						0x4523
#define	SKILL_SOUND_SKILL_FROST_JAVELIN						0x4530
#define	SKILL_SOUND_SKILL_VENGEANCE1						0x4541
#define	SKILL_SOUND_SKILL_VENGEANCE2						0x4542

#define	SKILL_SOUND_SKILL_GLACIAL_SPIKE1					0x4611
#define	SKILL_SOUND_SKILL_GLACIAL_SPIKE2					0x4612
#define	SKILL_SOUND_SKILL_REGEN_FIELD1						0x4621
#define	SKILL_SOUND_SKILL_REGEN_FIELD2						0x4622
#define	SKILL_SOUND_SKILL_CHAIN_LIGHTNING1					0x4631
#define	SKILL_SOUND_SKILL_CHAIN_LIGHTNING2					0x4632
#define	SKILL_SOUND_SKILL_MUSPELL1							0x4641
#define	SKILL_SOUND_SKILL_MUSPELL2							0x4642

#define	SKILL_SOUND_SKILL_FIRE_ELEMENTAL					0x4710
#define	SKILL_SOUND_SKILL_FLAME_WAVE1						0x4721
#define	SKILL_SOUND_SKILL_FLAME_WAVE2						0x4722
#define	SKILL_SOUND_SKILL_FLAME_WAVE3						0x4723
#define	SKILL_SOUND_SKILL_DISTORTION						0x4730
#define	SKILL_SOUND_SKILL_METEO1							0x4741
#define	SKILL_SOUND_SKILL_METEO2							0x4742
#define	SKILL_SOUND_SKILL_METEO3							0x4743


#define SKILL_SOUND_STINGGER								0x4800 //Stinger 01.wav
#define SKILL_SOUND_R_HIT									0x4801 //RunningHit 01.wav
#define SKILL_SOUND_WISP									0x4802 //Wisp 01.wav
#define SKILL_SOUND_V_THRONE								0x4803 //VenomThorn 01.wav
#define SKILL_SOUND_ALAS									0x4804 //Alas 01.wav
#define SKILL_SOUND_S_SHOCK									0x4805 //SoulShork 01.wav
#define SKILL_SOUND_S_SWORD									0x4806 //SoreSword 01.wav
#define SKILL_SOUND_B_UP									0x4807 //BeatUp 01.wav
#define SKILL_SOUND_INPES									0x4808 //Infes 01.wav
#define SKILL_SOUND_BLIND									0x4809 //Blind 01.wav
#define SKILL_SOUND_F_WIND									0x4810 //FrostWind 01.wav
#define SKILL_SOUND_POLLUTED								0x4811 //Polluted 01.wav
#define SKILL_SOUND_P_SHADOW								0x4812 //FastingShadow 01.wav
#define SKILL_SOUND_S_BOMB									0x4813 //ShadowBomb 01.wav
#define SKILL_SOUND_R_SLASH									0x4814 //RisingSlash 01.wav
#define SKILL_SOUND_V_STAB									0x4815 //ViolenceStep 01.wav
#define SKILL_SOUND_STORM									0x4816 //Storm 01.wav

#define SKILL_SOUND_DARK_BOLT1								0x4820 //Darkbolt01.wav
#define SKILL_SOUND_DARK_BOLT2								0x4821 //Darkbolt02.wav
#define SKILL_SOUND_DARK_WAVE1								0x4822 //Darkwave01.wav
#define SKILL_SOUND_DARK_WAVE2								0x4823 //Darkwave02.wav
#define SKILL_SOUND_CURSE_LAZY1								0x4825 //Curselazy01.wav
#define SKILL_SOUND_CURSE_LAZY2								0x4826 //Curselazy02.wav
#define SKILL_SOUND_S_FLARE1								0x4827 //SFlare01.wav
#define SKILL_SOUND_S_FLARE2								0x4828 //SFlare02.wav
#define SKILL_SOUND_S_MANACLE1								0x4829 //SManacle01.wav
#define SKILL_SOUND_S_MANACLE2								0x482A //SManacle02.wav
#define SKILL_SOUND_C_HUNT1									0x482B //CHunt01.wav
#define SKILL_SOUND_C_HUNT2									0x482C //CHunt02.wav
#define SKILL_SOUND_A_MICHAL1								0x482D //AMichal01.wav
#define SKILL_SOUND_A_MICHAL2								0x482E //AMichal02.wav
#define SKILL_SOUND_RAINMAKER1								0x482F //Rainmaker01.wav
#define SKILL_SOUND_RAINMAKER2								0x4830 //Rainmaker02.wav
#define SKILL_SOUND_L_O_GHOST1								0x4831 //LOGhost01.wav
#define SKILL_SOUND_L_O_GHOST2								0x4832 //LOGhost02.wav
#define SKILL_SOUND_CURSE_HAUNT1							0x4833 //Haunt01.wav
#define SKILL_SOUND_CURSE_HAUNT2							0x4834 //Haunt02.wav
#define SKILL_SOUND_S_SCRATCH1								0x4835 //Scratch01.wav
#define SKILL_SOUND_S_SCRATCH2								0x4836 //Scratch02.wav
#define SKILL_SOUND_BLOODY_KNIGHT1							0x4837 //BloodyKinght01.wav
#define SKILL_SOUND_BLOODY_KNIGHT2							0x4838 //BloodyKinght02.wav
#define SKILL_SOUND_JUDGEMENT1								0x4839 //Judgement01.wav
#define SKILL_SOUND_JUDGEMENT2								0x4840 //Judgement02.wav
#define SKILL_SOUND_A_MIDRANDA1								0x4841 //AMidranda01.wav
#define SKILL_SOUND_A_MIDRANDA2								0x4842 //AMidranda02.wav
#define SKILL_SOUND_M_O_PRAY1								0x4843 //MOPray01.wav
#define SKILL_SOUND_M_O_PRAY2								0x4844 //MOPray02.wav
#define SKILL_SOUND_CREED									0x4845 //Creed01.wav
#define SKILL_SOUND_PO_DEITY								0x4846 //PODeity01.wav
#define SKILL_SOUND_G_NAIL1									0x4847 //GNail01.wav
#define SKILL_SOUND_G_NAIL2									0x4848 //GNail02.wav

#define SKILL_SOUND_LEGENDARY_DROP							0x4950 //


#define NEWSKILLAP_FS  (1 << 8)
#define NEWSKILLAP_MS  (2 << 8)
#define NEWSKILLAP_AS  (3 << 8)
#define NEWSKILLAP_PS  (4 << 8)
#define NEWSKILLAP_ATA (5 << 8)
#define NEWSKILLAP_KS  (6 << 8)
#define NEWSKILLAP_MGS (7 << 8)
#define NEWSKILLAP_PRS (8 << 8)
#define NEWSKILLAP_ASS (9 << 8)
#define NEWSKILLAP_SHA (10 << 8)


enum
{
	MUSPEL_NONE,
	MUSPEL_SPAWN,
	MUSPEL_IDLE,
	MUSPEL_ATTACK,
	MUSPEL_DEFENCE,
	MUSPEL_WALK,
	MUSPEL_DEAD = 6
};

enum PikemanSkill
{
	DefaultPikemanAttack = 0x0,

	// Tier 1 -----------------
	PikeWind = 0x1A,
	PreciseHit = 0x1B, //was CriticalHit
	JumpingCrash = 0x1C,
	// ------------------------
	// Tier 2 -----------------
	GroundPike = 0x2A,
	Tornado = 0x2B,
	Expansion = 0x2C,
	// ------------------------
	// Tier 3 -----------------
	VenomSpear = 0x3A,
	Vanish = 0x3B,
	ChainLance = 0x3C,
	// ------------------------
	// Tier 4 -----------------
	AssassinsEye = 0x4A,
	ChargingStrike = 0x4B,
	Vague = 0x4C,
	ShadowMaster = 0x4D,
	// ------------------------
	// Tier 5 -----------------
	RingOfSpears = 0x5A,
};

enum AssassinSkill
{
	DefaultAssassinAttack = 0x0,
	// Tier 1 -----------------
	Stinger = 0x1A,
	RunningHit = 0x1B,
	Wisp = 0x1D,
	// ------------------------
	// Tier 2 -----------------
	VenomThorn = 0x2A,
	Alas = 0x2B,
	SoulShock = 0x2C,
	AttackMastery = 0x2D,
	// ------------------------
	// Tier 3 -----------------
	SoreBlade = 0x3A,
	GustSlash = 0x3B,
	Inpes = 0x3C,
	Blind = 0x3D,
	// ------------------------
	// Tier 4 -----------------
	Cripple = 0x4A,
	Polluted = 0x4C,
	NinjaShadow = 0x4D,
	// ------------------------
	// Tier 5 -----------------
	ShadowBlink = 0x5A,
	RisingSlash = 0x5B,
	ViolentStub = 0x5C,
	ShadowStorm = 0x5D,
	// ------------------------
};

enum MechanicanSkill
{
	// Tier 1 -----------------
	DefaultMechAttack = 0x0,
	ExtremeShield = 0x1A,
	MechanicBomb = 0x1B,
	PhysicalAbsorption = 0x1C,
	ElectricalOverload = 0x1D,
	// ------------------------
	// Tier 2 -----------------
	GreatSmash = 0x2A,
	Maximize = 0x2B,
	Automation = 0x2C,
	Spark = 0x2D,
	// ------------------------
	// Tier 3 -----------------
	MetalArmor = 0x3A,
	GrandSmash = 0x3B,
	SparkShield = 0x3C,
	// ------------------------
	// Tier 4 -----------------
	Impulsion = 0x4A,
	Compulsion = 0x4B,
	MagneticSphere = 0x4C,
	MetalGolem = 0x4D,
	// ------------------------
	// Tier 5 -----------------
	Rupture = 0x5A,
};


enum FighterSkill
{
	DefaultFighterAttack = 0x0,
	// Tier 1 -----------------
	Raving = 0x1A,
	DoubleImpact = 0x1B,
	// ------------------------
	// Tier 2 -----------------
	TripleImpact = 0x2A,
	BrutalSwing = 0x2B,
	WarCry = 0x2C,				//was Roar
	RageOfZecram = 0x2D,
	// ------------------------
	// Tier 3 -----------------
	DemonsBane = 0x3A,		//was Concentration
	AvengingCrash = 0x3B,
	SwiftAxe = 0x3C,			//Swift axe + Concentration
	BoneCrash = 0x3D,
	// ------------------------
	// Tier 4 -----------------
	Destroyer = 0x4A,
	Berserker = 0x4B,
	CycloneStrike = 0x4C,
	// ------------------------
	// Tier 5 -----------------
	SesmicImpact = 0x5A,
};


enum ArcherSkill
{
	DefaultArcherAttack = 0x0,

	// Tier 1 -----------------
	ScoutHawk = 0x1A,
	WindArrow = 0x1B,
	PerfectAim = 0x1C,

	// ------------------------
	// Tier 2 -----------------
	Falcon = 0x2A,
	ArrowofRage = 0x2B,
	Avalanche = 0x2C,

	// ------------------------
	// Tier 3 -----------------
	ElementalShot = 0x3A,
	GoldenFalcon = 0x3B,
	BombShot = 0x3C,
	Perforation = 0x3D,

	// ------------------------
	// Tier 4 -----------------
	RecallWolverine = 0x4A,
	PhoenixShot = 0x4B,
	ForceofNature = 0x4C,

	// ------------------------
	// Tier 5 -----------------
	FierceWind = 0x5A,
};

enum KnightSkill
{
	// Tier 1 -----------------
	DefaultKnightAttack = 0x0,
	SwordBlast = 0x1A,
	HolyBody = 0x1B,
	DoubleCrash = 0x1C,
	// ------------------------
	// Tier 2 -----------------
	HolyValor = 0x2A,
	Brandish = 0x2B,
	Piercing = 0x2C,
	DrasticSpirit = 0x2D,
	// ------------------------
	// Tier 3 -----------------
	DivineShield = 0x3A,
	HolyIncantation = 0x3B,
	GrandCross = 0x3C,
	// ------------------------
	// Tier 4 -----------------
	SwordofJustice = 0x4A,
	GodlyShield = 0x4B,
	GodsBlessing = 0x4C,
	DivinePiercing = 0x4D,
	// ------------------------
	// Tier 5 -----------------
	HolyConviction = 0x5A,
};

enum AtalantaSkill
{
	// Tier 1 -----------------
	AtalantaDefaultAttack = 0x0,
	ShieldStrike = 0x1A,
	Farina = 0x1B,
	BiggerSpear = 0x1C,
	// ------------------------
	// Tier 2 -----------------
	Windy = 0x2A,
	WindJavelin = 0x2B,
	SoulSucker = 0x2C,
	FireJavelin = 0x2D,
	// ------------------------
	// Tier 3 -----------------
	SplitJavelin = 0x3A,
	TriumphofValhalla = 0x3B,
	LightningJavelin = 0x3C,
	StormJavelin = 0x3D,
	// ------------------------
	// Tier 4 -----------------
	HallofValhalla = 0x4A,
	ExtremeRage = 0x4B,
	FrostJavelin = 0x4C,
	Vengeance = 0x4D,
	// ------------------------
	// Tier 5 -----------------
	SummonTiger = 0x5A,
};

enum MagicianSkill
{
	DefaultMagicianAttack = 0x0,

	// Tier 1 -----------------
	FireBolt = 0x1B,
	//Zenith = 0x1C,
	FireBall = 0x1D,
	// DoubleCrash = 0x1C,
	// ------------------------
	// Tier 2 -----------------
	Watornado = 0x2A,
	EnchantWeapon = 0x2B,
	DeathRay = 0x2C,
	// ------------------------
	// Tier 3 -----------------
	EnergyShield = 0x3A,
	Diastrophism = 0x3B,
	SpiritElemental = 0x3C,
	DancingSword = 0x3D,
	// ------------------------
	// Tier 4 -----------------
	FireElemental = 0x4A,
	FlameWave = 0x4B,
	Distortion = 0x4C,
	Meteorite = 0x4D,
	// ------------------------
	// Tier 5 -----------------
	WizardTrance = 0x5A,
};

enum PriestessSkill
{
	PriestessDefaultAttack = 0x0,

	// Tier 1 -----------------
	Healing = 0x1A,
	HolyBolt = 0x1B,
	MultiSpark = 0x1C,
	HolyMind = 0x1D,

	// ------------------------
	// Tier 2 -----------------
	//DiviningLightning = 0x2A,
	DivineLightning = 0x2A,
	HolyReflection = 0x2B,
	GrandHealing = 0x2C,

	// ------------------------
	// Tier 3 -----------------
	VigorBall = 0x3A,
	Resurrection = 0x3B,
	Extinction = 0x3C,
	VirtualLife = 0x3D,

	// ------------------------
	// Tier 4 -----------------
	GlacialSpike = 0x4A,
	RegenerationField = 0x4B,
	VirtualRegen = 0x4B,
	ChainLightning = 0x4C,
	SummonMuspell = 0x4D,

	// Tier 5 -----------------
	IceElemental = 0x5A
};

enum ShamanSkill
{
	ShamanDefaultAttack = 0x0,
	// Tier 1 -----------------
	DarkBolt = 0x1A,
	DarkWave = 0x1B,
	Inertia = 0x1C,
	InnerPeace = 0x1D,
	// ------------------------
	// Tier 2 -----------------
	SpiritualFlare = 0x2A,
	SpiritualManacle = 0x2B,
	ChasingHunt = 0x2C,
	AdventMigal = 0x2D,
	// ------------------------
	// Tier 3 -----------------
	Rainmaker = 0x3A,
	PhantomCall = 0x3B,
	Haunt = 0x3C,
	Scratch = 0x3D,
	// ------------------------
	// Tier 4 -----------------
	RecallHestian = 0x4A,
	Judgement = 0x4B,
	AdventMidranda = 0x4C,
	MourningPray = 0x4D,
	// ------------------------
};

enum ESkillArrayPointer
{
	//Archer
	//=========================================================
	//=========================================================

	//Scout Hawk
	Archer_ScoutHawk_AddAtkRtg_DoNotUse = 0x00993BB0,
	Archer_ScoutHawk_AddAtkRtg_New = NEWSKILLAP_AS | 1,
	Archer_ScoutHawk_Rotations = 0x00993BD8,
	Archer_ScoutHawk_UseMana = 0x00993C00,

	//Shooting Mastery
	Archer_ShootingMastery_AddWeaponDamagePercent = 0x00993C28,
	Archer_ShootingMastery_AddWeaponDamage = 0x00996B30,

	//Wind Arrow
	Archer_WindArrow_AddATKSpeed = 0x00993C50,
	Archer_WindArrow_DMGBoost = 0x00993C78,
	Archer_WindArrow_UseMana = 0x00993CA0,

	//Perfect Aim
	Archer_PerfectAim_AddAtkRtg = 0x00993CC8,
	Archer_PerfectAim_DMGBoost = 0x00993CF0,
	Archer_PerfectAim_UseMana = 0x00993D40,


	//Dion's Eye
	Archer_DionsEye_AddAtkRtg = 0x00993D68,

	//Falcon
	Archer_Falcon_Damage = 0x04B0CAB0,
	Archer_Falcon_DamagePercent = 0x00993D90,
	Archer_Falcon_Duration = 0x00993DE0,
	Archer_Falcon_UseMana = 0x00993E08,

	//Arrow of Rage
	Archer_ArrowofRage_DMGFlat = 0x00995CA8,
	Archer_ArrowofRage_DMGBoost = 0x00993E58,
	Archer_ArrowofRage_Area = 0x00993E80,
	Archer_ArrowofRage_UseMana = 0x00993EA8,

	//Avalanche
	Archer_Avalanche_AddATKSpeed = 0x00993ED0,
	Archer_Avalanche_DMGBoost = 0x00993EF8,
	Archer_Avalanche_NumberofArrows = 0x00993F20,
	Archer_Avalanche_UseMana = 0x00993F48,


	//Elemental Shot
	Archer_ElementalShot_FireDamage = 0x04B0CAE0,
	Archer_ElementalShot_FireDamagePercent = 0x00993FC0,
	Archer_ElementalShot_LightningDamage = 0x009962C4,
	Archer_ElementalShot_LightningDamagePercent = 0x00996270,
	Archer_ElementalShot_UseMana = 0x00994060,
	Archer_ElementalShot_LightningRange_New = NEWSKILLAP_AS | 2,

	//Falcon
	Archer_GoldenFalcon_FalconDamage = 0x00994088,
	Archer_GoldenFalcon_Duration = 0x009940D8,
	Archer_GoldenFalcon_HPRegen = 0x009940B0, //float
	Archer_GoldenFalcon_UseMana = 0x00994100,

	//Bomb Shot
	Archer_BombShot_Damage = 0x00994128,
	Archer_BombShot_DMGDemons = 0x00994150,
	Archer_BombShot_Area = 0x009941C8,
	Archer_BombShot_UseMana = 0x009941F0,

	//Perforation
	Archer_Perforation_AddShotRange = 0x00994290,
	Archer_Perforation_DMGBoost = 0x00994218,
	Archer_Perforation_UseMana = 0x00994268,
	Archer_Perforation_AddedCritical_New = NEWSKILLAP_AS | 3, //unique id for AS (3)
	Archer_Perforation_AttackWidth_New = NEWSKILLAP_AS | 4, //unique id for AS (3)


	//Wolverine
	Archer_Wolverine_ATKPowMin = 0x009942B8,			//first half
	Archer_Wolverine_ATKPowMax = 0x009942B8 + (10 * 4), //second half
	Archer_Wolverine_AtkRating = 0x00994308,
	Archer_Wolverine_Defense = 0x00994330,
	Archer_Wolverine_HP = 0x00994358,
	Archer_Wolverine_UseMana = 0x00994380,
	Archer_Wolverine_Duration = 0x0099617C,

	//Evasion Mastery
	Archer_EvasionMastery_IncreaseEvasion_DoNotUse = 0x009943A8,
	Archer_EvasionMastery_IncreaseEvasion_New = NEWSKILLAP_AS | 5,

	//Phoenix Shot
	Archer_PhoenixShot_AddedRange = 0x00994448,
	Archer_PhoenixShot_DMGBoost = 0x009943D0,
	Archer_PhoenixShot_AttackRange_New = NEWSKILLAP_AS | 6,
	Archer_PhoenixShot_AttackWidth_New = NEWSKILLAP_AS | 7,
	Archer_PhoenixShot_ChargingSpeed = 0x009943F8,
	Archer_PhoenixShot_ChargedDMGBoost_New = NEWSKILLAP_AS | 8,
	Archer_PhoenixShot_BurningChance_New = NEWSKILLAP_AS | 11,
	Archer_PhoenixShot_BurningPercentOfAP_New = NEWSKILLAP_AS | 12,
	Archer_PhoenixShot_BurningDurationSec_New = NEWSKILLAP_AS | 13,
	Archer_PhoenixShot_UseMana = 0x00994420,

	//Force of Nature (group buff)
	Archer_ForceofNature_AddATKPow_DoNotUse = 0x00994470,
	Archer_ForceofNature_AddATKPow_New = NEWSKILLAP_AS | 9,
	Archer_ForceofNature_AddAtkRtg = 0x00994498,
	Archer_ForceofNature_Duration = 0x009944E8,
	Archer_ForceofNature_UseMana = 0x00994510,
	Archer_ForceofNature_AddedRange_New = NEWSKILLAP_AS | 10, //unique id for AS (3)



	//Mechananic
	//=========================================================
	//=========================================================
	Mech_ExtremeShield_DoNotUse = 0x00992698,
	Mech_ExtremeShield_AddBlock_New = NEWSKILLAP_MS | 1,
	Mech_ExtremeShield_Duration = 0x009926C0,
	Mech_ExtremeShield_UseMana = 0x009926E8,

	Mech_MechanicBomb_Damage = 0x04B0CB10,
	Mech_MechanicBomb_DamagePercent = 0x00992710,
	Mech_MechanicBomb_Area = 0x00992760,
	Mech_MechanicBomb_AttackRange_New = NEWSKILLAP_MS | 2,
	Mech_MechanicBomb_AttackFrameSpeed_New = NEWSKILLAP_MS | 3,
	Mech_MechanicBomb_BurningChance = NEWSKILLAP_MS | 4,
	Mech_MechanicBomb_BurningDamagePercent  = NEWSKILLAP_MS | 5,
	Mech_MechanicBomb_BurningDurationSec = NEWSKILLAP_MS| 6,
	Mech_MechanicBomb_BurningTickRateMs = NEWSKILLAP_MS | 7,
	Mech_MechanicBomb_UseMana = 0x00992788,

	Mech_PoisonAttribute_Poison_DoNotUse = 0x00992850,
	Mech_ElectricalOverload_AttackSpeed = NEWSKILLAP_MS | 8,
	Mech_ElectricalOverload_Duration = NEWSKILLAP_MS | 9,

	Mech_PhysicalAbsorption_ABSBoost = 0x009927B0,
	Mech_PhysicalAbsorption_Duration = 0x00992800,
	Mech_PhysicalAbsorption_UseMana = 0x00992828,


	Mech_GreatSmash_AddDamage = 0x009928A0,
	Mech_GreatSmash_AddDamagePercent = 0x009964A0, //was Area
	Mech_GreatSmash_UseMana = 0x009928C8,

	Mech_Maximize_AddATKPow = 0x009928F0,
	Mech_Maximize_AtkRtgBoost = 0x04B0C034,
	Mech_Maximize_ATKSPDBoost_New = NEWSKILLAP_MS | 10,
	Mech_Maximize_Duration = 0x00992918,
	Mech_Maximize_UseMana = 0x00992940,

	Mech_Automation_DMGBoost = 0x00992968,
	Mech_Automation_AddATKSpeed = 0x00992990,
	Mech_Automation_MPDrain_New = NEWSKILLAP_MS | 11,
	Mech_Automation_SPDrain_New = NEWSKILLAP_MS | 12,
	Mech_Automation_Duration = 0x009929B8,

	Mech_Spark_DMGBoostFlat = 0x00992A08,
	Mech_Spark_DMGBoostPercent = 0x00995D98,
	Mech_Spark_Area = 0x00992A58,
	Mech_Spark_UseMana = 0x00992A80,
	Mech_Spark_AttackFrameSpeed_New = NEWSKILLAP_MS | 17,


	Mech_MetalArmor_AddDefense = 0x00992AA8,
	Mech_MetalArmor_Duration = 0x00992AD0,
	Mech_MetalArmor_UseMana = 0x00992AF8,

	Mech_GrandSmash_AtkRtgBoost = 0x00992B20,
	Mech_GrandSmash_DMGBoost = 0x00992B48,
	Mech_GrandSmash_UseMana = 0x00992B70,

	Mech_MechanicWeaponMastery_Obsolete = 0x00992B98, //do not use - broken. will be set to zero
	Mech_MechanicWeaponMastery_AddWeaponDamage_New = NEWSKILLAP_MS | 13,
	Mech_MechanicWeaponMastery_AddWeaponDamagePercent = 0x00996928,

	Mech_SparkShield_SparkDamage = 0x00992BC0,
	Mech_SparkShield_AddDefense = 0x00992BE8,
	Mech_SparkShield_Duration = 0x00992C10,
	Mech_SparkShield_UseMana = 0x00992C38,


	Mech_Impulsion_DMGBoost = 0x00992C60,
	Mech_Impulsion_Area = 0x00992CB0,
	Mech_Impulsion_MaxVolts = 0x00992CD8,
	Mech_Impulsion_UseMana = 0x00992D00,

	Mech_Compulsion_ExtraAbsorb_New = NEWSKILLAP_MS | 14,	//unique id for MS (2)
	Mech_Compulsion_AddAbsorbPercent_Old = 0x00992D28,  //not used. zeroed out to disable ASM
	Mech_Compulsion_AddAbsorbPercent_New = NEWSKILLAP_MS | 15,
	Mech_Compulsion_Range = 0x00992D50,
	Mech_Compulsion_Duration = 0x00992D78,
	Mech_Compulsion_UseMana = 0x00992DA0,

	Mech_MagneticSphere_SpheresDMG = 0x00992DC8,
	Mech_MagneticSphere_Range = 0x00992E18,
	Mech_MagneticSphere_AtkDelay = 0x00992E40,
	Mech_MagneticSphere_Duration = 0x00992E68,
	Mech_MagneticSphere_UseMana = 0x00992E90,

	Mech_MetalGolem_ATKPowMin = 0x00992EB8,
	Mech_MetalGolem_ATKPowMax = 0x00992EB8, //2nd half of 2D int array
	Mech_MetalGolem_AtkRating = 0x00992F08,
	Mech_MetalGolem_Defense = 0x00992F30,
	Mech_MetalGolem_HP = 0x00992F58,
	Mech_MetalGolem_UseMana = 0x00992F80,
	Mech_MetalGolem_Duration_New = NEWSKILLAP_MS | 16,



	//Fighter
	//=========================================================
	//=========================================================

	//Melee Mastery
	Fighter_MeleeMastery_WPAtkPowAdd_Obsolete = 0x00995230, //broken do not use
	Fighter_MeleeMastery_WPAtkPowAdd_New = 0x00998868,
	Fighter_MeleeMastery_WPAtkPowAddPercent_New = 0x009987F0,

	//Fire Attribute
	Fighter_FireAttributeOld_Flame_Obsolete  = 0x00995258, //don't use - set to zero

	//Recuperation
	Fighter_Recuperation_HPRegen_New  = NEWSKILLAP_FS | 1,

	//Raving
	Fighter_Raving_DMGBoost = 0x00995280,
	Fighter_Raving_Numofhits = 0x009952A8,
	Fighter_Raving_HPCost_DoNotUse = 0x009952D0, //float
	Fighter_Raving_HPCost_New = NEWSKILLAP_FS | 2, //int
	Fighter_Raving_UseMana = 0x009952F8,

	//DoubleImpact
	Fighter_DoubleImpact_AddAtkRtg = 0x00995320,
	Fighter_DoubleImpact_DMGBoost = 0x00995348,
	Fighter_DoubleImpact_UseMana = 0x00995370,

	//Triple DoubleImpact
	Fighter_TripleImpact_DMGBoost = 0x00995398,
	Fighter_TripleImpact_Numofhits = 0x009953C0,
	Fighter_TripleImpact_UseMana = 0x009953E8,

	//Brutal Swing
	Fighter_BrutalSwing_DMGBoost = 0x00995410,
	Fighter_BrutalSwing_HPCost_New = NEWSKILLAP_FS | 3,
	Fighter_BrutalSwing_AddCritical = 0x00995438,
	Fighter_BrutalSwing_CriticalDmgBoost_New = NEWSKILLAP_FS | 4,
	Fighter_BrutalSwing_UseMana = 0x00995460,

	//WarCry
	Fighter_WarCry_StunTime = 0x009954B0,
	Fighter_WarCry_StunChance_New = NEWSKILLAP_FS | 5,
	Fighter_WarCry_StunAtkPowPerc_New = NEWSKILLAP_FS | 6,
	Fighter_WarCry_HPRestore_New = NEWSKILLAP_FS | 7,
	Fighter_WarCry_SPRestore_New = NEWSKILLAP_FS | 8,
	Fighter_WarCry_HPRegen_New = NEWSKILLAP_FS | 9,
	Fighter_WarCry_SPRegen_New = NEWSKILLAP_FS | 10,
	Fighter_WarCry_RegenDuration_Sec = NEWSKILLAP_FS | 11,
	Fighter_WarCry_Radius = 0x00995488,
	Fighter_WarCry_UseMana = 0x009954D8,

	//Rage of Zecram
	Fighter_RageofZecram_Damage = 0x00995500,
	Fighter_RageofZecram_UseMana = 0x00995528,
	Fighter_RageofZecram_AttackRadius_New = NEWSKILLAP_FS | 12,
	Fighter_RageofZecram_BurningDamagePercent_New = NEWSKILLAP_FS | 13,
	Fighter_RageofZecram_BurningChance_New = NEWSKILLAP_FS | 14,
	Fighter_RageofZecram_BurningDurationSec_New = NEWSKILLAP_FS | 15,

	Fighter_ConcentrationOld_AtkRtgBoost_Obsolete = 0x00995578, //don't use. set to 0

	Fighter_DemonBuff_Duration = 0x00995550,
	Fighter_DemonBuff_UseMana = 0x009955A0,
	Fighter_DemonBuff_DamageBoostVsDemon_New = NEWSKILLAP_FS | 16,

	//Avenging Crash
	Fighter_AvengingCrash_AddAtkRtg = 0x009955F0, //% based
	Fighter_AvengingCrash_HPCost_New = NEWSKILLAP_FS | 17,
	Fighter_AvengingCrash_DMGBoost = 0x009955C8,
	Fighter_AvengingCrash_UseMana = 0x00995618,

	//Concentration + SwiftAxe
	Fighter_SwiftAxe_ATKSpdBoost_DoNotUse = 0x00995640,
	Fighter_Concentration_AtkRtgBoost_DoNotUse = 0x00995640,

	Fighter_ConcentrationSA_ATKRtgBoost_New = NEWSKILLAP_FS | 23,
	Fighter_ConcentrationSA_ATKSpdBoost_New = NEWSKILLAP_FS | 18,
	Fighter_ConcentrationSA_Duration = 0x00995668,
	Fighter_ConcentrationSA_UseMana = 0x00995690,

	//BoneCrash
	Fighter_BoneCrash_DMGBoost = 0x009956B8,
	Fighter_BoneCrash_BonusDMGDemons = 0x009956E0,
	Fighter_BoneCrash_UseMana = 0x00995708,

	//Destroyer
	Fighter_Destroyer_DMGBoost = 0x00995730,
	Fighter_Destroyer_HPCost_New = NEWSKILLAP_FS | 19,
	Fighter_Destroyer_AddCritical = 0x00995758,
	Fighter_Destroyer_UseMana = 0x00995780,

	//Berserker
	Fighter_Berserker_AddATKPow_DoNotUse = 0x009957A8,
	Fighter_Berserker_AddATKPow_New = NEWSKILLAP_FS | 24,
	Fighter_Berserker_DecreaseAbs = 0x009957D0,
	Fighter_Berserker_Duration = 0x009957F8,
	Fighter_Berserker_LifeLeechPercent_New = NEWSKILLAP_FS | 20,
	Fighter_Berserker_UseMana = 0x00995820,

	//Cyclone Strike
	Fighter_CycloneStrike_DMGBoost = 0x00995848,
	Fighter_CycloneStrike_Maxhits = 0x009958C0,
	Fighter_CycloneStrike_Range = 0x009958E8,
	Fighter_CycloneStrike_UseMana = 0x00995910,

	//Boost Health
	Fighter_BoostHealth_HPBoost = 0x00995938,
	Fighter_BoostHealth_HPBoostPercent = NEWSKILLAP_FS | 21 ,
	Fighter_BoostHealth_HPPotionBoost = NEWSKILLAP_FS | 22 ,


	//Pikeman
	//=========================================================
	//=========================================================
	//Pike Wind
	Pikeman_PikeWind_ATKPowMin = 0x009974E0,
	Pikeman_PikeWind_ATKPowMax = 0x009974E0, //2nd half of 2D int array
	Pikeman_PikeWind_Pushback = 0x00997530,
	Pikeman_PikeWind_UseMana = 0x00997558,

	//Ice Attribute
	Pikeman_IceAttribute_Frost = 0x00997580,

	//Polearm Mastery
	Pikeman_PolearmMastery_AddRange_New = NEWSKILLAP_PS | 1,

	//Critical Hit
	Pikeman_PreciseHit_CritDmg_DoNotUse = 0x009975A8,
	Pikeman_PreciseHit_ATKPow_New = NEWSKILLAP_PS | 2,
	Pikeman_PreciseHit_AddAttackRating_New = NEWSKILLAP_PS | 3,
	Pikeman_PreciseHit_UseMana = 0x009975D0,

	//Jumping Crash
	Pikeman_JumpingCrash_WPAtkRtgBoost = 0x00997620,
	Pikeman_JumpingCrash_DMGBoost = 0x00997648,
	Pikeman_JumpingCrash_UseMana = 0x009975F8,

	//Ground Pike
	Pikeman_GroundPike_FreezeChance = 0x00997670,
	Pikeman_GroundPike_FreezeDamage = 0x00997670, //2nd half of 2D int array
	Pikeman_GroundPike_SpeedMonDown_New = NEWSKILLAP_PS | 4,
	Pikeman_GroundPike_FreezeDuration = 0x00997710,
	Pikeman_GroundPike_Range = 0x009976C0,
	Pikeman_GroundPike_UseMana = 0x009976E8,

	//Tornado
	Pikeman_Tornado_DMGBoost = 0x00997738,
	Pikeman_Tornado_Range = 0x00997788,
	Pikeman_Tornado_UseMana = 0x009977B0,

	//Weapon Block Mastery
	Pikeman_WeaponBlockMastery_WPBlockBoost_DoNotUse = 0x009977D8,
	Pikeman_WeaponBlockMastery_WPBlockBoost_New = NEWSKILLAP_PS | 11,

	//Expansion
	Pikeman_Expansion_AddSize = 0x00997800,
	Pikeman_Expansion_DMGBoost = 0x00997828,
	Pikeman_Expansion_UseMana = 0x00997850,
	Pikeman_Expansion_Range_New = NEWSKILLAP_PS | 5,

	//Venom Spear
	Pikeman_VenomSpear_DMGBoost = 0x00997878,
	Pikeman_VenomSpear_Numberofspears = 0x009978C8,
	Pikeman_VenomSpear_Duration_NotUsed = 0x009978F0,
	Pikeman_VenomSpear_Range = 0x00997918,
	Pikeman_VenomSpear_UseMana = 0x00997940,

	Pikeman_VenomSpear_PoisonChance_New				= NEWSKILLAP_PS | 6,
	Pikeman_VenomSpear_PoisonDamagePercent_New		= NEWSKILLAP_PS | 7,
	Pikeman_VenomSpear_PoisonDurationSec_New		= NEWSKILLAP_PS | 8,
	Pikeman_VenomSpear_PoisonNumStack_New			= NEWSKILLAP_PS | 9,


	//Vanish
	Pikeman_Vanish_AfterDamage = 0x00997990,
	Pikeman_Vanish_MonsterSight = 0x009979B8,
	Pikeman_Vanish_Duration = 0x009979E0,
	Pikeman_Vanish_UseMana = 0x00997A08,

	//Critical Mastery
	Pikeman_CriticalMastery_WPCritBoost = 0x00997A30,

	//Chain Lancer
	Pikeman_ChainLance_DMGBoost = 0x00997A58,
	Pikeman_ChainLance_UseMana = 0x00997A80,

	//Assassin's Eye
	Pikeman_AssassinsEye_AddCriticalDmg = 0x00997AA8,
	Pikeman_AssassinsEye_AddCriticalChance = 0x00997AD0,
	Pikeman_AssassinsEye_Duration = 0x0997AF8,
	Pikeman_AssassinsEye_UseMana = 0x00997B20,

	//Charging Strike
	Pikeman_ChargingStrike_DMGBoost = 0x00997B48,
	Pikeman_ChargingStrike_ChargedDMGBst = 0x00997B70,
	Pikeman_ChargingStrike_RaiseSpeed = 0x00997B98,
	Pikeman_ChargingStrike_UseMana = 0x00997BC0,

	//Vague
	Pikeman_Vague_AddEvasion_DoNotUse = 0x00997BE8, //disable Asm
	Pikeman_Vague_AddEvasion_New = NEWSKILLAP_PS | 10,
	Pikeman_Vague_Duration = 0x00997C10,
	Pikeman_Vague_UseMana = 0x00997C38,

	//Shadow Master
	Pikeman_ShadowMaster_DMGBoost = 0x00997C60,
	Pikeman_ShadowMaster_Numofhits = 0x00997CB0,
	Pikeman_ShadowMaster_UseMana = 0x00997CD8,



	//Assassin
	//=========================================================
	//=========================================================

	//Stinger
	Assassin_Stinger_DMGBoost = 0x00997F80,
	Assassin_Stinger_UseMana = 0x00997F58,

	//Running Hit
	Assassin_RunningHit_DMGBoost = 0x00997FF8,
	Assassin_RunningHit_UseMana = 0x00997FD0,

	//Dual Wield Mastery
	Assassin_DualWieldMastery_WPAtkRtgBoost_DoNotUse = 0x00998228, //set to 0 to disable in asm

	Assassin_DualWieldMastery_WPAtkRtgBoost_New = NEWSKILLAP_ASS | 1,
	Assassin_DualWieldMastery_WPAtkPowBoost = 0x00998020,
	Assassin_DualWieldMastery_WPAtkPowBoostFlat_New = NEWSKILLAP_ASS | 2,

	//Wisp
	Assassin_Wisp_Curse_DamageReduction = 0x00998098,
	Assassin_Wisp_Curse_PercentOfAP_New = NEWSKILLAP_ASS | 3,
	Assassin_Wisp_Curse_Chance_New = NEWSKILLAP_ASS | 4,
	Assassin_Wisp_Curse_Duration_New = NEWSKILLAP_ASS | 5,
	Assassin_Wisp_Curse_SpeedReduction_New = NEWSKILLAP_ASS | 6,
	Assassin_Wisp_Range = 0x00998070,
	Assassin_Wisp_UseMana = 0x00998048,

	//Venom Thorn
	Assassin_VenomThorn_DMGBoost_Obsolete = 0x009980E8,
	Assassin_VenomThorn_UseMana = 0x009980C0,
	Assassin_VenomThorn_Duration_New = NEWSKILLAP_ASS | 7,
	Assassin_VenomThorn_PoisonChance_New = NEWSKILLAP_ASS | 8,
	Assassin_VenomThorn_PoisonDamagePercent_New = NEWSKILLAP_ASS | 9,
	Assassin_VenomThorn_PoisonDurationSec_New = NEWSKILLAP_ASS | 10,
	Assassin_VenomThorn_PoisonNumStack_New = NEWSKILLAP_ASS | 11,

	//Alas
	Assassin_Alas_AddEvasion = 0x00998160,
	Assassin_Alas_Duration = 0x00998138,
	Assassin_Alas_UseMana = 0x00998110,

	//Soul Shock
	Assassin_SoulShock_Radius = 0x009981B0,
	Assassin_SoulShock_StunTime = 0x009981D8,
	Assassin_SoulShock_StunAtkPowPerc_New = NEWSKILLAP_ASS | 12,
	Assassin_SoulShock_StunChance_New = NEWSKILLAP_ASS | 13,
	Assassin_SoulShock_UseMana = 0x00998188,

	//Attack Mastery (old)
	Assassin_AttackMastery_WPBlockBoost_DoNotUse = 0x00998200,

	//Dodge Mastery
	Assassin_DodgeMastery_AddEvasive_New = NEWSKILLAP_ASS | 14,

	//Sore Blade
	Assassin_SoreBlade_DMGBoost = 0x00998278,
	Assassin_SoreBlade_UseMana = 0x00998250,

	//Gust Slash
	Assassin_GustSlash_Area = 0x04B07040,
	Assassin_GustSlash_DMGBoost = 0x009982C8,
	Assassin_GustSlash_UseMana = 0x009982A0,

	//Inpes
	Assassin_Inpes_ATKSPDBoost_DoNotuse = 0x00998318,
	Assassin_Inpes_MovementSpeed_New = NEWSKILLAP_ASS | 15,
	Assassin_Inpes_Duration = 0x00998340,
	Assassin_Inpes_UseMana = 0x009982F0,

	//Blind
	Assassin_Blind_AfterDamage_Obosolete = 0x009983B8, //can be reused for anything else
	Assassin_Blind_MonsterSight = 0x009983E0,
	Assassin_Blind_Duration = 0x00998390,
	Assassin_Blind_UseMana = 0x00998368,

	//Cripple (was Frost Wind)
	Assassin_Cripple_DMGBoost = 0x00998430,
	Assassin_Cripple_UseMana = 0x00998408,
	Assassin_Cripple_PoisonDurationSec = 0x00998958,
	Assassin_Cripple_PoisonPercentOfAP_New = NEWSKILLAP_ASS | 16,
	Assassin_Cripple_PoisonChance_New = NEWSKILLAP_ASS | 17,
	Assassin_Cripple_PoisonNumStack_New = NEWSKILLAP_ASS | 18,
	Assassin_Cripple_SpeedMonDown_DoNotUse = 0x00998A20,


	//Fatal Mastery
	Assassin_AssassinMastery_WPCritBoost = 0x00998458,
	Assassin_AssassinMastery_AtkSpeedBoost_New = NEWSKILLAP_ASS | 19,

	//Polluted
	Assassin_Polluted_DMGBoost = 0x009984D0,
	Assassin_Polluted_PoisonPercentOfAP = 0x00994C68,
	Assassin_Polluted_PoisonChance_New = NEWSKILLAP_ASS | 20,
	Assassin_Polluted_MaxHits = 0x00998548,
	Assassin_Polluted_PoisonDurationSec = 0x00998520,
	Assassin_Polluted_Range = 0x009984A8,
	Assassin_Polluted_UseMana = 0x00998480,
	Assassin_Polluted_PoisonNumStack_New = NEWSKILLAP_ASS | 21,


	//Ninja Shadow
	Assassin_NinjaShadow_DMGBoost = 0x00998598,
	Assassin_NinjaShadow_UseMana = 0x00998570,





	//Atalanta
	//=========================================================
	//=========================================================

	//Shield Strike
	Atalanta_ShieldStrike_ATKPowMin = 0x009947E0,
	Atalanta_ShieldStrike_ATKPowMax = 0x009947E0 + (4 * 10), //2nd half of 2D int array
	Atalanta_ShieldStrike_UseMana = 0x009947B8,
	Atalanta_ShieldStrike_StunDamage = NEWSKILLAP_ATA | 1,
	Atalanta_ShieldStrike_StunChance = NEWSKILLAP_ATA | 2,
	Atalanta_ShieldStrike_StunDurationSec = NEWSKILLAP_ATA | 3,

	//Farina
	Atalanta_Farina_DMGBoost = 0x04B0B7F8,
	Atalanta_Farina_DMGBoostPercent_New = NEWSKILLAP_ATA | 4,
	Atalanta_Farina_AddATKSpeed = 0x00994880,
	Atalanta_Farina_AddAtkRtg = 0x009948A8,
	Atalanta_Farina_UseMana = 0x00994858,

	//Throwing Mastery
	Atalanta_ThrowingMastery_DMGBoost_Obsolete = 0x00994920, //do not use - broken. will be set to zero
	Atalanta_ThrowingMastery_DMGBoost_New  = NEWSKILLAP_ATA | 5,
	Atalanta_ThrowingMastery_DMGBoostPercent_New  = NEWSKILLAP_ATA | 6,

	//Bigger Spear
	Atalanta_BiggerSpear_DMGBoost = 0x00994920, //2d array, but only using first half
	Atalanta_BiggerSpear_UseMana = 0x009948F8,

	//Windy
	Atalanta_Windy_AtkRtgBoost_DoNotUse = 0x00994970,
	Atalanta_Windy_Duration = 0x00994998,
	Atalanta_Windy_UseMana = 0x009949C0,
	Atalanta_Windy_Range_New = NEWSKILLAP_ATA | 7,
	Atalanta_Windy_AddAttackRating_New = NEWSKILLAP_ATA | 8,

	//Twist Javelin
	Atalanta_WindJavelin_DMGBoost = 0x009949E8,
	Atalanta_WindJavelin_AtkRtgBoost = 0x00994A10,
	Atalanta_WindJavelin_UseMana = 0x00994A38,

	//Soul Sucker
	Atalanta_SoulSucker_LifeAbsorb = 0x00994A60,
	Atalanta_SoulSucker_Range_New = NEWSKILLAP_ATA | 22,
	Atalanta_SoulSucker_LifeAbsorbCap_New = NEWSKILLAP_ATA | 23,
	Atalanta_SoulSucker_UseMana = 0x00994A88,

	//Fire Javelin
	Atalanta_FireJavelin_DMGBoost = 0x00994920 + ( 10 * 4 ), //Offset from Bigger Spear unused half of 2d array
	Atalanta_FireJavelin_UseMana = 0x00994B00,
	Atalanta_FireJavelin_BurningChance = NEWSKILLAP_ATA | 9,
	Atalanta_FireJavelin_BurningDamagePercent = NEWSKILLAP_ATA | 10,
	Atalanta_FireJavelin_BurningDurationSec = NEWSKILLAP_ATA | 11,
	Atalanta_FireJavelin_BurningTickRateMs = NEWSKILLAP_ATA | 12,

	//Split Javelin
	Atalanta_SplitJavelin_NumberOfAttacks = 0x00994B28,
	Atalanta_SplitJavelin_DMGBoost = 0x00994B50,
	Atalanta_SplitJavelin_AtkRtgBoost = 0x00994B78,
	Atalanta_SplitJavelin_UseMana = 0x00994BA0,

	//Triumph Of Valhalla
	Atalanta_TriumphOfValhalla_AddMaxAtkPow_DoNotUse = 0x00994BC8, //disable behavior in ASM on client side
	Atalanta_TriumphOfValhalla_AddMaxAtkPowPerc_New = NEWSKILLAP_ATA | 13,
	Atalanta_TriumphOfValhalla_Duration = 0x00994BF0,
	Atalanta_TriumphOfValhalla_UseMana = 0x00994C18,

	//Lightning Javelin
	Atalanta_LightningJavelin_DMGBoost = 0x00994C40, //2D array, but only using first half
	Atalanta_LightningJavelin_UseMana = 0x00994C90,
	Atalanta_LightningJavelin_Radius_New = NEWSKILLAP_ATA | 14,
	Atalanta_LightningJavelin_StunChance = NEWSKILLAP_ATA | 15,
	Atalanta_LightningJavelin_StunDurationSec = NEWSKILLAP_ATA | 16,

	//Storm Javelin
	Atalanta_StormJavelin_DMGBoost = 0x00994CB8,
	Atalanta_StormJavelin_UseMana = 0x00994CE0,
	Atalanta_StormJavelin_AttackRange_New = NEWSKILLAP_ATA | 17,
	Atalanta_StormJavelin_AttackWidth_New = NEWSKILLAP_ATA | 18,

	//Hall Of Valhalla
	Atalanta_HallOfValhalla_AddEvasion_DoNotUse = 0x00994D08, //disable behavior in ASM on client side
	Atalanta_HallOfValhalla_AddBlock_New = NEWSKILLAP_ATA | 19,
	Atalanta_HallOfValhalla_Duration = 0x00994D30,
	Atalanta_HallOfValhalla_UseMana = 0x00994D58,

	//Extreme Rage
	Atalanta_ExtremeRage_DMGBoost = 0x00994D80,
	Atalanta_ExtremeRage_Area = 0x00994DA8,
	Atalanta_ExtremeRage_UseMana = 0x00994DD0,

	//Frost Javelin
	Atalanta_FrostJavelin_FreezeChance_DoNotUse = 0x00994DF8,
	Atalanta_FrostJavelin_FreezeDamage_DoNotUse = 0x00994DF8 + (4 * 10), //2nd half of 2D int array
	Atalanta_FrostJavelin_FreezeDamage_New = NEWSKILLAP_ATA | 20,
	Atalanta_FrostJavelin_FreezeChance_New = NEWSKILLAP_ATA | 21,
	Atalanta_FrostJavelin_SpeedMonDown = 0x00994E48,
	Atalanta_FrostJavelin_FreezeDuration = 0x0994E70,
	Atalanta_FrostJavelin_Duration = 0x00994E98,
	Atalanta_FrostJavelin_UseMana = 0x00994EC0,

	//Vengeance
	Atalanta_Vengeance_DMGBoost = 0x00994EE8,
	Atalanta_Vengeance_AddRange = 0x00994F38,
	Atalanta_Vengeance_AddCritical = 0x00994F10,
	Atalanta_Vengeance_UseMana = 0x00994F60,



	//Knight
	//=========================================================
	//=========================================================

	//Sword Blast
	Knight_SwordBlast_DMGBoostFlat = 0x00993250, //use this, and override
	Knight_SwordBlast_DMGBoostPercent = 0x00993278, //use this, and override
	Knight_SwordBlast_AttackRange = 0x00993A20,
	Knight_SwordBlast_AttackWidth_New = 0x00996B08,
	Knight_SwordBlast_UseMana = 0x00993200,

	//Holy Body
	Knight_HolyBody_DMGReduction_Obsolete = 0x009932C8,
	Knight_HolyBody_DmgToHealConversion_New = NEWSKILLAP_KS | 11,
	Knight_HolyBody_HealRadius_New = NEWSKILLAP_KS | 12,
	Knight_HolyBody_Duration = 0x009932F0,
	Knight_HolyBody_UseMana = 0x009932A0,

	//Physical Training
	Knight_PhysicalTraining_AddStamina = 0x00993318,

	//Double Crash
	Knight_DoubleCrash_DMGBoost = 0x00993368,
	Knight_DoubleCrash_AddCritical = 0x00993390,
	Knight_DoubleCrash_UseMana = 0x00993340,

	//HolyValor
	//Knight_HolyValor_PartyArea = 1 //use new - to override static va
	Knight_HolyValor_DamageBoostVsUndead = 0x009933B8,
	Knight_HolyValor_Duration = 0x009933E0,
	Knight_HolyValor_UseMana = 0x00993408,
	Knight_HolyValor_BuffRadius_New = NEWSKILLAP_KS | 1,

	//Brandish
	Knight_Brandish_Area = 0x00993430,
	Knight_Brandish_DMGBoost = 0x00993458,
	Knight_Brandish_UseMana = 0x00993480,

	//Piercing
	Knight_Piercing_DMGBoost = 0x009934A8,
	Knight_Piercing_Area = 0x009934D0,
	Knight_Piercing_PierceRange_New = NEWSKILLAP_KS | 2,
	Knight_Piercing_PierceWidth_New = NEWSKILLAP_KS | 3,
	Knight_Piercing_UseMana = 0x009934F8,

	//DrasticSpirit
	Knight_DrasticSpirit_AddDefense = 0x00993520,
	Knight_DrasticSpirit_Duration = 0x00993548,
	Knight_DrasticSpirit_UseMana = 0x00993570,
	Knight_DrasticSpirit_UndeadDmgReduction_New = NEWSKILLAP_KS | 13,

	//SwordMastery
	Knight_SwordMastery_AddWeaponDamagePercent = 0x00993598,
	Knight_SwordMastery_AddWeaponDamage = 0x00996D60,

	//DivineShield
	Knight_DivineShield_AddBlock_DoNotUse = 0x009935E8,
	Knight_DivineShield_AddBlock_New = NEWSKILLAP_KS | 4,
	Knight_DivineShield_ConvertLife = 0x009935C0,
	Knight_DivineShield_Duration = 0x00993610,
	Knight_DivineShield_UseMana = 0x00993638,


	//Holy Incantation
	Knight_HolyIncantation_SuccessRate = 0x00993660,
	Knight_HolyIncantation_LifeIncrease = 0x00993688,
	Knight_HolyIncantation_Duration = 0x009936B0,
	Knight_HolyIncantation_UseMana = 0x009936D8,
	Knight_HolyIncantation_MaxHP_New = NEWSKILLAP_KS | 5,
	Knight_HolyIncantation_MaxAbs_New = NEWSKILLAP_KS | 6,
	Knight_HolyIncantation_MaxDMGMin_New = NEWSKILLAP_KS | 7,
	Knight_HolyIncantation_MaxDMGMax_New = NEWSKILLAP_KS | 8,
	Knight_HolyIncantation_DMGIncrease_New = NEWSKILLAP_KS | 9,

	//Grand Cross
	Knight_GrandCross_AtkRtgBoost = 0x00993700,
	Knight_GrandCross_DMGBoost = 0x00993728,
	Knight_GrandCross_UndeadDamage = 0x00993750,
	Knight_GrandCross_UseMana = 0x00993778,

	//Sword of Justice
	Knight_SwordofJustice_DMGBoost = 0x00993930,
	Knight_SwordofJustice_Area = 0x00993958,
	Knight_SwordofJustice_UseMana = 0x00993980,

	//Godly Shield
	Knight_GodlyShield_ShieldABSBoost = 0x00993840,
	Knight_GodlyShield_Duration = 0x00993868,
	Knight_GodlyShield_UseMana = 0x00993890,

	//God's Blessing
	Knight_GodsBlessing_AddATKPow_DoNotUse = 0x009938B8,
	Knight_GodsBlessing_AddATKPowPercent_New = NEWSKILLAP_KS | 10,
	Knight_GodsBlessing_Duration = 0x009938E0,
	Knight_GodsBlessing_UseMana = 0x00993908,

	//Divine Piercing
	Knight_DivinePiercing_AddCritical = 0x04B06C18,
	Knight_DivinePiercing_DMGBoost = 0x009937A0,
	Knight_DivinePiercing_Numberofattacks = 0x009937F0,
	Knight_DivinePiercing_UseMana = 0x00993818,



	//Magician
	//=========================================================
	//=========================================================

	//Agony
	Magician_Agony_HPTaken = 0x00995C30,
	Magician_Agony_MPAdded = 0x4b0bcec,
	Magician_Agony_UseMana = 0x00995C08,

	//Fire Bolt
	Magician_FireBolt_DMGBoost = 0x04B0CA50,
	Magician_FireBolt_DMGBoostPercent = 0x00995C80,
	Magician_FireBolt_BurningChance				= NEWSKILLAP_MGS | 1,
	Magician_FireBolt_BurningDamagePercent		= NEWSKILLAP_MGS | 2,
	Magician_FireBolt_BurningDurationSec		= NEWSKILLAP_MGS | 3,
	Magician_FireBolt_BurningTickRateMs			= NEWSKILLAP_MGS | 4,
	Magician_FireBolt_UseMana = 0x00995C58,

	//Zenith
	Magician_Zenith_AddAbsorb = 0x04B06C44,
	Magician_Zenith_AddAbsorbPercent = 0x00995E60,
	Magician_Zenith_AddElement = 0x00995CF8,
	Magician_Zenith_Duration = 0x00995D20,
	Magician_Zenith_UseMana = 0x00995CD0,

	//Fire Ball
	Magician_FireBall_DMGBoost = 0x04B0CA80,
	Magician_FireBall_DMGBoostPercent = 0x00995D70,
	Magician_FireBall_Range_NotUsed = 0x00995DE8, //unused / spare

	Magician_FireBall_BurningChance				= NEWSKILLAP_MGS | 5,
	Magician_FireBall_BurningDamagePercent		= NEWSKILLAP_MGS | 6,
	Magician_FireBall_BurningDurationSec		= NEWSKILLAP_MGS | 7,
	Magician_FireBall_BurningTickRateMs			= NEWSKILLAP_MGS | 8,

	Magician_FireBall_Area = 0x00995DC0,
	Magician_FireBall_UseMana = 0x00995D48,

	//Mental Mastery
	Magician_MentalMastery_MPPotionBoost = 0x04B0D910,
	Magician_MentalMastery_MPBoost = 0x00995E10,
	Magician_MentalMastery_EW_Chain_MPDrain_New = NEWSKILLAP_MGS | 9, //chain values for Energy shield only

	//Waternado
	Magician_Watornado_DMGBoost = 0x00995E38,
	Magician_Watornado_Range = 0x00995EB0,
	Magician_Watornado_Area = 0x00995E88,
	Magician_Watornado_UseMana = 0x00995ED8,

	//Enchant Weapon
	Magician_EnchantWeapon_FireBurningDamagePercent = 0x00995F50,
	Magician_EnchantWeapon_FireBurningChance = 0x00995F50 + ( 4 * 10 ),	//2nd half of 2D int array
	Magician_EnchantWeapon_FireBurningDuration = NEWSKILLAP_MGS | 10,
	Magician_EnchantWeapon_FireBurningTickRateMs = NEWSKILLAP_MGS | 11,
	Magician_EnchantWeapon_IceFreezeAPPercent = 0x00995F00,
	Magician_EnchantWeapon_IceFreezeChance = 0x00995F00 + ( 4 * 10 ),	//2nd half of 2D int array
	Magician_EnchantWeapon_IceFreezeDuration = NEWSKILLAP_MGS | 12,
	Magician_EnchantWeapon_IceFreezeSpeedReduction = NEWSKILLAP_MGS | 13,
	Magician_EnchantWeapon_LightningStunAPPercent = 0x0995FA0,
	Magician_EnchantWeapon_LightningStunChance = 0x0995FA0 + ( 4 * 10 ),	//2nd half of 2D int array
	Magician_EnchantWeapon_LightningStunDuration = NEWSKILLAP_MGS | 14,
	Magician_EnchantWeapon_LightningRange_New = NEWSKILLAP_MGS | 15,
	Magician_EnchantWeapon_Duration = 0x00995FF0,
	Magician_EnchantWeapon_UseMana = 0x00996018,


	//Death Ray
	Magician_DeathRay_DMGBoost = 0x00996040,
	Magician_DeathRay_Range = 0x00996090,
	Magician_DeathRay_UseMana = 0x009960B8,

	//Energy Shield
	Magician_EnergyShield_ReduceDamage = 0x009960E0,
	Magician_EnergyShield_Duration = 0x00996108,
	Magician_EnergyShield_UseMana = 0x00996130,
	Magician_EnergyShield_MPDrain_New = NEWSKILLAP_MGS | 16,

	//Diastrophism
	Magician_Diastrophism_DMGBoost = 0x00996158,
	Magician_Diastrophism_Area = 0x009961A8,
	Magician_Diastrophism_UseMana = 0x009961D0,

	//Spirit Elemental
	Magician_SpiritElemental_Duration = 0x00996220,
	Magician_SpiritElemental_UseMana = 0x00996248,
	Magician_SpiritElemental_DamageBoostMin = 0x04B0DC04,
	Magician_SpiritElemental_DamageBoostMax = 0x04B0DC2C,

	//Dancing Sword
	Magician_DancingSword_DamagePercentOfAP = 0x009962E8,
	Magician_DancingSword_AtkDelay = 0x00996338,
	Magician_DancingSword_Duration = 0x00996360,
	Magician_DancingSword_UseMana = 0x00996388,

	Magician_DancingSword_FireBurningChance_New			= NEWSKILLAP_MGS | 17,
	Magician_DancingSword_FireBurningDamagePercent_New	= NEWSKILLAP_MGS | 18,
	Magician_DancingSword_FireBurningDurationSec_New	= NEWSKILLAP_MGS | 19,

	Magician_DancingSword_IceFreezeAPPercent			= 0x00996298,
	Magician_DancingSword_IceFreezeChance_New			= NEWSKILLAP_MGS | 20,
	Magician_DancingSword_IceFreezeDuration_New			= NEWSKILLAP_MGS | 21,
	Magician_DancingSword_IceFreezeSpeedReduction_New	= NEWSKILLAP_MGS | 22,

	//Fire Elemental
	Magician_FireElemental_Duration = 0x00996608, //from unused asm array
	Magician_FireElemental_ATKPowMin = 0x009963B0,
	Magician_FireElemental_ATKPowMax = 0x009963D8,
	Magician_FireElemental_AtkRating = 0x00996400,
	Magician_FireElemental_HP = 0x00996428,
	Magician_FireElemental_UseMana = 0x00996450,

	//Flame Wave
	Magician_FlameWave_DMGBoost = 0x00996478,
	Magician_FlameWave_Area = 0x009964C8,
	Magician_FlameWave_UseMana = 0x009964F0,
	Magician_FlameWave_BurningChance		= NEWSKILLAP_MGS | 23,
	Magician_FlameWave_BurningDamagePercent	= NEWSKILLAP_MGS | 24,
	Magician_FlameWave_BurningDurationSec	= NEWSKILLAP_MGS | 25,
	Magician_FlameWave_BurningTickRateMs	= NEWSKILLAP_MGS | 26,

	//Distortion
	Magician_Distortion_DamageReduction = 0x009961F8,
	Magician_Distortion_SpeedReduction = 0x00996518,
	Magician_Distortion_Area = 0x00996568,
	Magician_Distortion_Duration = 0x00996590,
	Magician_Distortion_Chance = NEWSKILLAP_MGS | 27,
	Magician_Distortion_DamagePercent = NEWSKILLAP_MGS | 28,
	Magician_Distortion_UseMana = 0x009965B8,

	//Meteorite
	Magician_Meteorite_DMGBoost = 0x009965E0,
	Magician_Meteorite_Area = 0x00996630,
	Magician_Meteorite_UseMana = 0x00996658,



	//Priestess
	//=========================================================
	//=========================================================

	//Divine Lightning
	Priestess_Healing_LifeRecovery = 0x04B0CB40,
	Priestess_Healing_LifeRecoveryPercent = 0x00996900,
	Priestess_Healing_UseMana = 0x009968D8,

	Priestess_HolyBolt_Damage = 0x04B0CB70,
	Priestess_HolyBolt_DamageBoost_DoNotUse = 0x00996974,
	Priestess_HolyBolt_DamageBoost_New = NEWSKILLAP_PRS | 11,
	Priestess_HolyBolt_UseMana = 0x00996950,

	Priestess_MultiSpark_Damage = 0x04B0CBA0,
	Priestess_MultiSpark_DamageBoost = 0x009969F0,
	Priestess_MultiSpark_NumSparksMin_New = 0x00997030,
	Priestess_MultiSpark_NumSparksMax = 0x00996A40,
	Priestess_MultiSpark_UseMana = 0x009969C8,

	Priestess_HolyMind_ReduceDamage = 0x00996A90,
	Priestess_HolyMind_Duration_New = 0x009970F8,
	Priestess_HolyMind_UseMana = 0x00996A68,

	Priestess_Meditation_MPPotionBoost = 0x04B0D8B0,
	Priestess_Meditation_AddMPRegen = 0x00996AE0, // float


	Priestess_DivineLightning_ATKRadius_New = 0x0099699C, //need new and hook in asm,
	Priestess_DivineLightning_Damage_New = NEWSKILLAP_PRS | 1,
	Priestess_DivineLightning_DamageBoost = 0x04B0CBD0,
	Priestess_DivineLightning_NumOfBolts = 0x00996B58,
	Priestess_DivineLightning_UseMana = 0x00996B80,

	Priestess_HolyReflection_ReflectDMG = 0x00996BA8,
	Priestess_HolyReflection_Duration = 0x00996BD0,
	Priestess_HolyReflection_UseMana = 0x00996BF8,

	Priestess_GrandHealing_LifeRecovery = 0x04B0CC00,
	Priestess_GrandHealing_LifeRecoveryPercent = 0x00996C20,
	Priestess_GrandHealing_Range = 0x00996C70,
	Priestess_GrandHealing_UseMana = 0x00996C98,

	Priestess_VigorBall_AtkRtgBoost = 0x04B06C70,
	Priestess_VigorBall_DamageBoost = 0x00996CC0,
	Priestess_VigorBall_Damage_New = NEWSKILLAP_PRS | 2,
	Priestess_VigorBall_UseMana = 0x00996D10,

	Priestess_Resurrection_SuccessRate = 0x00996D38,
	Priestess_Resurrection_MaxPlayers = 0x04B0DE60,
	Priestess_Resurrection_UseMana = 0x00996D88,

	Priestess_Extinction_SuccessRate = 0x00996DB0,
	Priestess_Extinction_HPReduction = 0x00996DD8,
	Priestess_Extinction_HPReductionCap_New = NEWSKILLAP_PRS | 4,
	Priestess_Extinction_Radius_New = NEWSKILLAP_PRS | 14,
	Priestess_Extinction_UseMana = 0x00996E00,

	Priestess_VirtualLife_HPBoost = 0x00996E28,
	Priestess_VirtualLife_Duration = 0x00996E50,
	Priestess_VirtualLife_UseMana = 0x00996E78,
	Priestess_VirtualLife_MPBoost_New =	NEWSKILLAP_PRS | 5,

	Priestess_GlacialSpike_DamageBoost = 0x00996EA0,
	Priestess_GlacialSpike_FreezeChance = 0x00996EC8, //spare
	Priestess_GlacialSpike_ATKRange = 0x00996EF0,
	Priestess_GlacialSpike_FreezeDurationSec = 0x04B0BA20,
	Priestess_GlacialSpike_UseMana = 0x00996F18,
	Priestess_GlacialSpike_SpeedMonDown_New = NEWSKILLAP_PRS | 15,
	Priestess_GlacialSpike_FreezePercentOfAP_New = NEWSKILLAP_PRS | 16,

	Priestess_RegenerationField_AddLifeRegen = 0x00996F40,
	Priestess_RegenerationField_AddManaRegen = 0x00996F68,
	Priestess_RegenerationField_Duration = 0x00996F90,
	Priestess_RegenerationField_Range = 0x00996FB8,
	Priestess_RegenerationField_UseMana = 0x00996FE0,

	Priestess_VirtualRegen_HPRegen_New = NEWSKILLAP_PRS | 6,
	Priestess_VirtualRegen_MPRegen_New = NEWSKILLAP_PRS | 7,
	Priestess_VirtualRegen_Duration_New = NEWSKILLAP_PRS | 8,
	Priestess_VirtualRegen_UseMana = 0x00996FE0,	//same as RegenerationField

	Priestess_ChainLightning_Damage_New = NEWSKILLAP_PRS | 9,
	Priestess_ChainLightning_DamageBoost = 0x00997008,
	Priestess_ChainLightning_Numberofbolts = 0x00997058,
	Priestess_ChainLightning_Range = 0x00997080,
	Priestess_ChainLightning_UseMana = 0x009970A8,

	Priestess_SummonMuspell_MuspellDMGBoost = 0x009970D0,
	Priestess_SummonMuspell_DamageBoostMin_DoNotUse = 0x04B0DC64,
	Priestess_SummonMuspell_DamageBoostMax_DoNotUse = 0x04B0DC8C,
	Priestess_SummonMuspell_DamageBoost_New  = NEWSKILLAP_PRS | 10,
	Priestess_SummonMuspell_VirtualLifeReduction = 0x04B0DCB4,
	Priestess_SummonMuspell_MuspellAttackDelaySec = 0x00997120,		//Muspell attack delay
	Priestess_SummonMuspell_AddEvasion_DoNotUse = 0x00997170,
	Priestess_SummonMuspell_AddEvasion_New  = NEWSKILLAP_PRS | 12,
	Priestess_SummonMuspell_Duration = 0x00997198,
	Priestess_SummonMuspell_UseMana = 0x009971C0,
	Priestess_SummonMuspell_UndeadAbsorb_DoNotUse = 0x00997148,			//Absorb rating bonus against Undead
	Priestess_SummonMuspell_UndeadAbsorb_New  = NEWSKILLAP_PRS | 13,


	//Shaman
	//=========================================================
	//=========================================================

	//Dark Bolt
	Shaman_DarkBolt_DMGBoost = 0x009987C8,
	Shaman_DarkBolt_Range = 0x04B07140,
	Shaman_DarkBolt_UseMana = 0x009987A0,

	//Dark Wave
	Shaman_DarkWave_DMGBoost = 0x00998840,
	Shaman_DarkWave_Range = 0x009988B8,
	Shaman_DarkWave_ATKRange = 0x00998890,
	Shaman_DarkWave_UseMana = 0x00998818,

	//Inertia
	Shaman_Inertia_Curse_DamagePercent_New = NEWSKILLAP_SHA | 1,
	Shaman_Inertia_Curse_Chance_New = NEWSKILLAP_SHA | 2,
	Shaman_Inertia_Curse_Duration = 0x00998908,
	Shaman_Inertia_Curse_SpeedReduction = 0x00998930,
	Shaman_Inertia_Curse_DamageReduction_New = NEWSKILLAP_SHA | 3,
	Shaman_Inertia_Area = 0x00998980,
	Shaman_Inertia_UseMana = 0x009988E0,

	//Inner Peace
	Shaman_InnerPeace_ManaPotionBoost = 0x04B0D950,
	Shaman_InnerPeace_ManaBoost = 0x009989A8,

	//Spiritual Flare
	Shaman_SpiritualFlare_DMGBoost = 0x009989F8,
	Shaman_SpiritualFlare_Range = 0x00998A48,
	Shaman_SpiritualFlare_ATKRange = 0x00998A70,
	Shaman_SpiritualFlare_UseMana = 0x009989D0,

	//Spiritual Manacle
	Shaman_SpiritualManacle_StunTime = 0x00998AC0,
	Shaman_SpiritualManacle_UseMana = 0x00998A98,

	//Chasing Hunt
	Shaman_ChasingHunt_AddAtkPowMin_DoNotUse = 0x04B0DBA4,
	Shaman_ChasingHunt_AddAtkPowMax_DoNotUse = 0x04B0DBCC,
	Shaman_ChasingHunt_AtkPowBoost_New = NEWSKILLAP_SHA | 6,
	Shaman_ChasingHunt_Range = 0x00998B38,
	Shaman_ChasingHunt_Duration = 0x00998B10,
	Shaman_ChasingHunt_UseMana = 0x00998AE8,

	//Advent Migal
	Shaman_AdventMigal_AddATKPow = 0x00998BB0,
	Shaman_AdventMigal_Duration = 0x00998B88,
	Shaman_AdventMigal_UseMana = 0x00998B60,

	//Rain Maker
	Shaman_Rainmaker_ReduceDamage_DoNotUse = 0x00998C50,
	Shaman_Rainmaker_AddEvasion_DoNotUse = 0x04B06F50,
	Shaman_Rainmaker_ReduceDamage_New = NEWSKILLAP_SHA | 4,
	Shaman_Rainmaker_AddEvasion_New = NEWSKILLAP_SHA | 5,
	Shaman_Rainmaker_ReduceMP = 0x00998C28,
	Shaman_Rainmaker_Duration = 0x00998C00,
	Shaman_Rainmaker_UseMana = 0x00998BD8,

	//Phantom Call
	Shaman_PhantomCall_DMGBoost = 0x00998CC8,
	Shaman_PhantomCall_Range = 0x00998CA0,
	Shaman_PhantomCall_UseMana = 0x00998C78,

	//Haunt
	Shaman_Haunt_LifeAbsorb = 0x00998D40,
	Shaman_Haunt_Radius_New = NEWSKILLAP_SHA | 7,
	Shaman_Haunt_LifeAbsorbCap_New = NEWSKILLAP_SHA | 8,
	Shaman_Haunt_UseMana = 0x00998D18,

	//Scratch
	Shaman_Scratch_DMGBoost = 0x00998DE0,
	Shaman_Scratch_UseMana = 0x00998DB8,

	//Sumon Hestian
	Shaman_SummonHestian_Duration =  0x00998F70,
	Shaman_SummonHestian_ATKPowMin = 0x00998EA8,
	Shaman_SummonHestian_ATKPowMax = 0x00998ED0,
	Shaman_SummonHestian_AtkRating = 0x00998E80,
	Shaman_SummonHestian_Defense = 0x00998CF0,
	Shaman_SummonHestian_HP = 0x00998E58,
	Shaman_SummonHestian_UseMana = 0x00998E30,

	//Judgement
	Shaman_Judgement_DMGBoost = 0x00998F48,
	Shaman_Judgement_AddCritical = 0x04B07460,
	Shaman_Judgement_Range = 0x00998F20,
	Shaman_Judgement_UseMana = 0x00998EF8,

	//Advent Midranda
	Shaman_AdventMidranda_ATKSPDBoost = 0x00998FC0,
	Shaman_AdventMidranda_Duration = 0x00998FE8,
	Shaman_AdventMidranda_UseMana = 0x00998F98,

	//Mourning Pray
	Shaman_MourningPray_DMGBoost = 0x00999060,
	Shaman_MourningPray_ATKRange = 0x00999038,
	Shaman_MourningPray_UseMana = 0x00999010,
};




const int iaMechT51DMGBoost[10] = { 60, 70, 80, 90, 100, 110, 120, 130, 140, 150 };
const int iaMechT52DMGBoost[10] = { 12, 16, 20, 24, 28, 32, 36, 40, 44, 50 };
const int iaMechT52PoisonTimeDuration[10] = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6 };
const int iaMechT53DMGReduction[10] = { 7, 8, 9, 10, 12, 14, 16, 18, 20 };
const int iaMechT53WPAtkRateRed[10] = { 20, 22, 24, 26, 27, 28, 29, 30, 31 };
const int iaMechT54DMGBoost[10] = { 20, 30, 40, 50, 60, 70, 80, 90, 100, 120 };


const float faFighterT51AddHPRegen[10] = { 2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f, 16.0f, 18.0f, 20.0f };
const int iaFighterT51AddHPPotionBoost[10] = { 4, 8, 12, 16, 20, 24, 28, 32, 36, 40 };
const int iaFighterT51BoostHealthBoost[10] = { 15, 30, 45, 60, 75, 90, 105, 120, 135, 150 };
const int iaFighterT52DMGBoost[10] = { 50, 60, 70, 80, 90, 100, 120, 140, 160, 180 };
const int iaFighterT52Area[10] = { 50, 55, 60, 65, 70, 80, 85, 90, 95 };
const float faFighterT53HPDec[10] = { 5.6f, 5.2f, 4.8f, 4.4f, 4.0f, 3.6f, 3.2f, 2.8f, 2.4f, 2.0f };
const int iaFighterT53HitsBoost[10] = { 6, 6, 6, 5, 5, 5, 4, 4, 4, 3 };
const int iaFighterT53DMGBoost[10] = { 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
const int iaFighterT54DMGBoost[10] = { 17, 24, 31, 38, 45, 52, 59, 66, 73, 80 };
const int iaFighterT54Duration[10] = { 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
const int iaFighterT54MPDrain[10] = { 17, 19, 21, 23, 25, 27, 29, 31, 33, 35 };
const int iaFighterT54SPDrain[10] = { 29, 31, 35, 37, 39, 41, 43, 45, 47, 50 };

const int iaPikeT51Duration[10] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
const int iaPikeT52MinATKRed[10] = { 20, 40, 60, 80, 100, 120, 140, 160, 180, 200 };
const int iaPikeT52MaxATKInc[10] = { 30, 60, 90, 120, 150, 180, 210, 240, 270, 300 };
const int iaPikeT53DMGBoost[10] = { 4, 8, 12, 16, 20, 24, 28, 32, 36, 40 };
const int iaPikeT53ATKRate[10] = { 24, 28, 32, 36, 40, 44, 48, 52, 56, 60 };
const int iaPikeT54DMGBoost[10] = { 18, 26, 34, 42, 50, 58, 66, 74, 82, 90 };

const int iaArcherT51RangeAdd[10] = { 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 };
const int iaArcherT51CritRate[10] = { 3, 6, 9, 12, 15, 18, 21, 24, 27, 30 };
const int iaArcherT51DMGBoost[10] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
const int iaArcherT52DMGBoost[10] = { 60, 70, 80, 90, 100, 110, 120, 130, 140 };
const int iaArcherT52CDMGBoost[10] = { 2, 3, 4, 5, 6, 8, 10, 12, 15 };
const int iaArcherT53Duration[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const int iaArcherT54DMGBoost[10] = { 86, 92, 98, 104, 110, 116, 122, 128, 134, 140 };
const int iaArcherT54Duration[10] = { 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
const int iaArcherT54MPDrain[10] = { 17, 19, 21, 23, 25, 27, 29, 31, 33, 35 };
const int iaArcherT54SPDrain[10] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 };

const int iaKnightT51DMGBoost[10] = { 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 };
const int iaKnightT52DMGBoost[10] = { 20, 25, 30, 35, 40, 45, 50, 55, 60, 65 };
const int iaKnightT52Area[10] = { 80, 85, 90, 95, 100, 105, 110, 115, 120, 125 };
const int iaKnightT52UndeadDMG[10] = { 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 };
const int iaKnightT53DefBoost[10] = { 40, 46, 52, 58, 64, 70, 76, 82, 96, 100 };
const int iaKnightT54DMGBoost[10] = { 4,8,12,16,20,24,28,32,36,40 };

const int iaAtaT51DMGBoost[10] = { 39, 41, 44, 47, 50, 53, 56, 59, 62, 75 };
const int iaAtaT51Targets[10] = { 3, 3, 4, 4, 5, 6, 7, 8, 9, 10 };
const int iaAtaT51PoisonDuration[10] = { 2, 2, 3, 3, 4, 4, 5, 5, 6, 7 };
const int iaAtaT52Speed[10] = { 8, 10, 12, 14, 16, 18, 20, 22, 24, 26 };
const int iaAtaT52Evade[10] = { 1, 1, 1, 2, 2, 2, 3, 3, 3, 4 };

const int iaPrsT52DMGBoost[10] = { 100, 115, 130, 145, 160, 175, 190, 205, 220, 235 };
const int iaPrsT52Area[10] = { 50, 53, 56, 59, 61, 64, 67, 70, 73, 76 };
const int iaPrsT52ChargedArea[10] = { 150, 160, 180, 190, 200, 210, 220, 230, 240, 250 };
const int iaPrsT53ATKRate[10] = { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
const int iaPrsT53Duration[10] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
const int iaPrsT53Area[10] = { 80, 85, 90, 95, 100, 105, 110, 115, 120, 125 };
const int iaPrsT54Defense[10] = { 70, 80, 90, 100, 110, 120, 130, 140, 150, 160 };
const int iaPrsT54Absorption[10] = { 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 };

const int iaPrsT2DMGBoost[10] = { 30, 40, 50, 60, 70, 80, 100, 110, 120, 130 };
const int iaPrsT2Range[10] = { 180, 180, 180, 180, 180, 180, 180, 180, 180, 180 };
const int iaPrsT2Bolts[10] = { 3, 3, 3, 4, 4, 4, 5, 5, 5, 6 };

const int iaMageT51BlockBoost[10] = { 4, 8, 12, 16, 20, 24, 28, 32, 36, 40 };
const int iaMageT51ATKRateRed[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
const int iaMageT52Duration[10] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
const int iaMageT52RunSpeedRed[10] = { 80, 76, 72, 68, 64, 56, 52, 48, 44 };
const int iaMageT53DMGBoost[10] = { 26, 32, 38, 44, 50, 56, 62, 68, 74, 80 };
const int iaMageT53AddRange[10] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
const int iaMageT54DMGBoost[10] = { 65, 80, 95, 110, 125, 140, 155, 170, 185, 200 };
const int iaMageT54Duration[10] = { 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
const int iaMageT54MPDrain[10] = { 75, 80, 85, 90, 95, 100, 105, 110, 115, 120 };
const int iaMageT54SPDrain[10] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 };

const int iaASNT51CritBoost[10] = { 4, 8, 12, 16, 20, 24, 28, 32, 36, 40 };
const int iaASNT51Range[10] = { 60, 80, 100, 120, 140, 160, 180, 200, 220, 240 };

const int iaShaT51MPBoost[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
const int iaShaT51STMBoost[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };

//Skill classification
enum class ESkillClass
{
	Undefined = 0,
	Passive,		//Non-castable
	Active,			//Active skill
	BuffSelf,       //Self buff only skill
	BuffParty,		//Party buff skill
	Summon,         //summon type skill
};


enum ESkillType
{
	SKILLTYPE_Undefined = 0,

	SKILLTYPE_Right = 0x01000000,
	SKILLTYPE_Left = 0x02000000,
	SKILLTYPE_All = 0x03000000,
	SKILLTYPE_Passive = 0x04000000,
};

enum ESkillPlayID
{
	SKILLPLAYID_Undefined = 0x00,

	//Tier1
	SKILLPLAYID_T11 = 0x1A,
	SKILLPLAYID_T12 = 0x1B,
	SKILLPLAYID_T13 = 0x1C,
	SKILLPLAYID_T14 = 0x1D,

	//Tier2
	SKILLPLAYID_T21 = 0x2A,
	SKILLPLAYID_T22 = 0x2B,
	SKILLPLAYID_T23 = 0x2C,
	SKILLPLAYID_T24 = 0x2D,

	//Tier3
	SKILLPLAYID_T31 = 0x3A,
	SKILLPLAYID_T32 = 0x3B,
	SKILLPLAYID_T33 = 0x3C,
	SKILLPLAYID_T34 = 0x3D,

	//Tier4
	SKILLPLAYID_T41 = 0x4A,
	SKILLPLAYID_T42 = 0x4B,
	SKILLPLAYID_T43 = 0x4C,
	SKILLPLAYID_T44 = 0x4D,

	//Tier5
	SKILLPLAYID_T51 = 0x5A,		//90
	SKILLPLAYID_T52 = 0x5B,		//91
	SKILLPLAYID_T53 = 0x5C,		//92
	SKILLPLAYID_T54 = 0x5D,		//93
};


enum ESkillSound
{
	SKILLSOUND_PhysicalAbsorb3 = 0x00001032,
	SKILLSOUND_Automation3 = 0x00002012,
	SKILLSOUND_AvengingCrash = 0x00003120,
};


enum ESkillMask
{
	SKILLMASK_ExtremeShield = 0x00000001,
	SKILLMASK_PhysicalAbsorb = 0x00000002,
	SKILLMASK_Automation = 0x00000004,
	SKILLMASK_CriticalHit = 0x00000008,
	SKILLMASK_SparkShield = 0x00000010,
	SKILLMASK_GodlyShield = 0x00000020,
	SKILLMASK_HolyReflection = 0x00000030,

	SKILLMASK_HolyBody = 0x00010000,
};

enum ESkillEffectID
{
	//Knight
	SKILLEFFECTID_HolyValorAction = 700,

	//Magician
	SKILLEFFECTID_EnchantWeaponFire = 100,
	SKILLEFFECTID_EnchantWeaponIce = 200,
	SKILLEFFECTID_EnchantWeaponLightning = 300,
};

enum ESkillID
{
	SKILLID_Unknown = 0,

	SKILLID_DefaultAttack = 0x11111111,

	//Fighter
	SKILLID_MeleeMastery = 0x02010001,
	SKILLID_Recuperation = 0x02010002,			 //was SKILLID_FireAttribute
	SKILLID_Raving = 0x02010003,
	SKILLID_DoubleImpact = 0x02010004,

	SKILLID_TripleImpact = 0x02020001,
	SKILLID_BrutalSwing = 0x02020002,
	SKILLID_WarCry = 0x02020003,					//Was Roar
	SKILLID_RageofZecram = 0x02020004,

	SKILLID_DemonsBane = 0x02040001,			//Was Concentration
	SKILLID_AvengingCrash = 0x02040002,
	SKILLID_ConcentrationSA = 0x02040003,			//Was SwiftAxe. Now Concentration and Swift Axe combined
	SKILLID_BoneCrash = 0x02040004,

	SKILLID_Destroyer = 0x02080001,
	SKILLID_Berserker = 0x02080002,
	SKILLID_CycloneStrike = 0x02080003,
	SKILLID_Resilience = 0x02080004,				//was SKILLID_BoostHealth

	SKILLID_SurvivalInstinct = 0x02100001,
	SKILLID_SeismicImpact = 0x02100002,
	SKILLID_RuthlessofZecram = 0x02100003,
	SKILLID_Whirlwind = 0x02100004,

	//Mechanician
	SKILLID_ExtremeShield = 0x01010001,
	SKILLID_MechanicBomb = 0x01010002,
	SKILLID_PhysicalAbsorb = 0x01010003,
	SKILLID_ElectricalOverload = 0x01010004,

	SKILLID_GreatSmash = 0x01020001,
	SKILLID_Maximize = 0x01020002,
	SKILLID_Automation = 0x01020003,
	SKILLID_Spark = 0x01020004,

	SKILLID_MetalArmor = 0x01040001,
	SKILLID_GrandSmash = 0x01040002,
	SKILLID_MechanicWeaponMastery = 0x01040003,
	SKILLID_SparkShield = 0x01040004,

	SKILLID_Impulsion = 0x01080001,
	SKILLID_Compulsion = 0x01080002,
	SKILLID_MagneticSphere = 0x01080003,
	SKILLID_MetalGolem = 0x01080004,

	SKILLID_Rupture = 0x01100001,
	SKILLID_ParasitShot = 0x01100002,
	SKILLID_HardeningShield = 0x01100003,
	SKILLID_MagneticDischarge = 0x01100004,

	//Archer
	SKILLID_ScoutHawk = 0x04010001,
	SKILLID_ShootingMastery = 0x04010002,
	SKILLID_WindArrow = 0x04010003,
	SKILLID_PerfectAim = 0x04010004,

	SKILLID_DionsEye = 0x04020001,
	SKILLID_Falcon = 0x04020002,
	SKILLID_ArrowofRage = 0x04020003,
	SKILLID_Avalanche = 0x04020004,

	SKILLID_ElementalShot = 0x04040001,
	SKILLID_GoldenFalcon = 0x04040002,
	SKILLID_BombShot = 0x04040003,
	SKILLID_Perforation = 0x04040004,

	SKILLID_RecallWolverine = 0x04080001,
	SKILLID_EvasionMastery = 0x04080002,
	SKILLID_PhoenixShot = 0x04080003,
	SKILLID_ForceofNature = 0x04080004,

	SKILLID_LethalSight = 0x04100001,
	SKILLID_FierceWind = 0x04100002,
	SKILLID_EntalingRoots = 0x04100003,
	SKILLID_Bombardment = 0x04100004,

	//Pikeman
	SKILLID_PikeWind = 0x03010001,
	SKILLID_IceAttribute_Old = 0x03010002,
	SKILLID_PolearmMastery = 0x03010002,
	SKILLID_PreciseHit = 0x03010003,
	SKILLID_JumpingCrash = 0x03010004,

	SKILLID_GroundPike = 0x03020001,
	SKILLID_Tornado = 0x03020002,
	SKILLID_WeaponBlockMastery = 0x03020003,
	SKILLID_Expansion = 0x03020004,

	SKILLID_VenomSpear = 0x03040001,
	SKILLID_Vanish = 0x03040002,
	SKILLID_CriticalMastery = 0x03040003,
	SKILLID_ChainLance = 0x03040004,

	SKILLID_AssassinsEye = 0x03080001,
	SKILLID_ChargingStrike = 0x03080002,
	SKILLID_Vague = 0x03080003,
	SKILLID_ShadowMaster = 0x03080004,

	SKILLID_RingofSpears = 0x03100001,
	SKILLID_Focus = 0x03100002,
	SKILLID_DeathMaster = 0x03100003,
	SKILLID_Twister = 0x03100004,

	//Atalanta
	SKILLID_ShieldStrike = 0x06010001,
	SKILLID_Farina = 0x06010002,
	SKILLID_ThrowingMastery = 0x06010003,
	SKILLID_BiggerSpear = 0x06010004,

	SKILLID_Windy = 0x06020001,
	SKILLID_WindJavelin = 0x06020002,
	SKILLID_SoulSucker = 0x06020003,
	SKILLID_FireJavelin = 0x06020004,

	SKILLID_SplitJavelin = 0x06040001,
	SKILLID_TriumphofValhalla = 0x06040002,
	SKILLID_LightningJavelin = 0x06040003,
	SKILLID_StormJavelin = 0x06040004,

	SKILLID_HallofValhalla = 0x06080001,
	SKILLID_ExtremeRage = 0x06080002,
	SKILLID_FrostJavelin = 0x06080003,
	SKILLID_Vengeance = 0x06080004,

	SKILLID_SummonTiger = 0x06100001,
	SKILLID_GoldenApple = 0x06100002,
	SKILLID_PlagueJavelin = 0x06100003,
	SKILLID_ComboJavelin = 0x06100004,

	//Knight
	SKILLID_SwordBlast = 0x05010001,
	SKILLID_HolyBody = 0x05010002,
	SKILLID_PhysicalTraining = 0x05010003,
	SKILLID_DoubleCrash = 0x05010004,

	SKILLID_HolyValor = 0x05020001,
	SKILLID_Brandish = 0x05020002,
	SKILLID_Piercing = 0x05020003,
	SKILLID_DrasticSpirit = 0x05020004,

	SKILLID_SwordMastery = 0x05040001,
	SKILLID_DivineShield = 0x05040002,
	SKILLID_HolyIncantation = 0x05040003,
	SKILLID_GrandCross = 0x05040004,

	SKILLID_SwordofJustice = 0x05080001,
	SKILLID_GodlyShield = 0x05080002,
	SKILLID_GodsBlessing = 0x05080003,
	SKILLID_DivinePiercing = 0x05080004,

	SKILLID_HolyConviction = 0x05100001,
	SKILLID_DivineInquisiton = 0x05100002,
	SKILLID_GloriousShield = 0x05100003,
	SKILLID_DivineCross = 0x05100004,

	//Magician
	SKILLID_Agony = 0x08010001,
	SKILLID_FireBolt = 0x08010002,
	SKILLID_Zenith = 0x08010003,
	SKILLID_FireBall = 0x08010004,

	SKILLID_MentalMastery = 0x08020001,
	SKILLID_Watornado = 0x08020002,
	SKILLID_EnchantWeapon = 0x08020003,
	SKILLID_DeathRay = 0x08020004,

	SKILLID_EnergyShield = 0x08040001,
	SKILLID_Diastrophism = 0x08040002,
	SKILLID_SpiritElemental = 0x08040003,
	SKILLID_DancingSword = 0x08040004,

	SKILLID_FireElemental = 0x08080001,
	SKILLID_FlameWave = 0x08080002,
	SKILLID_Distortion = 0x08080003,
	SKILLID_Meteorite = 0x08080004,

	SKILLID_WizardTrance = 0x08100001,
	SKILLID_StoneSkin = 0x08100002,
	SKILLID_RedRay = 0x08100003,
	SKILLID_Cataclysm = 0x08100004,

	//Priestess
	SKILLID_Healing = 0x07010001,
	SKILLID_HolyBolt = 0x07010002,
	SKILLID_MultiSpark = 0x07010003,
	SKILLID_HolyMind = 0x07010004,

	SKILLID_Meditation = 0x07020001,
	SKILLID_DivineLightning = 0x07020002,
	SKILLID_HolyReflection = 0x07020003,
	SKILLID_GrandHealing = 0x07020004,

	SKILLID_VigorBall = 0x07040001,
	SKILLID_Ressurection = 0x07040002,
	SKILLID_Extinction = 0x07040003,
	SKILLID_VirtualLife = 0x07040004,

	SKILLID_GlacialSpike = 0x07080001,
	SKILLID_VirtualRegen = 0x07080002,
	SKILLID_ChainLightning = 0x07080003,
	SKILLID_SummonMuspell = 0x07080004,



	SKILLID_IceElemental = 0x07100001,
	SKILLID_LightningSurge = 0x07100002,
	SKILLID_HeavenlyLight = 0x07100003,
	SKILLID_Consecration = 0x07100004,

	//Assassin
	SKILLID_Stinger = 0x10010001,
	SKILLID_RunningHit = 0x10010002,
	SKILLID_DualWieldMastery = 0x10010003,
	SKILLID_Wisp = 0x10010004,

	SKILLID_VenomThorn = 0x10020001,
	SKILLID_Alas = 0x10020002,
	SKILLID_SoulShock = 0x10020003,
	SKILLID_DodgeMastery = 0x10020004,

	SKILLID_SoreBlade = 0x10040001,
	SKILLID_GustSlash = 0x10040002,
	SKILLID_Inpes = 0x10040003,
	SKILLID_Blind = 0x10040004,

	SKILLID_Cripple			= 0x10080001,
	SKILLID_AssassinMastery		= 0x10080002, //was Fatal Mastery
	SKILLID_Polluted			= 0x10080003,
	SKILLID_NinjaShadow			= 0x10080004,

	SKILLID_ShadowBlink			 = 0x10100001,
	SKILLID_RisingSlash			 = 0x10100002,
	SKILLID_ViolentStub			 = 0x10100003,
	SKILLID_ShadowStorm			 = 0x10100004,

	//Shaman
	SKILLID_DarkBolt			= 0x50010001,
	SKILLID_DarkWave			= 0x50010002,
	SKILLID_Inertia				= 0x50010003,
	SKILLID_InnerPeace			= 0x50010004,

	SKILLID_SpiritualFlare		= 0x50020001,
	SKILLID_SpiritualManacle	= 0x50020002,
	SKILLID_ChasingHunt			= 0x50020003,
	SKILLID_AdventMigal			= 0x50020004,

	SKILLID_Rainmaker			= 0x50040001,
	SKILLID_PhantomCall			= 0x50040002,
	SKILLID_Haunt				= 0x50040003,
	SKILLID_Scratch				= 0x50040004,

	SKILLID_RecallHestian		= 0x50080001,
	SKILLID_Judgement			= 0x50080002,
	SKILLID_AdventMidranda		= 0x50080003,
	SKILLID_MourningPray		= 0x50080004,

	SKILLID_Creed				= 0x50100001,
	SKILLID_PressDeity			= 0x50100002,
	SKILLID_PhantomNail			= 0x50100003,
	SKILLID_OccultLife			= 0x50100004,

	//Stones
	SKILLID_FighterStone		= 0x09040001,
	SKILLID_MechanicianStone	= 0x09040002,
	SKILLID_PikemanStone		= 0x09040003,
	SKILLID_ArcherStone			= 0x09040004,
	SKILLID_KnightStone			= 0x09040005,
	SKILLID_AtalantaStone		= 0x09040006,
	SKILLID_PriestessStone		= 0x09040007,
	SKILLID_MagicianStone		= 0x09040008,
	SKILLID_ShamanStone			= 0x09040009,
	SKILLID_AssassinStone		= 0x0904000A,

	//Timers
	SKILLID_AvertScroll			= 0x09080003,
	SKILLID_AbsorptionOfGlory	= 0x09010004,
	SKILLID_PowerSiege			= 0x09010005,
	SKILLID_EvasionAdaption		= 0x09010006,
	SKILLID_ForceOrb			= 0x09010001,
	SKILLID_QUEST1				= 0x09010002,
	SKILLID_QUEST2				= 0x09010003,
	SKILLID_Booster1			= 0x09010007,
	SKILLID_Booster2			= 0x09010008,
	SKILLID_Booster3			= 0x09010009,
	SKILLID_Booster4			= 0x09010030,
	SKILLID_QUEST3				= 0x09010010,
	SKILLID_QUEST4				= 0x09010020,
	SKILLID_QUESTNew			= 0x096A0003,
	SKILLID_QUESTNewTime		= 0x096B0003,
	SKILLID_Atanasia			= 0x09020001,
	SKILLID_DeadlyEdge			= 0x09020002,
	SKILLID_AvailofEvasion		= 0x09020003,
	SKILLID_DefianceStoneA		= 0x09020004,
	SKILLID_DefianceStoneB		= 0x09020005,
	SKILLID_DefianceStoneC		= 0x09020006,
	SKILLID_MightStone			= 0x09040010,
	SKILLID_EternalLife			= 0x09080001,
	SKILLID_FatalEdge			= 0x09080002,
	SKILLID_Rivera				= 0x09080004,
	SKILLID_BlessCastleBuff2	= 0x096A0004,
	SKILLID_BlessCastleBuff3	= 0x096A0005,




	SKILLID_BellatraGold		= 0x096A0061,
	SKILLID_BellatraSilver		= 0x096A0062,
	SKILLID_BellatraBronze		= 0x096A0063,


	SKILLID_EnchantWeaponLightningAoE = 0x09990001,


};

struct ActiveSkill
{
	union
	{
		struct
		{
			BYTE		cSkillTinyID;	//tiny skill id (0x00 - 0xFF)
			BYTE		cLevel;			//1-based
			USHORT		sParam;			//skill param
		};

		int iValue;
	};

	ActiveSkill() {}
	ActiveSkill(int iValue) : iValue(iValue) {}
};

struct RecordSkill
{
	BYTE		baSkillPoints[20];
	WORD		waSkillMastery[20];
	BYTE		baShortKey[20];
	WORD		waSelectSkill[2];
};

struct SkillBox
{
	RECT								BoxRect;
	RECT								GageRect;
};



struct SkillBase
{
	char                                szName[64];					//Skill name
	char                                szDescription[150];			//Skill description
	int                                 iSkillLevelRequired;		//Levels available
	int                                 iaSkillUseStamina[2];		//0 = stamina value static, 1 = increase value each level skill
	int                                 iSkillMasteryBase;
	int                                 iSkillMasteryPerLevel;
	int                                 iaSkillElements[3];			//Skill Element Attribute (not really used)
	unsigned int                        eSkillItemAllowed[8];		//Up to 8 available weapons
	unsigned long						dwFunctionPointer;			//(sUSESKILL *pSkill); //Function pointer
	int									eSkillID;					//code
	int                          eSkillType;					//Available locations
	unsigned long						dwUseManaPointer;			//Mana Pointer

	int                                 iSkillIndex;				//Skill index (don't touch this as this is set from file)
};
























struct Skill
{
	char                                szName[32];					//Skill name
	DWORD                               iID;						//Skill code
	char                                szFileName[32];				//Skill file name
	char                                szIconName[32];				//Maintenance type skill file
	int                                 iFlag;						//Flags available (1 = active, 2 = active, received via party buff, 3 = use iSkillId for full skill name)
	BOOL                                bCanUse;					//Point allocable flag
	int                                 iLevel;						//Points assigned to skill (Point)
	int                                 ShortKey;					//Function key
	int                                 MousePosi;					//Skill mouse position
	int                                 Position;
	int                                 iUseTime;					//Usage time (the total duration)
	int                                 iCheckTime;					//Check the usage time (the time elapsed in seconds x 70)
	int                                 GageLength;					//Mastery gauge length /* delay value*/
	int                                 GageLength2;				//Mastery gauge length /* delay value*/
	int                                 Mastery;					//Mastery increase number
	int                                 UseSKillIncreCount;			//Count increase when using skill
	int                                 UseSkillCount;				//Skill Use Value (this increases UseSkillMastery)
	int 								UseSkillMastery;			//Value that increases when used (when fully mastered, there is no addition delay)
	int                                 UseSkillMasteryGage;		//Gauge that goes up when in use
	int                                 UseSkillFlag;				//1 is used 0 is not available
	int                                 PlusState[5];				//plus
	int                                 Time[2];					//0 Skill start time 1 Max time
	union
	{
		int                             PartyFlag;					//Flex applied to party members
		int                             Parameter;					//Custom parameters for this skill
	};

	int									ElementIndex;				//Which attribute is in effect (for force orbs only)

	int                                 MatIcon;

	int                                 IconAlpha;
	int                                 IconPosiX;
	int                                 IconTime;
	int                                 IconFlag;

	DWORD                               dwSkillTarget;
	int                                 SummonFlag;

	SkillBase							sSkillInfo;
};

struct UseSkill
{
	Skill		saUseSkill[20];
	SkillBox	saSkillBox[21];
	Skill	  * pLeftSkill;
	Skill	  * pRightSkill;
	int			iSkillPoint;			//Keywords: Skill point, SP
	int			iSkillPointTier4;		//Keywords: Elite point, SP
};

struct PacketSkillBuffStatus : Packet
{
	ID		iTargetID;

	USHORT	uConcealTimeLeft;
	BYTE	bConcealLevel1Based;

	USHORT	uDemonBaneTimeLeft;
	BYTE	bDemonBaneLevel1Based;

	USHORT	uAlasTimeLeft;
	BYTE	bAlasLevel1Based;

	USHORT	uHolyValorTimeLeft;
	BYTE	bHolyValorLevel1Based;

	USHORT	uHolyBodyTimeLeft;
	BYTE	bHolyBodyLevel1Based;

	USHORT	uDrasticSpiritTimeLeft;
	BYTE	bDrasticSpiritLevel1Based;

	USHORT	uForceOfNatureTimeLeft;
	BYTE	bForceOfNatureLevel1Based;

	USHORT	uInpesTimeLeft;
	BYTE	bInpesLevel1Based;

	USHORT	uAdventMigalTimeLeft;
	BYTE	bAdventMigalLevel1Based;

	USHORT	uHallOfValhallaTimeLeft;
	BYTE	bHallOfValhallaLevel1Based;

	USHORT	uTriumphOfValhallaTimeLeft;
	BYTE	bTriumphOfValhallaLevel1Based;

	USHORT	uVirtualLifeTimeLeft;
	BYTE	bVirtualLifeLevel1Based;

	USHORT	uVirtualRegenTimeLeft;
	BYTE	bVirtualRegenLevel1Based;

	USHORT	uVenomThornTimeLeft;
	BYTE	bVenomLevel1Based;

	USHORT  uElectricalOverloadTimeLeft;
	BYTE	bElectricalOverloadLevel1Based;

	USHORT  uEnchantTimeLeft;
	BYTE	bEnchantLevel1Based;

	UINT	bDataHash; //hash for level and data only. Ignoring timers. This is to detect difference
};

/// <summary>
/// Note: if adding new field, make sure to update SKILLBUFFCONTAINER_LENGTH
/// </summary>
struct PacketSkillBuffStatusContainer : Packet
{
	ID				  iTargetID;
	USHORT			  sCount;
	PacketSkillBuffStatus	  saBuff[100]; //100 * 12 = 1200
};

struct PacketSkillCast : Packet
{
	//WParam = dwSkillCode|(point<<8);
	//LParam = lpCurPlayer->dwObjectSerial;
	//SParam = Param1;
	//EParam = Param2;



	union
	{
		ID  iTargetID;	//LParam
		int LParam;
	};

	union
	{
		int iSkillCode;	//SkillTinyID | (Skill Level << 8)
		int WParam;
	};

	union
	{
		int iParam1;	//SParam
		int SParam;
	};

	union
	{
		int iParam2;	//EParam
		int EParam;
	};
};

//struct PacketSkillCast : Packet
//{
//	union
//	{
//		int						iClass;
//		int						iSkillLevel1Based;
//	};
//
//	ActiveSkill					sActiveSkill; //int
//
//	union
//	{
//		int						iID;
//		int						iParam1; //SParam
//	};
//
//	int							iParam2; //EParam
//};


struct PacketUseSkillBase : Packet
{
	ESkillID		  iSkillID;
	BYTE			  iSkillLevel1Based;
	short			  sTimeLeftSec;
	ECharacterClass	  eCasterCharacterClass;   //character class of caster
	unsigned int	  iParameter;
	ID				  lCasterID;
};

struct PacketUseSkillSingle : PacketUseSkillBase
{
	int				  lTargetID;
};

struct PacketUseSkillParty : PacketUseSkillBase
{
	int				  iUserCount;
	ID				  aiUserID[16];
};


const Rectangle2D saSkillPage2Button[4]
{
	//Skill T5A1
	{ 23, 108, 39, 36 },
	//Skill T5A2
{ 62, 152, 39, 36 },
//Skill T5A3
{ 72, 72, 39, 36 },
//Skill T5A4
{ 132, 82, 39, 36 },
};

/* See sinSkill.h
{ "Storm Javelin", //SkillName
"Throws two javelins with massive strength, damaging all within its path", //SkillDoc
50,//RequireLevel
48, //UseStamina
2,  //RequireMastery
70, //Element
3,  //UseWeaponCode
{ 0, 0, 0 }, //Element

{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None }, //UseWeaponCode
0x005219A0, //Function pointer

SKILLID_StormJavelin, //CODE

SKILLTYPE_All, //USECODE

0x00994CE0, //UseMana
0 }, //SkillNum
*/

static Skill saSkill[MAX_SKILLS] =
{
	{ "³ë¸Ö¾îÅÃ ", 0x11111111, "Skill_Normal" },

	//Mechanician Skills - Tier 1
{ "ÀÍ½ºÆ®¸²½¯µå", SKILLID_ExtremeShield, "TM10 Ex_shield", "Mc1_E_SHIELD.tga" },
{ "¸ÞÄ«´Ð¹ü", SKILLID_MechanicBomb, "TM12 M_bomb" },
{ "ÇÇÁöÄÃ ¾÷¼Ù", SKILLID_ElectricalOverload, "TM14 P_attribute", "Mc1_P_ABSORB.tga" },
{ "Æ÷ÀÌÁð ¾îÆ®¸®ºäÆ®", SKILLID_PhysicalAbsorb, "TM17 Ph_absorb", "Mc1_P_ABSORB.tga" },

//Tier 2
{ "¶ó¿îµå ÀÓÆåÆ®", SKILLID_GreatSmash, "TM20 G_Smash" },
{ "¸Æ½Ã¸¶ÀÌÁî ", SKILLID_Maximize, "TM23 Maximize", "Mc2_MAXIMIZE.tga" },
{ "¿ÀÅä¸ÞÀÌ¼Ç", SKILLID_Automation, "TM26 Automation", "Mc2_AUTOMATION.tga" },
{ "½ºÆÄÅ©", SKILLID_Spark, "TM30 Spark" },

//Tier 3
{ "¸ÞÅ» ¾Æ¸Ó", SKILLID_MetalArmor, "TM40 M_Armor", "Mc3_M_ARMOR.tga" },
{ "±×·£µå ½º¸Þ½¬", SKILLID_GrandSmash, "TM43 Grand_Smash" },
{ "¸ÞÄ«´Ð ¿þÆÝ", SKILLID_MechanicWeaponMastery, "TM44 M_Weapon" },
{ "½ºÆÄÆ® ½¯µå", SKILLID_SparkShield, "TM50 S_Shield", "Mc3_S_SHIELD.tga" },

//Tier 4
{ "ÀÓÇÃ·¹ÀÌ¼Ç", SKILLID_Impulsion, "TM60 Impulsion" },
{ "ÄÄÇÃ·¹ÀÌ¼Ç", SKILLID_Compulsion, "TM63 Compulsion", "Mc4_COMPULSION.tga" },
{ "¸Þ³×Æ½ ÇÇ¾î", SKILLID_MagneticSphere, "TM66 M_Sphere", "Mc4_M_SPHERE.tga" },
{ "¸ÞÅ» °ñ·½", SKILLID_MetalGolem, "TM70 M_Golem", "Mc4_M_GOLEM.tga" },

//Tier 5
{ "·Ñ¸µ ½º¸Å½¬", SKILLID_Rupture, "TM100 Rupture" },
{ "·£µå ¸¶ÀÌ´×", SKILLID_ParasitShot, "TM103 Parasit_Shot" },
{ "ÇÏÀÌÆÛ ¼Ò´Ð", SKILLID_HardeningShield, "TM106 Hardening_Shield" },
{ "ÆÄ¿ö ÀÎÇÚ½º", SKILLID_MagneticDischarge, "TM110 Magnetic_Discharge" },

//Fighter Skills - Tier 1
{ "¹èÆ² ¸¶½ºÅÍ¸®", SKILLID_MeleeMastery, "TF10 M_mastery" },
{ "ÆÄÀÌÅÍ ¾îÆ®¸®ºäÆ®", SKILLID_Recuperation, "TF12 F_attribute" },
{ "·¹ÀÌºù", SKILLID_Raving, "TF14 raving" },
{ "ÀÓÆÑÆ®", SKILLID_DoubleImpact, "TF17 impact" },

//Tier 2
{ "Æ®¸®ÇÃ ÀÓÆÑÆ®", SKILLID_TripleImpact, "TF20 T_Impact" },
{ "ºÎ·çÆ² ½ºÀ®", SKILLID_BrutalSwing, "TF23 B_Swing" },
{ "·Î¾î", SKILLID_WarCry, "TF26 WarCry" },
{ "·¹ÀÌÁö¿Àºê Á¦Å©¶÷", SKILLID_RageofZecram, "TF30 R_Zecram" },

//Tier 3
{ "ÄÁ¼¾Æ®·¹ÀÌ¼Ç", SKILLID_DemonsBane, "TF46 S_Axe", "Ft3_S_AXE.tga" },
{ "¾îº¥Â¡ Å©·¡½¬", SKILLID_AvengingCrash, "TF43 A_Crash" },
{ "½ºÀ§ÇÁ¿¢½º", SKILLID_ConcentrationSA, "TF40 Concentration", "Ft3_CONCENTRATION.tga" },
{ "º» Å©·¡½¬", SKILLID_BoneCrash, "TF50 B_Crash" },

//Tier 4
{ "µð½ºÅä¸®¾î", SKILLID_Destroyer, "TF60 Destoryer" },
{ "¹ö¼­Ä¿", SKILLID_Berserker, "TF63 Berserker", "Ft4_BERSERKER.tga" },
{ "½ÎÀÌÅ¬·Ð ½ºÆÄÀÌÅ©", SKILLID_CycloneStrike, "TF66 C_Strike" },
{ "ºÎ½ºÆ® Çï½º", SKILLID_Resilience, "TF70 B_Health" },

//Tier 5
{ "´Ù¿î Èý", SKILLID_SurvivalInstinct, "TF100 Survival_Instinct" },
{ "ÆÄ¿ö ´ë½Ã", SKILLID_SeismicImpact, "TF103 Sesmic_Impact" },
{ "¸ðÅ» ºí·Î¿ì", SKILLID_RuthlessofZecram, "TF106 Ruthless_Zecram" },
{ "ºí·¯µð ¹ö¼­Ä¿", SKILLID_Whirlwind, "TF110 Whirlwind" },

//Pikeman Skills - Tier 1
{ "ÆÄÀÌÅ©À©µå", SKILLID_PikeWind, "TP10 P_wind" },
{ "¾ÆÀÌ½º ¾îÆ®¸®ºäÆ®", SKILLID_IceAttribute_Old, "TP12 I_attribute" },
{ "Å©¸®Æ¼ÄÃ Èý", SKILLID_PreciseHit, "TP14 Cri_hit" },
{ "Á¡ÇÎÅ©·¡½¬", SKILLID_JumpingCrash, "TP17 J_Crash" },

//Tier 2
{ "±×¶ó¿îµåÆÄÀÌÅ©", SKILLID_GroundPike, "TP20 G_Pike" },
{ "Åä³×ÀÌµµ", SKILLID_Tornado, "TP23 Tornado" },
{ "¿þÆÝµðÆæ½º¸¶½ºÅÍ¸®", SKILLID_WeaponBlockMastery, "TP26 W_D_Mastery" },
{ "ÀÍ½ºÆæ¼Ç", SKILLID_Expansion, "TP30 Expasion" },

//Tier 3
{ "º£³ð ½ºÇÇ¾î", SKILLID_VenomSpear, "TP40 V_Spear" },
{ "ºí·¹ÀÌµå¿Àºêºí·¹ÀÌÁî", SKILLID_Vanish, "TP43 Vanish", "Pk3_VANISH.tga" },
{ "»þÇÁ¸¶½ºÅÍ¸®", SKILLID_CriticalMastery, "TP46 C_Mastery" },
{ "Ã¼ÀÎ·£½º", SKILLID_ChainLance, "TP50 C_Lance" },

//Tier 4
{ "¾î½Ø½Å ¾ÆÀÌ", SKILLID_AssassinsEye, "TP60 A_Eye", "Pk4_A_EYE.tga" },
{ "Â÷Â¡ ½ºÆ®¶óÀÌÅ©", SKILLID_ChargingStrike, "TP63 C_Strike" },
{ "º£ÀÌ±×", SKILLID_Vague, "TP66 Vague", "Pk4_VAGUE.tga" },
{ "»õµÍ¿ì ¸¶½ºÅÍ", SKILLID_ShadowMaster, "TP70 S_Master" },

//Tier 5
{ "´í½Ì ¸®ÆÛ", SKILLID_RingofSpears, "TP100 Ring_Spears" },
{ "ÆÄÀÌ³Î ½ºÇÇ¾î", SKILLID_Focus, "TP103 Focus" },
{ "¿¥ÇÃ¸®ÆÄÀÌµå", SKILLID_DeathMaster, "TP106 Death_Master" },
{ "»çÀÌµå ½ºÇÉ ¾îÅÃ", SKILLID_Twister, "TP110 Twister" },

//Archer Skills - Tier 1
{ "½ºÄ«¿ô È£Å©", SKILLID_ScoutHawk, "TA10 S_hawk", "Ac1_S_HAWK.tga" },
{ "½´ÆÃ ¸¶½ºÅÍ¸®", SKILLID_ShootingMastery, "TA12 S_mastery" },
{ "À©µå ¾Ö·Î¿ì", SKILLID_WindArrow, "TA14 W_arrow" },
{ "ÆÛÆåÆ® ¿¡ÀÓ", SKILLID_PerfectAim, "TA17 P_aim" },

//Tier 2
{ "µð¿Â½º ¾ÆÀÌ", SKILLID_DionsEye, "TA20 D_Eye" },
{ "ÆÈÄÜ", SKILLID_Falcon, "TA23 Falcon", "Ac2_FALCON.tga" },
{ "¾Ö·Î¿ì ¿Àºê·¹ÀÌÁö", SKILLID_ArrowofRage, "TA26 A_Rage" },
{ "¾Æ¹ß¶õÃ¼", SKILLID_Avalanche, "TA30 Avalanchie" },

//Tier 3
{ "¿¤·¹¸àÅ»¼¦", SKILLID_ElementalShot, "TA40 E_Shot" },
{ "°ñµçÆÈÄÜ", SKILLID_GoldenFalcon, "TA43 G_Falcon", "Ac3_G_FALCON.tga" },
{ "¹ü ¼¦", SKILLID_BombShot, "TA46 B_Shot" },
{ "ÆÛÆ÷·¹ÀÌ¼Ç", SKILLID_Perforation, "TA50 Perforation" },

//Tier 4
{ "¸®ÄÝ ¿ï¹ö¸°", SKILLID_RecallWolverine, "TA60 R_Wolverin", "Ac4_R_WOLVERIN.tga" },
{ "ÀÌº£ÀÌ¼Ç ¸¶½ºÅÍ¸®", SKILLID_EvasionMastery, "TA63 E_Mastery" },
{ "ÇÇ´Ð½º ¼¦", SKILLID_PhoenixShot, "TA66 P_Shot" },
{ "Æ÷½º ¿Àºê ³»Ãò·²", SKILLID_ForceofNature, "TA70 F_O_Nature", "Ac4_F_O_NATURE.tga" },

//Tier 5
{ "½ã´õ·çÇÁ", SKILLID_LethalSight, "TA100 Lethal_Sight" },
{ "ÀÌº£ÀÌµå¼¦", SKILLID_FierceWind, "TA103 Fierce_Wind" },
{ "³×ÀÌÃÄ ½ºÇÃ·¡½¬", SKILLID_EntalingRoots, "TA106 Entailing_Roots" },
{ "¼­Å¬Æ®·¦", SKILLID_Bombardment, "TA110 Bombardment" },

//Assassin Skills - Tier 1
{ "½ºÆÃ°Å", SKILLID_Stinger, "TA10 Stingger" },
{ "·¯´× Èý", SKILLID_RunningHit, "TA12 R_Hit" },
{ "µà¾ó¼Òµå ¸¶½ºÅÍ¸®", SKILLID_DualWieldMastery, "TA14 D_Mastery" },
{ "À§½ºÇÁ", SKILLID_Wisp, "TA17 Wisp", "As1_WISP.tga" },

//Tier 2
{ "º£³ð ½ð", SKILLID_VenomThorn, "TA20 V_Throne", "As2_VENOM.tga" },
{ "¾Ë¶ó½º", SKILLID_Alas, "TA23 Alas", "As2_ALAS.tga" },
{ "¼Ò¿ï ¼îÅ©", SKILLID_SoulShock, "TA26 S_Shock" },
{ "¾îÅÃ ¸¶½ºÅÍ¸®", SKILLID_DodgeMastery, "TA30 A_Mastery" },

//Tier 3
{ "¼Ò¾î ¼Òµå", SKILLID_SoreBlade, "TA40 S_Sword" },
{ "ºñÆ® ¾÷", SKILLID_GustSlash, "TA43 B_Up" },
{ "ÀÎÆä½º", SKILLID_Inpes, "TA46 Inpes", "As3_INPES.tga" },
{ "ºí¶óÀÎµå", SKILLID_Blind, "TA50 Blind", "As3_BLIND.tga" },

//Tier 4
{ "ÇÁ·Î½ºÆ® À©µå", SKILLID_Cripple, "TA60 F_Wind" },
{ "ÆäÀÌÆ² ¸¶½ºÅÍ¸®", SKILLID_AssassinMastery, "TA63 F_Mastery" },
{ "Æú·çÆ¼µå", SKILLID_Polluted, "TA66 Polluted" },
{ "ÆäÀÌ½ºÆÃ ½¦µµ¿ì", SKILLID_NinjaShadow, "TA70 P_Shadow" },

//Tier 5
{ "½¦µµ¿ì ¹ü", SKILLID_ShadowBlink, "TS80 J_Bomb" },
{ "¶óÀÌÂ¡ ½½·¡½¬", SKILLID_RisingSlash, "TS83 R_Slash" },
{ "¹ÙÀÌ¿Ã·±½º ½ºÅÇ", SKILLID_ViolentStub, "TS86 V_Stab" },
{ "½ºÅè", SKILLID_ShadowStorm, "TS90 Storm" },

//Knight Skills - Tier 1
{ "½º¿ö½ººí·¹½ºÆ®", SKILLID_SwordBlast, "MN10 S_Blast" },
{ "È¦¸® ¹Ùµð", SKILLID_HolyBody, "MN12 H_Body", "Kt1_H_BODY.tga" },
{ "ÇÇÁöÄÃÆ®·¹ÀÌ´×", SKILLID_PhysicalTraining, "MN14 P_Traning" },
{ "´õºí Å©·¡½¬", SKILLID_DoubleCrash, "MN17 D_Crash" },

//Tier 2
{ "È¦¸® ¹ë·¯", SKILLID_HolyValor, "MN20 H_Valor", "Kt2_H_VALOR.tga" },
{ "ºê·£µð½¬", SKILLID_Brandish, "MN23 Brandish" },
{ "ÇÇ¾î½Ì", SKILLID_Piercing, "MN26 Piercing" },
{ "µå·¹½ºÆ½ ½ºÇÇ¸´", SKILLID_DrasticSpirit, "MN30 D_Spirit", "Kt2_D_SPIRIT.tga" },

//Tier 3
{ "ÇÁ·¹ÀÓ ºê·£µð½¬", SKILLID_SwordMastery, "MN40 S_Mastery" },
{ "µð¹ÙÀÎÀÎÈ¦·¹ÀÌ¼Ç", SKILLID_DivineShield, "MN43 D_Inhalation", "Kt3_D_INHALATION.tga" },
{ "È¦¸®ÀÎÄµÅ×ÀÌ¼Ç", SKILLID_HolyIncantation, "MN46 H_Incantation" },
{ "±×·£µå Å©·Î½º", SKILLID_GrandCross, "MN50 G_Cross" },

//Tier 4
{ "½º¿öµå ¿Àºê Àú½ºÆ½½º", SKILLID_SwordofJustice, "MN60 S_O_Justice" },
{ "°«µé¸® ½¯µå", SKILLID_GodlyShield, "MN63 G_Shield", "Kt4_G_SHIELD.tga" },
{ "°« ºí·¹½º", SKILLID_GodsBlessing, "MN66 G_Bless", "Kt4_G_BLESS.tga" },
{ "µð¹ÙÀÎ ÇÇ¾î½Ì", SKILLID_DivinePiercing, "MN70 D_Piercing" },

//Tier 5
{ "¼Ò¿ï ºê·¹ÀÌÄ¿", SKILLID_HolyConviction, "MN100 Holy_Conviction" },
{ "Å©·¹¼¾Æ® ¹®", SKILLID_DivineInquisiton, "MN103 Divine_Inquisiton" },
{ "¼¼ÀÎÆ® ºí·¹ÀÌµå", SKILLID_GloriousShield, "MN106 Glorious_Shield" },
{ "È¦¸® º£³×µñ", SKILLID_DivineCross, "MN110 Divine_Cross" },

//Atalanta Skills - Tier 1
{ "½¯µå ½ºÆ®¶óÀÌÅ©", SKILLID_ShieldStrike, "MA10 S_Strike" },
{ "ÆÄ¸®³ª ", SKILLID_Farina, "MA12 Farina" },
{ "µå·ÎÀ×¸¶½ºÅÍ¸®", SKILLID_ThrowingMastery, "MA14 T_Mastery" },
{ "ºñ°Å ½ºÇÇ¾î", SKILLID_BiggerSpear, "MA17 V_Spear" },

//Tier 2
{ "À©µð", SKILLID_Windy, "MA20 Windy", "At2_WINDY.tga" },
{ "Æ®À§½ºÆ® Àçºí¸°", SKILLID_WindJavelin, "MA23 T_Javelin" },
{ "¼Ò¿ï ½âÄ¿", SKILLID_SoulSucker, "MA26 S_Sucker" },
{ "ÆÄÀÌ¾î Àçºí¸°", SKILLID_FireJavelin, "MA30 F_Javelin" },

//Tier 3
{ "½ºÇÃ¸´ ÀÚº§¸°", SKILLID_SplitJavelin, "MA40 Split_Javelin" },
{ "Æ®¶óÀÌ¾öÇÁ¿Àºê¹ßÇÒ¶ó", SKILLID_TriumphofValhalla, "MA43 T_Of_Valhalla", "At3_T_O_VALHALLA.tga" },
{ "¶óÀÌÆ®´×Àçºí¸°", SKILLID_LightningJavelin, "MA46 L_Javelin" },
{ "½ºÅè Àçºí¸°", SKILLID_StormJavelin, "MA50 Storm_Javelin" },

//Tier 4
{ "Çï ¿Àºê ¹ßÇÒ¶ó", SKILLID_HallofValhalla, "MA60 H_O_Valhalla", "At4_H_O_VALHALLA.tga" },
{ "¿¢½º ¶óÁö", SKILLID_ExtremeRage, "MA63 X_Rage" },
{ "ÇÁ·Î½ºÆ® Àçºí¸°", SKILLID_FrostJavelin, "MA66 F_Javelin", "At4_F_JAVELIN.tga" },
{ "º¥Ã¾", SKILLID_Vengeance, "MA70 Vengeance" },

//Tier 5
{ "¼­¸Õ ¾Æ¸£Äí´Ù", SKILLID_SummonTiger, "MA100 S_Tiger" },
{ "Å»¶ó¸®¾Æ", SKILLID_GoldenApple, "MA103 Golden_Apple" },
{ "°¶·°½Ã Äò", SKILLID_PlagueJavelin, "MA106 Plague_Javelin" },
{ "½º´ÏÇÇ ÇÇ¾î", SKILLID_ComboJavelin, "MA110 Combo_Javelin" },

//Priestess Skills - Tier 1
{ "Èú¸µ", SKILLID_Healing, "MP10 Healing" },
{ "È¦¸®º¼Æ® ", SKILLID_HolyBolt, "MP12 HolyBolt" },
{ "¸ÖÆ¼½ºÆÄÅ©", SKILLID_MultiSpark, "MP14 M_Spark" },
{ "È¦¸®¸¶ÀÎµå", SKILLID_HolyMind, "MP17 HolyMind" },

//Tier 2
{ "¸ÞµðÅ×ÀÌ¼Ç", SKILLID_Meditation, "MP20 Meditation" },
{ "µð¹ÙÀÎ¶óÀÌÆ®´×", SKILLID_DivineLightning, "MP23 D_Lightning" },
{ "È¦¸® ¸®ÇÃ·º¼Ç", SKILLID_HolyReflection, "MP26 H_Reflection", "Pr2_H_REFLECTION.tga" },
{ "±×·¹ÀÌÆ® Èú¸µ", SKILLID_GrandHealing, "MP30 G_Healing" },

//Tier 3
{ "ºñ°Å º¼", SKILLID_VigorBall, "MP40 V_Ball" },
{ "¸®Àú·º¼Ç", SKILLID_Ressurection, "MP43 Resurrection" },
{ "ÀÍ½ºÆÃ¼Ç", SKILLID_Extinction, "MP46 Extinction" },
{ "¹öÃò¾ó ¶óÀÌÇÁ", SKILLID_VirtualLife, "MP50 V_Life", "Pr3_V_LIFE.tga" },

//Tier 4
{ "±Û·¡¼Å ½ºÆÄÀÌÅ©", SKILLID_GlacialSpike, "MP60 G_Spike" },
{ "¸®Á¦³Ê·¹ÀÌ¼Ç ÇÊµå", SKILLID_VirtualRegen, "MP63 R_Field", "Pr4_R_FIELD.tga" },
{ "Ã¼ÀÎ ¶óÀÌÆ®´×", SKILLID_ChainLightning, "MP66 C_Lightning" },
{ "¼­¸Õ ¹«½ºÆç", SKILLID_SummonMuspell, "MP70 S_Muspell", "Pr4_S_MUSPELL.tga" },

//Tier 5
{ "½ºÇÇ¸´ ÀÓÆÑÆ®", SKILLID_IceElemental, "MP100 Ice_Elemental" },
{ "ÇÇ¾î½Ì ¾ÆÀÌ½º", SKILLID_LightningSurge, "MP103 Lightning_Surge" },
{ "¼­¸Õ ¶ó¹Ì¿¤", SKILLID_HeavenlyLight, "MP106 Heavenly_Light" },
{ "ºí·¹½Ì Å©¸®½´³ª", SKILLID_Consecration, "MP110 Consecration" },

//Magician Skills - Tier 1
{ "¾î°í´Ï", SKILLID_Agony, "MM10 Agony" },
{ "ÆÄÀÌ¾îº¼Æ® ", SKILLID_FireBolt, "MM12 FireBolt" },
{ "Á¦´Ï¾²", SKILLID_Zenith, "MM14 Zenith", "Mg1_ZENITH.tga" },
{ "ÆÄÀÌ¾îº¼", SKILLID_FireBall, "MM17 FireBall" },

//Tier 2
{ "¸ÇÅ» ¸¶½ºÅÍ¸®", SKILLID_MentalMastery, "MM20 M_Mastery" },
{ "ÄÝ·ë¿Àºê¿öÅÍ", SKILLID_Watornado, "MM23 Watornado" },
{ "ÀÎÃ¦Æ® ¿þÆù", SKILLID_EnchantWeapon, "MM26 E_Weapon", "Mg2_E_WEAPON.tga" },
{ "µ¥µå·¹ÀÌ", SKILLID_DeathRay, "MM30 D_Ray" },

//Tier 3
{ "¿¡³ÊÁö ½¯µå", SKILLID_EnergyShield, "MM40 E_Shield", "Mg3_E_SHIELD.tga" },
{ "´ÙÀÌ¾Æ½ºÆ®·ÎÇÇÁò", SKILLID_Diastrophism, "MM43 Diastrophism" },
{ "½ºÇÇ¸´ ¿¤·¹¸àÅ»", SKILLID_SpiritElemental, "MM46 S_Elemental", "Mg3_S_ELEMENTAL.tga" },
{ "´í½Ì½º¿öµå", SKILLID_DancingSword, "MM50 D_Sword", "Mg3_D_SWORD.tga" },

//Tier 4
{ "ÆÄÀÌ¾î ¿¤·¹¸àÅ»", SKILLID_FireElemental, "MM60 F_Elemental", "Mg4_F_ELEMENTAL.tga" },
{ "ÇÃ¶óÀÓ ¿þÀÌºê", SKILLID_FlameWave, "MM63 F_Wave" },
{ "µð½ºÆ®·Ï¼Ç", SKILLID_Distortion, "MM66 Distortion", "Mg4_DISTORTION.tga" },
{ "¸ÞÅ×¿À", SKILLID_Meteorite, "MM70 Meteo" },

//Tier 5
{ "ÇÁ¸®¸¶ ÀÌ±×´Ï½º", SKILLID_WizardTrance, "MM100 WizardTrance" },
{ "½Ç¶óÇË", SKILLID_StoneSkin, "MM103 Stone_Skin" },
{ "ºñ½º Å×´©½º", SKILLID_RedRay, "MM106 RedRay" },
{ "½áµå ¾Æ´Ï¸¶", SKILLID_Cataclysm, "MM110 Cataclysm" },

//Shaman Skills - Tier 1
{ "´ÙÅ© º¼Æ®", SKILLID_DarkBolt, "MS10 Darkbolt" },
{ "´ÙÅ© ¿þÀÌºê", SKILLID_DarkWave, "MS12 Darkwave" },
{ "Ä¿½º ·¹ÀÌÁö", SKILLID_Inertia, "MS14 Curselazy" },
{ "ÀÌ³Ê ÇÇ½º", SKILLID_InnerPeace, "MS17 I_peace" },

//Tier 2
{ "½ºÇÇ¸®Ãò¾ó ÇÃ·¹¾î", SKILLID_SpiritualFlare, "MS20 S_Flare" },
{ "¼Ò¿ï ¸Å³ÊÅ¬", SKILLID_SpiritualManacle, "MS23 S_Manacle" },
{ "Ã¼ÀÌ½Ì ÇåÆ®", SKILLID_ChasingHunt, "MS26 C_Hunt" },
{ "¾îµåº¥Æ®:¹Ì°¥", SKILLID_AdventMigal, "MS30 A_Migal", "Sh2_A_MIGAL.tga" },

//Tier 3
{ "·¹ÀÎ¸ÞÀÌÄ¿", SKILLID_Rainmaker, "MS40 R_Maker", "Sh3_R_MAKER.tga" },
{ "·£µå ¿Àºê °í½ºÆ®", SKILLID_PhantomCall, "MS43 L_Ghost" },
{ "È¥Æ®", SKILLID_Haunt, "MS46 Haunt" },
{ "½ºÅ©·¡Ä¡", SKILLID_Scratch, "MS50 Scratch" },

//Tier 4
{ "¸®ÄÝ ºí·¯µð³ªÀÌÆ®", SKILLID_RecallHestian, "MS60 R_Knight", "Sh4_R_KNIGHT.tga" },
{ "ÀúÁö¸ÕÆ®", SKILLID_Judgement, "MS63 Judge" },
{ "¾îµåº¥Æ®:¹Ìµå¶õ´Ù", SKILLID_AdventMidranda, "MS66 A_Midranda", "Sh4_A_MIDRANDA.tga" },
{ "¸ð´× ¿Àºê ÇÁ·¹ÀÌ", SKILLID_MourningPray, "MS70 M_pray" },

//Tier 5
{ "Å©¸®µå", SKILLID_Creed, "MS80 Creed", "Sh5_CREED.tga" },
{ "ÇÁ·¹½º ¿Àºê µ¥ÀÌ¾îÆ¼", SKILLID_PressDeity, "MS83 P_Deity" },
{ "°í½ºÆ¼ ³×ÀÏ", SKILLID_PhantomNail, "MS86 G_Nail" },
{ "ÇÏÀÌ ¸®Á¦³×·¹ÀÌ¼Ç", SKILLID_OccultLife, "MS90 H_Regene" },

//Other Skills
{ "Èí¼ö·Â »ó½Â", SKILLID_AbsorptionOfGlory, "", "wa1-1.tga" },
{ "°ø°Ý·Â »ó½Â", SKILLID_PowerSiege, "", "wa1-3.tga" },
{ "È¸ÇÇÀ² »ó½Â", SKILLID_EvasionAdaption, "", "wa1-2.tga" },
{ "¾ÆÅ¸³ª½Ã¾Æ", SKILLID_Atanasia, "", "S_Ath.tga" },
{ "µ¥µé¸® ¿¡Áö", SKILLID_DeadlyEdge, "", "S_Dea.tga" },
{ "¾îº£ÀÏ ¿Àºê ÀÌº£ÀÌµå", SKILLID_AvailofEvasion, "", "S_Eva.tga" },
{ "µðÆÄÀÌ¾ð½º ½ºÅæ", SKILLID_DefianceStoneA, "", "F_C_M.tga" },
{ "µðÆÄÀÌ¾ð½º ½ºÅæ", SKILLID_DefianceStoneB, "", "I_C_M.tga" },
{ "µðÆÄÀÌ¾ð½º ½ºÅæ", SKILLID_DefianceStoneC, "", "L_C_M.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_FighterStone, "", "Ft_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_MechanicianStone, "", "Mc_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_PikemanStone, "", "Pk_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_ArcherStone, "", "Ac_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_KnightStone, "", "Kt_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_AtalantaStone, "", "At_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_PriestessStone, "", "Mg_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_MagicianStone, "", "Pt_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_ShamanStone, "", "As_R.tga" },
{ "¸¶ÀÌÆ® ½ºÅæ", SKILLID_MightStone, "", "Sh_R.tga" },
{ "ÀÌÅÍ³Î ¶óÀÌÇÁ", SKILLID_EternalLife, "", "S_Ath.tga" },
{ "ÆäÀÌÆ² ¿¡Áö", SKILLID_FatalEdge, "", "S_Dea.tga" },
{ "¾î¹öÆ® ½ºÅ©·Ñ", SKILLID_AvertScroll, "", "S_Eva.tga" },
{ "¸®º£¶ó", SKILLID_Rivera, "", "S_Lib.tga" },
{ "»ý¸í·Â ºÎ½ºÅÍ", SKILLID_Booster1, "", "Booster_01.tga" },
{ "±â·Â ºÎ½ºÅÍ",   SKILLID_Booster2, "", "Booster_02.tga" },
{ "±Ù·Â ºÎ½ºÅÍ",   SKILLID_Booster3, "", "Booster_03.tga" },
{ "¸ÖÆ¼ ºÎ½ºÅÍ",   SKILLID_Booster4, "", "Booster_04.tga" },
};

static SkillBase saSkillBase[MAX_SKILLS] =
{
	{ "Extreme Shield", "Temporarily increases your block rating when a shield is being used",
	10, 34, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522DB0, SKILLID_ExtremeShield, SKILLTYPE_Right, 0x009926E8, 0 },

{ "Mechanic Bomb", "Throws a bomb that causes an explosion damaging enemies in that area",
12, 28, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522F70, SKILLID_MechanicBomb, SKILLTYPE_All, 0x00992788, 0 },

{ "Poison Attribute", "Permanently increases resistance to all poison property attacks",
14, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523220, SKILLID_ElectricalOverload, SKILLTYPE_All, 0x00992788, 0 },

{ "Physical Absorption", "Boosts your absorption rating for a period of time",
17, 30, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523090, SKILLID_PhysicalAbsorb, SKILLTYPE_Right, 0x00992828, 0 },

{ "Great Smash", "Delivers a powerful smash with some area damage",
20, 38, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523240, SKILLID_GreatSmash, SKILLTYPE_All, 0x009928C8, 0 },

{ "Maximize", "Increases your attack rating and attack power for a period of time when using melee weapons",
23, 41, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
0x00523390, SKILLID_Maximize, SKILLTYPE_Right, 0x00992940, 0 },

{ "Automation", "Increases your attack power and attack speed when using bows and javelins for a period of time",
26, 42, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523510, SKILLID_Automation, SKILLTYPE_Right, 0x009929E0, 0 },

{ "Spark", "Creates a spark of pure energy and releases it to damage enemies",
30, 40, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005236C0, SKILLID_Spark, SKILLTYPE_All, 0x00992A80, 0 },

{ "Metal Armor", "Increases your defense rating when using mechanician spec armors for a period of time",
40, 48, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524FB0, SKILLID_MetalArmor, SKILLTYPE_Right, 0x00992AF8, 0 },

{ "Grand Smash", "Unleashes two devastating blows to a single target with increased attack power and attack rating",
43, 44, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525000, SKILLID_GrandSmash, SKILLTYPE_All, 0x00992B70, 0 },

{ "Mechanic Weapon Mastery", "Permanently raises the base attack power of Mechanician spec weapons",
46, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525020, SKILLID_MechanicWeaponMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Spark Shield", "Increases your defense rating and deal damage to monsters when your shield blocks a hit successfully, based on damage of your Spark skill",
50, 52, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525030, SKILLID_SparkShield, SKILLTYPE_Right, 0x00992C38, 0 },

{ "Impulsion", "Creates a powerful chain of sparks that causes eletric damage against multiple targets",
60, 55, 2, 70, 3,
{ 1, 0, 0 },{ ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005252A0, SKILLID_Impulsion, SKILLTYPE_All, 0x00992D00, 0 },

{ "Compulsion", "Creates a magnetic field that pulls enemies towards the player while temporarily boosting absorption",
63, 65, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005252C0, SKILLID_Compulsion, SKILLTYPE_Right, 0x00992DA0, 0 },

{ "Magnetic Sphere", "Generates three magnetic spheres that attack nearby enemies",
66, 71, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525300, SKILLID_MagneticSphere, SKILLTYPE_Right, 0x00992E90, 0 },

{ "Metal Golem", "Summons a huge metallic golem to your aid, attacking nearby enemies",
70, 71, 2, 169, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525340, SKILLID_MetalGolem, SKILLTYPE_Right, 0x00992F80, 0 },

{ "Rupture", "Attacks the Enemy with a great force",
100, 71, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525670, SKILLID_Rupture, SKILLTYPE_All, 0x00993110, 0 },

{ "Swarm Shot", "Shots an arrow filled with parasites",
103, 65, 2, 93, 3,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525610, SKILLID_ParasitShot, SKILLTYPE_Right, 0x00992FA8, 0 },

{ "Hardened Ward", "Shield strenght is hardened by reducing your attack rating",
103, 90, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
0x00525630, SKILLID_HardeningShield, SKILLTYPE_Right, 0x00993098, 0 },

{ "Magnetic Discharge", "Releases a mechanical discharge of magnetics spheres damaging enemies around you",
110, 95, 2, 110, 5,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
0x00525690, SKILLID_MagneticDischarge, SKILLTYPE_Right, 0x00993160, 0 },

{ "Melee Mastery", "Permanently increases base attack power when using melee weapons",
10, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005237E0, SKILLID_MeleeMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Fire Attribute", "Permanently increases your resistance against fire attacks",
12, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523800, SKILLID_Recuperation, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Raving", "Goes berserk and attacks a single target with increased damage, losing HP when attacking",
14, 35, 2, 73, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523820, SKILLID_Raving, SKILLTYPE_All, 0x009952F8, 0 },

{ "DoubleImpact", "Delivers two powerful swings on a single target",
17, 37, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005239C0, SKILLID_DoubleImpact, SKILLTYPE_All, 0x00995370, 0 },

{ "Triple DoubleImpact", "Delivers devastating attack combos inflicting huge damage to the target",
20, 39, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523B10, SKILLID_TripleImpact, SKILLTYPE_All, 0x009953E8, 0 },

{ "Brutal Swing", "Unleashes a massive attack with improved critical chances",
23, 41, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523C60, SKILLID_BrutalSwing, SKILLTYPE_All, 0x00995460, 0 },

{ "WarCry", "Lets out a deafening roar to momentarily stun enemies",
26, 39, 2, 129, 4,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523DB0, SKILLID_WarCry, SKILLTYPE_Right, 0x009954D8, 0 },

{ "Rage of Zecram", "Jumps into the air smashing the axe into the ground to unleash a powerful eruption of fire that inflicts area damage",
30, 45, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Hammer, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00523ED0, SKILLID_RageofZecram, SKILLTYPE_All, 0x00995528, 0 },

{ "New Demon Buff", "New description needed",
40, 53, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525080, SKILLID_DemonsBane, SKILLTYPE_Right, 0x009955A0, 0 },

{ "Avenging Crash", "Inflicts great damage with the use of two deadly uppercuts",
43, 48, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005250D0, SKILLID_AvengingCrash, SKILLTYPE_All, 0x00995618, 0 },

{ "Swift Axe", "Greatly increases your attacking speed with axe weapons",
46, 53, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005250F0, SKILLID_ConcentrationSA, SKILLTYPE_Right, 0x00995690, 0 },

{ "Bone Crash", "Jumps into the air and delivers a smashing blow",
50, 54, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525140, SKILLID_BoneCrash, SKILLTYPE_All, 0x00995708, 0 },

{ "Destroyer", "Delivers a powerful combo with massive damage and increased critical rating",
60, 45, 2, 180, 4,
{ 1, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525390, SKILLID_Destroyer, SKILLTYPE_All, 0x00995780, 0 },

{ "Berserker", "Decreases absorb rating for an increased base attack power",
63, 60, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005253B0, SKILLID_Berserker, SKILLTYPE_Right, 0x00995820, 0 },

{ "Cyclone Strike", "Rotates and creates a huge cyclone damaging surrounding targets",
66, 55, 2, 70, 3,
{ 1, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525400, SKILLID_CycloneStrike, SKILLTYPE_All, 0x00995910, 0 },

{ "Boost Health", "Permanently raises your maximum heath capacity",
70, 0, 0, 0, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525420, SKILLID_Resilience, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Survival Instinct", "Permanently raises your rejuvenation capabilities",
103, 0, 0, 0, 0,
{ 2, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005256F0, SKILLID_SurvivalInstinct, SKILLTYPE_Passive, 0, 0 },

{ "Seismic DoubleImpact", "Deals a huge damage on the target that shakes the ground",
100, 68, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005256D0, SKILLID_SeismicImpact, SKILLTYPE_Right, 0x00995960, 0 },

{ "Might of Zecram", "Infuse your weapon with fire energy increasing damage after a number of hits. Reduces your HP with each hit",
106, 70, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525710, SKILLID_RuthlessofZecram, SKILLTYPE_Right, 0x00995A78, 0 },

{ "Raging Strikes", "Use your axe to rotate furiously damaging nearby targets",
110, 93, 2, 200, 0,
{ 2, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525730, SKILLID_Whirlwind, SKILLTYPE_Right, 0x00995B68, 0 },

{ "Pike Wind", "Creates a whirwind with the use of a scythe, pushing the enemies away",
10, 27, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524020, SKILLID_PikeWind, SKILLTYPE_Right, 0x00997558, 0 },

{ "Ice Attribute", "Permanently increases your resistance against ice attacks",
12, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524160, SKILLID_IceAttribute_Old, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Critical Hit", "Hits the enemy twice on their weak points causing increased critical damage",
14, 31, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524180, SKILLID_PreciseHit, SKILLTYPE_All, 0x009975D0, 0 },

{ "Jumping Crash", "Leaps up into the air to deliver a blow with massive power and accuracy",
17, 34, 2, 112, 4,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005242A0, SKILLID_JumpingCrash, SKILLTYPE_All, 0x009975F8, 0 },

{ "Ground Pike", "Thrusts spear into the ground freezing and slowing down enemies within range",
20, 36, 2, 115, 4,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005243F0, SKILLID_GroundPike, SKILLTYPE_Right, 0x009976E8, 0 },

{ "Tornado", "Brandishes scythe and creates a tornado, dealing serious damage to enemies within range",
23, 38, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524530, SKILLID_Tornado, SKILLTYPE_All, 0x009977B0, 0 },

{ "Weapon Block Mastery", "Permanently increases block rating when using two-handed scythes",
26, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524670, SKILLID_WeaponBlockMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Expansion", "Increases size of scythe and unleashes a devastating attack to strike the enemy",
30, 42, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524690, SKILLID_Expansion, SKILLTYPE_All, 0x00997850, 0 },

{ "Venom Spear", "Thrusts spear into the ground causing poisoned spears to consume surrounding enemies",
40, 50, 2, 124, 0,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525160, SKILLID_VenomSpear, SKILLTYPE_Right, 0x00997940, 0 },

{ "Vanish", "Vanishes into thin air, getting invisible to enemies eyes and increasing final damage to first hit",
43, 53, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525180, SKILLID_Vanish, SKILLTYPE_Right, 0x00997A08, 0 },

{ "Critical Mastery", "Permanently increases chance to inflict critical damage when using scythes",
46, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005251C0, SKILLID_CriticalMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Chain Lance", "Jumps into the air slashing a single opponent with a fast and devastating three hits combo",
50, 51, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005251D0, SKILLID_ChainLance, SKILLTYPE_All, 0x00997A80, 0 },

{ "Assassin's Eye", "Curses the target, increasing your critical damage against it temporarily",
60, 61, 2, 135, 4,
{ 1, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525430, SKILLID_AssassinsEye, SKILLTYPE_All, 0x00997B20, 0 },

{ "Charging Strike", "Charges at an enemy delivering three powerful blows on a single target",
63, 75, 2, 180, 4,
{ 1, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525450, SKILLID_ChargingStrike, SKILLTYPE_All, 0x00997BC0, 0 },

{ "Vague", "Increases evasion rating when using two handed scythes temporarily",
66, 71, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525470, SKILLID_Vague, SKILLTYPE_Right, 0x00997C38, 0 },

{ "Shadow Master", "Strikes a single target with a barrage of powerful devastating piercing attacks",
70, 51, 2, 180, 4,
{ 1, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005254B0, SKILLID_ShadowMaster, SKILLTYPE_All, 0x00997CD8, 0 },

{ "Ring of Spears", "Surround the target with spears stunning it with fear",
100, 75, 2, 136, 4,
{ 2, 0, 0 },{ ITEMTYPE_Claw, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525570, SKILLID_RingofSpears, SKILLTYPE_Right, 0x00997D00, 0 },

{ "Death Mantra", "Focus your ability increasing max attack power by reducing min attack power",
103, 70, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525590, SKILLID_Focus, SKILLTYPE_Right, 0x00997D50, 0 },

{ "Ex-Death", "Becomes the master of death by involving your weapon with the power of the shadows",
106, 93, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005255B0, SKILLID_DeathMaster, SKILLTYPE_Right, 0x00997E18, 0 },

{ "Sinister Spin", "Spins vigorously to create a deadly twister damaging nearby enemies",
110, 81, 2, 100, 0,
{ 2, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005255F0, SKILLID_Twister, SKILLTYPE_Right, 0x00997EE0, 0 },

{ "Scout Hawk", "Summons a Hawk to scout the surrounding area raising your base attack rating temporarily",
10, 25, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005247E0, SKILLID_ScoutHawk, SKILLTYPE_Right, 0x00993C00, 0 },

{ "Shooting Mastery", "Permanently raises the base attack power of bow-type weapons",
12, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524920, SKILLID_ShootingMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Wind Arrow", "Fires a wind arrow causing additional damage to normal monsters",
14, 25, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524940, SKILLID_WindArrow, SKILLTYPE_All, 0x00993CA0, 0 },

{ "Perfect Aim", "Fires an enhanced arrow that cause additional damage to demon monsters",
17, 29, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524A80, SKILLID_PerfectAim, SKILLTYPE_All, 0x00993D40, 0 },

{ "Dion's Eye", "Permanently raises the base attack rating of bow-type weapons",
20, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524BC0, SKILLID_DionsEye, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Falcon", "Summons a fierce falcon to your aid attacking all enemies that you hit with your weapon",
23, 36, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524BE0, SKILLID_Falcon, SKILLTYPE_Right, 0x00993E08, 0 },

{ "Arrow of Rage", "Launches a handful of arrows in midair that causes splash damage to certain targets",
26, 38, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524D30, SKILLID_ArrowofRage, SKILLTYPE_All, 0x00993EA8, 0 },

{ "Avalanche", "Shoots a series of multiple arrows to a single target causing massive damage",
30, 41, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00524E70, SKILLID_Avalanche, SKILLTYPE_All, 0x00993F48, 0 },

{ "Elemental Shot", "Fires a shot of fire or lightning arrow causing damage",
40, 43, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005251F0, SKILLID_ElementalShot, SKILLTYPE_All, 0x00994060, 0 },

{ "Golden Falcon", "Summons a falcon to help you by attacking and healing",
43, 52, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525210, SKILLID_GoldenFalcon, SKILLTYPE_Right, 0x00994100, 0 },

{ "Bomb Shot", "Fires two explosive arrows causing splash damage within the blast radius",
46, 46, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525260, SKILLID_BombShot, SKILLTYPE_All, 0x009941F0, 0 },

{ "Perforation", "Fires an arrow that pierces through the hardest obstacles and does splash damage",
50, 48, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525280, SKILLID_Perforation, SKILLTYPE_All, 0x00994268, 0 },

{ "Wolverine", "Summons the fierce beast Wolverine to aid attack your enemies",
60, 58, 2, 169, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005254D0, SKILLID_RecallWolverine, SKILLTYPE_Right, 0x00994380, 0 },

{ "Evasion Mastery", "Permanently increases your evasion rating when using a two-handed bow to improve capability of dodging attacks",
63, 0, 0, 0, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525520, SKILLID_EvasionMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Phoenix Shot", "Fires an arrow enchanted with the flames of Phoenix",
66, 43, 2, 70, 3,
{ 1, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525530, SKILLID_PhoenixShot, SKILLTYPE_All, 0x00994420, 0 },

{ "Force of Nature", "Amplifies and improves your offensive abilities with the use of nature's energy",
70, 56, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525550, SKILLID_ForceofNature, SKILLTYPE_Right, 0x00994510, 0 },

{ "Lethal Sight", "Focus your aim on a target at a far distance, making the next attack fatal",
100, 52, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005257A0, SKILLID_LethalSight, SKILLTYPE_Right, 0x009945D8, 0 },

{ "Mistral's Arrow", "Harness the power of wind to create an arrow that penetrates enemies and deals a heavy damage",
103, 52, 2, 82, 2,
{ 2, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525780, SKILLID_FierceWind, SKILLTYPE_Right, 0x00994538, 0 },

{ "Blooming Lotus", "Roots the enemy on the floor with the Nature's Power",
106, 57, 2, 87, 3,
{ 2, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005257C0, SKILLID_EntalingRoots, SKILLTYPE_Right, 0x00994678, 0 },

{ "Saggitarius Rain", "Bombards the enemy with a strong blast of continuous arrows",
110, 95, 2, 100, 0,
{ 2, 0, 0 },{ ITEMTYPE_Bow, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005257E0, SKILLID_Bombardment, SKILLTYPE_Right, 0x009946F0, 0 },

{ "Stinger", "Dash straight into the enemy and deal damage.",
10, 22, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525800, SKILLID_Stinger, SKILLTYPE_All, 0x00997F58, 0 },

{ "Running Hit", "Deals continuous damage to the enemy while spinning",
12, 27, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525820, SKILLID_RunningHit, SKILLTYPE_All, 0x00997FD0, 0 },

{ "Dual Wield Mastery", "Permanently raises the base attack power of dagger-type weapons",
14, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525840, SKILLID_DualWieldMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Wisp", "Reduces targeted enemy damage for a period of time",
17, 37, 2, 159, 0,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525850, SKILLID_Wisp, SKILLTYPE_All, 0x00998048, 0 },

{ "Venom Thorn", "Attacks the enemy twice rapidly with poisoned daggers",
20, 38, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525870, SKILLID_VenomThorn, SKILLTYPE_All, 0x009980C0, 0 },

{ "Alas", "Increases the evasion rating of party members for a certain duration",
23, 41, 2, 169, 0,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525890, SKILLID_Alas, SKILLTYPE_Right, 0x00998110, 0 },

{ "Soul Shock", "Performs a hard hit to the ground with daggers and stun all enemies around",
26, 39, 2, 129, 4,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005258D0, SKILLID_SoulShock, SKILLTYPE_Right, 0x00998188, 0 },

{ "Attack Mastery", "Permanently raises the base attack rating and block rating of dagger-type weapons",
30, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005258F0, SKILLID_DodgeMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Sore Blade", "Using your agility to deal two fast attacks to the enemy",
40, 45, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525900, SKILLID_SoreBlade, SKILLTYPE_All, 0x00998250, 0 },

{ "Gust Slash", "Jumps and performs a fast spinning attack, dealing massive damage to nearby enemies",
43, 48, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525920, SKILLID_GustSlash, SKILLTYPE_All, 0x009982A0, 0 },

{ "Inpes", "Increases your attack speed temporarily",
46, 48, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525940, SKILLID_Inpes, SKILLTYPE_Right, 0x009982F0, 0 },

{ "Blind", "Hide into your own shadow, turning invisible to enemies eyes for a short duration until you attack",
50, 53, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525980, SKILLID_Blind, SKILLTYPE_Right, 0x00998368, 0 },

{ "Frost Wind", "Perform two slow attacks, causing a fatal damage to the enemy",
60, 53, 2, 140, 2,
{ 1, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005259C0, SKILLID_Cripple, SKILLTYPE_All, 0x00998408, 0 },

{ "Fatal Mastery", "Permanently raises the chance to cause critical hits of dagger-type weapons",
63, 0, 0, 0, 0,
{ 1, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005259E0, SKILLID_AssassinMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Polluted", "Poison all nearby enemies and decrease their health for a period of time",
66, 55, 2, 124, 0,
{ 1, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005259F0, SKILLID_Polluted, SKILLTYPE_Right, 0x00998480, 0 },

{ "Ninja Shadow", "Moves fast to deal two powerful hits to the enemy with shadows",
70, 51, 2, 180, 4,
{ 1, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525A10, SKILLID_NinjaShadow, SKILLTYPE_All, 0x00998570, 0 },

{ "Shunpo", "Swiftly teleports onto your enemies",
100, 65, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525A30, SKILLID_ShadowBlink, SKILLTYPE_Right, 0x009985C0, 0 },

{ "Kiss of the Viper", "Catalyze the poison on enemies dealing a powerful damage",
103, 71, 2, 135, 0,
{ 2, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525A50, SKILLID_RisingSlash, SKILLTYPE_Right, 0x009986B0, 0 },

{ "Shade Kick", "Violently kicks your enemies dealing damage",
106, 90, 2, 70, 3,
{ 2, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525A70, SKILLID_ViolentStub, SKILLTYPE_Right, 0x00998700, 0 },

{ "Shadow Storm", "Engulfs your enemies into a powerfull shadow storm dealing damage",
110, 95, 2, 160, 0,
{ 2, 0, 0 },{ ITEMTYPE_Dagger, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00525A90, SKILLID_ShadowStorm, SKILLTYPE_Right, 0x00998750, 0 },

{ "Sword Blast", "Charges sword with divine energy, and blasts it away until a certain distance",
10, 22, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521320, SKILLID_SwordBlast, SKILLTYPE_All, 0x00993200, 0 },

{ "Holy Body", "Gathers holy energy to strengthen the knight against undead monsters temporarily",
12, 28, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521340, SKILLID_HolyBody, SKILLTYPE_Right, 0x009932A0, 0 },

{ "Physical Training", "Physically trains body to permanently increase stamina points",
14, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521380, SKILLID_PhysicalTraining, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Double Crash", "Performs a two hit crashing attack with increased critical chance",
17, 35, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521390, SKILLID_DoubleCrash, SKILLTYPE_All, 0x00993340, 0 },

{ "Holy Valor", "Increases party damage against undead enemies temporarily",
20, 36, 2, 169, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521620, SKILLID_HolyValor, SKILLTYPE_Right, 0x00993408, 0 },

{ "Brandish", "Brandishes the weapon causing area damage against multiple targets in front of player",
23, 38, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521660, SKILLID_Brandish, SKILLTYPE_All, 0x00993480, 0 },

{ "Piercing", "Pierces a sword through the targeted enemy with a powerful attack",
26, 41, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521680, SKILLID_Piercing, SKILLTYPE_All, 0x009934F8, 0 },

{ "Drastic Spirit", "Replenishes the knight's spirit to increase his defense for a period of time",
30, 44, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005216A0, SKILLID_DrasticSpirit, SKILLTYPE_Right, 0x00993570, 0 },

{ "Sword Mastery", "Increases base attack power when using swords permanently",
40, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521890, SKILLID_SwordMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Divine Shield", "Boosts your block rating and convert dealt undead damage into health points while using shield",
43, 46, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005218A0, SKILLID_DivineShield, SKILLTYPE_Right, 0x00993638, 0 },

{ "Holy Incantation", "Transforms the targeted monster into a pet that will fight for you in battle for a period of time",
46, 50, 2, 128, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005218F0, SKILLID_HolyIncantation, SKILLTYPE_All, 0x009936D8, 0 },

{ "Grand Cross", "Attacks the enemy with a deadly cross, causing extra damage against undead enemies",
50, 45, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521910, SKILLID_GrandCross, SKILLTYPE_All, 0x00993778, 0 },

{ "Sword of Justice", "Smash the ground with the sword to create a wave that damages all enemies within range",
60, 53, 2, 70, 3,
{ 1, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521BD0, SKILLID_SwordofJustice, SKILLTYPE_All, 0x00993980, 12 },

{ "Godly Shield", "Forms a Holy Shield blessed by God, increasing absorption rating while a shield is being used",
63, 50, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521B50, SKILLID_GodlyShield, SKILLTYPE_All, 0x00993890, 0 },

{ "God's Blessing", "Temporarily increases your base attack power with the divine blessings of God",
66, 51, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
0x00521B90, SKILLID_GodsBlessing, SKILLTYPE_Right, 0x00993908, 0 },

{ "Divine Piercing", "With the help of God's power, attacks the enemy unleashing a devastating strike with increased critical chance",
70, 51, 2, 180, 4,
{ 1, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521B30, SKILLID_DivinePiercing, SKILLTYPE_All, 0x00993818, 15 },

{ "Holy Conviction", "Gleams sacred belief to strengthen allies souls",
100, 69, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521DE0, SKILLID_HolyConviction, SKILLTYPE_Right, 0x009939A8, 0 },

{ "Divine Inquisiton", "Invokes Divine Swords to create a fortress to pierces enemies",
103, 70, 2, 70, 3,
{ 2, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521E00, SKILLID_DivineInquisiton, SKILLTYPE_Right, 0x00993A48, 0 },

{ "Glorious shield", "Forms the final shield to protect Knight",
106, 75, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Sword, ITEMTYPE_Hammer, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521E20, SKILLID_GloriousShield, SKILLTYPE_Right, 0x00993AC0, 0 },

{ "Divine Cross", "Perform the final cross with divine power",
110, 93, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None },
0x00521E40, SKILLID_DivineCross, SKILLTYPE_Right, 0x00993B38, 0 },

{ "Shield Strike", "Smites down and temporarily stuns enemies in linear range by smashing them with a shield",
10, 23, 2, 152, -1,
{ 0, 0, 0 },{ ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005213B0, SKILLID_ShieldStrike, SKILLTYPE_All, 0x009947B8, 0 },

{ "Farina", "Throws a javelin thrusting at a target's body with increased attack power and accuracy",
12, 26, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005213D0, SKILLID_Farina, SKILLTYPE_All, 0x00994858, 0 },

{ "Throwing Mastery", "Increases base attack power permanently when using Javelins",
14, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005213F0, SKILLID_ThrowingMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Bigger Spear", "Strikes down an enemy with immensely enhanced force and thrusting power",
17, 32, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Scythe, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521400, SKILLID_BiggerSpear, SKILLTYPE_All, 0x009948F8, 0 },

{ "Windy", "Temporarily increases base attack rating and attack range while using Javelin weapons",
20, 34, 2, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005216F0, SKILLID_Windy, SKILLTYPE_Right, 0x009949C0, 0 },

{ "Twist Javelin", "Throws a Javelin with increases attack power and rating, causing extra damage against demon monsters",
23, 36, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521730, SKILLID_WindJavelin, SKILLTYPE_All, 0x00994A38, 0 },

{ "Soul Sucker", "Absorbs the HP of monster and add it the casters's health points, but only when monster health points is higher than 50%",
26, 42, 2, 180, 4,
{ 0, 0, 0 },{ ITEMTYPE_Shield, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521750, SKILLID_SoulSucker, SKILLTYPE_All, 0x00994A88, 0 },

{ "Fire Javelin", "Throws a Javelin with the properties of fire causing extra damage against mutant monsters",
30, 40, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521770, SKILLID_FireJavelin, SKILLTYPE_All, 0x00994B00, 0 },

{ "Split Javelin", "Throws a Javelin that separates in mid-air and falls down damaging a single target",
40, 43, 2, 180, 4,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521930, SKILLID_SplitJavelin, SKILLTYPE_All, 0x00994BA0, 0 },

{ "Triumph of Valhalla", "Temporarily increases maximum attack power",
43, 50, 2, 169, 0,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_Sword, ITEMTYPE_Bow, ITEMTYPE_Scythe, ITEMTYPE_Hammer, ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Wand },
0x00521950, SKILLID_TriumphofValhalla, SKILLTYPE_Right, 0x00994C18, 0 },

{ "Lightning Javelin", "Throws a javelin with the properties of lightning causing extra damage against undead monsters",
46, 46, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521980, SKILLID_LightningJavelin, SKILLTYPE_All, 0x00994C90, 0 },

{ "Storm Javelin", "Throws two javelins with massive strength, damaging all within its path",
50, 48, 2, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005219A0, SKILLID_StormJavelin, SKILLTYPE_All, 0x00994CE0, 0 },

{ "Hall of Valhalla", "Temporarily raises evasion rating, automatically activating properties of Triumph of Valhalla to all friends in range",
60, 52, 2, 169, 0,
{ 1, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None },
0x00521BF0, SKILLID_HallofValhalla, SKILLTYPE_Right, 0x00994D58, 0 },

{ "Extreme Rage", "Leaps into the air throwing three explosive Javelins, damaging all enemies within the area",
63, 48, 2, 70, 3,
{ 1, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521C10, SKILLID_ExtremeRage, SKILLTYPE_All, 0x00994DD0, 0 },

{ "Frost Javelin", "Temporarily enchants the Javelin with the properties of ice, reducing enemy speed when hit",
66, 50, 2, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521C30, SKILLID_FrostJavelin, SKILLTYPE_Right, 0x00994EC0, 0 },

{ "Vengeance", "Throws two javelins with massive strength at a single target",
70, 45, 2, 70, 3,
{ 1, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521C70, SKILLID_Vengeance, SKILLTYPE_All, 0x00994F60, 0 },

{ "Nature's Call", "Summons the predator of the jungle to your aid",
100, 95, 2, 169, 0,
{ 2, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522000, SKILLID_SummonTiger, SKILLTYPE_Right, 0x00995168, 0 },

{ "Golden Apple", "Eats a sacred apple that boosts your speed and evade",
103, 52, 2, 0, 0,
{ 2, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521C70, SKILLID_GoldenApple, SKILLTYPE_Passive, 0x00995000, 0 },

{ "Plague Javelin", "Infuses your weapong with a distinct jungle poison",
106, 68, 2, 100, 4,
{ 2, 0, 0 },{ ITEMTYPE_Axe, ITEMTYPE_Claw, ITEMTYPE_Hammer, ITEMTYPE_Scythe, ITEMTYPE_Bow, ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None },
0x00521FA0, SKILLID_PlagueJavelin, SKILLTYPE_Right, 0x00994F88, 0 },

{ "Triple Threat", "Launcher a serious attack with increased power to dealing continuous damage",
110, 57, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Javelin, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521C70, SKILLID_ComboJavelin, SKILLTYPE_Right, 0x00995190, 0 },

{ "Healing", "Restore lost health points to a single target, based on your attack power",
10, 15, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521420, SKILLID_Healing, SKILLTYPE_All, 0x009968D8, 0 },

{ "Holy Bolt", "Shoots a bolt made of divine energy to disintegrate a single target",
12, 11, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521440, SKILLID_HolyBolt, SKILLTYPE_All, 0x00996950, 0 },

{ "Multi Spark", "Disperses multiple sparks which stacks and hits a single target",
14, 14, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521460, SKILLID_MultiSpark, SKILLTYPE_All, 0x009969C8, 0 },

{ "Holy Mind", "Casts a spell on target, reducing its damage against you for a period of time",
17, 19, 1, 159, 0,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521480, SKILLID_HolyMind, SKILLTYPE_All, 0x00996A68, 0 },

{ "Meditation", "Increases MP regen and boosts MP potion recovery permanently",
20, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521790, SKILLID_Meditation, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Divine Lightning", "Summons lightning bolts from the clouds to strike enemies below",
23, 21, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005217A0, SKILLID_DivineLightning, SKILLTYPE_Right, 0x00996B80, 0 }, // This is some information but not the total visual lightnings.

{ "Holy Reflection", "Creates a holy shield that reflects attacks from Undead Monsters",
26, 23, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005217C0, SKILLID_HolyReflection, SKILLTYPE_Right, 0x00996BF8, 0 },

{ "Grand Healing", "Restore health to multiple allies at once with the same group party at range, based on your attack power",
30, 27, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521800, SKILLID_GrandHealing, SKILLTYPE_Right, 0x00996C98, 0 },

{ "Vigor Ball", "Lauches two glowing spheres of powerful energy against a single target with increased damage and attack rating",
40, 24, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005219C0, SKILLID_VigorBall, SKILLTYPE_All, 0x00996D10, 0 },

{ "Resurrection", "With holy magic, revives dead players near you, not preventing them to lose gold or EXP",
43, 31, 1, 135, 0,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005219E0, SKILLID_Ressurection, SKILLTYPE_Right, 0x00996D88, 0 },

{ "Extinction", "Cast a spell to quickly reduce the health of undead monsters",
46, 28, 1, 180, 4,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521A00, SKILLID_Extinction, SKILLTYPE_Right, 0x00996E00, 0 },

{ "Virtual Life", "Increases health points of the targeted friend for a period of time",
50, 31, 1, 128, -1,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521A20, SKILLID_VirtualLife, SKILLTYPE_All, 0x00996E78, 0 },

{ "Glacial Spike", "Projects a sharp ice spikes on the ground damaging and freezing all enemies within range",
60, 35, 1, 124, 0,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521C90, SKILLID_GlacialSpike, SKILLTYPE_All, 0x00996F18, 0 },

{ "Virtual Regen", "Creates a Regeneration Field for a period of time that HP and MP regen of all friends in range",
63, 41, 1, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521CB0, SKILLID_VirtualRegen, SKILLTYPE_Right, 0x00996FE0, 0 },

{ "Chain Lightning", "Creates a chain of lightning bolts that can hit multiple enemies at once",
66, 41, 1, 140, 2,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521CD0, SKILLID_ChainLightning, SKILLTYPE_All, 0x009970A8, 0 },

{ "Summon Muspell", "Summons a Muspell who enhances priestess defensive and offensive abilities and helps by attacking nearby enemies",
70, 47, 1, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521CF0, SKILLID_SummonMuspell, SKILLTYPE_Right, 0x009971C0, 0 },

{ "Ice Elemental", "Summons an Ice Elemental to help you in battle",
100, 45, 2, 169, 0,
{ 2, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521F00, SKILLID_IceElemental, SKILLTYPE_Right, 0x009971E8, 0 },

{ "Lightning Surge", "Discharges a great sum of electricity into your target",
103, 50, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521F20, SKILLID_LightningSurge, SKILLTYPE_Right, 0x00997288, 0 },

{ "Heavenly Light", "Summons a divine light to purify allies and blind enemies",
106, 54, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521F40, SKILLID_HeavenlyLight, SKILLTYPE_Right, 0x00997300, 0 },

{ "Consecration", "Blesses party members with God's protection",
110, 60, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521F70, SKILLID_Consecration, SKILLTYPE_Right, 0x00997418, 0 },

{ "Agony", "Sacrifices your own health points to be converted to mana points",
10, 12, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005214A0, SKILLID_Agony, SKILLTYPE_Right, 0x00995C08, 0 },

{ "Fire Bolt", "Fires a small flaming bolt of hot lava to a single target",
12, 11, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521540, SKILLID_FireBolt, SKILLTYPE_All, 0x00995C58, 0 },

{ "Zenith", "Temporarily raises your resistance to all type of elemental attacks or spells",
14, 15, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521560, SKILLID_Zenith, SKILLTYPE_Right, 0x00995CD0, 0 },

{ "Fire Ball", "Creates a giant ball of flaming hot rocks to attack targeted enemies",
17, 19, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521600, SKILLID_FireBall, SKILLTYPE_All, 0x00995D48, 0 },

{ "Mental Mastery", "Increases your mana capacity and MP potion recovery permanently",
20, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521820, SKILLID_MentalMastery, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Watornado", "Creates a water tornado, causing a devastating damage to all enemies in range",
23, 21, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521830, SKILLID_Watornado, SKILLTYPE_All, 0x00995ED8, 0 },

{ "Enchant Weapon", "Temporarily increases your attack power with different elemental properties",
26, 28, 1, 128, -1,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521850, SKILLID_EnchantWeapon, SKILLTYPE_All, 0x00996018, 0 },

{ "Death Ray", "Fires a ray of pure energy to disintegrate a single target",
30, 26, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521870, SKILLID_DeathRay, SKILLTYPE_All, 0x009960B8, 0 },

{ "Energy Shield", "Generates a shield of energy, reducing damage by absorbing a portion of damage to mana points",
40, 40, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521A40, SKILLID_EnergyShield, SKILLTYPE_Right, 0x00996130, 0 },

{ "Diastrophism", "Creates a devastating explosion, damaging all enemies within the area",
43, 33, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521A80, SKILLID_Diastrophism, SKILLTYPE_Right, 0x009961D0, 0 },

{ "Spirit Elemental", "Summons a ghost to temporarily raises your base attack power",
46, 35, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521AA0, SKILLID_SpiritElemental, SKILLTYPE_Right, 0x00996248, 0 },

{ "Dancing Sword", "Summons a magical sword to attack nearby enemies attributed with Ice or Fire element",
50, 30, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521AF0, SKILLID_DancingSword, SKILLTYPE_Right, 0x00996388, 0 },

{ "Fire Elemental", "Summons a monster made of fire to help attacking neaby enemies",
60, 120, 2, 169, 0,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521D30, SKILLID_FireElemental, SKILLTYPE_Right, 0x00996450, 0 },

{ "Flame Wave", "Creates a giant wave of fire, damaging all enemies within range",
63, 32, 1, 140, 2,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521D70, SKILLID_FlameWave, SKILLTYPE_All, 0x009964F0, 0 },

{ "Distortion", "Generates a pulse of enegy, temporarily slowing down the movements of all enemies within the area",
66, 43, 1, 130, 6,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521D90, SKILLID_Distortion, SKILLTYPE_Right, 0x009965B8, 0 },

{ "Meteorite", "Summons a meteor shower to fall from the sky, damaging all enemies within the area",
70, 39, 1, 180, 4,
{ 1, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521DC0, SKILLID_Meteorite, SKILLTYPE_All, 0x00996658, 0 },

{ "Wizard's Trance", "Increase the Orb Effectiveness by concentrating in defenses but reduces attack rate. Only works when using Orb",
100, 54, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521EA0, SKILLID_WizardTrance, SKILLTYPE_Right, 0x009967C0, 0 },

{ "Stone Skin", "Becomes immune to any source of damage for a period of time, but becomes unable to use buffs and reduces your speed",
103, 45, 2, 203, 0,
{ 2, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521E70, SKILLID_StoneSkin, SKILLTYPE_Right, 0x00996680, 0 },

{ "Elemental Ray", "Launch a ray that defeat the enemy",
106, 50, 2, 70, 3,
{ 2, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521E90, SKILLID_RedRay, SKILLTYPE_Right, 0x00996720, 0 },

{ "Cataclysm", "Invokes the Wrath of Nature to form a series of powerful attacks",
110, 60, 2, 180, 4,
{ 2, 0, 0 },{ ITEMTYPE_Wand, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00521EC0, SKILLID_Cataclysm, SKILLTYPE_Right, 0x00996838, 0 },

{ "Dark Bolt", "Hits the enemy with an almighty bolt of dark magic created with sorcery",
10, 8, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522070, SKILLID_DarkBolt, SKILLTYPE_All, 0x009987A0, 0 },

{ "Dark Wave", "Performs a consecutive area attack with almighty bolts of dark magic",
12, 19, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522090, SKILLID_DarkWave, SKILLTYPE_All, 0x00998818, 0 },

{ "Inertia", "Throws a curse onto the enemies, slowing them down temporarily",
14, 20, 1, 130, 6,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005220B0, SKILLID_Inertia, SKILLTYPE_Right, 0x009988E0, 0 },

{ "Inner Peace", "Through mental training, increases MP potion recovery and mana capacity permanently",
17, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005220E0, SKILLID_InnerPeace, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Spiritual Flare", "Uses powerful sorcery to deal damage to all enemies nearby in the area",
20, 21, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005220F0, SKILLID_SpiritualFlare, SKILLTYPE_All, 0x009989D0, 0 },

{ "Spiritual Manacle", "Binds the target's soul with sorcery that results in a stun of the target",
23, 23, 1, 114, 4,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522110, SKILLID_SpiritualManacle, SKILLTYPE_All, 0x00998A98, 0 },

{ "Chasing Hunt", "Temporarily increases your base attack power and sight, and gatherers nearby enemies close by pulling them towards you",
26, 27, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522140, SKILLID_ChasingHunt, SKILLTYPE_Right, 0x00998AE8, 0 },

{ "Advent Migal", "Calls forth Migal to temporarily increases attack power of yourself and your party members",
30, 28, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522160, SKILLID_AdventMigal, SKILLTYPE_Right, 0x00998B60, 0 },

{ "Rainmaker", "Invoke the Gods of Rain to reduce received damage and increases your evasion rating temporarily",
40, 31, 1, 203, 0,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005221A0, SKILLID_Rainmaker, SKILLTYPE_Right, 0x00998BD8, 0 },

{ "Phantom Call", "Controlling cursed ghosts, invokes them to deal massive damage to enemies in range",
43, 33, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005221C0, SKILLID_PhantomCall, SKILLTYPE_Right, 0x00998C78, 0 },

{ "Haunt", "With help of Phantom's strength, strikes a forceful blow and absorbs damage into health points",
46, 35, 1, 169, 0,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005221E0, SKILLID_Haunt, SKILLTYPE_All, 0x00998D18, 0 },

{ "Scratch", "Claw the opponent fiercely with huge damage",
50, 32, 1, 70, 3,
{ 0, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522200, SKILLID_Scratch, SKILLTYPE_All, 0x00998DB8, 0 },

{ "Summon Hestian", "Summons a Hestian to your aid, attacking nearby enemies",
60, 41, 1, 169, 0,
{ 1, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522220, SKILLID_RecallHestian, SKILLTYPE_Right, 0x00998E30, 0 },

{ "Judgement", "Attacks with increased damage and critical chance, transforming the souls of enemies in a powerful energy beam",
63, 41, 1, 140, 2,
{ 1, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522260, SKILLID_Judgement, SKILLTYPE_All, 0x00998EF8, 0 },

{ "Advent Midranda", "Invokes Midranda to temporarily increase your attack speed",
66, 39, 1, 203, 0,
{ 1, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522280, SKILLID_AdventMidranda, SKILLTYPE_Right, 0x00998F98, 0 },

{ "Mourning Pray", "Using the sacred powers of gods, inflicts a damage to all nearby enemies",
70, 47, 1, 180, 4,
{ 1, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005222C0, SKILLID_MourningPray, SKILLTYPE_Right, 0x00999010, 0 },

{ "Creed", "Increases MP and STM of target temporarily",
100, 43, 1, 128, -1,
{ 2, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x005222E0, SKILLID_Creed, SKILLTYPE_Right, 0x009990B0, 0 },

{ "Press Deity", "Summons a powerful force from the darkness to damage your enemies in area",
103, 45, 2, 140, 2,
{ 2, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522300, SKILLID_PressDeity, SKILLTYPE_Right, 0x00999150, 0 },

{ "Phantom Nail", "Calls upon ghosts to scratch your enemies with powerful dark force",
106, 54, 2, 70, 3,
{ 2, 0, 0 },{ ITEMTYPE_Phantom, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522320, SKILLID_PhantomNail, SKILLTYPE_Right, 0x009991F0, 0 },

{ "Occult Life", "Harness powers of the occult increasing maximum health permanently",
110, 0, 0, 0, 0,
{ 2, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00522340, SKILLID_OccultLife, SKILLTYPE_Passive, 0x00000000, 0 },

{ "Force", "Increases your damage temporarily",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_ForceOrb, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "QUEST", "Hunt Monsters!",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_QUEST1, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "QUEST", "Hunt Monsters!",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_QUEST2, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "QUEST", "Hunt Monsters!",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_QUEST3, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "QUEST", "Hunt Monsters!",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_QUEST4, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Absorption of Glory", "Increases Absorption Rating of all Clan Members by 15%",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_AbsorptionOfGlory, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Power Siege", "Increases Attack Power of all Clan Members by 10%",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_PowerSiege, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Evasion Adaption", "Increases Evasion Rating of all Clan Members by 10%",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_EvasionAdaption, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Atanasia", "Decrease ATK Power by half and God Mode for a while",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_Atanasia, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Deadly Edge", "Add critical rating by +5% in PVP areas",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_DeadlyEdge, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Avail of Evasion", "Add evasion rating by +5% in PVP areas",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_AvailofEvasion, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Defiance Stone", "Decrease Damage by 50% of Fire Crystal Tower",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_DefianceStoneA, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Defiance Stone", "Decrease Damage by 50% of Ice Crystal Tower",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_DefianceStoneB, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Defiance Stone", "Decrease Damage by 50% of Lightning Crystal Tower",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_DefianceStoneC, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Fighters",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_FighterStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Mechanicians",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_MechanicianStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Pikemans",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_PikemanStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Archers",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_ArcherStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Knights",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_KnightStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Atalantas",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_AtalantaStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Magicians",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_PriestessStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Priestesses",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_MagicianStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Shamans",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_ShamanStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Might Stone", "Increases Final DMG Boost by +5% against Assassins",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_MightStone, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Eternal Life", "Becomes invincible for the set time. The attack power decrease in 1/2",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_EternalLife, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Fatal Edge", "+10% Final Damage Boost against Bellatra Monsters",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_FatalEdge, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Avert Scroll", "+10% of Absorption against Bellatra Monsters",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_AvertScroll, SKILLTYPE_Undefined, 0x00000000, 0 },

{ "Rivera", "Invincible character attcak. The attack power decrease in 1/2",
0, 0, 0, 0, 0,
{ 0, 0, 0 },{ ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None, ITEMTYPE_None },
0x00000000, SKILLID_Rivera, SKILLTYPE_Undefined, 0x00000000, 0 },
};


/* SOUND FILE TO SOUND ID DUMP
*
struct	sSKILL_SOUND_FILE
{
	char	*szFile;
	DWORD	dwCode;
};

sSKILL_SOUND_FILE * soundFilePointerArray = (sSKILL_SOUND_FILE *)0x9C3ED0;

for ( int i = 0; i < 257; i++ )
{
sSKILL_SOUND_FILE * file = soundFilePointerArray + i;

if ( file->szFile )
{
CLogger::GetLogger()->Log( "%s = 0x%04X", file->szFile, file->dwCode );
}
}
CLogger::GetLogger()->Flush();

wav\Effects\Skill\Learn_Skill.wav 						0x1000
wav\Effects\Skill\Tempskron\Extreme_Shield 01.wav       0x1010
wav\Effects\Skill\Tempskron\Jumping_Crash 01.wav        0x1130
wav\Effects\Skill\Tempskron\Jumping_Crash 02.wav        0x1131
wav\Effects\Skill\Tempskron\Mechanic_Bomb 01.wav        0x1020
wav\Effects\Skill\Tempskron\Mechanic_Bomb 02.wav        0x1021
wav\Effects\Skill\Tempskron\Perfect Aim 01.wav          0x1330
wav\Effects\Skill\Tempskron\Physical_Absorption 01.wav  0x1030
wav\Effects\Skill\Tempskron\Physical_Absorption 02.wav  0x1031
wav\Effects\Skill\Tempskron\Physical_Absorption 03.wav  0x1032
wav\Effects\Skill\Tempskron\Pike_Wind 01.wav            0x1200
wav\Effects\Skill\Tempskron\Scout_Hawk 01.wav           0x1310
wav\Effects\Skill\Tempskron\Scout_Hawk 02.wav           0x1311
wav\Effects\Skill\Tempskron\Wind_Arrow 01.wav           0x1320
wav\Effects\Skill\bow_skill_hit 01.wav                  0x1301
wav\Effects\Skill\bow_skill_hit 02.wav                  0x1302
wav\Effects\Skill\Skill_Swing 01.wav                    0x2000
wav\Effects\Skill\Skill_Swing 02.wav                    0x2001
wav\Effects\Skill\Tempskron\Automation 01.wav           0x2010
wav\Effects\Skill\Tempskron\Automation 02.wav           0x2011
wav\Effects\Skill\Tempskron\Automation 03.wav           0x2012
wav\Effects\Skill\Tempskron\Maximize 01.wav             0x2020
wav\Effects\Skill\Tempskron\Maximize 01.wav             0x2021
wav\Effects\Skill\Tempskron\Maximize 01.wav             0x2022
wav\Effects\Skill\Tempskron\Spark 01.wav                0x2030
wav\Effects\Skill\Tempskron\Spark 02.wav                0x2031
wav\Effects\Skill\Tempskron\Spark 03.wav                0x2032
wav\Effects\Skill\Tempskron\Ground Pike 01.wav          0x2110
wav\Effects\Skill\Tempskron\Tornado 01.wav              0x2120
wav\Effects\Skill\Tempskron\Falcon 02.wav               0x2310
wav\Effects\Skill\Tempskron\Falcon Attack 01.wav        0x2311
wav\Effects\Skill\Tempskron\Falcon Attack 02.wav        0x2312
wav\Effects\Skill\Tempskron\Avalanche 01.wav            0x2322
wav\Effects\Skill\Tempskron\Avalanche 02.wav            0x2322
wav\Effects\Skill\Tempskron\Avalanche 03.wav            0x2322
wav\Effects\Skill\Tempskron\Expansion 01.wav            0x2130
wav\Effects\Skill\Tempskron\Expansion 02.wav            0x2131
wav\Effects\Skill\Tempskron\RoZ 01.wav                  0x2211
wav\Effects\Skill\Tempskron\RoZ 02.wav                  0x2212
wav\Effects\Skill\Tempskron\RoZ 04.wav                  0x2213
wav\Effects\Skill\Tempskron\Roar 01.wav                 0x2201
wav\Effects\Skill\Tempskron\Roar 02.wav                 0x2202
wav\Effects\Skill\Tempskron\AofR 01.wav                 0x2332
wav\Effects\Skill\Morion\Casting_M.wav                  0x1800
wav\Effects\Skill\Morion\Casting_P.wav                  0x1700
wav\Effects\Skill\Morion\Agony 1.wav                    0x1810
wav\Effects\Skill\Morion\Farina 1.wav                   0x1620
wav\Effects\Skill\Morion\FireBall 1.wav                 0x1840
wav\Effects\Skill\Morion\FireBall 2.wav                 0x1841
wav\Effects\Skill\Morion\FireBolt 1.wav                 0x1820
wav\Effects\Skill\Morion\FireBolt 2.wav                 0x1821
wav\Effects\Skill\Morion\Healing 1.wav                  0x1710
wav\Effects\Skill\Morion\HolyBody 1.wav                 0x1520
wav\Effects\Skill\Morion\HolyBolt 1.wav                 0x1720
wav\Effects\Skill\Morion\HolyMind 1.wav                 0x1740
wav\Effects\Skill\Morion\MultiSpark 1.wav               0x1730
wav\Effects\Skill\Morion\ShieldSpake 1.wav              0x1610
wav\Effects\Skill\Morion\SwordBlast 1.wav               0x1510
wav\Effects\Skill\Morion\SwordBlast 2.wav               0x1511
wav\Effects\Skill\Morion\VigorSpear 1.wav               0x1640
wav\Effects\Skill\Morion\Zenith 1.wav                   0x1830
wav\Effects\Skill\Morion\HolyValor 1.wav                0x2510
wav\Effects\Skill\Morion\HolyValor 3.wav                0x2513
wav\Effects\Skill\Morion\Brandish 1.wav                 0x2520
wav\Effects\Skill\Morion\Brandish 2.wav                 0x2521
wav\Effects\Skill\Morion\Piercing 2.wav                 0x2530
wav\Effects\Skill\Morion\DrasticSpirit 1.wav            0x2540
wav\Effects\Skill\Morion\DrasticSpirit 3.wav            0x2543
wav\Effects\Skill\Morion\Windy 1.wav                    0x2610
wav\Effects\Skill\Morion\Windy 3.wav                    0x2613
wav\Effects\Skill\Morion\TwistJavelin.wav               0x2620
wav\Effects\Skill\Morion\SoulSucker 2.wav               0x2622
wav\Effects\Skill\Morion\SoulSucker 3.wav               0x2623
wav\Effects\Skill\Morion\SoulSucker 4.wav               0x2624
wav\Effects\Skill\Morion\FireJavelin 1.wav              0x2630
wav\Effects\Skill\Morion\DivineLightning 1.wav          0x2710
wav\Effects\Skill\Morion\DivineLightning 2.wav          0x2711
wav\Effects\Skill\Morion\DivineLightning 3.wav          0x2712
wav\Effects\Skill\Morion\HolyReflection 1.wav           0x2720
wav\Effects\Skill\Morion\HolyReflection 2.wav           0x2722
wav\Effects\Skill\Morion\GrandHealing 1.wav             0x2730
wav\Effects\Skill\Morion\Watornado 1.wav                0x2810
wav\Effects\Skill\Morion\Watornado 2.wav                0x2811
wav\Effects\Skill\Morion\EnchantWpn_add_C 1.wav         0x2821
wav\Effects\Skill\Morion\EnchantWpn_add_C 2.wav         0x2822
wav\Effects\Skill\Morion\EnchantWpn_add_F 1.wav         0x2823
wav\Effects\Skill\Morion\EnchantWpn_add_F 2.wav         0x2824
wav\Effects\Skill\Morion\EnchantWpn_add_L 1.wav         0x2825
wav\Effects\Skill\Morion\EnchantWpn_add_L 2.wav         0x2826
wav\Effects\Skill\Morion\EnchantWpn_cast.wav            0x2820
wav\Effects\Skill\Morion\EnchantWpn_magic_L 1.wav       0x2827
wav\Effects\Skill\Morion\EnchantWpn_magic_L 2.wav       0x2828
wav\Effects\Skill\Morion\EnchantWpn_magic_C 1.wav       0x2840
wav\Effects\Skill\Morion\EnchantWpn_magic_C 2.wav       0x2841
wav\Effects\Skill\Morion\DeathRay 1.wav                 0x2830
wav\Effects\Skill\Tempskron\AvengingCrash 01.wav        0x3120
wav\Effects\Skill\Tempskron\ElementalShot 01.wav        0x3310
wav\Effects\Skill\Tempskron\BombShot 01.wav             0x3340
wav\Effects\Skill\Tempskron\BombShot 02.wav             0x3341
wav\Effects\Skill\Tempskron\BoneCrash 01.wav            0x3140
wav\Effects\Skill\Tempskron\ChainLance 01.wav           0x3240
wav\Effects\Skill\Tempskron\ChainLance 02.wav           0x3241
wav\Effects\Skill\Tempskron\ChainLance 03.wav           0x3242
wav\Effects\Skill\Tempskron\Concentration 01.wav        0x3110
wav\Effects\Skill\Tempskron\Concentration 02.wav        0x3111
wav\Effects\Skill\Tempskron\GoldFalcon 02.wav           0x3320
wav\Effects\Skill\Tempskron\MetalArmor 01.wav           0x3010
wav\Effects\Skill\Tempskron\Perforation 01.wav          0x3330
wav\Effects\Skill\Tempskron\SparkShield 02.wav          0x3040
wav\Effects\Skill\Tempskron\SwiftAxe 01.wav             0x3130
wav\Effects\Skill\Tempskron\SwiftAxe 02.wav             0x3131
wav\Effects\Skill\Tempskron\VenomSpear 01.wav           0x3210
wav\Effects\Skill\Tempskron\VenomSpear 02.wav           0x3211
wav\Effects\Skill\Tempskron\vanish 01.wav               0x3220
wav\Effects\Skill\Tempskron\vanish 02.wav               0x3221
wav\Effects\Skill\Morion\DancingSword 1.wav             0x3740
wav\Effects\Skill\Morion\DancingSword_atk_1.wav         0x3741
wav\Effects\Skill\Morion\DancingSword_atk_2.wav         0x3742
wav\Effects\Skill\Morion\Diastrophism 2.wav             0x3720
wav\Effects\Skill\Morion\DivineInhalation 1.wav         0x3420
wav\Effects\Skill\Morion\EnergyShield 1.wav             0x3710
wav\Effects\Skill\Morion\Extinct 1.wav                  0x3630
wav\Effects\Skill\Morion\FlameBrandish 2.wav            0x3410
wav\Effects\Skill\Morion\GrandCross 2.wav               0x3440
wav\Effects\Skill\Morion\Holy Incantation 1.wav         0x3430
wav\Effects\Skill\Morion\LightningJavelin 1.wav         0x3530
wav\Effects\Skill\Morion\LightningJavelin 2.wav         0x3531
wav\Effects\Skill\Morion\Resurrection 2.wav             0x3620
wav\Effects\Skill\Morion\SpiritElemental 1.wav          0x3730
wav\Effects\Skill\Morion\SplitJavelin 1.wav             0x3510
wav\Effects\Skill\Morion\SplitJavelin 2.wav             0x3511
wav\Effects\Skill\Morion\StormJavelin 1.wav             0x3540
wav\Effects\Skill\Morion\StormJavelin 2.wav             0x3541
wav\Effects\Skill\Morion\TriumphofValhalla 1.wav        0x3520
wav\Effects\Skill\Morion\TriumphofValhalla 3.wav        0x3521
wav\Effects\Skill\Morion\VigorBall 1.wav                0x3610
wav\Effects\Skill\Morion\VigorBall 2.wav                0x3611
wav\Effects\Skill\Morion\VirtualLife 1.wav              0x3640
wav\Effects\Skill\Tempskron\AssassinEye 01.wav          0x4210
wav\Effects\Skill\Tempskron\Berserker 01.wav            0x4120
wav\Effects\Skill\Tempskron\Charging 01.wav             0x4220
wav\Effects\Skill\Tempskron\ChargingStrike 03.wav       0x4221
wav\Effects\Skill\Tempskron\Compulsion 01.wav           0x4021
wav\Effects\Skill\Tempskron\Compulsion 05.wav           0x4022
wav\Effects\Skill\Tempskron\Compulsion 06.wav           0x4023
wav\Effects\Skill\Tempskron\CycloneStrike 01.wav        0x4130
wav\Effects\Skill\Tempskron\ForceOfNature 01.wav        0x4340
wav\Effects\Skill\Tempskron\GodsBless 01.wav            0x4430
wav\Effects\Skill\Tempskron\GodlyShield 01.wav          0x4420
wav\Effects\Skill\Tempskron\impulsion 02.wav            0x4010
wav\Effects\Skill\Tempskron\MetalGolem 01.wav           0x4040
wav\Effects\Skill\Tempskron\PhoenixShot 01.wav          0x4330
wav\Effects\Skill\Tempskron\ReCallWolverin 01.wav       0x4310
wav\Effects\Skill\Tempskron\ShadowStrike 01.wav         0x4241
wav\Effects\Skill\Tempskron\ShadowStrike 02.wav         0x4242
wav\Effects\Skill\Tempskron\ShadowStrike 03.wav         0x4243
wav\Effects\Skill\Tempskron\ShadowStrikeFinish 01.wav   0x4244
wav\Effects\Skill\Tempskron\vague 01.wav                0x4230
wav\Effects\Skill\Morion\ChainLightning 01.wav          0x4631
wav\Effects\Skill\Morion\ChainLightning 02.wav          0x4632
wav\Effects\Skill\Morion\Distortion 01.wav              0x4730
wav\Effects\Skill\Morion\DIvinePiercing 07.wav          0x4410
wav\Effects\Skill\Morion\eXtremeRage 01.wav             0x4521
wav\Effects\Skill\Morion\eXtremeRage 02.wav             0x4522
wav\Effects\Skill\Morion\eXtremeRage 03.wav             0x4523
wav\Effects\Skill\Morion\FireElemental 01.wav           0x4710
wav\Effects\Skill\Morion\FlameWave 01.wav               0x4721
wav\Effects\Skill\Morion\FlameWave 02.wav               0x4722
wav\Effects\Skill\Morion\FlameWaveA 01.wav              0x4723
wav\Effects\Skill\Morion\FrostJavelin 01.wav            0x4530
wav\Effects\Skill\Morion\GlacialSpike 01.wav            0x4611
wav\Effects\Skill\Morion\GlacialSpike 02.wav            0x4612
wav\Effects\Skill\Morion\HallOfValhalla 01.wav          0x4510
wav\Effects\Skill\Morion\Meteo 01.wav                   0x4741
wav\Effects\Skill\Morion\Meteo 02.wav                   0x4742
wav\Effects\Skill\Morion\Meteo 03.wav                   0x4743
wav\Effects\Skill\Morion\Muspell 01.wav                 0x4641
wav\Effects\Skill\Morion\Muspell 02.wav                 0x4642
wav\Effects\Skill\Morion\RegenField 01.wav              0x4621
wav\Effects\Skill\Morion\RegenField 02.wav              0x4622
wav\Effects\Skill\Morion\SwordofJustice 01.wav          0x4440
wav\Effects\Skill\Morion\Vengeance 01.wav               0x4541
wav\Effects\Skill\Morion\Vengeance 02.wav               0x4542
wav\Effects\Skill\Tempskron\Stinger 01.wav              0x4800
wav\Effects\Skill\Tempskron\RunningHit 01.wav           0x4801
wav\Effects\Skill\Tempskron\Wisp 01.wav                 0x4802
wav\Effects\Skill\Tempskron\VenomThorn 01.wav           0x4803
wav\Effects\Skill\Tempskron\Alas 01.wav                 0x4804
wav\Effects\Skill\Tempskron\SoulShork 01.wav            0x4805
wav\Effects\Skill\Tempskron\SoreSword 01.wav            0x4806
wav\Effects\Skill\Tempskron\BeatUp 01.wav               0x4807
wav\Effects\Skill\Tempskron\Infes 01.wav                0x4808
wav\Effects\Skill\Tempskron\Blind 01.wav                0x4809
wav\Effects\Skill\Tempskron\FrostWind 01.wav            0x4810
wav\Effects\Skill\Tempskron\Polluted 01.wav             0x4811
wav\Effects\Skill\Tempskron\FastingShadow 01.wav        0x4812
wav\Effects\Skill\Tempskron\ShadowBomb 01.wav           0x4813
wav\Effects\Skill\Tempskron\RisingSlash 01.wav          0x4814
wav\Effects\Skill\Tempskron\ViolenceStep 01.wav         0x4815
wav\Effects\Skill\Tempskron\Storm 01.wav                0x4816
wav\Effects\Skill\Morion\Darkbolt01.wav                 0x4820
wav\Effects\Skill\Morion\Darkbolt02.wav                 0x4821
wav\Effects\Skill\Morion\Darkwave01.wav                 0x4822
wav\Effects\Skill\Morion\Darkwave02.wav                 0x4823
wav\Effects\Skill\Morion\Curselazy01.wav                0x4825
wav\Effects\Skill\Morion\Curselazy02.wav                0x4826
wav\Effects\Skill\Morion\SFlare01.wav                   0x4827
wav\Effects\Skill\Morion\SFlare02.wav                   0x4828
wav\Effects\Skill\Morion\SManacle01.wav                 0x4829
wav\Effects\Skill\Morion\SManacle02.wav                 0x482A
wav\Effects\Skill\Morion\CHunt01.wav                    0x482B
wav\Effects\Skill\Morion\CHunt02.wav                    0x482C
wav\Effects\Skill\Morion\AMichal01.wav                  0x482D
wav\Effects\Skill\Morion\AMichal02.wav                  0x482E
wav\Effects\Skill\Morion\Rainmaker01.wav                0x482F
wav\Effects\Skill\Morion\Rainmaker02.wav                0x4830
wav\Effects\Skill\Morion\LOGhost01.wav                  0x4831
wav\Effects\Skill\Morion\LOGhost02.wav                  0x4832
wav\Effects\Skill\Morion\Haunt01.wav                    0x4833
wav\Effects\Skill\Morion\Haunt02.wav                    0x4834
wav\Effects\Skill\Morion\Scratch01.wav                  0x4835
wav\Effects\Skill\Morion\Scratch02.wav                  0x4836
wav\Effects\Skill\Morion\BloodyKinght01.wav             0x4837
wav\Effects\Skill\Morion\BloodyKinght02.wav             0x4838
wav\Effects\Skill\Morion\Judgement01.wav                0x4839
wav\Effects\Skill\Morion\Judgement02.wav                0x4840
wav\Effects\Skill\Morion\AMidranda01.wav                0x4841
wav\Effects\Skill\Morion\AMidranda02.wav                0x4842
wav\Effects\Skill\Morion\MOPray01.wav                   0x4843
wav\Effects\Skill\Morion\MOPray02.wav                   0x4844
wav\Effects\Skill\Morion\Creed01.wav                    0x4845
wav\Effects\Skill\Morion\PODeity01.wav                  0x4846
wav\Effects\Skill\Morion\GNail01.wav                    0x4847
wav\Effects\Skill\Morion\GNail02.wav                    0x4848

*/