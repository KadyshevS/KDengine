#pragma once
#include "KDWin.h"

class GDIPlusManager
{
	static ULONG_PTR token;
	static int refCount;

public:
	GDIPlusManager();
	~GDIPlusManager();
};