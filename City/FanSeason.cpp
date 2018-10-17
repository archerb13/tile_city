/**
 * \file FanSeason.cpp
 *
 * \author Bryce Archer
 */

#include "stdafx.h"
#include "FanSeason.h"
#include "TileFan.h"


CFanSeason::CFanSeason()
{
}


CFanSeason::~CFanSeason()
{
}

void CFanSeason::VisitFan(CTileFan * fan) 
{
	fan->ResetSeason();
	//fan->Play();
}
