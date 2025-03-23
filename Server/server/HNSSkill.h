#pragma once
#ifndef __SKILLS_PT
#define __SKILLS_PT

#include "stdafx.h"
#include "SQLSkill.h"
#include "packetserver.h"
#include "Fighter.h"
#include "Archer.h"
#include "Priestess.h"
#include "Mechanician.h"
#include "Magician.h"
#include "Pikeman.h"
#include "Knight.h"
#include "Atalanta.h"
#include "Assassin.h"
#include "Shaman.h"


class UxxxTarget
{
    public:
        UserData * userData;
        UnitData * unitData;

    public:
        UxxxTarget() = delete;
        UxxxTarget(UxxxTarget&&) = delete;
        UxxxTarget(UserData * p_UserData, UnitData * p_UnitData)
            : userData(p_UserData), unitData(p_UnitData)
        {}
        UxxxTarget(UserData * p_UserData)
            : userData(p_UserData), unitData(nullptr)
        {}
        UxxxTarget(UnitData * p_UnitData)
            : userData(nullptr), unitData(p_UnitData)
        {}
        UxxxTarget(UxxxTarget & p_Copy)
            : userData(p_Copy.userData), unitData(p_Copy.unitData)
        {}

        /// Generic
        inline bool IsValid() { return IsTargetUnit() || IsTargetPlayer(); }
        inline DWORD GetID() { if (IsTargetUnit()) return unitData->iID; else if (IsTargetPlayer()) return userData->iID; else return -1; }
        inline char * GetName()
        {
            if (IsTargetUnit())
                return GetCharacterData()->szName;
            else if (IsTargetPlayer())
            {
                if (!GAME_SERVER)
                    return userData->szCharacterName;

                return GetCharacterData()->szName;
            }

            return "";
        }
        inline CharacterData * GetCharacterData() { if (IsTargetUnit()) return &unitData->sCharacterData; else if (IsTargetPlayer()) return &userData->sCharacterData; else return nullptr; }
        inline ECharacterType GetCharacterType() { return GetCharacterData() != nullptr ? GetCharacterData()->iType : ECharacterType::CHARACTERTYPE_None; }
        inline Point3D & GetPosition() { if (IsTargetUnit()) return unitData->sPosition; else if (IsTargetPlayer()) return userData->sPosition; else return Point3D(); }

        /// Monster only
        inline bool IsTargetUnit() { return unitData != nullptr; }
        inline bool IsUnitActive() { if (IsTargetUnit()) return unitData->bActive; return false; }
        inline bool IsUnitPetOf(UserData * p_Player)
        {
            if (!IsTargetUnit())
                return false;

            return GetMonsterType() == MONSTERTYPE_Summon
                && unitData->iPetID
                && unitData->pcOwner == p_Player;
        }
        inline EMonsterType GetMonsterType() { if (IsTargetUnit()) return unitData->sCharacterData.iMonsterType; return (EMonsterType)-1; }
        inline EMonsterClass GetMonsterClass() { if (IsTargetUnit()) return unitData->sCharacterData.sMonsterClass; return (EMonsterClass)-1; }
        inline EMonsterEffectID GetMonsterEffectID() { if (IsTargetUnit()) return unitData->sCharacterData.iMonsterEffectID; return (EMonsterEffectID)-1; }
        inline EAnimationType GetUnitAnimation() { if (IsTargetUnit()) return unitData->GetAnimation(); return ANIMATIONTYPE_None; }


        inline int GetCurrentHealth() { if ( IsTargetUnit() ) return unitData->GetCurrentHealth(); return userData->GetCurrentHealth(); }

        inline int GetMaxHealth() { if ( IsTargetUnit() ) return unitData->GetMaxHealth(); return userData->GetMaxHealth(); }

        /// Player only
        inline bool IsTargetPlayer() { return userData != nullptr; }
};



extern void		HNSSkill_Tick1s ();
extern void		CheckAndAutoSyncNewSkillsDataforGMsOnly ( User * pcGMUser );
extern void		CheckAndAutoSyncNewSkillsInfoforGMsOnly ( User * pcGMUser );

extern void     SendSelfBuffSkill( User * pcCaster, ESkillID eSKillId, int iLevel1Based, int iDurationSec, int iParameter = 0 );
extern void     SendPartyBuffSkill( User * pcCaster, ESkillID eSKillId, int iLevel1Based, int iDurationSec, int iParameter = 0, BOOL bIncludeRaidMembers = FALSE );
extern void     SendPartyBuffSkill( User * pcCaster, ESkillID eSKillId, int iLevel1Based, int iDurationSec, int iParameter, std::vector<User *> vpBuffUsers );

extern void		AppendZeroIntValuesToServerSkillDataBuffer ( PacketSkillDataContainer * container, enum ESkillID eSkillID, DWORD dwAddressOrIndex );
extern void		AppendIntValuesToServerSkillDataBuffer ( PacketSkillDataContainer * container, enum ESkillID eSkillID, DWORD dwAddressOrIndex, std::string query, BOOL bServerOnly = FALSE, BOOL bIgnoreWarning = FALSE );
extern void		AppendFloatValuesToServerSkillDataBuffer ( PacketSkillDataContainer * container, enum ESkillID eSkillID, DWORD dwAddressOrIndex, std::string query, BOOL bServerOnly = FALSE, BOOL bIgnoreWarning = FALSE);
extern int		GetSkillIntValueAt ( User * pcUser, enum ESkillArrayPointer dwSkillArrayId, int iLevel1Based, BOOL bChainned = FALSE );
extern float	GetSkillFloatValueAt ( User * pcUser, enum ESkillArrayPointer dwSkillArrayId, int iLevel1Based, BOOL bChainned = FALSE );
extern int		GetUserSkillLevel1Based( User * pcUser, ESkillID eSKillId );
extern void		CacheSkillInfoFromContainer ( PacketSkillInfoContainer * psSkillInfoContainer );
extern void		CacheSkillArrayDataFromContainer ( PacketSkillDataContainer * psSkillDataContainer );
extern void		CacheSkillArrayDataFromContainerForGMOnly ( PacketSkillDataContainer * psSkillDataContainer );

extern void		BuildPacketSkillInfoContainer ( PacketSkillInfoContainer * container, enum ECharacterClass eCharClass );
extern void		SendServerCommonSkillInfoToClient ( User * pcUser );
extern void		SendServerCommonSkillDataToClient ( User * pcUser );
extern void		UpdateAndCacheSkillInfoAndData ();
extern void		SendSkillChainScaleDataToClient ( User * pcUser );


extern void		CheckAndReSyncSkillsInfoForEveryone ();
extern void		CheckAndReSyncSkillsDataforEveryone ();
extern void		ClearAllSyncFlags ();
extern float	GetChainedSkillScale( enum ESkillArrayPointer arrayPointer );

extern const SkillInfo * GetSkillInfo( ESkillID p_SkillID );




void			AddToSkillInfoContainer ( PacketSkillInfoContainer * spContainer, enum ECharacterClass eCharClass );

void			SyncFilteredSkillsDataforUser ( User * pcGMUser, BOOL bCacheDataLocally = FALSE );
void			SyncFilteredSkillsInfoforUser ( User * pcGMUser );


void            UpdateServerSkillInfoData();

void			UpdateAndCacheServerCommonSkillDataForAllClasses ();
void			CompareServerAttackPowerWithClientVersion( User * pcUser, int iAttackPowerMin, int iAttackPowerMax );

//From DLL.cpp
extern BOOL __cdecl OnEvade ( UserData * pcUserData, DWORD dwObjectID );
extern int __cdecl UseDistortionMonster ( UserData * pcUserData, PacketSkillDamageRange * psPacket, int iLevelSkill );

extern CFighter									* pcFighter;
extern CMechanician								* pcMechanician;
extern CArcher									* pcArcher;
extern CPikeman									* pcPikeman;
extern CAtalanta								* pcAtalanta;
extern CKnight									* pcKnight;
extern CMagician								* pcMagician;
extern CPriestess								* pcPriestess;
extern CAssassin								* pcAssassin;
extern CShaman									* pcShaman;

#define FIGHTERSKILL							pcFighter
#define MECHANICIANSKILL						pcMechanician
#define ARCHERSKILL								pcArcher
#define PIKEMANSKILL							pcPikeman
#define ATALANTASKILL							pcAtalanta
#define KNIGHTSKILL								pcKnight
#define MAGICIANSKILL							pcMagician
#define PRIESTESSSKILL							pcPriestess
#define ASSASSINSKILL							pcAssassin
#define SHAMANSKILL								pcShaman





#endif