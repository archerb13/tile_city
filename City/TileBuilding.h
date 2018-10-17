/**
* \file TileBuilding.h
*
* \author Charles B. Owen
*
*  Class that implements a Building tile
*/

#pragma once

#include "Tile.h"
#include "TileVisitor.h"



/**
*  A Building tile
*/
class CTileBuilding : public CTile
{
public:
    CTileBuilding(CCity *city);

    ///  Default constructor (disabled)
    CTileBuilding() = delete;

    ///  Copy constructor (disabled)
    CTileBuilding(const CTileBuilding &) = delete;

    ~CTileBuilding();

    virtual std::shared_ptr<xmlnode::CXmlNode> XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node) override;
    virtual void XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node);

	/** Accept a visitor
	* \param visitor The visitor we accept */
	virtual void Accept(CTileVisitor *visitor) override { visitor->VisitBuilding(this); }
};

