/**
 * \file FanCount.h
 *
 * \author Bryce Archer
 *
 * Visitor for fan for plays
 */

#pragma once
#include "TileVisitor.h"


/**
 * Class for visitor to count plays
 */
class CFanCount :
	public CTileVisitor
{
public:
	CFanCount();
	virtual ~CFanCount();

	/** Visit a CTileFan object
	* \param fan Fan we are visiting */
	void VisitFan(CTileFan * fan) override;

};

