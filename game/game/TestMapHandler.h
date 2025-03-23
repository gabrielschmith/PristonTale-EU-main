#pragma once

#include <set>

class CTestMapHandler
{
public:
	CTestMapHandler ();
	virtual ~CTestMapHandler ();

	void	Init ();
	void	Render ();
	void	Tick1s ();
	void	Reset ();

	void    OnHitReceived( EDamageTextType eDamageTextType, int iValue );
	void    AddTargetDamageDealt ( PacketDebugDamageInfo * packet );
	void    AddTargetDamageDealt ( ID idAttacker, ID idTarget, EDamageTextType eTypeAction, int iValue );

	void	OnGeneralSkillHit ( );
	void	OnMultiTargetSkillHit ( PacketMultiTargetSkillData * psOutgoingAOESkillPacket, ActiveSkill * pcActiveSkill );
	void	OnSingleTargetSkillHit ( PacketSingleTargetSkillData * psOutgoingAttackPacket, ActiveSkill * pcActiveSkill );
	void	AddSkillAttackDelay ( enum ESkillID eSkillID, int iDelay );



private:


	DWORD dwStartDamageAllTick = 0;
	DWORD dwStartDamageReceivedTick = 0;
	DWORD dwLastDamageTick = 0;
	DWORD dwLastHitReceivedTick = 0;

	int iCurrentSkillId = 0;

	const int iFontHeight = 18;

	BOOL	bPaused	= TRUE;
	BOOL	bFirst	= TRUE;
	BOOL	bIgnoring	= TRUE;
	int		iFontID = -1;
	int		iSkillHitFrameMin = 0;

	DWORD	dwSkillFirstHitLastTick		= 0;
	int		iSkillFirstHitDelayMin		= 0;	//Delay in ms between skill animation's first hit. This measures the 'speed'
	int		iSkillFirstHitDelayLast		= 0;	//Delay in ms between skill animation's first hit. This measures the 'speed'

	int			eCurrentSkillTinyId			= 0;
	BOOL		bMultiSkillTarget		= FALSE;
	int			iCurrentDamageState		= 0;
	int			iCurrentCritical		= 0;
	int			iCurrentTarget			= 0;

	int			iCritCounter			= 0;
	int			iMissCounter			= 0;
	int			iBlockCounter			= 0;
	int			iDefenseCounter			= 0;
	int			iEvadeCounter			= 0;

	int			iPlayerCriticalChance	= 0;
	int			iPlayerAttackRating		= 0;
	int			iPlayerMinDamage		= 0;
	int			iPlayerMaxDamage		= 0;

	int			iAnimationType			= 0;
	int			iAnimationBeginFrame	= 0;
	int			iAnimationEndFrame		= 0;
	int			iAnimationFrameStep		= 0;
	EDamageTextType	iCurrentTypeAction	= EDamageTextType::Undefined; //6 = crit, 5 = normal
	int			iDPSAll					= 0;
	int			iDPSAll_NoCrit			= 0;
	int			iHitDealtAll			= 0;
	int			iHitsAttempted			= 0;
	int			iHitsAvoided			= 0;
	int			iHitsReceived			= 0;
	int			iTargetCount			= 0;
	int			iTotalDamageAll			= 0;
	float 		fDPSDurationTotalSec    = 0;
	float		fDamagedReceivedDuration = 0;
	int 		iTotalDamageReceived			= 0;
	float 		fDPSReceived			= 0;

	short       sActiveSkillTinyID = 0;

	DWORD		dwLastTickFirstSkillHit = FALSE;
	DWORD		dwIgnoreUntilTick = 0;		//to improve DPS consistency

	std::set<ID>	 mTargetIDs;

	std::vector<int> viDamageDealtAll_NoCrit;
	std::vector<int> viDamageDealtAll;
};
