#include "stdafx.h"
#include "character.h"
#include <strings.h>

EClassFlag CharacterClassToClassFlag( ECharacterClass eClass )
{
	switch( eClass )
	{
		//Tempskron
		case CHARACTERCLASS_Fighter:
			return CLASSFLAG_Fighter;
		case CHARACTERCLASS_Mechanician:
			return CLASSFLAG_Mechanician;
		case CHARACTERCLASS_Archer:
			return CLASSFLAG_Archer;
		case CHARACTERCLASS_Pikeman:
			return CLASSFLAG_Pikeman;
		case CHARACTERCLASS_Assassin:
			return CLASSFLAG_Assassin;

			//Morion
		case CHARACTERCLASS_Atalanta:
			return CLASSFLAG_Atalanta;
		case CHARACTERCLASS_Knight:
			return CLASSFLAG_Knight;
		case CHARACTERCLASS_Magician:
			return CLASSFLAG_Magician;
		case CHARACTERCLASS_Priestess:
			return CLASSFLAG_Priestess;
		case CHARACTERCLASS_Shaman:
			return CLASSFLAG_Shaman;
	}

	return CLASSFLAG_None;
}

ESkillID GetSkillIDFromTinyID ( ECharacterClass eClass, BYTE iTinySkillID )
{
	switch ( eClass )
	{
		case CHARACTERCLASS_Fighter:		return GetFighterSkillIDFromTinyID ( iTinySkillID );		break;
		case CHARACTERCLASS_Mechanician:	return GetMechanicanSkillIDFromTinyID ( iTinySkillID );		break;
		case CHARACTERCLASS_Archer:			return GetArcherSkillIDFromTinyID ( iTinySkillID );			break;
		case CHARACTERCLASS_Pikeman:		return GetPikemanSkillIDFromTinyID ( iTinySkillID );		break;
		case CHARACTERCLASS_Assassin:		return GetAssassinSkillIDFromTinyID ( iTinySkillID );		break;
		case CHARACTERCLASS_Atalanta:		return GetAtalantaSkillIDFromTinyID ( iTinySkillID );		break;
		case CHARACTERCLASS_Knight:			return GetKnightSkillIDFromTinyID ( iTinySkillID );			break;
		case CHARACTERCLASS_Magician:		return GetMagicianSkillIDFromTinyID ( iTinySkillID );		break;
		case CHARACTERCLASS_Priestess:		return GetPriestessSkillIDFromTinyID ( iTinySkillID );		break;
		case CHARACTERCLASS_Shaman:			return GetShamanSkillIDFromTinyID ( iTinySkillID );			break;
	}

	return ESkillID::SKILLID_DefaultAttack;
}

ESkillID GetArcherSkillIDFromTinyID (BYTE iTinySkillID)
{
	switch ( iTinySkillID )
	{
		case ArcherSkill::DefaultArcherAttack:		return ESkillID::SKILLID_DefaultAttack;
		case ArcherSkill::WindArrow: 				return ESkillID::SKILLID_WindArrow;
		case ArcherSkill::PerfectAim: 				return ESkillID::SKILLID_PerfectAim;
		case ArcherSkill::Falcon: 					return ESkillID::SKILLID_Falcon;
		case ArcherSkill::ArrowofRage: 				return ESkillID::SKILLID_ArrowofRage;
		case ArcherSkill::Avalanche: 				return ESkillID::SKILLID_Avalanche;
		case ArcherSkill::ElementalShot:			return ESkillID::SKILLID_ElementalShot;
		case ArcherSkill::GoldenFalcon: 			return ESkillID::SKILLID_GoldenFalcon;
		case ArcherSkill::BombShot: 				return ESkillID::SKILLID_BombShot;
		case ArcherSkill::Perforation: 				return ESkillID::SKILLID_Perforation;
		case ArcherSkill::RecallWolverine:			return ESkillID::SKILLID_RecallWolverine;
		case ArcherSkill::PhoenixShot: 				return ESkillID::SKILLID_PhoenixShot;
		case ArcherSkill::ForceofNature:			return ESkillID::SKILLID_ForceofNature;
		case ArcherSkill::FierceWind: 				return ESkillID::SKILLID_FierceWind;
	}

	return ESkillID::SKILLID_DefaultAttack;
}

/// <summary>
/// Get full skill id from tiny skill id (received from client);
/// </summary>
ESkillID GetAssassinSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case AssassinSkill::DefaultAssassinAttack: 	return ESkillID::SKILLID_DefaultAttack;
		case AssassinSkill::Stinger: 				return ESkillID::SKILLID_Stinger;
		case AssassinSkill::RunningHit: 			return ESkillID::SKILLID_RunningHit;
		case AssassinSkill::Wisp: 					return ESkillID::SKILLID_Wisp;
		case AssassinSkill::VenomThorn: 			return ESkillID::SKILLID_VenomThorn;
		case AssassinSkill::Alas: 					return ESkillID::SKILLID_Alas;
		case AssassinSkill::SoulShock: 				return ESkillID::SKILLID_SoulShock;
		case AssassinSkill::AttackMastery: 			return ESkillID::SKILLID_DodgeMastery;
		case AssassinSkill::SoreBlade: 				return ESkillID::SKILLID_SoreBlade;
		case AssassinSkill::GustSlash: 				return ESkillID::SKILLID_GustSlash;
		case AssassinSkill::Inpes: 					return ESkillID::SKILLID_Inpes;
		case AssassinSkill::Blind: 					return ESkillID::SKILLID_Blind;
		case AssassinSkill::Cripple: 				return ESkillID::SKILLID_Cripple;
		case AssassinSkill::Polluted: 				return ESkillID::SKILLID_Polluted;
		case AssassinSkill::NinjaShadow: 			return ESkillID::SKILLID_NinjaShadow;
		case AssassinSkill::ShadowBlink: 			return ESkillID::SKILLID_ShadowBlink;
		case AssassinSkill::RisingSlash: 			return ESkillID::SKILLID_RisingSlash;
		case AssassinSkill::ViolentStub: 			return ESkillID::SKILLID_ViolentStub;
		case AssassinSkill::ShadowStorm: 			return ESkillID::SKILLID_ShadowStorm;
	}

	return ESkillID::SKILLID_DefaultAttack;
}

/// <summary>
/// Get full skill id from tiny skill id (received from client);
/// </summary>
ESkillID GetAtalantaSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case AtalantaSkill::AtalantaDefaultAttack:	return ESkillID::SKILLID_DefaultAttack;
		case AtalantaSkill::ShieldStrike:			return ESkillID::SKILLID_ShieldStrike;
		case AtalantaSkill::Farina:					return ESkillID::SKILLID_Farina;
		case AtalantaSkill::BiggerSpear:			return ESkillID::SKILLID_BiggerSpear;
		case AtalantaSkill::Windy:					return ESkillID::SKILLID_Windy;
		case AtalantaSkill::WindJavelin:			return ESkillID::SKILLID_WindJavelin;
		case AtalantaSkill::SoulSucker:				return ESkillID::SKILLID_SoulSucker;
		case AtalantaSkill::FireJavelin:			return ESkillID::SKILLID_FireJavelin;
		case AtalantaSkill::SplitJavelin:			return ESkillID::SKILLID_SplitJavelin;
		case AtalantaSkill::TriumphofValhalla:		return ESkillID::SKILLID_TriumphofValhalla;
		case AtalantaSkill::LightningJavelin:		return ESkillID::SKILLID_LightningJavelin;
		case AtalantaSkill::StormJavelin:			return ESkillID::SKILLID_StormJavelin;
		case AtalantaSkill::HallofValhalla:			return ESkillID::SKILLID_HallofValhalla;
		case AtalantaSkill::ExtremeRage:			return ESkillID::SKILLID_ExtremeRage;
		case AtalantaSkill::FrostJavelin:			return ESkillID::SKILLID_FrostJavelin;
		case AtalantaSkill::Vengeance:				return ESkillID::SKILLID_Vengeance;
	}

	return ESkillID::SKILLID_DefaultAttack;
}

/// <summary>
/// Get full skill id from tiny skill id (received from client);
/// </summary>
ESkillID GetFighterSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case FighterSkill::DefaultFighterAttack:	return ESkillID::SKILLID_DefaultAttack;
		case FighterSkill::Raving:					return ESkillID::SKILLID_Raving;
		case FighterSkill::DoubleImpact:			return ESkillID::SKILLID_DoubleImpact;
		case FighterSkill::TripleImpact:			return ESkillID::SKILLID_TripleImpact;
		case FighterSkill::BrutalSwing:				return ESkillID::SKILLID_BrutalSwing;
		case FighterSkill::WarCry:					return ESkillID::SKILLID_WarCry;
		case FighterSkill::RageOfZecram:			return ESkillID::SKILLID_RageofZecram;
		case FighterSkill::AvengingCrash:			return ESkillID::SKILLID_AvengingCrash;
		case FighterSkill::BoneCrash:				return ESkillID::SKILLID_BoneCrash;
		case FighterSkill::Destroyer:				return ESkillID::SKILLID_Destroyer;
		case FighterSkill::Berserker:				return ESkillID::SKILLID_Berserker;
		case FighterSkill::CycloneStrike:			return ESkillID::SKILLID_CycloneStrike;
	}

	return ESkillID::SKILLID_DefaultAttack;
}


ESkillID GetKnightSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case KnightSkill::DefaultKnightAttack: 	return ESkillID::SKILLID_DefaultAttack;
		case KnightSkill::SwordBlast: 			return ESkillID::SKILLID_SwordBlast;
		case KnightSkill::HolyBody: 			return ESkillID::SKILLID_HolyBody;
		case KnightSkill::DoubleCrash: 			return ESkillID::SKILLID_DoubleCrash;
		case KnightSkill::HolyValor: 			return ESkillID::SKILLID_HolyValor;
		case KnightSkill::Brandish: 			return ESkillID::SKILLID_Brandish;
		case KnightSkill::Piercing: 			return ESkillID::SKILLID_Piercing;
		case KnightSkill::DrasticSpirit: 		return ESkillID::SKILLID_DrasticSpirit;
		case KnightSkill::DivineShield: 		return ESkillID::SKILLID_DivineShield;
		case KnightSkill::HolyIncantation:		return ESkillID::SKILLID_HolyIncantation;
		case KnightSkill::GrandCross: 			return ESkillID::SKILLID_GrandCross;
		case KnightSkill::SwordofJustice: 		return ESkillID::SKILLID_SwordofJustice;
		case KnightSkill::GodlyShield: 			return ESkillID::SKILLID_GodlyShield;
		case KnightSkill::GodsBlessing: 		return ESkillID::SKILLID_GodsBlessing;
		case KnightSkill::DivinePiercing: 		return ESkillID::SKILLID_DivinePiercing;
		case KnightSkill::HolyConviction: 		return ESkillID::SKILLID_HolyConviction;
	}

	return ESkillID::SKILLID_DefaultAttack;
}


ESkillID GetMagicianSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case MagicianSkill::DefaultMagicianAttack:			return ESkillID::SKILLID_DefaultAttack		;
		case MagicianSkill::FireBolt			:			return ESkillID::SKILLID_FireBolt			;
		case MagicianSkill::FireBall			:			return ESkillID::SKILLID_FireBall			;
		case MagicianSkill::Watornado			:			return ESkillID::SKILLID_Watornado			;
		case MagicianSkill::EnchantWeapon		:			return ESkillID::SKILLID_EnchantWeapon		;
		case MagicianSkill::DeathRay			:			return ESkillID::SKILLID_DeathRay			;
		case MagicianSkill::EnergyShield		:			return ESkillID::SKILLID_EnergyShield		;
		case MagicianSkill::Diastrophism		:			return ESkillID::SKILLID_Diastrophism		;
		case MagicianSkill::SpiritElemental	:				return ESkillID::SKILLID_SpiritElemental	;
		case MagicianSkill::DancingSword		:			return ESkillID::SKILLID_DancingSword		;
		case MagicianSkill::FireElemental		:			return ESkillID::SKILLID_FireElemental		;
		case MagicianSkill::FlameWave			:			return ESkillID::SKILLID_FlameWave			;
		case MagicianSkill::Distortion			:			return ESkillID::SKILLID_Distortion			;
		case MagicianSkill::Meteorite			:			return ESkillID::SKILLID_Meteorite			;
		case MagicianSkill::WizardTrance		:			return ESkillID::SKILLID_WizardTrance		;
	}

	return ESkillID::SKILLID_DefaultAttack;
}

ESkillID GetMechanicanSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case MechanicanSkill::DefaultMechAttack: 	 return ESkillID::SKILLID_DefaultAttack;
		case MechanicanSkill::ExtremeShield: 		 return ESkillID::SKILLID_ExtremeShield;
		case MechanicanSkill::MechanicBomb: 		 return ESkillID::SKILLID_MechanicBomb;
		case MechanicanSkill::ElectricalOverload:	 return ESkillID::SKILLID_ElectricalOverload;
		case MechanicanSkill::PhysicalAbsorption: 	 return ESkillID::SKILLID_PhysicalAbsorb;
		case MechanicanSkill::GreatSmash: 			 return ESkillID::SKILLID_GreatSmash;
		case MechanicanSkill::Maximize: 			 return ESkillID::SKILLID_Maximize;
		case MechanicanSkill::Automation: 			 return ESkillID::SKILLID_Automation;
		case MechanicanSkill::Spark: 				 return ESkillID::SKILLID_Spark;
		case MechanicanSkill::MetalArmor: 			 return ESkillID::SKILLID_MetalArmor;
		case MechanicanSkill::GrandSmash: 			 return ESkillID::SKILLID_GrandSmash;
		case MechanicanSkill::SparkShield: 			 return ESkillID::SKILLID_SparkShield;
		case MechanicanSkill::Impulsion: 			 return ESkillID::SKILLID_Impulsion;
		case MechanicanSkill::Compulsion: 			 return ESkillID::SKILLID_Compulsion;
		case MechanicanSkill::MagneticSphere: 		 return ESkillID::SKILLID_MagneticSphere;
		case MechanicanSkill::MetalGolem: 			 return ESkillID::SKILLID_MetalGolem;
		case MechanicanSkill::Rupture: 				 return ESkillID::SKILLID_Rupture;
	}

	return ESkillID::SKILLID_DefaultAttack;
}


ESkillID GetPikemanSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case PikemanSkill::DefaultPikemanAttack: 	return ESkillID::SKILLID_DefaultAttack;
		case PikemanSkill::PikeWind: 				return ESkillID::SKILLID_PikeWind;
		case PikemanSkill::PreciseHit: 				return ESkillID::SKILLID_PreciseHit;
		case PikemanSkill::JumpingCrash: 			return ESkillID::SKILLID_JumpingCrash;
		case PikemanSkill::GroundPike: 				return ESkillID::SKILLID_GroundPike;
		case PikemanSkill::Tornado: 				return ESkillID::SKILLID_Tornado;
		case PikemanSkill::Expansion: 				return ESkillID::SKILLID_Expansion;
		case PikemanSkill::VenomSpear: 				return ESkillID::SKILLID_VenomSpear;
		case PikemanSkill::Vanish: 					return ESkillID::SKILLID_Vanish;
		case PikemanSkill::ChainLance: 				return ESkillID::SKILLID_ChainLance;
		case PikemanSkill::AssassinsEye: 			return ESkillID::SKILLID_AssassinsEye;
		case PikemanSkill::ChargingStrike: 			return ESkillID::SKILLID_ChargingStrike;
		case PikemanSkill::Vague: 					return ESkillID::SKILLID_Vague;
		case PikemanSkill::ShadowMaster: 			return ESkillID::SKILLID_ShadowMaster;
		case PikemanSkill::RingOfSpears: 			return ESkillID::SKILLID_RingofSpears;
	}

	return ESkillID::SKILLID_DefaultAttack;
}

ESkillID GetPriestessSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case PriestessSkill::PriestessDefaultAttack: 	return ESkillID::SKILLID_DefaultAttack;
		case PriestessSkill::Healing: 					return ESkillID::SKILLID_Healing;
		case PriestessSkill::HolyBolt: 					return ESkillID::SKILLID_HolyBolt;
		case PriestessSkill::MultiSpark: 				return ESkillID::SKILLID_MultiSpark;
		case PriestessSkill::HolyMind: 					return ESkillID::SKILLID_HolyMind;
		case PriestessSkill::DivineLightning: 			return ESkillID::SKILLID_DivineLightning;
		case PriestessSkill::HolyReflection: 			return ESkillID::SKILLID_HolyReflection;
		case PriestessSkill::GrandHealing: 				return ESkillID::SKILLID_GrandHealing;
		case PriestessSkill::VigorBall: 				return ESkillID::SKILLID_VigorBall;
		case PriestessSkill::Resurrection: 				return ESkillID::SKILLID_Ressurection;
		case PriestessSkill::Extinction: 				return ESkillID::SKILLID_Extinction;
		case PriestessSkill::VirtualLife: 				return ESkillID::SKILLID_VirtualLife;
		case PriestessSkill::GlacialSpike: 				return ESkillID::SKILLID_GlacialSpike;
		//case PriestessSkill::RegenerationField: 		return ESkillID::SKILLID_RegenerationField; //obsolete (good riddance)
		case PriestessSkill::VirtualRegen: 				return ESkillID::SKILLID_VirtualRegen;
		case PriestessSkill::ChainLightning: 			return ESkillID::SKILLID_ChainLightning;
		case PriestessSkill::SummonMuspell: 			return ESkillID::SKILLID_SummonMuspell;
		case PriestessSkill::IceElemental: 				return ESkillID::SKILLID_IceElemental;
	}

	return ESkillID::SKILLID_DefaultAttack;
}

/// <summary>
/// Get full skill id from tiny skill id (received from client);
/// </summary>
ESkillID GetShamanSkillIDFromTinyID ( BYTE iTinySkillID )
{
	switch ( iTinySkillID )
	{
		case ShamanSkill::ShamanDefaultAttack:  return ESkillID::SKILLID_DefaultAttack;
		case ShamanSkill::DarkBolt: 			return ESkillID::SKILLID_DarkBolt;
		case ShamanSkill::DarkWave: 			return ESkillID::SKILLID_DarkWave;
		case ShamanSkill::Inertia: 				return ESkillID::SKILLID_Inertia;
		case ShamanSkill::InnerPeace: 			return ESkillID::SKILLID_InnerPeace;
		case ShamanSkill::SpiritualFlare: 		return ESkillID::SKILLID_SpiritualFlare;
		case ShamanSkill::SpiritualManacle: 	return ESkillID::SKILLID_SpiritualManacle;
		case ShamanSkill::ChasingHunt: 			return ESkillID::SKILLID_ChasingHunt;
		case ShamanSkill::AdventMigal: 			return ESkillID::SKILLID_AdventMigal;
		case ShamanSkill::Rainmaker: 			return ESkillID::SKILLID_Rainmaker;
		case ShamanSkill::PhantomCall: 			return ESkillID::SKILLID_PhantomCall;
		case ShamanSkill::Haunt: 				return ESkillID::SKILLID_Haunt;
		case ShamanSkill::Scratch: 				return ESkillID::SKILLID_Scratch;
		case ShamanSkill::RecallHestian: 		return ESkillID::SKILLID_RecallHestian;
		case ShamanSkill::Judgement: 			return ESkillID::SKILLID_Judgement;
		case ShamanSkill::AdventMidranda: 		return ESkillID::SKILLID_AdventMidranda;
		case ShamanSkill::MourningPray: 		return ESkillID::SKILLID_MourningPray;
	}

	return ESkillID::SKILLID_DefaultAttack;
}



ECharacterClass ClassFlagToCharacterClass( EClassFlag eFlag )
{
	ECharacterClass iClass = CHARACTERCLASS_None;

	switch( eFlag )
	{
		case CLASSFLAG_Fighter:
			iClass = CHARACTERCLASS_Fighter;
			break;
		case CLASSFLAG_Mechanician:
			iClass = CHARACTERCLASS_Mechanician;
			break;
		case CLASSFLAG_Archer:
			iClass = CHARACTERCLASS_Archer;
			break;
		case CLASSFLAG_Pikeman:
			iClass = CHARACTERCLASS_Pikeman;
			break;
		case CLASSFLAG_Atalanta:
			iClass = CHARACTERCLASS_Atalanta;
			break;
		case CLASSFLAG_Knight:
			iClass = CHARACTERCLASS_Knight;
			break;
		case CLASSFLAG_Magician:
			iClass = CHARACTERCLASS_Magician;
			break;
		case CLASSFLAG_Priestess:
			iClass = CHARACTERCLASS_Priestess;
			break;
		case CLASSFLAG_Assassin:
			iClass = CHARACTERCLASS_Assassin;
			break;
		case CLASSFLAG_Shaman:
			iClass = CHARACTERCLASS_Shaman;
			break;
	}

	return iClass;
}

/**
Return ECharacterClass Enum from a string
Acceptable string values are commonly recognized short id
fs | ms | as | ps | ata | ks | mgs| prs | ass | shm
*/
ECharacterClass ClassShortStrToCharacterClass(const char * str)
{

	if (STRINGCOMPAREI( "fs", str))
		return CHARACTERCLASS_Fighter;

	if (STRINGCOMPAREI("ms", str))
		return CHARACTERCLASS_Mechanician;

	if (STRINGCOMPAREI("as", str))
		return CHARACTERCLASS_Archer;

	if (STRINGCOMPAREI("ps", str))
		return CHARACTERCLASS_Pikeman;

	if (STRINGCOMPAREI("ata", str))
		return CHARACTERCLASS_Atalanta;

	if (STRINGCOMPAREI("ks", str))
		return CHARACTERCLASS_Knight;

	if (STRINGCOMPAREI("mgs", str))
		return CHARACTERCLASS_Magician;

	if (STRINGCOMPAREI("prs", str))
		return CHARACTERCLASS_Priestess;

	if (STRINGCOMPAREI("ass", str) || STRINGCOMPAREI("asn", str))
		return CHARACTERCLASS_Assassin;

	if (STRINGCOMPAREI("sha", str) || STRINGCOMPAREI("shm", str))
		return CHARACTERCLASS_Shaman;

	return CHARACTERCLASS_None;
}



char * GetCharacterClassStringByFlag( EClassFlag eFlag )
{
	switch( eFlag )
	{
		case CLASSFLAG_Fighter:			return "Fighter";
		case CLASSFLAG_Archer:			return "Archer";
		case CLASSFLAG_Assassin:		return "Assassin";
		case CLASSFLAG_Atalanta:		return "Atalanta";
		case CLASSFLAG_Knight:			return "Knight";
		case CLASSFLAG_Magician:		return "Magician";
		case CLASSFLAG_Mechanician:		return "Mechanician";
		case CLASSFLAG_Pikeman:			return "Pikeman";
		case CLASSFLAG_Priestess:		return "Priestess";
		case CLASSFLAG_Shaman:			return "Shaman";
	}

	return "";
};

char* GetCharacterClassStringByClassEnum(enum ECharacterClass eClass)
{
	switch (eClass)
	{
		case CHARACTERCLASS_Fighter:		return "Fighter";
		case CHARACTERCLASS_Archer:			return "Archer";
		case CHARACTERCLASS_Assassin:		return "Assassin";
		case CHARACTERCLASS_Atalanta:		return "Atalanta";
		case CHARACTERCLASS_Knight:			return "Knight";
		case CHARACTERCLASS_Magician:		return "Magician";
		case CHARACTERCLASS_Mechanician:	return "Mechanician";
		case CHARACTERCLASS_Pikeman:		return "Pikeman";
		case CHARACTERCLASS_Priestess:		return "Priestess";
		case CHARACTERCLASS_Shaman:			return "Shaman";
	}

	return "";
};

ECharacterRace GetCharacterRace( ECharacterClass iClass )
{
	ECharacterRace iRet = CHARACTERRACE_Morion;
	switch( iClass )
	{
		case CHARACTERCLASS_Archer:
		case CHARACTERCLASS_Fighter:
		case CHARACTERCLASS_Mechanician:
		case CHARACTERCLASS_Pikeman:
		case CHARACTERCLASS_Assassin:
			iRet = CHARACTERRACE_Tempskron;
			break;
	}
	return iRet;
}

const std::string GetCharacterTier3QuestItem( ECharacterClass iClass )
{
	switch (iClass)
	{
		case CHARACTERCLASS_Fighter:		return "WA108";
		case CHARACTERCLASS_Archer:			return "WS110";
		case CHARACTERCLASS_Assassin:		return "WD109";
		case CHARACTERCLASS_Atalanta:		return "WT109";
		case CHARACTERCLASS_Knight:			return "WS210";
		case CHARACTERCLASS_Magician:		return "WM109";
		case CHARACTERCLASS_Mechanician:	return "WC108";
		case CHARACTERCLASS_Pikeman:		return "WP109";
		case CHARACTERCLASS_Priestess:		return "WM109";
		case CHARACTERCLASS_Shaman:			return "WN109";
	}

	return "";
}

ItemRequeriments * GetItemRequirementAdjustmentForClass( ECharacterClass iClass )
{
	return &saItemRequeriments[(int)iClass];
}

CharacterClassData * GetCharacterClassData( int iClass, int iRankup )
{
	int iClassCheck = iRankup & 7;
	int iClassBase = iClass & 0xFFFF;

	if( !iRankup )
		iClassCheck = iClass >> 24;

	int iRankupCount = 0;
	for( size_t i = 0; i < _countof(saCharacterClassData); i++ )
	{
		CharacterClassData * psClassData = saCharacterClassData + i;

		if( !psClassData->pszName[0] )
			continue;

		if( iClassBase == psClassData->iClass )
		{
			if( iClassCheck == iRankupCount )
				return psClassData;

			iRankupCount++;
		}
	}

	return NULL;
}
