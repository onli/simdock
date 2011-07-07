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
#ifndef MAIN_H_
#define MAIN_H_
#include <iostream>		//cout
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>

#include <glib.h>
#include <gconf/gconf-client.h>



#include "wx/wx.h"
#include "wx/utils.h"
#include "wx/xml/xml.h"
#include "wx/event.h"
#include "wx/filesys.h"
#include "wx/encconv.h"
#include "wx/dialog.h"
#include "wx/cmdline.h"
#include "wx/graphics.h"
#include "wx/dynarray.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/aboutdlg.h"
#include "wx/generic/aboutdlgg.h"
#include "wx/propdlg.h"
#include "wx/generic/propdlg.h"
#include "wx/dcbuffer.h"
#include "wx/tooltip.h" 

/* For dialogs stuff */
#include "wx/checkbox.h"
#include "wx/imaglist.h"
#include "wx/artprov.h"
#include "wx/spinctrl.h"
#include "wx/toolbook.h"
#include "wx/stattext.h"
#include "wx/filedlg.h"



class simImage;
class simSettings;
class MyApp;
WX_DEFINE_ARRAY (simImage *, ImagesArray);

#define SIMDOCK_VERSION "1.2.1"
#define SIMDOCK_WEBSITE "http://sourceforge.net/projects/simdock"
//#define SIMDOCK_DEBUG
//#define SIMDOCK_DEBUG_ICON_POSITIONING //this is very verbose
#include "simImage.h"
#include "settingsDialog.h"

#include "myFrame.h"
//#include "getBg.h"
#include "gconf_settings.h"
#include "background.h"

//windows list
#include "tasks.h" 



/* Loading wxStrings from chars */
#ifdef wxUSE_UNICODE
#define _U(x) wxString((x),wxConvUTF8)
#define _UU(x,y) wxString((x),y)
#else
#define _U(x) (x)
#define _UU(x,y) (x)
#endif


#define CONF_DIR ".SimDock"
#define CONF_FILE "launchers.xml"

/* XML nodes */
#define XML_SIMDOCK "SimDock"
#define XML_PROGRAM "Program"
#define XML_PATH "path"
#define XML_ICON "icon"
#define XML_DESCR "description"
#define XML_NAME "name"

#define TIMER_TIMEOUT_BLUR 100

#define STRETCHED 1
#define CENTERED 2
#define SCALED 3
#define NONE 4
#define WALLPAPER 5

#define STRETCHED_STR "stretched"
#define CENTERED_STR "centered"
#define SCALED_STR "scaled"
#define NONE_STR  "none"
#define WALLPAPER_STR "wallpaper"

#define DATA_DIR "/usr/share/simdock"

//#include <wx/arrimpl.cpp>
//WX_DEFINE_OBJARRAY(simImage)
//WX_DEFINE_OBJARRAY(ImagesList);


//WX_DECLARE_LIST(simImage, ImagesList);




/* wxString to std::string converter. Useful for printing stuff */
std::string wx2std (const wxString & input);

/* Creates the given directory only if it does not exist */
bool fixSimDir (wxString * dirPath);
/* Opens the given filename or initialize it with given data */
bool openOrInitialize (wxString * fullPath, char *initializeData);
/* Opens the given filename or creates it if does not exist */
bool openOrCreate (wxFile * settingsFile, wxString * fullPath);
/* Saves the given images array to the given filesystem location*/
bool saveLaunchers(ImagesArray* list, const wxString& location);

/* Register the sigint callback to call MyApp::GracefullyExit ()*/
void register_sigint (MyApp * application);

/* ------ Various functions for simImage array manipulation -------*/

/* Reposition icons according to their size and order
 * Currently repositioning starts from index 0 so all the icons are moved to the right
 * "ind" is currently ignored.
 * TODO: Implement right side and left side repositioning starting from "ind" index?
 */
wxSize PositionIcons (wxSize sz, int ind, simSettings* settings, ImagesArray* ImagesList);
wxSize FirstPosition (wxSize sz, simSettings* settings, ImagesArray* list);

/* ------ Various functions for simImage array manipulation -------*/

class simSettings
{
public:
simSettings (int LEFT_BORDER, int RIGHT_BORDER,int BOTTOM_BORDER,int ICONW,int ICONH,
	int PERCENT,int RANGE,
	int SPACER,int BG_HEIGHT,wxString bgPath,bool SHOW_REFLEXES,int
	REFLEX_SCALING,int REFLEX_ALPHA, bool AUTO_POSITION, bool ENABLE_TASKS
    )
    {
    	this->LEFT_BORDER = LEFT_BORDER;
    	this->RIGHT_BORDER = RIGHT_BORDER;
    	this->BOTTOM_BORDER = BOTTOM_BORDER;
    	this->ICONW = ICONW;
    	this->ICONH = ICONH;
    	this->PERCENT = PERCENT;
    	this->RANGE = RANGE;
    	this->SPACER = SPACER;
    	this->BG_HEIGHT = BG_HEIGHT;
    	this->BG_PATH = bgPath;
    	this->SHOW_REFLEXES = SHOW_REFLEXES;
    	this->REFLEX_SCALING = REFLEX_SCALING;
    	this->REFLEX_ALPHA = REFLEX_ALPHA;
    	this->MAXSIZE = ICONW + ICONW * PERCENT / 100;
    	this->AUTO_POSITION = AUTO_POSITION;
    	this->ENABLE_TASKS = ENABLE_TASKS;

    }
    int LEFT_BORDER;
    int RIGHT_BORDER;
    int BOTTOM_BORDER;
    int ICONW;
    int ICONH;
    int PERCENT;
    int RANGE;
    int SPACER;
    int BG_HEIGHT;
    wxString BG_PATH;
    bool SHOW_REFLEXES;
    bool ENABLE_TASKS;
    bool AUTO_POSITION;
    int REFLEX_SCALING;
    int REFLEX_ALPHA;
    int MAXSIZE;
};


class MyApp:public wxApp
{
public:
  virtual int OnExit ();
  void GracefullyExit ();
  virtual bool OnInit ();
  virtual void OnInitCmdLine (wxCmdLineParser & parser);
  virtual bool OnCmdLineParsed (wxCmdLineParser & parser);
  void reposition() { if (frame) frame->appSize = FirstPosition (frame->GetClientSize (),settings,ImagesList); }
  void updateSize() { if (frame) frame->updateSize();}
  void refresh() { if (frame) frame->Refresh(false);}
  /* If launchers list have been modified from the starting */
  bool launchersModified;

  bool onTop;
  void SetWallpaper (wxBitmap * newBitmap) {if (frame) frame->SetWallpaper(newBitmap); }
  wxBitmap * GetWallpaper (){if (frame) frame->GetWallpaper(); }
private:

  simSettings* settings;
  /* -----Custom background stuff-----  */
  wxString customBackground;
  int customBackgroundType;
  wxColour customBackgroundColour;
  /* -----Custom background stuff-----  */
  MyFrame *frame;
  bool showInTray;
  ImagesArray* ImagesList;

  SettingsDialog* sd;
};



/* http://www.wxwidgets.org/wiki/index.php/Command-Line_Arguments */

static const wxCmdLineEntryDesc g_cmdLineDesc[] = {
  {wxCMD_LINE_SWITCH, wxT ("h"), wxT ("help"),
   wxT ("displays help on the command line parameters"),
   wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
  {wxCMD_LINE_OPTION, wxT ("b"), wxT ("background"),
   wxT ("use custom wallpaper"),
   wxCMD_LINE_VAL_STRING},
  {wxCMD_LINE_OPTION, wxT ("t"), wxT ("type"),
   _("wallpaper positioning: <" CENTERED_STR "> <" STRETCHED_STR "> <"
     NONE_STR ">"), wxCMD_LINE_VAL_STRING},
  {wxCMD_LINE_OPTION, wxT ("o"), wxT ("color"),
   wxT
   ("background color. Hex or name values accepted. examples: -o:#FF0000 , -o:red"),
   wxCMD_LINE_VAL_STRING},
  {wxCMD_LINE_SWITCH, wxT ("r"), wxT ("tray"), wxT ("show in the trayBar"),
   wxCMD_LINE_VAL_NONE},
  {wxCMD_LINE_SWITCH, wxT ("y"), wxT ("on top"), wxT ("show on-top of other windows"),
   wxCMD_LINE_VAL_NONE},

  {wxCMD_LINE_NONE}
};

DECLARE_APP (MyApp)
     


#endif
