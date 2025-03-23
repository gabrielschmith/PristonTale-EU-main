#include "stdafx.h"
#include "SQLSkill.h"

typedef int( __cdecl *t_rangeNumberPT ) ( int min, int max );
t_rangeNumberPT rangeNumberPT = ( t_rangeNumberPT )0x00439B50;

CSQLSkill::CSQLSkill()
{
}


/// <summary>
/// Get skill chained scale value along with class num
/// from a given skill id and column name from the database
/// </summary>
/// <param name="eSkillId">Skill id</param>
/// <param name="columnName">Column name</param>
/// <param name="piClass">Int pointer to class num</param>
/// <param name="pfScale">Float point to scale value</param>
/// <returns>TRUE if data was found</returns>
BOOL CSQLSkill::GetSkillChainScaleValue( enum ESkillID eSkillId, std::string columnName, int * piClass, float * pfScale)
{
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_SkillDBNew, 8 );

	const char * query = FormatString ( "Select TOP (1) Class, %s from SkillChain WHERE SkillId=%d", columnName.c_str (), eSkillId );

	float scale = 1.0f;
	int classNum = -1;

	if ( pcDB->Open () )
	{
		if ( pcDB->Prepare ( query ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &classNum );
				pcDB->GetData( 2, PARAMTYPE_Float, &scale );
			}
		}
		pcDB->Close ();
	}

	*pfScale = scale;
	*piClass = classNum;

	return classNum != -1;
}

/// <summary>
/// Get the skill data from SkillData table based on column name and skill ID
/// </summary>
/// <param name="columnName"></param>
/// <param name="iValues"></param>
/// <returns></returns>
BOOL CSQLSkill::GetSkillValueIntArray ( enum ESkillID eSkillId, std::string columnName, int * iValues, BOOL bIgnoreWarning )
{
	ZeroMemory ( iValues, sizeof ( int ) * 10 );
	int i_DataIndex = 0;
	int iValue = 0;
	BOOL bResult = FALSE;
	BOOL bFoundAnyZeroValue = FALSE;
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_SkillDBNew, 9 );


	const char * query = FormatString ( "Select %s from SkillData WHERE SkillId=%d", columnName.c_str (), eSkillId );

	if ( pcDB->Open () )
	{
		if ( pcDB->Prepare ( query ) )
		{
			if ( pcDB->Execute () )
			{
				while ( pcDB->Fetch () )
				{
					if ( i_DataIndex < 10 )
					{
						pcDB->GetData ( 1, PARAMTYPE_Integer, &iValues[i_DataIndex] );

						if ( !bFoundAnyZeroValue && iValues[i_DataIndex] == 0 )
						{
							bFoundAnyZeroValue = TRUE;
						}
					}

					i_DataIndex++;
				}

				//Expecting 10
				if ( i_DataIndex > 10 )
				{
					LOGERROR ( "GetSkillValueIntArray contains more than 10 records (%d) for query! %s", i_DataIndex, query );
				}

				//Expecting 10
				else if ( i_DataIndex < 10 )
				{
					LOGERROR ( "GetSkillValueIntArray contains less than than 10 records (%d) for query! %s", i_DataIndex, query );
				}

				//Expecting 10 values
				if ( bFoundAnyZeroValue && !bIgnoreWarning )
				{
					WARN ( "GetSkillValueIntArray contains zero value(s) for query! %s", query );
				}

				bResult = TRUE;
			}
		}
		pcDB->Close ();
	}

	if ( bResult == FALSE )
	{
		LOGERROR( "GetSkillValueIntArray failed for query: %s", query );
	}

	return bResult;
}

BOOL CSQLSkill::GetSkillValueFloatArray ( enum ESkillID eSkillId, std::string columnName, float * fValues, BOOL bIgnoreWarning  )
{
	ZeroMemory ( fValues, sizeof ( float ) * 10 );
	int i_DataIndex = 0;
	int iValue = 0;
	BOOL bResult = FALSE;
	BOOL bFoundAnyZeroValue = FALSE;
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_SkillDBNew, 10 );

	const char * query = FormatString ( "Select %s from SkillData WHERE SkillId=%d", columnName.c_str (), eSkillId);


	if ( pcDB->Open () )
	{
		if ( pcDB->Prepare ( query ) )
		{
			if ( pcDB->Execute () )
			{
				while ( pcDB->Fetch () )
				{
					if ( i_DataIndex < 10 )
					{
						pcDB->GetData ( 1, PARAMTYPE_Float, &fValues[i_DataIndex] );

						if ( !bFoundAnyZeroValue && fValues[i_DataIndex] == 0.0f )
						{
							bFoundAnyZeroValue = TRUE;
						}
					}

					i_DataIndex++;
				}

				//Expecting 10
				if ( i_DataIndex > 10)
				{
					LOGERROR ( "GetSkillValueFloatArray contains more than 10 records (%d) for query! %s", i_DataIndex, query );
				}

				//Expecting 10
				else if ( i_DataIndex < 10 )
				{
					LOGERROR ( "GetSkillValueFloatArray contains less than than 10 records (%d) for query! %s", i_DataIndex, query );
				}

				//Expecting 10 values
				if ( bFoundAnyZeroValue && !bIgnoreWarning)
				{
					WARN ( "GetSkillValueFloatArray contains zero value(s) for query! %s", query );
				}

				bResult = TRUE;
			}
		}
		pcDB->Close ();
	}

	if ( bResult == FALSE )
	{
		LOGERROR( "GetSkillValueFloatArray failed for query: %s", query );
	}

	return bResult;
}


CSQLSkill::~CSQLSkill()
{
}
