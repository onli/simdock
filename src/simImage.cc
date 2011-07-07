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
  status = STATUS_NONE;
  blur = 0;
  pid = PID_NONE;
  lastWindow = 0;
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
  status = STATUS_DECREASING;
  blur = MAX_BLUR;
  pid = PID_NONE;
  lastWindow = 0;
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
  	
  	
  	WnckWindow* value = array[lastWindow];
  	lastWindow = (lastWindow+1) % count;
  	return value;
}

bool simImage::removeWindow(WnckWindow* window)
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
   if (!img.LoadFile(path))
   	return false;
   
   if (!img.HasAlpha ())
   	img.InitAlpha ();
   	
   initReflex();
   return true;	
}


void
simImage::handleStatus ()
{
    switch (status)
    {
        case STATUS_NONE:
        break;
        case STATUS_DECREASING:
            blur--;
            if (blur == 0)
            {
                status = STATUS_NONE;
            }
        break;
        case STATUS_INCREASING:
            blur++;
            if (blur >= MAX_BLUR)
            {
                status = STATUS_DECREASING;
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
