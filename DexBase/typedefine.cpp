



#include "typedefine.h"
#include "DexLog.h"
#include "../state/DexGameEngine.h"
//================================================================
void _Message(char * message, ...)
{
	va_list v_p;
	va_start(v_p, message);
	TCHAR szText[1024];
	wvsprintf(szText, message,v_p);
	va_end(v_p);
	MessageBox(NULL, szText, TEXT("提示"), MB_OK);	
};
char *del(char *p,char *q)
{
	int i,j,k,m,n,count;
	n=strlen(p);
	m=strlen(q);
	for(i=0;i<n;i++)
	{
		j=0;
		k=i;
		count=0; /* 用于统计字符与子字符匹配数 */
		while(p[k]==q[j] && q[j]!='\0')
		{
			k++;
			j++;   
			count++; 
		}
		if(count==m)  
			break;
	}  
	for(;i<n-m;i++)  /* 删除子字符串 */
		p[i]=p[i+m];
	p[i]='\0';     /* 加上结构字符 */
	return p;
}

HRESULT FindMediaFile( TCHAR* strPath, TCHAR* strFilename )
{
	TCHAR strFullPath[1024];
	TCHAR *strShortName;
	TCHAR *path;
	DWORD cchPath;

	if( NULL==strFilename || NULL==strPath )
		return E_INVALIDARG;

	// Build full path name from strFileName (strShortName will be just the leaf filename)
	cchPath = GetFullPathName(strFilename, sizeof(strFullPath)/sizeof(TCHAR), strFullPath, &strShortName);
	if ((cchPath == 0) || (sizeof(strFullPath)/sizeof(TCHAR) <= cchPath))
		return E_FAIL;
	path=del(strFullPath,strFilename);
	sprintf(strPath,"%ssrc\\%s",path,strFilename);
	return S_OK;
}
void _GetExeParth(char * path)
{
	char ch[256];
	GetModuleFileName(NULL,ch,255);
	int i=strlen(ch);
	for(int j = i; j > 0; j--)
	{
		if(ch[j] == '\\')
		{
			ch[j] = '\0';
			break;
		}
	}
	strcpy(path, ch);
}
float _GetDistance(D3DXVECTOR3 point1, D3DXVECTOR3 point2)
{
	float _x = point1.x - point2.x;
	float _y = point1.y - point2.y;
	float _z = point1.z - point2.z;

	return sqrt(_x * _x + _y *_y + _z *_z);
}

bool DecideISClock(D3DXVECTOR3 old, D3DXVECTOR3 _new)
{
	if(old.z * _new.z > 0)
	{
		if(old.z > 0)
		{
			if(old.x < _new.x)
				return true;
			else
				return false;
		}
		else
		{
			if(old.x < _new.x)
				return false;
			else
				return true;
		}
	}
	else if(old.z * _new.z < 0)
	{
		if(old.x * _new.x > 0)
		{
			if(old.x > 0)
			{
				if(old.z >= _new.z)
					return true;
				else
					return false;
			}
			else
			{
				if(old.z >= _new.z)
					return false;
				else
					return true;
			}
		}
		else if(old.x * _new.x < 0)
		{
			D3DXVECTOR3 old_back;
			old_back.x = -old.x;
			old_back.y = -old.y;
			old_back.z = -old.z;
			return DecideISClock(_new, old_back);
		}
		else
		{
			if(old.x == 0)
			{
				if(old.z <= 0)
				{
					if(_new.x > 0)
						return false;
					else
						return true;

				}
				else
				{
					if(_new.x > 0)
						return true;
					else
						return false;
				}
			}
			else
			{
				if(_new.z <= 0)
				{
					if(old.x > 0)
						return true;
					else
						return false;

				}
				else
				{
					if(old.x > 0)
						return false;
					else
						return true;
				}
			}
		}
	}
	else
	{
		if(old.z == 0)
		{
			if(old.x <= 0)
			{
				if(_new.z >0)
					return true;
				else
					return false;
			}
			else
			{
				if(_new.z >0)
					return false;
				else
					return true;
			}
		}
		else
		{
			if(_new.x <= 0)
			{
				if(old.z >0)
					return false;
				else
					return true;			}
			else
			{
				if(old.z >0)
					return true;
				else
					return false;
			}
		}
	}
}

stTexture::stTexture()
{
	m_pTexture = NULL;
	m_iWidth = 0;
	m_iHeight = 0;
}
stTexture::stTexture(LPDIRECT3DDEVICE9 pD3DDevice, const char *filename)
{
	D3DXIMAGE_INFO info;
	if(D3DXCreateTextureFromFileEx(pD3DDevice, filename, D3DX_DEFAULT_NONPOW2/*使用此参数才不会放大，使用0或者D3DX_DEFAULT图片会被放大一倍*/
		, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,0, D3DFMT_UNKNOWN, 
		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE,
		NULL, &info, NULL, &m_pTexture) != D3D_OK)
	{
		_Message("加载%s失败!");
	}
	else
	{
	   m_iWidth = info.Width;
	   m_iHeight = info.Height;
	}
}
stTexture::~stTexture()
{
	_SafeRelease(m_pTexture);
}

int GetClientWidth()
{
	RECT rect;
	GetClientRect(DexGameEngine::getEngine()->GetHwnd(), &rect);
	return rect.right - rect.left;
}
int GetClientHeight()
{
	RECT rect;
	GetClientRect(DexGameEngine::getEngine()->GetHwnd(), &rect);
	return rect.bottom - rect.top;
}

int GetWorldX(float posX)
{
	return -GetClientWidth()/2+posX;
}

int GetWorldY(float posY)
{
	return GetClientHeight()/2-posY;
}

D3DXVECTOR2 GetWorldPos(const D3DXVECTOR2& pos)
{
	return D3DXVECTOR2(GetWorldX(pos.x), GetWorldY(pos.y));
}

void SplitStr(string str, char split_char, vector<string> &out)
{
	char* pt = (char*)str.c_str();
	char* begin = pt;
	char* end = strchr(pt, split_char);
	while(end != NULL)
	{		
		string temp;
		temp.insert(0, begin, end - begin);
		out.push_back(temp);
		begin = end +1;
		end = strchr(begin, split_char);
	}
	if(end == NULL && begin != NULL)
	{
		string temp;
		temp.insert(0, begin, pt + str.size() - begin);
		out.push_back(temp);
	}
}

stVertex0          g_pVertexList0[4];//所有渲染系统共用的顶点与缓冲区 各个渲染系统在渲染前要根据自身需要进行数据重置
LPDIRECT3DVERTEXBUFFER9 g_pBuffer0 = NULL;
stVertex1          g_pVertexList1[4];//所有渲染系统共用的顶点与缓冲区 各个渲染系统在渲染前要根据自身需要进行数据重置
LPDIRECT3DVERTEXBUFFER9 g_pBuffer1 = NULL;

bool InitVertexBuff()
{
	if(FAILED(DexGameEngine::getEngine()->GetDevice()->CreateVertexBuffer(4 * sizeof(stVertex0),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, FVF_XYZ_TEX1, D3DPOOL_DEFAULT,
		&g_pBuffer0, NULL))) return false;
	void* ptr;
	//g_pVertexList0[0] = stVertex0(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), 0.0f, 0.0f ); 
	//g_pVertexList0[1] = stVertex0(D3DXVECTOR3(0.5f, 0.5f, 0.0f),  1.0f, 0.0f ); 
	//g_pVertexList0[2] = stVertex0(D3DXVECTOR3(-0.5f, -0.5f, 0.0f),0.0f, 1.0f ); 
	//g_pVertexList0[3] = stVertex0(D3DXVECTOR3(0.5f, -0.5f, 0.0f), 1.0f, 1.0f ); 
	g_pBuffer0->Lock(0, sizeof(g_pVertexList0),(void**)&ptr,0);
	memcpy(ptr, g_pVertexList0, sizeof(g_pVertexList0));
	g_pBuffer0->Unlock();

	if(FAILED(DexGameEngine::getEngine()->GetDevice()->CreateVertexBuffer(4 * sizeof(stVertex1),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&g_pBuffer1, NULL))) return false;
	g_pVertexList1[0] = stVertex1(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), DexColor(0,0.5f,0.5f,0.5f),0.0f, 0.0f ); 
	g_pVertexList1[1] = stVertex1(D3DXVECTOR3(0.5f, 0.5f, 0.0f),  DexColor(0,0.5f,0.5f,0.5f), 1.0f, 0.0f ); 
	g_pVertexList1[2] = stVertex1(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), DexColor(0,0.5f,0.5f,0.5f), 0.0f, 1.0f ); 
	g_pVertexList1[3] = stVertex1(D3DXVECTOR3(0.5f, -0.5f, 0.0f),  DexColor(0,0.5f,0.5f,0.5f), 1.0f, 1.0f ); 
	g_pBuffer1->Lock(0, sizeof(g_pVertexList1),(void**)&ptr,0);
	memcpy(ptr, g_pVertexList1, sizeof(g_pVertexList1));
	g_pBuffer1->Unlock();

	return true;
}

bool IniEngineData()
{
	DEX_ENSURE_B(InitVertexBuff());
	return true;
}

char* getDexBuffer()
{
	static char g_pBuffer[2048];
	return g_pBuffer;
}
bool  setDexBuffer(char* data,...)
{
	if(data == NULL)
	{
		getLog()->BeginLog();
		getLog()->Log(log_allert, "dexSetBuffer传入空字符串！文件%s 行数%d", __FILE__, __LINE__);
		getLog()->EndLog();
		return false;
	}
	else if(strlen(data) > 2048)
	{
		getLog()->BeginLog();
		getLog()->Log(log_allert, "dexSetBuffer传入字符串长度大于2048！文件%s 行数%d", __FILE__, __LINE__);
		getLog()->EndLog();
		return false;
	}
	memset(getDexBuffer(), 0, 2048);
	va_list v_p;
	va_start(v_p, data);
	wvsprintf(getDexBuffer(), data,v_p);
	va_end(v_p);
	return true;
}


D3DXMATRIX g_scaleMatrix;
D3DXMATRIX g_transMatrix;
D3DXMATRIX g_rotateMatrix;
D3DXMATRIX g_matrixTemp;
D3DXMATRIX g_matrix;
