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
#ifndef SIMIMAGE_H_
#define SIMIMAGE_H_
#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1	// LOL !!
#include <libwnck/libwnck.h>
#include "wx/wx.h"


#define MAX_BLUR 10
#define PID_NONE 0

typedef enum {
	STATUS_NONE = 0,
	STATUS_INCREASING =1,
	STATUS_DECREASING =2,
}simImage_status;

WX_DEFINE_ARRAY (WnckWindow *, windowArray);


class simImage
{
public:
  wxString img_link;
  wxImage img;
  wxImage reflex;
  wxString link;
  wxString name;
  wxString descr;
  bool active;  // keep track of the currently active window
  bool cycleMinimize;  // minimize all windows one by one
  
  unsigned int pid;
  bool task; //This launcher is a task. It will not be saved
  
 
  int id;	//unique identificator
  wxCoord x, y; //position
  int w, h; //dimension
  simImage_status status; //animation status
  int blur; //animation intensity
  simImage();
  
  virtual ~simImage();
  simImage (wxImage img, const wxString& img_link, const wxString& link, const wxString& name, const wxString& descr, int i);
  /* If image status is Not STATUS_NONE, updates the internal simImage status */
  void handleStatus ();
  /* Loads an image from the given path */
  bool loadImage(const wxString& path);
  /* Uses the current image to initialize the reflex image */
  void initReflex();
  /* Returns true if the given point is inside the simImage bounds, false otherwise */
  bool isIn (int x, int y);
  /* Returns the center point of the image */
  wxPoint center();

  void addWindow(WnckWindow* window);
  WnckWindow* getWindow();
  bool removeWindow(WnckWindow* window);
  int windowCount();
  bool hasWindow(WnckWindow* window);
  bool allNotMinimized();
  bool allMinimized();
  
private:
  windowArray array;
  int lastWindow;
};




#endif
