/**
 * \file TileFan.cpp
 *
 * \author Bryce Archer
 */

#include "stdafx.h"
#include "TileFan.h"

#include <sstream>

using namespace std;
using namespace Gdiplus;

/// Image for background
const wstring TileBackground = L"grass.png";

/// How much we offset drawing the tile to the left of the center
const static int OffsetLeft = 64;

/// How much we offset drawing the tile above the center
const static int OffsetDown = 32;

/// Set const speed for UMich image
const double mSpeedX = 50;

/** Constructor
* \param city The city this is a member of
*/
CTileFan::CTileFan(CCity *city) : CTile(city)
{
	SetImage(TileBackground);

	LoadImage(mUMichOneImage, L"umguy1.png");
	LoadImage(mUMichTwoImage, L"umguy2.png");
	mRandom = ((double)rand() / RAND_MAX) * mSpeedX;
}

/**
* Load an image into a bitmap
* \param image Image pointer to load
* \param name Filename to load from
*/
void CTileFan::LoadImage(std::unique_ptr<Gdiplus::Bitmap> &image, std::wstring name)
{
	wstring filename = ImagesDirectory + name;
	image = unique_ptr<Bitmap>(Bitmap::FromFile(filename.c_str()));
	if (image->GetLastStatus() != Ok)
	{
		wstring msg(L"Failed to open ");
		msg += filename;
		AfxMessageBox(msg.c_str());
	}
}

/** Draw this item
* \param graphics The graphics context to draw on */
void CTileFan::Draw(Gdiplus::Graphics *graphics)
{
	CTile::Draw(graphics);

	SetState();

	if (mState == first)
	{
		int wid = mUMichOneImage->GetWidth();
		int hit = mUMichOneImage->GetHeight();

		graphics->DrawImage(mUMichOneImage.get(),
			(int)(GetX() - OffsetLeft), (int)(GetY() + OffsetDown - hit),
			wid, hit);
	}
	if (mState == second)
	{
		int wid = mUMichTwoImage->GetWidth();
		int hit = mUMichTwoImage->GetHeight();

		graphics->DrawImage(mUMichTwoImage.get(),
			(int)(GetX() - OffsetLeft), (int)(GetY() + OffsetDown - hit),
			wid, hit);
	}
	if (mState == third)
	{
		int wid = mUMichTwoImage->GetWidth();
		int hit = mUMichTwoImage->GetHeight();

		graphics->DrawImage(mUMichTwoImage.get(),
			(int)(GetX() - OffsetLeft) + mRunX, (int)(GetY() + OffsetDown - hit),
			wid, hit);
	}

}

/**
* Called before the image is drawn
* \param elapsed Time since last draw
*/
void CTileFan::Update(double elapsed)
{
	CTile::Update(elapsed);
	

	// when the Surrender Cobra is running...
	// mSpeedX is a constant pixels per second running speed...
	if (mState == third)
	{
		mRunX += mRandom * elapsed;
	}
}


CTileFan::~CTileFan()
{
}


/**
 * Sets the state of the image
 */
void CTileFan::SetState()
{
	if (mPlayCount <= 0)
	{
		mState = first;
	}
	if (mPlayCount > 0)
	{
		mState = second;
	}
	if (mPlayCount >= 3)
	{
		mState = third;
	}
}

/**
* Resets count and speed
*/
void CTileFan::ResetSeason()
{
	mPlayCount = 0;
	mRunX = 0;
}
