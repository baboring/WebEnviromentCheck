#pragma once
#include "ConfigFile.h"

class CTextLoader : public CConfigFile
{
public:
	BOOL	GetText(int nNum,CString& szTitle,CString& szValue);
	CTextLoader(void);
	~CTextLoader(void);
};


extern CTextLoader	TEXTLOADER;