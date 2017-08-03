


#include  "CCommandScript.h"



//------------------------------------------------------------------------------------------
CommandScript::CommandScript()
{
	m_bUsing = false;
	m_ppScript = NULL;
	BOOL m_bPause =  FALSE;
	m_iScriptLine = 0 ;  
  	m_iCurrScriptLine = 0; 
	m_iCurrChar = 0; 
    m_iPauseEndTime = 0;
}
//------------------------------------------------------------------------------------------
CommandScript::~CommandScript()
{
	CloseScript();
}
//------------------------------------------------------------------------------------------
BOOL CommandScript::OpenScript(const char *filename)
{
	FILE * pScriptFile;	
	TCHAR szText[64];
	if( (pScriptFile = fopen(filename, "rb")) == NULL)
	{
		wsprintf(szText, "加载文件%s失败！", filename);
		MessageBox(NULL, szText, TEXT("错误"), MB_OK);	
		return FALSE;
	}
	while(!feof(pScriptFile))
	    if(fgetc(pScriptFile) == '\n')
			++m_iScriptLine;
	++m_iScriptLine;
	fclose(pScriptFile);

	if( (pScriptFile = fopen(filename, "r")) == NULL)
	{
		wsprintf(szText, "打开文件%s失败！", filename);
		MessageBox(NULL, szText, TEXT("错误"), MB_OK);	
		return FALSE;
	}
	m_ppScript = (char **) malloc( m_iScriptLine * sizeof(char *));

	for(int iCurrLine = 0; iCurrLine < m_iScriptLine ; ++iCurrLine)
	{
		m_ppScript[iCurrLine] = (char *) malloc(MAX_LINE_SIZE + 1);
		memset(m_ppScript[iCurrLine], 0, MAX_LINE_SIZE);
		//if (iCurrLine >= 12606)
		//{
		//	char temptext[MAX_LINE_SIZE];
		//	fgets(temptext, MAX_LINE_SIZE, pScriptFile);
		//	int tttt = 0;
		//}
		//else
		fgets(m_ppScript[iCurrLine], MAX_LINE_SIZE, pScriptFile);
	}
	fclose(pScriptFile);
	m_iCurrChar = 0;
	m_iCurrScriptLine = 0;
	m_bUsing = true;
	return TRUE;
}
//------------------------------------------------------------------------------------------
void CommandScript::CloseScript()
{
	if(m_ppScript != NULL)
	{
		for(int iCurrLine = 0; iCurrLine < m_iScriptLine ; ++iCurrLine)
		{
			if(m_ppScript[iCurrLine] != NULL)
			{
				free(m_ppScript[iCurrLine]);
				m_ppScript[iCurrLine] = NULL;
			}

		}
		free(m_ppScript);
		m_ppScript = NULL;
	}
	m_bUsing = false;
	BOOL m_bPause =  FALSE;
	m_iScriptLine = 0 ;  
	m_iCurrScriptLine = 0; 
	m_iCurrChar = 0; 
	m_iPauseEndTime = 0;
}
//------------------------------------------------------------------------------------------
void CommandScript::GetCommand(char * pCommandString)
{
	int iCommandLen = 0;
	char cCurrChar;
	while(m_iCurrChar < (int) strlen(m_ppScript[m_iCurrScriptLine]))
	{
	   cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	   if(cCurrChar == ' ' || cCurrChar == '\n')
		   break;
	   pCommandString[iCommandLen] = cCurrChar;
	   ++iCommandLen;
	   ++m_iCurrChar;
	}
	m_iCurrChar ++;	  //跳过空格
    pCommandString[iCommandLen] = '\0';
	strupr(pCommandString);
}

void CommandScript::GetTotalLine(char* lineContent)
{
	m_iCurrChar = 0;
	char cCurrChar;
	char* testcharText = m_ppScript[m_iCurrScriptLine];
	size_t currLength = strlen(m_ppScript[m_iCurrScriptLine]);
	while (m_iCurrChar < (int)currLength)
	{
		cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
		if(cCurrChar == '\n')
			break;
		lineContent[m_iCurrChar] = cCurrChar;
		++m_iCurrChar;
	}
	lineContent[m_iCurrChar] = '\0';
}
//------------------------------------------------------------------------------------------
int CommandScript::GetIntParam()
{
	char pIntString[MAX_LINE_SIZE];
	int iIntParamLen = 0;
	char cCurrChar;
	cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	while(m_iCurrChar < (int) strlen(m_ppScript[m_iCurrScriptLine]))
	{
	   cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	   if(cCurrChar == ' ' || cCurrChar == '\n')
		   break;
	   pIntString[iIntParamLen] = cCurrChar;
	   m_iCurrChar++;
	   iIntParamLen++;
	}
	m_iCurrChar++;	    //跳过空格
	pIntString[iIntParamLen] = '\0';
	return atoi(pIntString);
}
//------------------------------------------------------------------------------------------
long CommandScript::GetLongParam()
{
	if(m_iCurrScriptLine >= m_iScriptLine)
	{
		return SCRIPT_ERROR;
	}
	char pIntString[MAX_LINE_SIZE];
	int iIntParamLen = 0;
	char cCurrChar;
	cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	while(m_iCurrChar < (int) strlen(m_ppScript[m_iCurrScriptLine]))
	{
		cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
		if(cCurrChar == ' ' || cCurrChar == '\n')
			break;
		pIntString[iIntParamLen] = cCurrChar;
		m_iCurrChar++;
		iIntParamLen++;
	}
	m_iCurrChar++;	    //跳过空格
	pIntString[iIntParamLen] = '\0';
	return atol(pIntString);
}
//------------------------------------------------------------------------------------------
DInt64 CommandScript::GetLLongParam()
{
	if(m_iCurrScriptLine >= m_iScriptLine)
	{
		return SCRIPT_ERROR;
	}
	char pIntString[MAX_LINE_SIZE];
	int iIntParamLen = 0;
	char cCurrChar;
	cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	while(m_iCurrChar < (int) strlen(m_ppScript[m_iCurrScriptLine]))
	{
		cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
		if(cCurrChar == ' ' || cCurrChar == '\n')
			break;
		pIntString[iIntParamLen] = cCurrChar;
		m_iCurrChar++;
		iIntParamLen++;
	}
	m_iCurrChar++;	    //跳过空格
	pIntString[iIntParamLen] = '\0';
	return _atoi64(pIntString);
}
//------------------------------------------------------------------------------------------
float CommandScript::GetFloatParam()
{
	if(m_iCurrScriptLine >= m_iScriptLine)
	{
		return SCRIPT_ERROR;
	}
	char pIntString[MAX_LINE_SIZE];
	int iIntParamLen = 0;
	char cCurrChar;
	cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	while(m_iCurrChar < (int) strlen(m_ppScript[m_iCurrScriptLine]))
	{
		cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
		if(cCurrChar == ' ' || cCurrChar == '\n')
			break;
		pIntString[iIntParamLen] = cCurrChar;
		m_iCurrChar++;
		iIntParamLen++;
	}
	m_iCurrChar++;	    //跳过空格
	pIntString[iIntParamLen] = '\0';
	float f = atof(pIntString);
	return f;
}
//------------------------------------------------------------------------------------------
void CommandScript::GetStringParam(char * pParamString)
{
	int iParamStringLen = 0;
	char cCurrChar;
	while(m_iCurrChar < (int) strlen(m_ppScript[m_iCurrScriptLine]))
	{
	   cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	   if(cCurrChar == ' ' || cCurrChar == '\n')
		   break;
	   pParamString[iParamStringLen] = cCurrChar;
	   iParamStringLen++;
	   m_iCurrChar++;
	}
	m_iCurrChar += 1;  //跳过空格
	pParamString[iParamStringLen] = '\0';
}

void CommandScript::GetTextguiString(char *pString)
{
	int iParamStringLen = 0;
	char cCurrChar;
	while(m_iCurrChar < (int) strlen(m_ppScript[m_iCurrScriptLine]))
	{
	   cCurrChar = m_ppScript[m_iCurrScriptLine][m_iCurrChar];
	   if(cCurrChar == '>' || cCurrChar == '\n')
		   break;
	   pString[iParamStringLen] = cCurrChar;
	   iParamStringLen++;
	   m_iCurrChar++;
	}
	m_iCurrChar += 1;  //跳过空格
	pString[iParamStringLen] = '\0';
}

CommandScript* getComandScript()
{
	static CommandScript g_commandScript[5];
	for(int i = 0; i < 5; i++)
	{
		if(!g_commandScript[i].GetUsing())
			return &(g_commandScript[i]);
	}
	return NULL;
}