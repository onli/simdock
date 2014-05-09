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
#include "xstuff.h"
#include "main.h"

static int (*old_error_handler)(Display *, XErrorEvent *) = NULL;

static int client_msg(Display *disp, Window win, std::string msg, 
        unsigned long data0, unsigned long data1, 
        unsigned long data2, unsigned long data3,
        unsigned long data4) {
    XEvent event;
    long mask = SubstructureRedirectMask | SubstructureNotifyMask;

    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.message_type = XInternAtom(disp, msg.c_str(), False);
    event.xclient.window = win;
    event.xclient.format = 32;
    event.xclient.data.l[0] = data0;
    event.xclient.data.l[1] = data1;
    event.xclient.data.l[2] = data2;
    event.xclient.data.l[3] = data3;
    event.xclient.data.l[4] = data4;
    
    if (XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event)) {
        return EXIT_SUCCESS;
    }
    else {
        fprintf(stderr, "Cannot send %s event.\n", msg.c_str());
        return EXIT_FAILURE;
    }
}

void xstuff_raiseWindow(Window winID)
{
	Display *dpy;
 	dpy = XOpenDisplay (NULL);
    std::string msg("_NET_ACTIVE_WINDOW");
 	client_msg(dpy, winID, msg, 
            0, 0, 0, 0, 0);
 	
	XCloseDisplay (dpy);
}

int ignore_bad_window(Display *d, XErrorEvent *e)
{
    /* Ignore bad window errors here, handle elsewhere */
    if (e->error_code != BadWindow) {
        assert(old_error_handler);
        (*old_error_handler)(d, e);
    }

    return 0;
}

unsigned int xstuff_getWindowPID(Window w)
{
        Atom atom,actual_type;
        Display *dpy;
 		dpy = XOpenDisplay (NULL);
        char *atom_name;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;
        int status;
        unsigned int pid;
                         
        atom = XInternAtom(dpy, "_NET_WM_PID", True);
        atom_name = XGetAtomName (dpy,atom);

        /* from http://www.oreillynet.com/linux/blog/2007/10/where_does_my_time_go.html
        * ignore bad window here because it get raised always when
        * closing multiple windows, from tasks_window_closed
        * */
        old_error_handler = XSetErrorHandler(ignore_bad_window);
       
        status = XGetWindowProperty(dpy, w, atom, 0, 1024,
                                    False, AnyPropertyType,
                                    &actual_type,
                                    &actual_format, &nitems,
                                    &bytes_after,
                                    &prop);

        XSetErrorHandler(old_error_handler);

        if (status!=0 || !prop) 
        {
		return 0;
        }

        pid = prop[1] * 256; 
        pid += prop[0];
        XCloseDisplay (dpy);
        return pid;
}


bool
xstuff_resizeScreen (const wxString & program_name, const wxFrame & frame, bool dock)
{
//doesnt work. Window w = tasks_getFromName((char*)program_name.c_str(),program_name.Len());
  
  wxSize sz = wxGetDisplaySize();
  
  int frameHeight = frame.GetClientSize ().GetHeight ();


  Display *xdisplay = NULL;
  xdisplay = XOpenDisplay (NULL);

  Window w =
    tasks_getFromName (wx2std (program_name).c_str (), program_name.Len ());
  
  if (w == 0)
    return false;
    

  
  bool result = xstuff_setStrut (xdisplay, w, frameHeight);
  if (!result)
    {
      XCloseDisplay (xdisplay);
      return false;
    }
  if (dock)
  {
	xstuff_setDock (xdisplay, w);
  }

  XCloseDisplay (xdisplay);

  return true;
}

/* Stolen from gnome-panel */
void
xstuff_set_wmspec_strut (Display * xdisplay, Window winID, int left,
			 int right, int top, int bottom)
{
  if (!xdisplay)
    return;

  long vals[4];
  Atom a = XInternAtom (xdisplay, "_NET_WM_STRUT", false);

  vals[0] = left;
  vals[1] = right;
  vals[2] = top;
  vals[3] = bottom;

  XChangeProperty (xdisplay,
		   winID,
		   a,
		   XA_CARDINAL, 32, PropModeReplace,
		   (unsigned char *) vals, 4);
}




bool
xstuff_setStrut (Display * xdisplay, Window winID, int size)
{
  if (winID == 0 || size <= 0)
    return false;

  if (!xdisplay)
    return false;



  xstuff_set_wmspec_strut (xdisplay, winID, 0, 0, 0, size);

  return true;
}

/* Stolen from gnome-panel */
bool
xstuff_setDock (Display * xdisplay, Window winID)
{
  if (winID == 0)
    return false;

  if (!xdisplay)
    return false;


  Atom a[2] = { None, None };
  a[0] = XInternAtom (xdisplay, "_NET_WM_WINDOW_TYPE_DOCK", FALSE);
  Atom a2 = XInternAtom (xdisplay, "_NET_WM_WINDOW_TYPE", FALSE);

  XChangeProperty (xdisplay,
		   winID,
		   a2, XA_ATOM, 32, PropModeReplace, (unsigned char *) a, 1);
  return true;

}
