#include "stdafx.h"
#include "unitinfo.h"

// Global Vars
UINT								uCurrentStageUnitStand = 0;
UINT								uCurrentStageUnitEnemy = 0;

UnitInfoServer::UnitInfoServer()
{

}

UnitInfoServer::~UnitInfoServer()
{
}

unsigned short read_ushort( char * pBuf )
{
	return (unsigned short)*(unsigned short*)((char*)pBuf);
}

int UnitInfoServer::GetEnemyEffect( char * pszEffect )
{
	for ( auto f : saCharacterSoundEffect )
	{
		if ( STRINGCOMPAREI( f.szCodeName, pszEffect ) )
			return f.iMonsterEffectID;
	}
	return 0;
}

EMonsterType UnitInfoServer::GetMonsterType( const char * szRaw )
{
	if ( STRINGCOMPAREI( szRaw, "undead" ) )
		return MONSTERTYPE_Undead;
	else if ( STRINGCOMPAREI( szRaw, "mutant" ) )
		return MONSTERTYPE_Mutant;
	else if ( STRINGCOMPAREI( szRaw, "demon" ) )
		return MONSTERTYPE_Demon;
	else if ( STRINGCOMPAREI( szRaw, "neutral" ) )
		return MONSTERTYPE_Netural;
	else if ( STRINGCOMPAREI( szRaw, "machine" ) )
		return MONSTERTYPE_Normal;
	else if ( STRINGCOMPAREI( szRaw, "normal" ) )
		return MONSTERTYPE_Normal;

	return MONSTERTYPE_Netural;
}

const std::string UnitInfoServer::GetMonsterTypeStr( EMonsterType eMonsterType )
{
	switch ( eMonsterType )
	{
		case EMonsterType::MONSTERTYPE_Undead:	return "Undead";
		case EMonsterType::MONSTERTYPE_Mutant:	return "Mutant";
		case EMonsterType::MONSTERTYPE_Demon:	return "Demon";
		case EMonsterType::MONSTERTYPE_Netural: return "Neutral";
		case EMonsterType::MONSTERTYPE_Normal:  return "Machine";
		case EMonsterType::MONSTERTYPE_Summon:  return "Summon";
	}

	return "Netural";
}

BOOL UnitInfoServer::EnemyDataItem( UnitInfo * pMonsterInfo, const char * pszItems, int iChance, int iGoldMin, int iGoldMax )
{
	return FALSE;

	/*
	if ( pMonsterInfo->iSpare2 == 200 )
		return FALSE;

	// Gold or Air
	{
		if ( STRINGCOMPAREI( pszItems, "Gold" ) )
		{
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].iItemID = ITEMID_Gold;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].iPercent    = iChance;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].sGoldMin    = iGoldMin;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].sGoldMax	  = iGoldMax;
			pMonsterInfo->iDropPercentNum += iChance;
			pMonsterInfo->iSpare2++;
			return TRUE;
		}
		else if ( STRINGCOMPAREI( pszItems, "Air" ) )
		{
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].iItemID = 0;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].iPercent	  = iChance;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].sGoldMin    = iGoldMin;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].sGoldMax    = iGoldMax;
			pMonsterInfo->iDropPercentNum += iChance;
			pMonsterInfo->iSpare2++;
			return TRUE;
		}
	}


	std::vector<std::string> vItems = split( pszItems, ' ' );

	UINT uVecSize = vItems.size() + pMonsterInfo->iSpare2;
	if ( uVecSize > 200 )
		uVecSize = 200;

	int iPosition = 0;

	pMonsterInfo->iDropPercentNum += iChance;

	UINT uTotalItem = uVecSize - pMonsterInfo->iSpare2;

	for ( UINT u = pMonsterInfo->iSpare2; u < uVecSize; u++ )
	{
		ItemData * pItem = FindItemPointerTable( vItems[ iPosition++ ].c_str() );
		if ( pItem )
		{
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].iItemID = pItem->sBaseItemID.ToItemID();
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].iPercent	  = iChance / uTotalItem;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].sGoldMin    = iGoldMin;
			pMonsterInfo->saDropData[ pMonsterInfo->iSpare2 ].sGoldMax    = iGoldMax;
			pMonsterInfo->iSpare2++;
			if ( pMonsterInfo->iSpare2 == 200 )
				return FALSE;
		}
	}
	return TRUE;*/
}

UINT UnitInfoServer::GetTotalUnitEnemy()
{
	return ( *( UINT* )0x07AC57E8 );
}

CharacterData * UnitInfoServer::GetEnemyCharInfoPtr()
{
	return ( *( CharacterData** )0x07AB3050 );
}

UnitInfo * UnitInfoServer::GetEnemyDataInfoPtr()
{
	return ( *( UnitInfo** )0x07AB22A4 );
}



CharacterData * UnitInfoServer::FindEnemyCharInfo( const char * pszName )
{
	CharacterData * pEnemyCharInfo = GetEnemyCharInfoPtr();
	UINT uEnemyCount = GetTotalUnitEnemy();

	for ( UINT u = 0; u < uEnemyCount; u++ )
	{
		if ( STRINGCOMPAREI( pEnemyCharInfo[ u ].szName, pszName ) )
			return &pEnemyCharInfo[ u ];
	}

	return NULL;
}

UnitInfo * UnitInfoServer::GetUnitInfoByMonsterID( int iID )
{
	UnitInfo * psaUnitInfo = *(UnitInfo**)0x07AB22A4;
	UINT uEnemyCount = GetTotalUnitEnemy();

	for ( UINT u = 0; u < uEnemyCount; u++ )
	{
		if ( psaUnitInfo[u].iUniqueMonsterID == iID )
		{
			return &psaUnitInfo[u];
		}
	}

	return NULL;
}

CharacterData * UnitInfoServer::GetCharacterDataByMonsterID( int iID )
{
	UnitInfo * psaUnitInfo = *(UnitInfo**)0x07AB22A4;
	UINT uEnemyCount = GetTotalUnitEnemy();

	for ( UINT u = 0; u < uEnemyCount; u++ )
	{
		if ( psaUnitInfo[u].iUniqueMonsterID == iID )
		{
			CharacterData * pEnemyCharInfo = GetEnemyCharInfoPtr();
			return &pEnemyCharInfo[u];
		}
	}

	return NULL;
}

int UnitInfoServer::GetModelID( const char * pszModelFile )
{
	int iID = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 9 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM ModelList WHERE Model=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszModelFile );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				if ( !pcDB->GetData( 1, PARAMTYPE_Integer, &iID ) )
					LOGERROR( "%s- %d", pszModelFile, iID );
			}
		}
		pcDB->Close();
	}
	return iID;
}

int UnitInfoServer::GetModelDeathID( const char * pszModelFile )
{
	int iID = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 10 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DeathModelID FROM ModelList WHERE Model=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszModelFile );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
			}
		}
		pcDB->Close();
	}

	return iID;
}

UINT UnitInfoServer::ReadModelSize( const char * pszModelFile )
{
	int iID = 0;
	// Is in SQL Data?
	if ( iID = GetModelID( pszModelFile ) )
	{
		int iWidth = 0;
		int iHeight = 0;
		UINT uModelPat = 0;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 11 );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT Width, Height FROM ModelList WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iWidth );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iHeight );
				}
			}
			pcDB->Close();
		}

		typedef UINT( __cdecl *t_newPT )( UINT Size );
		t_newPT newPT = ( t_newPT )0x005C6F40;
		uModelPat = newPT( 1252 );
		if ( uModelPat )
		{
			ZeroMemory( (void*)uModelPat, 1252 );
			*( int* )( uModelPat + 0x294 ) = iWidth;
			*( int* )( uModelPat + 0x298 ) = iHeight;
			*( int* )( uModelPat + 0x284 ) = newPT( 1252 ); // TmPattern new smPat3D ( death model work )
			return uModelPat;
		}
	}
	return NULL;
}

BOOL UnitInfoServer::ReadUnitAnimation( char * pszModelFile, ModelDataOld * pModelData )
{
	ModelDataOld sModelData;
	ZeroMemory( &sModelData, sizeof( ModelDataOld ) );

	sModelData.iTalkNumModelAnimation = 10;
	sModelData.iNumModelAnimation = 10;

	int iID = 0;

	// Is in SQL Data?
	if ( iID = GetModelID( pszModelFile ) )
	{
		int iCountAnimation = 0;
		int iType			= 0;
		int iBeginFrame		= 0;
		int iEndFrame		= 0;

		// Model File ( Pattern )
		STRINGCOPY( sModelData.szModelPath, pszModelFile );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 12 );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT COUNT(*) FROM ModelAnimationList WHERE ModelID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iCountAnimation );
				}
			}
			pcDB->Close();
		}

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM ModelAnimationList WHERE ModelID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					for ( int i = 10; i < (iCountAnimation+10); i++ )
					{
						pcDB->GetData( 3, PARAMTYPE_Integer, &iType );
						pcDB->GetData( 4, PARAMTYPE_Integer, &iBeginFrame );
						pcDB->GetData( 5, PARAMTYPE_Integer, &iEndFrame );
						sModelData.saModelAnimation[ i ].iType = (EAnimationType)iType;
						sModelData.saModelAnimation[ i ].iBeginFrame	= iBeginFrame;
						sModelData.saModelAnimation[ i ].iEndFrame		= iEndFrame;

						// Control Frames
						if ( sModelData.saModelAnimation[ i ].iType == EAnimationType::ANIMATIONTYPE_Running )
							sModelData.saModelAnimation[ i ].bLoop = TRUE;

		//				if ( sModelData.saModelAnimation[ i ].iType == ANIMATIONTYPE_Die )
		//					sModelData.saModelAnimation[ i ].iEndFrame -= 8;

						else if ( sModelData.saModelAnimation[ i ].iType == EAnimationType::ANIMATIONTYPE_Idle )
							sModelData.saModelAnimation[ i ].bLoop = TRUE;

						else if ( sModelData.saModelAnimation[ i ].iType == EAnimationType::ANIMATIONTYPE_Walking )
							sModelData.saModelAnimation[ i ].bLoop = TRUE;

						pcDB->Fetch();
					}
				}
			}
			pcDB->Close();
		}

		sModelData.iNumModelAnimation += ( iCountAnimation );

		// Death Model
		int iDeathModelID = 0;
		if ( iDeathModelID = GetModelDeathID( pszModelFile ) )
		{
			int iType = 0;
			int iBeginFrame = 0;
			int iEndFrame = 0;

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 13 );

			// Model Death File ( Pattern 2 )
			char szModelDeathFile[ 64 ] = { 0 };

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT Model FROM ModelList WHERE ID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iDeathModelID );
					if ( pcDB->Execute() && pcDB->Fetch() )
						pcDB->GetData( 1, PARAMTYPE_String, szModelDeathFile, 64 );
				}
				pcDB->Close();
			}

			STRINGCOPY( sModelData.szDeathModelDataFilePath, szModelDeathFile );
		}

		memcpy( pModelData, &sModelData, sizeof( ModelDataOld ) );

		return TRUE;
	}
	return FALSE;
}

int UnitInfoServer::ReadInx( char * pszFilePath )
{
//	if ( pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'x' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'X' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'i' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'I' )
//		return 0;

	//keep a filepath with INI
	char szTurd[ 256 ] = { 0 };
	char szTurd2[ 256 ] = { 0 };
	STRINGCOPY( szTurd, pszFilePath );
	STRINGCOPY( szTurd2, pszFilePath );
	szTurd[ lstrlenA( szTurd ) - 1 ] = 'I';
	szTurd2[ lstrlenA( szTurd2 ) - 1 ] = 'X';

	if ( GetFileSize( szTurd2 ) <= 67084 )
		return ReadInxOld( szTurd2 );

	// Lower case
	for ( int i = 0; i < lstrlenA( szTurd ); i++ )
		szTurd[ i ] = tolower( szTurd[ i ] );

	int iID = 0;

	if ( ( iID = GetModelID( szTurd ) ) != 0 )
		return iID;


	FILE * file;
	if ( fopen_s( &file, szTurd2, "rb" ) == 0 )
	{
		ModelData tmp;
		fread( &tmp, sizeof( tmp ), 1, file );
		fclose( file );

		int iDeathModelID = 0;

		//Has Death Model?
		if ( tmp.szDeathModelDataFilePath[ 0 ] != 0 )
			iDeathModelID = ReadInx( tmp.szDeathModelDataFilePath );

		//Obtain Width & Height
		DWORD dwWidth = 0, dwHeight = 0;

		int iLen = lstrlenA( tmp.szModelPath );
		tmp.szModelPath[ iLen - 3 ] = 's';
		tmp.szModelPath[ iLen - 2 ] = 'm';
		tmp.szModelPath[ iLen - 1 ] = 'd';

		if ( fopen_s( &file, tmp.szModelPath, "rb" ) == 0 )
		{
			int iFileLength;
			fseek( file, 0, SEEK_END );
			iFileLength = ftell( file );
			rewind( file );

			if ( iFileLength > 0 )
			{
				char * buffer = new char[ iFileLength ];
				fread( buffer, 1, iFileLength, file );

				for ( int i = 0; i < ( iFileLength - 2 ); i++ )
				{
					char d = buffer[ i ];
					char c = buffer[ i + 1 ];
					char b = buffer[ i + 2 ];

					if ( d == 'D' && c == 'C' && b == 'B' )
					{
						i += 44;

						DWORD * dwPtr = ( DWORD* )&buffer[ i ];

						dwWidth = *dwPtr;

						dwPtr += 2;

						dwHeight = *dwPtr;
						break;
					}
				}

				delete[] buffer;
			}

			fclose( file );
		}

		//Obtain Frame Info
		struct FrameInfo
		{
			int iStartFrame;
			int iEndFrame;
			int iPosition;
			int iCount;
		};

		struct FileHeader
		{
			char szHeader[ 24 ];
			int iObjectCount;
			int iMaterialCount;
			int iMaterialPosition;
			int iFirstObjectPosition;
			int iFrameCounter;
			FrameInfo saFrameInfo[ 32 ];
		};

		//Got overlaying Model Animation file?
		if ( tmp.szModelDataAnimationFile[0] )
		{
			tmp.szModelDataAnimationFile[ lstrlenA( tmp.szModelDataAnimationFile ) ] = 'x';

			if ( fopen_s( &file, tmp.szModelDataAnimationFile, "rb" ) == 0 )
			{
				fread( &tmp, sizeof( tmp ), 1, file );
				fclose( file );
			}
		}

		FileHeader sFileHeader;

		iLen = lstrlenA( tmp.szBoneModelFilePath );
		tmp.szBoneModelFilePath[ iLen - 3 ] = 's';
		tmp.szBoneModelFilePath[ iLen - 2 ] = 'm';
		tmp.szBoneModelFilePath[ iLen - 1 ] = 'b';

		if ( fopen_s( &file, tmp.szBoneModelFilePath, "rb" ) == 0 )
		{
			fread( &sFileHeader, sizeof( FileHeader ), 1, file );
			fclose( file );
		}

		// Insert model data in SQL
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 14 );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ModelList([Model],[Width],[Height],[DeathModelID]) VALUES(?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szTurd );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &dwWidth );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &dwHeight );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iDeathModelID );

					pcDB->Execute();
				}
				pcDB->Close();
			}

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT TOP 1 ID FROM ModelList ORDER BY ID DESC" ) )
				{
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					}
				}
				pcDB->Close();
			}

			for ( int i = 10; i < tmp.iNumModelAnimation; i++ )
			{
				ModelAnimation * p = tmp.saModelAnimation + i;

				if ( p->iType != NULL )
				{
					//Decode Frames
					char caBeginFrame[2], caEndFrame[2];
					caBeginFrame[0] = *(char*)((DWORD)p + 4);
					caBeginFrame[1] = *(char*)((DWORD)p + 6);
					p->iBeginFrame = read_ushort( caBeginFrame );

					caEndFrame[0] = *(char*)((DWORD)p + 16);
					caEndFrame[1] = *(char*)((DWORD)p + 18);
					p->iEndFrame = read_ushort( caEndFrame );
					if ( ( p->iSubFileFrameNumber - 1 ) < sFileHeader.iFrameCounter )
					{
						int iFrameAppend = sFileHeader.saFrameInfo[ p->iSubFileFrameNumber - 1 ].iStartFrame;
						iFrameAppend /= 160;

						p->iBeginFrame += iFrameAppend;
						p->iEndFrame += iFrameAppend;
					}


					if ( pcDB->Open() )
					{
						if ( pcDB->Prepare( "INSERT INTO ModelAnimationList([ModelID],[Type],[FrameBegin],[FrameEnd],[FrameEvent1],[FrameEvent2],[FrameEvent3],[FrameEvent4]) VALUES(?,?,?,?,?,?,?,?)" ) )
						{
							float iEventFrame1 = p->iaEventFrame[0] > 0 ? p->iBeginFrame + (p->iaEventFrame[0] / 160.0f) : 0;
							float iEventFrame2 = p->iaEventFrame[1] > 0 ? p->iBeginFrame + (p->iaEventFrame[1] / 160.0f) : 0;
							float iEventFrame3 = p->iaEventFrame[2] > 0 ? p->iBeginFrame + (p->iaEventFrame[2] / 160.0f) : 0;
							float iEventFrame4 = p->iaEventFrame[3] > 0 ? p->iBeginFrame + (p->iaEventFrame[3] / 160.0f) : 0;

							pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
							pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &p->iType );
							pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &p->iBeginFrame );
							pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &p->iEndFrame );

							pcDB->BindParameterInput(5, PARAMTYPE_Float, &iEventFrame1);
							pcDB->BindParameterInput(6, PARAMTYPE_Float, &iEventFrame2);
							pcDB->BindParameterInput(7, PARAMTYPE_Float, &iEventFrame3);
							pcDB->BindParameterInput(8, PARAMTYPE_Float, &iEventFrame4);

							pcDB->Execute();
						}


						pcDB->Close();
					}
				}
			}
		}
	}
	return iID;
}

int UnitInfoServer::ReadInxOld( char * pszFilePath )
{
//	if ( pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'x' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'X' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'i' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'I' )
//		return 0;

	//keep a filepath with INI
	char szTurd[ 256 ] = { 0 };
	char szTurd2[ 256 ] = { 0 };
	STRINGCOPY( szTurd, pszFilePath );
	STRINGCOPY( szTurd2, pszFilePath );
	szTurd[ lstrlenA( szTurd ) - 1 ] = 'I';
	szTurd2[ lstrlenA( szTurd2 ) - 1 ] = 'X';

	// Lower case
	for ( int i = 0; i < lstrlenA( szTurd ); i++ )
		szTurd[ i ] = tolower( szTurd[ i ] );

	int iID = 0;

	if ( ( iID = GetModelID( szTurd ) ) != 0 )
		return iID;


	FILE * file;
	if ( fopen_s( &file, szTurd2, "rb" ) == 0 )
	{
		ModelDataOld tmp;
		fread( &tmp, sizeof( tmp ), 1, file );
		fclose( file );

		int iDeathModelID = 0;

		//Has Death Model?
		if ( tmp.szDeathModelDataFilePath[ 0 ] != 0 )
			iDeathModelID = ReadInx( tmp.szDeathModelDataFilePath );

		//Obtain Width & Height
		DWORD dwWidth = 0, dwHeight = 0;

		int iLen = lstrlenA( tmp.szModelPath );
		tmp.szModelPath[ iLen - 3 ] = 's';
		tmp.szModelPath[ iLen - 2 ] = 'm';
		tmp.szModelPath[ iLen - 1 ] = 'd';

		if ( fopen_s( &file, tmp.szModelPath, "rb" ) == 0 )
		{
			int iFileLength;
			fseek( file, 0, SEEK_END );
			iFileLength = ftell( file );
			rewind( file );

			if ( iFileLength > 0 )
			{
				char * buffer = new char[ iFileLength ];
				fread( buffer, 1, iFileLength, file );

				for ( int i = 0; i < ( iFileLength - 2 ); i++ )
				{
					char d = buffer[ i ];
					char c = buffer[ i + 1 ];
					char b = buffer[ i + 2 ];

					if ( d == 'D' && c == 'C' && b == 'B' )
					{
						i += 44;

						DWORD * dwPtr = ( DWORD* )&buffer[ i ];

						dwWidth = *dwPtr;

						dwPtr += 2;

						dwHeight = *dwPtr;
						break;
					}
				}

				delete[] buffer;
			}

			fclose( file );
		}

		//Obtain Frame Info
		struct FrameInfo
		{
			int iStartFrame;
			int iEndFrame;
			int iPosition;
			int iCount;
		};

		struct FileHeader
		{
			char szHeader[ 24 ];
			int iObjectCount;
			int iMaterialCount;
			int iMaterialPosition;
			int iFirstObjectPosition;
			int iFrameCounter;
			FrameInfo saFrameInfo[ 32 ];
		};

		//Got overlaying Model Animation file?
		if ( tmp.szModelDataAnimationFile[ 0 ] )
		{
			tmp.szModelDataAnimationFile[ lstrlenA( tmp.szModelDataAnimationFile ) ] = 'x';

			if ( fopen_s( &file, tmp.szModelDataAnimationFile, "rb" ) == 0 )
			{
				fread( &tmp, sizeof( tmp ), 1, file );
				fclose( file );
			}
		}

		FileHeader sFileHeader;

		iLen = lstrlenA( tmp.szBoneModelFilePath );
		tmp.szBoneModelFilePath[ iLen - 3 ] = 's';
		tmp.szBoneModelFilePath[ iLen - 2 ] = 'm';
		tmp.szBoneModelFilePath[ iLen - 1 ] = 'b';

		if ( fopen_s( &file, tmp.szBoneModelFilePath, "rb" ) == 0 )
		{
			fread( &sFileHeader, sizeof( FileHeader ), 1, file );
			fclose( file );
		}

		// Insert model data in SQL
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 15 );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ModelList([Model],[Width],[Height],[DeathModelID]) VALUES(?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szTurd );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &dwWidth );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &dwHeight );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iDeathModelID );
					pcDB->Execute();
				}
				pcDB->Close();
			}

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT TOP 1 ID FROM ModelList ORDER BY ID DESC" ) )
				{
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					}
				}
				pcDB->Close();
			}

			for ( int i = 10; i < tmp.iNumModelAnimation; i++ )
			{
				ModelAnimationOld * p = tmp.saModelAnimation + i;

				if ( p->iType != NULL )
				{
					//Decode Frames
					char caBeginFrame[2], caEndFrame[2];
					caBeginFrame[0] = *(char*)((DWORD)p + 4);
					caBeginFrame[1] = *(char*)((DWORD)p + 6);
					p->iBeginFrame = read_ushort( caBeginFrame );

					caEndFrame[0] = *(char*)((DWORD)p + 16);
					caEndFrame[1] = *(char*)((DWORD)p + 18);
					p->iEndFrame = read_ushort( caEndFrame );
					if ( ( p->iSubFileFrameNumber - 1 ) < sFileHeader.iFrameCounter )
					{
						int iFrameAppend = sFileHeader.saFrameInfo[ p->iSubFileFrameNumber - 1 ].iStartFrame;
						iFrameAppend /= 160;

						p->iBeginFrame += iFrameAppend;
						p->iEndFrame += iFrameAppend;
					}


					if ( pcDB->Open() )
					{
						if ( pcDB->Prepare( "INSERT INTO ModelAnimationList([ModelID],[Type],[FrameBegin],[FrameEnd]) VALUES(?,?,?,?)" ) )
						{
							pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
							pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &p->iType );
							pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &p->iBeginFrame );
							pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &p->iEndFrame );

							pcDB->Execute();
						}
						pcDB->Close();
					}
				}
			}
		}
	}
	return iID;
}

void UnitInfoServer::CreateUnitEnemyData ()
{

	INFO ( "Reading in items" );
	ITEMSERVER->CreateItemMemoryTable();

	INFO ( "Reading in monster stats and data" );

	int iMonCount = 0;


//	if ( !GAME_SERVER )
//		return;

	//	if ( !GAME_SERVER )
	//		return;

		// Clear pcUnitData Memory Data
	ZeroMemory ( (void *)*(CharacterData **)0x07AB3050, sizeof ( CharacterData ) * MONSTER_SERVER_MAX );
	ZeroMemory ( (void *)*(UnitInfo **)0x07AB22A4, sizeof ( UnitInfo ) * MONSTER_SERVER_MAX );

	// Get Total Monsters
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_GameDB_Monsters, 16 );

	CharacterData * pMonsterCharInfo = *( CharacterData** )0x07AB3050;
	UnitInfo * pMonsterInfo  = *( UnitInfo** )0x07AB22A4;

	std::vector<int> vMonsterIds;

	// Select Data Monster
	if ( pcDB->Open () )
	{
		if ( pcDB->Prepare( "SELECT [Name],[ModelFile],[Level],[Glow],[Size],[CameraY],[CameraZ],[MonsterID],[DropIsPublic],[DropQuantity],[SpawnTime],[SpawnMin],[SpawnMax],[Inteligence],[MonsterType],[ViewSight],[HealthPoint],[EXP],[SpecialSkillType],[SpecialSkillHit],[SpecialHitRate],[SpecialHitScope],[SpecialHitPowerMin],[SpecialHitPowerMax],[ATKPowMin],[ATKPowMax],[Absorb],[Stunchance],[Block],[Defense],[AttackSpeed],[AttackRating],[AttackRange],[PerfectAttackRate],[SizeShadow],[Organic],[Lightning],[Ice],[Fire],[Poison],[Magic],[PropertyMon],[MoveSpeed],[PotionPercent],[Potions],[Effect],[QuestItemDrop],[QuestID],[QuestMap] FROM MonsterList" ) )
		{
			if ( pcDB->Execute() )
			{
				int col;
				int hp;

				while ( pcDB->Fetch ())
				{
				    col = 1; //column num

					pMonsterInfo[iMonCount].iMovementRange = 64 * 256;

					// Name
					pcDB->GetData( col++, PARAMTYPE_String, pMonsterCharInfo[iMonCount].szName, 32 );
					STRINGCOPY( pMonsterInfo[iMonCount].szName, pMonsterCharInfo[iMonCount].szName );

					if ( lstrcmpiA( pMonsterCharInfo[iMonCount].szName, "Zombie" ) == 0 )
						*(CharacterData**)0x07AC9D5C = &pMonsterCharInfo[iMonCount];

					// Model
					pcDB->GetData( col++, PARAMTYPE_String, pMonsterCharInfo[iMonCount].Monster.szBodyModel, 64 );

					// State
					pMonsterCharInfo[iMonCount].iType = CHARACTERTYPE_Monster;

					// Level
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iLevel );

					// Glow
					short iMonsterClass;
					pcDB->GetData( col++, PARAMTYPE_Short, &iMonsterClass );
					pMonsterCharInfo[iMonCount].sMonsterClass = (EMonsterClass)iMonsterClass;

					// Size
					float fSize = 0.0f;
					pcDB->GetData( col++, PARAMTYPE_Float, &fSize );
					pMonsterCharInfo[iMonCount].sSize = (short)(fSize * 256.0f);

					// Camera Y
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sViewBoxZoom.sMin );
					// Camera Z
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sViewBoxZoom.sMax );

					// pcUnitData ID
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iUniqueMonsterID );

					pMonsterCharInfo[iMonCount].iUniqueMonsterID = pMonsterInfo[iMonCount].iUniqueMonsterID;


					if ( GAME_SERVER )
					{
						if ( pMonsterCharInfo[iMonCount].iUniqueMonsterID == 0 )
						{
							WARN ( "MonsterId is 0 for monster %s!", pMonsterCharInfo[iMonCount].szName );
						}
						else
						{
							if ( std::find ( vMonsterIds.begin (), vMonsterIds.end (), pMonsterCharInfo[iMonCount].iUniqueMonsterID ) != vMonsterIds.end () )
							{
								LOGERROR ( "Monster Id %d is not unique!", pMonsterCharInfo[iMonCount].iUniqueMonsterID );
							}
							else
							{
								vMonsterIds.push_back ( pMonsterCharInfo[iMonCount].iUniqueMonsterID );
							}
						}
					}

					//DropIsPublic
					pcDB->GetData ( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].bPublicDrop );

					//DropQuantity
					pcDB->GetData ( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iNumDrops );

					// Spawn Time
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iActiveHour );

					// Spawn pcUnitData Min
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iGroupLimitMin );

					// Spawn pcUnitData Max
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iGroupLimitMax );

					// IQ
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iIntelligence );

					// Nature
					char szNatureName[32] = { 0 };
					pcDB->GetData( col++, PARAMTYPE_String, szNatureName, 32 );
					{
					pMonsterInfo[iMonCount].iNature = MONSTERNATURE_Neutral;

					if ( STRINGCOMPAREI( szNatureName, "good" ) )
						pMonsterInfo[iMonCount].iNature = MONSTERNATURE_Good;
					else if ( STRINGCOMPAREI( szNatureName, "evil" ) )
						pMonsterInfo[iMonCount].iNature = MONSTERNATURE_Evil;
					}

					// Undead (?)
					pMonsterInfo[iMonCount].iUndead = 1;

					// View Sight
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iSight );
					pMonsterCharInfo[iMonCount].iSight = pMonsterInfo[iMonCount].iSight;
					pMonsterCharInfo[iMonCount].iSight *= pMonsterCharInfo[iMonCount].iSight;


					pMonsterCharInfo[iMonCount].sHP.sCur = MONSTER_HP_OVERRIDE;
					pMonsterCharInfo[iMonCount].sHP.sMax = MONSTER_HP_OVERRIDE;


					// Health Point
					pcDB->GetData( col++, PARAMTYPE_Integer, &hp );

					UNITSERVER->AddOrUpdateMonsterHealthPointDefinition(pMonsterCharInfo[iMonCount].iUniqueMonsterID, hp);

					// EXP
					INT64 iExp = 0;
					pcDB->GetData( col++, PARAMTYPE_Int64, &iExp );
					if ( RATE_EXP > 0 )
					iExp = iExp * RATE_EXP;

					pMonsterInfo[iMonCount].iExp = (int)iExp;
					typedef void( __cdecl *t_SetExpUnitFunc )(CharacterData * pMonster, INT64 iExp);
					t_SetExpUnitFunc SetExpUnitFunc = (t_SetExpUnitFunc)0x08B815CC;
					SetExpUnitFunc( &pMonsterCharInfo[iMonCount], iExp );

					// Special Skill Curse
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iSkillType );

					// Special Skill Distance
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iSkillPierceRange );

					// Special Skill Rating
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iSkillChance );

					// Special Skill Range
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iSkillArea );

					// Special Skill Damage Min
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterInfo[iMonCount].sSkillPower.sMin );

					// Special Skill Damage Max
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterInfo[iMonCount].sSkillPower.sMax );

					// Damage Min
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iMinDamage );

					// Damage Max
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iMaxDamage );

					// Absorption
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iAbsorbRating );

					// Stun Chance ( 0% ~ 100% )
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iFlinchChance);

					// Block
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iBlockRating);

					// Defense
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iDefenseRating);

					// Attack Speed
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iAttackSpeed);
					pMonsterCharInfo[iMonCount].iAttackSpeed *= 256;

					// Attack Rating
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iAttackRating);

					// Shooting Range
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iAttackRange);
					pMonsterCharInfo[iMonCount].iAttackRange *= 256;

					// Attack Percent
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iLureDistance);

					// Size Shadow
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iShadowSize);

					// Organic
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sElementalDef[EElementID::Organic]);

					// Lightning
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sElementalDef[EElementID::Lightning]);

					// Ice
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sElementalDef[EElementID::Ice]);

					// Fire
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sElementalDef[EElementID::Fire]);

					// Poison
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sElementalDef[EElementID::Poison]);

					// Magic
					pcDB->GetData( col++, PARAMTYPE_Short, &pMonsterCharInfo[iMonCount].sElementalDef[EElementID::Wind]);


					// Property Monster
					char szPropertyName[32] = { 0 };
					pcDB->GetData( col++, PARAMTYPE_String, szPropertyName, 32 );
					{
						pMonsterCharInfo[iMonCount].iMonsterType = GetMonsterType( szPropertyName );
					}

					// Move Speed
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterCharInfo[iMonCount].iMovementSpeed);
					pMonsterCharInfo[iMonCount].iMovementSpeed = (int)(((float)pMonsterCharInfo[iMonCount].iMovementSpeed - 9) * 16) + 256;

					// Percent Potion
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iPerPotions);

					// Potion Count
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].iMaxPotions);

					// Sound Effect
					char szEffectName[64] = { 0 };
					pcDB->GetData( col++, PARAMTYPE_String, szEffectName, 64 );
					pMonsterCharInfo[iMonCount].iMonsterEffectID = (EMonsterEffectID)GetEnemyEffect( szEffectName );

					// Quest Item
					char szQuestItem[10] = { 0 };
					pcDB->GetData( col++, PARAMTYPE_String, szQuestItem, 10 );
					ItemData * pItem = ITEMSERVER->FindItemPointerTable( szQuestItem );
					if ( pItem )
					pMonsterInfo[iMonCount].iQuestItemID = (EItemID)pItem->sBaseItemID.ToItemID();

					// Quest ID
					int iSpecialType;

					pcDB->GetData( col++, PARAMTYPE_Integer, &iSpecialType);

					pMonsterInfo[iMonCount].eSpecialType = (ESpecialUnitType)iSpecialType;

					// Quest Map
					pcDB->GetData( col++, PARAMTYPE_Integer, &pMonsterInfo[iMonCount].eMapID);

					pMonsterCharInfo[iMonCount].psUnitInfo = (UnitInfo*)&pMonsterInfo[iMonCount];

					iMonCount++;
				}
			}
		}
		pcDB->Close();
	}
	( *( UINT* )0x07AC57E8 ) = iMonCount;

	// Old Code

	int LoopVars = 0;
	int LoopMob1 = 0;
	int LoopMob2 = 0;
	int CountMob = UNITINFODATA->GetTotalUnitEnemy ();

	int Mob1P = *( DWORD* )0x07AB3050;
	int Mob2P = *( DWORD* )0x07AB22A4;

	while ( LoopVars < CountMob )
	{
		if ( *( BYTE* )( Mob1P + LoopMob1 + 0x20 ) > 0 )
		{
			char szModelFile[ 64 ] = { 0 };
			STRINGCOPY( szModelFile, ( char* )( Mob1P + LoopMob1 + 0x20 ) );
			ReadInx( szModelFile );
			typedef void( __cdecl *t_ClassMob ) ( DWORD v );
			t_ClassMob LoadOftenMeshPattern = ( t_ClassMob )0x00442860;
			LoadOftenMeshPattern( Mob1P + LoopMob1 + 0x20 );
		}

		int LoopSide = 0x08B76514;

		while ( LoopSide < 0x08B76730 )
		{
			if ( lstrcmpA( ( char* )( Mob1P + LoopMob1 ), ( char* )*( UINT* )( LoopSide - 4 ) ) == 0 )
				*( DWORD* )LoopSide = ( Mob1P + LoopMob1 );
			LoopSide += 12;
		}

		LoopSide = 0x08B76610;

		while ( LoopSide < 0x08B7670C )
		{
			if ( lstrcmpA( ( char* )( Mob1P + LoopMob1 ), ( char* )*( UINT* )( LoopSide - 4 ) ) == 0 )
				*( DWORD* )LoopSide = ( Mob1P + LoopMob1 );
			LoopSide += 12;
		}

		LoopSide = 0x006E2BB4;

		while ( LoopSide < 0x006E2BD8 )
		{
			if ( lstrcmpA( ( char* )( Mob1P + LoopMob1 ), ( char* )*( UINT* )( LoopSide - 4 ) ) == 0 )
				*( DWORD* )LoopSide = ( Mob1P + LoopMob1 );
			LoopSide += 12;
		}

		// Hestian
		if ( lstrcmpA( (char*)(Mob1P + LoopMob1), "Hestian" ) == 0 )
		{
			DWORD dwOld = 0;
			VirtualProtect( (void*)0x005DF8F2, 8, PAGE_EXECUTE_READWRITE, &dwOld );
			*(DWORD*)0x005DF8F4 = (Mob1P + LoopMob1);
		}

		if ( *( WORD* )( Mob1P + LoopMob1 + 0x0180 ) == 0x0C8 )
		{
			if ( *( DWORD* )0x07AC9D04 < 0x10 )
			{
				*( DWORD* )( ( *( DWORD* )0x07AC9D04 * 4 ) + 0x07AC56A8 ) = ( Mob1P + LoopMob1 );
				*( DWORD* )0x07AC9D04 += 1;
			}
		}

		if ( *( BYTE* )( Mob2P + LoopMob2 ) != 0 )
		{
			if ( *( BYTE* )( Mob1P + LoopMob1 ) == 0 )
			{
				STRINGCOPYLEN( ( char* )( Mob1P + LoopMob1 ), 32, ( char* )( Mob2P + LoopMob2 ) );
				*( BYTE* )( Mob2P + LoopMob2 ) = 0;
			}
		}

		LoopMob1 += 0x01D0;
		LoopMob2 += 0x0C60;
		LoopVars += 1;
	}

}

void UnitInfoServer::ReadUnitSpawnData( Map * pcMap, int iStageID)
{
	if ( LOGIN_SERVER )
		return;

	// Get Pointers
	Map::SpawnFlag * pSpawnInfo = pcMap->saSpawnFlag;

	int iCount = 0;

	// Get SQL count
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 17 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(ID) FROM MapSpawnPoint WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iStageID);
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iCount);
		}
		pcDB->Close();
	}

	// Max Spawn is MAX_SPAWN_PER_MAP
	if ( iCount > MAX_SPAWNFLAGS )
		iCount = MAX_SPAWNFLAGS;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM MapSpawnPoint WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iStageID);
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				for ( int i = 0; i < iCount; i++ )
				{
					int iSpawnID = 0;
					char szName[32] = { 0 };
					pSpawnInfo[i].bActive = TRUE;
					pcDB->GetData( 1, PARAMTYPE_Integer, &iSpawnID);
					pcDB->GetData( 3, PARAMTYPE_Integer, &pSpawnInfo[i].iX);
					pcDB->GetData( 4, PARAMTYPE_Integer, &pSpawnInfo[i].iZ);
					pcDB->GetData( 5, PARAMTYPE_String, szName, 32 );
					pcDB->Fetch();

					MAPSERVER->AddMapSpawnData(iStageID, szName, iSpawnID, pSpawnInfo[i].iX, pSpawnInfo[i].iZ );
				}
			}
		}
		pcDB->Close();
	}
	pcMap->iState = 1;
}

void UnitInfoServer::ReadUnitEnemyStage( Map::SpawnSetting * pMonsterStageList )
{
	//Login server only
	if ( LOGIN_SERVER )
		return;

	pMonsterStageList->iSpawnIntervalMin = 127;
	pMonsterStageList->iSpawnMonsters = 3;

	int iCount = 0;

	const int iEnemyCountStage = 12; // Total Enemy in Stage
	const int iBossCountStage = 3;   // Total Boss in Stage

	// Get Count Data in SQL
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 18 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(Stage) FROM MapMonster WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitEnemy );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iCount);
			}
		}
		pcDB->Close();
	}

	// Is Data?
	if ( iCount != 0 )
	{
		// Get Enemies in Stage
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM MapMonster WHERE Stage=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitEnemy );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{

					// Stage Enemy Settings
					{
						pcDB->GetData( 3, PARAMTYPE_Integer, &pMonsterStageList->iSpawnMaxMonsters);
						pcDB->GetData( 4, PARAMTYPE_Integer, &pMonsterStageList->iSpawnIntervalMax);
						pcDB->GetData( 5, PARAMTYPE_Integer, &pMonsterStageList->iSpawnMonsters);
						pMonsterStageList->iSpawnIntervalMax *= 1000;
						pMonsterStageList->iSpawnIntervalMin = 1 << 2; //16 times per second
					}

					// Get Enemy data
					for ( int i = 0; i < iEnemyCountStage; i++ )
					{
						char szEnemyName[32] = { 0 };
						pcDB->GetData( 6 + (i * 2), PARAMTYPE_String, szEnemyName, 32 );

						// Is enemy?
						if ( !IsNull( szEnemyName ) )
						{
							CharacterData * pEnemyInfo = FindEnemyCharInfo( szEnemyName );

							// Is Enemy in Data?
							if ( pEnemyInfo )
							{
								STRINGCOPY( pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].szMonsterName, szEnemyName );
								pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].cd = pEnemyInfo;
								pcDB->GetData( 6 + (i * 2) + 1, PARAMTYPE_Integer, &pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].iSpawnRate);
								pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].iSpawnRateCum = pMonsterStageList->iTotalMonsterSpawnRate;
								pMonsterStageList->iTotalMonsterSpawnRate += pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].iSpawnRate;
								pMonsterStageList->iTotalMonsters++;

								//Special case
								if ( pEnemyInfo->iUniqueMonsterID != 1002 ) //Sod Spawner
								{
									//Check if drop definition exists for this monster
									if ( LOOTSERVER->DropDefinitionExistsForMonsterID ( pEnemyInfo->iUniqueMonsterID ) == FALSE )
									{
										WARN ( "MapMonster: Drop table not defined for monster: %s (%d)", szEnemyName, pEnemyInfo->iUniqueMonsterID );
									}
								}
							}
							else
							{
								LOGERROR ( "MapMonster: Unknown Monster name: %s", szEnemyName );
							}
						}
					}

					// Get Boss data
					std::vector<std::string> vTimeBoss;
					for ( int i = 0; i < iBossCountStage; i++ )
					{
						char szBossName[32] = { 0 };
						pcDB->GetData( 30 + (i * 4), PARAMTYPE_String, szBossName, 32 );

						// Is Boss?
						if ( !IsNull( szBossName ) )
						{
							CharacterData * pBossInfo = FindEnemyCharInfo( szBossName );

							// Is Boss in Data?
							if ( pBossInfo )
							{
								char szTimeBoss[128] = { 0 };
								pcDB->GetData( 30 + (i * 4) + 1, PARAMTYPE_String, szTimeBoss, 128 );
								vTimeBoss = split( szTimeBoss, ' ' );

								pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].cdBoss = pBossInfo;

								for ( UINT u = 0; u < vTimeBoss.size(); u++ )
								{
									pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].caBossHour[u] = (char)atoi( vTimeBoss[u].c_str() );
									pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].iBossHours++;
								}

								if ( LOOTSERVER->DropDefinitionExistsForMonsterID ( pBossInfo->iUniqueMonsterID ) == FALSE )
								{
									WARN ( "MapMonster: Drop table not defined for boss: %s (%d)", szBossName, pBossInfo->iUniqueMonsterID );
								}

								// Minion Boss
								char szMinionBoss[32]	= { 0 };
								int iMinionCount		= 0;
								pcDB->GetData( 30 + (i * 4) + 2, PARAMTYPE_String, szMinionBoss, 32 );
								pcDB->GetData( 30 + (i * 4) + 3, PARAMTYPE_Integer, &iMinionCount );

								// Is Minion Boss?
								if ( !IsNull( szMinionBoss ) )
								{
									CharacterData * pMinionBossInfo = FindEnemyCharInfo( szMinionBoss );

									// Is Minion Boss in Data?
									if ( pMinionBossInfo )
									{
										pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].cdMinion	= pMinionBossInfo;
										pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].iMinions	= iMinionCount;

										if ( LOOTSERVER->DropDefinitionExistsForMonsterID ( pMinionBossInfo->iUniqueMonsterID ) == FALSE )
										{
											WARN ( "MapMonster: Drop table not defined for boss minion: %s (%d)", szMinionBoss, pMinionBossInfo->iUniqueMonsterID );
										}
									}
									else
									{
										LOGERROR ( "MapMonster: Unknown Boss Minion Name: %s", szMinionBoss );
									}
								}
								pMonsterStageList->iTotalBosses++;
							}
							else
							{
								LOGERROR ( "MapMonster: Unknown Boss Name: %s", szBossName );
							}
						}
					}
				}
			}
			pcDB->Close();
		}
	}

	uCurrentStageUnitEnemy++;
}



void UnitInfoServer::ReadUnitNpcStand( Map * pcMap )
{
	if ( !GAME_SERVER )
		return;

	int iTotalUnits = 0;

	// Get Count NPCs in Stage

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Npc, 6 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(*) FROM MapNPC WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitStand );
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iTotalUnits);
		}
		pcDB->Close();
	}

	if ( iTotalUnits != 0 )
	{
		// Pointer to packet PlayInfo[200] in StageData
		PacketUnitInfo * psPacket = pcMap->saNPC;


		// Get Data Array

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM MapNPC WHERE Stage=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitStand );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					for ( int i = 0; i < iTotalUnits; i++ )
					{
						BOOL bEnabled = FALSE;

						// SQL data
						pcDB->GetData( 3, PARAMTYPE_Integer, &psPacket[i].sPosition.iX);
						pcDB->GetData( 4, PARAMTYPE_Integer, &psPacket[i].sPosition.iY);
						pcDB->GetData( 5, PARAMTYPE_Integer, &psPacket[i].sPosition.iZ);
						pcDB->GetData( 6, PARAMTYPE_Integer, &psPacket[i].sAngle.iY);
						pcDB->GetData( 7, PARAMTYPE_String, psPacket[i].sCharacterData.szName, 32 );
						pcDB->GetData( 8, PARAMTYPE_Integer, &bEnabled);
						pcDB->GetData( 9, PARAMTYPE_Integer, &psPacket[i].sCharacterData.iClanID);

						// Packet structure
						psPacket[i].iLength = sizeof( PacketUnitInfo );

						// Is Enabled?
						if ( bEnabled )
							psPacket[i].iHeader = PKTHDR_NpcData;

						psPacket[i].sCharacterData.saUnused = 200; // ???
						psPacket[i].sCharacterData.sHP.sCur = 100; // HP default
						psPacket[i].sCharacterData.sHP.sMax = 100; // HP default
						psPacket[i].iID = 8100; // Default ID
						psPacket[i].sPosition.iX *= 256; // X << 8
						psPacket[i].sPosition.iY *= 256; // Y << 8
						psPacket[i].sPosition.iZ *= 256; // Z << 8
						pcDB->Fetch();
					}
				}
			}
			pcDB->Close();
		}
	}

	// Next Stage
	uCurrentStageUnitStand++;
}

void UnitInfoServer::ReadUnitNpcData( CharacterData * pCharInfo, UnitInfo *pMonInfo, char *pDialogMessage )
{
	if ( !GAME_SERVER )
		return;

	int iTargetID = atoi( pCharInfo->szName );
	int iGMOnly = pCharInfo->iClanID;
	pCharInfo->iClanID = 0;

	pCharInfo->NPC.szHeadModel[0] = 0;
	pCharInfo->sSize = 0;
	pCharInfo->sViewBoxZoom.sMin = 0;
	pCharInfo->sViewBoxZoom.sMax = 0;

	ZeroMemory( ( void* )pMonInfo, sizeof( UnitInfo ) );
	pMonInfo->iMovementRange = 64 * 256;

	//NPC Movement Range
	if( iTargetID == 16 )
		pMonInfo->iMovementRange = 8 * 256;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Npc, 7);

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM NPCList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iTargetID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pCharInfo->iNPCId = iTargetID;

				pcDB->GetData( 2, PARAMTYPE_String, pCharInfo->szName, 32 );
				pcDB->GetData( 3, PARAMTYPE_String, pCharInfo->NPC.szBodyModel, 64 );


				// Messages Npc
				{
					typedef char*(__cdecl *t_SetNpcMessage) (char * pszMessage);
					t_SetNpcMessage SetNpcMessage = (t_SetNpcMessage)0x00433390;

					// Set Npc Messages
					for ( int i = 0; i < 4; i++ )
					{
						pcDB->GetData( 4 + i, PARAMTYPE_String, pDialogMessage, 128 );

						// Is string and not null?
						if ( (lstrlenA( pDialogMessage ) > 0) && (!IsNull( pDialogMessage )) )
							pMonInfo->pszaMessage[pMonInfo->iNumMessage++] = SetNpcMessage( pDialogMessage );
					}
				}

				int iEventNpc = 0;
				int iEventParam = 0;
				int iTeleport = 0;

				//NPC Event data
				pcDB->GetData( 8, PARAMTYPE_Integer, &iEventNpc);
				pcDB->GetData( 9, PARAMTYPE_Integer, &iEventParam);

				//Skill Quests
				pcDB->GetData( 10, PARAMTYPE_Integer, &pMonInfo->SkillChangeJobNPC);

				int iMonsterClass;
				pcDB->GetData( 11, PARAMTYPE_Integer, &iMonsterClass);
				pCharInfo->sMonsterClass = (EMonsterClass)iMonsterClass;

				//Quest Type
				pcDB->GetData( 12, PARAMTYPE_Integer, &pMonInfo->QuestCode);

				//Quest TypeSub
				pcDB->GetData( 13, PARAMTYPE_Integer, &pMonInfo->QuestParam);

				//Teleport
				pcDB->GetData( 14, PARAMTYPE_Integer, &iTeleport);

				//Event NPC
				switch ( iEventNpc )
				{
					case NPCID_EventGirl:
						pMonInfo->iEventGirl = TRUE;
						break;

					case NPCID_ItemDistributor:
						pMonInfo->iItemBox = TRUE;
						break;

					case NPCID_Warehouse_DoNotUse: //obsolete - use below
						//pMonInfo->WareHouseMasterNPC = TRUE;
						break;

					case NPCID_Warehouse:
						pMonInfo->NPCEventCode = NPCEVENTCODE_Warehouse;
						//pMonInfo->WareHouseMasterNPC = TRUE;
					break;

					case NPCID_Warehouse_Seasonal:
						pMonInfo->NPCEventCode = NPCEVENTCODE_WarehouseSeasonal;
						//pMonInfo->WareHouseMasterNPC = TRUE;
					break;

					case NPCID_Force:
						pMonInfo->ItemMixNPC = NPCEVENTCODE_Force;
						break;

					case NPCID_CastleControl:
						pMonInfo->BlessCastleNPC = TRUE;
						if ( iEventParam != 0 )
							pMonInfo->BlessCastleNPC = iEventParam;
						break;

					case NPCID_Aging:
						pMonInfo->ItemAgingNPC = TRUE;
						break;

					// xxstr mix guild
					case NPCID_MIXGUILD:
						pMonInfo->NPCEventCode = NPCEVENTCODE_MixGuild;
						break;

					// xxstr rank list
					case NPCID_RANKLIST:
						pMonInfo->NPCEventCode = NPCEVENTCODE_RankList;
						break;

					case NPCID_SOLORANKLIST:
						pMonInfo->NPCEventCode = NPCEVENTCODE_SoloRankList;
						break;

					case NPCID_Smelting:
						pMonInfo->NPCEventCode = NPCEVENTCODE_Smelting;
						break;

					case NPCID_ClanControl:
						pMonInfo->iClanController = TRUE;
						break;

					case NPCID_Teleport:
						pMonInfo->TeleportID = iTeleport; //1000 = Mayor Maz (Atlantis), 1002 = Bless Castle teleport
						break;

					case NPCID_SkillMaster:
						pMonInfo->SkillMasterNPC = TRUE;
						break;

					case NPCID_Mixing:
						pMonInfo->ItemMixNPC = TRUE;
						break;

					case NPCID_Manufacturing:
						pMonInfo->NPCEventCode = NPCEVENTCODE_Manufacture;
						break;

					case NPCID_ResetItem:
						pMonInfo->NPCEventCode = NPCEVENTCODE_ResetItem;
						break;

					case NPCID_CoinShop:
						pMonInfo->NPCEventCode = NPCEVENTCODE_CoinShop;
						break;

					case NPCID_FuryArenaNPC:
						pMonInfo->NPCEventCode = NPCEVENTCODE_FuryArenaNPCTeleportCT2;
						break;

					case NPCID_FuryArenaNPCEnter:
						pMonInfo->NPCEventCode = NPCEVENTCODE_FuryArenaNPCEnter;
						break;

					case NPCID_SocketSystemNPCDrill:
						pMonInfo->NPCEventCode = NPCEVENTCODE_SocketSystemNPCDrill;
						break;

					case NPCID_SocketSystemNPCStone:
						pMonInfo->NPCEventCode = NPCEVENTCODE_SocketSystemNPCStone;
						break;

					case NPCID_NPCEasterEgg:
						pMonInfo->NPCEventCode = NPCEVENTCODE_EasterNPC;
						break;

					case NPCID_LarryQuests:
						pMonInfo->NPCEventCode = NPCEVENTCODE_LarryQuestsNPC;
						break;

					case NPCID_IhinEvent:
						pMonInfo->NPCEventCode = NPCEVENTCODE_IhinEvent;
						break;

					case NPCID_MarinaQuests:
						pMonInfo->NPCEventCode = NPCEVENTCODE_MarinaQuestsNPC;
						break;

					case NPCID_RudolphChristmas:
						pMonInfo->NPCEventCode = NPCEVENTCODE_RudolphChristmasNPC;
						break;

					case NPCID_WarpGate:
						pMonInfo->WingQuestNpc = TRUE;
						if ( iEventParam != 0 )
							pMonInfo->WingQuestNpc = iEventParam;
						break;

					case NPCID_Bellatra:
						pMonInfo->EventNPC = TRUE;
						if ( iEventParam != 0 )
							pMonInfo->EventNPC = iEventParam;
						break;
					break;

					default:
						break;
				}

				// Weapon Shop
				{
					char szShopBuffer[512] = { 0 };
					ItemData * pItem = NULL;
					pcDB->GetData( 15, PARAMTYPE_String, szShopBuffer, 512 );
					std::vector<std::string> strShopItems = split( szShopBuffer, ' ' );

					for ( auto v : strShopItems )
					{
						pItem = ITEMSERVER->FindItemPointerTable( v.c_str() );
						if ( pItem )
							pMonInfo->SellAttackItem[pMonInfo->SellAttackItemCount++] = (DWORD)pItem->sBaseItemID.ToItemID();

						if ( pMonInfo->SellAttackItemCount == 32 )
							break;
					}
				}

				// Defense Shop
				{
					char szShopBuffer[512] = { 0 };
					ItemData * pItem = NULL;
					pcDB->GetData( 16, PARAMTYPE_String, szShopBuffer, 512 );
					std::vector<std::string> strShopItems = split( szShopBuffer, ' ' );

					for ( auto v : strShopItems )
					{
						pItem = ITEMSERVER->FindItemPointerTable( v.c_str() );
						if ( pItem )
							pMonInfo->SellDefenceItem[pMonInfo->SellDefenceItemCount++] = (DWORD)pItem->sBaseItemID.ToItemID();

						if ( pMonInfo->SellDefenceItemCount == 32 )
							break;
					}
				}

				// Misc Shop
				{
					char szShopBuffer[512] = { 0 };
					ItemData * pItem = NULL;
					pcDB->GetData( 17, PARAMTYPE_String, szShopBuffer, 512 );
					std::vector<std::string> strShopItems = split( szShopBuffer, ' ' );

					for ( auto v : strShopItems )
					{
						pItem = ITEMSERVER->FindItemPointerTable( v.c_str() );
						if ( pItem )
							pMonInfo->SellEtcItem[pMonInfo->SellEtcItemCount++] = (DWORD)pItem->sBaseItemID.ToItemID();

						if ( pMonInfo->SellEtcItemCount == 32 )
							break;
					}
				}

				// GM Only
				pMonInfo->bGMOnly = iGMOnly;

				// Set pcUnitData pointer
				pCharInfo->psUnitInfo = (UnitInfo*)pMonInfo;
			}
		}
		pcDB->Close();
	}

	char szModelFile[ 64 ] = { 0 };
	STRINGCOPY( szModelFile, pCharInfo->NPC.szBodyModel );
	ReadInx( szModelFile );

	STRINGCOPY( szModelFile, pCharInfo->NPC.szBodyModel );
	typedef void( __cdecl *t_ClassMob ) ( char * pszModelFile );
	t_ClassMob LoadOftenMeshPattern = ( t_ClassMob )0x00442860;
	LoadOftenMeshPattern( szModelFile );

}

void UnitInfoServer::OnDescriptionSpawn( int iMapID, int iX, int iZ, char * pszDescription )
{
	SpawnDataInfo::SpawnData * ps = MAPSERVER->GetMapSpawnData( iMapID, iX, iZ );

	if ( ps )
		STRINGCOPYLEN( pszDescription, 32, ps->szName );
}

void UnitInfoServer::SetupMonsterBellatra( UINT BellatraMemory, UINT ConfigMemory )
{
	// Total Rounds
	const int iRound = 8;

	// Set Memory Pointer bellatra
	UINT uMemBellatra = BellatraMemory;

	// Loop Struct Round
	for ( int i = 0; i < iRound; i++ )
	{
		// Rounds
		*( int* )( ConfigMemory + 0x10 ) += 1;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_EventDB );


		if ( pcDB->Open() )
		{
			std::string query = "SELECT * FROM BellatraMonster WHERE Round=?";

#ifdef DEV_MODE
			query = "SELECT * FROM BellatraMonsterDebugMode WHERE Round=?";
#endif // DEBUG


			if ( pcDB->Prepare(query.c_str()) )
			{
				int iRoundCur = i + 1;
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iRoundCur );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					// Set Monsters
					for ( int j = 0; j < 5; j++ )
					{
						char szMonsterName[32];
						int iMonsterCount = 0;
						pcDB->GetData( 3 + (j * 2), PARAMTYPE_String, szMonsterName, 32 );
						pcDB->GetData( 4 + (j * 2), PARAMTYPE_Integer, &iMonsterCount);
						STRINGCOPYLEN( (char*)(uMemBellatra + (0x28 * j)), 32, szMonsterName );
						(*(UINT*)(uMemBellatra + 0x24 + (0x28 * j))) = iMonsterCount;

						if ( j == 4 ) // Boss?
						{
							// Count Boss to appear in stage
							(*(UINT*)(0x08B80F50 + (i * 4))) = (*(UINT*)(uMemBellatra + 0x24 + (0x28 * j)));

							(*(UINT*)(uMemBellatra + 0x24 + (0x28 * j))) = 0;
						}
					}
					// View Sight
					int iViewSight = 0;
					pcDB->GetData( 13, PARAMTYPE_Integer, &iViewSight);


					// Hour to begin
					int iHour = 0;
					pcDB->GetData( 14, PARAMTYPE_Integer, &iHour);

					if ( *(UINT*)0x08B81130 == 0 )
					{
						*(UINT*)(uMemBellatra + 0x0CC) = iHour;
						if ( i == 0 )
						{
							*(UINT*)(0x006E3720 + 0xCC) = iHour;
							*(UINT*)(0x006E3EE0 + 0xCC) = iHour;
						}
					}

					// Gold
					int iGold = 0;
					pcDB->GetData( 15, PARAMTYPE_Integer, &iGold);
					*(UINT*)(uMemBellatra + 0x0D0) = iGold;

					// Delay to next round
					int iDelayRound = 0;
					pcDB->GetData( 16, PARAMTYPE_Integer, &iDelayRound);
					*(UINT*)(uMemBellatra + 0x0D8) = iDelayRound;

					// Percent Count Total
					int iMaxPercentCount = 0;
					pcDB->GetData( 17, PARAMTYPE_Integer, &iMaxPercentCount);
					*(UINT*)(uMemBellatra + 0x0C8) = iMaxPercentCount;

					// Max monsters in the round
					int iMaxMonsters = 0;
					pcDB->GetData( 18, PARAMTYPE_Integer, &iMaxMonsters);
					*(UINT*)(0x08B81078 + (i * 4)) = iMaxMonsters;
					*(UINT*)(uMemBellatra + 0x0D4) = iMaxMonsters;

				}
			}
			pcDB->Close();
		}
		uMemBellatra += 0x0DC;
	}
	UINT uLastHourStart = *(UINT*)(uMemBellatra - 0x0DC + 0x0CC);
	*(UINT*)(uMemBellatra + 0x0CC) = uLastHourStart + 3;
	if ( *(UINT*)(uMemBellatra + 0x0CC) > 23 )
		*(UINT*)(uMemBellatra + 0x0CC) -= 24;

	if ( *(UINT*)0x08B81130 == 1 )
	{
		*(UINT*)0x08B81130 = 0;
		*(UINT*)(BellatraMemory + 0xCC) = 6;
	}
}

void UnitInfoServer::Init()
{

}


