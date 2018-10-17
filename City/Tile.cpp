/**
 * \file Tile.cpp
 *
 * \author Charles B. Owen
 */

#include "stdafx.h"
#include "Tile.h"
#include "City.h"
#include "TilePropertiesDlg.h"

using namespace std;
using namespace Gdiplus;

/// How much we offset drawing the tile to the left of the center
const int OffsetLeft = 64;

/// How much we offset drawing the tile above the center
const int OffsetDown = 32;

/// The directory containing the file images
const std::wstring CTile::ImagesDirectory = L"images/";


/**
 *  Distance from center for inside of tiles.
 *
 * Our tiles are a diamond that is 64 pixels tall and 128 pixels
 * wide. So, if we take the distance from the center vertically and 
 * double it, it would be as if we had a 64 by 64 diamond. The 
 * "Manhattan distance" from the center would be no more than 64
 * in that case.
 */
const int InsideTolerance = 64;

/**  Constructor
 * \param city The city this item is a member of
 */
CTile::CTile(CCity *city) : mCity(city)
{
}


/**
*  Destructor
*/
CTile::~CTile()
{
}

/**
 *  Set the image file to draw
 * \param file The base filename. Blank files are allowed
 */
void CTile::SetImage(const std::wstring &file)
{
    if (!file.empty())
    {
        wstring filename = ImagesDirectory + file;
        mItemImage = unique_ptr<Bitmap>(Bitmap::FromFile(filename.c_str()));
        if (mItemImage->GetLastStatus() != Ok)
        {
            wstring msg(L"Failed to open ");
            msg += filename;
            AfxMessageBox(msg.c_str());
            return;
        }
    }
    else
    {
        mItemImage.release();
    }

    mFile = file;
}

/**
*  Draw our tile
* \param graphics The graphics context to draw on
*/
void CTile::Draw(Gdiplus::Graphics *graphics)
{
    if (mItemImage != nullptr)
    {
        int wid = mItemImage->GetWidth();
        int hit = mItemImage->GetHeight();

        graphics->DrawImage(mItemImage.get(),
            mX - OffsetLeft, mY + OffsetDown - hit,
            wid, hit);
    }

}


/**  Draw a border around the tile
 * \param graphics The graphics context to draw on
 * \param pen The pen to use to draw with
 */
void CTile::DrawBorder(Gdiplus::Graphics *graphics, Gdiplus::Pen *pen)
{
    Point points[] = { { mX - OffsetLeft, mY }, { mX, mY - OffsetDown }, { mX + OffsetLeft, mY }, { mX, mY + OffsetDown }, { mX - OffsetLeft, mY } };

    graphics->DrawLines(pen, points, 5);
}




/**  Test to see if we hit this object with a mouse.
* \param x X position to test
* \param y Y position to test
* \return true if hit.
*/
bool CTile::HitTest(int x, int y)
{
    // Simple manhattan distance 
    return (abs(x - mX) + abs(y - mY) * 2) <= InsideTolerance;
}


/**  Save this item to an XML node
 * \param node The node we are going to be a child of
 * \returns Created XML node
 */
std::shared_ptr<xmlnode::CXmlNode> CTile::XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
    auto itemNode = node->AddChild(L"tile");

    itemNode->SetAttribute(L"x", mX);
    itemNode->SetAttribute(L"y", mY);
    itemNode->SetAttribute(L"zoning", (int)mZoning);

    return itemNode;
}


/**
* brief Load the attributes for an item node.
*
* This is the  base class version that loads the attributes
* common to all items. Override this to load custom attributes
* for specific items.
*
* \param node The Xml node we are loading the item from
*/
void CTile::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
    mX = node->GetAttributeIntValue(L"x", 0);
    mY = node->GetAttributeIntValue(L"y", 0);
    mZoning = (CTile::Zonings)node->GetAttributeIntValue(L"zoning", 0);
}

/**
 *  Force the tile to a regular grid by forcing the values to be multiples of 32.
 *
 * This version works correctly for negative coordinates.
 */
void CTile::QuantizeLocation()
{
    int spacing = CCity::GridSpacing;
    if (mX < 0)
    {
        mX = ((mX + spacing / 2) / spacing) * spacing - spacing;
    }
    else
    {
        mX = ((mX + spacing / 2) / spacing) * spacing;
    }

    if (mY < 0)
    {
        mY = ((mY + spacing / 2) / spacing) * spacing - spacing;
    }
    else
    {
        mY = ((mY + spacing / 2) / spacing) * spacing;
    }

}

/**
 *  Get any adjacent tile.
 *
 * Given a tile in the city, this determines if there is another
 * tile adjacent to it. The parameters dx, dy determine which direction
 * to look.
 *
 * The values for specific adjacencies (dx, dy, and direction):
 *    - -1 -1 Upper left
 *    - 1 -1 Upper right
 *    - -1 1 Lower left
 *    - 1 1 Lower right
 *
 * \param dx Left/right determination, -1=left, 1=right
 * \param dy Up/Down determination, -1=up, 1=down
 * \returns Adjacent tile or nullptr if none.
 */
std::shared_ptr<CTile> CTile::GetAdjacent(int dx, int dy)
{
    return mCity->GetAdjacent(this, dx, dy);
}

/**  Bring up the properties dialog box for this tile
 */
void CTile::PropertiesDlg()
{
    CTilePropertiesDlg dlg;
    dlg.SetZoning(mZoning);

    if (dlg.DoModal() == IDOK)
    {
        mZoning = dlg.GetZoning();
    }
}

