/* Copyright 2011 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#ifndef searender_rules_h
#define searender_rules_h

#ifndef system_h
#define system_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#endif

#include "map.h"
#include "render.h"
#include "s57obj.h"
#include "s57att.h"
#include "s57val.h"

#define rules void main_rules(Item_t *item, Obj_t *obj)
#define object_rules(x, ...) void x(Item_t *item, Obj_t *obj, ## __VA_ARGS__)

#define type(x) for (item = findItem(x); item != NULL; item = findNext())
#define object(x, ...) x(item, obj == NULL ? getObj(item, item->objs.obj, 0) : obj, ## __VA_ARGS__)
#define use_object(x) { Obj_t *obj=getObj(item, enumType(x), 0);
#define use_object_select(x,n) { Obj_t *obj=getObj(item, enumType(x), n);
#define used }
#define object_attribute(x) stringValue((getTag(obj, enumAttribute(x, obj->obj))->val))
#define is_type(x) (compareTypes(item, x))
#define has_object(x) (compareObjects(item, x))
#define object_count(x) (countObjects(item, x))
#define this_type stringType(item->objs.obj)

#define has_attribute(x) (compareAttributes(obj, x))
#define has_type_attribute(x) (compareAttributes(&item->objs, x))
#define attribute(x) (stringValue(getTag(obj, enumAttribute(x, obj->obj))->val))
#define type_attribute(x) (stringValue(getTag(&item->objs, enumAttribute(x, obj->obj))->val))
#define literal_switch(x) { char *tag=x; if(tag == NULL);
#define literal_case(x) else if (compareLiterals(tag, x))
#define attribute_switch(x) { Tag_t *tag = getTag(obj, enumAttribute(x, obj->obj)); if(tag == NULL);
#define attribute_case(x) else if (compareValues(tag, x))
#define attribute_default else
#define attribute_test(x,v) (has_attribute(x) && compareValues(getTag(obj, enumAttribute(x, obj->obj)), v))
#define end_switch }
#define attribute_count(x) countValues(getTag(obj, enumAttribute(x, obj->obj)))
#define attribute_sequence(x) {char *val = strdup(stringValue(getTag(obj, enumAttribute(x, obj->obj))->val)); char *ptr = NULL;
#define attribute_next (ptr = strtok(ptr == NULL ? val : NULL, ";"))
#define end_sequence free(val);}
#define map(x) const Smap_t x[] = {

#define colour_symbol(s,p,c) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, CC, 0, 0, 0)
#define colour_symbol_place(s,p,c,h) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, h, 0, 0, 0)
#define colour_symbol_position(s,p,c,h,x,y) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, h, x, y, 0)
#define colour_symbol_orientation(s,p,c,h,x,y,r) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, h, x, y, r)
#define symbol(s) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "#000000", CC, 0, 0, 0)
#define symbol_place(s,h) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "#000000", h, 0, 0, 0)
#define symbol_position(s,h,x,y) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "#000000", h, x, y, 0)
#define symbol_orientation(s,h,x,y,r) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "#000000", h, x, y, r)
#define symbol_cluster(x) renderCluster(item, x)
#define symbol_notice renderNotice(item)
#define light_sector_caption(n,t,s,o,y) renderSector(item, n, t, s, o, y)
#define light_sector(n) renderSector(item, n, NULL, NULL, 0, 0)

#define make_string(s) { char *string=strdup(s)
#define make_char_string(x) { char *string=strdup(charString(item, x, 0))
#define make_char_string_indexed(x, n) { char *string=strdup(charString(item, x, n))
#define add_string(s) string = realloc(string, strlen(string) + strlen(s) + 1); strcat(string, s);
#define free_string free(string); }

#define line(s) drawLine(item, s)
#define line_text(t,s,o,y,r) drawLineText(item, t, s, o, y, r)
#define area(s) drawArea(item, s)
#define area_text(t,s) drawAreaText(item, t, s)
#define node_text(t,s,x,y) drawNodeText(item, t, s, x, y)

extern void main_rules(Item_t*, Obj_t*);

#endif

/*
 C Render rules macros
 =====================
 
 Blocks
 ------
 rules { <statements> } The main rules block.
 object_rules(<identifier>[, <arguments>]) { <statements> } A rules block called from any other block.
 object(<identifier>[, <arguments>]) Calls an object_rules block, passing the current object.
 
 Searching
 ---------
 type(<key>) { <statements> } Scans all items that have a matching <key> as master object.
 
 Tests
 -----
 is_type(<types>) True if master object matches one of <types>
 has_object(<objects>) True if current item contains one of <objects>
 has_attribute(<attributes>) True if current object has tag matching one of <attributes>
 
 Selections
 ----------
 use_object(<object>) Start block using this <object>
 used Terminate block
 
 Value Selectors
 ---------------
 list(<key>) Initialises scan of semicolon-separated values in value field. Returns number of values.
 list_next Returns pointer to next value or NULL
 tokens(<value>) Initialises scan of colon-separated values in value field. Returns number of values.
 token_next Returns pointer to next value or NULL
 
 Rendering Instructions
 ----------------------
 symbol(<symbol>) Places symbol centred at either current node or current area
 symbol_place(<symbol>, <handle>) Places symbol with <handle> positioned at current node/area
 symbol_position(<symbol>, <handle>, <x>, <y>) Places symbol with <handle> offset <x>,<y> from current node/area
 path(<style>) Draw path along current way.
 path_offset(<style>, <dy>) Draw a path along current way offset by <dy> from axis.
 area(<style>) Draw path around current closed way.
 area_offset(<style>, <dy>) Draw a path around current closed way offset by <dy> from axis.
 
 */
