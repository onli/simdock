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
    xcb_ewmh_connection_t* ewmh_conn = xstuff_getConnection();
    xcb_ewmh_set_wm_strut(ewmh_conn, winID, 0, 0, 0, size);
    
    int screenHeight = wnck_screen_get_height(wnck_screen_get_default());
    xcb_ewmh_wm_strut_partial_t wm_strut = {0, 0, 0, size, 0, 0, 0, 0, 0, 0, screenHeight - size, screenHeight};
    xcb_ewmh_set_wm_strut_partial(  ewmh_conn,
                                    winID,
                                    wm_strut);
    xcb_flush(ewmh_conn->connection);
    xstuff_closeConnection(ewmh_conn);
    return true;
}

void
xstuff_setDefaultWindowFlags(Window winID) {
    // the wnck-functions don't work here reliably, at least on my system, so set the states with xcb
    xcb_ewmh_connection_t* ewmh_conn = xstuff_getConnection();
    xcb_ewmh_request_change_wm_desktop(ewmh_conn, 0 /* Screen */, winID, 0xFFFFFFFF, XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL);

    xcb_atom_t states[3] = {ewmh_conn->_NET_WM_STATE_SKIP_PAGER, ewmh_conn->_NET_WM_STATE_SKIP_TASKBAR,ewmh_conn->_NET_WM_STATE_ABOVE};
    xcb_atom_t types[1] = {ewmh_conn->_NET_WM_WINDOW_TYPE_DOCK};
    
    xcb_ewmh_set_wm_state(ewmh_conn, winID, 3, states);
    xcb_ewmh_set_wm_window_type(ewmh_conn, winID, 1, types);
    
    xcb_flush(ewmh_conn->connection);
    xstuff_closeConnection(ewmh_conn);
}

xcb_ewmh_connection_t*
xstuff_getConnection() {
    xcb_connection_t* conn = xcb_connect(NULL, NULL);
    xcb_ewmh_connection_t* ewmh_conn = new xcb_ewmh_connection_t;
    xcb_intern_atom_cookie_t* ewmh_cookie = xcb_ewmh_init_atoms(conn, ewmh_conn);
    if(! xcb_ewmh_init_atoms_replies(ewmh_conn, ewmh_cookie, NULL)) {
        std::cout << "Could not initialize X-connection \n";
        return NULL;
    }
    return ewmh_conn;
}

void
xstuff_closeConnection(xcb_ewmh_connection_t* ewmh_conn) {
    xcb_ewmh_connection_wipe(ewmh_conn);
    xcb_disconnect(ewmh_conn->connection);
    free(ewmh_conn);
}

