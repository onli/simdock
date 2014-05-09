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


wxXmlNode* createNode(const wxString& name, int* content)
{
	wxString x_s = wxString::Format(_T("%d"), *content);
	wxXmlNode* x_node_content = new wxXmlNode(wxXML_TEXT_NODE,name,x_s);
	wxXmlNode* x_node = new wxXmlNode(wxXML_ELEMENT_NODE,name);
	x_node->AddChild(x_node_content);
	return x_node;
}

wxXmlNode* createNode(const wxString& name, const wxString& text)
{
	wxXmlNode* x_node_content = new wxXmlNode(wxXML_TEXT_NODE,name, text);
	wxXmlNode* x_node = new wxXmlNode(wxXML_ELEMENT_NODE,name);
	x_node->AddChild(x_node_content);
	return x_node;
}

bool saveLaunchers(ImagesArray* list)
{
	
	wxXmlNode* Program = new wxXmlNode(wxXML_ELEMENT_NODE,_T(XML_PROGRAM));
	
	for (unsigned int i = 0; i < list->GetCount (); i++)
	{
        simImage *img = (*list)[i];
        if (img->task)
        {
        	continue;
        }
        
        wxXmlNode* SimDock = new wxXmlNode(wxXML_ELEMENT_NODE,_T(XML_SIMDOCK));
        SimDock->AddChild(createNode(_T(XML_PATH),img->link));
		SimDock->AddChild(createNode(_T(XML_ICON),img->img_link));
		SimDock->AddChild(createNode(_T(XML_DESCR),img->descr));
		SimDock->AddChild(createNode(_T(XML_NAME),img->name));

        Program->AddChild(SimDock);
    }
        
	wxXmlDocument doc;
	doc.SetRoot(Program);
    return doc.Save(PROGRAMS_PATH);

}
