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

// Function to fetch the root window background pixmap
Pixmap getRootPixmap(Display* display, Window root) {
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char* prop = nullptr;
    Atom prop_root = XInternAtom(display, "_XROOTPMAP_ID", True);

    if (XGetWindowProperty(display, root, prop_root, 0, 1, False, XA_PIXMAP, 
                           &actual_type, &actual_format, &nitems, &bytes_after, 
                           &prop) == Success) {
        if (nitems == 1) {
            Pixmap pixmap = *(Pixmap*)prop;
            XFree(prop);
            return pixmap;
        }
    }
    return None;
}

// Function to convert X11 Pixmap into wxImage
wxImage ConvertPixmapToWxImage(Display* display, Pixmap pixmap, int width, int height) {
    XImage* ximage = XGetImage(display, pixmap, 0, 0, width, height, AllPlanes, ZPixmap);
    if (!ximage) {
        std::cerr << "Failed to get XImage from Pixmap!" << std::endl;
        return wxImage(width, height);
    }

    wxImage image(width, height);
    unsigned char* data = image.GetData();
    if (!data) {
        std::cerr << "Failed to allocate memory for wxImage data!" << std::endl;
        XDestroyImage(ximage);
        return wxImage(width, height);
    }

    // Convert pixel data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned long pixel = XGetPixel(ximage, x, y);
            unsigned char red   = (pixel >> 16) & 0xFF;
            unsigned char green = (pixel >> 8) & 0xFF;
            unsigned char blue  = pixel & 0xFF;

            data[(y * width + x) * 3 + 0] = red;
            data[(y * width + x) * 3 + 1] = green;
            data[(y * width + x) * 3 + 2] = blue;
        }
    }

    XDestroyImage(ximage);
    return image;
}

//get the currently used background 
wxBitmap* getRootWallpaper()
{
	wxBitmap* backImage;
	Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Unable to open X display" << std::endl;
        return NULL;
    }

    Window root = DefaultRootWindow(display);

    // Get the root window background pixmap
    Pixmap rootPixmap = getRootPixmap(display, root);
    if (rootPixmap != None) {
        int screen_width = DisplayWidth(display, DefaultScreen(display));
        int screen_height = DisplayHeight(display, DefaultScreen(display));

        // Convert Pixmap to wxImage and then to wxBitmap
        wxImage image = ConvertPixmapToWxImage(display, rootPixmap, screen_width, screen_height);
        if (image.IsOk()) {
		  backImage = new wxBitmap(image);
        }
    }
	XCloseDisplay(display);
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
