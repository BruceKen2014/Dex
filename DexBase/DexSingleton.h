
#ifndef _DEX_SINGLETON_H
#define _DEX_SINGLETON_H

#define DECLARE_SINGLETON(cls)\
public:\
	static cls* getSingleton()\
{\
	static cls* instance = new cls(); \
	return instance; \
}

#endif