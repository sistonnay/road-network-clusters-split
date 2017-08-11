#pragma once

#ifndef BRIDGE_H
#define BRIDGE_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

class EX_IM_PORT Bridge
{

public:

	Bridge(void);
	Bridge(int _u,int _v);
	Bridge(const Bridge & other);
	Bridge & operator = (const Bridge & other);
	~Bridge(void);
	bool operator == (const Bridge & other);

public:

	int u,v;

};

#endif

