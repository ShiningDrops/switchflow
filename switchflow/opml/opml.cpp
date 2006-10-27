//
// Copyright 2003-2006 Christopher Baus. http://baus.net/
// Read the LICENSE file for more information.

/**
 * @file opml.c generic OPML 1.0 support
 * 
 * Copyright (C) 2003, 2004 Lars Lindner <lars.lindner@gmx.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "opml.h"

/**
#include "support.h"
#include "conf.h"
#include "common.h"
#include "feed.h"
#include "item.h"
#include "opml.h"
#include "callbacks.h"
#include <string>
*/

/* you can find the OPML specification at Userland:

   http://www.opml.org/
   
 */
 
/* this is a generic subtag list for directory, channel and format description tags */
#define OPML_TITLE    0
#define OPML_CREATED    1
#define OPML_MODIFIED   2
#define OPML_OWNERNAME    3
#define OPML_OWNEREMAIL   4
#define OPML_MAX_TAG    5

/* note: the tag order has to correspond with the OCS_* defines in the header file */
static char *opmlTagList[] = {  "title",
        "dateCreated",
        "dateModified",
        "ownerName",
        "ownerEmail",
        NULL
            };


/* ---------------------------------------------------------------------------- */
/* OPML parsing and HTML output             */
/* ---------------------------------------------------------------------------- */
            
/* retruns a HTML string containing the text and attributes of the outline */

static void getOutlineContents(xmlNodePtr cur,
                               i_opml_handler* p_opml_handler)
{
  //  char    *buffer = NULL;
  xmlChar* value;
  char    *tmp, *tmp2;
  xmlAttrPtr  attr;

  attr = cur->properties;
  while(NULL != attr) {
    /* get prop value */
    value = xmlGetProp(cur, attr->name);
    if(NULL != value) {
      if(!xmlStrcmp(attr->name, BAD_CAST"text")) {    

      } else if(!xmlStrcmp(attr->name, BAD_CAST"isComment")) {
        /* don't output anything */

      } else if(!xmlStrcmp(attr->name, BAD_CAST"type")) {
        /* don't output anything */

      } else if(!xmlStrcmp(attr->name, BAD_CAST"url")) {
        
      } else if(!xmlStrcmp(attr->name, BAD_CAST"htmlUrl") ||
                !xmlStrcmp(attr->name, BAD_CAST"htmlurl")) {
                
      } else if(!xmlStrcmp(attr->name, BAD_CAST"xmlUrl") ||
                !xmlStrcmp(attr->name, BAD_CAST"xmlurl")) {
        p_opml_handler->feed_url((char*)value);
      } else {
      }

      free(value);
    }
    attr = attr->next;
  }
  /* check for <outline> subtags */

  if(NULL != cur->xmlChildrenNode) {
    cur = cur->xmlChildrenNode;
    while(NULL != cur) {
      if(!xmlStrcmp(cur->name, BAD_CAST"outline")) {
        getOutlineContents(cur, p_opml_handler);
        //addToHTMLBufferFast(&buffer, tmp);
        //g_free(tmp);
        //g_free(tmp2);
      }
      cur = cur->next;
    }
  }
  return;
}

void opml_parse(xmlDocPtr doc,
                xmlNodePtr cur,
                i_opml_handler* p_opml_handler)
{
  xmlNodePtr  child;
  xmlChar* tmp;
  char    *buffer, *line;
  char    *headTags[OPML_MAX_TAG];
  int     i, error = 0;

  do {

    if(!xmlStrcmp(cur->name, BAD_CAST"opml") ||
       !xmlStrcmp(cur->name, BAD_CAST"oml") ||
       !xmlStrcmp(cur->name, BAD_CAST"outlineDocument")) {
        /* nothing */
    } else {
      printf("Could not find OPML header!\n");
      xmlFreeDoc(doc);
      error = 1;
      break;      
    }

    cur = cur->xmlChildrenNode;
    while (cur && xmlIsBlankNode(cur)) {
      cur = cur->next;
    }

    memset(headTags, 0, sizeof(char *)*OPML_MAX_TAG);   
    while (cur != NULL) {
      if(!xmlStrcmp(cur->name, BAD_CAST"head")) {
        /* check for <head> tags */
        child = cur->xmlChildrenNode;
        while(child != NULL) {
          for(i = 0; i < OPML_MAX_TAG; i++) {
            if(!xmlStrcmp(child->name, (const xmlChar *)opmlTagList[i])) {
              tmp = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);           
              if(NULL != tmp) {
                free(headTags[i]);
                headTags[i] = (char*)tmp;
              }
            }   
          }
          child = child->next;
        }
      }
      
      if(!xmlStrcmp(cur->name, BAD_CAST"body")) {
        /* process all <outline> tags */
        child = cur->xmlChildrenNode;
        while(child != NULL) {
          if(!xmlStrcmp(child->name, BAD_CAST"outline")) {
            getOutlineContents(child, p_opml_handler);

            tmp = xmlGetProp(child, BAD_CAST"text");
            if(NULL == tmp)
              tmp = xmlGetProp(child, BAD_CAST"title");
            free(tmp);
          }
          child = child->next;
        }
      }
      
      cur = cur->next;
    }
#ifdef ZED
    /* after parsing we fill in the infos into the feedPtr structure */   
    feed_add_items(fp, items);
    feed_set_update_interval(fp, -1);
    if(NULL == (fp->title = headTags[OPML_TITLE]))
      fp->title = g_strdup(fp->source);
    
    if(0 == error) {
      /* prepare HTML output */
      buffer = NULL;
      addToHTMLBuffer(&buffer, HEAD_START); 
      
      line = g_strdup_printf(HEAD_LINE, _("Feed:"), fp->title);
      addToHTMLBuffer(&buffer, line);
      g_free(line);

      if(NULL != fp->source) {
        tmp = g_strdup_printf("<a href=\"%s\">%s</a>", fp->source, fp->source);
        line = g_strdup_printf(HEAD_LINE, _("Source:"), tmp);
        g_free(tmp);
        addToHTMLBuffer(&buffer, line);
        g_free(line);
      }

      addToHTMLBuffer(&buffer, HEAD_END); 

      addToHTMLBuffer(&buffer, FEED_FOOT_TABLE_START);
      FEED_FOOT_WRITE(buffer, "title",    headTags[OPML_TITLE]);
      FEED_FOOT_WRITE(buffer, "creation date",  headTags[OPML_CREATED]);
      FEED_FOOT_WRITE(buffer, "last modified",  headTags[OPML_MODIFIED]);
      FEED_FOOT_WRITE(buffer, "owner name",   headTags[OPML_OWNERNAME]);
      FEED_FOOT_WRITE(buffer, "owner email",    headTags[OPML_OWNEREMAIL]);
      addToHTMLBuffer(&buffer, FEED_FOOT_TABLE_END);
      
      feed_set_description(fp, buffer);
      g_free(buffer);
      
      feed_set_available(fp, TRUE);
    } else {
      ui_mainwindow_set_status_bar(_("There were errors while parsing this feed!"));
    }
#endif    
    break;
  } while (0);

}

#ifdef ZED
static gboolean opml_format_check(xmlDocPtr doc, xmlNodePtr cur) {
  if(!xmlStrcmp(cur->name, BAD_CAST"opml") ||
     !xmlStrcmp(cur->name, BAD_CAST"oml") || 
     !xmlStrcmp(cur->name, BAD_CAST"outlineDocument")) {
    
    return TRUE;
  }
  return FALSE;
}
/* ---------------------------------------------------------------------------- */
/* initialization               */
/* ---------------------------------------------------------------------------- */

feedHandlerPtr opml_init_feed_handler(void) {
  feedHandlerPtr  fhp;
  
  fhp = g_new0(struct feedHandler, 1);
  
  /* prepare feed handler structure */
  fhp->typeStr = "opml";
  fhp->icon = ICON_OCS;
  fhp->directory = FALSE;
  fhp->feedParser = opml_parse;
  fhp->checkFormat = opml_format_check;
  fhp->merge    = FALSE;
  
  return fhp;
}

#endif

