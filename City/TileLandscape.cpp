#include "stdafx.h"
#include "TileLandscape.h"

using namespace std;
using namespace Gdiplus;

/** Constructor
* \param city The city this is a member of
*/
CTileLandscape::CTileLandscape(CCity *city) : CTile(city)
{
}

/**
*  Destructor
*/
CTileLandscape::~CTileLandscape()
{
}


/**  Save this item to an XML node
* \param node The node we are going to be a child of
* \returns Allocated node
*/
std::shared_ptr<xmlnode::CXmlNode> CTileLandscape::XmlSave(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
    auto itemNode = CTile::XmlSave(node);

    itemNode->SetAttribute(L"type", L"landscape");
    itemNode->SetAttribute(L"file", GetFile());

    return itemNode;
}


/**
* brief Load the attributes for an item node.
* \param node The Xml node we are loading the item from
*/
void CTileLandscape::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode> &node)
{
    CTile::XmlLoad(node);
    SetImage(node->GetAttributeValue(L"file", L""));
}
