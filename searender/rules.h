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
#define is_type(x) (compareTypes(item, x))
#define has_object(x) (compareObjects(item, x))
#define object_count(x) (countObjects(item, x))
#define this_type stringType(item->objs.obj)
#define feature (testFeature(item))
#define is_node (testNode(item))
#define is_line (testLine(item))
#define is_area (testArea(item))
#define nodes (countNodes(item))
#define extent (calcArea(item))

#define has_attribute(x) (compareAttributes(obj, x))
#define has_item_attribute(x) (compareAttributes(&item->objs, x))
#define attribute(x) (stringValue(getAtt(obj, enumAttribute(x, obj->obj))->val))
#define item_attribute(x) (stringValue(getAtt(&item->objs, enumAttribute(x, item->objs.obj))->val))
#define literal_switch(x) { char *tag=x; if(tag == NULL);
#define literal_case(x) else if (compareLiterals(tag, x))
#define attribute_switch(x) { Att_t *att = getAtt(obj, enumAttribute(x, obj->obj)); if(att == NULL);
#define attribute_case(x) else if (compareValues(att, x))
#define attribute_default else
#define attribute_test(x,v) (has_attribute(x) && compareValues(getAtt(obj, enumAttribute(x, obj->obj)), v))
#define end_switch }
#define attribute_count(x) countValues(getAtt(obj, enumAttribute(x, obj->obj)))
#define attribute_sequence(x) {char *val = strdup(stringValue(getAtt(obj, enumAttribute(x, obj->obj))->val)); char *ptr = NULL;
#define attribute_next (ptr = strtok(ptr == NULL ? val : NULL, ";"))
#define end_sequence free(val);}

#define colour_symbol(s,p,c) renderColourSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, CC, 0, 0, 0)
#define colour_symbol_place(s,p,c,h) renderColourSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, h, 0, 0, 0)
#define colour_symbol_position(s,p,c,h,x,y) renderColourSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, h, x, y, 0)
#define colour_symbol_orientation(s,p,c,h,x,y,r) renderColourSymbol(item, (obj!=NULL)?obj->obj:0, s, p, c, h, x, y, r)
#define symbol(s) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "", CC, 0, 0, 0)
#define symbol_place(s,h) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "", h, 0, 0, 0)
#define symbol_position(s,h,x,y) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "", h, x, y, 0)
#define symbol_orientation(s,h,x,y,r) renderSymbol(item, (obj!=NULL)?obj->obj:0, s, "", "", h, x, y, r)
#define symbol_cluster(x) renderCluster(item, x)
#define symbol_notice renderNotice(item)
#define light_sector_caption(n,t,s,o,y) renderSector(item, n, t, s, o, y)
#define light_sector(n) renderSector(item, n, NULL, NULL, 0, 0)
#define light_flare renderFlare(item)

#define make_string(s) { char *string=strdup(s)
#define make_char_string(x) { char *string=strdup(charString(item, x, 0))
#define make_char_string_indexed(x, n) { char *string=strdup(charString(item, x, n))
#define add_string(s) {string = realloc(string, strlen(string) + strlen(s) + 1); strcat(string, s);}
#define free_string free(string); }

#define line(s) drawLine(item, s)
#define line_symbols(s,g,ss,n) drawLineSymbols(item, s, g, ss, n)
#define line_text(t,s,o,y,r) drawLineText(item, t, s, o, y, r)
#define way_text(t,s,o,y,r) drawWayText(item, t, s, o, y, r)
#define area(s) drawArea(item, s)
#define text(t,s,x,y) drawText(item, t, s, x, y)

extern void main_rules(Item_t*, Obj_t*);

#endif

/*
 C Render rules macros
 =====================
 
 Contexts
 --------
 In any object_rules block there exists an item and an object. The item is global to the entire block. The object
 is initialized to the item type, but can be changed within sub-blocks. Additional arguments can be passed to
 object_rules blocks and are also global to the whole block unless overloaded within sub-blocks.
 
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
 has_item_attribute(<attributes>) True if current item has tag matching one of <attributes>
 attribute_test(<attribute>, <values>) True if <attribute> has one of the <values>
 
 Selections
 ----------
 use_object(<object>) Start block using this <object> with index=0
 use_object_select(<object>, <index>) Start block using this <object> with <index>
 used Terminate block
 
 Value Selectors
 ---------------
 attribute(<attribute>) get <attribute> value of current object
 item_attribute(<attribute>) get <attribute> value of item
 object_count(<object>) returns the highest contiguous index number of an <object>
 attribute_count(<attribute>) Returns the number of enumeration values
 attribute_next Returns next enumeration value
 this_type return item type
 
 Switch & Control Macros
 -----------------------
 literal_switch(<literal>) Switch on <literal> vlaues
 literal_case(<value>) <literal> has this <value>
 attribute_switch(<attribute>) Switch on <attribute> values
 attribute_case(<value>) <attribute> has this <value>
 attribute_default <attribute has all other values
 end_switch Terminate switch
 attribute_sequence(<attribute>) Step though list of enumeration values
 end_sequence Terminate sequence
 
 String Construction
 -------------------
 make_string(<string>) Create <string> instance 
 make_char_string(<type>) Create signal characteristics string for <type>
 make_char_string_indexed(<type>, <index>) Create signal characteristics string for <type> at <index>
 add_string(<string>) Append <string> to current string
 free_string Release current string
  
 Rendering Instructions
 ----------------------
 symbol(<symbol>) Places symbol centred at either current node or current area
 symbol_place(<symbol>, <handle>) Places symbol with <handle> positioned at current node/area
 symbol_position(<symbol>, <handle>, <x>, <y>) Places symbol with <handle> offset <x>,<y> from current node/area
 symbol_orientation(<symbol>, <handle>, <x>, <y>, <o>) Places symbol with <handle> offset <x>,<y> from current node/area and rotated <o> degrees
 colour_symbol(<symbol>, <panel>, <colour>)
 colour_symbol_place(<symbol>, <panel>, <colour>, <handle>)
 colour_symbol_position(<symbol>, <panel>, <colour>, <handle>, <x>, <y>)
 colour_symbol_orientation(<symbol>, <panel>, <colour>, <handle>, <x>, <y>, <o>)
 symbol_cluster(<type>) Place cluster of symbols 
 symbol_notice Place Notice Marks
 path(<style>) Draw path along current way.
 path_offset(<style>, <dy>) Draw a path along current way offset by <dy> from axis.
 area(<style>) Draw path around current closed way.
 area_offset(<style>, <dy>) Draw a path around current closed way offset by <dy> from axis.
 light_sector_caption(<index> , <text>, <style>, <offset>, (y offset>) Render light sector with characteristic string on sector arc
 light_sector(<index>) Render uncaptioned light sector
 light_flare Render light flare
 line(<style>) Draw line with <style>
 line_text(<text>, <style>, <offset> , <y offset> ,<reference>) Render <text> with <style> along line
 text(<text>, <style>, <x offset>, <y offset>) Place <text> with <style> <offset x> and <offset y> from centre of item
 */
