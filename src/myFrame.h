/*  
 *   Copyright 2007 Simone Della Longa <simonedll@yahoo.it>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _MYFRAME_H
#define _MYFRAME_H
#include "settingsDialog.h"
#include "launcher_dialog.h"
#include "simSettings.h"


/* Forward declaration */
class SettingsDialog;


class MyFrame:public wxFrame
{

public:
  MyFrame (wxWindow * parent,simSettings ss, ImagesArray* array, wxWindowID id, const wxString & title,
	   const wxPoint & pos = wxDefaultPosition, const wxSize & size =
	   wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
  wxPoint lastPosition;		//Last position on-screen before being destroyed
  void SetWallpaper (wxBitmap * newBitmap);
  wxBitmap * GetWallpaper ();
  
  
  void SetBG (wxImage * newImg);
  wxImage * GetBG ();
  
  void SetMarkBitmap (wxBitmap * newBmp);
  wxBitmap * GetMarkBitmap ();
  
  
  void updateSize();
  bool disposed;
  /* Auto-hide feature */
  int frameOptions;
  wxSize appSize;
  simSettings settings;
private:
    void RefreshSizes (simImage * img, int distance);
    /* Mouse */
    void OnMiddleDown (wxMouseEvent & event);
    void OnMiddleUp (wxMouseEvent & event);
    void OnMouseMove (wxMouseEvent & event);
    void OnMouseLeave (wxMouseEvent & event);
    void OnMouseEnter (wxMouseEvent & event);
    void OnMouseEnterIcon (wxMouseEvent & event, simImage* img);
    void OnMouseLeaveIcon (wxMouseEvent & event);
    void OnLeftUp (wxMouseEvent & event);
    void OnLeftDown(wxMouseEvent& event);
    void OnRightClick (wxMouseEvent & event);

    simImage* getClickedIcon(wxMouseEvent & event);

    void OnContextMenu (wxContextMenuEvent & event);
    void OnKeyDown (wxKeyEvent & event);

    void OnFrameMove (wxMoveEvent & event);
    void OnPaint (wxPaintEvent & event);
    void OnBackground (wxEraseEvent & event);
    void OnBlurTimerTick (wxTimerEvent & event);
    void OnHoverTimerTick (wxTimerEvent & event);
    /* Menu callbacks */
    void OnQuit (wxCommandEvent & event);
    void OnAbout (wxCommandEvent & event);
    void OnSettings(wxCommandEvent & event);
    void OnAdd (wxCommandEvent & event);
    void OnKeep (wxCommandEvent & event);
    void OnEdit (wxCommandEvent &event);
    void OnDelete (wxCommandEvent &event);
    wxMenu* GetPopMenu();
    /* popUp menu */
    wxMenu *popMenu;

    wxAboutDialogInfo* info; //About informations

    wxMenuItem* EditMenuItem; 
    wxMenuItem* DeleteMenuItem;
    int clickedID; //-1 means no launcher clicked

    /* Icons Drag n drop */
    bool dragging;
    bool moving;
    int draggedID;
    wxPoint draggedPos;


    
    ImagesArray* ImagesList;

    wxTimer *	 blurTimer;

    wxTimer*	 hoverTimer;
    simImage* hoveringIcon;
    bool showTooltip;

    wxImage * appBackground; //Simdock background
    wxMemoryDC * src_dc; //Device context used to  the wallpaper
    wxBitmap * backImage; //DE wallpaper
    wxBitmap* markBitmap;


    bool middleClicked;
    wxPoint middleClick;

    SettingsDialog *settingsDialog;
    void OnClose (wxCloseEvent & event);

    DECLARE_EVENT_TABLE () 
  
};

#endif /* _MYFRAME_H */
