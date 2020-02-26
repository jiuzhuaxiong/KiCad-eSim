/**
 * @file dialog_export_idf.cpp
 */

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013-2015  Cirilo Bernardo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <wxPcbStruct.h>
#include <kiface_i.h>
#include <pcbnew.h>
#include <class_board.h>
#include <convert_from_iu.h>

// IDF export header generated by wxFormBuilder
#include <dialog_export_idf_base.h>

#define OPTKEY_IDF_THOU wxT( "IDFExportThou" )
#define OPTKEY_IDF_REF_AUTOADJ wxT( "IDFRefAutoAdj" )
#define OPTKEY_IDF_REF_UNITS wxT( "IDFRefUnits" )
#define OPTKEY_IDF_REF_X wxT( "IDFRefX" )
#define OPTKEY_IDF_REF_Y wxT( "IDFRefY" )


/**
 * Function Export_IDF3
 * Creates an IDF3 compliant BOARD (*.emn) and LIBRARY (*.emp) file.
 *
 * @param aPcb = a pointer to the board to be exported to IDF
 * @param aFullFileName = the full filename of the export file
 * @param aUseThou = set to true if the desired IDF unit is thou (mil)
 * @param aXRef = the board Reference Point in mm, X value
 * @param aYRef = the board Reference Point in mm, Y value
 * @return true if OK
 */
bool Export_IDF3( BOARD *aPcb, const wxString & aFullFileName, bool aUseThou,
                  double aXRef, double aYRef );


class DIALOG_EXPORT_IDF3: public DIALOG_EXPORT_IDF3_BASE
{
private:
    PCB_EDIT_FRAME* m_parent;
    wxConfigBase* m_config;
    bool   m_idfThouOpt;    // remember last preference for units in THOU
    bool   m_AutoAdjust;    // remember last Reference Point AutoAdjust setting
    int    m_RefUnits;      // remember last units for Reference Point
    double m_XRef;          // remember last X Reference Point
    double m_YRef;          // remember last Y Reference Point

public:
    DIALOG_EXPORT_IDF3( PCB_EDIT_FRAME* parent ) :
            DIALOG_EXPORT_IDF3_BASE( parent )
    {
        m_parent = parent;
        m_config = Kiface().KifaceSettings();
        SetFocus();
        m_idfThouOpt = false;
        m_config->Read( OPTKEY_IDF_THOU, &m_idfThouOpt );
        m_rbUnitSelection->SetSelection( m_idfThouOpt ? 1 : 0 );
        m_config->Read( OPTKEY_IDF_REF_AUTOADJ, &m_AutoAdjust, false );
        m_config->Read( OPTKEY_IDF_REF_UNITS, &m_RefUnits, 0 );
        m_config->Read( OPTKEY_IDF_REF_X, &m_XRef, 0.0 );
        m_config->Read( OPTKEY_IDF_REF_Y, &m_YRef, 0.0 );

        m_cbAutoAdjustOffset->SetValue( m_AutoAdjust );
        m_cbAutoAdjustOffset->Bind( wxEVT_CHECKBOX, &DIALOG_EXPORT_IDF3::OnAutoAdjustOffset, this );

        m_IDF_RefUnitChoice->SetSelection( m_RefUnits );
        wxString tmpStr;
        tmpStr << m_XRef;
        m_IDF_Xref->SetValue( tmpStr );
        tmpStr = wxT( "" );
        tmpStr << m_YRef;
        m_IDF_Yref->SetValue( tmpStr );

        if( m_AutoAdjust )
        {
            m_IDF_RefUnitChoice->Enable( false );
            m_IDF_Xref->Enable( false );
            m_IDF_Yref->Enable( false );
        }
        else
        {
            m_IDF_RefUnitChoice->Enable( true );
            m_IDF_Xref->Enable( true );
            m_IDF_Yref->Enable( true );
        }

        m_sdbSizerOK->SetDefault();

        FixOSXCancelButtonIssue();

        // Now all widgets have the size fixed, call FinishDialogSettings
        FinishDialogSettings();
    }

    ~DIALOG_EXPORT_IDF3()
    {
        m_idfThouOpt = m_rbUnitSelection->GetSelection() == 1;
        m_config->Write( OPTKEY_IDF_THOU, m_idfThouOpt );
        m_config->Write( OPTKEY_IDF_REF_AUTOADJ, GetAutoAdjustOffset() );
        m_config->Write( OPTKEY_IDF_REF_UNITS, m_IDF_RefUnitChoice->GetSelection() );
        m_config->Write( OPTKEY_IDF_REF_X, m_IDF_Xref->GetValue() );
        m_config->Write( OPTKEY_IDF_REF_Y, m_IDF_Yref->GetValue() );
    }

    bool GetThouOption()
    {
        return m_rbUnitSelection->GetSelection() == 1;
    }

    wxFilePickerCtrl* FilePicker()
    {
        return m_filePickerIDF;
    }

    int GetRefUnitsChoice()
    {
        return m_IDF_RefUnitChoice->GetSelection();
    }

    double GetXRef()
    {
        return DoubleValueFromString( UNSCALED_UNITS, m_IDF_Xref->GetValue() );
    }

    double GetYRef()
    {
        return DoubleValueFromString( UNSCALED_UNITS, m_IDF_Yref->GetValue() );
    }

    bool GetAutoAdjustOffset()
    {
        return m_cbAutoAdjustOffset->GetValue();
    }

    void OnAutoAdjustOffset( wxCommandEvent& event )
    {
        if( GetAutoAdjustOffset() )
        {
            m_IDF_RefUnitChoice->Enable( false );
            m_IDF_Xref->Enable( false );
            m_IDF_Yref->Enable( false );
        }
        else
        {
            m_IDF_RefUnitChoice->Enable( true );
            m_IDF_Xref->Enable( true );
            m_IDF_Yref->Enable( true );
        }

        event.Skip();
    }

};


/**
 * Function OnExportIDF3
 * will export the current BOARD to IDF board and lib files.
 */
void PCB_EDIT_FRAME::ExportToIDF3( wxCommandEvent& event )
{
    wxFileName fn;

    // Build default file name
    fn = GetBoard()->GetFileName();
    fn.SetExt( wxT( "emn" ) );

    DIALOG_EXPORT_IDF3 dlg( this );
    dlg.FilePicker()->SetPath( fn.GetFullPath() );

    if ( dlg.ShowModal() != wxID_OK )
        return;

    bool thou = dlg.GetThouOption();
    double aXRef;
    double aYRef;

    if( dlg.GetAutoAdjustOffset() )
    {
        EDA_RECT bbox = GetBoard()->ComputeBoundingBox( true );

        aXRef = bbox.Centre().x * MM_PER_IU;
        aYRef = bbox.Centre().y * MM_PER_IU;
    }
    else
    {
        aXRef = dlg.GetXRef();
        aYRef = dlg.GetYRef();

        if( dlg.GetRefUnitsChoice() == 1 )
        {
            // selected reference unit is in inches
            aXRef *= 25.4;
            aYRef *= 25.4;
        }

    }

    wxBusyCursor dummy;

    wxString fullFilename = dlg.FilePicker()->GetPath();

    if( !Export_IDF3( GetBoard(), fullFilename, thou, aXRef, aYRef ) )
    {
        wxString msg = _( "Unable to create " ) + fullFilename;
        wxMessageBox( msg );
        return;
    }
}
