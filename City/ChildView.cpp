/**
 * \file ChildView.cpp
 *
 * \author Charles B. Owen
 */

#include "stdafx.h"
#include "DoubleBufferDC.h"
#include "CityApp.h"
#include "ChildView.h"
#include "Tile.h"
#include "TileLandscape.h"
#include "TileBuilding.h"
#include "TileRoad.h"
#include "TileCoalmine.h"
#include "BuildingCounter.h"
#include "TileFan.h"
#include "FanCount.h"
#include "FanSeason.h"

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;
using namespace std;

/// Frame duration in milliseconds
const int FrameDuration = 100;


/// Initial tile X location
const int InitialX = CCity::GridSpacing * 5;

/// Initial tile Y location
const int InitialY = CCity::GridSpacing * 3;

/// Margin of trashcan from side and bottom in pixels
const int TrashcanMargin = 5;

/**
 * Constructor
 */
CChildView::CChildView()
{
    srand((unsigned int)time(nullptr));

    // Load the trash can image
    mTrashcan = unique_ptr<Bitmap>(Bitmap::FromFile(L"images/trashcan.png"));
    if (mTrashcan->GetLastStatus() != Ok)
    {
        AfxMessageBox(L"Failed to open images/trashcan.png");
    }
}

/**
 * Destructor
 */
CChildView::~CChildView()
{
}

/// \cond
/// This doxygen switch tells it to ignore this block of code
BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_COMMAND(ID_FILE_OPEN, &CChildView::OnFileOpen)
    ON_COMMAND(ID_FILE_SAVEAS, &CChildView::OnFileSaveas)
    ON_COMMAND(ID_BUILDINGS_FARMHOUSE, &CChildView::OnBuildingsFarmhouse)
    ON_COMMAND(ID_LANDSCAPING_GRASS, &CChildView::OnLandscapingGrass)
    ON_COMMAND(ID_LANDSCAPING_SPARTYSTATUE, &CChildView::OnLandscapingSpartystatue)
    ON_COMMAND(ID_BUILDINGS_BLACKSMITHSHOP, &CChildView::OnBuildingsBlacksmithshop)
    ON_COMMAND(ID_LANDSCAPING_TALLGRASS, &CChildView::OnLandscapingTallgrass)
    ON_COMMAND(ID_BUILDINGS_BROWNHOUSE, &CChildView::OnBuildingsBrownhouse)
    ON_COMMAND(ID_BUILDINGS_YELLOWHOUSE, &CChildView::OnBuildingsYellowhouse)
    ON_COMMAND(ID_BUILDINGS_F, &CChildView::OnBuildingsF)
    ON_COMMAND(ID_BUILDINGS_HOSPITAL, &CChildView::OnBuildingsHospital)
    ON_COMMAND(ID_BUILDINGS_MARKET, &CChildView::OnBuildingsMarket)
    ON_COMMAND(ID_BUILDINGS_CONDOS, &CChildView::OnBuildingsCondos)
    ON_COMMAND(ID_LANDSCAPING_TREE, &CChildView::OnLandscapingTree)
    ON_COMMAND(ID_LANDSCAPING_TREES, &CChildView::OnLandscapingTrees)
    ON_COMMAND(ID_LANDSCAPING_BIGTREES, &CChildView::OnLandscapingBigtrees)
    ON_COMMAND(ID_LANDSCAPING_ROAD, &CChildView::OnLandscapingRoad)
    ON_COMMAND(ID_BUSINESSES_COALMINE, &CChildView::OnBusinessesCoalmine)
    ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_BUILDINGS_COUNT, &CChildView::OnBuildingsCount)
	ON_COMMAND(ID_BIGTEN_PLAY, &CChildView::OnBigtenPlay)
	ON_UPDATE_COMMAND_UI(ID_BIGTEN_PLAY, &CChildView::OnUpdateBigtenPlay)
	ON_COMMAND(ID_BIGTEN_MICHIGANFAN, &CChildView::OnBigtenMichiganfan)
	ON_COMMAND(ID_BIGTEN_NEWSEASON, &CChildView::OnBigtenNewseason)
	ON_UPDATE_COMMAND_UI(ID_BIGTEN_NEWSEASON, &CChildView::OnUpdateBigtenNewseason)
	ON_COMMAND(ID_VIEW_MOUSETRACKS, &CChildView::OnViewMousetracks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOUSETRACKS, &CChildView::OnUpdateViewMousetracks)
END_MESSAGE_MAP()
/// \endcond



/** 
* This function is called before the window is created.
* \param cs A structure with the window creation parameters
* \returns TRUE
*/
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

/** 
* This function is called to draw in the window.
*
* This function is called in response to a drawing message
* whenever we need to redraw the window on the screen.
* It is responsible for painting the window.
*/void CChildView::OnPaint()
{
    CPaintDC paintDC(this);     // device context for painting
    CDoubleBufferDC dc(&paintDC); // device context for painting

    Graphics graphics(dc.m_hDC);
    graphics.Clear(Color(0, 0, 0));

    if (mFirstDraw)
    {
        mFirstDraw = false;
        SetTimer(1, FrameDuration, nullptr);

        /*
        * Initialize the elapsed time system
        */
        LARGE_INTEGER time, freq;
        QueryPerformanceCounter(&time);
        QueryPerformanceFrequency(&freq);

        mLastTime = time.QuadPart;
        mTimeFreq = double(freq.QuadPart);
    }

    /*
    * Compute the elapsed time since the last draw
    */
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    long long diff = time.QuadPart - mLastTime;
    double elapsed = double(diff) / mTimeFreq;
    mLastTime = time.QuadPart;

    mCity.Update(elapsed);

    // Get the size of the window
    CRect rect;
    GetClientRect(&rect);

    /*
     * Draw the trash can
     */
    
    // Bottom minus image size minus margin is top of the image
    mTrashcanTop = rect.Height() - mTrashcan->GetHeight() - TrashcanMargin;
    mTrashcanRight = TrashcanMargin + mTrashcan->GetWidth();

    graphics.DrawImage(mTrashcan.get(), TrashcanMargin, mTrashcanTop,
        mTrashcan->GetWidth(), mTrashcan->GetHeight());

    /*
     * Actually Draw the city
     */
    mCity.OnDraw(&graphics);

	Pen pen(Color::Green, 2);

	for (auto tile : mCity)
	{
		tile->DrawBorder(&graphics, &pen);
	}

	if (mMouseTracks)
	{
		Pen pen(Color(255, 255, 0));
		bool first = true;
		Point p1;

		for (auto p2 : mMouseHistory)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				graphics.DrawLine(&pen, p1, p2);
			}

			p1 = p2;
		}
	}
}


/**
*  Erase the background prior to drawing.
*
* We return true so this does nothing. This prevents the flicker
* of a repaint of white before actual drawing.
*
* \param pDC A device context to draw on
* \returns TRUE
*/
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
    // This disabled the default background erase
    return TRUE;
}

/**  Handle a left button double-click on a tile
* \param nFlags Mouse flags
* \param point Where we clicked
*/
void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    auto tile = mCity.HitTest(point.x, point.y);
    if (tile != nullptr) 
    {
        // We double-clicked on a tile
        // Bring up the tile editing dialog box
        tile->PropertiesDlg();
        Invalidate();
    }

}



/**  Called when there is a left mouse button press
* \param nFlags Flags associated with the mouse button press
* \param point Where the button was pressed
*/
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mMouseHistory.Add(Point(point.x, point.y));

    mGrabbedItem = mCity.HitTest(point.x, point.y);
	if (mGrabbedItem != nullptr)
	{
		if (!mFeature)
		{
			// We grabbed something
			// Move it to the front
			mCity.MoveToFront(mGrabbedItem);

			Invalidate();
		}

		else
		{
			CFanCount visitor;
			mGrabbedItem->Accept(&visitor);
			mGrabbedItem = nullptr;
		}
	}
}

/**  Called when the left mouse button is released
* \param nFlags Flags associated with the mouse button release
* \param point Where the button was pressed
*/
void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
    OnMouseMove(nFlags, point);
}



/**  Called when the mouse is moved
* \param nFlags Flags associated with the mouse movement
* \param point Where the button was pressed
*/
void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	mMouseHistory.Add(Point(point.x, point.y));

    // See if an item is currently being moved by the mouse
    if (mGrabbedItem != nullptr)
    {
        // If an item is being moved, we only continue to 
        // move it while the left button is down.
        if (nFlags & MK_LBUTTON)
        {
            mGrabbedItem->SetLocation(point.x, point.y);
        }
        else
        {
            // When the left button is released we release
            // the item. If we release it on the trashcan,
            // delete it.
            if (point.x < mTrashcanRight && point.y > mTrashcanTop)
            {
                // We have clicked on the trash can
                mCity.DeleteItem(mGrabbedItem);
            }
            else
            {
                mGrabbedItem->QuantizeLocation();
            }

            mCity.SortTiles();
            mGrabbedItem = nullptr;
        }

        // Force the screen to redraw
        Invalidate();
    }
}

/**
*  Handle timer events
* \param nIDEvent The timer event ID
*/
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    Invalidate();
    CWnd::OnTimer(nIDEvent);
}



/**
*  Handler for the File/Save As menu option
*/
void CChildView::OnFileSaveas()
{
    CFileDialog dlg(false,  // false = Save dialog box
        L".city",           // Default file extension
        nullptr,            // Default file name (none)
        OFN_OVERWRITEPROMPT,    // Flags
        L"City Files (*.city)|*.city|All Files (*.*)|*.*||");    // Filter
    if (dlg.DoModal() != IDOK)
        return;

    wstring filename = dlg.GetPathName();

    mCity.Save(filename);
}


/**  This function is called when an File Open menu item is selected.
*
* It loads the city from a file.
*/
void CChildView::OnFileOpen()
{
    CFileDialog dlg(true,  // true = Open dialog box
        L".city",           // Default file extension
        nullptr,            // Default file name (none)
        0,    // Flags
        L"City Files (*.city)|*.city|All Files (*.*)|*.*||");    // Filter
    if (dlg.DoModal() != IDOK)
        return;

    wstring filename = dlg.GetPathName();
    mCity.Load(filename);
    Invalidate();
}



/**
 *  Add a CTileBuilding tile to the drawing.
 * \param file The file for the building image
 */
void CChildView::AddBuilding(const std::wstring &file)
{
    auto tile = make_shared<CTileBuilding>(&mCity);
    tile->SetImage(file);
    tile->SetLocation(InitialX, InitialY);
    mCity.Add(tile);
    Invalidate();
}


/**
*  Add a CTileLandscaping tile to the drawing.
* \param file The file for the landscape image
*/
void CChildView::AddLandscape(const std::wstring &file)
{
    auto tile = make_shared<CTileLandscape>(&mCity);
    tile->SetImage(file);
    tile->SetLocation(InitialX, InitialY);
    mCity.Add(tile);
    Invalidate();
}

/**
 * \cond
 *
 * I'm not going to document these menu handlers, since what they 
 * so is obvious. I'm using a Doxygen feature to ignore the functions
 */


void CChildView::OnBuildingsFarmhouse()
{
    AddBuilding(L"farm0.png");
}


void CChildView::OnLandscapingGrass()
{
    AddLandscape(L"grass.png");
}


void CChildView::OnLandscapingSpartystatue()
{
    AddLandscape(L"sparty.png");
}


void CChildView::OnBuildingsBlacksmithshop()
{
    AddBuilding(L"blacksmith.png");
}


void CChildView::OnLandscapingTallgrass()
{
    AddLandscape(L"tallgrass.png");
}


void CChildView::OnBuildingsBrownhouse()
{
    AddBuilding(L"house.png");
}


void CChildView::OnBuildingsYellowhouse()
{
    AddBuilding(L"yellowhouse.png");
}


void CChildView::OnBuildingsF()
{
    AddBuilding(L"firestation.png");
}


void CChildView::OnBuildingsHospital()
{
    AddBuilding(L"hospital.png");
}


void CChildView::OnBuildingsMarket()
{
    AddBuilding(L"market.png");
}


void CChildView::OnBuildingsCondos()
{
    AddBuilding(L"condos.png");
}


void CChildView::OnLandscapingTree()
{
    AddLandscape(L"tree.png");
}


void CChildView::OnLandscapingTrees()
{
    AddLandscape(L"tree2.png");
}


void CChildView::OnLandscapingBigtrees()
{
    AddLandscape(L"tree3.png");
}


void CChildView::OnLandscapingRoad()
{
    auto tile = make_shared<CTileRoad>(&mCity);
    tile->SetLocation(InitialX, InitialY);
    mCity.Add(tile);
    Invalidate();
}


void CChildView::OnBusinessesCoalmine()
{
    auto tile = make_shared<CTileCoalmine>(&mCity);
    tile->SetLocation(InitialX, InitialY);
    mCity.Add(tile);
    Invalidate();
}

/** \endcond */

/**
*  Counts the buildings.
*/
void CChildView::OnBuildingsCount()
{
	CBuildingCounter visitor;
	mCity.Accept(&visitor);
	int cnt = visitor.GetNumBuildings();

	wstringstream str;
	str << L"There are " << cnt << L" buildings.";
	AfxMessageBox(str.str().c_str());
}

/**
*  Sets bool for check in Play
*/
void CChildView::OnBigtenPlay()
{
	mFeature = !mFeature;
}

/**
*  Enables play when fan on field
\param pCmdUI helps with enabling and disabling menu
*/
void CChildView::OnUpdateBigtenPlay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(false);  		// Will disable the menu option
	if (mCount > 0)
	{
		pCmdUI->Enable(true);  		// Will enable the menu option
	}
	
	if (mCount <= 0)
	{
		pCmdUI->Enable(false);  		// Will disable the menu option
	}
	pCmdUI->SetCheck(mFeature);
}

/**
* Adds one to count when fan added
* Adds a fan
*/
void CChildView::OnBigtenMichiganfan()
{
	++mCount;
	auto tile = make_shared<CTileFan>(&mCity);
	tile->SetLocation(InitialX, InitialY);
	mCity.Add(tile);
	Invalidate();
}

/**
* Sends the visitor to fan for new season to work.
*/
void CChildView::OnBigtenNewseason()
{
	CFanSeason visitor;
	mCity.Accept(&visitor);
}

/**
* Activates New season when fan enters.
\param pCmdUI helps with enabling and disabling menu
*/
void CChildView::OnUpdateBigtenNewseason(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(false);  		// Will disable the menu option
	if (mCount > 0)
	{
		pCmdUI->Enable(true);  		// Will enable the menu option
	}

	if (mCount <= 0)
	{
		pCmdUI->Enable(false);  		// Will disable the menu option
	}
	//pCmdUI->SetCheck(mNewSeason);
}


void CChildView::OnViewMousetracks()
{
	mMouseTracks = !mMouseTracks;
	Invalidate();
}


void CChildView::OnUpdateViewMousetracks(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(mMouseTracks);
}
