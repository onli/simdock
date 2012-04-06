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
#ifndef TASKS_H_
#define TASKS_H_


#include "main.h"
#include "xstuff.h"


class callbackArgs
{
public:
	ImagesArray * ImagesList;
	simSettings* settings;	
};

class taskInfo
{
public:
    bool Init(WnckWindow* w);
    void setName (char *processName);
    void setPath (char *processName);
    bool compareWith (simImage * img);
    wxString name;
    wxString path;
    unsigned int pid;
    wxString title;
    wxBitmap icon;
};



Window tasks_getFromName (const char *name, int nameLength);
int tasks_getProcessName (unsigned int pid, char *target_name, int size_targetname);
void tasks_fillList (ImagesArray * ImagesList,simSettings* settings);
void tasks_raise (WnckWindow* win);
void tasks_minimize (WnckWindow* win);
void tasks_register_signals(ImagesArray * ImagesList,simSettings* settings);
/* Adds a new image to the images list given a window and task informations */
void tasks_addNewImage(WnckWindow *window, ImagesArray* ImagesList, simSettings* settings,const taskInfo& ti);
wxBitmap* tasks_getRootWallpaper();

#endif
