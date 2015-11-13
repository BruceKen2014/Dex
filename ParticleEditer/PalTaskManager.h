#ifndef _PAL_TASK_MANAGER_H
#define _PAL_TASK_MANAGER_H

typedef struct _stTask
{
	int id;
	string condition; //完成任务的条件
	bool complete;
}stTask;

class CPalTaskManager
{
protected:
};
#endif