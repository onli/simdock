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
using namespace std;

#define _P(x) wx2std(x)
std::string wx2std (const wxString & input)
{
    int size = input.Length () + 1;	// will be big enough
    char *buffer = new char[size];
    wxEncodingConverter wxec;
    wxec.Init (wxFONTENCODING_UNICODE, wxFONTENCODING_ISO8859_1);
    wxec.Convert (input.wc_str (wxConvUTF8), buffer);
    std::string temp (buffer);
    delete [] buffer;
    return temp;
}

string
getGTKIconPath(string name) {
    GtkIconInfo *iconInfo = gtk_icon_theme_lookup_icon (gtk_icon_theme_get_default(),
                                   name.c_str(), // icon name
                                   48,
                                   GTK_ICON_LOOKUP_FORCE_SIZE);

    if (iconInfo != NULL) {
        return gtk_icon_info_get_filename (iconInfo);
    }
    return string("");
}


IMPLEMENT_APP (MyApp)



int	startPositionX = 50;
int startPositionY = 50;
double borderRatio; // border size ratio, left to right
wxString dirPath;

wxString get_selfpath() {
    char buff[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len != -1) {
      buff[len] = '\0';
      return wxString::FromUTF8(buff);
    }
    /* handle error condition */
}

wxFileName selfexe(get_selfpath());
wxString exeDir = selfexe.GetPath();
wxString markPath = exeDir + _T("/../share/simdock/mark.png");
wxString questionPath = exeDir + _T("/../share/simdock/question.png");
wxString bgPath = exeDir + _T("/../share/simdock/bg5.png");

bool
loadSettings (simSettings* settings)
{
    simGconf_loadSettings (settings);
    simGconf_loadPosition (&startPositionX, &startPositionY);
    settings->MAXSIZE = (int)(settings->ICONH + (settings->ICONH * ((double)settings->PERCENT / 100)));
    borderRatio = (double)settings->LEFT_BORDER / (settings->LEFT_BORDER + settings->RIGHT_BORDER);
    return TRUE;
}

bool
loadPrograms (wxString fullPath, ImagesArray * list,simSettings* settings, string defaultLaunchers)
{
    openOrInitialize (&fullPath, &defaultLaunchers);
    wxXmlDocument doc;
    if (!doc.Load (fullPath)) {
        exit (1);
        return FALSE;
	}
    // start processing the XML file
    if (doc.GetRoot ()->GetName () != wxT (XML_PROGRAM)) {
	    cout << "invalid xml file" << endl;
	    return FALSE;
	}

    wxXmlNode *program = doc.GetRoot ()->GetChildren ();
    int id = 0;
    while (program) {

		if (program->GetName () == wxT (XML_SIMDOCK)) {
			wxXmlNode *child = program->GetChildren ();

			wxString path, icon, descr, name;
			while (child) {
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
			if (!path.IsEmpty () && !icon.IsEmpty ()) {
		        wxImage img;
				if (icon.EndsWith(".svg")) {
					// we use the questionPath as placeholder for the constructor
					img.LoadFile(questionPath);
				} else {
					if (!img.LoadFile (icon)) {
						img.LoadFile(questionPath);
					}
				}
			    simImage *sim = new simImage (img,
							  icon,
							  path,
							  name,
							  descr,
							  id++);
			    sim->w = settings->ICONW;
			    sim->h = settings->ICONH;
			    sim->y = (settings->MAXSIZE + settings->BOTTOM_BORDER) - settings->ICONH - settings->BOTTOM_BORDER;
				if (icon.EndsWith(".svg")) {
					sim->loadImage(icon);
				}
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
loadAll (ImagesArray * list, simSettings* settings, string defaultLaunchers)
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
    if (!fixSimDir (&dirPath)) {
        exit (1);
    }
    if (!loadSettings (settings)) {
        exit (2);
    }
    if (!loadPrograms (PROGRAMS_PATH, list,settings, defaultLaunchers)) {
        exit (3);
    }
}


wxSize
PositionIcons (simSettings settings, ImagesArray* ImagesList, bool* changeIcons) {
    int neededSpace = 0;
    unsigned int imgCount = ImagesList->GetCount();
    int availableSpace = settings.LEFT_BORDER +
                            imgCount * (settings.ICONW + settings.SPACER) +
                            settings.RIGHT_BORDER - settings.SPACER;
    for (unsigned int i = 0; i < imgCount; i++) {
        simImage *img = (*ImagesList)[i];
        neededSpace += img->w + settings.SPACER;
    }
    neededSpace -= settings.SPACER;
    int positionX = (availableSpace - neededSpace) * borderRatio;
    for (unsigned int i = 0; i < imgCount; i++) {
        simImage *img = (*ImagesList)[i];
        if (changeIcons[i] == true) {
            img->x = positionX;
        }
        positionX += img->w + settings.SPACER;
    }
    
    return wxSize (availableSpace, settings.MAXSIZE);
}

wxSize
FirstPosition (simSettings settings, ImagesArray* list)
{
    bool changeIcons[list->GetCount()];
    for (unsigned int i = 0; i < list->GetCount(); i++) {
        changeIcons[i] = true;
    }
    return PositionIcons (settings,list, changeIcons);
}

void
MyApp::GracefullyExit () {
    if (!frame) {
        cout << "GracefullyExit: Null Frame!!" << endl;
        return;
    }
    if (!frame->disposed) {
        simGconf_savePosition (frame->GetPosition ().x, frame->GetPosition ().y);
    } else {
        simGconf_savePosition (frame->lastPosition.x, frame->lastPosition.y);
    }
}

int
MyApp::OnExit ()
{
    GracefullyExit ();
    return 0;			// Reference sais this is ignored..
}

bool
MyApp::OnInit () {
    if (!wxApp::OnInit ()) { return false; }

    string filepath = getGTKIconPath("firefox");
    defaultLaunchers = "<Program>\n"
        "<SimDock>\n"
        "</SimDock>\n"
        "</Program>\n";
    if (filepath != "") {
        defaultLaunchers = "<Program>\n"
        "<SimDock>\n"
        "<path>/usr/bin/firefox</path>\n"
        "<icon>" + getGTKIconPath("firefox") + "</icon>\n"
        "<description>Firefox web browser</description>\n"
        "<name>Firefox</name>\n" "</SimDock>\n" "</Program>\n";
    }
	
	ImagesList = new ImagesArray ();
	wxInitAllImageHandlers ();
    	
    //default values 
	simSettings settings = {
        30,  // LEFT_BORDER,
        30,  //RIGHT_BORDER,
        15,  // BOTTOM_BORDER,
        30,  //ICONW,
        30,  //ICONH,
        50,  //PERCENT,
        90,  // RANGE,
        8,   // SPACER,
        30,  // BG_HEIGHT,
        5,   // REFLEX_SCALING,
        100, // REFLEX_ALPHA,
        30,  // MAXSIZE
        50,  //BLUR_TIMEOUT
        bgPath, // bgPath,
        true, // SHOW_REFLEXES,
        true, // ENABLE_TASKS,
        true, // ENABLE_MINIMIZE,
        true, //AUTO_POSITION
		true  // FAST_ANIMATIONS
    };
    loadAll (ImagesList, & settings, defaultLaunchers);

    long options = wxNO_BORDER;
    options = options | wxFRAME_TOOL_WINDOW;
    if (!showInTray) {
	  options = options | wxFRAME_NO_TASKBAR;
    }

    if (onTop) {
        options = options | wxSTAY_ON_TOP;
	}
    frame = new MyFrame (NULL, settings,ImagesList,-1, _T ("SimDock"), wxPoint (10, 10),
			 wxSize (450, 150), options);
    wxImage* appBackground = new wxImage (settings.BG_PATH);
    if (! appBackground->IsOk()) {
        delete appBackground;
        wxImage* appBackground = new wxImage (bgPath);
    }
    appBackground->Rescale (frame->GetClientSize ().GetWidth(), settings.BG_HEIGHT, wxIMAGE_QUALITY_HIGH);
	frame->SetBG(appBackground);
	
	wxBitmap* bmp = new wxBitmap(wxImage (markPath));
	frame->SetMarkBitmap(bmp);
    frame->frameOptions = options;

    frame->updateSize();
	
    register_sigint (this);	// CTRL-C Handling
    
    frame->Show (TRUE);
    frame->Freeze();

    GtkWidget* widget = frame->GetHandle();
    XID xid = GDK_WINDOW_XWINDOW(widget->window);
    xstuff_resizeScreen(xid, *frame);
    if (settings.ENABLE_TASKS) {
		tasks_fillList(ImagesList, settings);
		tasks_register_signals(ImagesList, settings);
    }
	frame->appSize = FirstPosition (settings, ImagesList);
	frame->updateSize();
	if (!settings.AUTO_POSITION) {
		frame->Move (startPositionX, startPositionY);
    } else {
    	wxSize sz = wxGetDisplaySize();
    	frame->Move ((sz.GetWidth() - frame->GetClientSize().GetWidth()) / 2, 
    	sz.GetHeight() - frame->GetClientSize().GetHeight());
    }
    //getting the background from root will only work at a late point
    //like this
	wxBitmap * backImage;
    if (!customBackground.IsEmpty ()) {
        backImage = fixImage (customBackground, customBackgroundType,
                        customBackgroundColour);
    } else {
        backImage = getRootWallpaper();
    }
    frame->SetWallpaper(backImage);
	frame->Thaw();
    xstuff_setDefaultWindowFlags(xid);
    
    return TRUE;
}




