#pragma once
class CHalloweenHandler
{
protected:
	DWORD												dwUpdateCombinationTime = 0;
	DWORD												dwRuneEvil[3];
	DWORD												dwRuneInfernal[3];
	int													iCombinationID;
public:
	CHalloweenHandler();
	virtual ~CHalloweenHandler();


	void												Init();

	void												SendEventStatusToAllUsers ( );
	void												SendEventStatusToUser ( User * pcUser );
	BOOL												OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket );

	BOOL												OnSpawnEventMonster( struct CharacterData * psCharInfo );
	BOOL												OnUnitKilled( UnitData * pcUnitData );

	int													GetCombinationID() { return iCombinationID; };

	void												SetCombinationID( int iID );

	void												Tick();

};

