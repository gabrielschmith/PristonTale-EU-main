#include "stdafx.h"
#include "DLL.h"
#include "HNSSkill.h"

extern int __cdecl RecvSkillSingleTarget( UserData * pcUserData, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
extern int RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
extern int __cdecl RecvBuffSkill( UserData * pcUserData, PacketSkillCast * packet );
extern void __cdecl SendAttackRatting( UserData * pcUserData, UINT iAttackRating, UINT idEnemy );

void __cdecl InitServer()
{

}

void __cdecl InitSocketServer( HWND hWnd )
{

}

void __cdecl CreateUnitEnemyData()
{
	UNITINFODATA->CreateUnitEnemyData();
}

void __cdecl CreateItemMemoryTable()
{
	ITEMSERVER->CreateItemMemoryTable();
	LOOTSERVER->SQLUpdateDropTableFromDatabase ();
}

void __cdecl ReadUnitNpcStand( Map * pStageData )
{
	UNITINFODATA->ReadUnitNpcStand( pStageData );
}

void __cdecl ReadUnitNpcData( CharacterData * pCharInfo, UnitInfo *pMonInfo, char *pDialogMessage )
{
	UNITINFODATA->ReadUnitNpcData( pCharInfo, pMonInfo, pDialogMessage );
}

void __cdecl LoadAreaStages()
{
	MAPSERVER->Load();
}


UINT uCurrentStageUnitSpawn = 0;


//LoadAreaStages is called before this
void __cdecl ReadUnitSpawnData( Map * pStageData )
{
	//std::cout << "ReadUnitSpawnData " << uCurrentStageUnitSpawn << std::endl;

	UNITINFODATA->ReadUnitSpawnData( pStageData, uCurrentStageUnitSpawn);
	uCurrentStageUnitSpawn++;
}

void __cdecl ReadUnitEnemyStage( Map::SpawnSetting * pMonsterStageList )
{
	UNITINFODATA->ReadUnitEnemyStage( pMonsterStageList );
}

BOOL __stdcall AddMonsterSpawn( UnitData * pcUnitData )
{
	Map * pcMap = NULL;
	READTHIS( pcMap );
	if ( MAPSERVER->AddMonsterSpawn( pcMap, pcUnitData ) )
	{
		UNITSERVER->OnMonsterAddedToMap( pcMap, pcUnitData );
	}
}

BOOL __stdcall DelMonsterSpawn( UnitData * pcUnitData )
{
	Map * pcMap = NULL;
	READTHIS( pcMap );
	return MAPSERVER->DelMonsterSpawn( pcMap, pcUnitData );
}

int __cdecl OnLoginAccount( UserData * pcUserData, const char * pszUserID, const char * pszPassword )
{
	return 0;
}



BOOL __cdecl OnLoginSuccess( UserData * pcUserData )
{
	return ACCOUNTSERVER->OnLoginSuccess( pcUserData );
}

void __cdecl SendDebugChat( UserData * pcUserData, const char * pszText )
{
	CHATSERVER->SendDebugChat( USERDATATOUSER(pcUserData), CHATCOLOR_Error, pszText );
}

void __cdecl SendBossMessage( BaseMap * pcBaseMap, UINT pUnit )
{
	UNITSERVER->SendBossMessage( pcBaseMap, pUnit );
}

/// <summary>
/// Called via PKTHDR_AttackResult
/// </summary>
BOOL __cdecl OnUnitNpcClick( UserData * pcUserData, UnitData * pcUnitData )
{
	WARN( "NPC Clicked via ASM?: %s", pcUnitData->GetName() );

	return UNITSERVER->OnUnitNpcClick( USERDATATOUSER( pcUserData ), pcUnitData );
}

void __cdecl OnSendExp( UserData * pcUserData, UnitData * pcUnitData )
{
	DEBUGSTATUS( "OnSendExp called from ASM" );

	UNITSERVER->OnSendExp( pcUserData, pcUnitData, 0 );
}

void __cdecl OnSetItemPerfect( Item * pItem )
{
	//ITEMSERVER->OnSetItemPerfect( pItem );
}

void __cdecl OnReadPostBox( UserData * pcUserData )
{
	ACCOUNTSERVER->OnReadPostBox( pcUserData );
}

void __cdecl OnDeletePostBox( UINT ItemID )
{
	ACCOUNTSERVER->OnDeletePostBox( ItemID );
}

BOOL __cdecl ReadUnitAnimation( char * pszModelFile, ModelDataOld * pModelData )
{
	BOOL bResult = UNITINFODATA->ReadUnitAnimation( pszModelFile, pModelData );
	return bResult;
}

UINT __cdecl ReadModelSize( const char * pszModelFile )
{
	UINT uResult = UNITINFODATA->ReadModelSize( pszModelFile );
	return uResult;
}

void __cdecl SendMixDescription( UserData * pcUserData, UINT puMix )
{
	MIXHANDLER->SendMixDescription( pcUserData, puMix );
}

/**
Save item to database (ItemCreateLog)
*/
BOOL __cdecl OnSaveItemData( Item * psItem )
{
	return ITEMSERVER->OnSaveItemData( psItem );
}

BOOL __cdecl OnLoadItemData( ItemLoadData * psItem, const char * pszCode )
{
	return ITEMSERVER->OnLoadItemData( psItem, pszCode );
}

BOOL __cdecl RecordPoint( UserData * pcUserData, int iLevel, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode )
{
	return BELLATRASERVER->RecordPoint( pcUserData, iLevel, SodScore, SodKillCount, SodTotalScore, SodTotalSucessUser, SodTotalUser, SodCode );
}

BOOL __cdecl AddUserOnline( UserData * pcUserData, BOOL bOffline )
{
	return USERSERVER->AddUserOnline( pcUserData, bOffline );
}

int __cdecl GetAgingType( UserData * pcUserData )
{
	int result = ITEMSERVER->GetAgingType ( pcUserData ); //test

	//std::cout << "GetAgingType = " << result << std::endl;

	return result;
}

void __cdecl OnLogWrite( UserData * pcUserData, int iLogID, const char * pszLog, int iLogType )
{
	LOGSERVER->OnLogWrite( pcUserData, iLogID, pszLog, ( LogServer::ELogServerType )iLogType );
}

void __cdecl OnLogItem( int iType, int iSize, void * pLog )
{
	LOGSERVER->OnLogItem( iType, iSize, pLog );
}

void __cdecl SetupMonsterBellatra( UINT BellatraMemory, UINT ConfigMemory )
{
	UNITINFODATA->SetupMonsterBellatra( BellatraMemory, ConfigMemory );
}

BOOL __cdecl OnItemPremium( DWORD dwCode, int iX, int iY, int iZ, UserData * pcUserData )
{
	typedef BOOL( __cdecl *t_PremiumUse )( DWORD dwCode, int iX, int iY, int iZ, UserData * pcUserData );
	t_PremiumUse PremiumUse = (t_PremiumUse)0x00558140;
	if( !ITEMSERVER->OnPremiumItem( dwCode, iX, iY, iZ, pcUserData ) )
		return PremiumUse( dwCode, iX, iY, iZ, pcUserData );
	return TRUE;
}

void __cdecl OnSetDrop( UserData * pcUserData, UnitData * pcUnitData )
{
	UNITSERVER->OnSetDrop( pcUserData, pcUnitData );
}

BOOL __cdecl SendChatServer( SocketData * pcSocketData, void * pData )
{
	CHATSERVER->SendChatSpyMember( USERSERVER->GetUserdataSocket( pcSocketData ), pData );
	return TRUE;
}

BOOL __cdecl SocketAccept( SOCKET s, sockaddr_in * addr )
{
	SOCKETSERVER->SocketAccept( s, addr );
	return TRUE;
}

void __cdecl SocketPacket( SocketData * sd, PacketReceiving * p )
{
	SOCKETSERVER->SocketPacket( sd, p );
}

void __stdcall SocketSend( PacketSending * p, BOOL bEncrypt )
{
   	SocketData * sd = NULL;
	READTHIS( sd );
    if (sd)
    {
        PACKETSERVER->Send(sd->ud, p, bEncrypt);

		//sd->SendPacket( (Packet*)p, bEncrypt );
    }
}

void __cdecl ReadSocketHandler( DWORD s, DWORD dwParam )
{
	SOCKETSERVER->OnReadHandler( (SOCKET)s, dwParam );
}

void __cdecl OnLoadCharacterInventory( UserData * pcUserData )
{
	LOGSERVER->OnLogLoadItemCharacter( pcUserData );
}

BOOL __cdecl InventoryServerCheck( UserData * pcUserData, int iUnused )
{
	return USERSERVER->InventoryServerCheck( pcUserData, iUnused );
}

void __cdecl SendChatTrade( UserData * pcUserData, PacketChatBoxMessage * pPacket )
{
}

void __cdecl OnDisconnectUser( SocketData * pcSocketData )
{
	if( pcSocketData )
	{
		LOGSERVER->LogSocketClose( pcSocketData, "OnDisconnectUser [0x0055C390]", _ReturnAddress() );

		if( pcSocketData->u && pcSocketData->u->bNetServer )
		{
			LOGERROR( "NET SERVER TRIED DC %p", _ReturnAddress() );
			return;
		}
		if( NETSERVER->GetNetConnectionBySocketData( pcSocketData ) != NULL )
		{
			LOGERROR( "NET SERVER TRIED DC %p", _ReturnAddress() );
			return;
		}
		WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcSocketData );
	}
}

BOOL __cdecl OnDisconnectUserEx( UserData * pcUserData )
{
	if( pcUserData )
		LOGSERVER->LogSocketClose( pcUserData->pcSocketData, "OnDisconnectUserEx (Hook)", _ReturnAddress() );

	if( pcUserData )
		pcUserData->bDisconnectFunction = TRUE;
	ACCOUNTSERVER->OnDisconnectUser( pcUserData );
	return FALSE;
}

BOOL __cdecl FindItemWarehouse( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwCheckSum )
{
	//std::cout << " __cdecl FindItemWarehouse, pcUserData->iWarehouseStatus = " << pcUserData->iWarehouseStatus << std::endl;

	return ACCOUNTSERVER->FindItemWarehouse( pcUserData, dwCode, dwHead, dwCheckSum, pcUserData->iWarehouseStatus == 2);
}

BOOL __cdecl AddItemWarehouse( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwCheckSum )
{
	//std::cout << " __cdecl AddItemWarehouse, pcUserData->iWarehouseStatus = " << pcUserData->iWarehouseStatus << std::endl;

	//Disallow adding of items to
	//warehouse (for post-merge after seasonal)
	if ( pcUserData->iWarehouseStatus == 2 )
		return FALSE;

	DropItemData sItem;
	sItem.iItemID = dwCode;
	sItem.iChk1 = dwHead;
	sItem.iChk2 = dwCheckSum;
	return ACCOUNTSERVER->AddWarehouseItem( pcUserData, &sItem, FALSE);
}

BOOL __cdecl DeleteItemWarehouse( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwCheckSum )
{
	//std::cout << " __cdecl DeleteItemWarehouse, pcUserData->sWarehouseMode = " << pcUserData->iWarehouseStatus << std::endl;


	DropItemData sItem;
	sItem.iItemID = dwCode;
	sItem.iChk1 = dwHead;
	sItem.iChk2 = dwCheckSum;
	return ACCOUNTSERVER->DeleteWarehouseItem( pcUserData, &sItem, pcUserData->iWarehouseStatus == 2 );
}

BOOL __cdecl CheatLog( UserData * pcUserData, void * pCheatLog )
{
	return LOGSERVER->OnLogCheat( pcUserData, pCheatLog );
}

void __cdecl OnHandlerFunctions( DWORD dwCode, DWORD * pMemory )
{
	return USERSERVER->OnHandlerFunctions( dwCode, pMemory );
}

void __cdecl OnSetTargetPosition( UserData * pcUserData, UnitData * pcUnitData )
{
	UNITSERVER->OnSetTargetPosition( pcUserData, pcUnitData );
}

UINT __cdecl AllocMemoryPT( UINT uSize )
{
	UINT uMemory = (UINT)VirtualAlloc( NULL, uSize + 4, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
	*( (UINT*)uMemory ) = uSize;
	return ( uMemory + 4 );
}

void __cdecl DeallocMemoryPT( UINT uMemory )
{
	VirtualFree( (void*)( uMemory ), *(UINT*)( uMemory - 4 ), MEM_RELEASE );
}

void __cdecl OnOutCharacter( UserData * pcUserData )
{
	CHARACTERSERVER->OnOutCharacterData( pcUserData );
}

BOOL __stdcall SetSpawnUnitData( UnitData * pcUnitData )
{
	Map * pcMap;
	READTHIS( pcMap );
	return MAPSERVER->SetSpawnMonster( pcMap, pcUnitData );
}

BOOL __cdecl UnitAutoMain( UnitData * pcUnitData )
{
	return UNITSERVER->UpdateUnitData( pcUnitData );
}

DWORD __cdecl OnSetClassJobItem( int iClass, DefinitionItem * ps )
{
	return (DWORD)ItemServer::GetItemSpecClassJob( iClass, ps );
}

BOOL __cdecl UpdateBellatra( Map * pcStage, MonsterMapSpawnHandle * psMonsterData )
{
	return BELLATRASERVER->UpdateServer( pcStage, psMonsterData );
}

BOOL __cdecl OnDownAgeHandler( Item * psItem, int iCount )
{//0x004BD8F0
	return AGEHANDLER->OnDownAgeHandler( psItem, iCount );
}

BOOL __cdecl OnUpAgeHandler( Item * psItem )
{//0x004BD540
	return AGEHANDLER->OnUpAgeHandler( psItem );
}

BOOL __cdecl OnCrystalUse( EItemID iItemID, int iX, int iY, int iZ, UserData * pcUserData )
{
	return MAPSERVER->SpawnCrystal( iItemID, iX, iY, iZ, pcUserData );
}

BOOL __cdecl UpdateDamageDataPVP( UserData * pcKiller, UserData * pcVictim, int iDamage )
{
	return DAMAGEHANDLER->UpdateBCPVPScoring( pcKiller, pcVictim, iDamage );
}

BOOL __cdecl IsAreaDamage()
{
	return FALSE;
}

BOOL __cdecl SendPvPDamage( UserData * pcUserData, UserData * pcTarget, int iDamage, EAttackState iID, int iParam1, int iParam2 )
{
	//should no longer be used
	return FALSE; // return DAMAGEHANDLER->SendDamagePvPData( pcUserData, pcTarget, iDamage, iID, iParam1, iParam2 );
}

int __cdecl IsFreezedUnitFollow( UnitData * pcUnitData )
{
	return UNITSERVER->IsFreezedUnitFollow( pcUnitData );
}

BOOL __cdecl CanHitTargetAreaDamage( UserData * pcUserData, UnitData * pcUnitData )
{
	return DAMAGEHANDLER->CanHitTargetAreaDamage( pcUserData, pcUnitData );
}

int __stdcall MakeFaceArea( int iX, int iZ, int iWidth, int iHeight, int iTop, int iBottom )
{
	Stage * pcThis;
	READTHIS( pcThis );

	return MAPSERVER->MakeFaceArea( pcThis, iX, iZ, iWidth, iHeight, iTop, iBottom );
}

BOOL __stdcall SetupPolygonArea()
{
	Stage * pcThis;
	READTHIS( pcThis );

	return MAPSERVER->SetupPolygonArea( pcThis );
}

BOOL __stdcall SendItemNearUsers( Loot * psItemStage )
{
	//std::cout << TICKCOUNT << " : " << "SendItemNearUsers" << std::endl;

	Map * pcMap;
	READTHIS( pcMap );

	return MAPSERVER->SendItemStageNearUsers( pcMap, psItemStage );
}

void PatchMemory(DWORD p_Address, char* p_Patch, std::size_t p_Size)
{
	DWORD l_OldFlags;
	VirtualProtect((void*)p_Address, p_Size, PAGE_EXECUTE_READWRITE, &l_OldFlags);
	for (size_t l_I = 0; l_I < p_Size; ++l_I)
		*reinterpret_cast<BYTE*>(p_Address + l_I) = p_Patch[l_I];
	VirtualProtect((void*)p_Address, p_Size, l_OldFlags, NULL);
}
void PatchMemoryNOP(DWORD p_Address, std::size_t p_Size)
{
	DWORD l_OldFlags;
	VirtualProtect((void*)p_Address, p_Size, PAGE_EXECUTE_READWRITE, &l_OldFlags);
	for (size_t l_I = 0; l_I < p_Size; ++l_I)
		*reinterpret_cast<BYTE*>(p_Address + l_I) = 0x90;	///< NOP
	VirtualProtect((void*)p_Address, p_Size, l_OldFlags, NULL);
}

void Patch_smTRANSCODE_SAVETHROWITEM2()
{
	char l_Patch1[] = { '\xEB', '\x3D'};
	char l_Patch2[] = { '\xEB', '\x54'};

	/// if (rsDeleteInvenItem(lpPlayInfo, lpTransCommandEx->WParam, lpTransCommandEx->LParam, lpTransCommandEx->SParam) < 0)
	PatchMemoryNOP(0x5704BD, 25);
	/// if (rsDeleteInvenItem(lpPlayInfo, lpTransCommandEx->WParam, lpTransCommandEx->LParam, lpTransCommandEx->SParam) < 0)
	PatchMemory   (0x5704D6, l_Patch1, 2);
	/// lpPlayInfo->SubServerMoney(lpTransCommandEx->ExParam, WHERE_THROW_ITEM);
	PatchMemoryNOP(0x570616, 5);
	/// rsRecord_ItemLog(lpPlayInfo, sinGG1 | sin01, lpTransCommandEx->ExParam, 0, ITEMLOG_PUT);
	PatchMemoryNOP(0x57061B, 13);
	/// rsRecord_ItemLog(lpPlayInfo, sinGG1 | sin01, lpTransCommandEx->ExParam, 0, ITEMLOG_PUT);
	PatchMemory   (0x570628, l_Patch2, 2);
	/// lpPlayInfo->UnsaveMoney = lpTransCommandEx->WxParam - lpTransCommandEx->ExParam;
	PatchMemoryNOP(0x570684, 6);
}


//////////////////////////////////////////////////////////////////////////

extern int __cdecl CustomRecordCharacterDamage(UserData * pUserData, UnitData * pUnitData, int Damage);

extern signed int CustomReadStageData();

/// Here this is my 2 customl function ok ? ok
extern BOOL __fastcall CustomAddGoldServer(UserData * p_UserData, void * p_PlaceHolder, int p_Gold, EWhereAction p_Where);
extern BOOL __fastcall CustomSubGoldServer(UserData * p_UserData, void * p_PlaceHolder, int p_Gold, EWhereAction p_Where);

/// Now a special trick
/// extern BOOL __thiscall sinSubServerGold(UserData *p_UserData,  int p_Gold, EWhereAction p_Where)
/// this is the prototype of the function to patch
/// So our custom function have to match the same prototype
/// Don't u see we have 1 more argument ?true do you see something else different except the name ? a lot of it is different,
/// and now ? now it makes sense. I have a lot to learn lol nothing different ? the void.  yes __fastcall? yes


//// __stdcall, __fastcall, __thiscall, __cdecl are calling conventions
/// Each calling convetion have a different manear to push arguement into the stack when you call a function
//// Interesting! I will need to study more about these calls online.

/// the function  we want to deturn is __thiscall it's mean this is a CLASS method with the "this" variable

/// Ok so in X86 asm the THIS oibject is always in the ECX register ok ? ok. good info
///  __stdcall, __cdecl  conventions put all arguements into an array in EBP same for __thiscall
/// but __fastcall put the 2 first arguments into ECX and EDX registers then the others in EBP ok ? i seee!
/// We exploit a small trick we make our custom function __fastcall so that p_UserData = ECX & p_PlaceHolder = EDX
/// So we now can retrieve the THIS pointer you see ? yup i follow now. it's starting to make sense. let's see on IDA ok ? ok


/// HEre by default we have no log for money change in the server .Exe so i patch all call to function
////  sinSub/Add money to our custom one so i can log all changes
//// how did you patch the calls to point to this function>?
void PatchServerGoldFunctions()
{
    DWORD l_SubGoldCalls[] = {

		/// So here we have all the addresses of call to original sub gold function ok ? ok
		/// a call in X86 asm is 5 bytes long
		/// 1 first is 0xE8 which mean call in asm ok? asm? assembly, machine language ,  cpu instructions. ok
		/// the next 4 bytes (4bytes= int32 variable from 0 to 0xFFFFFFFF) is the RELATIVE distance to the function you want to call

		//// so to patch the relative distance is :  FunctionToCall - (CurrentAddress - SizeOFACall)
		/// What is the size of a call instruction ? 4 bytes, right? int32 + 0xE8 so 5. i see.
         0x54BB0E, 0x556BD6, 0x556F04, 0x556F32, 0x568801, 0x5696F1, 0x569B69, 0x56A365, 0x56A410, 0x56B773, 0x570616
    };
    int l_SubGoldCallsCount = sizeof(l_SubGoldCalls) / sizeof(l_SubGoldCalls[0]);

    for (int l_I = 0; l_I < l_SubGoldCallsCount; ++l_I)
    {
        DWORD l_Address     = l_SubGoldCalls[l_I];
        DWORD l_PatchSize   = 5;
        DWORD l_OldFlags;

        VirtualProtect((void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags);
		/// So here for each address to patch in array l_SubGoldCalls
		/// We write 0xE8
		/// Then the current relative distance to our function. i see
        *reinterpret_cast<BYTE *> (l_Address)       = 0xE8;
        *reinterpret_cast<DWORD *>(l_Address + 1)   = (DWORD)CustomSubGoldServer - (DWORD)l_Address - l_PatchSize;
        VirtualProtect((void *)l_Address, l_PatchSize, l_OldFlags, NULL);
    }

    DWORD l_AddGoldCalls[] = {
        0x54BB35, 0x568810, 0x568BCA, 0x56A613, 0x56B783, 0x57084B
    };
    int l_AddGoldCallsCount = sizeof(l_AddGoldCalls) / sizeof(l_AddGoldCalls[0]);

    for (int l_I = 0; l_I < l_AddGoldCallsCount; ++l_I)
    {
        DWORD l_Address     = l_AddGoldCalls[l_I];
        DWORD l_PatchSize   = 5;
        DWORD l_OldFlags;

        VirtualProtect((void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags);
        *reinterpret_cast<BYTE *> (l_Address)       = 0xE8;
        *reinterpret_cast<DWORD *>(l_Address + 1)   = (DWORD)CustomAddGoldServer - (DWORD)l_Address - l_PatchSize;
        VirtualProtect((void *)l_Address, l_PatchSize, l_OldFlags, NULL);
    }
}

/// <summary>
/// Leaked: rsCheckItemCode
/// Address: 0x0055B880
/// </summary>
/// <param name="p_UserData"></param>
/// <param name="p_PlaceHolder"></param>
/// <param name="p_Gold"></param>
/// <param name="p_Where"></param>
/// <returns></returns>
signed int __cdecl CustomCheckItemCode ( UserData * p_UserData, Item * psItem )
{
	int result = CALL_WITH_ARG2 ( 0x0055B880, (DWORD)p_UserData, (DWORD)psItem );
	DEBUG ( "CustomCheckItemCode - Result = %d", result );
	return result;
}

void PatchCheckItemCode ()
{
	/*DWORD l_Address = 0x00573276;
	DWORD l_PatchSize = 5;
	DWORD l_OldFlags;

	VirtualProtect ( (void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags );
	/// So here for each address to patch in array l_SubGoldCalls
	/// We write 0xE8
	/// Then the current relative distance to our function. i see
	*reinterpret_cast<BYTE *> ( l_Address ) = 0xE8;
	*reinterpret_cast<DWORD *>( l_Address + 1 ) = (DWORD)CustomCheckItemCode - (DWORD)l_Address - l_PatchSize;
	VirtualProtect ( (void *)l_Address, l_PatchSize, l_OldFlags, NULL );*/
}

//extern BOOL __fastcall CustomAddGoldServer(UserData * p_UserData, void * p_PlaceHolder, int p_Gold, EWhereAction p_Where);
Loot * __fastcall CustomAddItem ( Map * p_Map, void * p_PlaceHolder, LootItem * lpItem, int x, int y, int z, int BeginMode )
{
	return MAPSERVER->AddItemV2( p_Map, lpItem, x, y, z, (ELootType)BeginMode );
}

void PatchAddItem ()
{
	DWORD l_Calls[] = {
		0x5500B0, 0x552693, 0x55C046, 0x560B41, 0x560CD4, 0x56DCB2, 0x56F04A, 0x570447
	};
	int l_CallsCount = sizeof(l_Calls) / sizeof(l_Calls[0]);

	for (int l_I = 0; l_I < l_CallsCount; ++l_I)
	{
		DWORD l_Address     = l_Calls[l_I];
		DWORD l_PatchSize   = 5;
		DWORD l_OldFlags;

		VirtualProtect((void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags);
		*reinterpret_cast<BYTE *> (l_Address)       = 0xE8;
		*reinterpret_cast<DWORD *>(l_Address + 1)   = (DWORD)CustomAddItem - (DWORD)l_Address - l_PatchSize;
		VirtualProtect((void *)l_Address, l_PatchSize, l_OldFlags, NULL);
	}
}

void memcpy_s(PVOID address, char* val, int bytes)
{
	DWORD d, ds;
	VirtualProtect(address, bytes, PAGE_EXECUTE_READWRITE, &d);
	memcpy(address, val, bytes);
	VirtualProtect(address, bytes, d, &ds);
}


void PatchSingleByte(PVOID address, BYTE val)
{
	DWORD l_OldFlags;
	VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &l_OldFlags);
	*reinterpret_cast<BYTE*>(address) = val;
	VirtualProtect((void*)address, 1, l_OldFlags, NULL);
}


//Patch all /5 (remove) in Server EXE
// so that we can do our own PVP scaling
void PatchPvPDamageReduction()
{
	DWORD l_PatchAddressForNop[] =
	{
		0x57D8D2,
		0x57D8D3,

		0x57CB5E,
		0x57CB5F,
		0x57CB62,
		0x57CB63,
		0x57CB65,
		0x57CB66,
		0x57CB67,

		0x57AABF,
		0x57AAC0,

		0x415A49,
		0x415A4A
	};

	int l_Count = sizeof(l_PatchAddressForNop) / sizeof(l_PatchAddressForNop[0]);
	for (int l_I = 0; l_I < l_Count; ++l_I)
	{
		PatchSingleByte((PVOID)l_PatchAddressForNop[l_I], 0x90); //NOP
	}

	PatchSingleByte((PVOID)0x57D8E6, 0x53);
	PatchSingleByte((PVOID)0x57CB6F, 0x51);
	PatchSingleByte((PVOID)0x57ADB8, 0x56);
	PatchSingleByte((PVOID)0x57AAD3, 0x56);
	PatchSingleByte((PVOID)0x415A5D, 0x56);

	unsigned char szPatch[3] = { 0xFF, 0x73, 0x20 };
	memcpy_s((PVOID)0x57AC2C, (char*)szPatch, 3);
}

void PatchRecordCharDamageFunctions()
{
	DWORD l_RecordDamageCalls[] = {
	  0x57B679, 0x57BB8A, 0x57BF2F, 0x57C1FB, 0x57C4F5, 0x57D307
	};
	int l_RecordDamageCallsCount = sizeof(l_RecordDamageCalls) / sizeof(l_RecordDamageCalls[0]);

	for (int l_I = 0; l_I < l_RecordDamageCallsCount; ++l_I)
	{
		DWORD l_Address = l_RecordDamageCalls[l_I];
		DWORD l_PatchSize = 5;
		DWORD l_OldFlags;

		VirtualProtect((void*)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags);
		*reinterpret_cast<BYTE*> (l_Address) = 0xE8;
		*reinterpret_cast<DWORD*>(l_Address + 1) = (DWORD)CustomRecordCharacterDamage - (DWORD)l_Address - l_PatchSize;
		VirtualProtect((void*)l_Address, l_PatchSize, l_OldFlags, NULL);
	}
}

void PatchLoadMapFunctions()
{
	DWORD l_Calls[] = {
	  0x56BBD0
	};
	int l_CallsCount = sizeof(l_Calls) / sizeof(l_Calls[0]);

	for (int l_I = 0; l_I < l_CallsCount; ++l_I)
	{
		DWORD l_Address = l_Calls[l_I];
		DWORD l_PatchSize = 5;
		DWORD l_OldFlags;

		VirtualProtect((void*)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags);
		*reinterpret_cast<BYTE*> (l_Address) = 0xE8;
		*reinterpret_cast<DWORD*>(l_Address + 1) = (DWORD)CustomReadStageData - (DWORD)l_Address - l_PatchSize;
		VirtualProtect((void*)l_Address, l_PatchSize, l_OldFlags, NULL);
	}
}

/// <summary>
/// Override the SetStartPosNearChar function in assembly
/// </summary>
BOOL __fastcall CustomSetStartPosNearChar(Map * pcMap, void * p_PlaceHolder, UnitData *lpChar, int cx, int cy, int cz)
{
	return MAPSERVER->SetStartPosNearChar( pcMap, lpChar, cx, cy, cz );
}

void PatchSetStartPosNearChar()
{
	DWORD l_Calls[] = {
		0x55A1C3
	};

	int l_CallsCount = sizeof( l_Calls ) / sizeof( l_Calls[0] );

	for ( int l_I = 0; l_I < l_CallsCount; ++l_I )
	{
		DWORD l_Address = l_Calls[l_I];
		DWORD l_PatchSize = 5;
		DWORD l_OldFlags;

		VirtualProtect( (void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags );
		*reinterpret_cast<BYTE *> ( l_Address ) = 0xE8;
		*reinterpret_cast<DWORD *>( l_Address + 1 ) = (DWORD)CustomSetStartPosNearChar - (DWORD)l_Address - l_PatchSize;
		VirtualProtect( (void *)l_Address, l_PatchSize, l_OldFlags, NULL );
	}
}

/// <summary>
/// Hook calls to the the OpenMonster function in assembly
/// See smCHAR *STG_AREA::OpenMonster(STG_CHAR_INFO *lpStgCharInfo, int gGroup)
/// </summary>
UnitData * __cdecl CustomOpenSubMonster(UnitData *lpMainChar, DWORD dwCharSoundCode, int x, int y, int z, int MaxFlag)
{
	//int	rsPLAYINFO::SubServerMoney( int Money , int WhereParam )
	typedef UnitData *(__thiscall * tfnOpenSubMonster)(UnitData *lpMainChar, DWORD dwCharSoundCode, int x, int y, int z, int MaxFlag);
	tfnOpenSubMonster fnOpenMonster = (tfnOpenSubMonster)0x00559610;
	UnitData * pcUnitData = fnOpenMonster( lpMainChar, dwCharSoundCode, x, y, z, MaxFlag );

	if ( pcUnitData )
	{
		//UNITSERVER->OnSubMonsterOpened(pcUnitData);
	}

	return pcUnitData;
}


/// <summary>
/// smCHAR *rsOpenSubMonster(smCHAR *lpMainChar, DWORD dwCharSoundCode, int x, int y, int z, int MaxFlag)
/// 0x559610
/// </summary>
void HookCallsToFunction_OpenSubMonster()
{
	DWORD l_Address = 0x0042CBF0; //call
	DWORD l_PatchSize = 5;
	DWORD l_OldFlags;
	VirtualProtect( (void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags );
	*reinterpret_cast<BYTE *> ( l_Address ) = 0xE8;
	*reinterpret_cast<DWORD *>( l_Address + 1 ) = (DWORD)CustomOpenSubMonster - (DWORD)l_Address - l_PatchSize;
	VirtualProtect( (void *)l_Address, l_PatchSize, l_OldFlags, NULL );
}

/// <summary>
/// Hook calls to the the OpenMonster function in assembly
/// See smCHAR *STG_AREA::OpenMonster(STG_CHAR_INFO *lpStgCharInfo, int gGroup)
/// </summary>
UnitData * __fastcall CustomOpenMonster(Map * pcMap, void * p_PlaceHolder, MonsterMapSpawnHandle * lpStgCharInfo, int gGroup)
{
	//int	rsPLAYINFO::SubServerMoney( int Money , int WhereParam )
	typedef UnitData *(__thiscall * tfnOpenMonster)(Map * pcMap, MonsterMapSpawnHandle *lpStgCharInfo, int gGroup);
	tfnOpenMonster fnOpenMonster = (tfnOpenMonster)0x00559F30;
	UnitData * pcUnitData = fnOpenMonster(pcMap, lpStgCharInfo, gGroup);

	if ( pcUnitData )
	{
		//UNITSERVER->OnMonsterOpened(pcMap, pcUnitData); //not realy needed
	}

	return pcUnitData;
}


void HookCallsToFunction_OpenMonster()
{
	DWORD l_Address = 0x0055BB7D; //call
	DWORD l_PatchSize = 5;
	DWORD l_OldFlags;
	VirtualProtect( (void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags );
	*reinterpret_cast<BYTE *> ( l_Address ) = 0xE8;
	*reinterpret_cast<DWORD *>( l_Address + 1 ) = (DWORD)CustomOpenMonster - (DWORD)l_Address - l_PatchSize;
	VirtualProtect( (void *)l_Address, l_PatchSize, l_OldFlags, NULL );


	l_Address = 0x08B81018,	//jmp
	l_PatchSize = 5;
	l_OldFlags;
	VirtualProtect( (void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags );
	*reinterpret_cast<DWORD *>( l_Address + 1 ) = (DWORD)CustomOpenMonster - (DWORD)l_Address - l_PatchSize;
	VirtualProtect( (void *)l_Address, l_PatchSize, l_OldFlags, NULL );


	l_Address = 0x08B81050;	//jnz
	l_PatchSize = 6;
	l_OldFlags;
	VirtualProtect( (void *)l_Address, l_PatchSize, PAGE_EXECUTE_READWRITE, &l_OldFlags );
	*reinterpret_cast<DWORD *>( l_Address + 2 ) = (DWORD)CustomOpenMonster - (DWORD)l_Address - l_PatchSize;
	VirtualProtect( (void *)l_Address, l_PatchSize, l_OldFlags, NULL );
}


//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH: // I think you noticed some adm//console kind of thing first time you looked? dont think we can fix it without looking at the source code during run time
		// need to attach debugger
		// k how we do that?
	//	Patch_smTRANSCODE_SAVETHROWITEM2();

		PatchSetStartPosNearChar();
		PatchAddItem();
		PatchCheckItemCode ();
        PatchServerGoldFunctions();
		PatchPvPDamageReduction();
		PatchRecordCharDamageFunctions();
		PatchLoadMapFunctions();


        /// Bellatra castle, on fail, disable gold restore
        PatchMemoryNOP(0x5604C7, 5);

		//Player data save fail, item loss related.
		char l_SavePatchFailed[] = { '\xEB' };
		PatchMemory(0x44ff88, l_SavePatchFailed, 1);

		char szDataRETNShop[] = { '\xC3', '\x90' };
		PatchMemory( 0x00551290, szDataRETNShop, 2 );


		// dont we just try to find a statement that does /close login /close server ? a kill switch
		//(*(DWORD*)0x08B64330) = (DWORD)&RecvSkillSingleTarget;
		//(*(DWORD*)0x08B64334) = (DWORD)&RecvSkillAreaTarget;
		(*(DWORD*)0x08B64338) = (DWORD)&SocketSend;
		(*(DWORD*)0x08B6433C) = (DWORD)&OnLoginAccount;
		(*(DWORD*)0x08B64340) = (DWORD)&OnLoginSuccess;
		(*(DWORD*)0x08B64344) = (DWORD)&CreateItemMemoryTable;
		(*(DWORD*)0x08B64348) = (DWORD)&LoadAreaStages;
		(*(DWORD*)0x08B6434C) = (DWORD)&OnCrystalUse;
		(*(DWORD*)0x08B64350) = (DWORD)&OnSetItemPerfect;
		(*(DWORD*)0x08B64354) = (DWORD)&CreateUnitEnemyData;
		(*(DWORD*)0x08B64358) = (DWORD)&OnLogWrite;
		(*(DWORD*)0x08B6435C) = (DWORD)&ReadUnitNpcData;
		(*(DWORD*)0x08B64360) = (DWORD)&ReadUnitSpawnData;
		(*(DWORD*)0x08B64364) = (DWORD)&ReadUnitEnemyStage;
		(*(DWORD*)0x08B64368) = (DWORD)&ReadUnitNpcStand;
		(*(DWORD*)0x08B6436C) = (DWORD)&SendBossMessage;
		(*(DWORD*)0x08B64370) = (DWORD)&SetSpawnUnitData;
		(*(DWORD*)0x08B64374) = (DWORD)&InitSocketServer;
		(*(DWORD*)0x08B64378) = (DWORD)&OnItemPremium;
		(*(DWORD*)0x08B6437C) = (DWORD)&OnUnitNpcClick;
		(*(DWORD*)0x08B64380) = (DWORD)&RecvBuffSkill;
		(*(DWORD*)0x08B64384) = (DWORD)&OnReadPostBox;
		(*(DWORD*)0x08B64388) = (DWORD)&OnDeletePostBox;
		(*(DWORD*)0x08B6438C) = (DWORD)&SendAttackRatting;
		(*(DWORD*)0x08B64390) = (DWORD)&InitServer;
		(*(DWORD*)0x08B64394) = (DWORD)&UseDistortionMonster;
		(*(DWORD*)0x08B64398) = (DWORD)&OnLogItem;
		(*(DWORD*)0x08B6439C) = (DWORD)&GetAgingType;
		(*(DWORD*)0x08B643A0) = (DWORD)&SetupMonsterBellatra;
		(*(DWORD*)0x08B643A4) = (DWORD)&SendDebugChat;
		(*(DWORD*)0x08B643A8) = (DWORD)&SendMixDescription;
		(*(DWORD*)0x08B643B4) = (DWORD)&OnOutCharacter;
		(*(DWORD*)0x08B643B8) = (DWORD)&DeallocMemoryPT;
		(*(DWORD*)0x08B643BC) = (DWORD)&OnSendExp;
		(*(DWORD*)0x08B643C0) = (DWORD)&Dice::RandomI;
		(*(DWORD*)0x08B643C4) = (DWORD)&ReadUnitAnimation;
		(*(DWORD*)0x08B643C8) = (DWORD)&ReadModelSize;
		(*(DWORD*)0x08B643CC) = (DWORD)&OnSetDrop;
		(*(DWORD*)0x08B643D0) = (DWORD)&CheatLog;
		(*(DWORD*)0x08B643D4) = (DWORD)&OnHandlerFunctions;
		(*(DWORD*)0x08B643D8) = (DWORD)&SendChatServer;
		(*(DWORD*)0x08B643DC) = (DWORD)&OnLoadCharacterInventory;
		(*(DWORD*)0x08B643E0) = (DWORD)&OnDisconnectUser;
		(*(DWORD*)0x08B643E4) = (DWORD)&InventoryServerCheck;
		(*(DWORD*)0x08B643E8) = (DWORD)&SendChatTrade;
		(*(DWORD*)0x08B643EC) = (DWORD)&FindItemWarehouse;
		(*(DWORD*)0x08B643F0) = (DWORD)&AddItemWarehouse;
		(*(DWORD*)0x08B643F4) = (DWORD)&DeleteItemWarehouse;
		(*(DWORD*)0x08B643F8) = (DWORD)&OnSetTargetPosition;
		(*(DWORD*)0x08B643FC) = (DWORD)&OnDisconnectUserEx;
		(*(DWORD*)0x08B64400) = (DWORD)&OnSaveItemData;
		(*(DWORD*)0x08B64404) = (DWORD)&OnLoadItemData;
		(*(DWORD*)0x08B64408) = (DWORD)&RecordPoint;
		(*(DWORD*)0x08B6440C) = (DWORD)&AddUserOnline;
		(*(DWORD*)0x08B64410) = (DWORD)&USERSERVER->UsersUpdate;
		(*(DWORD*)0x08B64414) = (DWORD)&SendItemNearUsers;  //SendStgItemToNearUsers
		(*(DWORD*)0x08B64418) = (DWORD)&SocketAccept;
		(*(DWORD*)0x08B6441C) = (DWORD)&ReadSocketHandler;
		(*(DWORD*)0x08B64420) = (DWORD)&SocketPacket;
		(*(DWORD*)0x08B64424) = (DWORD)&AddMonsterSpawn;
		(*(DWORD*)0x08B64428) = (DWORD)&DelMonsterSpawn;
		(*(DWORD*)0x08B6442C) = (DWORD)&OnEvade;
		(*(DWORD*)0x08B64430) = (DWORD)&MapServer::FindItem;
		(*(DWORD*)0x08B64434) = (DWORD)&UnitAutoMain;
		(*(DWORD*)0x08B64438) = (DWORD)&OnSetClassJobItem;
		(*(DWORD*)0x08B6443C) = (DWORD)&UpdateBellatra;
		(*(DWORD*)0x08B64440) = (DWORD)&OnDownAgeHandler;
		(*(DWORD*)0x08B64444) = (DWORD)&OnUpAgeHandler;
		(*(DWORD*)0x08B64448) = (DWORD)&UpdateDamageDataPVP;
		(*(DWORD*)0x08B6444C) = (DWORD)&SendPvPDamage;
		(*(DWORD*)0x08B64450) = (DWORD)&IsFreezedUnitFollow;
		(*(DWORD*)0x08B64454) = (DWORD)&CanHitTargetAreaDamage;
		(*(DWORD*)0x08B64458) = (DWORD)&MakeFaceArea;
		(*(DWORD*)0x08B6445C) = (DWORD)&SetupPolygonArea;
		break;
	}

	return TRUE;
}