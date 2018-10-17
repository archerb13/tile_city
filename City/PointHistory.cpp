/**
 * \file PointHistory.cpp
 *
 * \author Charles Owen
 */


#include "stdafx.h"
#include "PointHistory.h"

using namespace std;
using namespace Gdiplus;

/**
* Constructor
*/
CPointHistory::CPointHistory()
{
	// Lists are initially empty
	mHead = nullptr;
	mTail = nullptr;
}


/**
 * Destructor
 */
CPointHistory::~CPointHistory()
{
}



/**
* Add a pointer to the collection of points.
* \param point Point to add
*/
void CPointHistory::Add(Gdiplus::Point point)
{
	// On first add we create the first node
	if (mHead == nullptr)
	{
		mHead = make_shared<MiniTreeNode>(point);
		mTail = mHead;
		return;
	}

	// Try to add the point to the last node
	if (!mTail->Add(point, mTail)) 
	{
		// If add failed, create a new tail node
		auto newTail = make_shared<MiniTreeNode>(point);
		mTail->SetNext(newTail);
		mTail = newTail;
	}
}

/**
 * Add a point to this MiniTree.
 * \param point Point to add
 * \param me Pointer to this node as shared_ptr
 * \return True if successful, false if full
 */
bool CPointHistory::MiniTreeNode::Add(Gdiplus::Point point,
	std::shared_ptr<MiniTreeNode> me
	)
{
	if (mLeft == nullptr)
	{
		// There is no left tree. Create a left tree
		// with the current value and add new value as 
		// the node's value.
		mLeft = make_shared<MiniTreeNode>(mPoint);
		mLeft->mParent = me;
		mPoint = point;
		return true;
	}

	if (mRight == nullptr)
	{
		// There is no right tree. Create a right tree
		// with the new value.
		mRight = make_shared<MiniTreeNode>(point);
		mRight->mParent = me;
		return true;
	}

	// We are full
	return false;
}
