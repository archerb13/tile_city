/**
* \file TileRoad.h
*
* \author Charles B. Owen
*
*  Class that implements a Road tile
*/

#pragma once

#include "Tile.h"
#include "TileVisitor.h"


/**
*  A Road tile
*/
class CTileRoad : public CTile
{
public:
    CTileRoad(CCity *city);

    ///  Default constructor (disabled)
    CTileRoad() = delete;

    ///  Copy constructor (disabled)
    CTileRoad(const CTileRoad &) = delete;

    ~CTileRoad();

    virtual std::shared_ptr<xmlnode::CXmlNode> XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node) override;

    void SetAdjacencies(bool ul, bool ur, bool ll, bool lr);

	/** Accept a visitor
	 * \param visitor The visitor we accept */
	virtual void Accept(CTileVisitor *visitor) override { visitor->VisitRoad(this); }

private:
    /// The current adjacency integer or -1 if none
    int mCurrentAdj = -1;
};

