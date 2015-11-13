/*************************************************************
说明：由于网上找到的X文件动画索引不统一，在此定义每个X文件特定的动作索引
2012-12-8
*************************************************************/

#ifndef  _CDEFINEANI_H
#define  _CDEFINEANI_H

#define  USE_DEFINEANI  
#ifdef USE_DEFINEANI

//模型类别(碰撞检测、事件触发时要用到)
	#define   ID_FLAG      0
	#define   ID_JINGTIAN  (ID_FLAG+1)
	#define   ID_PANDA     (ID_FLAG+2)
	#define   ID_HUNTER    (ID_FLAG+3)
	#define   ID_BEAR	   (ID_FLAG+4)
    #define   ID_THINNER       (ID_FLAG+5)	 //瘦子
    #define   ID_DRAGON       (ID_FLAG+6)	    

    //景天
	#define   JINGTIAN_STAND          0  //站立
	#define   JINGTIAN_WALK	          4  //行走
	#define   JINTTIAN_RUN            3  //奔跑
	#define   JINGTIAN_ROTATE_ATTACK  1  //旋转攻击
    #define   JINGTIAN_ATTACK         2  //原地攻击  
    //拿着砍刀的熊猫
	#define   PANDA_STAND        8  //站立
	#define   PANDA_RUN          0  //奔跑
	#define   PANDA_LOOK         7  //左右张望	
    #define   PANDA_DIE          2  //死亡
	#define   PANDA_FLY_UP       1  //从地上头朝下飞起
	#define   PANDA_ATTACK_TWO   3  //双刀砍
	#define   PANDA_ATTACK_ONE   4  //单刀砍 
	#define   PANDA_ATTACK_HARD  5  //用力砍
	#define   PANDA_ATTACK_STAB  6  //用刀刺
    //恶魔猎手
	#define   HUNTER_LOOK        0  //左右张望
	#define   HUNTER_RUN         4  //奔跑	 
	#define   HUNTER_DIE         5  //死亡
	#define   HUNTER_POSE        1  //摆pose
	#define   HUNTER_ATTACK      2  //攻击
	#define   HUNTER_ATTACK_JUMP 3  //跳起攻击
    //熊
	#define   BEAR_STAND  3  //站立
	#define   BEAR_WALK   2  //行走
	#define   BEAR_DIE    0 //死亡 
	#define   BEAR_ATTACK 1	//攻击
	//瘦子
	#define   THINNER_STAND     0
	#define   THINNER_WALK	   1 
	#define   THINNER_WALK_QUIK 2 //快步行走
	#define   THINNER_RUN       5
	#define   THINNER_ATTACK	   3
	#define   THINNER_ROTATE    4//旋转攻击



#endif


#endif