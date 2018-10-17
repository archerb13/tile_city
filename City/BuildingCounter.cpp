#include "stdafx.h"
#include "BuildingCounter.h"


CBuildingCounter::CBuildingCounter()
{
}


CBuildingCounter::~CBuildingCounter()
{
}

/** Visit a CTileBuilding object
* \param building Building we are visiting */
void CBuildingCounter::VisitBuilding(CTileBuilding *building)
{
	mNumBuildings++;
}