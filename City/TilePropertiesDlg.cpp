/**
 * \file TilePropertiesDlg.cpp
 *
 * \author Charles B. Owen
 */

#include "stdafx.h"
#include "City.h"
#include "TilePropertiesDlg.h"
#include "afxdialogex.h"


// CTilePropertiesDlg dialog

IMPLEMENT_DYNAMIC(CTilePropertiesDlg, CDialogEx)

/**  Constructor
 * \param pParent Any parent window
 */
CTilePropertiesDlg::CTilePropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTilePropertiesDlg::IDD, pParent)
{
}

/**  Destructor
 */
CTilePropertiesDlg::~CTilePropertiesDlg()
{
}

/**  Exchange data between the dialog box and the class
 * \param pDX the data exchange control object
 */
void CTilePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ZONING, m_zoningCtrl);

    if (!pDX->m_bSaveAndValidate)
    {
        m_zoningCtrl.AddString(L"None");
        m_zoningCtrl.AddString(L"Residential");
        m_zoningCtrl.AddString(L"Industrial");
        m_zoningCtrl.AddString(L"Agricultural");

        m_zoningCtrl.SetCurSel((int)m_zoning);
    }
    else
    {
        m_zoning = (CTile::Zonings)m_zoningCtrl.GetCurSel();
    }
}


BEGIN_MESSAGE_MAP(CTilePropertiesDlg, CDialogEx)
END_MESSAGE_MAP()


// CTilePropertiesDlg message handlers
