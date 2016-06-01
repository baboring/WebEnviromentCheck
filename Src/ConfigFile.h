#ifndef _CCONFIGFILE_H
#define _CCONFIGFILE_H
////////////////////////////////////////////////////////////////////////////////////////
//
//    * 다음과 같은 형식의 Config파일을 읽어들여 각종 설정을 얻을 수 있음
//      # Comment
//      {name} = {value}
//      {name} = "{string value}"
////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <vector>
using namespace std;

struct CONFIGITEM
{
	char	name[128];
	char	value[128];
};

typedef std::vector<CONFIGITEM*>				ConfigFile;
typedef std::vector<CONFIGITEM*>::iterator	ConfigFileItr;

class CConfigFile
{
public:
	CConfigFile();
	CConfigFile(char *filename);
	virtual ~CConfigFile() { Close(); }

	int		Open(char *filename,char *app = NULL);
	int		Open(char* szBuffer,int nLength);
	bool	Save(char* filename);
	void	Close();
	void	Destroy();
	bool	IsOpen()			{ return m_bOpen;}

	bool	AddValue(char *name, char *value);
	bool	SetValue(char *name, char *value);
	char*	GetStringValue(char *name, char *defval = 0);
	char*	GetStringValueCopy(char *name,char *result, char *defval = 0);
	bool	GetBoolValue(char *name, bool defval = false);
	int		GetIntValue(char *name, int defval = 0);
	float	GetFloatValue(char *name, float defval = 0.f);

	void	MakeConfigList(char* szBuffer,int nLength);

	ConfigFileItr	GetBegin()		{ return m_ConfigList.begin();}
	ConfigFileItr	GetEnd()		{ return m_ConfigList.end();}
	int				GetCount()		{ return m_ConfigList.size();}

protected:
	virtual void	MakeConfigList();
	char*	GetValue(char *name);

	// Helper
	void	RemoveSideChar(char **str, char ch);

	bool			m_bOpen;
	ConfigFile		m_ConfigList;
	FILE*			m_fs;
};

extern	CConfigFile	g_Config;
////////////////////////////////////////////////////////////////////////////////////////
#endif
