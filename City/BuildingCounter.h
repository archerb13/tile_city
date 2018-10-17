/**
 * \file BuildingCounter.h
 *
 * \author Bryce Archer
 *
 * Visitor class to find number of buildings
 */

#pragma once
#include "TileVisitor.h"


/**
 * Class to visit biuldings to count them
 */
class CBuildingCounter : public CTileVisitor
{
public:
	CBuildingCounter();
	virtual ~CBuildingCounter();

	void VisitBuilding(CTileBuilding * building);

	/** Get the number of buildings
	* \returns Number of buildings */
	int GetNumBuildings() const { return mNumBuildings; }

private:
	/// Buildings counter
	int mNumBuildings = 0;
};

