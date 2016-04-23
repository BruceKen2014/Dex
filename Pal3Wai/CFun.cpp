
#include "CFun.h"


bool DoLuaFile(char* file)
{
	if(int error = luaL_dofile(L, file) != 0)   //打开文件并编译
	{
		_Message("脚本%s初始化出错！", file);
		return false;
	}
	return true;
}

int SetCameraPos(lua_State *L)
{
	float xpos;
	float ypos;
	float zpos;
	//_lua_getfield(L, "xpos", &xpos, LUA_DOUBLE);
	//_lua_getfield(L, "ypos", &ypos, LUA_DOUBLE);
	//_lua_getfield(L, "zpos", &zpos, LUA_DOUBLE);
	int n= lua_gettop(L);  //返回栈中的元素

	xpos = (double)lua_tonumber(L, 1);
	ypos = (double)lua_tonumber(L, 2);
	zpos = (double)lua_tonumber(L, 3);
	g_camera.SetPosition(DexVector3(xpos, ypos, zpos));

	//返回压栈参数的个数,没有就返回0
	return 0;
}

int SetCameraFox(lua_State* L)
{
	float xpos;
	float ypos;
	float zpos;
	xpos = (double)lua_tonumber(L, 1);
	ypos = (double)lua_tonumber(L, 2);
	zpos = (double)lua_tonumber(L, 3);
	g_camera.SetFocus(DexVector3(xpos, ypos, zpos));
	//返回压栈参数的个数,没有就返回0
	return 0;
}

int SetCameraDest(lua_State* L)
{
	float xpos;
	float ypos;
	float zpos;
	xpos = (double)lua_tonumber(L, 1);
	ypos = (double)lua_tonumber(L, 2);
	zpos = (double)lua_tonumber(L, 3);
	g_camera.SetDestPos(DexVector3(xpos, ypos, zpos));
	//返回压栈参数的个数,没有就返回0
	return 0;
}
int EnableCameraMove(lua_State *L)
{
	int move;
	move = lua_tointeger(L, 1);
	g_camera.EnableMove(move);
	return 0;
}

int SetCameraVel(lua_State *L)
{
	float vel;
	vel = (double)lua_tonumber(L, 1);
	g_camera.SetMoveVel(vel);

	return 0;
}

int LuaMessageBox(lua_State* L)
{
	char message[256] = "";
	int i;
	int n= lua_gettop(L);  //返回栈中的元素
	for(i = 1; i <= n; i++)
	{
		if(lua_isstring(L, i))
			strcat(message, lua_tostring(L, i));
	}
	MessageBox(NULL,TEXT(message), "lua_MessageBox", MB_OK);

	//返回压栈参数的个数,没有就返回0
	//lua_pushnumber(L, 500);
	//lua_pushnumber(L, 500);
	//lua_pushnumber(L, 500);
	//return 3;
	return 0;
};
int LuaAddTalk(lua_State *L)
{
	TalkStruct* temp =new TalkStruct();
	temp->m_head = lua_tointeger(L,1);
	temp->m_dir = lua_tointeger(L,2);
	temp->m_words = (char*)malloc(sizeof(char)*SPEAK_MAX_WORDS);
	strcpy(temp->m_words, lua_tostring(L, 3));
	g_talk.push_back(temp);
	return 0;
}
int CreateObject(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));
	float xpos, ypos, zpos, angle,angleX, scale;
	int collide, render_sphere, cullNone;
	xpos = (double)lua_tonumber(L, 2);
	ypos = (double)lua_tonumber(L, 3);
	zpos = (double)lua_tonumber(L, 4);
	angleX = (double)lua_tonumber(L, 5);  //绕X轴旋转
	angle  = (double)lua_tonumber(L, 6);  //绕Y轴旋转
	scale = (double)lua_tonumber(L, 7);
	collide = lua_tointeger(L, 8);
	render_sphere = lua_tointeger(L, 9);
	cullNone = lua_tointeger(L, 10);
	CModelX* object = new CModelX(g_D3DDevice, filename); 
	object->SetScale(scale);
	object->SetPosition(xpos, ypos, zpos);	
	object->SetRotateX(_getRadian(angleX));
	object->SetRotateY(_getRadian(angle));

	object->SetCollide(collide);
	object->SetRenderSphere(render_sphere);
	object->SetCullNone(cullNone);
	g_modelVector.push_back(object);
	return 0;
}

int CreateAlphaObject(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));
	float xpos, ypos, zpos, angle,angleX, scale;
	int collide, render_sphere, cullNone;
	xpos = (double)lua_tonumber(L, 2);
	ypos = (double)lua_tonumber(L, 3);
	zpos = (double)lua_tonumber(L, 4);
	angleX = (double)lua_tonumber(L, 5);  //绕X轴旋转
	angle  = (double)lua_tonumber(L, 6);  //绕Y轴旋转
	scale = (double)lua_tonumber(L, 7);
	collide = lua_tointeger(L, 8);
	render_sphere = lua_tointeger(L, 9);
	cullNone = lua_tointeger(L, 10);
	CModelAlpha* object = new CModelAlpha(g_D3DDevice, filename); 
	object->SetScale(scale);
	object->SetPosition(xpos, ypos, zpos);	
	object->SetRotateX(_getRadian(angleX));
	object->SetRotateY(_getRadian(angle));

	object->SetCollide(collide);
	object->SetRenderSphere(render_sphere);
	object->SetCullNone(cullNone);
	g_alphaModelVector.push_back(object);
	return 0;
}

int CreateAniObject(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));
	D3DXVECTOR3 pos, iniDir, lookAt;
	float aniSpeed, vecl, scale, boxscale;
	int _id, render_sphere, _type;

	pos.x = (double)lua_tonumber(L, 2);
	pos.y = (double)lua_tonumber(L, 3);
	pos.z = (double)lua_tonumber(L, 4);
	iniDir.x = (double)lua_tonumber(L, 5);
	iniDir.y = (double)lua_tonumber(L, 6);
	iniDir.z = (double)lua_tonumber(L, 7);
	lookAt.x = (double)lua_tonumber(L, 8);
	lookAt.y = (double)lua_tonumber(L, 9);
	lookAt.z = (double)lua_tonumber(L, 10);
	aniSpeed = (double)lua_tonumber(L, 11);
	_id = lua_tointeger(L, 12);	 
	vecl = (double)lua_tonumber(L, 13);
	scale = (double)lua_tonumber(L, 14);
	boxscale = (double)lua_tonumber(L, 15);
	render_sphere = lua_tointeger(L, 16);
	_type = lua_tointeger(L, 17);

	CModelXAni* object = new CModelXAni(g_D3DDevice); 
	if(FAILED(object->LoadXFile(filename)))
	{
		//_SafeDelete(object);
		return 0;
	}
	object->SetScale(scale);
	object->SetPosition(pos);
	object->SetVel(vecl);
	object->SetRenderSphere(render_sphere);
	object->IniDirection(iniDir);
	object->FaceTo(lookAt);
	object->SetAniSpeed(aniSpeed);
	object->SetId(_id);
	object->SetBallScale(boxscale);
	object->SetMoving(false) ;
	g_aniModelVector.push_back(object);

	return 0;
}

int CreateSphere(lua_State* L)
{
	float centerx, centery, centerz, radius;
	int   _render;
	centerx = (double)lua_tonumber(L, 1);
	centery = (double)lua_tonumber(L, 2);
	centerz = (double)lua_tonumber(L, 3);
	radius = (double)lua_tonumber(L, 4);
	_render = lua_tointeger(L, 5);
	CCollideSphere* sphere = new CCollideSphere(g_D3DDevice, D3DXVECTOR3(centerx, centery, centerz), radius, _render);
	g_collideSphere.push_back(sphere);
	return 0;
}
int CreateBox(lua_State* L)
{
	float posX, posY, posZ;
	float width, height, depth;
	int   _render;
	posX = (double)lua_tonumber(L, 1);
	posY = (double)lua_tonumber(L, 2);
	posZ = (double)lua_tonumber(L, 3);
	width = (double)lua_tonumber(L, 4);
	height = (double)lua_tonumber(L, 5);
	depth = (double)lua_tonumber(L, 6);
	_render = lua_tointeger(L, 7);
	CCollideBox* box = new CCollideBox(g_D3DDevice, D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(width,  height, depth), _render);
	g_collideBox.push_back(box);
	return 0;
}

int CreateRoad(lua_State* L)
{
	char filename[64]; 
	char sndFile[64];
	strcpy(filename, lua_tostring(L, 1));
	float posX, posY, posZ;
	float width, length;
	float tileX, tileY;
	int   id, _render, dir;	 
	posX = (double)lua_tonumber(L, 2);
	posY = (double)lua_tonumber(L, 3);
	posZ = (double)lua_tonumber(L, 4);
	width = (double)lua_tonumber(L, 5);
	length = (double)lua_tonumber(L, 6);
	id = lua_tointeger(L, 7);
	strcpy(sndFile, lua_tostring(L, 8));
	_render = lua_tointeger(L, 9);
	dir = lua_tointeger(L, 10);
	tileX = (double)lua_tonumber(L, 11);
	tileY = (double)lua_tonumber(L, 12);
	CRoad* road = new CRoad(g_D3DDevice, D3DXVECTOR3(posX, posY, posZ), width, length, id, dir, tileX, tileY);
	road->LoadTexture(filename);
	road->SetSoundFile(sndFile);
	road->SetRender(_render);
	g_sndObject.push_back(road);
	return 0;
}

int CreateLadder(lua_State* L)
{
	char sndFile[64];
	float posX, posY, posZ;
	float width, length, height;
	int   dir, id, _render, texture;
	posX = (double)lua_tonumber(L, 1);
	posY = (double)lua_tonumber(L, 2);
	posZ = (double)lua_tonumber(L, 3);	
	dir = lua_tointeger(L, 4);
	width = (double)lua_tonumber(L, 5);
	length = (double)lua_tonumber(L, 6);
	height = (double)lua_tonumber(L, 7);
	id = lua_tointeger(L, 8);
	strcpy(sndFile, lua_tostring(L, 9));
	_render = lua_tointeger(L, 10);
	texture = lua_tointeger(L, 11);
	CLadder* ladder = new CLadder(g_D3DDevice, D3DXVECTOR3(posX, posY, posZ), width, length, height, dir, id);
	if(texture)
	{
		char filename[64];
		strcpy(filename, lua_tostring(L, 12)); 
		ladder->LoadTexture(filename);
	}
	ladder->SetRender(_render);
	ladder->SetSoundFile(sndFile);
	g_sndObject.push_back(ladder);
	return 0;
}
int CreateNpc(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));
	D3DXVECTOR3 pos, iniDir, lookAt;
	float aniSpeed, vecl, scale, boxscale;
	int _id, render_sphere, _type;

	pos.x = (double)lua_tonumber(L, 2);
	pos.y = (double)lua_tonumber(L, 3);
	pos.z = (double)lua_tonumber(L, 4);
	iniDir.x = (double)lua_tonumber(L, 5);
	iniDir.y = (double)lua_tonumber(L, 6);
	iniDir.z = (double)lua_tonumber(L, 7);
	lookAt.x = (double)lua_tonumber(L, 8);
	lookAt.y = (double)lua_tonumber(L, 9);
	lookAt.z = (double)lua_tonumber(L, 10);
	aniSpeed = (double)lua_tonumber(L, 11);
	_id = lua_tointeger(L, 12);	 
	vecl = (double)lua_tonumber(L, 13);
	scale = (double)lua_tonumber(L, 14);
	boxscale = (double)lua_tonumber(L, 15);
	render_sphere = lua_tointeger(L, 16);
	_type = lua_tointeger(L, 17);

	CNpc* object = new CNpc(g_D3DDevice); 
	if(FAILED(object->LoadXFile(filename)))
	{
		//_SafeDelete(object);
		return 0;
	}

	object->SetScale(scale);
	object->SetPosition(pos);
	object->SetVel(vecl);
	object->SetRenderSphere(render_sphere);
	object->IniDirection(iniDir);
	object->FaceTo(lookAt);
	object->SetAniSpeed(aniSpeed);
	object->SetId(_id);
	object->SetBallScale(boxscale);
	object->SetMoving(true) ;
	object->SetType(_type);
	
	switch(_id)
	{
	case ID_JINGTIAN:
		{
			object->SetAnimation(JINGTIAN_STAND);
			object->SetIndex(JINGTIAN_STAND, JINGTIAN_WALK);
			break;
		}
	case ID_PANDA:
		{
			object->SetAnimation(PANDA_STAND);
			object->SetIndex(PANDA_LOOK, PANDA_RUN);
			break;
		}
	case ID_HUNTER:
		{
			object->SetAnimation(HUNTER_LOOK);
			object->SetIndex(HUNTER_LOOK, HUNTER_RUN);
			break;
		}
	case ID_BEAR:
		{
			object->SetAnimation(BEAR_STAND);
			object->SetIndex(BEAR_STAND, BEAR_WALK);
			break;
		}
	case ID_THINNER:
		{
			object->SetAnimation(THINNER_STAND);
			object->SetIndex(THINNER_STAND, THINNER_WALK);
		}
	default:
		break;
	}
	if(g_isTownNpc)
		g_npcVector.push_back(object);
	else
		g_npcVector2.push_back(object);
	return 0;
}

int AddNpcNode(lua_State* L)
{
	if(g_isTownNpc)
	{
		if(g_npcVector.size() == 0)
			return 0;
	}
	else
	{
		if(g_npcVector2.size() == 0)
			return 0;
	}
	D3DXVECTOR3 pos;
	int _time;
	pos.x = (double)lua_tonumber(L, 1);
	pos.y = (double)lua_tonumber(L, 2);
	pos.z = (double)lua_tonumber(L, 3);

	_time = lua_tointeger(L, 4);

	if(g_isTownNpc)
	{
		size_t index = g_npcVector.size()- 1;
		g_npcVector[index]->AddNode(pos, _time);
	}
	else
	{
		size_t index = g_npcVector2.size()- 1;
		g_npcVector2[index]->AddNode(pos, _time);
	}
	return 0;
}

int  AddEnemy(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));
	D3DXVECTOR3 pos, iniDir, lookAt;
	float aniSpeed, vecl, scale, boxscale;
	int _id, render_sphere;

	pos.x = (double)lua_tonumber(L, 2);
	pos.y = (double)lua_tonumber(L, 3);
	pos.z = (double)lua_tonumber(L, 4);
	iniDir.x = (double)lua_tonumber(L, 5);
	iniDir.y = (double)lua_tonumber(L, 6);
	iniDir.z = (double)lua_tonumber(L, 7);
	lookAt.x = (double)lua_tonumber(L, 8);
	lookAt.y = (double)lua_tonumber(L, 9);
	lookAt.z = (double)lua_tonumber(L, 10);
	aniSpeed = (double)lua_tonumber(L, 11);
	_id = lua_tointeger(L, 12);	 
	vecl = (double)lua_tonumber(L, 13);
	scale = (double)lua_tonumber(L, 14);
	boxscale = (double)lua_tonumber(L, 15);
	render_sphere = lua_tointeger(L, 16);
	//_type = lua_tointeger(L, 17);

													 
	//CModelXAni* tempEnemy = new CModelXAni(g_D3DDevice);
	CEnemy* tempEnemy = new CEnemy(g_D3DDevice);

	if(FAILED(tempEnemy->LoadXFile(filename)))
	{
		_SafeDelete(tempEnemy);
		return 0;
	}
	tempEnemy->SetPosition(pos);
	tempEnemy->IniDirection(iniDir);
	tempEnemy->FaceTo(lookAt);
	tempEnemy->SetId(_id);
	tempEnemy->SetRenderSphere(render_sphere);
	tempEnemy->SetAniSpeed(aniSpeed);
	tempEnemy->SetScale(scale);
	tempEnemy->SetBallScale(boxscale);
	tempEnemy->SetVel(vecl);

	EnmyPara para;
	para.m_destPos = D3DXVECTOR3(3169   ,0,  2194 );
	para.m_time = 100;
	para.m_standPos = pos;

	tempEnemy->SetMoving(true) ;
	switch(_id)
	{
	case ID_JINGTIAN:
		{
			tempEnemy->SetAnimation(JINGTIAN_STAND);
			tempEnemy->LoadIni("ini\\fight.ini", "jingtian");
			para.m_standIndex = JINGTIAN_STAND; 
			para.m_runIndex = JINGTIAN_WALK;
			para.m_dieIndex = JINGTIAN_STAND;
			para.m_attackIndex.push_back(JINGTIAN_ATTACK);
			//object->SetIndex(JINGTIAN_STAND, JINGTIAN_WALK);
			break;
		}
	case ID_PANDA:
		{
			tempEnemy->SetAnimation(PANDA_STAND);
			tempEnemy->LoadIni("ini\\fight.ini", "panda");
			para.m_standIndex = PANDA_LOOK; 
			para.m_runIndex = PANDA_RUN;
			para.m_dieIndex = PANDA_DIE;
			para.m_attackIndex.push_back(PANDA_ATTACK_TWO);
			//object->SetIndex(PANDA_LOOK, PANDA_RUN);
			break;
		}
	case ID_HUNTER:
		{
			tempEnemy->SetAnimation(HUNTER_LOOK);
			tempEnemy->LoadIni("ini\\fight.ini", "hunter");
			para.m_standIndex = HUNTER_LOOK; 
			para.m_runIndex = HUNTER_RUN;
			para.m_dieIndex = HUNTER_DIE;
			para.m_attackIndex.push_back(HUNTER_ATTACK);
			//object->SetIndex(HUNTER_LOOK, HUNTER_RUN);
			break;
		}
	case ID_BEAR:
		{
			tempEnemy->SetAnimation(BEAR_STAND);
			tempEnemy->LoadIni("ini\\fight.ini", "bear");
			para.m_standIndex = BEAR_STAND; 
			para.m_runIndex = BEAR_WALK;
			para.m_dieIndex = BEAR_DIE;
			para.m_attackIndex.push_back(BEAR_ATTACK);
			//object->SetIndex(BEAR_STAND, BEAR_WALK);
			break;
		}
	case ID_THINNER:
		{
			tempEnemy->SetAnimation(THINNER_STAND);
			tempEnemy->LoadIni("ini\\fight.ini", "thinner");
			para.m_standIndex = THINNER_STAND; 
			para.m_runIndex = THINNER_WALK;
			para.m_dieIndex = BEAR_DIE;
			para.m_attackIndex.push_back(THINNER_ATTACK);
			//object->SetIndex(THINNER_STAND, THINNER_WALK);
		}
	case ID_DRAGON:
		{
			tempEnemy->LoadIni("ini\\fight.ini", "dragon");
		}
	default:
		break;
	}
	tempEnemy->SetEnmyPara(para);
	g_enemyVector.push_back(tempEnemy);
	return 0;
}

int CreateWeather(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));

	float posX, posY, posZ;
	float width, height, depth;
	float vel, par_size;			 //速度 颗粒大小
	int   unique_color;				//是否独色
	int   color_r, color_g, color_b;  //颜色
	int   cnt, render;	 //数量  是否渲染
	posX = (double)lua_tonumber(L, 2);
	posY = (double)lua_tonumber(L, 3);
	posZ = (double)lua_tonumber(L, 4);
	width = (double)lua_tonumber(L, 5);
	height = (double)lua_tonumber(L, 6);
	depth = (double)lua_tonumber(L, 7);
	vel = (double)lua_tonumber(L, 8);
	par_size = (double)lua_tonumber(L, 9);

	unique_color = lua_tointeger(L, 10);
	color_r = lua_tointeger(L, 11);
	color_g = lua_tointeger(L, 12);
	color_b = lua_tointeger(L, 13);
	cnt = lua_tointeger(L, 14);
	render = lua_tointeger(L, 15);

	CRainSystem * weather = new CRainSystem(g_D3DDevice, posX, posY, posZ, cnt);
	g_SlowWeather = weather;
	weather->LoadTexture(filename);	 
	weather->SetRainPS(width, height, depth);
	weather->SetParticalVelocity(vel);
	weather->SetParticalSize(par_size);
	weather->SetColor(DexColor(color_r, color_g, color_b));
	weather->SetIsUniqueColor(unique_color);
	weather->SetRender(render);
	weather->Initial();

	g_partical.push_back(weather);
	return 0;
}

int CreateFire(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));

	float posX, posY, posZ;
	float radius, power, wave;
	float vel, par_size;			 //速度 颗粒大小
	int   unique_color;				//是否独色
	int   color_r, color_g, color_b;  //颜色
	int   cnt, render;	 //数量  是否渲染
	posX = (double)lua_tonumber(L, 2);
	posY = (double)lua_tonumber(L, 3);
	posZ = (double)lua_tonumber(L, 4);
	radius = (double)lua_tonumber(L, 5);
	power = (double)lua_tonumber(L, 6);
	wave = (double)lua_tonumber(L, 7);
	vel = (double)lua_tonumber(L, 8);
	par_size = (double)lua_tonumber(L, 9);

	unique_color = lua_tointeger(L, 10);
	color_r = lua_tointeger(L, 11);
	color_g = lua_tointeger(L, 12);
	color_b = lua_tointeger(L, 13);
	cnt = lua_tointeger(L, 14);
	render = lua_tointeger(L, 15);

	CFireSystem * fire = new CFireSystem(g_D3DDevice, posX, posY, posZ, cnt);
	fire->LoadTexture(filename);	 
	fire->SetFirePS(radius, power, wave, color_r, color_g, color_b);
	fire->SetParticalVelocity(vel);
	fire->SetParticalSize(par_size);
	//weather->SetColor(D3DCOLOR_XRGB(color_r, color_g, color_b));
	fire->SetIsUniqueColor(unique_color);
	fire->SetRender(render);
	fire->Initial();

	g_partical.push_back(fire);
	return 0;
}

int CreateTerrain(lua_State* L)
{  
	_SafeDelete(g_terrain);
	char raw[64];
	char texture[64];
	int cellWidth, cellHeight; 
	float scale;
	float posX, posY, posZ;
	strcpy(raw, lua_tostring(L, 1));
	strcpy(texture, lua_tostring(L, 2));

	cellWidth = lua_tointeger(L, 3);
	cellHeight = lua_tointeger(L, 4);
	scale = (double)lua_tonumber(L, 5);
	posX = (double)lua_tonumber(L, 6);
	posY = (double)lua_tonumber(L, 7);
	posZ = (double)lua_tonumber(L, 8);

	g_terrain = new CTerrain(g_D3DDevice);
	g_terrain->CreateTerrain(raw, cellWidth, cellHeight, scale);
	g_terrain->LoadTexture(texture);
	g_terrain->SetPos(D3DXVECTOR3(posX, posY, posZ));
	return 0;

}

int OpenFog(lua_State* L)
{
	g_OpenFog = true;
	return 0;
}

int CloseFog(lua_State* L)
{
	g_OpenFog = false;
	return 0;
}

int PlayMusic(lua_State* L)
{
	char filename[64];
	strcpy(filename, lua_tostring(L, 1));
	int loop;
	loop = lua_tointeger(L, 2);
	CMusicMidi::getMidiSingleton().Play(filename, loop);
	return 0;
}

int GetClickId(lua_State* L)
{
	lua_pushinteger(L, g_clickId);
	return 1;
}
int ReSetClick(lua_State* L)
{
	g_clickId = 0;
	return 0;
}

int GetTask(lua_State* L)
{
	lua_pushinteger(L, g_task);
	return 1;
}
int SetTask(lua_State* L)
{
	g_task = lua_tointeger(L, 1);
	return 0;
}

int SetSkyPra(lua_State* L)
{
	if(g_pSky == NULL)
		return 0;
	float posX, posY, posZ;
	float width,height,depth;
	posX = (double)lua_tonumber(L, 1);
	posY = (double)lua_tonumber(L, 2);
	posZ = (double)lua_tonumber(L, 3);
	width = (double)lua_tonumber(L, 4);
	height = (double)lua_tonumber(L, 5);
	depth = (double)lua_tonumber(L, 6);

	g_pSky->SetPosition(posX, posY, posZ);
    g_pSky->SetSize(width, height, depth);
	return 0;
}

int SetSkyFace(lua_State* L)
{
	if(g_pSky == NULL)
		return 0;
	char front[64];
	char back[64];
	char left[64];
	char right[64];
	char top[64];
	char bottom[64];
	strcpy(front, lua_tostring(L, 1));
	strcpy(back, lua_tostring(L, 2));
	strcpy(left, lua_tostring(L, 3));
	strcpy(right, lua_tostring(L, 4));
	strcpy(top, lua_tostring(L, 5));
	strcpy(bottom, lua_tostring(L, 6));

	g_pSky->SetFace(SF_FRONT, front);
	g_pSky->SetFace(SF_BACK, back);
	g_pSky->SetFace(SF_LEFT, left);
	g_pSky->SetFace(SF_RIGHT, right);
	g_pSky->SetFace(SF_TOP, top);
	g_pSky->SetFace(SF_BOTTOM, bottom);
	return 0;
}
void LuaRegisterFun(lua_State *L)
{
	lua_register(L, "SetCameraPos", SetCameraPos);
	lua_register(L, "SetCameraFox", SetCameraFox);
	lua_register(L, "SetCameraDest", SetCameraDest);
	lua_register(L, "EnableCameraMove", EnableCameraMove);
	lua_register(L, "SetCameraVel", SetCameraVel);

	lua_register(L, "LuaMessageBox", LuaMessageBox);  
	lua_register(L, "Talk", LuaAddTalk);

	lua_register(L, "CreateObject", CreateObject);
	lua_register(L, "CreateAlphaObject", CreateAlphaObject);
	lua_register(L, "CreateAniObject", CreateAniObject);
	lua_register(L, "CreateSphere", CreateSphere);
	lua_register(L, "CreateBox", CreateBox);
	lua_register(L, "CreateRoad", CreateRoad);	   
	lua_register(L, "CreateLadder", CreateLadder);
	lua_register(L, "CreateNpc", CreateNpc);  
	lua_register(L, "AddEnemy", AddEnemy); 
	lua_register(L, "AddNpcNode", AddNpcNode);
	lua_register(L, "CreateWeather", CreateWeather);   
	lua_register(L, "CreateFire", CreateFire);
	lua_register(L, "CreateTerrain", CreateTerrain);

	lua_register(L, "PlayMusic", PlayMusic);
	lua_register(L, "OpenFog", OpenFog);
	lua_register(L, "CloseFog", CloseFog);

	lua_register(L, "GetClickId", GetClickId);
	lua_register(L, "ReSetClick", ReSetClick);
	lua_register(L, "GetTask", GetTask);

	lua_register(L, "SetTask", SetTask);
	lua_register(L, "SetSkyPra", SetSkyPra);
	lua_register(L, "SetSkyFace", SetSkyFace);
}
void GetFps()
{
	g_currentTime = getTime()->GetTotalMillSeconds();
	if(g_currentTime - g_lastTime > 1000.0f)
	{
		g_lastTime = g_currentTime;
		g_fps = g_counter;
		g_counter = 0;
	}
	else
	{
		g_counter++;
	}
};