

#include "../Source/CTexture.h"
#include "../widget/DexGuiStruct.h"
#include "../widget/CDexUiSrcMgr.h"
#include "CDexPartical.h"
#include "DexLog.h"
#include "CFile.h"
#include "CDexTime.h"
#include "../Source/CCommandScript.h"
#include "../state/DexGameEngine.h"
#include "../DexBase/DexCollideObject.h"


CDexPartical::CDexPartical()
{
	m_fScale = 1.0f;
	m_fU1 = 0.0f;
	m_fV1 = 0.0f;		     
	m_fU2 = 1.0f;
	m_fV2 = 1.0f;
	m_pos = D3DXVECTOR3(100,50,0);
	m_vel = D3DXVECTOR3(0,0,0);
	m_size = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_color = 0xffff0000;
	m_rotateVel = D3DXVECTOR3(0,0,0);
	m_currRotate = D3DXVECTOR3(0,0,0); 
	m_iCurrLiveTime = 0;
	m_pEmit = NULL;
}

CDexPartical::~CDexPartical()
{
	getLog()->BeginLog();
	getLog()->Log(log_ok, "销毁粒子...");
	getLog()->EndLog();
}
void CDexPartical::Update(int delta, const D3DXVECTOR3& cam_pos)
{
	m_iCurrLiveTime += delta;
	m_currRotate.x += _getRadian(m_rotateVel.x * delta/1000);
	m_currRotate.y += _getRadian(m_rotateVel.y * delta/1000);
	m_currRotate.z += _getRadian(m_rotateVel.z * delta/1000);
	m_pos.x += m_vel.x * delta/1000;
	m_pos.y += m_vel.y * delta/1000;
	m_pos.z += m_vel.z * delta/1000;
	m_vel.x += m_acc.x *delta/1000;
	m_vel.y += m_acc.y *delta/1000;
	m_vel.z += m_acc.z *delta/1000;
	if(m_iCurrLiveTime <= m_pEmit->m_iParticalLiveTime * m_pEmit->m_fMiddleTimeRate)
	{//前期
		m_fScale  += delta * (m_pEmit->m_fScaleMiddle - m_pEmit->m_fScaleStart)  /(m_pEmit->m_iParticalLiveTime* m_pEmit->m_fMiddleTimeRate);
		m_color.a += delta * (m_pEmit->m_middleColor.a - m_pEmit->m_startColor.a)/(m_pEmit->m_iParticalLiveTime* m_pEmit->m_fMiddleTimeRate);
		m_color.r += delta * (m_pEmit->m_middleColor.r - m_pEmit->m_startColor.r)/(m_pEmit->m_iParticalLiveTime* m_pEmit->m_fMiddleTimeRate);
		m_color.g += delta * (m_pEmit->m_middleColor.g - m_pEmit->m_startColor.g)/(m_pEmit->m_iParticalLiveTime* m_pEmit->m_fMiddleTimeRate);
		m_color.b += delta * (m_pEmit->m_middleColor.b - m_pEmit->m_startColor.b)/(m_pEmit->m_iParticalLiveTime* m_pEmit->m_fMiddleTimeRate);
	}
	else
	{
		m_fScale  += delta * (m_pEmit->m_fScaleEnd - m_pEmit->m_fScaleMiddle)  /(m_pEmit->m_iParticalLiveTime* (1 - m_pEmit->m_fMiddleTimeRate));
		m_color.a += delta * (m_pEmit->m_endColor.a - m_pEmit->m_middleColor.a)/(m_pEmit->m_iParticalLiveTime* (1 - m_pEmit->m_fMiddleTimeRate));
		m_color.r += delta * (m_pEmit->m_endColor.r - m_pEmit->m_middleColor.r)/(m_pEmit->m_iParticalLiveTime* (1 - m_pEmit->m_fMiddleTimeRate));
		m_color.g += delta * (m_pEmit->m_endColor.g - m_pEmit->m_middleColor.g)/(m_pEmit->m_iParticalLiveTime* (1 - m_pEmit->m_fMiddleTimeRate));
		m_color.b += delta * (m_pEmit->m_endColor.b - m_pEmit->m_middleColor.b)/(m_pEmit->m_iParticalLiveTime* (1 - m_pEmit->m_fMiddleTimeRate));
	}

	//设置对应生命周期的纹理UV
	if(m_pEmit->m_pDexTex != NULL && m_pEmit->m_pDexTex->GetTexPt() != NULL)
	{	
		int tex_width = m_pEmit->m_pDexTex->GetWidth(); 
		int per_frame_time = m_pEmit->m_iParticalLiveTime/m_pEmit->m_iTexFrameTotal;  //每一帧的持续时间
		int curr_frame = m_iCurrLiveTime/per_frame_time; //当前帧
		int total_lines = m_pEmit->m_iTexFrameTotal/m_pEmit->m_iTexFrameX;
		if(total_lines*m_pEmit->m_iTexFrameX != m_pEmit->m_iTexFrameTotal)
			total_lines++; //最后可以不满 但必须有资源
		int line = curr_frame/m_pEmit->m_iTexFrameX; //第几行
		int col  =   curr_frame%m_pEmit->m_iTexFrameX; //第几列
		m_fU1 = (float)col/m_pEmit->m_iTexFrameX;
		m_fV1 = (float)line/total_lines;
		m_fU2 = (float)(col+1)/m_pEmit->m_iTexFrameX;
		m_fV2 = (float)(line+1)/total_lines;
	}
	
	
}

bool CDexPartical::LiveOver()
{
	return m_iCurrLiveTime >= m_pEmit->m_iParticalLiveTime;
}
D3DXMATRIX CDexPartical::GetMatrix(const D3DXVECTOR3& cam_pos)
{
	//先平移到原点，然后旋转，缩放，再平移到原来位置
	D3DXMatrixIdentity(&g_rotateMatrix);
	D3DXMatrixIdentity(&g_transMatrix);
	D3DXMatrixIdentity(&g_scaleMatrix);
	D3DXMatrixIdentity(&g_matrix);	
	//在原点的矩阵旋转	先Z  再 Y	 后X 
	 

	//先绕Z轴旋转
	D3DXMatrixIdentity(&g_matrixTemp);
	D3DXMatrixRotationZ(&g_matrixTemp,  m_currRotate.z);
	D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);

	switch(m_pEmit->m_eParticalFaceState)
	{
	case EPFS_X:
		{	  
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationY(&g_matrixTemp,  _getRadian(90));
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);
			break;
		}
	case EPFS_Y:
		{
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationX(&g_matrixTemp,  _getRadian(90));
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);
			break;
		}
	case EPFS_Z:
		{
			//默认是Z轴朝向
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);

			break;
		}
	case EPFS_CAM:
		{	  
			D3DXVECTOR3 v = cam_pos - m_pos;
			D3DXVec3Normalize(&v, &v);
			float y_r = 0.0f;
			float r2 = 0.0f;
			D3DXVECTOR3 v2;
			//计算绕y轴旋转的弧度
			if(v.z == 0)
			{//正好绕Y轴旋转90°
				y_r = _getRadian(90);
				v2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			}
			else
			{
				y_r = atan(v.x/v.z);
				//修正 y_r = atan(v.x/v.z);只能得到 -PI/2 ~ PI/2之间的值 
				if(v.x > 0 && v.z > 0)
					y_r += PI;
				else if(v.x < 0 && v.z > 0)
					y_r -= PI;
			}
			if(v.x == 0 && v.z == 0)
			{
				r2 = _getRadian(90);
			}
			else
			{
				r2 = atan(v.y/sqrt(v.x*v.x + v.z*v.z));
			}
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationX(&g_matrixTemp,  r2);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);

			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationY(&g_matrixTemp,  y_r);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);
			break;
		}
	case EPFS_CAM_Y:
		{	  
			D3DXVECTOR3 v = cam_pos - m_pos;
 			D3DXVec3Normalize(&v, &v);
			float y_r = 0.0f;
			D3DXVECTOR3 v2;
			//计算绕y轴旋转的弧度
			if(v.z == 0)
			{//正好绕Y轴旋转90°
				y_r = _getRadian(90);
				v2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			}
			else
			{
				y_r = atan(v.x/v.z);
				//修正 y_r = atan(v.x/v.z);只能得到 -PI/2 ~ PI/2之间的值 
				if(v.x > 0 && v.z > 0)
					y_r += PI;
				else if(v.x < 0 && v.z > 0)
					y_r -= PI;
			}
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationY(&g_matrixTemp,  y_r);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);
			break;
		}
	case EPFS_V_V:
		{//始终和速度方向垂直
			//与面朝摄像机原理相同
			D3DXVECTOR3 v = m_vel;
			D3DXVec3Normalize(&v, &v);
			float y_r = 0.0f;
			float r2 = 0.0f;
			D3DXVECTOR3 v2;
			//计算绕y轴旋转的弧度
			if(v.z == 0)
			{//正好绕Y轴旋转90°
				y_r = _getRadian(90);
				v2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			}
			else
			{
				y_r = atan(v.x/v.z);
				//修正 y_r = atan(v.x/v.z);只能得到 -PI/2 ~ PI/2之间的值 
				if(v.x > 0 && v.z > 0)
					y_r += PI;
				else if(v.x < 0 && v.z > 0)
					y_r -= PI;
			}
			if(v.x == 0 && v.z == 0)
			{
				r2 = _getRadian(90);
			}
			else
			{
				r2 = atan(v.y/sqrt(v.x*v.x + v.z*v.z));
			}
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationX(&g_matrixTemp,  r2);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);

			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationY(&g_matrixTemp,  y_r);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);
			break;
		}
	case EPFS_F_V:
		{//始终和速度方向一致	  
			D3DXVECTOR3 v = m_vel;
			D3DXVec3Normalize(&v, &v);
			float y_r = 0.0f;
			float r2 = 0.0f;
			D3DXVECTOR3 v2;
			//计算绕y轴旋转的弧度
			if(v.z == 0)
			{//正好绕Y轴旋转90°
				y_r = _getRadian(90);
				v2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			}
			else
			{
				y_r = atan(v.x/v.z);
				//修正 y_r = atan(v.x/v.z);只能得到 -PI/2 ~ PI/2之间的值 
				if(v.x > 0 && v.z > 0)
					y_r += PI;
				else if(v.x < 0 && v.z > 0)
					y_r -= PI;
			}
			if(v.x == 0 && v.z == 0)
			{
				r2 = _getRadian(90);
			}
			else
			{
				r2 = atan(v.y/sqrt(v.x*v.x + v.z*v.z));
			}
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationX(&g_matrixTemp,  -PI / 2 + r2);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);

			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationY(&g_matrixTemp,  y_r);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);
			
			break;
		}
	case EPFS_F_V_L:
		{//左边始终和速度方向一致	(暂时绕Y旋转没问题 因为速度只支持绕Y轴旋转 其他待完善)  
			D3DXVECTOR3 v = m_vel;
			D3DXVec3Normalize(&v, &v);
			float y_r = 0.0f;
			float r2 = 0.0f;
			D3DXVECTOR3 v2;
			//计算绕y轴旋转的弧度
			if(v.z == 0)
			{//正好绕Y轴旋转90°
				y_r = _getRadian(90);
				v2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			}
			else
			{
				y_r = atan(v.x/v.z);
				//修正 y_r = atan(v.x/v.z);只能得到 -PI/2 ~ PI/2之间的值 
				if(v.x > 0 && v.z > 0)
					y_r += PI;
				else if(v.x < 0 && v.z > 0)
					y_r -= PI;
			}
			if(v.x == 0 && v.z == 0)
			{
				r2 = _getRadian(90);
			}
			else
			{
				r2 = atan(v.y/sqrt(v.x*v.x + v.z*v.z));
			}	   
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationY(&g_matrixTemp,  -PI/2 + y_r);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);

			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationX(&g_matrixTemp,  r2);								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);



			break;
		}
	case EPFS_FREE:
		{//自由旋转
			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationX(&g_matrixTemp,  (m_currRotate.x));								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);

			D3DXMatrixIdentity(&g_matrixTemp);
			D3DXMatrixRotationY(&g_matrixTemp,  (m_currRotate.y));								 
			D3DXMatrixMultiply(&g_rotateMatrix, &g_rotateMatrix, &g_matrixTemp);
			break;
		}
	}
	//平移到原点
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);	 
	//相乘
	D3DXMatrixScaling(&g_scaleMatrix, m_fScale, m_fScale, 1.0f);
	//乘上缩放矩阵  
	D3DXMatrixMultiply(&g_matrix, &g_scaleMatrix, &g_transMatrix);
	//旋转
	D3DXMatrixMultiply(&g_matrix, &g_matrix, &g_rotateMatrix);	 
	//最后平移到物体位置
	D3DXMatrixTranslation(&g_transMatrix, m_pos.x , m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_Matrix, &rotate, &trans);	
	D3DXMatrixMultiply(&g_matrix, &g_matrix, &g_transMatrix);
	return g_matrix;
}
void CDexPartical::Render()
{
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x01);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
}

CDexParticalEmit::CDexParticalEmit()
{ 
	m_EmitType = EET_NORMAL;
	m_pDexTex = NULL;
	m_iCurFrame = 0;
	m_eParticalFaceState = EPFS_CAM;
	m_createMaxSize = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
	m_createMinSize = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
	m_createPosMin = D3DXVECTOR3(0, 20, 0);
	m_createPosMax = D3DXVECTOR3(0, 20, 0);
	m_rotateInit =   D3DXVECTOR3(0, 0, 0);
	m_rotateVelMax = D3DXVECTOR3(0, 0, 0);
	m_rotateVelMin = D3DXVECTOR3(0, 0, 0);
	m_iParticalLiveTime = 100;
	m_iCreateLastParticalTime = 0;
	m_createInterval = 100;
	//m_EmitPos = D3DXVECTOR3(0, 0, 0);
	//m_EmitFaceVec = D3DXVECTOR3(0, 0, 0);
	m_velMin = D3DXVECTOR3(0, 0, -50);
	m_velMax = D3DXVECTOR3(0, 0, 50);
	m_acc = D3DXVECTOR3(0, 0, 0);	
	m_fMiddleTimeRate = 0.5f;
	m_fScaleStart = 1.0f;
	m_iTexFrameX  = 1;
	m_iTexFrameTotal  = 1;
	m_fScaleMiddle = 2.0f;
	m_fScaleEnd = 1.0f;
	m_startColor = DexColor(1.0f, 1.0f, 0,1.0f);
	m_middleColor = DexColor(1.0f, 1.0f, 0,1.0f);
	m_endColor = DexColor(1.0f, 1.0f, 0, 1.0f);
	m_SeeTexState = false;
	m_bAlphaSort = true; 
	///m_bFolloEmitRotate = false;
}
CDexParticalEmit::~CDexParticalEmit()
{
	_REDUCEREF(m_pDexTex);
	getParticalPool()->FreePartical(m_particalList.size());	  
	m_particalList.clear();
}


void CDexParticalEmit::SetCurrFrame(int16 frame_index)
{
	m_iCurFrame = frame_index;
}

void CDexParticalEmit::SetTexFile(string filename)
{
	_REDUCEREF(m_pDexTex);
	m_pDexTex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(filename.c_str());
	_ADDREF(m_pDexTex);
}

bool CDexParticalEmit::AddKeyFrame(int16 frame_index, const stFrame data)
{
	map<int16, stFrame>::iterator it = m_keyFrame.find(frame_index);
	DEX_ENSURE_B(it == m_keyFrame.end());
	m_keyFrame[frame_index] = data;
	return true;
}
bool CDexParticalEmit::AddKeyFrame(const stFrame& data)
{
	TMapFrame::iterator it = m_keyFrame.find(data.frame);
	DEX_ENSURE_B(it == m_keyFrame.end());
	m_keyFrame[data.frame] = data;
	return true;
}

bool CDexParticalEmit::AddKeyFrame(int16 frame_index, int16 count)
{
	map<int16, stFrame>::iterator it = m_keyFrame.find(frame_index);
	DEX_ENSURE_B(it == m_keyFrame.end());
	m_keyFrame[frame_index].frame = frame_index;
	m_keyFrame[frame_index].param1 = count;
	return true;
}

bool CDexParticalEmit::AddKeyFrameDur(int16 frame_start, int16 frame_end)
{
	map<int16, stFrame>::iterator it = m_keyFrame.find(frame_start);
	DEX_ENSURE_B(it == m_keyFrame.end());
	m_keyFrame[frame_start].frame = frame_start;
	m_keyFrame[frame_start].param1 = frame_end;
	return true;
}

void CDexParticalEmit::PreInitMemRes()
{
	//粒子数量
	int count = m_iParticalLiveTime/m_createInterval;
	getParticalPool()->CreatePartical(count);
}

void TransVector(D3DXVECTOR3& vec, const D3DXMATRIX& matrix)
{
	float old_x = vec.x;
	float old_y = vec.y;
	float old_z = vec.z;
	vec.x = old_x * matrix._11 + old_y * matrix._21 + old_z * matrix._31 + 1 * matrix._41;
	vec.y = old_x * matrix._12 + old_y * matrix._22 + old_z * matrix._32 + 1 * matrix._42;
	vec.z = old_x * matrix._13 + old_y * matrix._23 + old_z * matrix._33 + 1 * matrix._43;
}

void CDexParticalEmit::_UpdateNormal(int delta, const D3DXVECTOR3& cam_pos)
{
	if(getTime()->GetTotalMillSeconds() - m_iCreateLastParticalTime > m_createInterval)
	{
		m_iCreateLastParticalTime = getTime()->GetTotalMillSeconds(); 
		CDexPartical* partical = NULL;
		partical = getParticalPool()->GetPartical();
		_InitParticalData(partical);
		m_particalList.push_back(partical);
	}
}

void CDexParticalEmit::_UpdateExplode(int delta, const D3DXVECTOR3& cam_pos)
{
	map<int16, stFrame>::iterator it = m_keyFrame.find(m_iCurFrame);
	DEX_ENSURE(it != m_keyFrame.end());
	int16 count = it->second.param1;
	while(count > 0)
	{
		count--;
		m_iCreateLastParticalTime = getTime()->GetTotalMillSeconds(); 
		CDexPartical* partical = NULL;
		partical = getParticalPool()->GetPartical();
		_InitParticalData(partical);
		m_particalList.push_back(partical);
	}
}

void CDexParticalEmit::_UpdateFrameNormal(int delta, const D3DXVECTOR3& cam_pos)
{
	map<int16, stFrame>::iterator it = m_keyFrame.begin();
	bool find = false;
	for(; it != m_keyFrame.end(); it++)
	{
		if(m_iCurFrame >= it->second.frame &&
		   m_iCurFrame <= it->second.param1)
		{//当前帧有对应的特效
			find = true;
		}
	}
	DEX_ENSURE(find);
	if(getTime()->GetTotalMillSeconds() - m_iCreateLastParticalTime > m_createInterval)
	{
		m_iCreateLastParticalTime = getTime()->GetTotalMillSeconds(); 
		CDexPartical* partical = NULL;
		partical = getParticalPool()->GetPartical();
		_InitParticalData(partical);
		m_particalList.push_back(partical);
	}
}

void CDexParticalEmit::_InitParticalData(CDexPartical* partical)
{
	DEX_ENSURE(partical != NULL);
	partical->m_pEmit = this;
	partical->m_fScale = m_fScaleStart;
	partical->m_size.x = m_createMinSize.x + RandFloat()* (m_createMaxSize.x - m_createMinSize.x); 
	partical->m_size.y = m_createMinSize.y + RandFloat()* (m_createMaxSize.y - m_createMinSize.y); 
	partical->m_size.z = m_createMinSize.z + RandFloat()* (m_createMaxSize.z - m_createMinSize.z); 
	partical->m_pos.x = m_createPosMin.x + RandFloat()* (m_createPosMax.x - m_createPosMin.x);// + m_EmitPos.x; 
	
	partical->m_rotateVel.x =  m_rotateVelMin.x + RandFloat()* (m_rotateVelMax.x - m_rotateVelMin.x);
	partical->m_rotateVel.y =  m_rotateVelMin.y + RandFloat()* (m_rotateVelMax.y - m_rotateVelMin.y);
	partical->m_rotateVel.z =  m_rotateVelMin.z + RandFloat()* (m_rotateVelMax.z - m_rotateVelMin.z);
	partical->m_currRotate = m_rotateInit;
	partical->m_currRotate.x = _getRadian (partical->m_currRotate.x);
	partical->m_currRotate.y = _getRadian (partical->m_currRotate.y); 
	partical->m_currRotate.z = _getRadian (partical->m_currRotate.z); 
	
	partical->m_pos.y = m_createPosMin.y + RandFloat()* (m_createPosMax.y - m_createPosMin.y);// + m_EmitPos.y;
	partical->m_pos.z = m_createPosMin.z + RandFloat()* (m_createPosMax.z - m_createPosMin.z);// + m_EmitPos.z;

	//粒子被绑定在场景节点上 根据节点的最终矩阵进行相应的位置平移
	//partical->m_pos.x += getFinalMatrix()->_41;
	//partical->m_pos.y += getFinalMatrix()->_42;
	//partical->m_pos.z += getFinalMatrix()->_43;
	D3DXVec3TransformCoord(&partical->m_pos, &partical->m_pos, getFinalMatrix());

	partical->m_vel.x = m_velMin.x + RandFloat()*(m_velMax.x - m_velMin.x);
	partical->m_vel.y = m_velMin.y + RandFloat()*(m_velMax.y - m_velMin.y); 
	partical->m_vel.z = m_velMin.z + RandFloat()*(m_velMax.z - m_velMin.z);  

	D3DXVec3TransformNormal(&partical->m_vel, &partical->m_vel, getFinalMatrix());

	//缩放
	//partical->m_size.x *= m_pSceneNode->getFinalMatrix()._11;
	//partical->m_size.y *= m_pSceneNode->getFinalMatrix()._22;
	//partical->m_size.z *= m_pSceneNode->getFinalMatrix()._33;
	
	/*
	if(m_bFolloEmitRotate)
	{//如果跟随发射器旋转
		if(m_EmitFaceVec.z == 0)
		{
			float theta = 0.0f;
			if(m_EmitFaceVec.x == 0)
			{
				if(m_EmitFaceVec.y > 0)
				{
					partical->m_vel.y = -m_velMax.z;
				}
				else 
				{
					partical->m_vel.y = m_velMax.z;
				}
			}
			else
			{
				theta = atan(m_EmitFaceVec.y/m_EmitFaceVec.x);
				if(m_EmitFaceVec.y > 0 && m_EmitFaceVec.x > 0)
					theta += PI;
				else if(m_EmitFaceVec.y < 0 && m_EmitFaceVec.x> 0)
					theta -= PI;
				partical->m_vel.x = m_velMax.z *cos(theta);
				partical->m_vel.y = m_velMax.z *sin(theta);
				partical->m_vel.z = 0;
			}
		}
		else
		{
			float theta1 = atan(m_EmitFaceVec.x/m_EmitFaceVec.z);	 
			if(m_EmitFaceVec.x > 0 && m_EmitFaceVec.z > 0)
				theta1 += PI;
			else if(m_EmitFaceVec.x < 0 && m_EmitFaceVec.z> 0)
				theta1 -= PI;
			float theta2 = 0;
			theta2 = atan(m_EmitFaceVec.y/sqrt(m_EmitFaceVec.x*m_EmitFaceVec.x + m_EmitFaceVec.z*m_EmitFaceVec.z));
			if(m_EmitFaceVec.y > 0)
				theta2 += PI;
			else if(m_EmitFaceVec.y < 0)
				theta2 -= PI;
			partical->m_vel.x = m_velMax.z * sin(theta1) * cos(theta2);
			partical->m_vel.y = m_velMax.z * sin(theta2);
			partical->m_vel.z = m_velMax.z * cos(theta1) * cos(theta2);
		}
	}
	else
	{
		partical->m_vel.x = m_velMin.x + RandFloat()*(m_velMax.x - m_velMin.x);
		partical->m_vel.y = m_velMin.y + RandFloat()*(m_velMax.y - m_velMin.y); 
		partical->m_vel.z = m_velMin.z + RandFloat()*(m_velMax.z - m_velMin.z);  

		D3DXMATRIX rotatey;
		D3DXMatrixIdentity(&rotatey);
		//D3DXVECTOR3 direction = m_pSceneNode->getDirection();
		D3DXVECTOR3 direction = getDirection();
		direction.y = 0;

		D3DXVECTOR3 ini(0, 0, -1);
		float dot =D3DXVec3Dot(&ini, &direction);
		float cos_radius = dot;  //因为两个皆是单位向量，故除模省去
		float angle_minus =	acos(cos_radius);
		if(DecideISClock(ini, direction))
		{
			D3DXMatrixRotationY(&rotatey, angle_minus);
		}
		else
		{
			D3DXMatrixRotationY(&rotatey, -angle_minus);
		} 
		TransVector(partical->m_vel, rotatey);
		//TransVector(partical->m_currRotate, rotatey);
	}
	*/

	partical->m_acc = m_acc;
	partical->m_color = m_startColor;
	partical->m_iCurrLiveTime = 0;
}

bool CDexParticalEmit::Update(int delta)
{
	DEX_ENSURE_B(CDexSceneObject::Update(delta));
	D3DXVECTOR3 cam_pos = DexGameEngine::getEngine()->getCamera()->GetPosition();
	switch(m_EmitType)
	{
	case EET_NORMAL:
		{
			_UpdateNormal(delta, cam_pos);
			break;
		}
	case EET_EXPLODE:
		{
			_UpdateExplode(delta, cam_pos);
			break;
		}
	case EET_FRAME_NORMAL:
		{
			_UpdateFrameNormal(delta, cam_pos);
			break;
		}
	}
	foreach(std::list<CDexPartical*>, ite, m_particalList)
	{
		(*ite)->Update(delta, cam_pos);
		if((*ite)->LiveOver())
		{
			getParticalPool()->PushPartical(*ite);
			ite = m_particalList.erase(ite);
			if(ite == m_particalList.end())
				break;
			continue;
		}
	}
	return true;
}

void CDexParticalEmit::Reset()
{
	_REDUCEREF(m_pDexTex);
	getParticalPool()->FreePartical(m_particalList.size());	  
	m_keyFrame.clear();
	m_particalList.clear();
}
bool CDexParticalEmit::Render()
{
	DEX_ENSURE_B(CDexSceneObject::Render());
	D3DXVECTOR3 cam_pos = DexGameEngine::getEngine()->getCamera()->GetPosition();
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);	  //必须禁止深度缓冲的写入
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(m_SeeTexState)
		DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	else
		DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );	
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DexGameEngine::getEngine()->GetDevice()->SetSamplerState(0,   D3DSAMP_MIPFILTER,   D3DTEXF_LINEAR);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1 );	    
	bool fog = DexGameEngine::getEngine()->OpenFog();
	if(fog)
		DexGameEngine::getEngine()->DisableFog();
	if(m_pDexTex != NULL)
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, m_pDexTex->GetTexPt());
	else
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
	foreach(std::list<CDexPartical*>, ite, m_particalList)
	{		     
		g_pVertexList1[0].m_pos = D3DXVECTOR3(-(*ite)->m_size.x/2, (*ite)->m_size.y/2, 0.0f);
		g_pVertexList1[1].m_pos = D3DXVECTOR3((*ite)->m_size.x/2, (*ite)->m_size.y/2, 0.0f);
		g_pVertexList1[2].m_pos = D3DXVECTOR3(-(*ite)->m_size.x/2, -(*ite)->m_size.y/2, 0.0f);
		g_pVertexList1[3].m_pos = D3DXVECTOR3((*ite)->m_size.x/2, -(*ite)->m_size.y/2, 0.0f);
		
		g_pVertexList1[0].m_color = getD3DColor((*ite)->m_color);
		g_pVertexList1[0].m_u = (*ite)->m_fU1;
		g_pVertexList1[0].m_v = (*ite)->m_fV1;
		g_pVertexList1[1].m_color = getD3DColor((*ite)->m_color);
		g_pVertexList1[1].m_u = (*ite)->m_fU2;
		g_pVertexList1[1].m_v = (*ite)->m_fV1;
		g_pVertexList1[2].m_color = getD3DColor((*ite)->m_color);
		g_pVertexList1[2].m_u = (*ite)->m_fU1;
		g_pVertexList1[2].m_v = (*ite)->m_fV2;
		g_pVertexList1[3].m_color = getD3DColor((*ite)->m_color);
		g_pVertexList1[3].m_u = (*ite)->m_fU2;
		g_pVertexList1[3].m_v = (*ite)->m_fV2;
			
		DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &((*ite)->GetMatrix(cam_pos)));
		DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_pVertexList1, sizeof(stVertex1));
	}
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &g_transMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);		  //状态恢复
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	if(fog)
		DexGameEngine::getEngine()->EnableFog();
	return true;
}

void CDexParticalEmit::SetPos(const D3DXVECTOR3& pos)
{
	//m_EmitPos = pos;
}
bool CDexParticalEmit::ArchiveIn(DexMem& mem)
{
	//mem.BeginRead();
	//暂时archive的时候根据位置 为emit新建一个场景node进行绑定
	mem>>m_EmitType;
	int size = 0;
	mem>>size;
	stFrame frameInfo;
	for(int i = 0 ; i < size; i++)
	{
		mem>>frameInfo;
		AddKeyFrame(frameInfo);
	}

	char texname[64];
	mem.ReadString(texname);
	SetTexFile(texname);
	mem>>m_iTexFrameX;
	mem>>m_iTexFrameTotal;
	mem>>m_eParticalFaceState;
	//mem>>m_EmitPos;
	//mem>>m_EmitFaceVec;
	mem>>m_createMaxSize;
	mem>>m_createMinSize;
	mem>>m_createPosMin;
	mem>>m_createPosMax;
	mem>>m_rotateInit;
	mem>>m_rotateVelMax;
	mem>>m_rotateVelMin;
	mem>>m_iParticalLiveTime;
	mem>>m_createInterval;
	mem>>m_velMin;
	mem>>m_velMax;
	mem>>m_acc;
	mem>>m_fScaleStart;
	mem>>m_fScaleMiddle;
	mem>>m_fScaleEnd;
	mem>>m_fMiddleTimeRate;
	//mem>>m_bFolloEmitRotate;
	mem>>m_SeeTexState;
	mem>>m_startColor;
	mem>>m_middleColor;
	mem>>m_endColor;

	return true;
}

bool CDexParticalEmit::ArchiveOut(DexMem& mem) const
{
	//mem.BeginWrite();
	mem<<m_EmitType;
	mem<<m_keyFrame.size();
	for(TMapFrame::const_iterator ite = m_keyFrame.begin(); ite != m_keyFrame.end(); ++ite)
	//for(TMapFrame::iterator ite = m_keyFrame.begin(); ite != m_keyFrame.end(); ite++)
	{
		mem<<ite->second;
	}
	if(m_pDexTex != NULL)
	{   
		string texname = m_pDexTex->FileName();
		mem.WriteString((char*)texname.c_str());
	}
	else
		mem.WriteString(" ");
	mem<<m_iTexFrameX;
	mem<<m_iTexFrameTotal;
	mem<<m_eParticalFaceState;
	//mem<<m_EmitPos;
	//mem<<m_EmitFaceVec;
	mem<<m_createMaxSize;
	mem<<m_createMinSize;
	mem<<m_createPosMin;
	mem<<m_createPosMax;
	mem<<m_rotateInit;
	mem<<m_rotateVelMax;
	mem<<m_rotateVelMin;
	mem<<m_iParticalLiveTime;
	mem<<m_createInterval;
	mem<<m_velMin;
	mem<<m_velMax;
	mem<<m_acc;
	mem<<m_fScaleStart;
	mem<<m_fScaleMiddle;
	mem<<m_fScaleEnd;
	mem<<m_fMiddleTimeRate;
	//mem<<m_bFolloEmitRotate;
	mem<<m_SeeTexState;
	mem<<m_startColor;
	mem<<m_middleColor;
	mem<<m_endColor;
	return true;
}

bool CDexParticalEmit::Archive(DexMem& mem, bool mode)
{
	if(mode == DexMem::MEM_READ)
	{
		return ArchiveIn(mem);
	}
	else
		return ArchiveOut(mem);
}

bool CDexParticalEmit::saveToInitFile(const char* filename)
{
	getLog()->BeginLog();

	CFile* file = getFileHandler();
	if(!file->CreateCFile((char*)filename))
	{
		getLog()->Log(log_allert, "save particle file % failed！\n", filename);
		getLog()->EndLog();
		return false;
	}
	file->InNumber((int)m_eParticalFaceState);file->InBlankChar();
	file->InNumber((int)m_EmitType);
	file->InBlankLine();

	file->InNumber(m_createMaxSize.x, 1);file->InBlankChar();
	file->InNumber(m_createMaxSize.y, 1);file->InBlankChar();
	file->InNumber(m_createMaxSize.z, 1);file->InBlankChar();
	file->InString("--maxsize");
	file->InBlankLine();

	file->InNumber(m_createMinSize.x, 1);file->InBlankChar();
	file->InNumber(m_createMinSize.y, 1);file->InBlankChar();
	file->InNumber(m_createMinSize.z, 1);file->InBlankChar();
	file->InString("--minsize");
	file->InBlankLine();

	file->InNumber(m_createPosMax.x, 1);file->InBlankChar();
	file->InNumber(m_createPosMax.y, 1);file->InBlankChar();
	file->InNumber(m_createPosMax.z, 1);file->InBlankChar();
	file->InString("--maxpos");
	file->InBlankLine();

	file->InNumber(m_createPosMin.x, 1);file->InBlankChar();
	file->InNumber(m_createPosMin.y, 1);file->InBlankChar();
	file->InNumber(m_createPosMin.z, 1);file->InBlankChar();
	file->InString("--minpos");
	file->InBlankLine();

	file->InNumber(m_velMax.x, 1);file->InBlankChar();
	file->InNumber(m_velMax.y, 1);file->InBlankChar();
	file->InNumber(m_velMax.z, 1);file->InBlankChar();
	file->InString("--velmax");
	file->InBlankLine();

	file->InNumber(m_velMin.x, 1);file->InBlankChar();
	file->InNumber(m_velMin.y, 1);file->InBlankChar();
	file->InNumber(m_velMin.z, 1);file->InBlankChar();
	file->InString("--velmin");
	file->InBlankLine();

	file->InNumber(m_acc.x, 1);file->InBlankChar();
	file->InNumber(m_acc.y, 1);file->InBlankChar();
	file->InNumber(m_acc.z, 1);file->InBlankChar();
	file->InString("--acc");
	file->InBlankLine();

	file->InNumber(m_startColor.a, 1);file->InBlankChar();
	file->InNumber(m_startColor.r, 1);file->InBlankChar();
	file->InNumber(m_startColor.g, 1);file->InBlankChar();
	file->InNumber(m_startColor.b, 1);file->InBlankChar();
	file->InString("--startcolor argb");
	file->InBlankLine();

	file->InNumber(m_middleColor.a, 1);file->InBlankChar();
	file->InNumber(m_middleColor.r, 1);file->InBlankChar();
	file->InNumber(m_middleColor.g, 1);file->InBlankChar();
	file->InNumber(m_middleColor.b, 1);file->InBlankChar();
	file->InString("--middlecolor argb");
	file->InBlankLine();

	file->InNumber(m_endColor.a, 1);file->InBlankChar();
	file->InNumber(m_endColor.r, 1);file->InBlankChar();
	file->InNumber(m_endColor.g, 1);file->InBlankChar();
	file->InNumber(m_endColor.b, 1);file->InBlankChar();
	file->InString("--endcolor argb");
	file->InBlankLine();

	if(m_pDexTex != NULL && m_pDexTex->GetTexPt() != NULL)
		file->InString((char*)m_pDexTex->FileName().c_str());
	else
		file->InString("NULL");
	file->InBlankChar();
	file->InString("--texture");
	file->InBlankLine();

	file->InNumber(m_iTexFrameX);file->InBlankChar();
	file->InNumber(m_iTexFrameTotal);file->InBlankChar();
	file->InString("--texframe x total");
	file->InBlankLine();

	file->InNumber(m_iParticalLiveTime);file->InBlankChar();
	file->InString("--livetime");
	file->InBlankLine();

	file->InNumber(m_createInterval);file->InBlankChar();
	file->InString("--createinterval");
	file->InBlankLine();

	file->InNumber(m_fMiddleTimeRate, 1);file->InBlankChar();
	file->InString("--middlerate");
	file->InBlankLine();

	file->InNumber(m_fScaleStart, 1);file->InBlankChar();
	file->InString("--scalestart");
	file->InBlankLine();

	file->InNumber(m_fScaleMiddle, 1);file->InBlankChar();
	file->InString("--scalemiddle");
	file->InBlankLine();

	file->InNumber(m_fScaleEnd, 1);file->InBlankChar();
	file->InString("--scaleend");
	file->InBlankLine();

	file->Close();
	getLog()->Log(log_ok, "save particle file %s successfully！\n", filename);
	getLog()->EndLog();
	return true;
}

bool CDexParticalEmit::initFromInitFile(const char* filename)
{
	getLog()->BeginLog();
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		getLog()->Log(log_allert, "open particle init file %s failed!\n",filename );
		getLog()->EndLog();
		pScript->CloseScript();
		return false;
	}
	char str[128];
	pScript->SetCurrChar();

	m_eParticalFaceState = (EParticalFaceState)pScript->GetIntParam();m_EmitType = (EEmitType)pScript->GetIntParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_createMaxSize.x = pScript->GetFloatParam();m_createMaxSize.y = pScript->GetFloatParam();m_createMaxSize.z = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_createMinSize.x = pScript->GetFloatParam();m_createMinSize.y = pScript->GetFloatParam();m_createMinSize.z = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_createPosMax.x = pScript->GetFloatParam();m_createPosMax.y = pScript->GetFloatParam();m_createPosMax.z = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_createPosMin.x = pScript->GetFloatParam();m_createPosMin.y = pScript->GetFloatParam();m_createPosMin.z = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_velMax.x = pScript->GetFloatParam();m_velMax.y = pScript->GetFloatParam();m_velMax.z = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_velMin.x = pScript->GetFloatParam();m_velMin.y = pScript->GetFloatParam();m_velMin.z = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_acc.x = pScript->GetFloatParam();m_acc.y = pScript->GetFloatParam();m_acc.z = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_startColor.a = pScript->GetFloatParam();m_startColor.r = pScript->GetFloatParam();m_startColor.g = pScript->GetFloatParam();m_startColor.b = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_middleColor.a = pScript->GetFloatParam();m_middleColor.r = pScript->GetFloatParam();m_middleColor.g = pScript->GetFloatParam();m_middleColor.b = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_endColor.a = pScript->GetFloatParam();m_endColor.r = pScript->GetFloatParam();m_endColor.g = pScript->GetFloatParam();m_endColor.b = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	pScript->GetStringParam(str);
	pScript->MoveCurrLine();pScript->SetCurrChar();
	if(strcmp(str, "NULL") != 0)
	{
		SetTexFile(str);
	}
	m_iTexFrameX = pScript->GetIntParam();m_iTexFrameTotal = pScript->GetIntParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_iParticalLiveTime = pScript->GetIntParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_createInterval = pScript->GetIntParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_fMiddleTimeRate = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_fScaleStart = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_fScaleMiddle = pScript->GetFloatParam();
	pScript->MoveCurrLine();pScript->SetCurrChar();

	m_fScaleEnd = pScript->GetFloatParam();
	//pScript->MoveCurrLine();pScript->SetCurrChar();
	pScript->CloseScript();
	getLog()->Log(log_ok, "emitter init file: %s ok!\n ", filename);
	return true;
}

//-----------------------CDexEffectInstance------------------
CDexEffectInstance::CDexEffectInstance()
{
	m_pEffectBeginNotify = NULL;
	m_bCycle = true;
	m_bEnd = false;
}
CDexEffectInstance::~CDexEffectInstance()
{

}
void CDexEffectInstance::RegisterNotify(CDexEffectNotify* notify)
{
	 m_pEffectBeginNotify = notify;
}

bool CDexEffectInstance::Update(int delta)
{
	DEX_ENSURE_B(CDexSceneObject::Update(delta));
	DEX_ENSURE_B(!m_bEnd);
	return true;
}

bool CDexEffectInstance::Render()
{
	DEX_ENSURE_B(CDexSceneObject::Render());
	return true;
}

void CDexEffectInstance::setCycle(bool cycle)
{
	m_bCycle = cycle;
}

bool CDexEffectInstance::getCycle() const
{
	return m_bCycle;
}

bool CDexEffectInstance::getEnd() const
{
	return m_bEnd;
}

//---------------------CDexEffectEmitInstance------------------
CDexEffectEmitInstance::CDexEffectEmitInstance()
{
	m_bAlphaSort = true;
	m_iPerFrameTime = 1000/60;
	m_iTotalFrames = 1;
	m_iTotalTime = 0;
}
CDexEffectEmitInstance::~CDexEffectEmitInstance()
{
	list<CDexParticalEmit*>::iterator it =m_EmiterList.begin();
	for(; it != m_EmiterList.end(); ++it)
	{
	    (*it)->Release();
	}
}


void CDexEffectEmitInstance::AddEmitter(CDexParticalEmit* emitter)
{
	DEX_ENSURE(emitter != NULL);
	emitter->bindSceneNode(this);
	m_EmiterList.push_back(emitter);
}


bool CDexEffectEmitInstance::Update(int delta)
{
	DEX_ENSURE_B(CDexEffectInstance::Update(delta));
	DEX_ENSURE_B(!m_bEnd);
	m_iTotalTime += delta;
	//如果一个特效反复播的话 要取得对于一轮来说经过的时间
	int one_cycle_time = m_iTotalTime % (m_iPerFrameTime* m_iTotalFrames);
	int curr_frame = one_cycle_time/m_iPerFrameTime;
	if(curr_frame == 0)
	{
		if(m_pEffectBeginNotify != NULL)
			m_pEffectBeginNotify->OnEffectBegin(this);
	}
	if(curr_frame >= m_iTotalFrames-1)
	{
		if(m_pEffectBeginNotify != NULL)
			m_pEffectBeginNotify->OnEffectEnd(this);
		if(!m_bCycle)
		{
			m_bEnd = true;
			curr_frame++; //因为结束了，所以最后一帧一直对应最有一个特效，导致最后一个特效一直有效，这里置为一个非法frame，
			//这样所有发射器就不再继续发射粒子了
		}

	}
	list<CDexParticalEmit*>::iterator it =m_EmiterList.begin();
	for(; it != m_EmiterList.end(); it++)
	{
		(*it)->SetCurrFrame(curr_frame);
		//(*it)->Update(delta);
	}
	return true;
}

void CDexEffectEmitInstance::StartFromBegin()
{
	m_iTotalTime = 0;
	m_bEnd = false;
}

bool CDexEffectEmitInstance::Render()
{
	DEX_ENSURE_B(CDexEffectInstance::Render());
	//list<CDexParticalEmit*>::iterator it =m_EmiterList.begin();
	//for(; it != m_EmiterList.end(); it++)
	//{
	//	(*it)->Render();
	//}
	return true;
}

void CDexEffectEmitInstance::Reset()
{
	//m_iPerFrameTime = 1000/60;
	///m_iTotalFrames = 1;
	m_iTotalTime = 0;
	list<CDexParticalEmit*>::iterator it =m_EmiterList.begin();
	for(; it != m_EmiterList.end();)
	{
		(*it)->Release();
	}
}

bool CDexEffectEmitInstance::ArchiveOut(DexMem& mem)const
{
	mem<<(int)m_EmiterList.size();
	list<CDexParticalEmit*>::const_iterator it =m_EmiterList.begin();
	for(; it != m_EmiterList.end(); ++it)
	{
		(*it)->Archive(mem, DexMem::MEM_WRITE);
	}
	return true;
}
bool CDexEffectEmitInstance::ArchiveIn(DexMem& mem)
{
	int emitterCount = 0;
	mem>>emitterCount;
	CDexParticalEmit* emitter = NULL;
	for(int i = 0; i < emitterCount; i++)
	{
		emitter = new CDexParticalEmit;
		emitter->Archive(mem, DexMem::MEM_READ);
		emitter->PreInitMemRes();
		AddEmitter(emitter);
	}
	return true;
}

bool CDexEffectEmitInstance::Archive(DexMem& mem, bool mode)
{
	if(mode == DexMem::MEM_READ)
	{
		return ArchiveIn(mem);
	}
	else
		return ArchiveOut(mem);
}

bool CDexEffectEmitInstance::LoadInitFile(const char* filename)
{
	getLog()->BeginLog();
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		getLog()->Log(log_allert, "open emitInstance init file %s failed!\n",filename );
		getLog()->EndLog();
		pScript->CloseScript();
		return false;
	}
	getLog()->Log(log_ok, "begin loading emitInstance init file %s !\n",filename );
	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	pScript->SetCurrChar();
	m_iTotalFrames = pScript->GetIntParam();
	m_iPerFrameTime = pScript->GetIntParam();
	pScript->MoveCurrLine();
	char str[128];
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar() ;
		CurrChar = pScript->GetCurrChar();
		CurrLine = pScript->GetCurrLine(); 
		if(strlen(Script[CurrLine]) == 1 || 
			(Script[CurrLine][CurrChar] == '-' 
			&& Script[CurrLine][CurrChar+1] == '-'))  	//略过注释或空白行
			continue;
		pScript->GetStringParam(str);
		if(strcmp(str, "emitter") == 0)
		{//emitter
			pScript->GetStringParam(str);
			CDexParticalEmit* emitter = Dex_QueryObject(CDexParticalEmit);
			DEX_ENSURE_B(emitter);
			emitter->initFromInitFile(str);
			emitter->SetParticalFace((EParticalFaceState)pScript->GetIntParam());
			emitter->m_EmitType = (EEmitType)pScript->GetIntParam();
			int index = pScript->GetIntParam();
			int count = pScript->GetIntParam();
			emitter->AddKeyFrame(index, count);
			D3DXVECTOR3 trans(pScript->GetIntParam(), pScript->GetIntParam(), pScript->GetIntParam());
			int degreex = pScript->GetIntParam();
			int degreey = pScript->GetIntParam();
			int degreez = pScript->GetIntParam();
			if( degreex != 0)
			{
				emitter->setLocalMatrix(trans, D3DXVECTOR3(1.0f, 1.0f, 1.0f), AXIS_X, _getRadian(degreex));
			}
			else if( degreey != 0)
			{
				emitter->setLocalMatrix(trans, D3DXVECTOR3(1.0f, 1.0f, 1.0f), AXIS_Y, _getRadian(degreey));
			}
			else if( degreez != 0)
			{
				emitter->setLocalMatrix(trans, D3DXVECTOR3(1.0f, 1.0f, 1.0f), AXIS_Z, _getRadian(degreez));
			}
			else
			{
				emitter->setLocalMatrix(trans);
			}
			pScript->GetStringParam(str);
			if(strcmp(str, "NULL") != 0)
			{
				char actionType[64];
				pScript->GetStringParam(actionType);
				if(strcmp(actionType, "catmull") == 0)
				{
					DexSceneObjectActionCatmull* action = Dex_QueryObject(DexSceneObjectActionCatmull);
					action->LoadInitFile(str);
					float time = pScript->GetFloatParam();
					float factor = pScript->GetFloatParam();
					action->setCatmullSpeed(time);
					action->setCatmullFactor(factor);
					emitter->addAction(action);
				}
				else if(strcmp(actionType, "rotate") == 0)
				{
					DexSceneObjectActionRotate* action = Dex_QueryObject(DexSceneObjectActionRotate);
					action->LoadInitFile(str);
					emitter->addAction(action);
				}

			}
			emitter->PreInitMemRes();
			AddEmitter(emitter);
			getLog()->Log(log_ok, "		emitInstance add emitter ok!\n");
		}
	}
	getLog()->Log(log_ok, "load emitInstance init file %s ok!\n",filename );
	getLog()->EndLog();
	pScript->CloseScript();
	return true;
}
//=================================================================


CDexClassParticalPool::CDexClassParticalPool()
{
	m_particalListFree.clear();
	m_pBuffer = NULL;


	if(FAILED(DexGameEngine::getEngine()->GetDevice()->CreateVertexBuffer(4 * sizeof(stVertex1),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_pBuffer, NULL))) return ;

	void* ptr;
	m_pParticalList[0] = stVertex1(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), DexColor(0,0.5f,0.5f,0.5f),0.0f, 0.0f ); 
	m_pParticalList[1] = stVertex1(D3DXVECTOR3(0.5f, 0.5f, 0.0f),  DexColor(0,0.5f,0.5f,0.5f), 1.0f, 0.0f ); 
	m_pParticalList[2] = stVertex1(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), DexColor(0,0.5f,0.5f,0.5f), 0.0f, 1.0f ); 
	m_pParticalList[3] = stVertex1(D3DXVECTOR3(0.5f, -0.5f, 0.0f),  DexColor(0,0.5f,0.5f,0.5f), 1.0f, 1.0f ); 
	m_pBuffer->Lock(0, sizeof(m_pParticalList),(void**)&ptr,0);
	memcpy(ptr, m_pParticalList, sizeof(m_pParticalList));
	m_pBuffer->Unlock();
}
CDexClassParticalPool::~CDexClassParticalPool()
{
	_SafeRelease(m_pBuffer);
	//释放粒子内存
	foreach(std::list<CDexPartical*>, ite, m_particalListFree)
	{
		if(*ite != NULL)
		{
			delete *ite;
			*ite = NULL;
			ite = m_particalListFree.erase(ite);
			if(ite == m_particalListFree.end())
			{
				break;
			}
		}
	}
}

void CDexClassParticalPool::CreatePartical(int iCount)
{		
	int64 time1 = getTime()->GetTotalMillSeconds();
	for(int i = 0 ; i < iCount; i++)
	{
		CDexPartical* partical = new CDexPartical;
		m_particalListFree.push_back(partical);
	}
	getLog()->BeginLog();
	getLog()->Log(log_ok, "申请%d个粒子...，耗时%d ms", iCount, getTime()->GetTotalMillSeconds()- time1);
	getLog()->EndLog();
}

void CDexClassParticalPool::FreePartical(int iCount)
{
	int64 time1 = getTime()->GetTotalMillSeconds();
	int i = 0;
	foreach(std::list<CDexPartical*>, ite, m_particalListFree)
	{	 
		if(i == iCount)
			break;
		if(*ite != NULL)
		{
			delete *ite;
			*ite = NULL;
			ite = m_particalListFree.erase(ite);
			if(ite == m_particalListFree.end())
			{
				break;
			}
			i++;
		}
	}
	getLog()->BeginLog();
	getLog()->Log(log_ok, "释放%d个粒子...，耗时%d ms", iCount, getTime()->GetTotalMillSeconds()- time1);
	getLog()->EndLog();
}

CDexPartical* CDexClassParticalPool::GetPartical()
{
	CDexPartical* partical = NULL;
	if(m_particalListFree.size() == 0)
	{//粒子池中没有粒子了 新建(这种情况不应该出现 因为所有要求的粒子内存都已经预分配了 警告！)
		getLog()->BeginLog();
		getLog()->Log(log_error, "粒子池创建了一个不该创建的粒子，警告！");
		getLog()->EndLog();
		partical = new CDexPartical;
		return partical;
	}
	else
	{
		partical = *(m_particalListFree.begin());
		m_particalListFree.pop_front();
		return partical;
	}

}

void CDexClassParticalPool::PushPartical(CDexPartical* pPartical)
{
	DEX_ENSURE(pPartical);
	m_particalListFree.push_back(pPartical);
}

CDexClassParticalPool* getParticalPool()
{
	static CDexClassParticalPool* g_pParticalPool = NULL ;
	if(g_pParticalPool == NULL)
		g_pParticalPool = new CDexClassParticalPool;
	return g_pParticalPool;
}

//===============CDexPieceEffectInstance=======================================

CDexPieceEffectInstance::_stPiece::_stPiece()
{
	for(int i = 0 ; i < 4; i++)
	{
		point[i] = D3DXVECTOR3(0,0,0);
		m_color = DEXCOLOR_WHITE;
		instance = NULL;
		alreadyTime = 0.0f;
	}
}

void CDexPieceEffectInstance::_stPiece::Update(int delta)
{
	alreadyTime += delta;
	if(alreadyTime <= instance->m_iPieceLiveTime * instance->m_fMiddleTimeRate)
	{
		m_color.a += delta * (instance->m_middleColor.a - instance->m_startColor.a)/(instance->m_iPieceLiveTime* instance->m_fMiddleTimeRate);
		m_color.r += delta * (instance->m_middleColor.r - instance->m_startColor.r)/(instance->m_iPieceLiveTime* instance->m_fMiddleTimeRate);
		m_color.g += delta * (instance->m_middleColor.g - instance->m_startColor.g)/(instance->m_iPieceLiveTime* instance->m_fMiddleTimeRate);
		m_color.b += delta * (instance->m_middleColor.b - instance->m_startColor.b)/(instance->m_iPieceLiveTime* instance->m_fMiddleTimeRate);
	}
	else if(alreadyTime <= instance->m_iPieceLiveTime)
	{
		m_color.a += delta * (instance->m_endColor.a - instance->m_middleColor.a)/(instance->m_iPieceLiveTime* (1 - instance->m_fMiddleTimeRate));
		m_color.r += delta * (instance->m_endColor.r - instance->m_middleColor.r)/(instance->m_iPieceLiveTime* (1 - instance->m_fMiddleTimeRate));
		m_color.g += delta * (instance->m_endColor.g - instance->m_middleColor.g)/(instance->m_iPieceLiveTime* (1 - instance->m_fMiddleTimeRate));
		m_color.b += delta * (instance->m_endColor.b - instance->m_middleColor.b)/(instance->m_iPieceLiveTime* (1 - instance->m_fMiddleTimeRate));
	}
}

CDexPieceEffectInstance::_stPieceControlPoint::_stPieceControlPoint()
{
	pnt1 = D3DXVECTOR3(0,0,0);
	pnt2 = D3DXVECTOR3(0,0,0);
}

CDexPieceEffectInstance::CDexPieceEffectInstance()
{
	m_bAlphaSort = true;
	m_pDexTex = NULL;
	m_fFactor = 0.8f;
	m_fSpeed = 1.0f;
	m_fCurrPosRate = 0.0f;
	m_fLastPosRate = 0.0f;
	m_fMiddleTimeRate = 0.5f;
	m_iPieceLiveTime = 1000;
	m_startColor = DEXCOLOR_WHITE;
	m_middleColor = DEXCOLOR_WHITE;
	m_endColor = DEXCOLOR_WHITE;
	m_bRenderPieceRoute = true;
}
CDexPieceEffectInstance::~CDexPieceEffectInstance()
{
	getPiecePool()->FreePiece(m_listPiece.size());
	m_listPiece.clear();
	_REDUCEREF(m_pDexTex);
	m_vecControlPoints.clear();
}

void CDexPieceEffectInstance::Reset()
{
	m_fSpeed = 1.0f;
	m_fCurrPosRate = 0.0f;
	m_fLastPosRate = 0.0f;
	m_fMiddleTimeRate = 0.5f;
	m_iPieceLiveTime = 1000;
	m_startColor = DEXCOLOR_WHITE;
	m_middleColor = DEXCOLOR_WHITE;
	m_endColor = DEXCOLOR_WHITE;
	m_bRenderPieceRoute = true;

	getPiecePool()->FreePiece(m_listPiece.size());
	m_listPiece.clear();
	_REDUCEREF(m_pDexTex);
	m_vecControlPoints.clear();
}
bool CDexPieceEffectInstance::Update(int delta)
{
	DEX_ENSURE_B(CDexEffectInstance::Update(delta));
	foreach(TListPiece, ite, m_listPiece)
	{
		(*ite)->Update(delta);
		if((*ite)->alreadyTime > m_iPieceLiveTime)
		{
			getPiecePool()->Push(*ite);
			ite = m_listPiece.erase(ite);
			if(ite == m_listPiece.end())
				break;
			continue;
		}
	}
	if(m_vecControlPoints.size() >= 4)
	{//最少要有4个点才能决定一条曲线段
		float vel = 1.0f/(1000.0f*m_fSpeed);
		m_fCurrPosRate += delta*vel;
		if(m_fCurrPosRate >= 1.0f)
		{
			if(m_bCycle)
			{
				m_fCurrPosRate = 0.0f;
			}
			else
			{
				m_bEnd = true;
			}
		}
		float f0 = 1.0f/(m_vecControlPoints.size()-3);  //每一小段对应的增加量 n = 7  则分四段 每段 f0 = 0.25f
		
		int pnt1_index = m_fCurrPosRate/f0;    //curr_delta所在线段的前一点 curr_delta = 0.5f时  pnt1_index = 0.5f/0.25f = 2 即正好落在第四个点上
		float fm = (m_fCurrPosRate - f0*pnt1_index)/f0;  //当前所在的线段内的偏移量 curr_delta = 0.6f fm =(0.6-0.25*2)/0.25 = 0.4 即在当前区间的0.4分量上
		pnt1_index += 1; //因为第一个点不参加计算 所以向后偏移一位

		//此帧的两点
		D3DXVECTOR3 pos21 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[pnt1_index-1].pnt1, m_vecControlPoints[pnt1_index].pnt1,
			m_vecControlPoints[pnt1_index+1].pnt1,m_vecControlPoints[pnt1_index+2].pnt1,fm, m_fFactor);
		D3DXVECTOR3 pos22 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[pnt1_index-1].pnt2, m_vecControlPoints[pnt1_index].pnt2,
			m_vecControlPoints[pnt1_index+1].pnt2,m_vecControlPoints[pnt1_index+2].pnt2,fm, m_fFactor);
		//上帧的亮点
		pnt1_index = m_fLastPosRate/f0;    //curr_delta所在线段的前一点 curr_delta = 0.5f时  pnt1_index = 0.5f/0.25f = 2 即正好落在第四个点上
		fm = (m_fLastPosRate - f0*pnt1_index)/f0;  //当前所在的线段内的偏移量 curr_delta = 0.6f fm =(0.6-0.25*2)/0.25 = 0.4 即在当前区间的0.4分量上
		pnt1_index += 1; //因为第一个点不参加计算 所以向后偏移一位
		
		D3DXVECTOR3 pos11 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[pnt1_index-1].pnt1, m_vecControlPoints[pnt1_index].pnt1,
			m_vecControlPoints[pnt1_index+1].pnt1,m_vecControlPoints[pnt1_index+2].pnt1,fm, m_fFactor);
		D3DXVECTOR3 pos12 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[pnt1_index-1].pnt2, m_vecControlPoints[pnt1_index].pnt2,
			m_vecControlPoints[pnt1_index+1].pnt2,m_vecControlPoints[pnt1_index+2].pnt2,fm, m_fFactor);
		stPiece* p = getPiecePool()->GetPiece();
		p->point[0] = pos11;
		p->point[1] = pos21;
		p->point[2] = pos12;
		p->point[3] = pos22;
		p->m_color = m_startColor;
		p->instance = this;
		p->alreadyTime = 0.0f;
		m_listPiece.push_back(p);
		m_fLastPosRate = m_fCurrPosRate;
	}
	return true;
}

bool CDexPieceEffectInstance::LoadInitFile(const char* filename)
{
	getLog()->BeginLog();
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		getLog()->Log(log_allert, "open piece effect init file %s failed!\n",filename );
		getLog()->EndLog();
		pScript->CloseScript();
		return false;
	}
	getLog()->Log(log_ok, "begin loading piece effect init file %s !\n",filename );
	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	pScript->SetCurrChar();
	m_fSpeed = pScript->GetFloatParam();pScript->MoveCurrLine();pScript->SetCurrChar();
	m_iPieceLiveTime = pScript->GetIntParam();pScript->MoveCurrLine();pScript->SetCurrChar();
	m_bRenderPieceRoute =(bool) pScript->GetIntParam();pScript->MoveCurrLine();pScript->SetCurrChar();
	char texname[64];
	pScript->GetStringParam(texname);pScript->MoveCurrLine();pScript->SetCurrChar();
	setTexture(texname);
	m_fFactor = pScript->GetFloatParam();pScript->MoveCurrLine();pScript->SetCurrChar();
	m_startColor.r = pScript->GetFloatParam();m_startColor.g = pScript->GetFloatParam();m_startColor.b = pScript->GetFloatParam();m_startColor.a = pScript->GetFloatParam();pScript->MoveCurrLine();pScript->SetCurrChar();
	m_middleColor.r = pScript->GetFloatParam();m_middleColor.g = pScript->GetFloatParam();m_middleColor.b = pScript->GetFloatParam();m_middleColor.a = pScript->GetFloatParam();pScript->MoveCurrLine();pScript->SetCurrChar();
	m_endColor.r = pScript->GetFloatParam();m_endColor.g = pScript->GetFloatParam();m_endColor.b = pScript->GetFloatParam();m_endColor.a = pScript->GetFloatParam();pScript->MoveCurrLine();pScript->SetCurrChar();
	float x1 = 0.0f; float y1 = 0.0f; float z1 = 0.0f;
	float x2 = 0.0f; float y2 = 0.0f; float z2 = 0.0f;
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar() ;
		x1 = pScript->GetFloatParam(); y1 = pScript->GetFloatParam(); z1 = pScript->GetFloatParam(); 
		x2 = pScript->GetFloatParam(); y2 = pScript->GetFloatParam(); z2 = pScript->GetFloatParam(); 
		addControlPoint(D3DXVECTOR3(x1, y1, z1),D3DXVECTOR3(x2, y2, z2));
	}
	PreInitMemRes();
	getLog()->Log(log_ok, "load piece effect init file %s ok!\n",filename );
	getLog()->EndLog();
	pScript->CloseScript();
	return true;
}

bool CDexPieceEffectInstance::Render()
{
	DEX_ENSURE_B(CDexEffectInstance::Render());
	if(m_bRenderPieceRoute && DexGameEngine::getEngine()->getRenderPieceEffectRoute())
	{
		for(size_t i = 1; i < m_vecControlPoints.size()-2; i++)
		{
			int cut = 10; //两点间画多少线段描述曲线
			float add = 1.0f/10;
			DexColor color(0.0f, 0.0f, 1.0f, 1.0f);
			for(float f = 0.0f; f < 1.0f;)
			{
				D3DXVECTOR3 p1;
				D3DXVECTOR3 p2; 
				p1 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[i-1].pnt1, m_vecControlPoints[i].pnt1,
					m_vecControlPoints[i+1].pnt1,m_vecControlPoints[i+2].pnt1,f, m_fFactor);
				p2 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[i-1].pnt1, m_vecControlPoints[i].pnt1,
					m_vecControlPoints[i+1].pnt1,m_vecControlPoints[i+2].pnt1,f+add, m_fFactor);
				DexGameEngine::getEngine()->Render3DLine(p1, p2, color, color);

				p1 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[i-1].pnt2, m_vecControlPoints[i].pnt2,
					m_vecControlPoints[i+1].pnt2,m_vecControlPoints[i+2].pnt1,f, m_fFactor);
				p2 = DexGUI::CDexFun::get_catmull_value(m_vecControlPoints[i-1].pnt2, m_vecControlPoints[i].pnt2,
					m_vecControlPoints[i+1].pnt2,m_vecControlPoints[i+2].pnt1,f+add, m_fFactor);

				DexGameEngine::getEngine()->Render3DLine(p1, p2, color, color);
				color.r += add;
				f += add;
			}
		}
	}
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);	  //必须禁止深度缓冲的写入
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );	
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DexGameEngine::getEngine()->GetDevice()->SetSamplerState(0,   D3DSAMP_MIPFILTER,   D3DTEXF_LINEAR);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1 );	    
	bool fog = DexGameEngine::getEngine()->OpenFog();
	if(fog)
		DexGameEngine::getEngine()->DisableFog();
	if(m_pDexTex != NULL)
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, m_pDexTex->GetTexPt());
	else
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);

	float count = 0.0f;
	foreach(TListPiece, ite, m_listPiece)
	{		
		for(int i = 0 ; i < 4 ; i++)
		{
			g_pVertexList1[i].m_pos = (*ite)->point[i];
			g_pVertexList1[i].m_color = getD3DColor((*ite)->m_color);
		}
		g_pVertexList1[0].m_u = 1 - (count)/m_listPiece.size();
		g_pVertexList1[0].m_v = 0.0f;

		g_pVertexList1[1].m_u = 1 - (count+1)/m_listPiece.size();
		g_pVertexList1[1].m_v = 0.0f;

		g_pVertexList1[2].m_u = 1 - (count)/m_listPiece.size();
		g_pVertexList1[2].m_v = 1.0f;

		g_pVertexList1[3].m_u = 1 - (count+1)/m_listPiece.size();
		g_pVertexList1[3].m_v = 1.0f;

		++count;
		DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, getFinalMatrix());
		DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_pVertexList1, sizeof(stVertex1));
	}
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &g_transMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);		  //状态恢复
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	if(fog)
		DexGameEngine::getEngine()->EnableFog();
	return true;
}

void CDexPieceEffectInstance::PreInitMemRes()
{
	//speed越长 面片越多 生存时间约短 面片越少
	int count = m_fSpeed * 60 * m_iPieceLiveTime/1000 ;
	getPiecePool()->CreatePiece(count);
}

void CDexPieceEffectInstance::setRenderPieceRoute(bool b)
{
	m_bRenderPieceRoute = b;
}

void CDexPieceEffectInstance::addControlPoint(const D3DXVECTOR3& pnt1, const D3DXVECTOR3& pnt2)
{
	stPieceControlPoint p;
	p.pnt1 = pnt1;
	p.pnt2 = pnt2;
	m_vecControlPoints.push_back(p);
}

void CDexPieceEffectInstance::setPieceLiveTime(int16 time)
{
	m_iPieceLiveTime = time;
}

void CDexPieceEffectInstance::setFactor(float factor)
{
	m_fFactor = factor;
}

void CDexPieceEffectInstance::setSpeed(float speed)
{
	m_fSpeed = speed;
}

bool CDexPieceEffectInstance::setTexture(string filename)
{
	_REDUCEREF(m_pDexTex);
	m_pDexTex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(filename.c_str());
	_ADDREF(m_pDexTex);
	return true;
}

//================CDexPiecePool==========================================

CDexPiecePool::CDexPiecePool()
{
	m_listFreePiece.clear();

}
CDexPiecePool::~CDexPiecePool()
{
	//释放内存
	foreach(TlistPiece, ite, m_listFreePiece)
	{
		if(*ite != NULL)
		{
			delete *ite;
			*ite = NULL;
			ite = m_listFreePiece.erase(ite);
			if(ite == m_listFreePiece.end())
			{
				break;
			}
		}
	}
}

void CDexPiecePool::CreatePiece(int iCount)
{		
	int64 time1 = getTime()->GetTotalMillSeconds();
	for(int i = 0 ; i < iCount; i++)
	{
		CDexPieceEffectInstance::stPiece * piece = new CDexPieceEffectInstance::stPiece;
		m_listFreePiece.push_back(piece);
	}
	getLog()->BeginLog();
	getLog()->Log(log_ok, "申请%d个面片...，耗时%d ms", iCount, getTime()->GetTotalMillSeconds()- time1);
	getLog()->EndLog();
}

void CDexPiecePool::FreePiece(int iCount)
{
	int64 time1 = getTime()->GetTotalMillSeconds();
	int i = 0;
	foreach(std::list<CDexPieceEffectInstance::stPiece *>, ite, m_listFreePiece)
	{	 
		if(i == iCount)
			break;
		if(*ite != NULL)
		{
			delete *ite;
			*ite = NULL;
			ite = m_listFreePiece.erase(ite);
			if(ite == m_listFreePiece.end())
			{
				break;
			}
			i++;
		}
	}
	getLog()->BeginLog();
	getLog()->Log(log_ok, "释放%d个面片...，耗时%d ms", iCount, getTime()->GetTotalMillSeconds()- time1);
	getLog()->EndLog();
}

CDexPieceEffectInstance::stPiece* CDexPiecePool::GetPiece()
{
	CDexPieceEffectInstance::stPiece* piece = NULL;
	if(m_listFreePiece.size() == 0)
	{//面片池中没有面片了 新建(这种情况不应该出现 因为所有要求的面片内存都已经预分配了 警告！)
		getLog()->BeginLog();
		getLog()->Log(log_error, "面片池创建了一个不该创建的面片，警告！");
		getLog()->EndLog();
		piece = new CDexPieceEffectInstance::stPiece;
		return piece;
	}
	else
	{
		piece = *(m_listFreePiece.begin());
		m_listFreePiece.pop_front();
		return piece;
	}

}

void CDexPiecePool::Push(CDexPieceEffectInstance::stPiece* piece)
{
	DEX_ENSURE(piece);
	m_listFreePiece.push_back(piece);
}

CDexPiecePool* getPiecePool()
{
	static CDexPiecePool* g_pPiecePool = NULL ;
	if(g_pPiecePool == NULL)
		g_pPiecePool = new CDexPiecePool;
	return g_pPiecePool;
}
