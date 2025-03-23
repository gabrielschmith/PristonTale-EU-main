#include "stdafx.h"
#include "chatserver.h"


ChatServer::ChatServer()
{
}


ChatServer::~ChatServer()
{
}

void ChatServer::SendGMError ( UserData * pcUserData, const char * pszFormat, ... )
{
	User * pcUser = USERDATATOUSER ( pcUserData );
	if ( pcUserData )
	{
		if ( pcUserData->iGameLevel >= GAMELEVEL_Two && pcUser->bDebugChat )
		{
			va_list ap;

			PacketChatBoxMessage sPacket;
			ZeroMemory ( &sPacket, sizeof ( PacketChatBoxMessage ) );
			sPacket.iLength = sizeof ( PacketChatBoxMessage );
			sPacket.iHeader = PKTHDR_ChatMessage;
			sPacket.iChatColor = EChatColor::CHATCOLOR_Error;

			char szMessage[246];

			va_start ( ap, pszFormat );
			vsnprintf_s ( szMessage, 246, pszFormat, ap );
			va_end ( ap );


			std::string message = "ERROR> " + std::string ( szMessage );

			STRINGCOPY (sPacket.szChatBoxMessage, message.c_str () );

			SENDPACKET ( USERDATATOUSER ( pcUserData ), &sPacket );
		}
	}
}

void ChatServer::SendDebugChat( User * pcUser, EChatColor eColor, const char * pszFormat, ... )
{
	if ( pcUser == FALSE || pcUser->IsValidAndInGame() == FALSE )
		return;

	if ( pcUser->GetGameLevel() >= GAMELEVEL_Two )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
		sPacket.iLength = sizeof(PacketChatBoxMessage);
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = EChatColor::CHATCOLOR_Error;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 252, pszFormat, ap );
		va_end( ap );

		if (pcUser->bDebugChat)
		{
			SENDPACKET( pcUser, &sPacket );
			DEBUG( "CHAT_DEBUG -- %s", sPacket.szChatBoxMessage );
		}
	}
}

void ChatServer::SendChatAllUsersInStage( int iStage, EChatColor eColor, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iLength = sizeof(PacketChatBoxMessage);
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			if ( USERSDATA[i].iMapID == iStage )
				SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
		}
	}
}

void ChatServer::SendChatAll( EChatColor eColor, const char * pszText )
{
	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
	sPacket.iLength = sizeof( PacketChatBoxMessage );
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	STRINGCOPY( sPacket.szChatBoxMessage, pszText );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			SENDPACKET( USERDATATOUSER( &USERSDATA[i] ), &sPacket );
		}
	}
}

void ChatServer::SendChatAllGM(const char* pszFormat, ...)
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
	sPacket.iLength = sizeof(PacketChatBoxMessage);
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = CHATCOLOR_GameMaster;

	va_start(ap, pszFormat);
	vsnprintf_s(sPacket.szChatBoxMessage, 252, pszFormat, ap);
	va_end(ap);

	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		if (USERSDATA[i].iGameLevel >= GAMELEVEL_Two &&
			USERSDATA[i].pcSocketData)
		{
			SENDPACKET(USERDATATOUSER(&USERSDATA[i]), &sPacket);
		}
	}
}

void ChatServer::SendChatAllEx( EChatColor eColor, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
	sPacket.iLength = sizeof( PacketChatBoxMessage );
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			if ( pcUser->bNetServer )
				continue;

			SENDPACKET( pcUser, &sPacket );
		}
	}
}

void ChatServer::SendChatAllUsersInRange( int iX, int iZ, EMapID iStage, int iRange, EChatColor eColor, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iLength = sizeof(PacketChatBoxMessage);
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = eColor;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData* userData = &USERSDATA[i];
		if (userData && userData->pcSocketData && userData->iMapID == iStage)
		{
			int iXP = ( iX - USERSDATA[i].sPosition.iX ) >> 8;
			int iZP = ( iZ - USERSDATA[i].sPosition.iZ ) >> 8;
			int iDist = iXP*iXP + iZP*iZP;
			if ( iDist < iRange && abs( iXP ) < 4096 && abs( iZP ) < 4096 )
				SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
		}
	}
}

void ChatServer::SendChatAllUsersInRangeButMe(UserData* pcUserData, int iRange, EChatColor eColor, std::string str)
{
	if (pcUserData && pcUserData->pcSocketData)
	{
		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.iLength = sizeof(PacketChatBoxMessage);
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = eColor;
		sPacket.lID = pcUserData->iID;

		STRINGCOPY(sPacket.szChatBoxMessage, str.c_str());

		for (int i = 0; i < PLAYERS_MAX; i++)
		{
			UserData* userData = &USERSDATA[i];
			if (userData && userData->pcSocketData && userData->pcSocketData != pcUserData->pcSocketData)
			{
				if (pcUserData->sPosition.WithinXZDistance(&userData->sPosition, iRange, DISTANCE_MISC_Y_EX))
				{
					SENDPACKET(USERDATATOUSER(userData), &sPacket);
				}
			}
		}
	}
}



void ChatServer::SendChatAllUsersInClan( UserData * pcUserData, EChatColor eColor, const char * pszFormat, ... )
{
	if ( pcUserData && pcUserData->pcSocketData )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
		sPacket.iLength = sizeof(PacketChatBoxMessage);
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = eColor;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 256, pszFormat, ap );
		va_end( ap );

		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			if ( USERSDATA[i].pcSocketData )
			{
				if ( USERSDATA[i].iClanID == pcUserData->iClanID && USERSDATA[i].pcSocketData != pcUserData->pcSocketData )
					SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
			}
		}
	}
}


void ChatServer::SendChat( User * pcUser, EChatColor eColor, const char * pszText )
{
	if ( pcUser )
	{

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = eColor;

		STRINGCOPY( sPacket.szChatBoxMessage, pszText );

		sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

		SENDPACKET( pcUser, &sPacket );
	}
}

void ChatServer::SendChatEx( User * pcUser, EChatColor eColor, const char * pszFormat, ... )
{
	if ( pcUser )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof( PacketChatBoxMessage ) );
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = eColor;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
		va_end( ap );

		sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

		SENDPACKET( pcUser, &sPacket );
	}
}

void ChatServer::SendUserBoxChat( UserData * pcUserData, DWORD dwObjectID, const char * pszFormat, ... )
{
	if ( pcUserData )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
		sPacket.iHeader = PKTHDR_ChatMessage;
		sPacket.iChatColor = CHATCOLOR_Error;
		sPacket.lID = dwObjectID;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
		va_end( ap );

		sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}
}

void ChatServer::SendUserBoxChatRange( int iX, int iZ, int iRange, DWORD dwObjectID, const char * pszFormat, ... )
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iHeader = PKTHDR_ChatMessage;
	sPacket.iChatColor = CHATCOLOR_Error;
	sPacket.lID = dwObjectID;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );

	sPacket.iLength = 16 + lstrlenA( sPacket.szChatBoxMessage ) + 12;

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			int iXP = ( iX - USERSDATA[i].sPosition.iX ) >> 8;
			int iZP = ( iZ - USERSDATA[i].sPosition.iZ ) >> 8;
			int iDist = iXP*iXP + iZP*iZP;
			int im = iRange / 1000;
			if ( iDist < iRange && abs( iXP ) < 4096 && abs( iZP ) < 4096 )
				SENDPACKET( USERDATATOUSER(&USERSDATA[i]), &sPacket );
		}
	}
}

void ChatServer::SendTitleBox( User * pcUser, const char * pszFormat, ... )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return;


	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
	sPacket.iHeader = PKTHDR_TitleBoxMessage;
	sPacket.iChatColor = CHATCOLOR_Error;

	va_start( ap, pszFormat );
	vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
	va_end( ap );
	sPacket.iLength = 0x8 + 0x4 + 0x4 + lstrlenA( sPacket.szChatBoxMessage );

	SENDPACKET( pcUser, &sPacket );

}

void ChatServer::SendTitleBox( UserData * pcUserData, const char * pszFormat, ... )
{
	if ( pcUserData )
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory( &sPacket, sizeof(PacketChatBoxMessage) );
		sPacket.iHeader = PKTHDR_TitleBoxMessage;
		sPacket.iChatColor = CHATCOLOR_Error;

		va_start( ap, pszFormat );
		vsnprintf_s( sPacket.szChatBoxMessage, 255, pszFormat, ap );
		va_end( ap );
		sPacket.iLength = 0x8 + 0x4 + 0x4 + lstrlenA( sPacket.szChatBoxMessage );

		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}
}

void ChatServer::SendChatSpyMember( UserData * pcUserData, void * pPacket )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	SENDPACKET( pcUser, pPacket );
	if ( pcUserData && pcUser->pcUserDataSpyoner )
	{
		SendChatEx( pcUser, EChatColor::CHATCOLOR_Error, "> [%s User]", pcUserData->sCharacterData.szName );
		SENDPACKET( USERDATATOUSER(pcUser->pcUserDataSpyoner), pPacket );
	}
}

void ChatServer::SendChatTrade( UserData * pcUserData, std::string strMessage )
{
	if ( pcUserData )
	{
		User * pcUser = USERDATATOUSER( pcUserData );

		if (pcUser == NULL || pcUserData == NULL )
			return;

		if ( pcUser->bMuted )
		{
			INFO( "Player '%s' is muted", CHARACTERSERVER->GetCharacterName( pcUserData ) );
			CHATSERVER->SendChat ( pcUser, CHATCOLOR_Error, "> You have been muted by a GM. You may not talk freely until your mute has expired." );
			CHATSERVER->SendTitleBox( pcUser->pcUserData, "You are muted!" );
			return;
		}

		if ( pcUserData->sCharacterData.iLevel < 10 )
		{
			CHATSERVER->SendChat ( pcUser, CHATCOLOR_Error, "> You must be level 10 or higher to use the Trade chat" );
			return;
		}

		if ( CanSendMessage( pcUser, strMessage, CHATCOLOR_Trade ) )
		{
			strMessage.erase( 0, 7 );

			const char * szMessage = strMessage.c_str();

			SendChatAll( CHATCOLOR_Trade, FormatString( "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUserData ), szMessage) );
			pcUser->dwChatTradeTime = GetCurrentTime() + ((2 * 60) * 1000);

			RecordTradeChatToDatabase(pcUserData, szMessage);
		}
	}
}

std::string ChatServer::GetFormatWhisper( User * pcUser, const std::string pszMessage, BOOL bReceiverTime )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return "";

	return FormatString( "%s> %s: %s", bReceiverTime ? "From" : "To", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pszMessage.c_str() );
}

void ChatServer::SendWhisper(User* pcUserSender, User* pcUserReceiver, const std::string pszMessage)
{
	if ( pcUserSender == NULL || pcUserSender->IsValidAndInGame() == FALSE )
		return;

	if ( pcUserSender->bMuted )
	{
		INFO("Player '%s' is muted", CHARACTERSERVER->GetCharacterName(pcUserSender->pcUserData));

		CHATSERVER->SendChat(pcUserSender, CHATCOLOR_Error, "> You have been muted by a GM. You may not talk freely until your mute has expired.");
		CHATSERVER->SendTitleBox(pcUserSender->pcUserData, "You are muted!");
		return;
	}

	if ( pcUserReceiver == NULL || pcUserReceiver->IsValidAndInGame() == FALSE )
		return;

	if ( !pszMessage.empty() )
	{
		if ( !pcUserReceiver->pcUserData->bDisabledWhisper )
		{
			//To Receiver
			CHATSERVER->SendChat(pcUserReceiver, CHATCOLOR_Blue, GetFormatWhisper(pcUserSender, pszMessage, TRUE).c_str());

			//To Sender
			CHATSERVER->SendChat(pcUserSender, CHATCOLOR_Blue, GetFormatWhisper(pcUserReceiver, pszMessage, FALSE).c_str());
		}
		else
			CHATSERVER->SendChatEx(pcUserSender, CHATCOLOR_Error, "%s disabled private message!", CHARACTERSERVER->GetCharacterName(pcUserReceiver->pcUserData));
	}
	else
	{
		CHATSERVER->SendChatEx(pcUserSender, CHATCOLOR_Error, "%s is online at %s%s", CHARACTERSERVER->GetCharacterName(pcUserReceiver->pcUserData), GSERVER->GetServerName(pcUserReceiver->pcUserData->bServerIndexID), pcUserReceiver->pcUserData->bDisabledWhisper ? "(No Whisper)" : "");

	}
}

void ChatServer::SendWhisperItemLink( User * pcUserSender, User * pcUserReceiver, PacketItemLinkChat * psPacket )
{
	if ( pcUserSender == NULL || pcUserSender->IsValidAndInGame () == FALSE )
		return;

	if ( pcUserReceiver == NULL || pcUserReceiver->IsValidAndInGame () == FALSE )
		return;

	if ( !pcUserReceiver->pcUserData->bDisabledWhisper )
	{
		std::string strMessage = psPacket->szMessage;

		if ( strMessage.length() == 0 )
			strMessage = psPacket->cItemData.sItem.szItemName;

		if ( strMessage.find( "<item>" ) != std::string::npos )
			strMessage.replace( strMessage.find( "<item>" ), STRLEN( "<item>" ), psPacket->cItemData.sItem.szItemName );

		//To Receiver
		STRINGCOPY( psPacket->szMessage, GetFormatWhisper( pcUserSender, strMessage.c_str(), TRUE ).c_str() );
		SENDPACKET( pcUserReceiver, psPacket );

		//To Sender
		STRINGCOPY( psPacket->szMessage, GetFormatWhisper( pcUserReceiver, strMessage.c_str(), FALSE ).c_str() );
		SENDPACKET( pcUserSender, psPacket );
	}
	else
		CHATSERVER->SendChatEx( pcUserSender, CHATCOLOR_Error, "%s disabled private message!", CHARACTERSERVER->GetCharacterName( pcUserReceiver->pcUserData ) );
}



void ChatServer::RecordPublicChatToDatabase(UserData* pcUserData, const char* szMessage)
{
	if (pcUserData == NULL)
		return;

	std::string l_Message = szMessage;
	std::size_t l_EndPos = l_Message.find_first_of(' ');

	if (l_EndPos == std::string::npos)
		return;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ChatDB, 1);

	if (pcDB->Open())
	{
		l_Message = l_Message.substr(l_EndPos + 1);

		if (pcDB->Prepare("INSERT INTO PublicChat([MapID],[CharacterName],[Message],[UnixTime]) VALUES(?,?,?,?)"))
		{
			DWORD unixTime = GetUnixTime();

			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUserData->iMapID);
			pcDB->BindParameterInput(2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUserData), 32);
			pcDB->BindParameterInput(3, PARAMTYPE_String, (char*)l_Message.c_str(), 264);
			pcDB->BindParameterInput(4, PARAMTYPE_Integer, &unixTime);
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void ChatServer::RecordGameMasterCommandToDatabase ( User * pcUser, const char * szMessage )
{
	std::string l_Message = szMessage;
	std::size_t l_EndPos = l_Message.find_first_of ( ' ' );

	if ( l_EndPos == std::string::npos )
		return;

	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_ChatDB, 2 );

	if ( pcDB->Open () )
	{
		l_Message = l_Message.substr ( l_EndPos + 1 );

		if ( pcDB->Prepare ( "INSERT INTO GameMasterCommand([IP],[CharacterName],[Command],[UnixTime]) VALUES(?,?,?,?)" ) )
		{
			DWORD unixTime = GetUnixTime ();


			pcDB->BindParameterInput ( 1, PARAMTYPE_String, pcUser->pcUserData->pcSocketData->szIP, 16 );
			pcDB->BindParameterInput ( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName ( pcUser->pcUserData ), 32 );
			pcDB->BindParameterInput ( 3, PARAMTYPE_String, (char *)l_Message.c_str (), 264 );
			pcDB->BindParameterInput ( 4, PARAMTYPE_Integer, &unixTime );
			pcDB->Execute ();
		}

		pcDB->Close ();
	}
}

void ChatServer::RecordClanChatToDatabase(UserData* pcUserData, const char* szMessage)
{
	if (pcUserData == NULL || pcUserData->iClanID == 0)
		return;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ChatDB, 3);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("INSERT INTO ClanChat([ClanID],[CharacterName],[Message],[UnixTime]) VALUES(?,?,?,?)"))
		{
			DWORD unixTime = GetUnixTime();

			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUserData->iClanID);
			pcDB->BindParameterInput(2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUserData), 32);
			pcDB->BindParameterInput(3, PARAMTYPE_String, (char*)szMessage, 264);
			pcDB->BindParameterInput(4, PARAMTYPE_Integer, &unixTime);
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void ChatServer::RecordPartyChatToDatabase(UserData* pcUserData, const char* szMessage)
{
	if (pcUserData == NULL)
		return;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ChatDB, 4);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("INSERT INTO PartyChat([CharacterName],[Message],[UnixTime]) VALUES(?,?,?)"))
		{
			DWORD unixTime = GetUnixTime();

			pcDB->BindParameterInput(1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUserData), 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, (char*)szMessage, 264);
			pcDB->BindParameterInput(3, PARAMTYPE_Integer, &unixTime);
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void ChatServer::RecordTradeChatToDatabase(UserData* pcUserData, const char* szMessage)
{
	if (pcUserData == NULL)
		return;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ChatDB, 5);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("INSERT INTO TradeChat([CharacterName],[Message],[UnixTime]) VALUES(?,?,?)"))
		{
			DWORD unixTime = GetUnixTime();

			pcDB->BindParameterInput(1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUserData), 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, (char*)szMessage, 264);
			pcDB->BindParameterInput(3, PARAMTYPE_Integer, &unixTime);
			pcDB->Execute();
		}

		pcDB->Close();
	}
}


void ChatServer::RecordPrivateChatToDatabase(UserData* pcSender, UserData* pcReceiver, const std::string pszMessage)
{
	if (pcSender == NULL || pcReceiver == NULL)
		return;

	if (pszMessage.empty())
		return;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ChatDB, 6);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("INSERT INTO PrivateChat([SenderName],[ReceiverName],[Message],[UnixTime]) VALUES(?,?,?,?)"))
		{
			DWORD unixTime = GetUnixTime();


			pcDB->BindParameterInput(1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcSender), 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcReceiver), 32);
			pcDB->BindParameterInput(3, PARAMTYPE_String, (char*)pszMessage.c_str(), 264);
			pcDB->BindParameterInput(4, PARAMTYPE_Integer, &unixTime);
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void ChatServer::SendPersonalShopChat(UserData* pcUserData, DWORD dwObjectID, const char* pszFormat, ...)
{
	if (pcUserData)
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.iHeader = PKTHDR_PersonalShopMessage;
		sPacket.iChatColor = CHATCOLOR_Error;
		sPacket.lID = dwObjectID;

		va_start(ap, pszFormat);
		vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
		va_end(ap);

		sPacket.iLength = 16 + lstrlenA(sPacket.szChatBoxMessage) + 12;

		SENDPACKET(USERDATATOUSER(pcUserData), &sPacket);
	}
}


void ChatServer::HandlePacket( User * pcUser, PacketItemLinkChat * psPacket )
{
	if ( pcUser->bMuted )
	{
		INFO( "Player '%s' is muted", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		CHATSERVER->SendChat ( pcUser, CHATCOLOR_Error, "> You have been muted by a GM. You may not talk freely until your mute has expired." );
		CHATSERVER->SendTitleBox( pcUser->pcUserData, "You are muted!" );
		return;
	}

	psPacket->cItemData.sItem.iChk1 += Dice::RandomI( 1000, 20000 );
	psPacket->cItemData.sItem.iChk2 += Dice::RandomI( 2000, 10000 );

	if ( psPacket->szCharacterName[0] != 0 )
	{
		User * pcUserReceiver = USERDATATOUSER( USERDATABYNAME( psPacket->szCharacterName ) );

		if ( pcUserReceiver )
			SendWhisperItemLink( pcUser, pcUserReceiver, psPacket );
		else
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> %s is offline or not found!", psPacket->szCharacterName );
	}
	else
	{
		std::string strMessage = GetMessageString( psPacket->szMessage, psPacket->eChatColor );

		if ( strMessage.length() == 0 )
			strMessage = psPacket->cItemData.sItem.szItemName;

		if ( strMessage.find( "<item>" ) != std::string::npos )
			strMessage.replace( strMessage.find( "<item>" ), STRLEN( "<item>" ), psPacket->cItemData.sItem.szItemName );

		if ( psPacket->eChatColor == CHATCOLOR_Normal )
		{
			STRINGFORMAT( psPacket->szMessage, "%s: %s", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

			for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
			{
				User * pcUserReceiver = USERSERVER->pcaUserInGame[i];

				if ( pcUserReceiver && pcUserReceiver->pcUserData && (pcUserReceiver->bNetServer == FALSE) && (pcUser != pcUserReceiver) )
				{
					if ( pcUser->pcUserData->sPosition.WithinXZDistance( &pcUserReceiver->pcUserData->sPosition, DISTANCE_MAX_CHATRANGE, DISTANCE_MISC_Y_EX) )
						SENDPACKET( pcUserReceiver, psPacket );
				}
			}

			SENDPACKET( pcUser, psPacket );
		}
		else if ( psPacket->eChatColor == CHATCOLOR_Trade )
		{
			if ( CanSendMessage( pcUser, psPacket->szMessage, psPacket->eChatColor ) )
			{
				STRINGFORMAT( psPacket->szMessage, "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

				for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
				{
					User * pcUserReceiver = USERSERVER->pcaUserInGame[i];

					if ( pcUserReceiver && pcUserReceiver->pcUserData && (pcUserReceiver->bNetServer == FALSE) && (pcUser != pcUserReceiver) )
					{
						if ( pcUser->pcUserData->sPosition.WithinXZDistance( &pcUserReceiver->pcUserData->sPosition, DISTANCE_MAX_CHATRANGE, DISTANCE_MISC_Y_EX ) )
							SENDPACKET( pcUserReceiver, psPacket );
					}
				}

				pcUser->dwChatTradeTime = GetCurrentTime() + ((2 * 60) * 1000);

				SENDPACKET( pcUser, psPacket );
			}
		}
		else if ( (psPacket->eChatColor == CHATCOLOR_Party) || (psPacket->eChatColor == CHATCOLOR_Raid) )
		{
			STRINGFORMAT( psPacket->szMessage, "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

			if ( pcUser->bParty && pcUser->psParty )
			{
				std::vector<User*> vUsers = PARTYHANDLER->GetPartyMembers( pcUser->psParty, (psPacket->eChatColor == CHATCOLOR_Raid) ? true : false );

				for ( auto &v : vUsers )
				{
					if ( v != pcUser )
						SENDPACKET( v, psPacket );
				}
			}

			SENDPACKET( pcUser, psPacket );
		}
		else if ( psPacket->eChatColor == CHATCOLOR_Clan )
		{
			if ( CHARACTERSERVER->IsCharacterLeader( pcUser ) )
				psPacket->eChatColor = CHATCOLOR_ClanLeader;

			STRINGFORMAT( psPacket->szMessage, "[%c]%s: %s", GSERVER->GetServerName( pcUser->pcUserData->bServerIndexID )[0], CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), strMessage.c_str() );

			if ( pcUser->pcUserData->iClanID )
			{
				for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
				{
					User * pcUserReceiver = USERSERVER->pcaUserInGame[i];

					if ( pcUserReceiver && pcUserReceiver->pcUserData && (pcUserReceiver->bNetServer == FALSE) && (pcUser != pcUserReceiver) )
					{
						if ( pcUserReceiver->pcUserData->iClanID && (pcUser->pcUserData->iClanID == pcUserReceiver->pcUserData->iClanID) )
							SENDPACKET( pcUserReceiver, psPacket );
					}
				}

				SENDPACKET( pcUser, psPacket );
			}
		}
	}
}

std::string ChatServer::GetMessageString( std::string strMessage, int iChatColor )
{
	std::string str = strMessage;

	if ( iChatColor == CHATCOLOR_Trade )
	{
		if ( strMessage.length() >= 7 )
		{
			if ( strMessage.find( "TRADE>" ) != std::string::npos )
			{
				str = strMessage.substr( strMessage.find_first_of( ">" ) + 1 );
				str = trim( str );
			}
		}
	}
	else if ( iChatColor == CHATCOLOR_Clan )
	{
		if ( strMessage.length() >= 6 )
		{
			if ( strMessage.find( "CLAN>" ) != std::string::npos )
			{
				str = strMessage.substr( strMessage.find_first_of( ">" ) + 1 );
				str = trim( str );
			}
		}
	}
	else if ( (iChatColor == CHATCOLOR_Party) || (iChatColor == CHATCOLOR_Raid) )
	{
		if ( (strMessage.length() >= 1) && ((strMessage[0] == '@') || (strMessage[0] == '#')) )
		{
			str = strMessage.substr( 1 );
			str = trim( str );
		}
	}

	return str;
}


BOOL ChatServer::CanSendMessage( User * pcUser, std::string strMessage, int iChatColor )
{
	BOOL bRet = TRUE;

	if ( iChatColor == CHATCOLOR_Trade )
	{
		if ( strMessage.length() >= 7 )
		{
			strMessage = strMessage.substr( strMessage.find_first_of( ">" ) + 1 );
			strMessage = trim( strMessage );

			std::string strFind = strMessage;

			//To lower
			for ( UINT i = 0; i < strMessage.length(); i++ )
				strFind[i] = tolower( strFind[i] );

			char cFirstLetter = strMessage[0];
			char cSecondLetter = strMessage[1];

			BOOL bFreeTrade = TRUE;

			//Not GM and on time delay?
			if ( pcUser->pcUserData->iGameLevel == GAMELEVEL_None &&
				 pcUser->dwChatTradeTime > GetCurrentTime() )
			{
				if ( !FREE_TRADECHAT )
				{
					//Warning user
					SendChatEx( pcUser, CHATCOLOR_Error, "> Wait %d seconds to write again", (pcUser->dwChatTradeTime - GetCurrentTime()) / 1000 );
					bFreeTrade = FALSE;
					bRet = FALSE;
				}
			}

			if ( bFreeTrade )
			{
				//Not correct first letters?
				if ( (cFirstLetter != 'S' && cFirstLetter != 'C' && cFirstLetter != 'B' && cFirstLetter != 'P' && cFirstLetter != 'T') || cSecondLetter != '>' )
				{
					//Notify Users
					SendChat( pcUser, CHATCOLOR_Error, "> You can only start your phrase with" );
					SendChat( pcUser, CHATCOLOR_Error, "S> For selling" );
					SendChat( pcUser, CHATCOLOR_Error, "B> For buying" );
					SendChat( pcUser, CHATCOLOR_Error, "T> For trading" );
					SendChat( pcUser, CHATCOLOR_Error, "P> Looking for party" );
					SendChat( pcUser, CHATCOLOR_Error, "C> Looking for clan" );
					SendChat( pcUser, CHATCOLOR_Error, "Example: P> Need party in Iron2" );
					bRet = FALSE;
				}
			}

			if ( bRet )
			{
				for ( int i = 0; i < _countof( pszaWordsTrade ); i++ )
				{
					//Is found words? don't write in chat
					if ( strstr( strFind.c_str(), pszaWordsTrade[i] ) )
					{
						bRet = FALSE;
						break;
					}
				}
			}
		}
		else
			bRet = FALSE;
	}

	return bRet;
}
