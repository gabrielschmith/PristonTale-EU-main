#pragma once
class CGeneralEventHandler
{
private:
	BOOL												IsEasterRune( DWORD dwCode );
public:
	CGeneralEventHandler();
	virtual ~CGeneralEventHandler();

	BOOL												OnSpawnMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap );
	BOOL												OnSpawnStarWarsMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap );
	BOOL												OnSpawnBeeMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap );
	BOOL												OnSpawnEasterMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap );

	BOOL												OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket );

	void												OnSayTime( UnitData * pcUnitData );

};

