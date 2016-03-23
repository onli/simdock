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
#include "launcher_dialog.h"

using namespace std;

enum
{
ID_bmp_button = 99,
};

IMPLEMENT_CLASS(LauncherDialog, wxDialog)


BEGIN_EVENT_TABLE(LauncherDialog, wxDialog)
EVT_BUTTON(ID_bmp_button, LauncherDialog::OnBrowseEvent)
END_EVENT_TABLE()

wxString LauncherDialog::lastPath = wxString();

void LauncherDialog::SetIcon(const wxBitmap& value)
{
	wxBitmap v;
	if (value.GetWidth() != 50 || value.GetHeight() != 50) {
		v = value.ConvertToImage().Scale(50,50);
	} else {
		v = value;
	}
	bmp_button->SetBitmapLabel(v);
}

bool LauncherDialog::saveChanges()
{
if (!bmp_path.IsEmpty() && bmp_path != launcher->img_link)
{
	launcher -> img_link  = bmp_path;
	if (!launcher ->loadImage(bmp_path))
	{
	      wxMessageBox (_T ("Error! Could not load Image"),
		    _T ("SimDock"), wxOK | wxICON_INFORMATION, NULL);
		    
		return false;	
		}
}

if (!launcher -> img.IsOk())
{
      wxMessageBox (_T ("Error! Could not load Background Image!"),
		    _T ("SimDock"), wxOK | wxICON_INFORMATION, NULL);
return false;
}

launcher->name = name_text->GetValue();
launcher->descr = description_text->GetValue();
launcher->link = command_text->GetValue();

return true;


}

LauncherDialog::LauncherDialog(wxWindow* parent, simImage* launcher): wxDialog(parent, -1, _T("Add/Edit Launcher"))
	{
	if (!parent || !launcher)
	{
	printf ("LauncherDialog::LauncherDialog - NULL argument!\n");
	return;
	}
    this->launcher = launcher;
    bmp_path = launcher->img_link;
    
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
   
    wxStaticBoxSizer* LauncherBox = new wxStaticBoxSizer(wxVERTICAL, this, _T("Launcher"));
    
    wxBoxSizer* Horizontal0 = new wxBoxSizer( wxHORIZONTAL );
    
    bmp_button = new wxButton(this,ID_bmp_button,_T("Icon"),wxDefaultPosition,wxSize(60,60));
	if (launcher->img.IsOk()) {
		SetIcon(wxBitmap(launcher->img));
	}
    Horizontal0->Add(bmp_button,0,5);
    LauncherBox->Add(Horizontal0, 0, wxGROW|wxALL, 5);
    
 //Name
    wxBoxSizer* Horizontal1 = new wxBoxSizer( wxHORIZONTAL );
    name_text = new wxTextCtrl(this,-1,launcher->name);
    Horizontal1->Add(new wxStaticText(this,-1,_T("Name")),wxGROW|wxALL,5);
    Horizontal1->Add(name_text,wxGROW|wxALL,5);
    LauncherBox->Add(Horizontal1, 0, wxGROW|wxALL, 5);
    
//Command
    wxBoxSizer* Horizontal2 = new wxBoxSizer( wxHORIZONTAL );
    command_text = new wxTextCtrl(this,-1,launcher->link,wxDefaultPosition,wxSize(150,wxDefaultSize.GetHeight()));
    Horizontal2->Add(new wxStaticText(this,-1,_T("Command")),wxGROW|wxALL,5);
    Horizontal2->Add(command_text,wxGROW|wxALL,5);
    LauncherBox->Add(Horizontal2, 0, wxGROW|wxALL, 5);
    
//Description
    wxBoxSizer* Horizontal3 = new wxBoxSizer( wxHORIZONTAL );
    description_text = new wxTextCtrl(this,-1,launcher->descr,wxDefaultPosition,wxSize(150,wxDefaultSize.GetHeight()));
    Horizontal3->Add(new wxStaticText(this,-1,_T("Description")),wxGROW|wxALL,5);
    Horizontal3->Add(description_text,wxGROW|wxALL,5);
    LauncherBox->Add(Horizontal3, 0, wxGROW|wxALL, 5);
    
    wxSizer * sz = CreateButtonSizer(wxOK|wxCANCEL);
    LauncherBox->Add(sz, 0, wxGROW|wxALL, 5);
 
    topSizer->Add(LauncherBox, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 ); 
    
    if (launcher->task)
    {
    	description_text->Enable(false);
    	command_text->Enable(false);
    	name_text->Enable(false);
    	bmp_button->Enable(false);
    }
    
    this->SetSizer(topSizer);
    topSizer->Fit(this);
    

}
void LauncherDialog::OnBrowseEvent(wxCommandEvent& event)
{
	wxString startingPath = lastPath;
	if (!bmp_path.IsEmpty()) {
		startingPath = bmp_path;
	}
	
	wxFileName fn(startingPath);
	
	wxString filename = wxFileSelector(_T("Choose a file to open"),fn.GetPath() /*wxPathOnly(startingPath)*/, _T(""), _T("png"), wxImage::GetImageExtWildcard() + ";*.svg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if ( !filename.empty() )
	{
		bmp_path = filename;
		SetIcon(wxBitmap(filename));
		lastPath = filename;
	}


}

LauncherDialog::~LauncherDialog()
{
//	delete(panel);
}
