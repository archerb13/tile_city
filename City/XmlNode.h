/**
 * \file
 *
 * \author Charles B. Owen
 *
 *  Class that implements a wrapper for msxml nodes. 
 */
#pragma once

#include <MsXml6.h>
#include <memory>
#include <string>
#include <exception>

namespace xmlnode
{

    /**
     *  A wrapper for msxml nodes.

     * MSXML was designed to be used by a variety of languages,
     * including C++, C#, and Visual Basic. Consequently, it is
     * a compromise that is very cumbersome to use. This class
     * is designed to simplify the process considerably.
     *
     * All an XML document is a tree of nodes. This class
     * represents nodes in that tree and the root node represents
     * that underlying document. This class can be used to
     * both create and load XML documents.
     *
     * \version 1.01 07-16-2014 Initial development 
     * \version 1.02 07-17-2014 Namespace, added GetXML, added tests
     * \version 1.03 07-17-2014 Exceptions
     */
    class CXmlNode
    {
    public:
        CXmlNode();
        virtual ~CXmlNode();

        /*
         * Document management functions
         */
        static std::shared_ptr<CXmlNode> OpenDocument(const std::wstring &filename);
        static std::shared_ptr<CXmlNode> CreateDocument(const std::wstring &rootname);

        void Open(const std::wstring &filename);
        void Create(const std::wstring &rootname);

        void Save(const std::wstring &filename);
        std::wstring GetXML();

        /*
         * Node name, type, and value
         */
        std::wstring GetName() const;
        DOMNodeType GetType() const;
        std::wstring GetValue() const;

        int GetIntValue() const;
        double GetDoubleValue() const;

        /*
         * Attributes
         */
        std::shared_ptr<CXmlNode> GetAttribute(const std::wstring &name);

        std::wstring GetAttributeValue(const std::wstring &name, const std::wstring &def);
        int GetAttributeIntValue(const std::wstring &name, int def);
        double GetAttributeDoubleValue(const std::wstring &name, double def);

        void SetAttribute(const std::wstring &name, const std::wstring &val);
        void SetAttribute(const std::wstring &name, int val);
        void SetAttribute(const std::wstring &name, double val);

        class Children;

        /**
         *  Support for iterating over the children of a node
         */
        class Iterator
        {
        public:
            friend class CXmlNode;

            /**  Test to see if two iterator are at the same location
             * \param other The other object we are testing against
             * \returns true if they are equal.
             */
            bool operator!= (const Iterator& other) const
            {
                return mPos != other.mPos;
            }

            // this method must be defined after the definition of IntVector
            // since it needs to use it
            std::shared_ptr<CXmlNode> operator* ();

            /**  Advance to the next item in the collection
             * \returns Reference to this iterator.
             */
            const Iterator& operator++ ()
            {
                ++mPos;
                return *this;
            }

        private:
            /**  Constructor
            * \param children The children object we iterate over
            * \param pos Initial position in the collection
            */
            Iterator(Children* children, int pos)
                : mPos(pos)
                , mChildren(children)
            {
            }

            int mPos;            ///< Iterator position in the collection
            Children *mChildren; ///< Pointer to the children object for the collection
        };

        /**
         *  Representation of children to support iteration
         */
        class Children
        {
        public:
            Iterator begin();
            Iterator end();

            friend class Iterator;
            friend class CXmlNode;

        private:
            Children(CXmlNode *node);

            CXmlNode *mNode;                    ///< Node we are children of
            CComPtr<IXMLDOMNodeList> mChildren; ///< Node list of children
        };

        /**
         *  Exceptions for CXmlNode
         */
        class Exception : public std::exception
        {
        public:
            /**  Exception types */
            enum Types {
                None,           ///< No exception type indicated
                UnableToOpen,   ///< Unable to open file to read
                UnableToWrite,  ///< Unable to open file to write
                UnableToCreate, ///< Unable to create XML document
                NoRoot          ///< Not XML document root node
            };

            /**  Default Constructor */
            Exception() {}

            /**  Copy Constructor 
             * \param other Object to copy */
            Exception(const Exception &other) : mType(other.mType), mMsg(other.mMsg) {}
            
            /**  Assignment operator
            * \param other Object to copy 
            * \returns Reference to this object */
            Exception& operator= (const Exception &other) { mMsg = other.mMsg; mType = other.mType; }
            
            /**  Constructor
             * \param type Exception type
             * \param msg Message associated with exception */
            Exception(Types type, const std::wstring &msg) : mType(type), mMsg(msg) {}
            
            /**  Destructor */
            virtual ~Exception() {}

            /**  Exception message
             *
             * More verbose exception messages are provided in 
             * Unicode as they should be by the Message() function.
             * \returns "CXmlNode exception" */
            virtual const char* what() const throw()
            {
                return "CXmlNode exception.";
            }

            /**  Exception message
            * \returns Exception message */
            std::wstring Message() { return mMsg; }

            /**  Exception type
            * \returns Exception type of type CXmlNode::Exception::Types */
            Types Type() { return mType; }

        private:
            Types mType = None; ///< Exception type
            std::wstring mMsg;  ///< Exception error message
        };

        std::shared_ptr<CXmlNode> GetChild(int n);

        Children GetChildren();
        int GetNumChildren();

        /*
        * Support for adding new children
        */
        std::shared_ptr<CXmlNode> AddChild(const std::wstring &name);

    private:
        CXmlNode(CXmlNode *parent, IXMLDOMNode *node);

        ///  Nested class for an object that describes one XML document
        class Document
        {
        public:
            Document();
            ~Document();

            void Open(const std::wstring &filename);
            void Create(const std::wstring &rootname);
            void Save(const std::wstring &filename);
            std::wstring GetXML();

            /**  Get pointer to the root node
             * \returns Pointer to root node */
            IXMLDOMNode *GetRoot() { return mRoot; }

            /**  Get pointer to underlying document
             * \returns Pointer to underlying document */
            IXMLDOMDocument *GetDoc() { return mDoc; }

        private:
            CComPtr<IXMLDOMDocument>  mDoc; ///< Underlying msxml document
            CComPtr<IXMLDOMNode> mRoot;     ///< Document root node
        };

        // The open document
        std::shared_ptr<Document> mDocument;    ///< Reference to the document
        CComPtr<IXMLDOMNode> mNode;             ///< msxml node
    };

}
