/**
* \file TileLandscape.h
*
* \author Charles B. Owen
*
*  Class that implements a Landscape tile
*/

#pragma once

#include "Tile.h"
#include "TileVisitor.h"


/**
*  A Landscape tile
*/
class CTileLandscape : public CTile
{
public:
    CTileLandscape(CCity *city);

    ///  Default constructor (disabled)
    CTileLandscape() = delete;

    ///  Copy constructor (disabled)
    CTileLandscape(const CTileLandscape &) = delete;

    ~CTileLandscape();

    virtual std::shared_ptr<xmlnode::CXmlNode> XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node) override;
    virtual void XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node);

	/** Accept a visitor
	* \param visitor The visitor we accept */
	virtual void Accept(CTileVisitor *visitor) override { visitor->VisitLandscape(this); }
};

