#include "stdafx.h"
#include "TileRoad.h"

using namespace std;
using namespace Gdiplus;

/** Constructor
* \param city The city this is a member of
*/
CTileRoad::CTileRoad(CCity *city) : CTile(city)
{
    SetAdjacencies(false, false, false, false);
}

/**
*  Destructor
*/
CTileRoad::~CTileRoad()
{
}

/**  Save this item to an XML node
 * \param node The node we are going to be a child of
 * \returns Pointer to the crated node
 */
std::shared_ptr<xmlnode::CXmlNode> CTileRoad::XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
    auto itemNode = CTile::XmlSave(node);

    itemNode->SetAttribute(L"type", L"road");

    return itemNode;
}


/**
 *  Indicate the road tiles adjacent to this one
 *
 * The road time image chosen is dependent on the tiles around
 * it. This is where the adjacency of road tiles is indicated.
 * 
 * \param ul True if road tile to upper left
 * \param ur True if road tile to upper right
 * \param ll True if road tile to lower left
 * \param lr True if road tile to lower right
 */
void CTileRoad::SetAdjacencies(bool ul, bool ur, bool ll, bool lr)
{
    // Create the adjacency code
    int code = (ul ? 1 : 0) | (ur ? 2 : 0) | (ll ? 4 : 0) | (lr ? 8 : 0);
    if (mCurrentAdj == code)
    {
        // We are already set. Do nothing
        return;
    }

    static wstring files[] = {
        L"roadlr.png",      // 0
        L"roadlr.png",      // 1
        L"roadud.png",      // 2
        L"roadlu.png",      // 3
        L"roadud.png",      // 4
        L"roadld.png",      // 5
        L"roadud.png",      // 6
        L"roadlud.png",     // 7
        L"roadlr.png",      // 8
        L"roadlr.png",      // 9
        L"roadur.png",      // 10
        L"roadlur.png",     // 11
        L"roaddr.png",      // 12
        L"roadldr.png",     // 13
        L"roadudr.png",     // 14
        L"roadludr.png"    // 15
    };

    // Select the appropriate image
    mCurrentAdj = code;
    SetImage(files[code]);
}
