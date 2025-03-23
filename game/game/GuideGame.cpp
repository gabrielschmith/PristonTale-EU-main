#include "stdafx.h"
#include "GuideGame.h"


BOOL CGuideGame::guide_handle( int iLevel, ECharacterClass iClass )
{
	CGuideGame * p = this;
	switch ( iLevel )
	{
		case 1:
			p->open_guide("Level_1", "Message", "Icon");
			break;
		case 2:
			p->open_guide("Level_2", "Message", "Icon");
			break;

		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			return false;

		case 10:
			p->open_guide( "Level_10", "Message", "Icon" );
			break;

		case 11:
		case 12:
		case 13:
		case 14:
			return false;

		case 15:
			p->open_guide("Level_15", "Message", "Icon");
			break;

		case 16:
		case 17:
		case 18:
		case 19:
			return false;

		case 20:
			p->open_guide( "Level_20", "Message", "Icon" );
			break;

		case 21:
		case 22:
		case 23:
		case 24:
			return false;

		case 25:
			p->open_guide("Level_25", "Message", "Icon");
			break;

		case 26:
		case 27:
		case 28:
		case 29:
			return false;

		case 30:
			p->open_guide( "Level_30", "Message", "Icon" );
			break;

		case 31:
		case 32:
		case 33:
		case 34:
			return false;

		case 35:
			p->open_guide("Level_35", "Message", "Icon");
			break;

		case 36:
		case 37:
		case 38:
		case 39:
			return false;

		case 40:
			p->open_guide( "Level_40", "Message", "Icon" );
			break;
		case 41:
			p->open_guide( "Level_41", "Message", "Icon" );
			break;

		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
			return false;

		case 50:
			p->open_guide( "Level_50", "Message", "Icon" );
			break;

		case 51:
		case 52:
		case 53:
		case 54:
			return false;

		case 55:
			p->open_guide( "Level_55", "Message", "Icon" );
			break;

		case 56:
		case 57:
		case 58:
		case 59:
			return false;

		case 60:
			p->open_guide( "Level_60", "Message", "Icon" );
			break;
		case 61:
			p->open_guide( "Level_61", "Message", "Icon" );
			break;

		case 62:
		case 63:
		case 64:
			return false;

		case 65:
			p->open_guide( "Level_65", "Message", "Icon" );
			break;

		case 66:
		case 67:
		case 68:
		case 69:
			return false;

		case 70:
			p->open_guide( "Level_70", "Message", "Icon" );
			break;
		case 71:
			p->open_guide( "Level_71", "Message", "Icon" );
			break;

		case 72:
		case 73:
		case 74:
			return false;

		case 75:
			p->open_guide( "Level_75", "Message", "Icon" );
			break;

		case 76:
		case 77:
		case 78:
		case 79:
			return false;

		case 80:
			p->open_guide( "Level_80", "Message", "Icon" );
			break;
		case 81:
			p->open_guide( "Level_81", "Message", "Icon" );
			break;

		case 82:
		case 83:
		case 84:
			return false;

		case 85:
			p->open_guide( "Level_85", "Message", "Icon" );
			break;

		case 86:
		case 87:
		case 88:
		case 89:
			return false;

		case 90:
			p->open_guide( "Level_90", "Message", "Icon" );
			break;

		case 91:
		case 92:
		case 93:
		case 94:
		case 95:
		case 96:
		case 97:
		case 98:
		case 99:
			return false;

		case 100:
			p->open_guide( "Level_100", "Message", "Icon" );
			break;
		case 101:
			p->open_guide( "Level_101", "Message", "Icon" );
			break;

		case 102:
		case 103:
		case 104:
			return false;

		case 105:
			p->open_guide( "Level_105", "Message", "Icon" );
			break;

		case 106:
		case 107:
		case 108:
		case 109:
			return false;

		case 110:
			p->open_guide( "Level_110", "Message", "Icon" );
			break;
		case 111:
			p->open_guide( "Level_111", "Message", "Icon" );
			break;

		case 112:
		case 113:
		case 114:
			return false;

		case 115:
			p->open_guide( "Level_115", "Message", "Icon" );
			break;

		case 116:
		case 117:
		case 118:
		case 119:
			return false;

		case 120:
			p->open_guide( "Level_120", "Message", "Icon" );
			break;
		/*case 125:
			p->open_guide( "Level_125", "Message", "Icon" );
			break;
		case 126:
			p->open_guide( "Level_126", "Message", "Icon" );
			break;
		case 130:
			p->open_guide( "Level_130", "Message", "Icon" );
			break;*/
	}
	return TRUE;
}

void CGuideGame::open_guide( char * head, char * key, char * guide )
{
	INI::CReader cReader( "game\\guides\\list.ini" );

	std::string str = cReader.ReadString( head, key );

	BOOL bNextLine = FALSE;

	for ( size_t i = 1; i < str.size(); i++ )
	{
		if ( (i % 50) == 0 )
			bNextLine = TRUE;

		if ( (str[i] == ' ') && bNextLine )
		{
			str.insert( i, "\r\n" );
			bNextLine = FALSE;
		}
	}

	ZeroMemory ( this->szBuffer, _countof ( this->szBuffer ) );
	CALLT_WITH_ARG1( 0x00640420, (DWORD)this, (DWORD)str.c_str() );
}
