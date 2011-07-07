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


static gboolean
gconf_key_exist(GConfClient *gconf_client, const gchar *key)
{
    GConfValue *value;
    gboolean result = FALSE;
    
    value = gconf_client_get(gconf_client, key, NULL);
    if (value != NULL) {
        gconf_value_free(value);
        result = TRUE;
    }
    return result;
}


bool
SimGconf::init ()
{
  /*
   * TODO: Add argc and argv to gconf init ?
   */
  g_type_init ();
  GError *err = NULL;

  if (!gconf_init (0, NULL, &err))
    {
      printf ("Failed to init GConf: %s", err->message);
      g_error_free (err);
      err = NULL;

      return false;
    }
  //initialized = true;
  return true;
}

GConfClient *
SimGconf::GetClient ()
{
  return gconf_client_get_default ();
}

bool
SimGconf::CreateDirectory (GConfClient * client, const char *key)
{
  if (!gconf_client_dir_exists (client, key, NULL))
    {
      gconf_client_add_dir (client, key, GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
    }
  return true;
}

bool
SimGconf::GetString (GConfClient * client, const char *key, wxString * result)
{
  gchar *str;
  str = gconf_client_get_string (client, key, NULL);
  if (str == NULL)
    return false;
  *result = wxString::FromAscii (str);
  g_free (str);
  return true;
}

bool
SimGconf::SetString (GConfClient * client, const char *key, wxString * value)
{
  const gchar *data = wx2std (*value).c_str ();
  bool result = gconf_client_set_string (client, key, data, NULL);
  return result;

}

bool
SimGconf::GetInt (GConfClient * client, const char *key, int *result)
{
  if (!gconf_key_exist(client, key))
	return false;
	
  GError *err = NULL;
  int v = gconf_client_get_int (client, key, &err);
  
  if (err)
    {
      printf ("Failed to get value: %s", err->message);
      g_error_free (err);
      return false;
    }
#ifdef SIMDOCK_DEBUG
  printf ("GCONF: %s=%d\n", key, v);
#endif
  *result = v;
  return true;
}

bool
SimGconf::SetInt (GConfClient * client, const char *key, int *value)
{
#ifdef SIMDOCK_DEBUG
  printf ("setting %s\n", key);
#endif
  return gconf_client_set_int (client, key, *value, NULL);
}

bool
SimGconf::GetColour (GConfClient * client, const char *key, wxColour * result)
{
  gchar *str;
  str = gconf_client_get_string (client, key, NULL);
  if (str == NULL)
    return false;

  result->Set (wxString::FromAscii (str));
  g_free (str);
  return true;

}

bool
SimGconf::SetColour (GConfClient * client, const char *key, wxColour * value)
{
  wxString val = value->GetAsString (wxC2S_HTML_SYNTAX);
  return SimGconf::SetString (client, key, &val);

}

bool
SimGconf::GetBool (GConfClient * client, const char *key, bool * result)
{
  int res;
  if (SimGconf::GetInt (client, key, &res))
    {
      if (res)
	*result = true;
      else
	*result = false;
      return true;

    }
  return false;
}

bool
SimGconf::SetBool (GConfClient * client, const char *key, bool * value)
{
  int v = (int) *value;
  return SimGconf::SetInt (client, key, &v);

}
