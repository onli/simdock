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
#include "background.h"

using namespace std;

//get the currently used background 
wxBitmap* getRootWallpaper()
{
    wxBitmap* backImage = new wxBitmap();
    WnckScreen *screen = wnck_screen_get_default ();

	// This is the X window ID of the desktop
    Pixmap pm = wnck_screen_get_background_pixmap(screen);
    int i = 0;
    while(i < 5)
    {
        if (pm != None) {
            break;
        }
        wxMilliSleep(1000);
        pm = wnck_screen_get_background_pixmap(screen);
        i++;
    }
    
    if (pm != None) {
		wxSize sz = wxGetDisplaySize();
		// Note: This code to hide the app is duplicated with the outer call in tasks.cc, but this is needed for first app start
		wxGetApp().frame->SetTransparent(0);
		wxGetApp().frame->Hide();
		wxGetApp().frame->Disable();
		// Give the main UI thread a chance to hide the app first
		while(wxGetApp().frame->IsShown()) {
			wxMilliSleep(20);
		}
		// This sleep should be unnecessary, but without it the UI will not be hidden in the next step
		wxMilliSleep(20);
		wxBitmap* backImage = new wxBitmap(
			gdk_pixbuf_get_from_window(
				gdk_x11_window_foreign_new_for_display(
					gdk_display_get_default(),
					gdk_x11_get_default_root_xwindow()
				),
				0,
				0,
				sz.GetWidth(),
				sz.GetHeight()
			)
		  );
		wxGetApp().frame->Show();
		wxGetApp().frame->SetTransparent(255);
		wxGetApp().frame->Enable();
		return backImage;
    } else {
        wxSize sz = wxGetDisplaySize();
		wxBitmap* backImage = new wxBitmap(sz.GetWidth(), sz.GetHeight());
        wxMemoryDC dc;
        dc.SelectObject(*backImage);
        dc.SetBackground(*wxTRANSPARENT_BRUSH);
        dc.Clear();
        dc.SelectObject(*backImage);
		return backImage;
    }
    return backImage;
}

wxBitmap *fixImage (wxString url, int type, wxColour c)
{
  if (url.IsEmpty ())
    return NULL;

  wxImage img = wxImage (url); 
  wxSize sz = wxGetDisplaySize();
  int w = sz.GetWidth();
  int h = sz.GetHeight();
  printf ("Image size:%d,%d Screen size:%d,%d\n", img.GetWidth (),
	  img.GetHeight (), w, h);
  // cout << wx2std(c.GetAsString(wxC2S_HTML_SYNTAX)) <<endl;
  switch (type)
    {
    case STRETCHED:
      return new wxBitmap (img.Scale (w, h, wxIMAGE_QUALITY_HIGH));
      break;
    case CENTERED:
      int x, y;
      if (img.GetWidth () > w)
	x = 0;
      else
	x = (w - img.GetWidth ()) / 2;
      if (img.GetHeight () > h)
	y = 0;
      else
	y = (h - img.GetHeight ()) / 2;


      img.Resize (wxSize (w, h), wxPoint (x, y), c.Red (), c.Green (),
		  c.Blue ());
      return new wxBitmap (img);
      break;
    default:
      return new wxBitmap (img);
      break;

    }

  return NULL;
}
