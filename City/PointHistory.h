/**
* \file PointHistory.cpp
*
* \author Charles Owen
*
* Class that maintains a collection of Point objects in order.
*/

#pragma once
#include <memory>

/**
 * Class that maintains a collection of Point objects in order.
 */
class CPointHistory
{
public:
	CPointHistory();
	virtual ~CPointHistory();

	void Add(Gdiplus::Point p);

protected:
	/**
	* Nested class that stores up to five points in a bin.
	*/
	class MiniTreeNode
	{
	public:
		/// Maximum number of points we can put into a bin
		const static int MaxPoints = 5;

		/**
		 * Constructor
		 * \param point The point for node
		 */
		MiniTreeNode(Gdiplus::Point point) : mPoint(point) {}

		/// Destructor
		virtual ~MiniTreeNode() {}

		bool Add(Gdiplus::Point point, std::shared_ptr<MiniTreeNode> me);

		/// Set the stored point
		/// \param point Point to save
		void Set(Gdiplus::Point point) { mPoint = point; }

		/// Get a stored point
		/// \return Stored point value
		Gdiplus::Point Get() { return mPoint; }

		/// Get the next bin in the list
		/// \return Pointer to next bin or null if none
		std::shared_ptr<MiniTreeNode> GetNext() { return mNext; }

		/// Get the left subtree
		/// \return Pointer to left subtree or null if none
		std::shared_ptr<MiniTreeNode> GetLeft() { return mLeft; }

		/// Get the right subtree
		/// \return Pointer to right subtree or null if none
		std::shared_ptr<MiniTreeNode> GetRight() { return mRight; }

		/// Get the parent node
		/// \return Pointer to parent node or null if none
		std::shared_ptr<MiniTreeNode> GetParent() { return mParent.lock(); }

		/// Set the next bin in the list
		/// \param next Next pointer to set
		void SetNext(std::shared_ptr<MiniTreeNode> next) { mNext = next; }

	private:
		/// The point stored in this tree node.
		Gdiplus::Point mPoint;

		/// Parent of this MiniTree or null if root tree
		std::weak_ptr<MiniTreeNode> mParent;

		/// Left sub-tree
		std::shared_ptr<MiniTreeNode> mLeft;

		/// Right sub-tree
		std::shared_ptr<MiniTreeNode> mRight;

		/// Next MiniTree object
		std::shared_ptr<MiniTreeNode> mNext;
	};
	
	/// Pointer to the head of the linked list of MiniTrees
	std::shared_ptr<MiniTreeNode> mHead;

private:
	/// Pointer to the tail of the linked list of MiniTrees (last node)
	std::shared_ptr<MiniTreeNode> mTail;

public:
	/** Iterator that iterates over the point history */
	class Iter
	{
	public:
		/**
		* Constructor
		* \param history The history object we are iterating over
		*/
		Iter(CPointHistory *history, std::shared_ptr<MiniTreeNode> position) :
			mHistory(history), mPosition(position)
		{
		}

		/** Test for end of the iterator
		* \returns True if we this position equals not equal to the other position */
		bool operator!=(const Iter &other) const
		{
			return mPosition != other.mPosition;
		}

		/** Get value at current position
		 * \returns Value at mPosition in the collection
		 */
		Gdiplus::Point operator *() const { return mPosition->Get(); }

		/** Increment the iterator
		* \returns Reference to this iterator */
		const Iter& operator++()
		{
			// Nowhere to go if at end of list
			if (mPosition == nullptr)
			{
				return *this;
			}

			//
			// There are three possibilities here:
			// 
			// 1. We are a left subtree
			// 2. We are a root node
			// 3. We are a right subtree
			//
			auto parent = mPosition->GetParent();
			if (parent != nullptr && parent->GetLeft() == mPosition)
			{
				// We are the left subtree. Move up to the root
				mPosition = parent;
			}
			else if (parent == nullptr)
			{
				// We are the root of the tree
				// Move to the right subtree if it exists or the next node if not
				if (mPosition->GetRight() != nullptr)
				{
					mPosition = mPosition->GetRight();
				}
				else
				{
					mPosition = mPosition->GetNext();
					if (mPosition != nullptr && mPosition->GetLeft() != nullptr)
					{
						mPosition = mPosition->GetLeft();
					}
				}
			}
			else
			{
				// We are the right subtree
				// Move to the next node
				mPosition = parent->GetNext();
				if (mPosition != nullptr && mPosition->GetLeft() != nullptr)
				{
					mPosition = mPosition->GetLeft();
				}
			}

			return *this;
		}


	private:
		/// History we are iterating over
		CPointHistory *mHistory;

		/// Current position in the collection
		std::shared_ptr<MiniTreeNode> mPosition;
	};


	/**
	 * Get an iterator for the beginning of the collection
	 * \returns Iter object at position 0
	 */
	Iter begin() {
		if (mHead == nullptr)
		{
			return Iter(this, nullptr);
		}

		if (mHead->GetLeft() != nullptr)
		{
			return Iter(this, mHead->GetLeft());
		}
		return Iter(this, mHead);
	}

	/** Get an iterator for the end of the collection
	* \returns Iter object at position past the end */
	Iter end() { return Iter(this, nullptr); }
};
