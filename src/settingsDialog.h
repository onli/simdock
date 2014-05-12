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
    ID_Enable_Minimize = 2,
    ID_LEFT_BORDER = 3,
    ID_RIGHT_BORDER = 4,
    ID_BOTTOM_BORDER = 5,
    ID_ICONW = 6,
    ID_ICONH = 7,
    ID_PERCENT = 8,
    ID_RANGE = 9,
    ID_SPACER = 11,
    ID_BG_HEIGHT = 12,
    ID_bgPath = 13,
    ID_SHOW_REFLEXES = 14,
    ID_REFLEX_SCALING = 15,
    ID_REFLEX_ALPHA = 16,
    ID_AUTO_POSITION = 17,
    ID_ENABLE_TASKS = 18,
    ID_BLUR_TIMEOUT = 19
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
    void SaveSettings();
    
protected:
    void OnBrowseEvent(wxCommandEvent& event);
    void OnChange(wxCommandEvent& event);
    void OnSpinChange(wxSpinEvent& event);
    /* TODO: make sure the destroyer really gets all of this */
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
    wxSpinCtrl* blur_timeout_text;
    /* Zoom */
    wxSpinCtrl* zoom_text;
    wxSpinCtrl* range_text;
    /* Background */
    wxSpinCtrl* bg_height_text;
    wxTextCtrl* bg_path_text;
    /* Borders */
    wxSpinCtrl* left_border_text;
    wxSpinCtrl* right_border_text;
    wxSpinCtrl* bottom_border_text;
    wxSpinCtrl* spacer_border_text;
    wxSpinCtrl* width_text;
    wxSpinCtrl* height_text;
    /* Reflexes */
    wxCheckBox* reflex_enabled;
    wxSpinCtrl* reflex_scaling_text;
    wxSpinCtrl* reflex_alpha_text;
    
    wxButton* browse_button;

DECLARE_EVENT_TABLE()
};

#endif
