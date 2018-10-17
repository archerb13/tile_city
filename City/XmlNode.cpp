/**
 * \file
 *
 * \author Charles B. Owen
 *  Class that implements a wrapper for msxml nodes
 *
 * \version 1.01 07-16-2014
 * \version 1.02 07-17-2014
 * \version 1.03 07-17-2014
 */

// Ensure the file version above matches the class version
// in XmlNode.h

#include "stdafx.h"
#include "XmlNode.h"

using namespace std;
using namespace xmlnode;

/** 
 *  Constructor
 *
 * Creates an empty CXmlNode with no attached XML document.
 * You can then call either Open or Create on the node. However,
 * the preferred method is to use the static member functions
 * OpenDocument and CreateDocument.
 *
 */
CXmlNode::CXmlNode()
{
}

/**  Constructor (private, used internally only)
 * \param parent The parent of this node
 * \param node The underlying msxml node
 */
CXmlNode::CXmlNode(CXmlNode *parent, IXMLDOMNode *node)
{
    mDocument = parent->mDocument;
    mNode = node;
}

/**  Destructor */
CXmlNode::~CXmlNode()
{
}


/**
*  Open an XML document and return the document root node.
*
* \param filename Filename to open
* \returns Document root node
*/
std::shared_ptr<CXmlNode> CXmlNode::OpenDocument(const std::wstring &filename)
{
    auto node = make_shared<CXmlNode>();
    node->Open(filename);
    return node;
}

/** 
 *  Open a file as an XML document.
 * \param filename Filename to open
 */
void CXmlNode::Open(const std::wstring &filename)
{
    // Open the document
    mDocument = make_shared<Document>();
    mDocument->Open(filename);

    mNode = mDocument->GetRoot();
}


/**
*  Create an empty XML document and return the document root node.
*
* \param rootname The name for the root node.
* \returns Document root node
*/
std::shared_ptr<CXmlNode> CXmlNode::CreateDocument(const std::wstring &rootname)
{
    auto node = make_shared<CXmlNode>();
    node->Create(rootname);
    return node;
}

/**
 *  Create an empty XML document in this node.
 * \param rootname The name for the root node.
 */
void CXmlNode::Create(const std::wstring &rootname)
{
    // Open the document
    mDocument = make_shared<Document>();
    mDocument->Create(rootname);

    mNode = mDocument->GetRoot();
}


/**
 *  Save XML document to a file.
 * \param filename File to save to
 * \throws CXmlNode::Exception If there are errors in saving
 */
void CXmlNode::Save(const std::wstring &filename)
{
    mDocument->Save(filename);
}


/**
 *  Obtain the created XML as a string.
 * \returns String containing created XML
 */
std::wstring CXmlNode::GetXML()
{
    return mDocument->GetXML();
}


/**
 *  The node name.
 * \returns Node name
 */
std::wstring CXmlNode::GetName() const
{
    CComBSTR name;
    mNode->get_nodeName(&name);
    return wstring(name);
}


/**
 *  The node type
 * 
 * The possible node types are:
 * - NODE_INVALID
 * - NODE_ELEMENT
 * - NODE_ATTRIBUTE
 * - NODE_TEXT
 * - NODE_CDATA_SECTION
 * - NODE_ENTITY_REFERENCE
 * - NODE_ENTITY
 * - NODE_PROCESSING_INSTRUCTION
 * - NODE_COMMENT
 * - NODE_DOCUMENT
 * - NODE_DOCUMENT_TYPE
 * - NODE_DOCUMENT_FRAGMENT
 * - NODE_NOTATION
 *
 * The types that are usually important are:
 * - NODE_ELEMENT the XML tags
 * - NODE_TEXT text between tags
 * 
 * \returns The node type
 */
DOMNodeType CXmlNode::GetType() const
{
    DOMNodeType t;
    mNode->get_nodeType(&t);
    return t;
}


/**
 *  Get the node value
 * 
 * This is most useful for text and attributes.
 * \returns The node value as a string
 */
std::wstring CXmlNode::GetValue() const
{
    CComVariant value;
    mNode->get_nodeValue(&value);

    value.ChangeType(VT_BSTR);
    return wstring(value.bstrVal);
}

/**
*  Get the node value
*
* This is most useful for text and attributes.
* \returns The node value as an integer
* \throws std::invalid_argument if no conversion could be performed
* \throws std::out_of_range if the converted value would 
* fall out of the range of the result type or if the underlying function
*/
int CXmlNode::GetIntValue() const
{
    return stoi(GetValue());
}

/**
*  Get the node value
*
* This is most useful for text and attributes.
* \returns The node value as a double
* \throws std::invalid_argument if no conversion could be performed
* \throws std::out_of_range if the converted value would
* fall out of the range of the result type or if the underlying function
*/
double CXmlNode::GetDoubleValue() const
{
    return stod(GetValue());
}




/**
 *  Get an attribute.
 *
 * This returns a CXmlNode object that is the attribute.
 * You an then use the GetValue function to get the value of the attribute.
 * 
 * Calls to this function create a CXmlNode object as a wrapper for
 * the underlying attribute. Subsequent calls for the same attribute
 * will return a different object.
 *
 * \param name Attribute name
 * \returns Attribute as a CXmlNode object or null if attribute does not exist.
 */
std::shared_ptr<CXmlNode>  CXmlNode::GetAttribute(const wstring &name)
{
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    mNode->get_attributes(&attributes);

    CComPtr<IXMLDOMNode> attrib;
    attributes->getNamedItem(CComBSTR(name.c_str()), &attrib);
    if (attrib == nullptr) {
        return shared_ptr<CXmlNode>();
    }

    return shared_ptr<CXmlNode>(new CXmlNode(this, attrib));
}


/**
 *  Get the value of an attribute.
 * \param name Name of the attribute
 * \param def Default value to return if attribute does not exist.
 * \returns Attribute value as a string or the default if does not exist.
 */
std::wstring CXmlNode::GetAttributeValue(const std::wstring &name, const std::wstring &def)
{
    std::shared_ptr<CXmlNode> attr = GetAttribute(name);
    if (attr == nullptr)
    {
        return def;
    }

    return attr->GetValue();
}

/**
*  Get the value of an attribute.
* \param name Name of the attribute
* \param def Default value to return if attribute does not exist.
* \returns Attribute value as an int or the default if does not exist.
* \throws std::invalid_argument if no conversion could be performed
* \throws std::out_of_range if the converted value would
* fall out of the range of the result type or if the underlying function
*/
int CXmlNode::GetAttributeIntValue(const std::wstring &name, int def)
{
    std::shared_ptr<CXmlNode> attr = GetAttribute(name);
    if (attr == nullptr)
    {
        return def;
    }

    return attr->GetIntValue();
}

/**
*  Get the value of an attribute.
* \param name Name of the attribute
* \param def Default value to return if attribute does not exist.
* \returns Attribute value as a double or the default if does not exist.
* \throws std::invalid_argument if no conversion could be performed
* \throws std::out_of_range if the converted value would
* fall out of the range of the result type or if the underlying function
*/
double CXmlNode::GetAttributeDoubleValue(const std::wstring &name, double def)
{
    std::shared_ptr<CXmlNode> attr = GetAttribute(name);
    if (attr == nullptr)
    {
        return def;
    }

    return attr->GetDoubleValue();
}


/**
 *  Set an attribute on this node.
 * \param name Name of the attribute to set
 * \param val Value to set as a string
 */
void CXmlNode::SetAttribute(const std::wstring &name, const std::wstring &val)
{
    CComPtr<IXMLDOMElement> element;
    if (mNode.QueryInterface(&element) == S_OK)
    {
        element->setAttribute(CComBSTR(name.c_str()), CComVariant(val.c_str()));
    }
}

/**
*  Set an attribute on this node.
* \param name Name of the attribute to set
* \param val Value to set as an integer
*/
void CXmlNode::SetAttribute(const std::wstring &name, int val)
{
    CComPtr<IXMLDOMElement> element;
    if (mNode.QueryInterface(&element) == S_OK)
    {
        element->setAttribute(CComBSTR(name.c_str()), CComVariant(val));
    }
}

/**
*  Set an attribute on this node.
* \param name Name of the attribute to set
* \param val Value to set as a double
*/
void CXmlNode::SetAttribute(const std::wstring &name, double val)
{
    CComPtr<IXMLDOMElement> element;
    if (mNode.QueryInterface(&element) == S_OK)
    {
        element->setAttribute(CComBSTR(name.c_str()), CComVariant(val));
    }
}

/**
 *  Add a child node to this node.
 * \param name Name of the child node.
 * \returns CXmlNode object for the child node.
 */
std::shared_ptr<CXmlNode> CXmlNode::AddChild(const std::wstring &name)
{
    // Create an XML node for the item
    CComPtr<IXMLDOMNode> node;
    CComPtr<IXMLDOMElement> element;
    mDocument->GetDoc()->createElement(CComBSTR(name.c_str()), &element);
    mNode->appendChild(element, &node);

    return shared_ptr<CXmlNode>(new CXmlNode(this, node));
}

/** 
 *  Get a child node of this node by index
 * \param n Index into the child nodes
 * \returns The child node as a shared pointer
 */
std::shared_ptr<CXmlNode> CXmlNode::GetChild(int n)
{
    CComPtr<IXMLDOMNodeList> children;
    mNode->get_childNodes(&children);

    CComPtr<IXMLDOMNode> child;
    children->get_item(n, &child);

    return shared_ptr<CXmlNode>(new CXmlNode(this, child));
}


/**  Number of children of this node
 * \returns Number of children
 */
int CXmlNode::GetNumChildren()
{
    CComPtr<IXMLDOMNodeList> children;
    mNode->get_childNodes(&children);

    long len;
    children->get_length(&len);

    return (int)len;
}


/**
 *  Get the children of this node.
 *
 * This allows the children to be iterated over easily 
 * using C++ 11 range for loops:
 * \code
 * for(auto child : node->GetChildren())
 * {
 *     child->...
 * }
 * \endcode
 * \returns CXmlNode::Children object suitable to use as an iterator.
 */
CXmlNode::Children CXmlNode::GetChildren()
{
    return Children(this);
}


/**
 *  Get the beginning of the child node collection.
 * \returns Iterator at beginning of collection.
 */
CXmlNode::Iterator CXmlNode::Children::begin()
{
    return Iterator(this, 0);
}

/**
*  Get the end of the child node collection.
* \returns Iterator at end of collection.
*/
CXmlNode::Iterator CXmlNode::Children::end()
{
    long len;
    mChildren->get_length(&len);
    return Iterator(this, (int)len);
}


/**
 *  Constructor
 * \param node Node this class represents the children of.
 */
CXmlNode::Children::Children(CXmlNode *node)
{
    mNode = node;
    node->mNode->get_childNodes(&mChildren);
}


/**
 *  Operation * for the iterator. 
 *
 * Indiates the child that is the current iterator reference.
 *
 * \returns Pointer to child node.
 */
std::shared_ptr<CXmlNode>  CXmlNode::Iterator::operator* ()
{
    CComPtr<IXMLDOMNode> child;

    mChildren->mChildren->get_item(mPos, &child);

    return shared_ptr<CXmlNode>(new CXmlNode(mChildren->mNode, child));
}


/**
 *  Constructor
 */
CXmlNode::Document::Document()
{
    CoInitialize(NULL);
}


/**
 *  Destructor
 */
CXmlNode::Document::~Document()
{
    CoUninitialize();
}



/**  Open the document
 * \param filename File to open
 * \throws CXmlNode::Exception If there are errors loading document
 */
void CXmlNode::Document::Open(const std::wstring &filename)
{
    //
    // Create an XML document
    //
    HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
        IID_IXMLDOMDocument, (void**)&mDoc);
    if (hr != S_OK)
    {
        wstring err(L"Failed to create an XML document to use");
        throw Exception(Exception::UnableToCreate, err);
    }

    // Open the XML document
    VARIANT_BOOL ok;
    hr = mDoc->load(CComVariant(filename.c_str()), &ok);
    if (hr != S_OK || ok == VARIANT_FALSE)
    {
        wstring err(L"Unable to open file: ");
        err += filename;
        throw Exception(Exception::UnableToOpen, err);
    }

    //
    // Get the document root node
    //
    CComPtr<IXMLDOMElement> rootElement;
    if (mDoc->get_documentElement(&rootElement) != S_OK ||
        rootElement.QueryInterface(&mRoot) != S_OK)
    {
        wstring err(L"Unable to find a root element in file: ");
        err += filename;
        throw Exception(Exception::NoRoot, err);
    }

}

/**  Create a new, empty document.
* \param rootname The name for the document root node.
* \throws CXmlNode::Exception If there are errors creating the document
*/
void CXmlNode::Document::Create(const std::wstring &rootname)
{
    HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL,
        CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&mDoc);
    if (hr != S_OK)
    {
        wstring err(L"Failed to create an XML document to use");
        throw Exception(Exception::UnableToCreate, err);
    }

    // Create a processing instruction targeted for xml.
    // This creates the line:  <?xml version='1.0'>
    CComPtr<IXMLDOMProcessingInstruction> pi;
    CComPtr<IXMLDOMNode> pinode;
    mDoc->createProcessingInstruction(L"xml", L"version='1.0' encoding='UTF-8'", &pi);
    mDoc->appendChild(pi, &pinode);

    // Create the root element
    CComPtr<IXMLDOMElement> pe;
    mDoc->createElement(CComBSTR(rootname.c_str()), &pe);
    mDoc->appendChild(pe, &mRoot);
}


/**
 *  Save the document to a file.
 * \param filename Filename to save as.
 * \throws CXmlNode::Exception If there are errors in saving
 */
void CXmlNode::Document::Save(const std::wstring &filename)
{
    HRESULT hr = mDoc->save(CComVariant(filename.c_str()));
    if (hr != S_OK)
    {
        wstring err(L"Unable to write file: ");
        err += filename;
        throw Exception(Exception::UnableToWrite, err);
    }
}



/**
 *  Returns the document as a string.
 * \returns XML data as a string
 */
std::wstring CXmlNode::Document::GetXML()
{
    CComBSTR xml;
    mDoc->get_xml(&xml);
    return wstring(xml);
}

