/**
 * \file FanSeason.h
 *
 * \author Bryce Archer
 *
 * Visitor for fan for new season
 */

#pragma once
#include "TileVisitor.h"


/**
 * Class for visitor for new season
 */
class CFanSeason :
	public CTileVisitor
{
public:
	CFanSeason();
	virtual ~CFanSeason();

	/** Visit a CTileFan object
	* \param fan Fan we are visiting */
	void VisitFan(CTileFan * fan) override;
};

