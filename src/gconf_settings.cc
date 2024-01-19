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

#include "gconf_settings.h"

using namespace std;



void simGconf_loadSettings (simSettings* data)
{

    wxConfigBase* config = wxConfig::Get();
    
    config->Read(GCONF_LEFT_BORDER, &data->LEFT_BORDER);
    config->Read(GCONF_RIGHT_BORDER, &data->RIGHT_BORDER);
    config->Read(GCONF_BOTTOM_BORDER, &data->BOTTOM_BORDER);
    config->Read(GCONF_ICON_WIDTH, &data->ICONW);
    config->Read(GCONF_ICON_HEIGHT, &data->ICONH);
    config->Read(GCONF_ZOOM, &data->PERCENT);
    config->Read(GCONF_RANGE, &data->RANGE);
    config->Read(GCONF_BG_HEIGHT, &data->BG_HEIGHT);
    config->Read(GCONF_SPACER, &data->SPACER);
    config->Read(GCONF_BLUR_TIMEOUT, &data->BLUR_TIMEOUT);
    config->Read(GCONF_BG_URL, &data->BG_PATH);
    
    config->Read(GCONF_SHOW_REFLEXES, &data->SHOW_REFLEXES);
    config->Read(GCONF_REFLEX_SCALING, &data->REFLEX_SCALING);
    config->Read(GCONF_REFLEX_ALPHA, &data->REFLEX_ALPHA);
    config->Read(GCONF_AUTO_POSITION, &data->AUTO_POSITION);
    config->Read(GCONF_ENABLE_TASKS, &data->ENABLE_TASKS);

    config->Read(GCONF_ENABLE_MINIMIZE, &data->ENABLE_MINIMIZE);
    config->Read(GCONF_FAST_ANIMATIONS, &data->FAST_ANIMATIONS);
}

void simGconf_loadPosition (int* x, int* y)
{
    wxConfigBase* config = wxConfig::Get();

	config->Read(GCONF_SETTINGS_X, x);
    config->Read(GCONF_SETTINGS_Y, y);
}

void
simGconf_saveSettings (struct simSettings* data)
{
    wxConfigBase* config = wxConfig::Get();

    config->Write(GCONF_LEFT_BORDER, data->LEFT_BORDER);
    config->Write(GCONF_RIGHT_BORDER, data->RIGHT_BORDER);
    config->Write(GCONF_BOTTOM_BORDER, data->BOTTOM_BORDER);
    config->Write(GCONF_ICON_WIDTH, data->ICONW);
    config->Write(GCONF_ICON_HEIGHT, data->ICONH);
    config->Write(GCONF_ZOOM, data->PERCENT);
    config->Write(GCONF_RANGE, data->RANGE);
    config->Write(GCONF_BG_HEIGHT, data->BG_HEIGHT);
    config->Write(GCONF_SPACER, data->SPACER);
    config->Write(GCONF_BLUR_TIMEOUT, data->BLUR_TIMEOUT);
    config->Write(GCONF_BG_URL, data->BG_PATH);
    config->Write(GCONF_SHOW_REFLEXES, data->SHOW_REFLEXES);
    config->Write(GCONF_REFLEX_SCALING, data->REFLEX_SCALING);
    config->Write(GCONF_REFLEX_ALPHA, data->REFLEX_ALPHA);
    config->Write(GCONF_AUTO_POSITION, data->AUTO_POSITION);
    config->Write(GCONF_ENABLE_TASKS, data->ENABLE_TASKS);
    
    config->Write(GCONF_ENABLE_MINIMIZE, data->ENABLE_MINIMIZE);
    config->Write(GCONF_FAST_ANIMATIONS, data->FAST_ANIMATIONS);

    config->Flush();

}

void simGconf_savePosition(int x, int y)
{
	wxConfigBase* config = wxConfig::Get();
	
	config->Write(APPLICATION_PATH GCONF_SETTINGS_X, x);
    config->Write(GCONF_SETTINGS_Y, y);
	
	config->Flush();
}
