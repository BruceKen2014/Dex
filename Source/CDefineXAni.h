/*************************************************************
˵�������������ҵ���X�ļ�����������ͳһ���ڴ˶���ÿ��X�ļ��ض��Ķ�������
2012-12-8
*************************************************************/

#ifndef  _CDEFINEANI_H
#define  _CDEFINEANI_H

#define  USE_DEFINEANI  
#ifdef USE_DEFINEANI

//ģ�����(��ײ��⡢�¼�����ʱҪ�õ�)
	#define   ID_FLAG      0
	#define   ID_JINGTIAN  (ID_FLAG+1)
	#define   ID_PANDA     (ID_FLAG+2)
	#define   ID_HUNTER    (ID_FLAG+3)
	#define   ID_BEAR	   (ID_FLAG+4)
    #define   ID_THINNER       (ID_FLAG+5)	 //����
    #define   ID_DRAGON       (ID_FLAG+6)	    

    //����
	#define   JINGTIAN_STAND          0  //վ��
	#define   JINGTIAN_WALK	          4  //����
	#define   JINTTIAN_RUN            3  //����
	#define   JINGTIAN_ROTATE_ATTACK  1  //��ת����
    #define   JINGTIAN_ATTACK         2  //ԭ�ع���  
    //���ſ�������è
	#define   PANDA_STAND        8  //վ��
	#define   PANDA_RUN          0  //����
	#define   PANDA_LOOK         7  //��������	
    #define   PANDA_DIE          2  //����
	#define   PANDA_FLY_UP       1  //�ӵ���ͷ���·���
	#define   PANDA_ATTACK_TWO   3  //˫����
	#define   PANDA_ATTACK_ONE   4  //������ 
	#define   PANDA_ATTACK_HARD  5  //������
	#define   PANDA_ATTACK_STAB  6  //�õ���
    //��ħ����
	#define   HUNTER_LOOK        0  //��������
	#define   HUNTER_RUN         4  //����	 
	#define   HUNTER_DIE         5  //����
	#define   HUNTER_POSE        1  //��pose
	#define   HUNTER_ATTACK      2  //����
	#define   HUNTER_ATTACK_JUMP 3  //���𹥻�
    //��
	#define   BEAR_STAND  3  //վ��
	#define   BEAR_WALK   2  //����
	#define   BEAR_DIE    0 //���� 
	#define   BEAR_ATTACK 1	//����
	//����
	#define   THINNER_STAND     0
	#define   THINNER_WALK	   1 
	#define   THINNER_WALK_QUIK 2 //�첽����
	#define   THINNER_RUN       5
	#define   THINNER_ATTACK	   3
	#define   THINNER_ROTATE    4//��ת����



#endif


#endif