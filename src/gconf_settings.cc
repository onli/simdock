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

#include "sim_gconf.h"
#include "gconf_settings.h"

using namespace std;



void simGconf_loadSettings (struct simSettings* data)
{

    GConfClient *client = SimGconf::GetClient ();
    
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_LEFT_BORDER, &data->LEFT_BORDER);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_RIGHT_BORDER, &data->RIGHT_BORDER);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_BOTTOM_BORDER, &data->BOTTOM_BORDER);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_ICON_WIDTH, &data->ICONW);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_ICON_HEIGHT, &data->ICONH);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_ZOOM, &data->PERCENT);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_RANGE, &data->RANGE);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_BG_HEIGHT, &data->BG_HEIGHT);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_SPACER, &data->SPACER);
    SimGconf::GetString(client, APPLICATION_PATH GCONF_BG_URL, &data->BG_PATH);
    
    SimGconf::GetBool(client, APPLICATION_PATH GCONF_SHOW_REFLEXES, &data->SHOW_REFLEXES);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_REFLEX_SCALING, &data->REFLEX_SCALING);
    SimGconf::GetInt(client, APPLICATION_PATH GCONF_REFLEX_ALPHA, &data->REFLEX_ALPHA);
    SimGconf::GetBool(client, APPLICATION_PATH GCONF_AUTO_POSITION, &data->AUTO_POSITION);
    SimGconf::GetBool(client, APPLICATION_PATH GCONF_ENABLE_TASKS, &data->ENABLE_TASKS);

    
      g_object_unref (client);
}

void simGconf_loadPosition (int* x, int* y)
{
        GConfClient *client = SimGconf::GetClient ();

	SimGconf::GetInt(client, APPLICATION_PATH GCONF_SETTINGS_X, x);
        SimGconf::GetInt(client, APPLICATION_PATH GCONF_SETTINGS_Y, y);

  	g_object_unref (client);
}

void
simGconf_saveSettings (struct simSettings* data)
{
    GConfClient *client = SimGconf::GetClient ();

    SimGconf::SetInt(client, APPLICATION_PATH GCONF_LEFT_BORDER, &data->LEFT_BORDER);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_RIGHT_BORDER, &data->RIGHT_BORDER);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_BOTTOM_BORDER, &data->BOTTOM_BORDER);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_ICON_WIDTH, &data->ICONW);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_ICON_HEIGHT, &data->ICONH);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_ZOOM, &data->PERCENT);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_RANGE, &data->RANGE);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_BG_HEIGHT, &data->BG_HEIGHT);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_SPACER, &data->SPACER);
    SimGconf::SetString(client, APPLICATION_PATH GCONF_BG_URL, &data->BG_PATH);
    SimGconf::SetBool(client, APPLICATION_PATH GCONF_SHOW_REFLEXES, &data->SHOW_REFLEXES);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_REFLEX_SCALING, &data->REFLEX_SCALING);
    SimGconf::SetInt(client, APPLICATION_PATH GCONF_REFLEX_ALPHA, &data->REFLEX_ALPHA);
    SimGconf::SetBool(client, APPLICATION_PATH GCONF_AUTO_POSITION, &data->AUTO_POSITION);
    SimGconf::SetBool(client, APPLICATION_PATH GCONF_ENABLE_TASKS, &data->ENABLE_TASKS);

    g_object_unref (client);

}

void simGconf_savePosition(int x, int y)
{
	GConfClient *client = SimGconf::GetClient ();
	
	SimGconf::SetInt(client, APPLICATION_PATH GCONF_SETTINGS_X, &x);
        SimGconf::SetInt(client, APPLICATION_PATH GCONF_SETTINGS_Y, &y);
	
	g_object_unref (client);
}
