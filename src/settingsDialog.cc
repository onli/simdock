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

#include "settingsDialog.h"
#include "wx/textctrl.h"

using namespace std;


IMPLEMENT_CLASS(SettingsDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(SettingsDialog, wxPropertySheetDialog)
EVT_BUTTON(ID_Browse_Bg, SettingsDialog::OnBrowseEvent)
EVT_CHECKBOX(ID_Enable_Minimize, SettingsDialog::OnChange)
EVT_CHECKBOX(ID_SHOW_REFLEXES, SettingsDialog::OnChange)
EVT_CHECKBOX(ID_AUTO_POSITION, SettingsDialog::OnChange)
EVT_CHECKBOX(ID_ENABLE_TASKS, SettingsDialog::OnChange)
EVT_CHECKBOX(ID_FAST_ANIMATIONS, SettingsDialog::OnChange)
EVT_SPINCTRL(ID_LEFT_BORDER, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_RIGHT_BORDER, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_BOTTOM_BORDER, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_ICONW, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_ICONH, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_PERCENT, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_RANGE, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_SPACER, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_BG_HEIGHT, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_REFLEX_SCALING , SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_REFLEX_ALPHA, SettingsDialog::OnSpinChange)
EVT_SPINCTRL(ID_BLUR_TIMEOUT, SettingsDialog::OnSpinChange)

END_EVENT_TABLE()



SettingsDialog::SettingsDialog(wxWindow* win, simSettings* settings)
{
    this->settings = settings;
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);


    int  tabImage1 = 0;
	int tabImage2 = 1;
	int tabImage3 = 2;

    int sheetStyle = wxPROPSHEET_TOOLBOOK;


    SetSheetStyle(sheetStyle);
    SetSheetInnerBorder(0);
    SetSheetOuterBorder(0);
    // create a dummy image list with a few icons
    const wxSize imageSize(32, 32);

    m_imageList = new wxImageList(imageSize.GetWidth(), imageSize.GetHeight());
        
    /* TODO: Check for wxNullIcon here */
    m_imageList->Add(wxArtProvider::GetIcon(_T("applications-system"), wxART_OTHER, imageSize));
    m_imageList->Add(wxArtProvider::GetIcon(_T("applications-accessories"), wxART_OTHER, imageSize));
    m_imageList->Add(wxArtProvider::GetIcon(_T("applications-graphics"), wxART_OTHER, imageSize));
    m_imageList->Add(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, imageSize));


    Create(win, wxID_ANY, _("SimDock - Preferences"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE| wxRESIZE_BORDER);
        

    notebook = GetBookCtrl();
    
    notebook->SetImageList(m_imageList);

    generalSettings = CreateGeneralSettingsPage(notebook);
    aestheticSettings = CreateAestheticSettingsPage(notebook);
    backgroundSettings = CreateBackgroundPage(notebook);

    if (!notebook->AddPage(generalSettings, _("General"), true, tabImage1))
    {
    	cout << "Error adding general settings page" << endl;
    }
    if (!notebook->AddPage(aestheticSettings, _("Borders"), false, tabImage2))
    {
    	cout << "Error adding Borders settings page" << endl;
    }
    if (!notebook->AddPage(backgroundSettings, _("Appearance"), false, tabImage3))
    {
    	cout << "Error adding Background settings page" << endl;
    }

    CreateButtons(wxOK);
    LayoutDialog();
    
}


SettingsDialog::~SettingsDialog()
{ 
    delete(m_imageList); 
    delete (generalSettings);
    delete (aestheticSettings);
    delete (backgroundSettings);
    delete (notebook);
}

bool checkInt(wxSpinCtrl* control, int* value, signed long minvalue, signed long maxvalue)
{

    if (control->GetValue() < minvalue || control->GetValue() > maxvalue)
    {
	    wxMessageBox (_T ("Wrong value! ") + control->GetValue()+
	    wxString::Format(_T("Should be between %d and %d"),minvalue,maxvalue) , _T ("SimDock"), wxOK | wxICON_ERROR, NULL);
	    return false;
    }
    
	*value = control->GetValue();    
	return true;

}

simSettings* SettingsDialog::GetSettings()
{
    /* Zoom */
    if (!checkInt(zoom_text, &settings->PERCENT,0,1000))
	return NULL;
	
    if (!checkInt(range_text, &settings->RANGE,0,1000))
	return NULL;
	
    /* BackGround */
    
    if (!checkInt(bg_height_text, &settings->BG_HEIGHT,0,1000))
	return NULL;
	
    if (!wxFileExists(bg_path_text->GetValue())) {
	    wxMessageBox (_T ("Invalid value for ") + bg_path_text->GetValue() , _T ("SimDock"), wxOK | wxICON_ERROR, NULL);
	    return NULL;
    } else {
        settings->BG_PATH = bg_path_text->GetValue();
    }
    
	
    /* Borders */
	  if (!checkInt(left_border_text, &settings->LEFT_BORDER,0,1000))
	return NULL;
	  if (!checkInt(right_border_text, &settings->RIGHT_BORDER,0,1000))
	return NULL;
	  if (!checkInt(bottom_border_text, &settings->BOTTOM_BORDER,0,1000))
	return NULL;
	  if (!checkInt(spacer_border_text, &settings->SPACER,0,1000))
	return NULL;
	  if (!checkInt(width_text, &settings->ICONW,0,200))
	return NULL;
	  if (!checkInt(height_text, &settings->ICONH,0,200))
	return NULL;
	 if (!checkInt(reflex_scaling_text, &settings->REFLEX_SCALING,1,100))
	return NULL;
	if (!checkInt(reflex_alpha_text, &settings->REFLEX_ALPHA,0,255))
	  return NULL;

	settings->SHOW_REFLEXES = reflex_enabled->IsChecked();
	settings->AUTO_POSITION = auto_position->IsChecked();
	settings->ENABLE_TASKS = enable_tasks->IsChecked();	
	settings->ENABLE_MINIMIZE = enable_minimize->IsChecked();
	settings->FAST_ANIMATIONS = fast_animations->IsChecked();

    if (!checkInt(blur_timeout_text, &settings->BLUR_TIMEOUT,10,200)) {
        return NULL;
    }
    return settings;
}



wxPanel* SettingsDialog::CreateGeneralSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    
    /* ---------------  ZOOM STUFF  ----------------------*/
    wxStaticBoxSizer* item2 = new wxStaticBoxSizer(wxVERTICAL, panel, _T("Zoom"));
    /* Zoom */
    wxBoxSizer* itemSizer6 = new wxBoxSizer( wxHORIZONTAL );
    zoom_text = new wxSpinCtrl(panel, ID_PERCENT, wxString::Format(_T("%d"),settings->PERCENT));
    zoom_text->SetRange(0, 1000);
    itemSizer6->Add(new wxStaticText(panel,-1,_T("Zoom")),wxGROW|wxALL,5);
    itemSizer6->Add(zoom_text,wxGROW|wxALL,5);
    item2->Add(itemSizer6, 0, wxGROW|wxALL, 5);
    
    /* Zoom range */
    wxBoxSizer* itemSizer7 = new wxBoxSizer( wxHORIZONTAL );
    range_text = new wxSpinCtrl(panel, ID_RANGE,wxString::Format(_T("%d"),settings->RANGE));
    range_text->SetRange(0, 1000);
    itemSizer7->Add(new wxStaticText(panel,-1,_T("Range")),wxGROW|wxALL,5);
    itemSizer7->Add(range_text,wxGROW|wxALL,5);
    item2->Add(itemSizer7, 0, wxGROW|wxALL, 5);

    /* Fast Animations */
    fast_animations = new wxCheckBox(panel,ID_FAST_ANIMATIONS, _T("Fast Animations"));
    fast_animations->SetValue(settings->FAST_ANIMATIONS);
    item2->Add(fast_animations, 0, wxGROW|wxALL, 5);

    topSizer->Add( item2, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    /* ---------------  ZOOM STUFF  ----------------------*/
    
    
    /* ---------------  BEHAVIOUR STUFF  ----------------------*/
    wxStaticBoxSizer* item3 = new wxStaticBoxSizer(wxVERTICAL, panel, _T("Behaviour"));
    /* Automatic positioning */    
    auto_position = new wxCheckBox(panel, ID_AUTO_POSITION, _T("Automatic positioning"));
    auto_position->SetValue(settings->AUTO_POSITION);
    item3->Add(auto_position, 0, wxGROW|wxALL, 5);
    
    /* Enable tasks */
    enable_tasks = new wxCheckBox(panel,ID_ENABLE_TASKS, _T("Show tasks"));
    enable_tasks->SetValue(settings->ENABLE_TASKS);
    item3->Add(enable_tasks, 0, wxGROW|wxALL, 5);
    
    /* Enable minimize */
    enable_minimize = new wxCheckBox(panel,ID_Enable_Minimize,_T("Minimize Windows"));
    enable_minimize->SetValue(settings->ENABLE_MINIMIZE);
    item3->Add(enable_minimize, 0, wxGROW|wxALL, 5);

    
    wxBoxSizer* itemSizer8 = new wxBoxSizer( wxHORIZONTAL );
    blur_timeout_text =  new wxSpinCtrl(panel, ID_BLUR_TIMEOUT,wxString::Format(_T("%d"),settings->BLUR_TIMEOUT));
    blur_timeout_text->SetRange(10, 200);
    itemSizer8->Add(new wxStaticText(panel,-1,_T("Blur length")),wxGROW|wxALL,5);
    itemSizer8->Add(blur_timeout_text,wxGROW|wxALL,5);
    item3->Add(itemSizer8, 0, wxGROW|wxALL, 5);

    topSizer->Add( item3, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    /* ---------------  BEHAVIOUR STUFF  ----------------------*/

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

wxPanel* SettingsDialog::CreateBackgroundPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    
    /* --------------- BACKGROUND STUFF ----------------------*/
    wxStaticBoxSizer* item0 = new wxStaticBoxSizer(wxVERTICAL, panel, _T("Background"));


    /* Background height */
    wxBoxSizer* itemSizer4 = new wxBoxSizer( wxHORIZONTAL );
    bg_height_text = new wxSpinCtrl(panel, ID_BG_HEIGHT, wxString::Format(_T("%d"),settings->BG_HEIGHT));
    bg_height_text->SetRange(0, 1000);
    itemSizer4->Add(new wxStaticText(panel,-1,_T("Background Height")),wxGROW|wxALL,5);
    itemSizer4->Add(bg_height_text,wxGROW|wxALL,5);
    item0->Add(itemSizer4, 0, wxGROW|wxALL, 5);
    
    /* Background path */
    wxBoxSizer* itemSizer5 = new wxBoxSizer( wxHORIZONTAL );
    bg_path_text = new wxTextCtrl(panel, ID_bgPath, settings->BG_PATH,wxDefaultPosition,wxSize(150,wxDefaultSize.GetHeight()));
    itemSizer5->Add(new wxStaticText(panel,-1,_T("Background Path")),wxGROW|wxALL,5);
    itemSizer5->Add(bg_path_text,wxGROW|wxALL,5);
    
    /* Browse button */
    wxBoxSizer* itemSizer6 = new wxBoxSizer( wxHORIZONTAL );
    browse_button  = new wxButton(panel,ID_Browse_Bg, _T("Browse"));  
    itemSizer6->Add( 200,20, 1 );
    itemSizer6->Add(browse_button,0,wxRIGHT);
    
    item0->Add(itemSizer5, 0, wxGROW|wxALL, 5);
    item0->Add(itemSizer6, wxALL,5);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    /* --------------- BACKGROUND STUFF ----------------------*/
    
    
    /* --------------- REFLEX STUFF ----------------------*/
    wxStaticBoxSizer* item3 = new wxStaticBoxSizer(wxVERTICAL, panel, _T("Reflexes"));
    
    /* enable reflexes */
    reflex_enabled = new wxCheckBox(panel, ID_SHOW_REFLEXES,_T("Enable reflexes"));
    reflex_enabled->SetValue(settings->SHOW_REFLEXES);
    item3->Add(reflex_enabled, 0, wxGROW|wxALL, 5);
    
    /* reflex scaling */
    wxBoxSizer* itemSizer9 = new wxBoxSizer( wxHORIZONTAL );
    reflex_scaling_text = new wxSpinCtrl(panel, ID_REFLEX_SCALING,wxString::Format(_T("%d"),settings->REFLEX_SCALING));
    reflex_scaling_text->SetRange(0, 100);
    itemSizer9->Add(new wxStaticText(panel,-1,_T("Reflex scaling")),wxGROW|wxALL,5);
    itemSizer9->Add(reflex_scaling_text,wxGROW|wxALL,5);
    item3->Add(itemSizer9, 0, wxGROW|wxALL, 5);
    
    
    /* reflex transparency */
    wxBoxSizer* itemSizer10 = new wxBoxSizer( wxHORIZONTAL );
    reflex_alpha_text = new wxSpinCtrl(panel, ID_REFLEX_ALPHA,wxString::Format(_T("%d"),settings->REFLEX_ALPHA));
    reflex_alpha_text->SetRange(0, 255);
    itemSizer10->Add(new wxStaticText(panel,-1,_T("Reflex transparency")),wxGROW|wxALL,5);
    itemSizer10->Add(reflex_alpha_text,wxGROW|wxALL,5);
    item3->Add(itemSizer10, 0, wxGROW|wxALL, 5);
   
    
    

    topSizer->Add( item3, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    /* --------------- REFLEX STUFF ----------------------*/
    
    
    
    
    
    
    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

wxPanel* SettingsDialog::CreateAestheticSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxStaticBoxSizer* item0 = new wxStaticBoxSizer(wxVERTICAL, panel, _T("Borders"));   


    /* Left border */
    wxBoxSizer* itemSizer0 = new wxBoxSizer( wxHORIZONTAL );
    left_border_text = new wxSpinCtrl(panel, ID_LEFT_BORDER, wxString::Format(_T("%d"),settings->LEFT_BORDER));
    left_border_text->SetRange(0, 1000);
    itemSizer0->Add(new wxStaticText(panel,-1,_T("Left Border")),wxGROW|wxALL,5);
    itemSizer0->Add(left_border_text,wxGROW|wxALL,5);
    item0->Add(itemSizer0, 0, wxGROW|wxALL, 5);
    
    /* Right border */
    wxBoxSizer* itemSizer1 = new wxBoxSizer( wxHORIZONTAL );
    right_border_text = new wxSpinCtrl(panel, ID_RIGHT_BORDER, wxString::Format(_T("%d"),settings->RIGHT_BORDER));
    right_border_text->SetRange(0, 1000);
    itemSizer1->Add(new wxStaticText(panel,-1,_T("Right Border")),wxGROW|wxALL,5);
    itemSizer1->Add(right_border_text,wxGROW|wxALL,5);
    item0->Add(itemSizer1, 0, wxGROW|wxALL, 5);
    
    /* Bottom border */
    wxBoxSizer* itemSizer2 = new wxBoxSizer( wxHORIZONTAL );
    bottom_border_text = new wxSpinCtrl(panel,ID_BOTTOM_BORDER,wxString::Format(_T("%d"),settings->BOTTOM_BORDER));
    bottom_border_text->SetRange(0, 1000);
    itemSizer2->Add(new wxStaticText(panel,-1,_T("Bottom Border")),wxGROW|wxALL,5);
    itemSizer2->Add(bottom_border_text,wxGROW|wxALL,5);
    item0->Add(itemSizer2, 0, wxGROW|wxALL, 5);
    
    /* Spacer */
    wxBoxSizer* itemSizer3 = new wxBoxSizer( wxHORIZONTAL );
    spacer_border_text = new wxSpinCtrl(panel,ID_SPACER,wxString::Format(_T("%d"),settings->SPACER));
    spacer_border_text->SetRange(0, 1000);
    itemSizer3->Add(new wxStaticText(panel,-1,_T("Icons spacer")),wxGROW|wxALL,5);
    itemSizer3->Add(spacer_border_text,wxGROW|wxALL,5);
    item0->Add(itemSizer3, 0, wxGROW|wxALL, 5);
    
    
    
    wxStaticBoxSizer* item1 = new wxStaticBoxSizer(wxVERTICAL, panel, _T("Icons"));
    
    /* Icons width */
    wxBoxSizer* itemSizer4 = new wxBoxSizer( wxHORIZONTAL );
    width_text = new wxSpinCtrl(panel, ID_ICONW,wxString::Format(_T("%d"),settings->ICONW));
    width_text->SetRange(0, 200);
    itemSizer4->Add(new wxStaticText(panel,-1,_T("Icons width")),wxGROW|wxALL,5);
    itemSizer4->Add(width_text,wxGROW|wxALL,5);
    item1->Add(itemSizer4, 0, wxGROW|wxALL, 5);
    
    /* Icons height */
    wxBoxSizer* itemSizer5 = new wxBoxSizer( wxHORIZONTAL );
    height_text = new wxSpinCtrl(panel, ID_ICONH,wxString::Format(_T("%d"),settings->ICONH));
    height_text->SetRange(0, 200);
    itemSizer5->Add(new wxStaticText(panel,-1,_T("Icons height")),wxGROW|wxALL,5);
    itemSizer5->Add(height_text,wxGROW|wxALL,5);
    item1->Add(itemSizer5, 0, wxGROW|wxALL, 5);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    topSizer->Add( item1, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    topSizer->AddSpacer(5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

/* -------------- Event callbacks -------------------- */


void SettingsDialog::OnBrowseEvent(wxCommandEvent& event)
{
    wxFileName fn(bg_path_text->GetValue());

    wxString filename = wxFileSelector(_T("Choose a file to open"),fn.GetPath(), _T(""), _T("png"), _T( "BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|PNG files (*.png)|*.png|All files (*.*)|*.*"), wxOPEN | wxFILE_MUST_EXIST);
    if ( !filename.empty() )
    {
        bg_path_text->SetValue(filename);
    }
    this->OnChange(event);
}

void SettingsDialog::OnChange(wxCommandEvent& event) {
    this->SaveSettings();
}
void SettingsDialog::OnSpinChange(wxSpinEvent& event) {
    this->SaveSettings();
}

void SettingsDialog::SaveSettings() {
    simSettings *settings = this->GetSettings ();
    if (settings) {
        simGconf_saveSettings (settings);
    }
    
}



