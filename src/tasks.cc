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
 
#include "tasks.h"

bool taskInfo::Init (WnckWindow* window) {
    if (!window) {
        printf("taskInfo::Init NULL argument!\n");
        return false;
    }

    /* First set the icon. This will work always, as wnck_window_get_icon
    returns a fallbackicon if necessary, the rest may fail */
    wnck_set_default_icon_size(256); 
    GdkPixbuf* pb = gdk_pixbuf_copy(wnck_window_get_icon (window));
    
    wxBitmap* bmp = new wxBitmap(pb);
    if (!bmp->IsOk ()) {
        return false;
    }
    this->icon = *bmp;
    
    enum { BUFFERSIZE = 1024 };
    char processName[BUFFERSIZE];
    /* Since X doesn't rely on pids there's not a clear way to get the
     * pid associated with a window. We use 2 different ways to get the
     * pid. None of them is 100% accurate. (aMSN!!!) */
    
    pid = wnck_window_get_pid (window);    
    if (!pid) {
        pid = wnck_application_get_pid(wnck_window_get_application(window));
    }
        
    if (!pid){
        printf("Could not get pid for 0x%x\n",(unsigned int) wnck_window_get_xid (window));
        return false;
    }    
    
    if (!tasks_getProcessName(pid, &processName[0], sizeof (processName) -1))
    {
        printf("Could not get process name for %d\n", pid);
         return false;
    }
    
    setPath(&processName[0]);
    setName(&processName[0]);
    
    return true;
}

// Return true if both simImages belong to the same application
bool taskInfo::compareWith (simImage * img) {
    if (name.IsEmpty() || img->link.IsEmpty()) {
        return false;
    }
      
    if (pid == img->pid) {
        return true;
    }
      
      
    wxFileName launcherFN (img->link);
    wxString currLauncher = launcherFN.GetName ();
    if (launcherFN.HasExt ()) {
        currLauncher = currLauncher + _T (".") + launcherFN.GetExt ();
    }
    

    if ( (currLauncher.Len() > name.Len() && currLauncher.StartsWith(name))
        || name.StartsWith(currLauncher)
        || currLauncher.CmpNoCase(name) == 0) {
        return true;
    }

    return false;
}
  
void taskInfo::setName (char *processName) {
    wxString s = wxString ((const char *) processName, wxConvUTF8);
    wxFileName programFN (s);
    name = programFN.GetName ();
    if (programFN.HasExt ()) {
        name = name + _T (".") + programFN.GetExt ();
    }
}

void taskInfo::setPath (char *processName) {
    path = wxString ((const char *) processName, wxConvUTF8);
}


int tasks_getProcessName (unsigned int pid, char *target_name, int size_targetname) {
    int target_link;
    char dir_name[PATH_MAX];

    sprintf(dir_name, "/proc/%d/exe", pid);

    /* Getting the target of the exe ie to which binary it points to */
    target_link = readlink(dir_name, target_name, size_targetname);
    
    if (target_link <= 0) {
        return false;
    }

    target_name[target_link] = '\0';

    return true;
}

static void tasks_window_closed (WnckScreen *screen, WnckWindow *window,callbackArgs* ca) {
    if (!WNCK_IS_WINDOW (window) || wnck_window_is_skip_tasklist (window)) {
        return;
    }
    
    ImagesArray * ImagesList = ca->ImagesList;
    if (!ImagesList) {
        return;
    }

    for (unsigned int i = 0; i < ImagesList->GetCount (); i++) {
        simImage *img = (*ImagesList)[i];

        if (img->hasWindow(window)) {
            img->removeWindow(window);
            if (img->task && img->windowCount() == 0) {
                // This can be safely deleted, it is not a starter and all windows are closed
                delete img;
                ImagesList->RemoveAt (i);
            } else if (img->windowCount() == 0) {
                // we need to fall back to the default image, as it is a permanent starter
                img->loadImage(img->img_link);
            } else if (img->windowCount() > 0) {
                taskInfo ti;
                ti.Init(img->getWindow());
                if (img->task) {
                    // starters still have their own image we prefer to use, so we only do
                    // this for the non-starters
                    img->img = ti.icon.ConvertToImage();
                }
            }
            wxGetApp().frame->appSize = FirstPosition(ca->settings, ImagesList);
            wxGetApp().updateSize();
            wxGetApp().refresh();
            return;
        }
    }
}

static void tasks_window_opened (WnckScreen *screen, WnckWindow *window, callbackArgs* ca) {
    if (!ca || !ca->ImagesList) {
        printf("tasks_window_opened NULL ARGUMENT!\n");
        return;
    }

    if (!WNCK_IS_WINDOW (window) || wnck_window_is_skip_tasklist (window)) {
        return;
    }
    
    ImagesArray * ImagesList = ca->ImagesList;
    simSettings settings = ca->settings;
    taskInfo ti;
    ti.Init(window);

    for (int i = 0; i < ImagesList->GetCount (); i++) {
        simImage *img = (*ImagesList)[i];

        if (ti.compareWith (img)) {
            img->addWindow(window);
            img->pid = ti.pid;
            // refresh the icon, cause sometimes the splash-screen has no
            // valid icon, but the program does
            if (img->task) {
                // but don't do that for starters, as we want to preserve their image
                img->img = ti.icon.ConvertToImage();
            }
            wxGetApp().refresh();
            return;
        }
    }
    // if we are here the window is not linked to an existing icon
    tasks_addNewImage(window, ImagesList, settings, ti);
}

// Check whether our simdock window intersects with a different window.
// If it does, it is probably covered by it - and our background grabbing will fail
bool is_covered(WnckScreen *screen, WnckWindow *window) {
    GdkDisplay* display = gdk_display_get_default();
    GtkWidget* widget = wxGetApp().frame->GetHandle();
    GdkWindow* gdkWindow = gtk_widget_get_window(widget);
    XID xid = gdk_x11_window_get_xid(gdkWindow);
    cairo_region_t* visibleDockRegion = gdk_window_get_visible_region(gdkWindow);
    cairo_rectangle_int_t dockRect;
    cairo_region_get_rectangle(visibleDockRegion, 0, &dockRect);
    std::cout << "region x: " << dockRect.x << std::endl;
    std::cout << "region y: " << dockRect.y << std::endl;
                

    GList* allWindows = wnck_screen_get_windows(screen);
    for (GList *l = allWindows; l != NULL; l = l->next) {
        WnckWindow* curWindow = WNCK_WINDOW(l->data);
        if (wnck_window_is_visible_on_workspace(curWindow, wnck_screen_get_active_workspace(screen))) {
            Window otherXid = wnck_window_get_xid(curWindow);

            // We use the XID here just to avoid confusion with different pointers
            if (xid != otherXid) {
                GdkWindow* gdkWindowOther = gdk_x11_window_foreign_new_for_display(display, otherXid);
                cairo_region_t* otherVisibleRegion = gdk_window_get_visible_region(gdkWindowOther);
                // To check for intersection, we need to work with individual points, like stored
                // in a rectangle
                cairo_rectangle_int_t rect;
                cairo_region_get_rectangle(otherVisibleRegion, 0, &rect);
                cairo_region_overlap_t intersection = cairo_region_contains_rectangle(visibleDockRegion, &rect);
            
                if (intersection !=  CAIRO_REGION_OVERLAP_OUT) {
                    std::cout << "Colliding Window Name: " << wnck_window_get_name(curWindow) << std::endl;
                    std::cout << "with region x: " << rect.x << std::endl;
                    std::cout << "with region y: " << rect.y << std::endl;
                    cairo_region_destroy(visibleDockRegion);
                    cairo_region_destroy(otherVisibleRegion);
                    return true;
                }
                cairo_region_destroy(otherVisibleRegion);
            }
        }
    }
    cairo_region_destroy(visibleDockRegion);
    return false;
}

void tasks_window_background_change (WnckScreen *screen, WnckWindow *window, callbackArgs* ca) {
    if (wxGetApp().frame->IsTransparentBackgroundSupported()) {
        // With real transparency enabled we have to do nothing here
        return;
    }
    /*if (! wxGetApp().frame->IsExposed(wxGetApp().frame->GetRect())) {
        // TODO: IsExposed does not work, it always thinks it's not exposed
        std::cout << "not exposed" << "\n";
        // If simdock itself is covered, the fetched background will be unusable, so we stop
        return;
    }*/

    //GtkWidget* widget = wxGetApp().frame->GetHandle();
    //GdkWindow* gdkWindow = gtk_widget_get_window(widget);
    //cairo_region_t* visibleRegion = gdk_window_get_visible_region(gdkWindow);
    //cairo_rectangle_int_t* extents = new cairo_rectangle_int_t();
    //cairo_region_get_extents(visibleRegion, extents);
    //std::cout << extents->width << "\n";
    //if (cairo_region_is_empty(visibleRegion)) {
        // //TODO: cairo_region_is_empty does not work, it is never empty
        //std::cout << "not exposed" << "\n";
        //cairo_region_destroy(visibleRegion);
        //return;
    //}
    //cairo_region_destroy(visibleRegion);
    if (is_covered(screen, window)) {
        return;
    }
    
    
    // The signal arrives before the background is actually changed. The small sleep workarounds this issue.
    wxMilliSleep(20);
    // Now we hide the app, because getRootWallpaper() since the switch to GTK3 also sees simdock itself
    wxGetApp().frame->SetTransparent(0);
    wxGetApp().frame->Hide();
    wxGetApp().frame->Disable();
    wxGetApp().CallAfter([]{
        // The use of CallAfter is needed, otheerwise the app won't be hidden while getRoootWallpaper runs
        wxBitmap* backImage = getRootWallpaper();
        wxGetApp().SetWallpaper(backImage);
    });
    
}

void tasks_track_active_window (WnckScreen *screen, WnckWindow *window, callbackArgs* ca) {
    if (!WNCK_IS_WINDOW (window)) {
        return;
    }

    ImagesArray * ImagesList = ca->ImagesList;
    if (!ImagesList) {
        return;
    }
    
    if (wnck_window_is_skip_tasklist (window)) {
        // note that this also catches simdock itself
        return;
    }
    
    for (unsigned int i = 0; i < ImagesList->GetCount (); i++) {
        simImage *img = (*ImagesList)[i];
        img->active = img->hasWindow(window);
    }
}


void tasks_register_signals(ImagesArray * ImagesList,simSettings settings) {
    callbackArgs* ca = new callbackArgs();
    ca->ImagesList = ImagesList;
    ca->settings = settings;
    
    WnckScreen *defaultScreen = wnck_screen_get_default ();
    g_signal_connect (G_OBJECT(defaultScreen), "window_opened",
                      G_CALLBACK (tasks_window_opened),
                      ca);
                      
    g_signal_connect (G_OBJECT(defaultScreen), "background-changed",
                      G_CALLBACK (tasks_window_background_change),
                      ca); 

    g_signal_connect (G_OBJECT(defaultScreen), "window_closed",
                      G_CALLBACK(tasks_window_closed),
                      ca);
                      
    g_signal_connect (G_OBJECT(defaultScreen), "active-window-changed",
                      G_CALLBACK(tasks_track_active_window),
                      ca);
}


void tasks_addNewImage(WnckWindow *window, ImagesArray* ImagesList, simSettings settings,const taskInfo& ti) {
    if (!window || !ImagesList) {
        printf("addNewImage::Null argument\n");
        return;    
    }
    simImage *si = new simImage (ti.icon.ConvertToImage (), _T (" "), ti.name, ti.name, _T (" "), 1);

    si->addWindow(window);
    si->pid = ti.pid;
    si->w = settings.ICONW;
    si->h = settings.ICONH;
    si->y = (settings.MAXSIZE + settings.BOTTOM_BORDER) - settings.ICONH - settings.BOTTOM_BORDER;
    si->task = true;
    si->blurStatus = STATUS_NONE;
    si->blur = 0;
    ImagesList->Add(si);
    wxGetApp().frame->appSize = FirstPosition(settings, ImagesList);
    wxGetApp().updateSize();
    wxGetApp().refresh();
    wxGetApp().update();
}

void tasks_fillList (ImagesArray * ImagesList, simSettings settings) {
    WnckScreen *defaultScreen = wnck_screen_get_default ();
    wnck_screen_force_update (defaultScreen);
    GList *windowz = wnck_screen_get_windows (defaultScreen);

    WnckWindow *curr_Win=NULL;
    int l = g_list_length (windowz);

    //printf ("%d windows found\n", l);

    for (int i = 0; i < l; i++) {
        curr_Win = (WnckWindow *) g_list_nth_data (windowz, i);
        if (!curr_Win || wnck_window_is_skip_tasklist (curr_Win)) {
            continue;
        }

        taskInfo ti;
        bool init_ok = ti.Init(curr_Win);
          
        if (init_ok) {
            unsigned int i;
            for (i = 0; i < ImagesList->GetCount (); i++) {
                simImage *img = (*ImagesList)[i];
                if (ti.compareWith (img)) {
                    img->addWindow(curr_Win);
                    img->pid = ti.pid;
                    break;
                }
            }
            if (i == ImagesList->GetCount()) {
                tasks_addNewImage(curr_Win, ImagesList, settings, ti);
            }
        } else {
            tasks_addNewImage(curr_Win, ImagesList, settings, ti);
        }
    }
}


Window tasks_getFromName (const char *name, int nameLength) {

    if (!name) {
        return 0;
    }

    WnckScreen *defaultScreen = wnck_screen_get_default ();
    wnck_screen_force_update (defaultScreen);
    GList *windowz = wnck_screen_get_windows (defaultScreen);

    WnckWindow *curr_Win;
    int l = g_list_length (windowz);

    for (int i = 0; i < l; i++) {
        curr_Win = (WnckWindow *) g_list_nth_data (windowz, i);
        WnckApplication *app = wnck_window_get_application (curr_Win);
        if (strncmp (name, wnck_application_get_name (app), nameLength) == 0) {
            return wnck_window_get_xid (curr_Win);
        }
    }
    return 0;
}

Window tasks_getFromWindowName (const char *name, int nameLength) {

    if (!name) {
        return 0;
    }

    WnckScreen *defaultScreen = wnck_screen_get_default ();
    wnck_screen_force_update (defaultScreen);
    GList *windowz = wnck_screen_get_windows (defaultScreen);

    WnckWindow *curr_Win;
    int l = g_list_length (windowz);

    for (int i = 0; i < l; i++) {
        curr_Win = (WnckWindow *) g_list_nth_data (windowz, i);
        printf("%s \n", wnck_window_get_name (curr_Win));
        if (strncmp (name, wnck_window_get_name (curr_Win), nameLength) == 0) {
            return wnck_window_get_xid (curr_Win);
        }
    }
    return 0;
}

void tasks_raise (WnckWindow* win) {
    if (!win) {
        return;
    }
    
    wnck_window_unminimize(win, gtk_get_current_event_time());
    wnck_window_activate(win, gtk_get_current_event_time());
}

void tasks_minimize (WnckWindow* win) {
    if (!win) {
        return;
    }
    wnck_window_minimize(win);
}
