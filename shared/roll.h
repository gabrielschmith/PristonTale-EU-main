#pragma once

#define MAX_ROLLDICEDROP_TIME		15
#define ROLLDICE_DAMAGE_PERCENT		3

#define MAX_ROLLDICERANKING_USERS	10

class RollDiceRankingUser
{
public:
	char			szCharacterName[32];
	UINT			uNumber;

					RollDiceRankingUser( char * pszCharacterName, UINT _uNumber ){ STRINGCOPY(szCharacterName,pszCharacterName); uNumber = _uNumber; }
};

struct PacketRollDiceDrop : Packet
{
	int				iID;
	EItemID			eItemID;

	BOOL			bAcceptItem;
};

struct PacketRollDiceDropItem : PacketRollDiceDrop
{
	Item			sItem;
};

struct PacketRollDiceRanking : Packet
{
	char			szCharacterName[32];
	UINT			uNumber;
};