/* Copyright 2012 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#include "map.h"
#include "render.h"
#include "s57obj.h"
#include "s57att.h"
#include "s57val.h"

extern double minlat, minlon, maxlat, maxlon;
extern int zoom;
extern bool bb;

int main (int argc, const char * argv[]) {
  
  char line[10000];
  char *ele;
  char *key;
  char *prefix;
  char *attribute;
  int idx;
  char *value;
  
  Flag_t flag = EMPTY;
  Item_t *item = NULL;
  Obj_t *object = NULL;
  
  maxlat = -90.0;
  minlat = 90.0;
  maxlon = -180.0;
  minlon = 180.0;
  
#ifdef DEBUG
  sleep(2); // *** for debugger capture
#endif

  set_conv("UTF-8");
  
  if ((argc < 3) || (argc > 4)) {
    fprintf(stderr, "usage: searender symbols zoom [bb]\n");
    exit (EXIT_FAILURE);
  }
  zoom = atoi(argv[2]);
  if ((zoom < 0) || (zoom > 19)) {
    printf("zoom out of range\n");
    exit (EXIT_FAILURE);
  }
    
  if (argc == 4) {
    char *bbstr = strdup(argv[3]);
    bbstr = strtok(bbstr, ",");
    minlat = atof(bbstr);
    bbstr = strtok(NULL, ",");
    minlon = atof(bbstr);
    bbstr = strtok(NULL, ",");
    maxlat = atof(bbstr);
    bbstr = strtok(NULL, ",");
    maxlon = atof(bbstr);
    bb = true;
  }
    
	while (fgets(line, 10000, stdin) != NULL) {
		ele = strtok(line, " \t<");
    if ((strcmp(ele, "bounds") == 0) && !bb)  {
      char *token = strtok(NULL, " =");
      do {
        char *val = strtok(NULL, "'\"");
        if (strcmp(token, "minlat") == 0) {
          minlat = atof(val);
        } else if (strcmp(token, "minlon") == 0) {
          minlon = atof(val);
        } else if (strcmp(token, "maxlat") == 0) {
          maxlat = atof(val);
        } else if (strcmp(token, "maxlon") == 0) {
          maxlon = atof(val);
        }
        token = strtok(NULL, " =");
      } while (token != NULL);
      bb = true;
    } else if (strcmp(ele, "node") == 0) {
      flag = NODE;
      item = addItem(NODE);
    } else if (strcmp(ele, "/node>\n") == 0) {
      flag = EMPTY;
      item = NULL;
    } else if (strcmp(ele, "way") == 0) {
      flag = WAY;
      item = addItem(WAY);
    } else if (strcmp(ele, "/way>\n") == 0) {
      flag = EMPTY;
      item = NULL;
    } else if (strcmp(ele, "relation") == 0) {
      flag = RELN;
      item = addItem(RELN);
    } else if (strcmp(ele, "/relation>\n") == 0) {
      flag = EMPTY;
      item = NULL;
    } else if ((strcmp(ele, "tag") == 0) && (flag != EMPTY)) {
      strtok(NULL, "\"'");
      key = strtok(NULL, "\"'");
      strtok(NULL, "\"'");
      value = strtok(NULL, "\"");
      addTag(item, key, value);
      if (strncmp(key, "seamark:", 8) != 0) continue;
      prefix = strtok(key, ":");
      prefix = strtok(NULL, ":");
      idx = 0;
      attribute = strtok(NULL, ":");
      if (attribute == NULL)
        if (strcmp(prefix, "type") == 0) {
          item->objs.obj = enumType(value);
        } else {
          int obj = enumType(prefix);
          if (obj != 0) {
            object = getObj(item, obj, idx);
            if (object == NULL)
              object = addObj(item, obj, idx);
          addAtt(object, attribute, value);
          } else {
            Atta_t att = enumAttribute(prefix, UNKOBJ);
            if (att != UNKATT) {
              addAtt(&item->objs, prefix, value);
            }
          }
        }
      else {
        if (isdigit(*attribute)) {
          idx = atoi(attribute);
          attribute = strtok(NULL, ":");
        }
        int obj = enumType(prefix);
        if (obj != 0) {
          object = getObj(item, obj, idx);
          if (object == NULL)
            object = addObj(item, obj, idx);
          addAtt(object, attribute, value);
        }
      }
    } else if ((strcmp(ele, "nd") == 0) && (flag == WAY)) {
      char *token = strtok(NULL, " =");
      do {
        char *val = strtok(NULL, "'\"");
        if (strcmp(token, "ref") == 0) {
          addRef(item, val);
          break;
        }
        token = strtok(NULL, " =");
      } while (token != NULL);
    } else if ((strcmp(ele, "member") == 0) && (flag == RELN)) {
/*      char *ref;
      char *role;
      char *token = strtok(NULL, " =");
      do {
        char *val = strtok(NULL, "'\"");
        if (strcmp(token, "ref") == 0) {
          ref = strdup(val);
        } else if (strcmp(token, "role") == 0) {
          role = strdup(val);
        }
        token = strtok(NULL, " =");
      } while (token != NULL);
      addMemb(item, ref, role);
*/    }
  }
  
  render(argv[1]);
}
