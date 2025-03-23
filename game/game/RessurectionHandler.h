#pragma once


#define PRIESTRESS_RESSURECTION_RANGE (300)
#define PRIESTRESS_RESSURECTION_RANGE_SQ (PRIESTRESS_RESSURECTION_RANGE*PRIESTRESS_RESSURECTION_RANGE)

class CRessurectionHandler
{
public:

	BOOL								OnUseSkill( UnitData * pcUnitData );
	BOOL								OnSkillCasted( PacketSkillCast* psPacket );

	CRessurectionHandler();
	virtual ~CRessurectionHandler();
};

