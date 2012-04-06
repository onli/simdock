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


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "main.h"





enum {
ID_Browse_Bg = 1,
};

// Property sheet dialog
class SettingsDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(SettingsDialog)
public:
    SettingsDialog(wxWindow* parent,  simSettings* settings);
    ~SettingsDialog();

    wxPanel* CreateGeneralSettingsPage(wxWindow* parent);
    wxPanel* CreateAestheticSettingsPage(wxWindow* parent);
    wxPanel* CreateBackgroundPage(wxWindow* parent);
    simSettings* GetSettings();
    
protected:
    void OnBrowseEvent(wxCommandEvent& event);
    /* Hey you! if you add something, remember to add it to the destructor! */
    wxBookCtrlBase* notebook;
    wxPanel* generalSettings;
    wxPanel* aestheticSettings;
    wxPanel* backgroundSettings;
    
    
    
    simSettings* settings;
    wxImageList*    m_imageList;
    /* Behaviour*/
    wxCheckBox* auto_position;
    wxCheckBox* enable_tasks;
    wxCheckBox* enable_minimize;
    /* Zoom */
    wxTextCtrl* zoom_text;
    wxTextCtrl* range_text;
    /* Background */
    wxTextCtrl* bg_height_text;
    wxTextCtrl* bg_path_text;
    /* Borders */
    wxTextCtrl* left_border_text;
    wxTextCtrl* right_border_text;
    wxTextCtrl* bottom_border_text;
    wxTextCtrl* spacer_border_text;
    wxTextCtrl* width_text;
    wxTextCtrl* height_text;
    /* Reflexes */
    wxCheckBox* reflex_enabled;
    wxTextCtrl* reflex_scaling_text;
    wxTextCtrl* reflex_alpha_text;
    
    wxButton* browse_button;

DECLARE_EVENT_TABLE()
};

#endif
