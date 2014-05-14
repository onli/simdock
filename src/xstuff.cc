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

void
xstuff_raiseWindow(Window winID) {
	wnck_window_activate(wnck_window_get(winID), 0);
}

unsigned int
xstuff_getWindowPID(Window winID) {
    xcb_connection_t* conn = xcb_connect (NULL, NULL);
    xcb_ewmh_connection_t ewmh_conn;
    xcb_intern_atom_cookie_t* ewmh_cookie = xcb_ewmh_init_atoms(conn, &ewmh_conn);
    if(! xcb_ewmh_init_atoms_replies(&ewmh_conn, ewmh_cookie, NULL)) {
        return false;
    }

    uint32_t pid;
    xcb_ewmh_get_wm_pid_reply(&ewmh_conn, xcb_ewmh_get_wm_pid(&ewmh_conn, winID), &pid, NULL);

    xcb_disconnect(conn);
    return pid;
}


bool
xstuff_resizeScreen (Window winID, const wxFrame & frame) {  
    int frameHeight = frame.GetClientSize ().GetHeight();
  
    bool result = xstuff_setStrut (winID, frameHeight);

    return result;
}



bool
xstuff_setStrut (Window winID, int size) {
    if (winID == 0 || size <= 0) {
        return false;
    }

    xcb_connection_t* conn = xcb_connect (NULL, NULL);;
    xcb_ewmh_connection_t ewmh_conn;
    xcb_intern_atom_cookie_t* ewmh_cookie = xcb_ewmh_init_atoms(conn, &ewmh_conn);
    if(! xcb_ewmh_init_atoms_replies(&ewmh_conn, ewmh_cookie, NULL)) {
        return false;
    }

    
    xcb_ewmh_set_wm_strut( &ewmh_conn, winID, 0, 0, 0, size);
    
    int screenHeight = wnck_screen_get_height(wnck_screen_get_default());
    xcb_ewmh_wm_strut_partial_t wm_strut = {0, 0, 0, size, 0, 0, 0, 0, 0, 0, screenHeight - size, screenHeight};
    xcb_ewmh_set_wm_strut_partial(  &ewmh_conn,
                                    winID,
                                    wm_strut);
                            
    xcb_flush(conn);
    xcb_disconnect(conn);
    return true;
}

void
xstuff_setDefaultWindowFlags(Window winID) {
    WnckWindow* window = wnck_window_get(winID);
    wnck_window_pin(window);
    wnck_window_set_window_type(window, WNCK_WINDOW_DOCK);
    
    // skip_pager with wnck doesn't work, at least on my system, so set the states with xcb
    xcb_connection_t* conn = xcb_connect (NULL, NULL);
    xcb_ewmh_connection_t ewmh_conn;
    xcb_intern_atom_cookie_t* ewmh_cookie = xcb_ewmh_init_atoms(conn, &ewmh_conn);
    if(! xcb_ewmh_init_atoms_replies(&ewmh_conn, ewmh_cookie, NULL)) {
        return;
    }

    xcb_atom_t states[3] = {ewmh_conn._NET_WM_STATE_SKIP_PAGER, ewmh_conn._NET_WM_STATE_SKIP_TASKBAR, ewmh_conn._NET_WM_STATE_ABOVE};

    xcb_ewmh_set_wm_state(&ewmh_conn, winID, 3, states);
    xcb_flush(conn);
    xcb_disconnect(conn);
}

