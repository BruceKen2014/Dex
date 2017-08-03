
#ifndef _DEX_DEFINE_H
#define _DEX_DEFINE_H

#define  DEX_DEBGU   //开发模式  
#define  DEX_LOG     //开启log

#define  DEMO_ERROR   0xFFFF   //异常错误
#define  PI           3.14159265358979323846
#define  FVF_XYZ_TEX1		  (D3DFVF_XYZ | D3DFVF_TEX1 )
#define  FVF_XYZ_T1_N		  (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL)
#define  FVF_XYZ_DIFF_TEX1	  (D3DFVF_XYZ| D3DFVF_DIFFUSE |D3DFVF_TEX1 )
#define  FVF_XYZ_COLOR_T1_N	  (D3DFVF_XYZ| D3DFVF_DIFFUSE  | D3DFVF_TEX1 | D3DFVF_NORMAL)

//取0~1之间的浮点数随机数
#define RandFloat()	  (((float)rand()) / RAND_MAX)
//角度弧度转换
#define  _getDegree(radian)		 180.0f * (radian)/PI
#define  _getRadian(degree)		 PI * (degree)/180.0f

#define _SafeRelease(t)	  \
{						 \
	if((t) != NULL)			 \
			{						  \
		(t)->Release();			\
		(t) = NULL;			 \
			}						 \
}
#define _SafeDelete(t)	  \
{						 \
	if((t) != NULL)			 \
			{						  \
		delete (t);			\
		(t) = NULL;			 \
			}						 \
}
#define SAFE_DELETE(T)  _SafeDelete(T)

#define  _SafeDeleteArr(t) \
{							  \
	if((t) != NULL)			   \
			{							\
		delete[] (t);				\
		(t) = NULL;				 \
			}							\
}
#define _SafeFree(t)	   \
{						   \
	if((t) != NULL)		  \
			{					  \
		free((t));		 \
		(t) = NULL;	   \
			}					\
}
#define _SafeUpdate(t)   \
{						\
	if((t) != NULL)		\
		(t)->Update();   \
}

#define  _SafeRender(t)	\
{					   \
	if((t) != NULL)		\
		(t)->Render();   \
}
#define  _SafeAddRef(t)\
{	if((t) != NULL)\
		(t)->AddRef();\
}
#define  _SafeReduceRef(t)\
{	if ((t) != NULL)\
	(t)->ReduceRef(); \
}
#define  CheckPoint(t)  ((t)!=NULL)
#define  DEX_ENSURE(p)     if(!(p))   return; 
#define  DEX_ENSURE_B(p)   if(!(p))   return false;
#define  DEX_ENSURE_P(p)   if(!(p))   return DexNull;
#define  DEX_ENSURE_I(p)   if(!(p))   return 0;
#define  DEX_ENSURE_F(p)   if(!(p))   return 0.0f;
#define  DEX_ENSURE_S(p)   if(!(p))   return DString();

#define  DEX_ENSURE_LOG(p,warning)     if(!(p))  { DexLog::getSingleton()->LogLine(log_allert, (warning));  return;} 
#define  DEX_ENSURE_B_LOG(p,warning)     if(!(p))  { DexLog::getSingleton()->LogLine(log_allert, (warning));  return false;} 
#define  DEX_CLIP(v,s,b)   if((v) < (s) )\
						       (v) = (s);\
						   else if ((v) > (b))	\
							   (v) = (b);
#define foreach(type, it, value)  type::iterator it;for( it= (value).begin(); it != (value).end(); ++it)
#define foreach_const(type, it, value)  type::const_iterator it;for( it= (value).begin(); it != (value).end(); ++it)

#include <vector>
//安全清空std::vector对象向量
template<typename T>
void _SafeClearVector(std::vector<T>& vec)
{
	for (std::vector<T>::iterator it = (vec).begin(); it != (vec).end();)
	{
		if (*it != NULL)
		{
			_SafeDelete((*it));
			(vec).erase(it);
			it = (vec).begin();
			if (it == (vec).end())
				break;
		}
	}
	(vec).clear(); 
}
#endif