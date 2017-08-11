//
#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "Bridge.h"

Bridge::Bridge(void){}

Bridge::Bridge(int _u,int _v):u(_u),v(_v){}

Bridge::Bridge(const Bridge & other)
{
	u = other.u;
	v = other.v;
}

Bridge & Bridge::operator = (const Bridge & other)
{
	if(this!=&other)
	{
		u = other.u;
		v = other.v;
	}
	return *this;
}

Bridge::~Bridge(void){}

bool Bridge::operator == (const Bridge & other)
{
	if(u==other.u && v==other.v)
		return true;
	if(v==other.u && u==other.v)
		return true;
	return false;
}
