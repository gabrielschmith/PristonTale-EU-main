#pragma once

struct MixServerData
{
	int						iaItemID[10];
	int						iaSheltomCount[15]; //# sheltom req (index 0 = Lucidy, index 1 = Sereneo etc)
	int						iaTypeAttributte[8];
	float					faAttributte[8];
	int						iaAttributtePercent[8];
	char					szDescription[64];
	int						iMixUniqueId;
	EMixTypeName			eMixTypeName;
	EMixType				iType;
	int						iSheltomCount;
	char					cPadding[0x090];
};


class CMixHandler
{
public:
	CMixHandler();
	virtual ~CMixHandler();

	void							SQLLoadMixingList( );

	void							OnSocketSystemNPCClicked ( User * pcUser, UnitData * pcUnitData );
	void							SendPacketMixList ( User * pcUser );


	BOOL							OnMixingItem(User* pcUser, struct PacketMixServer* psPacket);

	EMixColor						GetMixColorFromMixType(EMixTypeName pMixTypeName);
	const char*						GetMixTypeStringFromMixType(EMixTypeName pMixTypeName);

	int								GetSheltomIndexFromCode(int iSheltomItemCode);
	void							TestJeraRespec(int iItemCode);

	BOOL							OnRespecingItem(User* pcUser, struct PacketMixServer* psPacket, int numOfRespecsFound);

	void							SendMixDescription(UserData* pcUserData, const char* pMixDescription);
	void							SendMixDescription(UserData* pcUserData, UINT puMix);

	BOOL							SetMixStatus( Item * psItem, MixServerData * psMixServerData);

	void							AddAttributeBonusToItem(short* value, int attributeBonus, int attributeBonusType);
	void							AddAttributeBonusToItem(int* value, int attributeBonus, int attributeBonusType);
	void							AddAttributeBonusToItem(float* value, float attributeBonus, int attributeBonusType);

	void							RemoveAttributeBonusFromItem(short* value, int attributeBonus, int attributeBonusType);
	void							RemoveAttributeBonusFromItem(int* value, int attributeBonus, int attributeBonusType);
	void							RemoveAttributeBonusFromItem(float* value, float attributeBonus, int attributeBonusType);

	BOOL							SetClearMixStatus( Item * psItem, int iUniqueID);

	EMixType						GetTypeMixByItemCode( int iItemID );

	void							SetItemMixByUniqueID ( Item * psItem, int iUniqueID );

private:
	MixServerData*					GetMixServerDataByUniqueID(int iUniqueID);
};

