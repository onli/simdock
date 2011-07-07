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

#ifndef LAUNCHER_DIALOG_H_
#define LAUNCHER_DIALOG_H
#include "simImage.h"
#include "wx/bmpbuttn.h"
#include "wx/filename.h"

class LauncherDialog : public wxDialog
{
DECLARE_CLASS(LauncherDialog)

public:

LauncherDialog(wxWindow* parent, simImage* launcher);
~LauncherDialog();
bool saveChanges();
void SetIcon(const wxBitmap& value);




protected:
void OnBrowseEvent(wxCommandEvent& event);
static wxString lastPath;


simImage* launcher;
wxBitmapButton* bmp_button;
wxString bmp_path;
wxTextCtrl* name_text;
wxTextCtrl* command_text;
wxTextCtrl* description_text;

DECLARE_EVENT_TABLE()

};


#endif
