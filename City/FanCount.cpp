/**
 * \file FanCount.cpp
 *
 * \author Bryce Archer
 */

#include "stdafx.h"
#include "FanCount.h"
#include "TileFan.h"


CFanCount::CFanCount()
{
}


CFanCount::~CFanCount()
{
}

void CFanCount::VisitFan(CTileFan * fan)
{
	fan->Play();
}
