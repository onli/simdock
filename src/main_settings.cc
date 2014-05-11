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

#include "main.h"
using namespace std;
bool
fixSimDir (wxString * dirPath)
{
  if (!wxDirExists (*dirPath))
    {
      if (!wxMkdir (*dirPath))
	{
	  cout << "Could not Create Directory " << wx2std (*dirPath) << endl;
	  return FALSE;
	}
    }
  return TRUE;
}

bool
openOrInitialize (wxString * fullPath, string *initializeData)
{
    wxFile settingsFile;
    if (!settingsFile.Exists (*fullPath)) {
        if (!openOrCreate (&settingsFile, fullPath)) {
            exit (1);
        }

        if (! settingsFile.Write (wxString(initializeData->c_str(), wxConvUTF8))) {
            wxMessageBox (  _T ("Some error occurred writing to file?"),
                            _T ("SimDock"), wxOK | wxICON_INFORMATION, NULL);
            return FALSE;
        }
    }     

    return TRUE;
}



bool
openOrCreate (wxFile * settingsFile, wxString * fullPath)
{
  if (!settingsFile->Create (*fullPath, true))
    {
      wxMessageBox (_T ("Could not Create file!!!"), _T ("SimDock"),
		    wxOK | wxICON_INFORMATION, NULL);
      return FALSE;
    }
  if (!settingsFile->Open (*fullPath, wxFile::write))
    {
      wxMessageBox (_T ("Could not open file for writing!!!"), _T ("SimDock"),
		    wxOK | wxICON_INFORMATION, NULL);
      return FALSE;
    }
  return TRUE;
}
