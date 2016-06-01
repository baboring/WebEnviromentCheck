#include "StdAfx.h"
#include ".\textloader.h"

CTextLoader	TEXTLOADER;


CTextLoader::CTextLoader(void)
{

}

CTextLoader::~CTextLoader(void)
{

}

BOOL CTextLoader::GetText(int nNum,CString& szTitle,CString& szValue)
{
	if(nNum > -1 && nNum < GetCount())
	{
		szTitle = m_ConfigList[nNum]->name;
		szValue = m_ConfigList[nNum]->value;
		return TRUE;
	}
	return FALSE;
}