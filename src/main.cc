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
#include "math.h"
#include "main.h"
// #include <wx/listimpl.cpp> //To use Lists

char defaultLaunchers[] = "<Program>\n"
    "<SimDock>\n"
    "<path>/usr/bin/firefox</path>\n"
    "<icon>/usr/share/firefox/icons/mozicon128.png</icon>\n"
    "<description>Firefox web browser</description>\n"
    "<name>Firefox</name>\n" "</SimDock>\n" "</Program>\n";

char defaultSetts[] = "<Program><SimDock>\n"
    "<x>100</x>\n" "<y>100</y>\n" "</SimDock></Program>";

char first_time[] =
    "This is the first time you run this program.\n"
    "Generated a default settings file\nPlease edit ~/" CONF_DIR "/"
    CONF_FILE " to add new launchers\n";

#define _P(x) wx2std(x)
std::string wx2std (const wxString & input)
{
    int size = input.Length () + 1;	// will be big enough
    char *buffer = new char[size];
    wxEncodingConverter wxec;
    wxec.Init (wxFONTENCODING_UNICODE, wxFONTENCODING_ISO8859_1);
    wxec.Convert (input.wc_str (wxConvUTF8), buffer);
    std::string temp (buffer);
    delete buffer;
    return temp;
}

using namespace std;





IMPLEMENT_APP (MyApp)



int	startPositionX = 50;
int startPositionY = 50;
wxString dirPath;
wxString programsPath;

wxString markPath = _T (DATA_DIR "/mark.png");
wxString questionPath = _T (DATA_DIR "/question.png");






bool
loadSettings (simSettings* settings)
{
    // Loading default settings
    //struct simSettings data ;
    /*
	{ LEFT_BORDER, RIGHT_BORDER, BOTTOM_BORDER, ICONW, ICONH,
	PERCENT, RANGE,
	SPACER, BG_HEIGHT, bgPath, SHOW_REFLEXES,
	REFLEX_SCALING, REFLEX_ALPHA
    };*/
    simGconf_loadSettings (settings);
    /*
    LEFT_BORDER = data.LEFT_BORDER;
    RIGHT_BORDER = data.RIGHT_BORDER;
    BOTTOM_BORDER = data.BOTTOM_BORDER;
    ICONW = data.ICONW;
    ICONH = data.ICONH;
    PERCENT = data.PERCENT;
    RANGE = data.RANGE;
    SPACER = data.SPACER;
    BG_HEIGHT = data.BG_HEIGHT;
    bgPath = data.BG_PATH;
    SHOW_REFLEXES = data.SHOW_REFLEXES;
    REFLEX_SCALING = data.REFLEX_SCALING;
    REFLEX_ALPHA = data.REFLEX_ALPHA;
*/
    simGconf_loadPosition (&startPositionX, &startPositionY);
    settings->MAXSIZE = settings->ICONW + settings->ICONW * settings->PERCENT / 100;

    return TRUE;
}

bool
loadPrograms (wxString fullPath, ImagesArray * list,simSettings* settings)
{
    openOrInitialize (&fullPath, defaultLaunchers);
    wxXmlDocument doc;
    if (!doc.Load (fullPath))
      {
	  exit (1);
	  return FALSE;
      }
    // start processing the XML file
    if (doc.GetRoot ()->GetName () != wxT (XML_PROGRAM))
      {
	  cout << "invalid xml file" << endl;
	  return FALSE;
      }

    wxXmlNode *program = doc.GetRoot ()->GetChildren ();
    int id = 0;
    while (program)
      {

	  if (program->GetName () == wxT (XML_SIMDOCK))
	    {
		wxXmlNode *child = program->GetChildren ();

		wxString path, icon, descr, name;
		while (child)
		  {
		      if (child->GetName () == wxT (XML_PATH))
			  path = child->GetNodeContent ();
		      if (child->GetName () == wxT (XML_ICON))
			  icon = child->GetNodeContent ();
		      if (child->GetName () == wxT (XML_DESCR))
			  descr = child->GetNodeContent ();
		      if (child->GetName () == wxT (XML_NAME))
			  name = child->GetNodeContent ();
		      child = child->GetNext ();
		  }
		if (!path.IsEmpty () && !icon.IsEmpty ())
		  {
		      wxImage img;
		      if (!img.LoadFile (icon))
			  {
				img.LoadFile(questionPath);
			  }
			    simImage *sim = new simImage (img,
							  icon,
							  path,
							  name,
							  descr,
							  id++);
			    sim->w = settings->ICONW;
			    sim->h = settings->ICONH;
			    sim->y = (settings->MAXSIZE + settings->BOTTOM_BORDER) - settings->ICONH - settings->BOTTOM_BORDER;	// I 
			    // know, 
			    // it's 
			    // ugly

			    list->Add (sim);
#ifdef SIMDOCK_DEBUG
			    cout << id << ":" << wx2std (name) << endl;
#endif



		  }
	    }
	  program = program->GetNext ();

      }

    return TRUE;
}

void
loadAll (ImagesArray * list, wxSize sz, simSettings* settings)
{
    wxString home = wxGetHomeDir ();
    if (home.IsEmpty ())
      {
	  cout <<
	      "Could not determine user's home directory."
	      "\nplease set HOME environment variable"
	      << endl;
	  exit (1);
      }

    dirPath = home + _T ("/") + _T (CONF_DIR);
    programsPath = dirPath + _T ("/") + _T (CONF_FILE);

    if (!fixSimDir (&dirPath))
      {
	  exit (1);
      }
    if (!loadSettings (settings))
      {
	  exit (1);
      }
    if (!loadPrograms (programsPath, list,settings))
      {
	  exit (1);
      }
}


wxSize
PositionIcons (wxSize sz, int ind, simSettings* settings, ImagesArray* ImagesList)
{
	 /*
     * Trying to implement right AND left side repositioning.... 
     */
#if 0    
     int index ; if (ind < 0 || ind >ImagesList.GetCount()-1) { index =
      ImagesList.GetCount() /2; cout << "ERROR: Invalid index value"
      <<endl; } else { index = id; }
      
      int width = ImagesList[index]->w; int positionX =
      ImagesList[index]->x; for (unsigned int i =index;
      i<ImagesList.GetCount() ; i++) { simImage *img = ImagesList[i];
      img->x = positionX; positionX += img->w + SPACER; }
      
      if (index > 0) { positionX = ImagesList[index]->x; for (int i
      =index-1; i>=0 ; i--) {
      
      simImage *img = ImagesList[i]; positionX -= img->w + SPACER; img->x 
      = positionX; }
      
      } if (ImagesList.GetCount() >0) {
      wxSize(ImagesList[ImagesList.GetCount()-1]->x +
      ImagesList[ImagesList.GetCount()-1]->w + LEFT_BORDER,
      sz.GetHeight()); }
      
      return wxSize();
      
 #endif
    
    /*
     * -------------------Right-side only repositioning--------------- 
     */
    int positionX = settings->LEFT_BORDER;

    for (unsigned int i = 0; i < ImagesList->GetCount (); i++)
      {
	  simImage *img = (*ImagesList)[i];

	  img->x = positionX;
	  positionX += img->w + settings->SPACER;

	  if (i == ImagesList->GetCount () - 1)
	    {
		return wxSize (positionX - settings->SPACER + settings->LEFT_BORDER,
			       sz.GetHeight ());
	    }
#ifdef SIMDOCK_DEBUG_ICON_POSITIONING
	  cout << "ID:" << img->id << "[" << img->x << "," << img->
	      y << "]" << "[" << img->w << "," << img->h << "]" << endl;
#endif
      }
    return wxSize ();

}

/*
 * Might change this if i'll implement right and left side repositioning 
 */
wxSize
FirstPosition (wxSize sz, simSettings* settings, ImagesArray* list)
{
    return PositionIcons (sz, 0, settings,list);
}


void
MyApp::GracefullyExit ()
{
    if (!frame)
      {
	  cout << "GracefullyExit: Null Frame!!" << endl;
	  return;
      }
    if (!frame->disposed)
      {
	  simGconf_savePosition (frame->GetPosition ().x, frame->GetPosition ().y);
	  if (launchersModified)
	  	saveLaunchers (ImagesList, programsPath);
      }
    else
      {
	  simGconf_savePosition (frame->lastPosition.x, frame->lastPosition.y);
	  if (launchersModified)
	  	saveLaunchers (ImagesList, programsPath);
      }
}

int
MyApp::OnExit ()
{
    GracefullyExit ();
    return 0;			// Reference sais this is ignored..
}




bool
MyApp::OnInit ()
{
    if (!wxApp::OnInit ())
	return false;
	
	ImagesList = new ImagesArray ();
	launchersModified = false;
	wxInitAllImageHandlers ();
	
	//default values
	int LEFT_BORDER = 30;	// 30
    int RIGHT_BORDER = 90;	// 90
    int BOTTOM_BORDER = 15;	// 30 space from bottom to the icon list
    int ICONW = 30;		// Default icon size
    int ICONH = 30;		// Default icon size
    int PERCENT = 50;	// 100 //Maximum increase
    int RANGE = 90;		// 180 //Where it has no effect
    int SPACER = 8;		// 10 //Pixel space between 2 starters
    int BG_HEIGHT = 30;	// 70
	bool ENABLE_TASKS = true;
    bool AUTO_POSITION = true;
	bool SHOW_REFLEXES = true;
    int REFLEX_SCALING = 5;
    int REFLEX_ALPHA = 100;
	wxString bgPath = _T (DATA_DIR "/bg5.png");
	 
	settings = new simSettings( LEFT_BORDER, RIGHT_BORDER, BOTTOM_BORDER, ICONW, ICONH,
	PERCENT, RANGE,
	SPACER, BG_HEIGHT, bgPath, SHOW_REFLEXES,
	REFLEX_SCALING, REFLEX_ALPHA,AUTO_POSITION,ENABLE_TASKS
    );
    
    //sMAXSIZE = settings->ICONW + settings->ICONW * settings->PERCENT / 100;

  
	
      
      
    //src_dc = new wxMemoryDC (*backImage);


    long options = wxNO_BORDER;
    if (!showInTray)
    {
	  options = options | wxFRAME_NO_TASKBAR;
    }

    if (onTop)
	options = options | wxSTAY_ON_TOP;

    frame = new MyFrame (NULL, settings,ImagesList,-1, _T ("SimDock"), wxPoint (10, 10),
			 wxSize (450, 150), options);
    wxImage* appBackground = new wxImage (settings->BG_PATH);
    appBackground->Rescale (frame->GetClientSize ().GetWidth(), settings->BG_HEIGHT, wxIMAGE_QUALITY_HIGH);
	frame->SetBG(appBackground);
	
	wxBitmap* bmp = new wxBitmap(wxImage (markPath));
	frame->SetMarkBitmap(bmp);
    frame->frameOptions = options;
    


    loadAll (ImagesList, frame->GetClientSize (), settings);

	frame->updateSize();
	
    register_sigint (this);	// CTRL-C Handling
    
    frame->Show (TRUE);
    frame->Freeze();
    xstuff_resizeScreen (wxFileNameFromPath (wxString (argv[0])), *frame,true);
    if (settings->ENABLE_TASKS)
    {
		tasks_fillList(ImagesList,settings);
		tasks_register_signals(ImagesList,settings);
    }
	frame->appSize = FirstPosition (frame->GetClientSize (),settings,ImagesList);
	frame->updateSize();
	if (!settings->AUTO_POSITION)
    {
		frame->Move (startPositionX, startPositionY);
    }
    else
    {
    	wxSize sz = wxGetDisplaySize();
    	frame->Move ((sz.GetWidth() - frame->GetClientSize().GetWidth()) / 2, 
    	sz.GetHeight() - frame->GetClientSize().GetHeight());
    }
    //getting the background from root will only work at a late point
    //like this
	wxBitmap * backImage;
    if (!customBackground.IsEmpty ())
    {
        backImage =
            fixImage (customBackground, customBackgroundType,
                        customBackgroundColour);
    }
    else
    {
        backImage = getRootWallpaper();
    }
    frame->SetWallpaper(backImage);
    
	frame->Thaw();
    
    return TRUE;
}




