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
#include "simImage.h"


simImage::simImage()
{
  img = wxImage();
  blurStatus = STATUS_NONE;
  blur = 0;
  pid = PID_NONE;
  lastWindow = 0;
  // Task gets set to true when this simImage is connected to a currently open window
  // and iff it is not a permanent starter
  task = false;
}

simImage::~simImage()
{
	
}


simImage::simImage (wxImage img, const wxString& img_link, const wxString& link, const wxString& name, const wxString& descr, int i)
{
  if (!img.HasAlpha ())
    img.InitAlpha ();

  task = false;
  this->img = img;		//wxImage(path);
  initReflex();
  this->img_link = img_link;
  id = i;
  this->link = link;
  this->name = name;
  this->descr = descr;
  blurStatus = STATUS_DECREASING;
  blur = MAX_BLUR;
  pid = PID_NONE;
  lastWindow = 0;
  this->cycleMinimize = true;
  /*
     status = STATUS_NONE;
     blur = 0;
   */
}

void simImage::addWindow(WnckWindow* window)
{
  	array.Add(window);
}

WnckWindow* simImage::getWindow()
{
  	int count = array.GetCount();
  	if (count == 0)
  	return NULL;
  	
  	
  	WnckWindow* value = array[this->lastWindow];
  	this->lastWindow = (lastWindow+1) % count;
  	return value;
}

void simImage::removeWindow(WnckWindow* window)
{
	/*
	int idx = array.Index(window);
	if (idx == wxNOT_FOUND)
	{
		return false;
	}
	*/
	if (lastWindow > 0)
	{
		lastWindow--;
	}

	array.Remove(window);
}

bool simImage::hasWindow(WnckWindow* window)
{
	return array.Index(window) != wxNOT_FOUND;
}

int simImage::windowCount()
{
	return array.GetCount();
}

void simImage::initReflex()
{
	reflex = img.Mirror(false);
}

bool simImage::loadImage(const wxString& path)
{
    struct stat buffer;   
    if (stat (path.c_str(), &buffer) != 0) {
        // we need to check that file really exists
        printf("could not load image file");
        return false;
    }
    if (path.EndsWith(".svg")) {
        GError *err = NULL;
        RsvgHandle* handle = rsvg_handle_new_from_file(path.ToUTF8().data(), &err);
        if (err != NULL) {
            printf("could not load svg from file");
            return false;
        }
        GdkPixbuf* pixbuf = rsvg_handle_get_pixbuf(handle);
        wxBitmap bmp = wxBitmap(pixbuf);
        img = bmp.ConvertToImage();
    } else {
        if (!img.LoadFile(path))
        return false;
    }
   
    if (!img.HasAlpha ()) {
        img.InitAlpha ();
    }
   	
    initReflex();
    return true;	
}

bool simImage::allNotMinimized() {
    int count = array.GetCount();
    int i = 0;
    while (i < count) {
        bool comp =  wnck_window_is_minimized(array[i]);
        if (comp) {
            return false;
        }
        i++;
    }
    return true;
}

bool simImage::allMinimized() {
    int count = array.GetCount();
    int i = 0;
    while (i < count) {
        bool comp =  wnck_window_is_minimized(array[i]);
        if (!comp) {
            return false;
        }
        i++;
    }
    return true;
}

void
simImage::handleStatus ()
{
    switch (blurStatus)
    {
        case STATUS_NONE:
        break;
        case STATUS_DECREASING:
            blur--;
            if (blur == 0)
            {
                blurStatus = STATUS_NONE;
            }
        break;
        case STATUS_INCREASING:
            blur++;
            if (blur >= MAX_BLUR)
            {
                blurStatus = STATUS_DECREASING;
            }
        break;
    }
}
bool simImage::isIn (int m_x, int m_y)
{
  return m_x > x && m_y > y && m_x < x + w && m_y < y + h;
}
wxPoint simImage::center()
{
	return wxPoint(x+w/2,y+h/2);
}
