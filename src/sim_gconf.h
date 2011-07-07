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
#ifndef SIM_GCONF_H_
#define SIM_GCONF_H_

#include "main.h"

class SimGconf
{
public:
	static bool init();
	static bool CreateDirectory(GConfClient * client,const char* key);
	
	static bool SetString(GConfClient * client,const char* key, wxString* value);
	static bool SetInt(GConfClient * client, const char* key, int* value);
	static bool SetColour(GConfClient * client, const char* key, wxColour* value);
	static bool SetBool(GConfClient * client, const char* key, bool* value);
	
	static bool GetString(GConfClient * client,const char* key, wxString* result);
	static bool GetInt(GConfClient * client, const char* key, int* result);
	static bool GetColour(GConfClient * client, const char* key, wxColour* result);
	static bool GetBool(GConfClient * client, const char* key, bool* result);
	
	static GConfClient* GetClient(); // must call g_object_unref(client) after use!
protected:
	static bool initialized;
};


#endif
