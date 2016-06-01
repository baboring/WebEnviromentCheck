////////////////////////////////////////////////////////////////////////////////////////
// CConfigList
//    2002.06.14 : kined created
////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <string.h>
#include "ConfigFile.h"

CConfigFile	g_Config;

#ifdef WIN32
#define strcasecmp(a,b)		_stricmp(a,b)
#endif

// ===============================================================================================
// 문자열 분할
// ===============================================================================================
int SpliteStr(char *szSource,char *szDest,char nCode,int nNum)
{
	char *ptr = szSource;
	char *ptrPrev = NULL;
	int nCnt=0;
	ptrPrev = ptr;

	int nLimit = strlen(szSource);
	// 문장 전체를 검사..
	for(int i=0;i<strlen(szSource)+1;i++)
	{
		// 단어를 찾음..
		if (*ptr==nCode || nLimit == i || *ptr == 0)
		{
			// 해단 순번의 단어인지 확인..
			if(nCnt == nNum)
			{
				int nSize = ptr-ptrPrev;
				if(nSize>1)
				{
					strncpy(szDest,ptrPrev,nSize);
					szDest[ptr-ptrPrev] = '\n';
				}
				else
					szDest[0] = '\n';
				return nSize;
			}
			// 문장의 끝이면 종료
			if(strlen(szSource) == i)
			{
				return 0;
			}
			ptrPrev = ptr;
			ptrPrev++;	// code값 다음부터 문장작으로 인식..
			nCnt++;
		}
		// 다음 포인터..
		ptr++;
	}
	return 0;
}
CConfigFile::CConfigFile() : m_bOpen(false),m_fs(NULL) 
{
}

CConfigFile::CConfigFile(char *filename) : m_bOpen(false) ,m_fs(NULL) 
{ 
	Open( filename ); 
}
bool CConfigFile::SetValue(char *name,char *value)
{
	if( !m_bOpen || !name )
		return false;

	ConfigFileItr		it;

	for( it = m_ConfigList.begin() ; it != m_ConfigList.end() ; it++ )
	{
		if( !strcasecmp( name, (*it)->name ) )
		{
			strcpy((*it)->value,value);
			return true;
		}
	}
	return false;
}
bool CConfigFile::AddValue(char *name,char *value)
{
	if( !m_bOpen || !name )
		return false;

	CONFIGITEM	*t = new CONFIGITEM;

	strcpy( t->name, name );
	strcpy( t->value, value );
	m_ConfigList.push_back( t );

	return true;
}
char* CConfigFile::GetStringValue(char *name, char *defval)
{
	char	*value = GetValue( name );
	if( value == NULL)
		return defval;

	return value;
}
char* CConfigFile::GetStringValueCopy(char *name,char *result, char *defval)
{
	char	*value = GetValue( name );
	if( value == NULL)
		return defval;

	strcpy(result,value);
	return defval;
}

bool CConfigFile::GetBoolValue(char *name, bool defval)
{
	char	*value = GetValue( name );
	if( !value )
		return defval;

	bool	bTmp;
	bTmp = (!strcasecmp( value, "yes" ) || !strcasecmp( value, "true" ))?true:false;
	return bTmp;
}

int CConfigFile::GetIntValue(char *name, int defval)
{
	char	*value = GetValue( name );
	if( !value )
		return defval;
	int		nTmp;
	if( value[1] == 'x' )	// 16진수 판별
		nTmp = strtol( value, NULL, 16 );
	else
		nTmp = atoi( value );
	return nTmp;
}
float CConfigFile::GetFloatValue(char *name, float defval)
{
	char	*value = GetValue( name );
	if( !value )
		return defval;
	float	nTmp = defval;
	if( value[1] == 'x' )	// 16진수 판별
		nTmp = strtol( value, NULL, 16 );
	else
		nTmp = atof( value );
	return nTmp;
}

char* CConfigFile::GetValue(char *name)
{
	if( !m_bOpen || !name )
		return NULL;

	ConfigFileItr		it;

	for( it = m_ConfigList.begin() ; it != m_ConfigList.end() ; it++ )
	{
		if( !strcasecmp( name, (*it)->name ) )
			return (*it)->value;
	}
	return NULL;
}

void CConfigFile::RemoveSideChar(char **str, char ch)
{
	int		i, len = strlen( *str );
	char	*pstr = *str;

	for( i = 0 ; i < len ; i++ )
	{
		if( pstr[len-i-1] != ch )
			break;
		pstr[len-i-1] = '\0';
	}
	len = strlen( *str );

	for( i = 0 ; i < len ; i++ )
	{
		if( pstr[i] != ch )
			break;
		*str = &pstr[i+1];
	}
}

void CConfigFile::MakeConfigList()
{
	char	buf[512];
	char	tok[] = "=\t\r\n";
	char	*nametoken, *valuetoken;

	if(m_fs)
	{
		while( !feof(m_fs) )
		{
			if(fgets(buf,512,m_fs)==NULL)
				break;

			if( buf[0] == '/' || buf[0] == '#' || buf[0] == '*' )	// #,/,*이 먼저 나오는 문장은 주석처리
				continue;

			nametoken = strtok( buf, tok );
			if( !nametoken )
				continue;
			valuetoken = strtok( NULL, tok );
			if( !valuetoken )
				continue;

			CONFIGITEM	*t = new CONFIGITEM;

			RemoveSideChar( &nametoken, ' ' );
			RemoveSideChar( &valuetoken, ' ' );
			RemoveSideChar( &valuetoken, '"' );	// 문자열 처리
			strcpy( t->name, nametoken );
			strcpy( t->value, valuetoken );
			m_ConfigList.push_back( t );
		}
	}
}
void CConfigFile::MakeConfigList(char* szBuffer,int nLength)
{
	char	buf[512];
	char	tok[] = "=\t\r\n";
	char	*nametoken, *valuetoken;

	int nTotal = 0;
	int nSize = 0;

	Destroy();

	while(1)
	{
		if(nTotal>=nLength)
			break;
		memset(buf,0,sizeof(buf));
		szBuffer = &szBuffer[nSize];
		nSize = SpliteStr(szBuffer,buf,'\n',0) + 1;
		nTotal+=nSize;

		if( buf[0] == '/' || buf[0] == '#' || buf[0] == '*' )	// #,/,*이 먼저 나오는 문장은 주석처리
			continue;

		nametoken = strtok( buf, tok );
		if( !nametoken )
			continue;
		valuetoken = strtok( NULL, tok );
		if( !valuetoken )
			continue;

		CONFIGITEM	*t = new CONFIGITEM;

		RemoveSideChar( &nametoken, ' ' );
		RemoveSideChar( &valuetoken, ' ' );
		RemoveSideChar( &valuetoken, '"' );	// 문자열 처리
		strcpy( t->name, nametoken );
		strcpy( t->value, valuetoken );
		m_ConfigList.push_back( t );
	}
}
int CConfigFile::Open(char *filename,char *app)
{
	if( m_bOpen )
		Close();

	m_fs = fopen( filename ,"rt");
	if( !m_fs )
		return 0;

	MakeConfigList();
	m_bOpen = true;
	return 1;
}
int CConfigFile::Open(char* szBuffer,int nLength)
{
	if( m_bOpen )
		Close();

	if(!szBuffer || nLength<1)
		return 0;

	MakeConfigList(szBuffer,nLength);
	m_bOpen = true;
	return 1;
}
bool CConfigFile::Save(char* filename)
{
	FILE* fp = fopen(filename,"wt");
	if(fp)
	{
		ConfigFileItr		it = m_ConfigList.begin();
		for( ;it != m_ConfigList.end();it++ )
		{
			CONFIGITEM	*p = *it;
			fprintf(fp,"%s\t=\t%s\n",p->name,p->value);
		}
		fclose(fp);
		return true;
	}
	return false;
}

void CConfigFile::Destroy()
{
	ConfigFileItr		it = m_ConfigList.begin();
	while( it != m_ConfigList.end() )
	{
		CONFIGITEM	*p = *it;
		delete p;
		it = m_ConfigList.erase( it );
	}
	m_ConfigList.clear();
}
void CConfigFile::Close()
{
	if( !m_bOpen )
		return;

	if(m_fs)
		fclose(m_fs);
	m_fs = NULL;

	Destroy();

	m_bOpen = false;
}
