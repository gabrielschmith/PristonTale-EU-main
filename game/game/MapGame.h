#pragma once

class MapGame
{
public:
	//Constructor
											MapGame();
	virtual								   ~MapGame();

	void									Load();

	void									SetMessageBoxMap( int iMap, BOOL bUsingCore = FALSE );

	void									UpdateBlockedPosition( UnitData * pcUnitData );

	BaseMap								  * pcaBaseMap;
};