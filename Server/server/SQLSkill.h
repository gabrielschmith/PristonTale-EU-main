#pragma once
class CSQLSkill
{
public:
	CSQLSkill();
	BOOL							GetSkillValueFloatArray ( enum ESkillID eSkillId, std::string columnName, float * fValues, BOOL bIgnoreWarning = FALSE );
	BOOL							GetSkillValueIntArray ( enum ESkillID eSkillId, std::string columnName, int * iValues, BOOL bIgnoreWarning = FALSE );
	BOOL							GetSkillChainScaleValue( enum ESkillID eSkillId, std::string columnName, int * piClass, float * pfScale );
	~CSQLSkill( );
};

