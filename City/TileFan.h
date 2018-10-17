/**
 * \file TileFan.h
 *
 * \author Bryce Archer
 *
 * Class to define your tile fan
 */

#pragma once
#include "Tile.h"


/**
 * Class to define the fan tile
 */
class CTileFan : public CTile
{
public:
	CTileFan(CCity * city);
	void LoadImage(std::unique_ptr<Gdiplus::Bitmap>& image, std::wstring name);

	void Draw(Gdiplus::Graphics * graphics);

	void Update(double elapsed);

	virtual ~CTileFan();

	// Sets the State for the image
	void SetState();

	// Resests everything for new season
	void ResetSeason();

	/** Accept a visitor
	* \param visitor The visitor we accept */
	virtual void Accept(CTileVisitor *visitor) override { visitor->VisitFan(this); }

	/** Adds to play 
	* \return mPlayCount adds one to play count when called */
	int Play() { return ++mPlayCount; }

private:
	/// The image of the first UMich guy
	std::unique_ptr<Gdiplus::Bitmap> mUMichOneImage;

	/// The image of the second UMich guy
	std::unique_ptr<Gdiplus::Bitmap> mUMichTwoImage;

	/// Sets the three states for the image
	enum States { first, second, third };
	/// Sets the state of the image to default to first
	States mState = first;

	/// Makes the play count
	int mPlayCount = 0;

	/// Sets the run speed
	double mRunX = 0;

	/// Sets random speed
	double mRandom;
};

