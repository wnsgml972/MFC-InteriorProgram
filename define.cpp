#include "stdafx.h"
#include "define.h"



GlobalNum* GlobalNum::instance = NULL;

GlobalNum::GlobalNum()
{
	nPaintStatus = 0;
}

GlobalNum* GlobalNum::getInstance()
{
	if (!instance) {
		instance = new GlobalNum();
	}
	return instance;
}
