#pragma once

#define SKILL_NONE							500
#define SKILL_HOMING						5500
#define SKILL_METEO							6000
#define SKILL_UP1							6500

#define SKILLSOUND_STOP(id)					(CALL_WITH_ARG1(0x00543DA0, id))

#define CURRENTSKILL						(*(Skill**)0x03673B80)

#define SKILLS_TIERCOUNT					(*(int*)0x0367DFA0)
#define SKILLS_TIERCOUNT_BACKUP				(*(int*)0x0367E084)
#define SKILL_INDEXBUTTON					(*(int*)0x0367DF88)
#define SKILL_SHORTCUT_CLICK				(*(int*)0x0367DF90)
#define SKILL_LEFTSIDE						(*(Skill**)0x03673B68)  //leak: sinSkill.pLeftSkill

#define USESKILLS							(*(UseSkill*)0x3670F88)	//leak: sUseSkill

#define UNITS_AREA_TARGET					((UnitData**)0x04B0D680) //leak: lpSelected_Char[42]
#define UNITS_AREA_COUNT					(*(int*)0x00A046A8)	     //leak: dmSelected_CharCnt


#define HOVEREDSKILL						(*(Skill**)0x03673B80)

struct DebuffSkill
{
	int											iClass;
	BYTE										SkillCode;
};

struct SkillData
{
	char							* pszSkillName;
	int								  iSkillCode;
};

struct SkillSoundData
{
	char							* pszSkillSoundFile;
	int								iID;
};

static SkillData saSkillData[209] = {
	{ "xxxxxxxxxxxxx", SKILLPLAYID_Undefined },						//0

	//Mechanician
	{ "ExtremeShield", SKILLPLAYID_T11 },							//1
	{ "MechanicBomb", SKILLPLAYID_T12 },							//2
	{ "PhysicalAbsorption", SKILLPLAYID_T13 },						//3
	{ "ElectricalOverload", SKILLPLAYID_T14 },						//4

	{ "roundimpact", SKILLPLAYID_T21 },								//5
	{ "Maximize", SKILLPLAYID_T22 },								//6
	{ "Automation", SKILLPLAYID_T23 },								//7
	{ "Spark", SKILLPLAYID_T24 },									//8

	{ "MetalArmor", SKILLPLAYID_T31 },								//9
	{ "GrandSmash", SKILLPLAYID_T32 },								//10
	{ "MechanicMastery", SKILLPLAYID_Undefined },					//11
	{ "SparkShield", SKILLPLAYID_T33 },								//12

	{ "IMPULSION", SKILLPLAYID_T41 },								//13
	{ "Compulsion", SKILLPLAYID_T42 },								//14
	{ "MAGNETICSPHERE", SKILLPLAYID_T43 },							//15
	{ "METALGOLEM", SKILLPLAYID_T44 },								//16

	{ "Rupture", SKILLPLAYID_T51 },									//17
	{ "ParasitShot", SKILLPLAYID_T52 },								//18
	{ "HardeningShield", SKILLPLAYID_T53 },							//19
	{ "MagneticDischarge", SKILLPLAYID_T54 },						//20

	//Fighter
	{ "MeleeMastery", SKILLPLAYID_Undefined },						//21
	{ "FireAttribute", SKILLPLAYID_Undefined },						//22
	{ "Raving", SKILLPLAYID_T11 },									//23
	{ "Impact", SKILLPLAYID_T12 },									//24

	{ "TripleImpact", SKILLPLAYID_T21 },							//25
	{ "BrutalSwing", SKILLPLAYID_T22 },								//26
	{ "Roar", SKILLPLAYID_T23 },									//27
	{ "RageofZecram", SKILLPLAYID_T24 },							//28

	{ "Concentration", SKILLPLAYID_T31 },							//29
	{ "AvangingCrash", SKILLPLAYID_T32 },							//30
	{ "SwiftAxe", SKILLPLAYID_T33 },								//31
	{ "BoneSmash", SKILLPLAYID_T34 },								//32

	{ "DESTROYER", SKILLPLAYID_T41 },								//33
	{ "BERSERKER", SKILLPLAYID_T42 },								//34
	{ "CYCLONESTRIKE", SKILLPLAYID_T43 },							//35
	{ "BoostHealth", SKILLPLAYID_Undefined },						//36

	{ "SesmicImpact", SKILLPLAYID_T51 },							//37
	{ "POWERDASH", SKILLPLAYID_T52 },								//38
	{ "MOTALBLOW", SKILLPLAYID_T53 },								//39
	{ "BLOODYBERSERKER", SKILLPLAYID_T54 },							//40

	//Pikeman
	{ "PikeWind", SKILLPLAYID_T11 },								//41
	{ "IceAttribute", SKILLPLAYID_Undefined },						//42
	{ "CriticalHit", SKILLPLAYID_T12 },								//43
	{ "JumpingCrash", SKILLPLAYID_T13 },							//44

	{ "GroundPike", SKILLPLAYID_T21 },								//45
	{ "Tornado", SKILLPLAYID_T22 },									//46
	{ "WeaponDefenceMastery", SKILLPLAYID_Undefined },				//47
	{ "Expansion", SKILLPLAYID_T23 },								//48

	{ "VenomSpear", SKILLPLAYID_T31 },								//49
	{ "Vanish", SKILLPLAYID_T32 },									//50
	{ "CriticalMastery", SKILLPLAYID_Undefined },					//51
	{ "ChainLance", SKILLPLAYID_T33 },								//52

	{ "ASSASSINEYE", SKILLPLAYID_T41 },								//53
	{ "CHARGINGSTRIKE", SKILLPLAYID_T42 },							//54
	{ "VAGUE", SKILLPLAYID_T43 },									//55
	{ "SHADOWMASTER", SKILLPLAYID_T44 },							//56

	{ "DANCINGREAPER", SKILLPLAYID_T51 },							//57
	{ "FINALSPEAR", SKILLPLAYID_T52 },								//58
	{ "AMPLIFIED", SKILLPLAYID_T53 },								//59
	{ "SIDESPINATTACK", SKILLPLAYID_T54 },							//60

	//Archer
	{ "ScoutHawk", SKILLPLAYID_T11 },								//61
	{ "ShootingMastery", SKILLPLAYID_Undefined },					//62
	{ "WindArrow", SKILLPLAYID_T12 },								//63
	{ "PerfectAim", SKILLPLAYID_T13 },								//64

	{ "DionsEye", SKILLPLAYID_Undefined },							//65
	{ "Falcon", SKILLPLAYID_T21 },									//66
	{ "ArrowofRage", SKILLPLAYID_T22 },								//67
	{ "Avalanche", SKILLPLAYID_T23 },								//68

	{ "ElementalShot", SKILLPLAYID_T31 },							//69
	{ "GoldenFalcon", SKILLPLAYID_T32 },							//70
	{ "BombShot", SKILLPLAYID_T33 },								//71
	{ "Perforation", SKILLPLAYID_T34 },								//72

	{ "RECALLWOLVERIN", SKILLPLAYID_T41 },							//73
	{ "EvasionMastery", SKILLPLAYID_Undefined },					//74
	{ "PHOENIXSHOT", SKILLPLAYID_T42 },								//75
	{ "FORCEOFNATURE", SKILLPLAYID_T43 },							//76

	{ "LethalSight", SKILLPLAYID_T51 },								//77
	{ "FierceWind", SKILLPLAYID_T52 },								//78
	{ "NATURESPLASH", SKILLPLAYID_T53 },							//79
	{ "CIRCLETRAP", SKILLPLAYID_T54 },								//80

	//Knight
	{ "SwordBlast", SKILLPLAYID_T11 },								//81
	{ "HolyBody", SKILLPLAYID_T12 },								//82
	{ "PhysicalTraning", SKILLPLAYID_Undefined },					//83
	{ "DoubleCrash", SKILLPLAYID_T13 },								//84

	{ "holyteam", SKILLPLAYID_T21 },								//85
	{ "Brandish", SKILLPLAYID_T22 },								//86
	{ "Piercing", SKILLPLAYID_T23 },								//87
	{ "DrasticSpirit", SKILLPLAYID_T24 },							//88

	{ "SwordMastery", SKILLPLAYID_Undefined },						//89
	{ "Divineinhalation", SKILLPLAYID_T31 },						//90
	{ "HolyIncantation", SKILLPLAYID_T32 },							//91
	{ "GrandCross", SKILLPLAYID_T33 },								//92

	{ "DIVINEPIERCING", SKILLPLAYID_T44 },							//93
	{ "DIVINEPIERCING2", 0x90 },									//94
	{ "DIVINEPIERCING3", 0x91 },									//95
	{ "GODLYSHIELD", SKILLPLAYID_T42 },								//96
	{ "GODSBLESS", SKILLPLAYID_T43 },								//97
	{ "SWORDOFJUSTICE", SKILLPLAYID_T41 },							//98

	{ "HolyConviction", SKILLPLAYID_T51 },							//99
	{ "DivineInquisition", SKILLPLAYID_T52 },						//100
	{ "SAINTBLADE", SKILLPLAYID_T53 },								//101
	{ "HOLYBENEDIC", SKILLPLAYID_T54 },								//102

	//Atalanta
	{ "ShieldStrike", SKILLPLAYID_T11 },							//103
	{ "Farina", SKILLPLAYID_T12 },									//104
	{ "ThrowingMastery", SKILLPLAYID_Undefined },					//105
	{ "VigorSpear", SKILLPLAYID_T13 },								//106

	{ "Windy", SKILLPLAYID_T21 },									//107
	{ "TwistJavelin", SKILLPLAYID_T22 },							//108
	{ "SoulSucker", SKILLPLAYID_T23 },								//109
	{ "FireJavelin", SKILLPLAYID_T24 },								//110

	{ "SplitJavelin", SKILLPLAYID_T31 },							//111
	{ "TriumphOfValhalla", SKILLPLAYID_T32 },						//112
	{ "LightningJavelin", SKILLPLAYID_T33 },						//113
	{ "StormJavelin", SKILLPLAYID_T34 },							//114

	{ "HALLOFVALHALLA", SKILLPLAYID_T41 },							//115
	{ "XRAGE", SKILLPLAYID_T42 },									//116
	{ "FROSTJAVELIN", SKILLPLAYID_T43 },							//117
	{ "VENGEANCE", SKILLPLAYID_T44 },								//118

	{ "ComboJavelin", SKILLPLAYID_T51 },							//119
	{ "GALAXYCOUP", SKILLPLAYID_T52 },								//120
	{ "SUMMONARCUDA", SKILLPLAYID_T53 },							//121
	{ "SNIPPYFEAR", SKILLPLAYID_T54 },								//122

	//Priestess
	{ "Healing", SKILLPLAYID_T11 },									//123
	{ "HolyBolt", SKILLPLAYID_T12 },								//124
	{ "MultiSpark", SKILLPLAYID_T13 },								//125
	{ "HolyMind", SKILLPLAYID_T14 },								//126

	{ "Meditation", SKILLPLAYID_Undefined },						//127
	{ "DivineLightning", SKILLPLAYID_T21 },							//128
	{ "HolyReflection", SKILLPLAYID_T22 },							//129
	{ "GrandHealing", SKILLPLAYID_T23 },							//130

	{ "VigorBall", SKILLPLAYID_T31 },								//131
	{ "Resurrection", SKILLPLAYID_T32 },							//132
	{ "Extinction", SKILLPLAYID_T33 },								//133
	{ "VirtualLife", SKILLPLAYID_T34 },								//134

	{ "GLACIALSPIKE", SKILLPLAYID_T41 },							//135
	{ "REGENERATIONFIELD", SKILLPLAYID_T42 },						//136
	{ "CHAINLIGHTNING", SKILLPLAYID_T43 },							//137
	{ "SUMMONMUSPELL", SKILLPLAYID_T44 },							//138

	{ "SpiritImpact", SKILLPLAYID_T51 },							//139
	{ "PiercingIce", SKILLPLAYID_T52 },								//140
	{ "SummonRamiel", SKILLPLAYID_T53 },							//141
	{ "BlessingKrishna", SKILLPLAYID_T54 },							//142

	//Magician
	{ "Agony", SKILLPLAYID_T11 },									//143
	{ "FireBolt", SKILLPLAYID_T12 },								//144
	{ "Zenith", SKILLPLAYID_T13 },									//145
	{ "FireBall", SKILLPLAYID_T14 },								//146

	{ "MentalMastery", SKILLPLAYID_Undefined },						//147
	{ "ColumnOfWater", SKILLPLAYID_T21 },							//148
	{ "EnchantWeapon", SKILLPLAYID_T22 },							//149
	{ "DeadRay", SKILLPLAYID_T23 },									//150

	{ "EnergyShield", SKILLPLAYID_T31 },							//151
	{ "Diastrophism", SKILLPLAYID_T32 },							//152
	{ "SpiritElemental", SKILLPLAYID_T33 },							//153
	{ "DancingSword", SKILLPLAYID_T34 },							//154

	{ "FIREELEMENTAL", SKILLPLAYID_T41 },							//155
	{ "FLAMEWAVE", SKILLPLAYID_T42 },								//156
	{ "DISTORTION", SKILLPLAYID_T43 },								//157
	{ "METEO", SKILLPLAYID_T44 },									//158

	{ "StoneSkin", SKILLPLAYID_T51 },								//159
	{ "RedRay", SKILLPLAYID_T52 },									//160
	{ "Primaignis", SKILLPLAYID_T53 },								//161
	{ "Thirdanima", SKILLPLAYID_T54 },								//162

	//Assassin
	{ "Stingger", SKILLPLAYID_T11 },								//163
	{ "RunningHit", SKILLPLAYID_T12 },								//164
	{ "DualSwordMastery", SKILLPLAYID_Undefined },					//165
	{ "Wisp", SKILLPLAYID_T14 },									//166

	{ "VenomThrone", SKILLPLAYID_T21 },								//167
	{ "Alas", SKILLPLAYID_T22 },									//168
	{ "SoulShock", SKILLPLAYID_T23 },								//169
	{ "AttackMastery", SKILLPLAYID_Undefined },						//170

	{ "SoreSword", SKILLPLAYID_T31 },								//171
	{ "BeatUp", SKILLPLAYID_T32 },									//172
	{ "Inpes", SKILLPLAYID_T33 },									//173
	{ "Blind", SKILLPLAYID_T34 },									//174

	{ "CrippleWind", SKILLPLAYID_T41 },								//175
	{ "CriticalMastery", SKILLPLAYID_Undefined },					//176
	{ "Polluted", SKILLPLAYID_T43 },								//177
	{ "PastingShadow", SKILLPLAYID_T44 },							//178

	{ "ShadowBomb", SKILLPLAYID_T51 },								//179
	{ "RisingSlash", SKILLPLAYID_T52 },								//180
	{ "ViolenceStab", SKILLPLAYID_T53 },							//181
	{ "Storm", SKILLPLAYID_T54 },									//182

	//Shaman
	{ "DarkBolt", SKILLPLAYID_T11 },								//183
	{ "DarkWave", SKILLPLAYID_T12 },								//184
	{ "CurseLazy", SKILLPLAYID_T13 },								//185
	{ "InnerPeace", SKILLPLAYID_Undefined },						//186

	{ "SpiritualFlare", SKILLPLAYID_T21 },							//187
	{ "SoulManacle", SKILLPLAYID_T22 },								//188
	{ "ChasingHunt", SKILLPLAYID_T23 },								//189
	{ "AdventMigal", SKILLPLAYID_T24 },								//190

	{ "RainMaker", SKILLPLAYID_T31 },								//191
	{ "LandOfGhost", SKILLPLAYID_T32 },								//192
	{ "Haunt", SKILLPLAYID_T33 },									//193
	{ "Scratch", SKILLPLAYID_T34 },									//194

	{ "RecallBloodyKnight", SKILLPLAYID_T41 },						//195
	{ "Judgement", SKILLPLAYID_T42 },								//196
	{ "AdventMidranda", SKILLPLAYID_T43 },							//197
	{ "MourningOfPray", SKILLPLAYID_T44 },							//198

	{ "Creed", SKILLPLAYID_T51 },									//199
	{ "PressOfDeity", SKILLPLAYID_T52 },							//200
	{ "GhostyNail", SKILLPLAYID_T53 },								//201
	{ "HighRegeneration", SKILLPLAYID_Undefined },					//202

	{ "JUMPATTACK", 0x98 },											//203
	{ "CONTATTACK1", 0x9A },										//204
	{ "CONTATTACK2", 0x9B },										//205
	{ "CONTATTACK3", 0x9C },										//206
	{ "CONTATTACK4", 0x9D },										//207

	{ 0, 0 },
};

static SkillSoundData saSoundData[248]
{
	{ "wav\\Effects\\Skill\\Learn_Skill.wav", 0x1000 },		//0
	{ "wav\\Effects\\Skill\\Tempskron\\Extreme_Shield 01.wav", 0x1010 },		//1
	{ "wav\\Effects\\Skill\\Tempskron\\Jumping_Crash 01.wav", 0x1130 },		//2
	{ "wav\\Effects\\Skill\\Tempskron\\Jumping_Crash 02.wav", 0x1131 },		//3
	{ "wav\\Effects\\Skill\\Tempskron\\Mechanic_Bomb 01.wav", 0x1020 },		//4
	{ "wav\\Effects\\Skill\\Tempskron\\Mechanic_Bomb 02.wav", 0x1021 },		//5
	{ "wav\\Effects\\Skill\\Tempskron\\Perfect Aim 01.wav", 0x1330 },		//6
	{ "wav\\Effects\\Skill\\Tempskron\\Physical_Absorption 01.wav", 0x1030 },		//7
	{ "wav\\Effects\\Skill\\Tempskron\\Physical_Absorption 02.wav", 0x1031 },		//8
	{ "wav\\Effects\\Skill\\Tempskron\\Physical_Absorption 03.wav", 0x1032 },		//9
	{ "wav\\Effects\\Skill\\Tempskron\\Pike_Wind 01.wav", 0x1200 },		//10
	{ "wav\\Effects\\Skill\\Tempskron\\Scout_Hawk 01.wav", 0x1310 },		//11
	{ "wav\\Effects\\Skill\\Tempskron\\Scout_Hawk 02.wav", 0x1311 },		//12
	{ "wav\\Effects\\Skill\\Tempskron\\Wind_Arrow 01.wav", 0x1320 },		//13
	{ "wav\\Effects\\Skill\\bow_skill_hit 01.wav", 0x1301 },		//14
	{ "wav\\Effects\\Skill\\bow_skill_hit 02.wav", 0x1302 },		//15
	{ "wav\\Effects\\Skill\\Skill_Swing 01.wav", 0x2000 },		//16
	{ "wav\\Effects\\Skill\\Skill_Swing 02.wav", 0x2001 },		//17
	{ "wav\\Effects\\Skill\\Tempskron\\Automation 01.wav", 0x2010 },		//18
	{ "wav\\Effects\\Skill\\Tempskron\\Automation 02.wav", 0x2011 },		//19
	{ "wav\\Effects\\Skill\\Tempskron\\Automation 03.wav", 0x2012 },		//20
	{ "wav\\Effects\\Skill\\Tempskron\\Maximize 01.wav", 0x2020 },		//21
	{ "wav\\Effects\\Skill\\Tempskron\\Maximize 01.wav", 0x2021 },		//22
	{ "wav\\Effects\\Skill\\Tempskron\\Maximize 01.wav", 0x2022 },		//23
	{ "wav\\Effects\\Skill\\Tempskron\\Spark 01.wav", 0x2030 },		//24
	{ "wav\\Effects\\Skill\\Tempskron\\Spark 02.wav", 0x2031 },		//25
	{ "wav\\Effects\\Skill\\Tempskron\\Spark 03.wav", 0x2032 },		//26
	{ "wav\\Effects\\Skill\\Tempskron\\Ground Pike 01.wav", 0x2110 },		//27
	{ "wav\\Effects\\Skill\\Tempskron\\Tornado 01.wav", 0x2120 },		//28
	{ "wav\\Effects\\Skill\\Tempskron\\Falcon 02.wav", 0x2310 },		//29
	{ "wav\\Effects\\Skill\\Tempskron\\Falcon Attack 01.wav", 0x2311 },		//30
	{ "wav\\Effects\\Skill\\Tempskron\\Falcon Attack 02.wav", 0x2312 },		//31
	{ "wav\\Effects\\Skill\\Tempskron\\Avalanche 01.wav", 0x2322 },		//32
	{ "wav\\Effects\\Skill\\Tempskron\\Avalanche 02.wav", 0x2322 },		//33
	{ "wav\\Effects\\Skill\\Tempskron\\Avalanche 03.wav", 0x2322 },		//34
	{ "wav\\Effects\\Skill\\Tempskron\\Expansion 01.wav", 0x2130 },		//35
	{ "wav\\Effects\\Skill\\Tempskron\\Expansion 02.wav", 0x2131 },		//36
	{ "wav\\Effects\\Skill\\Tempskron\\RoZ 01.wav", 0x2211 },		//37
	{ "wav\\Effects\\Skill\\Tempskron\\RoZ 02.wav", 0x2212 },		//38
	{ "wav\\Effects\\Skill\\Tempskron\\RoZ 04.wav", 0x2213 },		//39
	{ "wav\\Effects\\Skill\\Tempskron\\Roar 01.wav", 0x2201 },		//40
	{ "wav\\Effects\\Skill\\Tempskron\\Roar 02.wav", 0x2202 },		//41
	{ "wav\\Effects\\Skill\\Tempskron\\AofR 01.wav", 0x2332 },		//42
	{ "wav\\Effects\\Skill\\Morion\\Casting_M.wav", 0x1800 },		//43
	{ "wav\\Effects\\Skill\\Morion\\Casting_P.wav", 0x1700 },		//44
	{ "wav\\Effects\\Skill\\Morion\\Agony 1.wav", 0x1810 },		//45
	{ "wav\\Effects\\Skill\\Morion\\Farina 1.wav", 0x1620 },		//46
	{ "wav\\Effects\\Skill\\Morion\\FireBall 1.wav", 0x1840 },		//47
	{ "wav\\Effects\\Skill\\Morion\\FireBall 2.wav", 0x1841 },		//48
	{ "wav\\Effects\\Skill\\Morion\\FireBolt 1.wav", 0x1820 },		//49
	{ "wav\\Effects\\Skill\\Morion\\FireBolt 2.wav", 0x1821 },		//50
	{ "wav\\Effects\\Skill\\Morion\\Healing 1.wav", 0x1710 },		//51
	{ "wav\\Effects\\Skill\\Morion\\HolyBody 1.wav", 0x1520 },		//52
	{ "wav\\Effects\\Skill\\Morion\\HolyBolt 1.wav", 0x1720 },		//53
	{ "wav\\Effects\\Skill\\Morion\\HolyMind 1.wav", 0x1740 },		//54
	{ "wav\\Effects\\Skill\\Morion\\MultiSpark 1.wav", 0x1730 },		//55
	{ "wav\\Effects\\Skill\\Morion\\ShieldSpake 1.wav", 0x1610 },		//56
	{ "wav\\Effects\\Skill\\Morion\\SwordBlast 1.wav", 0x1510 },		//57
	{ "wav\\Effects\\Skill\\Morion\\SwordBlast 2.wav", 0x1511 },		//58
	{ "wav\\Effects\\Skill\\Morion\\VigorSpear 1.wav", 0x1640 },		//59
	{ "wav\\Effects\\Skill\\Morion\\Zenith 1.wav", 0x1830 },		//60
	{ "wav\\Effects\\Skill\\Morion\\HolyValor 1.wav", 0x2510 },		//61
	{ "wav\\Effects\\Skill\\Morion\\HolyValor 3.wav", 0x2513 },		//62
	{ "wav\\Effects\\Skill\\Morion\\Brandish 1.wav", 0x2520 },		//63
	{ "wav\\Effects\\Skill\\Morion\\Brandish 2.wav", 0x2521 },		//64
	{ "wav\\Effects\\Skill\\Morion\\Piercing 2.wav", 0x2530 },		//65
	{ "wav\\Effects\\Skill\\Morion\\DrasticSpirit 1.wav", 0x2540 },		//66
	{ "wav\\Effects\\Skill\\Morion\\DrasticSpirit 3.wav", 0x2543 },		//67
	{ "wav\\Effects\\Skill\\Morion\\Windy 1.wav", 0x2610 },		//68
	{ "wav\\Effects\\Skill\\Morion\\Windy 3.wav", 0x2613 },		//69
	{ "wav\\Effects\\Skill\\Morion\\TwistJavelin.wav", 0x2620 },		//70
	{ "wav\\Effects\\Skill\\Morion\\SoulSucker 2.wav", 0x2622 },		//71
	{ "wav\\Effects\\Skill\\Morion\\SoulSucker 3.wav", 0x2623 },		//72
	{ "wav\\Effects\\Skill\\Morion\\SoulSucker 4.wav", 0x2624 },		//73
	{ "wav\\Effects\\Skill\\Morion\\FireJavelin 1.wav", 0x2630 },		//74
	{ "wav\\Effects\\Skill\\Morion\\DivineLightning 1.wav", 0x2710 },		//75
	{ "wav\\Effects\\Skill\\Morion\\DivineLightning 2.wav", 0x2711 },		//76
	{ "wav\\Effects\\Skill\\Morion\\DivineLightning 3.wav", 0x2712 },		//77
	{ "wav\\Effects\\Skill\\Morion\\HolyReflection 1.wav", 0x2720 },		//78
	{ "wav\\Effects\\Skill\\Morion\\HolyReflection 2.wav", 0x2722 },		//79
	{ "wav\\Effects\\Skill\\Morion\\GrandHealing 1.wav", 0x2730 },		//80
	{ "wav\\Effects\\Skill\\Morion\\Watornado 1.wav", 0x2810 },		//81
	{ "wav\\Effects\\Skill\\Morion\\Watornado 2.wav", 0x2811 },		//82
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_add_C 1.wav", 0x2821 },		//83
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_add_C 2.wav", 0x2822 },		//84
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_add_F 1.wav", 0x2823 },		//85
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_add_F 2.wav", 0x2824 },		//86
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_add_L 1.wav", 0x2825 },		//87
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_add_L 2.wav", 0x2826 },		//88
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_cast.wav", 0x2820 },		//89
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_magic_L 1.wav", 0x2827 },		//90
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_magic_L 2.wav", 0x2828 },		//91
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_magic_C 1.wav", 0x2840 },		//92
	{ "wav\\Effects\\Skill\\Morion\\EnchantWpn_magic_C 2.wav", 0x2841 },		//93
	{ "wav\\Effects\\Skill\\Morion\\DeathRay 1.wav", 0x2830 },		//94
	{ "wav\\Effects\\Skill\\Tempskron\\AvengingCrash 01.wav", 0x3120 },		//95
	{ "wav\\Effects\\Skill\\Tempskron\\ElementalShot 01.wav", 0x3310 },		//96
	{ "wav\\Effects\\Skill\\Tempskron\\BombShot 01.wav", 0x3340 },		//97
	{ "wav\\Effects\\Skill\\Tempskron\\BombShot 02.wav", 0x3341 },		//98
	{ "wav\\Effects\\Skill\\Tempskron\\BoneCrash 01.wav", 0x3140 },		//99
	{ "wav\\Effects\\Skill\\Tempskron\\ChainLance 01.wav", 0x3240 },		//100
	{ "wav\\Effects\\Skill\\Tempskron\\ChainLance 02.wav", 0x3241 },		//101
	{ "wav\\Effects\\Skill\\Tempskron\\ChainLance 03.wav", 0x3242 },		//102
	{ "wav\\Effects\\Skill\\Tempskron\\Concentration 01.wav", 0x3110 },		//103
	{ "wav\\Effects\\Skill\\Tempskron\\Concentration 02.wav", 0x3111 },		//104
	{ "wav\\Effects\\Skill\\Tempskron\\GoldFalcon 02.wav", 0x3320 },		//105
	{ "wav\\Effects\\Skill\\Tempskron\\MetalArmor 01.wav", 0x3010 },		//106
	{ "wav\\Effects\\Skill\\Tempskron\\Perforation 01.wav", 0x3330 },		//107
	{ "wav\\Effects\\Skill\\Tempskron\\SparkShield 02.wav", 0x3040 },		//108
	{ "wav\\Effects\\Skill\\Tempskron\\SwiftAxe 01.wav", 0x3130 },		//109
	{ "wav\\Effects\\Skill\\Tempskron\\SwiftAxe 02.wav", 0x3131 },		//110
	{ "wav\\Effects\\Skill\\Tempskron\\VenomSpear 01.wav", 0x3210 },		//111
	{ "wav\\Effects\\Skill\\Tempskron\\VenomSpear 02.wav", 0x3211 },		//112
	{ "wav\\Effects\\Skill\\Tempskron\\vanish 01.wav", 0x3220 },		//113
	{ "wav\\Effects\\Skill\\Tempskron\\vanish 02.wav", 0x3221 },		//114
	{ "wav\\Effects\\Skill\\Morion\\DancingSword 1.wav", 0x3740 },		//115
	{ "wav\\Effects\\Skill\\Morion\\DancingSword_atk_1.wav", 0x3741 },		//116
	{ "wav\\Effects\\Skill\\Morion\\DancingSword_atk_2.wav", 0x3742 },		//117
	{ "wav\\Effects\\Skill\\Morion\\Diastrophism 2.wav", 0x3720 },		//118
	{ "wav\\Effects\\Skill\\Morion\\DivineInhalation 1.wav", 0x3420 },		//119
	{ "wav\\Effects\\Skill\\Morion\\EnergyShield 1.wav", 0x3710 },		//120
	{ "wav\\Effects\\Skill\\Morion\\Extinct 1.wav", 0x3630 },		//121
	{ "wav\\Effects\\Skill\\Morion\\FlameBrandish 2.wav", 0x3410 },		//122
	{ "wav\\Effects\\Skill\\Morion\\GrandCross 2.wav", 0x3440 },		//123
	{ "wav\\Effects\\Skill\\Morion\\Holy Incantation 1.wav", 0x3430 },		//124
	{ "wav\\Effects\\Skill\\Morion\\LightningJavelin 1.wav", 0x3530 },		//125
	{ "wav\\Effects\\Skill\\Morion\\LightningJavelin 2.wav", 0x3531 },		//126
	{ "wav\\Effects\\Skill\\Morion\\Resurrection 2.wav", 0x3620 },		//127
	{ "wav\\Effects\\Skill\\Morion\\SpiritElemental 1.wav", 0x3730 },		//128
	{ "wav\\Effects\\Skill\\Morion\\SplitJavelin 1.wav", 0x3510 },		//129
	{ "wav\\Effects\\Skill\\Morion\\SplitJavelin 2.wav", 0x3511 },		//130
	{ "wav\\Effects\\Skill\\Morion\\StormJavelin 1.wav", 0x3540 },		//131
	{ "wav\\Effects\\Skill\\Morion\\StormJavelin 2.wav", 0x3541 },		//132
	{ "wav\\Effects\\Skill\\Morion\\TriumphofValhalla 1.wav", 0x3520 },		//133
	{ "wav\\Effects\\Skill\\Morion\\TriumphofValhalla 3.wav", 0x3521 },		//134
	{ "wav\\Effects\\Skill\\Morion\\VigorBall 1.wav", 0x3610 },		//135
	{ "wav\\Effects\\Skill\\Morion\\VigorBall 2.wav", 0x3611 },		//136
	{ "wav\\Effects\\Skill\\Morion\\VirtualLife 1.wav", 0x3640 },		//137
	{ "wav\\Effects\\Skill\\Tempskron\\AssassinEye 01.wav", 0x4210 },		//138
	{ "wav\\Effects\\Skill\\Tempskron\\Berserker 01.wav", 0x4120 },		//139
	{ "wav\\Effects\\Skill\\Tempskron\\Charging 01.wav", 0x4220 },		//140
	{ "wav\\Effects\\Skill\\Tempskron\\ChargingStrike 03.wav", 0x4221 },		//141
	{ "wav\\Effects\\Skill\\Tempskron\\Compulsion 01.wav", 0x4021 },		//142
	{ "wav\\Effects\\Skill\\Tempskron\\Compulsion 05.wav", 0x4022 },		//143
	{ "wav\\Effects\\Skill\\Tempskron\\Compulsion 06.wav", 0x4023 },		//144
	{ "wav\\Effects\\Skill\\Tempskron\\CycloneStrike 01.wav", 0x4130 },		//145
	{ "wav\\Effects\\Skill\\Tempskron\\ForceOfNature 01.wav", 0x4340 },		//146
	{ "wav\\Effects\\Skill\\Tempskron\\GodsBless 01.wav", 0x4430 },		//147
	{ "wav\\Effects\\Skill\\Tempskron\\GodlyShield 01.wav", 0x4420 },		//148
	{ "wav\\Effects\\Skill\\Tempskron\\impulsion 02.wav", 0x4010 },		//149
	{ "wav\\Effects\\Skill\\Tempskron\\MetalGolem 01.wav", 0x4040 },		//150
	{ "wav\\Effects\\Skill\\Tempskron\\PhoenixShot 01.wav", 0x4330 },		//151
	{ "wav\\Effects\\Skill\\Tempskron\\ReCallWolverin 01.wav", 0x4310 },		//152
	{ "wav\\Effects\\Skill\\Tempskron\\ShadowStrike 01.wav", 0x4241 },		//153
	{ "wav\\Effects\\Skill\\Tempskron\\ShadowStrike 02.wav", 0x4242 },		//154
	{ "wav\\Effects\\Skill\\Tempskron\\ShadowStrike 03.wav", 0x4243 },		//155
	{ "wav\\Effects\\Skill\\Tempskron\\ShadowStrikeFinish 01.wav", 0x4244 },		//156
	{ "wav\\Effects\\Skill\\Tempskron\\vague 01.wav", 0x4230 },		//157
	{ "wav\\Effects\\Skill\\Morion\\ChainLightning 01.wav", 0x4631 },		//158
	{ "wav\\Effects\\Skill\\Morion\\ChainLightning 02.wav", 0x4632 },		//159
	{ "wav\\Effects\\Skill\\Morion\\Distortion 01.wav", 0x4730 },		//160
	{ "wav\\Effects\\Skill\\Morion\\DIvinePiercing 07.wav", 0x4410 },		//161
	{ "wav\\Effects\\Skill\\Morion\\eXtremeRage 01.wav", 0x4521 },		//162
	{ "wav\\Effects\\Skill\\Morion\\eXtremeRage 02.wav", 0x4522 },		//163
	{ "wav\\Effects\\Skill\\Morion\\eXtremeRage 03.wav", 0x4523 },		//164
	{ "wav\\Effects\\Skill\\Morion\\FireElemental 01.wav", 0x4710 },		//165
	{ "wav\\Effects\\Skill\\Morion\\FlameWave 01.wav", 0x4721 },		//166
	{ "wav\\Effects\\Skill\\Morion\\FlameWave 02.wav", 0x4722 },		//167
	{ "wav\\Effects\\Skill\\Morion\\FlameWaveA 01.wav", 0x4723 },		//168
	{ "wav\\Effects\\Skill\\Morion\\FrostJavelin 01.wav", 0x4530 },		//169
	{ "wav\\Effects\\Skill\\Morion\\GlacialSpike 01.wav", 0x4611 },		//170
	{ "wav\\Effects\\Skill\\Morion\\GlacialSpike 02.wav", 0x4612 },		//171
	{ "wav\\Effects\\Skill\\Morion\\HallOfValhalla 01.wav", 0x4510 },		//172
	{ "wav\\Effects\\Skill\\Morion\\Meteo 01.wav", 0x4741 },		//173
	{ "wav\\Effects\\Skill\\Morion\\Meteo 02.wav", 0x4742 },		//174
	{ "wav\\Effects\\Skill\\Morion\\Meteo 03.wav", 0x4743 },		//175
	{ "wav\\Effects\\Skill\\Morion\\Muspell 01.wav", 0x4641 },		//176
	{ "wav\\Effects\\Skill\\Morion\\Muspell 02.wav", 0x4642 },		//177
	{ "wav\\Effects\\Skill\\Morion\\RegenField 01.wav", 0x4621 },		//178
	{ "wav\\Effects\\Skill\\Morion\\RegenField 02.wav", 0x4622 },		//179
	{ "wav\\Effects\\Skill\\Morion\\SwordofJustice 01.wav", 0x4440 },		//180
	{ "wav\\Effects\\Skill\\Morion\\Vengeance 01.wav", 0x4541 },		//181
	{ "wav\\Effects\\Skill\\Morion\\Vengeance 02.wav", 0x4542 },		//182
	{ "wav\\Effects\\Skill\\Tempskron\\Stinger 01.wav", 0x4800 },		//183
	{ "wav\\Effects\\Skill\\Tempskron\\RunningHit 01.wav", 0x4801 },		//184
	{ "wav\\Effects\\Skill\\Tempskron\\Wisp 01.wav", 0x4802 },		//185
	{ "wav\\Effects\\Skill\\Tempskron\\VenomThorn 01.wav", 0x4803 },		//186
	{ "wav\\Effects\\Skill\\Tempskron\\Alas 01.wav", 0x4804 },		//187
	{ "wav\\Effects\\Skill\\Tempskron\\SoulShork 01.wav", 0x4805 },		//188
	{ "wav\\Effects\\Skill\\Tempskron\\SoreSword 01.wav", 0x4806 },		//189
	{ "wav\\Effects\\Skill\\Tempskron\\BeatUp 01.wav", 0x4807 },		//190
	{ "wav\\Effects\\Skill\\Tempskron\\Infes 01.wav", 0x4808 },		//191
	{ "wav\\Effects\\Skill\\Tempskron\\Blind 01.wav", 0x4809 },		//192
	{ "wav\\Effects\\Skill\\Tempskron\\CrippleWind 01.wav", 0x4810 },		//193
	{ "wav\\Effects\\Skill\\Tempskron\\Polluted 01.wav", 0x4811 },		//194
	{ "wav\\Effects\\Skill\\Tempskron\\FastingShadow 01.wav", 0x4812 },		//195
	{ "wav\\Effects\\Skill\\Tempskron\\ShadowBomb 01.wav", 0x4813 },		//196
	{ "wav\\Effects\\Skill\\Tempskron\\RisingSlash 01.wav", 0x4814 },		//197
	{ "wav\\Effects\\Skill\\Tempskron\\ViolenceStep 01.wav", 0x4815 },		//198
	{ "wav\\Effects\\Skill\\Tempskron\\Storm 01.wav", 0x4816 },		//199
	{ "wav\\Effects\\Skill\\Morion\\Darkbolt01.wav", 0x4820 },		//200
	{ "wav\\Effects\\Skill\\Morion\\Darkbolt02.wav", 0x4821 },		//201
	{ "wav\\Effects\\Skill\\Morion\\Darkwave01.wav", 0x4822 },		//202
	{ "wav\\Effects\\Skill\\Morion\\Darkwave02.wav", 0x4823 },		//203
	{ "wav\\Effects\\Skill\\Morion\\Curselazy01.wav", 0x4825 },		//204
	{ "wav\\Effects\\Skill\\Morion\\Curselazy02.wav", 0x4826 },		//205
	{ "wav\\Effects\\Skill\\Morion\\SFlare01.wav", 0x4827 },		//206
	{ "wav\\Effects\\Skill\\Morion\\SFlare02.wav", 0x4828 },		//207
	{ "wav\\Effects\\Skill\\Morion\\SManacle01.wav", 0x4829 },		//208
	{ "wav\\Effects\\Skill\\Morion\\SManacle02.wav", 0x482A },		//209
	{ "wav\\Effects\\Skill\\Morion\\CHunt01.wav", 0x482B },		//210
	{ "wav\\Effects\\Skill\\Morion\\CHunt02.wav", 0x482C },		//211
	{ "wav\\Effects\\Skill\\Morion\\AMichal01.wav", 0x482D },		//212
	{ "wav\\Effects\\Skill\\Morion\\AMichal02.wav", 0x482E },		//213
	{ "wav\\Effects\\Skill\\Morion\\Rainmaker01.wav", 0x482F },		//214
	{ "wav\\Effects\\Skill\\Morion\\Rainmaker02.wav", 0x4830 },		//215
	{ "wav\\Effects\\Skill\\Morion\\LOGhost01.wav", 0x4831 },		//216
	{ "wav\\Effects\\Skill\\Morion\\LOGhost02.wav", 0x4832 },		//217
	{ "wav\\Effects\\Skill\\Morion\\Haunt01.wav", 0x4833 },		//218
	{ "wav\\Effects\\Skill\\Morion\\Haunt02.wav", 0x4834 },		//219
	{ "wav\\Effects\\Skill\\Morion\\Scratch01.wav", 0x4835 },		//220
	{ "wav\\Effects\\Skill\\Morion\\Scratch02.wav", 0x4836 },		//221
	{ "wav\\Effects\\Skill\\Morion\\BloodyKinght01.wav", 0x4837 },		//222
	{ "wav\\Effects\\Skill\\Morion\\BloodyKinght02.wav", 0x4838 },		//223
	{ "wav\\Effects\\Skill\\Morion\\Judgement01.wav", 0x4839 },		//224
	{ "wav\\Effects\\Skill\\Morion\\Judgement02.wav", 0x4840 },		//225
	{ "wav\\Effects\\Skill\\Morion\\AMidranda01.wav", 0x4841 },		//226
	{ "wav\\Effects\\Skill\\Morion\\AMidranda02.wav", 0x4842 },		//227
	{ "wav\\Effects\\Skill\\Morion\\MOPray01.wav", 0x4843 },		//228
	{ "wav\\Effects\\Skill\\Morion\\MOPray02.wav", 0x4844 },		//229
	{ "wav\\Effects\\Skill\\Morion\\Creed01.wav", 0x4845 },		//230
	{ "wav\\Effects\\Skill\\Morion\\PODeity01.wav", 0x4846 },		//231
	{ "wav\\Effects\\Skill\\Morion\\GNail01.wav", 0x4847 },		//232
	{ "wav\\Effects\\Skill\\Morion\\GNail02.wav", 0x4848 },		//233
	{ "game\\sounds\\skills\\R5FWind01.wav", 0x4849 },		//234
	{ "game\\sounds\\skills\\R5FWind02.wav", 0x484A },		//235

	//Kills
	{ "game\\sounds\\kills\\DoubleKill.wav", 0x484B },		//236
	{ "game\\sounds\\kills\\TripleKill.wav", 0x484C },		//237
	{ "game\\sounds\\kills\\QuadraKill.wav", 0x484D },		//238
	{ "game\\sounds\\kills\\PentaKill.wav", 0x484E },		//239
	{ "game\\sounds\\kills\\ShutdownKill.wav", 0x484F },	//240
	{ "game\\sounds\\kills\\RampageKill.wav", 0x4850 },		//241
	{ "game\\sounds\\kills\\UnstoppableKill.wav", 0x4851 },		//242

	//Rupture
	{ "game\\sounds\\skills\\Rupture 1.wav", 0x4855 },	//243
	{ "game\\sounds\\skills\\Rupture 2.wav", 0x4856 },	//244

	//Ring of Spears
	{ "game\\sounds\\skills\\Ring Of Spears.wav", 0x4857 },	//245

	//Legendary Drop
	{ "game\\sounds\\Legendary_Drop.wav", 0x4950 },		//246

	{ 0,0 },
};

#define SKILLREPLACE_MAX_LINES			10
#define SKILLREPLACE_MAX_COLUMNS		10

const int iaSkillReplaceTimer[SKILLREPLACE_MAX_LINES][SKILLREPLACE_MAX_COLUMNS]
{
	//PH Absorb is a buff
	{ SKILLID_PhysicalAbsorb, SKILLID_Maximize, SKILLID_Automation, SKILLID_MetalArmor },
	{ SKILLID_ExtremeShield, SKILLID_SparkShield },
	{ SKILLID_HolyBody, SKILLID_HolyValor, SKILLID_HolyConviction },
	{ SKILLID_Falcon, SKILLID_GoldenFalcon, SKILLID_ScoutHawk },
	{ SKILLID_DivineShield, SKILLID_GodlyShield },
};

const int iaSkillPet[] =
{
	SKILLID_MetalGolem,
	SKILLID_FireElemental,
	SKILLID_RecallWolverine,
	SKILLID_SummonTiger,
	SKILLID_RecallHestian,
	SKILLID_IceElemental,
};

static int iaLethalSightTime[10]
{
	300, 300, 300, 300, 300, 300, 300, 300, 300, 300
};

enum SkillInfoLabelId
{
	AddAbsorb = 0,
	Range	  = 1,
	Duration  = 2,
};


#define SKIL_ATTACK_CHAR_MAX 42

class SkillManager
{
private:
	IMPFNC										pfnClearSkillBuff = 0x004ED280;
	IMPFNC										pfnResetSkills = 0x00504290;
	IMPFNC										pfnStartSkillEffect = 0x005742E0;
	IMPFNC										pfnGetWeaponPosition = 0x005404B0;
	IMPFNC										pfnFindNearestMonster = 0x0061F240;

	BOOL										baPetAggressive[8];

	//Skill array pointer to chain scaling
	std::map<DWORD, float>						mapSkillChainScale;

	DWORD	dmAttackCount = 0;

	//Fighter
	int											ia_fs_RageOfZecram_Radius[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_RageofZecram_BurningDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_RageofZecram_BurningChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_RageofZecram_BurningDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_BoostHealth_BoostPercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_BoostHealth_HPPotionBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_Recuperation_HPRegen[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_ConcentrationSA_ATKSpdBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_ConcentrationSA_AtkRtgBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_Destroyer_HPCost_Percent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_AvengingCrash_HPCost_Percent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_BrutalSwing_CriticalDmgBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_BrutalSwing_HPCost_Percent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_Berserker_LifeLeechPercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_Berserker_AddATKPow[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_WarCry_StunChance	[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_WarCry_StunAtkPowPerc[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_WarCry_HPRestore[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_WarCry_SPRestore[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_WarCry_HPRegen[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_WarCry_SPRegen[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_WarCry_RegenDuration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_Raving_HPCost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_fs_DemonBuff_DamageBoostVsDemon[10] = { 0,0,0,0,0,0,0,0,0,0 };

	//Mech
	int											ia_Ms_ExtremeShield_AddBlock[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_Automation_MPDrain[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_Automation_SPDrain[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_MechanicBomb_AttackRange[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_MechanicBomb_AttackSpeed[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_MechanicBomb_BurningChance[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_MechanicBomb_BurningDamagePercent[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_MechanicBomb_BurningDurationSec[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_MechanicBomb_BurningTickRateMs[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_Maximize_AddAttackSpeed[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_MetalGolem_Duration[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_WeaponMastery_AddFlat[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int											ia_Ms_Complusion_AddAbsFlat[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ms_Complusion_AddAbsPercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ms_ElectricalOverload_AttackSpeed[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ms_ElectricalOverload_Duration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ms_Spark_AttackFrameSpeed[10] = { 0,0,0,0,0,0,0,0,0,0 };

	//Archer
	int											ia_As_Perforation_AddedCrit[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_Perforation_AttackWidth[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_ForceOfNature_AddedRange[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_ScoutHawk_AddAtkRtg[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_ElementalShot_LightningRange[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_PhoenixShot_AttackRange[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_PhoenixShot_AttackWidth[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_PhoenixShot_BurningPercentOfAP[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_PhoenixShot_BurningDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_PhoenixShot_BurningChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_PhoenixShot_ChargedDamageBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_EvasionMastery_IncreaseEvasion[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_As_ForceofNature_AddATKPow[10] = { 0,0,0,0,0,0,0,0,0,0 };

	//Pikeman (4)
	int											ia_Ps_Expansion_Range[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_PolearmMastery_AddRange[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_PreciseHit_AtkPow[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_PreciseHit_AddAttackRating[10] = { 0,0,0,0,0,0,0,0,0,0 };

	int											ia_Ps_VenomSpear_PoisonNumStack[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_VenomSpear_PoisonChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_VenomSpear_PoisonDuration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_VenomSpear_PoisonDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_GroundPike_SpeedMonDown[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_Vague_AddEvasion[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ps_WeaponBlockMastery_WPBlockBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };

	// Priestess
	int											ia_Prs_VirtualLife_MPBoostPercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_VirtualLife_Duration[10] = { 0,0,0,0,0,0,0,0,0,0 };

	int											ia_Prs_VirtualRegen_HPRegen[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_VirtualRegen_MPRegen[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_VirtualRegen_Duration[10] = { 0,0,0,0,0,0,0,0,0,0 };

	int											ia_Prs_DivineLightning_Damage[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_VigorBall_Damage[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_ChainLightning_Damage[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_MultiSpark_NumSparksMin[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_SummonMuspell_DamageBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_SummonMuspell_UndeadAbsorb[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_SummonMuspell_AddEvasion[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_HolyBolt_DamageBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_Extinction_Radius[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_GlacialSpike_SpeedMonDown[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Prs_GlacialSpike_FreezePercentOfAP[10] = { 0,0,0,0,0,0,0,0,0,0 };

	//Knight
	//int										ia_Ks_SwordBlast_DamageBoost[10] = { 0,0,0,0,0,0,0,0,0,0 }; //@todo
	//int										ia_Ks_SwordBlast_DamageBoostPercentage[10] = { 0,0,0,0,0,0,0,0,0,0 }; //@todo

	int											ia_Ks_HolyValor_BuffRadius[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_Piercing_PierceRange[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_Piercing_PierceWidth[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_HolyIncantation_MaxHP[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_HolyIncantation_MaxAbs[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_HolyIncantation_MaxDMGMin[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_HolyIncantation_MaxDMGMax[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_HolyIncantation_DMGIncrease[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_DivineShield_AddBlock[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_GodsBlessing_AddATKPowPercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_DrasticSpirit_UndeadDmgReduction[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_HolyBody_HealRadius[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ks_HolyBody_DmgToHealConversion[10] = { 0,0,0,0,0,0,0,0,0,0 };


	//Atalanta
	int											ia_Ata_Windy_Range[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_ShieldStrike_StunDamage[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_ShieldStrike_StunChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_ShieldStrike_StunDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int                                         ia_Ata_Windy_AttackRate[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_Farina_DMGBoostPercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_ThrowingMastery_DMGBoost_New[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_ThrowingMastery_DMGBoostPercent_New[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_StormJavelin_AttackRange_New[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_StormJavelin_AttackWidth_New[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_LightningJavelin_Radius_New[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_LightningJavelin_StunChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_LightningJavelin_StunDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_FireJavelin_BurningChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_FireJavelin_BurningDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_FireJavelin_BurningDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_FireJavelin_BurningTickRateMs[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_HallOfValhalla_AddBlock[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_FrostJavelin_FreezeChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_FrostJavelin_FreezeDamage[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_TriumphOfValhalla_AddMaxDamagePerc[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_SoulSucker_Range[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ata_SoulSucker_LifeAbsorbCap[10] = { 0,0,0,0,0,0,0,0,0,0 };


	//Magician
	int											ia_Mgs_FireBolt_BurningChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FireBolt_BurningDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FireBolt_BurningDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FireBall_BurningChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FireBall_BurningDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FireBall_BurningDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_EnchantWeapon_FireBurningDuration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_EnchantWeapon_FireBurningTickRateMs[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_EnchantWeapon_IceFreezeDuration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_EnchantWeapon_IceFreezeSlow[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_EnchantWeapon_LightningStunDuration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_EnchantWeapon_LightningRange[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FlameWave_BurningChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FlameWave_BurningDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_FlameWave_BurningDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_Distortion_Chance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_Distortion_DamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_EnergyShield_MPDrain_New[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_MentalMastery_EW_Chain_MPDrain_New[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_DancingSword_FireBurningChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_DancingSword_FireBurningDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_DancingSword_FireBurningDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_DancingSword_IceFreezeChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_DancingSword_IceFreezeDuration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Mgs_DancingSword_IceFreezeSpeedReduction[10] = { 0,0,0,0,0,0,0,0,0,0 };

	//Assassin
	int											ia_Ass_Polluate_PoisonNumStack[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Polluate_PoisonChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Inpes_MovementSpeed[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_DodgeMastery_AddEvasive[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_DualWieldMastery_WPAtkPowBoostFlat[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_DualWieldMastery_WPAtkRtgBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_AssassinMastery_AtkSpeedBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Wisp_Curse_PercentOfAP[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Wisp_Curse_Chance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Wisp_Curse_Duration[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Wisp_Curse_SpeedReduction[10] = { 0,0,0,0,0,0,0,0,0,0 };

	int											ia_Ass_Cripple_PoisonPercentOfAP[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Cripple_PoisonChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Cripple_PoisonDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_Cripple_PoisonNumStack[10] = { 0,0,0,0,0,0,0,0,0,0 };

	int											ia_Ass_VenomThorn_PoisonDurationSec[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_VenomThorn_PoisonDamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_VenomThorn_PoisonChance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_VenomThorn_PoisonNumStack[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_VenomThorn_Duration[10] = { 0,0,0,0,0,0,0,0,0,0 };

	int											ia_Ass_SoulShock_StunAtkPowPerc[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Ass_SoulShock_StunChance[10] = { 0,0,0,0,0,0,0,0,0,0 };

	//Shaman
	int											ia_Sha_Inertia_Curse_DamagePercent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Sha_Inertia_Curse_Chance[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Sha_Inertia_Curse_DamageReduction[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Sha_Rainmaker_AddEvasion[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Sha_Rainmaker_ReduceDamage[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Sha_ChasingHunt_AtkPowBoost[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Sha_Haunt_Radius[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int											ia_Sha_Haunt_LifeStealCap[10] = { 0,0,0,0,0,0,0,0,0,0 };

	int											iLastAttackRange;

public:
	// Construction
	SkillManager();
	virtual										~SkillManager();

	BOOL										IsActiveSkill( ESkillID eSkillID );
	BOOL										IsPassiveSkill( ESkillID eSkillID );
	BOOL										IsDebuffSkill( ESkillID eSkillID );
	BOOL										IsBuffSkill( ESkillID eSkillID );
	BOOL										IsSummonSkill( ESkillID eSkillID );

	BOOL										ResetSkills( int iNum );

	BOOL										IsActiveBuff( UINT uBuffID );

	BOOL										ResetSkillTimer( int iSkillID );

	BOOL										UseManaStamina( Skill * psSkill );

	float										GetChainedSkillScale( int iArrayPointer );
	int											GetLevelSkill( UINT uSkillCode );
	BOOL										IsDebuffSkill( UnitData * pcUnit, DWORD dwSkillCode );

	int											GetResistanceTime( int iTime, int iResistanceID );

	void										StunEffect( UnitData * pcUnitData, int iTime );

	void										CancelForceOrb( int iForceOrbID );

	void										GetInfoBoxSize( struct sSKILLBOX * psSkillBox, int iColumn, int iIcon );

	void										AddSkillInfo( int iColumn, const char * pszText, ... );
	BOOL										AddSkillInfoToBox ( int iSkillID, int iSkillLevelZeroBased );

	int											GetRadian2D( int x1, int y1, int x2, int y2 );

	void										SetDynLight( Point3D & point3D, int iR, int iG, int iB, int iA, int iPower, int DecPower=10 );
	void										SetDynLight( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iPower, int DecPower=10 );
	void										StartSkillEffect( int iX, int iY, int iZ, int iAngleX, int iAngleY, int iAngleZ, int iSkillType, int iLevel = 0 );
	void										PlayWeaponSound( UnitData * pcUnitData );
	void										StartSkillEffect( Point3D & point3D, int iSkillType, int iLevel = 0 );
	void										AddTimeEffect(  DWORD EffectCode , DWORD dwTime , int x, int y, int z );
	static void									StartSkillEffectOld( int iX, int iY, int iZ, int iID, int iLevel = 1 );
	void										SkillPlaySound( DWORD dwCode, int iX, int iY, int iZ, int iFlag = 0 );
	void										SkillPlaySound( DWORD dwCode, Point3D & point, int iFlag = 0 );
	void										SkillPlaySound( DWORD dwCode, UnitData * pcUnitData, int iFlag = 0 );
	int											RetryPlayAttack( UnitData * pcUnitData );
	int											PlaySkillLoop( ActiveSkill sActiveSkill, UnitData * pcUnitData );
	BOOL										CanUseSkill( struct Skill * psSkill );

	BOOL										BeginSkill( UnitData * pcThis, int iSkillID, int iLevel, class UnitData * pcUnitData, int iX, int iY, int iZ );
	BOOL										PlaySkillAttack( struct Skill * psSkill, UnitData * pcUnitData );
	int											OpenPlaySkill( struct Skill * psSkill );

	BOOL										HandleDebuffSkill( UnitData * pcUnitData, int iID, int iValue );

	int											GetSkillRange( struct Skill * psSkill, int iAttackRange = 0, int iAttackMode = 0 );

	BOOL										EventSkill( UnitData * pcUnitData );
	void										EventSkillPostASM( UnitData * pcUnitData );
	BOOL										BeginSkillMonster( UnitData * pcUnitData );
	BOOL										EventSkillMonster( UnitData * pcUnitData );
	BOOL										EventHitMonster( UnitData * pcUnitData );

	BOOL										EndSkill( class UnitData * pcUnitData );

	void										OnEffect();

	void										LoadParticles();

	int											FormatSkillTimerInfo( struct Skill * pSkill );
	BOOL										ResetForceOrb();

	void										ReformSkill( struct Skill * psSkill, int iSkill = 0 );
	void										ReformCharSkillInfo ( );
	void										SaveCheckSkill ( );
	void										ReformSkillInfo ( );
	void										ResetSkillCountdown ( enum ESkillID eSkillID );


	BOOL										OnMouseClick( class CMouse * pcMouse );

	BOOL										OnMouseClickSkillPet();
	BOOL										IsAgressive( int iSkillID );
	BOOL										SetDefaultPetMode ( int iSkillID, BOOL bAgressive );

	void										ResetSkillEffectParticle( DWORD dwSkillID );

	void										ResetBuff( DWORD dwSkillID, UnitData * pcUnitData );

	void										AssaParticle_KeepSkillEffect( char * FileName, UnitData * pChar, int iDurationSec, ESkillID eSkillID );
	void										AssaParticle_AttackSkillEffect( char * FileName, UnitData * pChar, DWORD dwSkillCode = 0);

	void										StartSkillEffect( Point3D sPosition, Point3D sTargetPosition, int iSkillID, int iLevel = 0 );

	static bool									GetWeaponPosition( UnitData * pcUnitData, Point3D * psPositionOut, float fU = 0.0f );

	static UnitData *							FindNearestMonster( UnitData * lpCurChar);
	static UnitData *							FindNearestMonster( UnitData * lpCurChar, int iRangeThreshold);
	static UnitData *							FindNearestPlayer( UnitData * lpCurChar, int iRangeThreshold);

	BOOL										GetWeaponPositionNew( UnitData * pcUnitData, Point3D & sPositionOut, float fScaleSize = 0.0f );

	void										HandlePacket( PacketSkillPartyData * psPacket );

	void										CancelSkillOnServer( int iID, int iP1 = 0, int iP2 = 0, int iP3 = 0 );

	void										ProcessSkillInParty( int iID, int iLevel, int iArea, int iP1 = 0, int iP2 = 0, int iP3 = 0, int iP4 = 0 );

	BOOL										dm_OpenTransRangeDamage ();

	void										SetLastAttackRange( int iValue ) { iLastAttackRange = iValue; }
	int											GetLastAttackRange( ) { return iLastAttackRange; }

	int											SendProcessSkillToServer( DWORD dwSkillCode, int point, int param1 = 0, int param2 = 0 );
	int											SendTransDamage( UnitData * pcUnitData, const ActiveSkill & sActiveSkill, BOOL bUseAttackRating = TRUE );
	int											SendTransDamage( UnitData * pcUnitData, int iPP1 = 0, int iPP2 = 0, int eAttackState = 0, int iResistance = 0, int iID = 0, BOOL bAttackRatingUpdate = FALSE );
	int											SendTransDamageSock( UnitData * pcUnitData, int iPP1, int iPP2, int eAttackState, int iResistance, int iID, BOOL bAttackRatingUpdate, SocketData * pcSocketData );
	void										SendRangeDamage( UnitData * pcUnitData, Point3D & sPosition, int iPP1 = 0, int iPP2 = 0, int eAttackState = 0, int iResistance = 0, int iID = 0 );
	void										SendRangeDamage( UnitData * pcUnitData, int iAttackSkillID );
	void										SelectRangeBox( UnitData * pcUnitData, RECT & rRect, BOOL bUseAttackRating = FALSE );
	int											SelectRangeChained ( UnitData * lpCurChar, UnitData * lpTragetChar, int SelCount, int Size, int MaxRange = 0);
	void										GetMoveLocation ( int x, int y, int z, short AngX, short AngY, short AngZ );
	void										SelectDamageCount( UnitData * lpCurChar, int SelCount, int Size );
	void										SetUnitsRange( Point3D & sPosition, int iRange, BOOL bUseAttackRating = FALSE );
	void										SetDamageCount( Point3D & sPosition, int iRange, BOOL bUseAttackRating = FALSE );
	static void									SetUnitsRangeCount( Point3D & sPosition, int iRange, BOOL bUseAttackRating = FALSE, int iTargetCount = 2 );

	int											GetFrameSpeed( int iSpeed );
	int											GetAttackSpeedMainFrame ( int iAttackSpeed );
	int											GetAttackFrameSpeed( int iAttackSpeed, int iAddSpeed = 1 );
	int											GetAttackFrameSpeed_Half ( int iAttackSpeed );

	void										CheckSkillValidate( Skill * psSkill );
	void										ValidateSkill( Skill * psSkill );

	void										AddSkillElement( EElementID eElement );
	void										SkillEffectLearn( UnitData * pcUnitData = NULL );

	//T5 Skills
	BOOL										OnBuySkill();

	void										LoadServerSkillChainData( SkillChainScaleData * data );
	void										LoadServerCommonSkillInfo ( SkillInfoCommon * data );
	void										LoadServerSkillInfo ( SkillInfo * dataEx );
	void										LoadServerSkillData ( SkillArrayData * data );

	void										ApplySkillBuffToSelf ();
	void										ApplyPassiveSkillToSelf ();
	void										SetWeaponDamageBuffFromSkill ( int * iSkillDamage, int * iWeaponDamage );


	void										AddSkillBoxEmptyLine ( );
	void										AddSkillBoxGroupTitle ( const char * pszTitle, D3DCOLOR dwColor, EElementID eElementID = EElementID::None );
	void										AddSkillBoxInfo ( const char * pszLabel, const char * pszString );
	void										AddSkillBoxInfo ( const char * pszLabel, const char * pszString, DWORD dwColor );
	void										AddSkillBoxInfo ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased, BOOL bShowNextLevel = TRUE, float fScaling = 1.0f );
	void										AddSkillBoxInfoDuration ( int iSkillArrayPointerID, int iSkillLevelZeroBased );
	void										AddSkillBoxInfoRange ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased, BOOL bShowNextLevel = TRUE, float fScaling = 1.0f );
	void										AddSkillBoxInfoDual ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerIDFirst, int iSkillArrayPointerIDSecond, int iSkillLevelZeroBased, BOOL bShowNextLevel = TRUE, float fScaling1 = 1.0f, float fScaling2 = 1.0f);

	void										AddChainSkillBoxInfo ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased );
	void										AddChainSkillBoxInfoRange ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased);
	void										AddChainSkillBoxInfoDual ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerIDFirst, int iSkillArrayPointerIDSecond, int iSkillLevelZeroBased);

	int	*										GetNewSkillIntArrayPointer ( int iSkillArrayPointerID );
	float *										GetNewSkillFloatArrayPointer ( int iSkillArrayPointerID );
	int											GetSkillIntValue ( enum ESkillArrayPointer eSkillArrayPointerID, enum ESkillID eSkillID, BOOL bChained = FALSE );
	int											GetSkillIntValue ( int iSkillArrayPointerID, int iSkillLevel0Based, BOOL bChained = FALSE );
	float										GetSkillFloatValue ( int iSkillArrayPointerID, enum ESkillID eSkillID, BOOL bChained = FALSE );
	float										GetSkillFloatValue ( int iSkillArrayPointerID, int iSkillLevel0Based, BOOL bChained = FALSE );


	BOOL										AddDamageVsMonsterTypeInfo ( Skill * pcSkill );
	BOOL										AddSpecificMonsterTypeInfo ( Skill * pcSkill );
	BOOL										AddSummonInfo ( Skill * pcSkill );
	BOOL										AddPassiveInfo ( Skill * pcSkill );
	BOOL										AddDebuffInfo ( Skill * pcSkill );
	BOOL										AddBuffInfo ( Skill * pcSkill );
	BOOL										AddHintInfo ( Skill * pcSkill );

	BOOL										AddSkillInfoToBox_FS ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_AS ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_MS ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_PS ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_ASS ( int iSkillID, int iSkillLevelZeroBased );

	BOOL										AddSkillInfoToBox_ATA ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_KS ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_PRS ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_MGS ( int iSkillID, int iSkillLevelZeroBased );
	BOOL										AddSkillInfoToBox_SHA ( int iSkillID, int iSkillLevelZeroBased );

	void										UpdateSkillCanBuy();

	void										OnClickSkill();

	void										OnEndSkillTimer( Skill * psSkill );

	BOOL										OnKeyPress ( class CKeyboard * pcKeyboard );
	void										GMAddSkillPoint ( int iPoint );

	BOOL										HandlePacket( PacketUseSkillBase * psPacket, Unit * pcTargetUnit );
	BOOL										HandlePacket( PacketUseSkillParty * psPacket );
	BOOL										HandlePacket( PacketUseSkillSingle * psPacket );
	void										HandlePacket( PacketSkillBuffStatus * psPacket );
	void										HandlePacket( PacketSkillCancel * psPacket );
	void										HandlePacket( PacketSkillCast * psPacket );



};

