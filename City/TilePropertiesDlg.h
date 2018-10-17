/**
 * \file TilePropertiesDlg.h
 *
 * \author Charles B. Owen
 *
 *  The tile properties dialog box
 */

#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "Tile.h"

/**  The tile properties dialog box
*/
class CTilePropertiesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTilePropertiesDlg)

public:
	CTilePropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTilePropertiesDlg();

	/// Dialog box ID
	enum { IDD = IDD_TILEEDITDLG };

    /**  The zoning selected by the dialog box
     * \returns Edited zoning value */
    CTile::Zonings GetZoning() { return m_zoning; }

    /**  Initial zoning for the dialog box 
     * \param zoning Initial zoning value */
    void SetZoning(CTile::Zonings zoning) { m_zoning = zoning; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
    CComboBox m_zoningCtrl;     ///< The combo box control for zoning
    CTile::Zonings m_zoning;    ///< The zoning value
};
