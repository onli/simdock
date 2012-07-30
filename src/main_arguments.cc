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
#define SIMDOCK_INFOS "SimDock " SIMDOCK_VERSION " by Simone Della Longa, Malte Paskuda\nYou can add new programs to the list editing the file ~/.simdock/settings.xml"
using namespace std;

void
MyApp::OnInitCmdLine (wxCmdLineParser & parser)
{
  parser.SetDesc (g_cmdLineDesc);
  parser.SetLogo (_T (SIMDOCK_INFOS));
  // must refuse '/' as parameter starter or cannot use "/path" style paths
  parser.SetSwitchChars (wxT ("-"));
}


bool MyApp::OnCmdLineParsed (wxCmdLineParser & parser)
{
  wxString
    stretched = _T (STRETCHED_STR);
  wxString
    none = _T (NONE_STR);
  wxString
    centered = _T (CENTERED_STR);


  /* ------- handling Back file --------- */
  parser.Found (wxT ("b"), &customBackground);


  /* ------- handling back type --------- */
  wxString
    backType;
  customBackgroundType = STRETCHED;	//default
  if (parser.Found (wxT ("t"), &backType))
    {
      bool
	found = false;
      if (backType == stretched)
	{
	  customBackgroundType = STRETCHED;
	  found = true;
	}
      if (backType == none)
	{
	  customBackgroundType = NONE;
	  found = true;
	}
      if (backType == centered)
	{
	  customBackgroundType = CENTERED;
	  found = true;
	}
      if (!found)
	{
	  cout << "Invalid background type!!" << endl;
	  exit (1);
	}
    }


  /* ------- handling Colour --------- */
  wxString
    color;
  if (parser.Found (wxT ("o"), &color))
    {
      if (!customBackgroundColour.Set (color))
	{
	  cout << "Invalid color type!! run with --help for details" << endl;
	  exit (1);
	}
    }
  else
    {
      customBackgroundColour.Set (_T ("black"));
    }

  /* --------- handling Show In Tray ------- */
  showInTray = parser.Found (wxT ("r"));
  
  onTop = parser.Found (wxT("y"));


  //silent_mode = parser.Found(wxT("s"));
  /*
     if (parser.Found(wxT("h")))
     {
     cout << "Help" <<endl;
     }
   */
  // to get at your unnamed parameters use
  /*
     wxArrayString files;
     for (unsigned int i = 0; i < parser.GetParamCount(); i++)
     {
     files.Add(parser.GetParam(i));
     }
   */
  // and other command line parameters

  // then do what you need with them.

  return true;
}
