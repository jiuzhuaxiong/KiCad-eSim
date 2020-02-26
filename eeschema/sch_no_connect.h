/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2009 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 1992-2011 KiCad Developers, see AUTHORS.txt for contributors.
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

/**
 * @file sch_no_connect.h
 */

#ifndef _SCH_NO_CONNECT_H_
#define _SCH_NO_CONNECT_H_


#include <sch_item_struct.h>

class NETLIST_OBJECT_LIST;

class SCH_NO_CONNECT : public SCH_ITEM
{
    wxPoint m_pos;                 ///< Position of the no connect object.
    wxSize  m_size;                ///< Size of the no connect object.

public:
    SCH_NO_CONNECT( const wxPoint& pos = wxPoint( 0, 0 ) );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    ~SCH_NO_CONNECT() { }

    wxString GetClass() const
    {
        return wxT( "SCH_NO_CONNECT" );
    }

    int GetPenSize() const;

    void SwapData( SCH_ITEM* aItem );

    void Draw( EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aOffset,
               GR_DRAWMODE aDrawMode, EDA_COLOR_T aColor = UNSPECIFIED_COLOR );

    bool Save( FILE* aFile ) const;

    bool Load( LINE_READER& aLine, wxString& aErrorMsg );

    void GetEndPoints( std::vector< DANGLING_END_ITEM >& aItemList );

    const EDA_RECT GetBoundingBox() const;  // Virtual

    // Geometric transforms (used in block operations):

    void Move( const wxPoint& aMoveVector )
    {
        m_pos += aMoveVector;
    }

    void MirrorY( int aYaxis_position );

    void MirrorX( int aXaxis_position );

    void Rotate( wxPoint aPosition );

    bool IsSelectStateChanged( const wxRect& aRect );

    bool IsConnectable() const { return true; }

    void GetConnectionPoints( std::vector< wxPoint >& aPoints ) const;

    wxString GetSelectMenuText() const { return wxString( _( "No Connect" ) ); }

    BITMAP_DEF GetMenuImage() const { return noconn_xpm; }

    void GetNetListItem( NETLIST_OBJECT_LIST& aNetListItems, SCH_SHEET_PATH* aSheetPath );

    wxPoint GetPosition() const { return m_pos; }

    void SetPosition( const wxPoint& aPosition ) { m_pos = aPosition; }

    bool HitTest( const wxPoint& aPosition, int aAccuracy ) const;

    bool HitTest( const EDA_RECT& aRect, bool aContained = false, int aAccuracy = 0 ) const;

    void Plot( PLOTTER* aPlotter );

    EDA_ITEM* Clone() const;

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const { ShowDummy( os ); } // override
#endif

private:
    bool doIsConnected( const wxPoint& aPosition ) const;
};


#endif    // _SCH_NO_CONNECT_H_
