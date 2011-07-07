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

#ifndef GCONF_SETTINGS_H_
#define GCONF_SETTINGS_H

//#include "main.h"

// Gconf does not accept slash-ended strings as keys. Just in case
#define APPLICATION_PATH "/apps/simdock/" 
#define GCONF_SETTINGS_X "x"
#define GCONF_SETTINGS_Y "y"
#define GCONF_LEFT_BORDER "left_border"
#define GCONF_RIGHT_BORDER "right_border"
#define GCONF_BOTTOM_BORDER "bottom_border"
#define GCONF_ICON_WIDTH "icon_width"
#define GCONF_ICON_HEIGHT "icon_height"
#define GCONF_ZOOM "zoom"
#define GCONF_RANGE "range"
#define GCONF_SPACER "spacer"
#define GCONF_SHOW_REFLEXES "show_reflexes"
#define GCONF_REFLEX_SCALING "reflexes_scaling"
#define GCONF_REFLEX_ALPHA "reflexes_alpha"
#define GCONF_AUTO_POSITION "auto_position"
#define GCONF_ENABLE_TASKS "enable_tasks"


#define GCONF_BG_HEIGHT "bg_height"
#define GCONF_BG_URL "bg_url"
#define GCONF_WALLPAPER_URL "wallpaper_url"

/* Loads simdock settings from libgconf2 */
void simGconf_loadSettings (simSettings* data);

/* Loads last simdock position using libgconf2 */
void simGconf_loadPosition (int* x, int* y);


/* 
 * Saves given simSettings to the gnome configuration,
 * using libgconf-2
 */
void simGconf_saveSettings (simSettings* data);

/* 
Saves given simdock position into Gconf
*/
void simGconf_savePosition(int x, int y);



#endif
