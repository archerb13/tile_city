/**
 * \file City.h
 *
 * \author Charles B. Owen
 *
 *  Class that implements a simple city with tiles we can manipulate
 */

#pragma once

#include <memory>
#include <vector>
#include <map>
#include <utility>

#include "XmlNode.h"
#include "Tile.h"

/**
 *  Implements a simple city with tiles we can manipulate
 */
class CCity
{
public:
    CCity();
    virtual ~CCity();

    /// The spacing between grid locations
    static const int GridSpacing = 32;


    void Add(std::shared_ptr<CTile> item);
    std::shared_ptr<CTile> HitTest(int x, int y);
    void MoveToFront(std::shared_ptr<CTile> item);
    void DeleteItem(std::shared_ptr<CTile> item);

    void OnDraw(Gdiplus::Graphics *graphics);

    void Save(const std::wstring &filename);
    void Load(const std::wstring &filename);
    void Clear();

    void Update(double elapsed);
    void SortTiles();

    std::shared_ptr<CTile> GetAdjacent(std::shared_ptr<CTile> tile, int dx, int dy);
    std::shared_ptr<CTile> GetAdjacent(CTile *tile, int dx, int dy);

	void Accept(CTileVisitor * visitor);

	/** Iterator that iterates over the city tiles */
	class Iter
	{
	public:
		/** Constructor
		 * \param city The city we are iterating over
		 * \param pos The current position*/
		Iter(CCity *city, int pos) : mCity(city), mPos(pos) {}

		/** Test for end of the iterator
		 * \param other Other position to compare to 
		 * \returns True if we this position equals not equal to the other position*/
		bool operator!=(const Iter &other) const
		{
			return mPos != other.mPos;
		}

		/** Get value at current position
		 * \returns Value at mPos in the collection */
		std::shared_ptr<CTile> operator *() const { return mCity->mTiles[mPos]; }

		/** Increment the iterator
		 * \returns Reference to this iterator */
		const Iter& operator++()
		{
			mPos++;
			return *this;
		}


	private:
		CCity *mCity;   ///< City we are iterating over
		int mPos;       ///< Position in the collection
	};


	/** Get an iterator for the beginning of the collection
	* \returns Iter object at position 0 */
	Iter begin() { return Iter(this, 0); }

	/** Get an iterator for the end of the collection
	 * \returns Iter object at position past the end */
	Iter end() { return Iter(this, mTiles.size()); }

	/// Get the empty city
	/// \returns the empty city
	bool EmptyCity() { return mEmptyCity; }

private:
    void XmlTile(const std::shared_ptr<xmlnode::CXmlNode> &node);
    void BuildAdjacencies();

    /// All of the tiles that make up our city
    std::vector<std::shared_ptr<CTile> > mTiles;

    /// Adjacency lookup support
    std::map<std::pair<int, int>, std::shared_ptr<CTile> > mAdjacency;

	/// Bool for empty city
	bool mEmptyCity = true;
};

