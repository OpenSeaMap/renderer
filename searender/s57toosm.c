/* Copyright 2012 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "s57obj.h"
#include "s57att.h"
#include "s57val.h"

typedef struct ATTRIBUTE {
  int attl;
  char *atvl;
  struct ATTRIBUTE *next;
} attribute_t;

typedef struct REFERENCE {
  long id;
  int rcid;
  int rcnm;
  int ornt;
  int usag;
  int topi;
  struct REFERENCE *next;
} reference_t;

typedef struct POINTER {
  int agen;
  int fidn;
  int fids;
  struct POINTER *next;
} pointer_t;

typedef struct FEATURE {
  long id;
  int prim;
  int agen;
  int fidn;
  int fids;
  int objl;
  attribute_t *att;
  reference_t *ref;
  pointer_t *master;
  pointer_t *peer;
  pointer_t *slave;
  int sflag;
  struct FEATURE *next;
} feature_t;

typedef struct COORDINATE {
  long id;
  float lat;
  float lon;
  struct COORDINATE *next;
} coordinate_t;

typedef struct XNODE {
  long id;
  int rcid;
  int rcnm;
  reference_t *ref;
  coordinate_t *xy;
  struct XNODE *next;
} node_t;

node_t nodes;
feature_t features;

node_t *node = NULL;
feature_t *feature = NULL;

coordinate_t *xy = NULL;
reference_t *ref = NULL;
attribute_t *att = NULL;

int agen;
int fidn;
int fids;

enum {NONE, VRID, FRID} record = NONE;
float comf = 1.0;

double minlat = 90.0;
double minlon = 180.0;
double maxlat = -90.0;
double maxlon = -180.0;

char line[1000];
char *ele;
char *val;

/* Search for referenced node */

node_t *findNode(int rcid, int rcnm) {
  node_t *nptr = nodes.next;
  while (nptr != NULL) {
    if ((nptr->rcid == rcid) && (nptr->rcnm == rcnm)) {
      break;
    }
    nptr = nptr->next;
  }
  return (nptr);
}

/* Search for referenced feature */

feature_t *findFeature(pointer_t *ref) {
  if (ref == NULL) return NULL;
  feature_t *fptr = features.next;
  while (fptr != NULL) {
    if ((fptr->agen == ref->agen) && (fptr->fidn == ref->fidn) && (fptr->fids == ref->fids)) {
      break;
    }
    fptr = fptr->next;
  }
  return (fptr);
}

int main (int argc, const char * argv[]) {

#ifdef DEBUG
  sleep(2); // *** for debugger capture
#endif
  
  int args = 1;
  
  if (argc <= 1) {
    printf("No arguments - usage: s57toosm [-s <source aknowledgement>] all|nodes|ways|<space separated seamark type list>\n");
    exit (EXIT_FAILURE);
  }
  
  if (strcmp(argv[1], "-s") == 0) {
    args = 3;
  }
  
  nodes.next = NULL;
  features.next = NULL;
  
  /* Build S-57 model from output of 8211view */
  
	while (fgets(line, 1000, stdin) != NULL) {
		ele = strtok(line, " ");
    if (strcmp(ele, "Field") == 0) {
      ele = strtok(NULL, " :");
    }
    val = strtok(NULL, " ");
    if ((val != NULL) && (strcmp(val, "=") == 0))
      val = &val[2];
    if (strcmp(ele, "COMF") == 0) {
      comf = atof(val);
    } else if (strcmp(ele, "AALL") == 0) {
      if (atoi(val) == 2)
        set_conv("UCS-2");
      else {
        set_conv("ISO-8859-1");
      }
    } else if (strcmp(ele, "VRID") == 0) {
      if (record != VRID) {
        for (node = &nodes; node->next != NULL; node = node->next);
      }
      node->next = calloc(1, sizeof(node_t));
      node = node->next;
      xy = NULL;
      ref = NULL;
      record = VRID;
    } else if (strcmp(ele, "FRID") == 0) {
      if (record != FRID) {
        for (feature = &features; feature->next != NULL; feature = feature->next);
      }
      feature->next = calloc(1, sizeof(feature_t));
      feature = feature->next;
      ref = NULL;
      att = NULL;
      record = FRID;
    } else {
      if (record == NONE) {
      } else if (record == VRID) {
        
        /* Process nodes */
        
        if (strcmp(ele, "RCID") == 0) {
          node->rcid = atoi(val);
        } else if (strcmp(ele, "RCNM") == 0) {
          node->rcnm = atoi(val);
        } else if (strcmp(ele, "NAME") == 0) {
          
          /* Make list of referenced nodes */
          
          if (ref == NULL) {
            ref = node->ref = calloc(1, sizeof(reference_t));
          } else {
            ref->next = calloc(1, sizeof(reference_t));
            ref = ref->next;
          }
          ref->ornt = ref->usag = ref->topi = 255;
          ref->next = NULL;
          do val = strtok(NULL, " =,");
          while (strcmp(val, "RCNM") != 0);
          val = strtok(NULL, " =,");
          ref->rcnm = atoi(val);
          do val = strtok(NULL, " =,");
          while (strcmp(val, "RCID") != 0);
          val = strtok(NULL, " =,");
          ref->rcid = atoi(val);
        } else if (strcmp(ele, "XCOO") == 0) {
          
          /* Make list of coordinates */
          
          if (xy == NULL) {
            xy = node->xy = calloc(1, sizeof(coordinate_t));
            xy->lat = xy->lon = 360.0;
            xy->next = NULL;
          } else if (xy->lon < 360.0) {
            xy->next = calloc(1, sizeof(coordinate_t));
            xy = xy->next;
            xy->lat = xy->lon = 360.0;
            xy->next = NULL;
          }
          xy->lon = atof(val) / comf;
          if (xy->lon < minlon) minlon = xy->lon;
          if (xy->lon > maxlon) maxlon = xy->lon;
        } else if (strcmp(ele, "YCOO") == 0) {
          if (xy == NULL) {
            xy = node->xy = calloc(1, sizeof(coordinate_t));
            xy->lat = xy->lon = 360.0;
            xy->next = NULL;
          } else if (xy->lat < 360.0) {
            xy->next = calloc(1, sizeof(coordinate_t));
            xy = xy->next;
            xy->lat = xy->lon = 360.0;
            xy->next = NULL;
          }
          xy->lat = atof(val) / comf;
          if (xy->lat < minlat) minlat = xy->lat;
          if (xy->lat > maxlat) maxlat = xy->lat;
        }
      } else if (record == FRID) {
        
        /* Process features */
        
        if (strcmp(ele, "AGEN") == 0) {
          feature->agen = atoi(val);
        } else if (strcmp(ele, "FIDN") == 0) {
          feature->fidn = atoi(val);
        } else if (strcmp(ele, "FIDS") == 0) {
          feature->fids = atoi(val);
        } else if (strcmp(ele, "OBJL") == 0) {
          feature->objl = atoi(val);
        } else if (strcmp(ele, "PRIM") == 0) {
          feature->prim = atoi(val);
        } else if (strcmp(ele, "NAME") == 0) {
          
          /* Make list of referenced nodes */
          
          if (ref == NULL) {
            ref = feature->ref = calloc(1, sizeof(reference_t));
          } else {
            ref->next = calloc(1, sizeof(reference_t));
            ref = ref->next;
          }
          ref->ornt = ref->usag = ref->topi = 255;
          ref->next = NULL;
          do val = strtok(NULL, " =,");
          while (strcmp(val, "RCNM") != 0);
          val = strtok(NULL, " =,");
          ref->rcnm = atoi(val);
          do val = strtok(NULL, " =,");
          while (strcmp(val, "RCID") != 0);
          val = strtok(NULL, " =,");
          ref->rcid = atoi(val);
        } else if (strcmp(ele, "LNAM") == 0) {
          
          /* Make lists of referenced features */
          
          do val = strtok(NULL, " =,");
          while (strcmp(val, "AGEN") != 0);
          val = strtok(NULL, " =,");
          agen = atoi(val);
          do val = strtok(NULL, " =,");
          while (strcmp(val, "FIDN") != 0);
          val = strtok(NULL, " =,");
          fidn = atoi(val);
          do val = strtok(NULL, " =,");
          while (strcmp(val, "FIDS") != 0);
          val = strtok(NULL, " =,");
          fids = atoi(val);
        } else if (strcmp(ele, "ORNT") == 0) {
          ref->ornt = atoi(val);
        } else if (strcmp(ele, "USAG") == 0) {
          ref->usag = atoi(val);
        } else if (strcmp(ele, "RIND") == 0) {
          pointer_t *link = calloc(1, sizeof(pointer_t));
          link->agen = agen;
          link->fidn = fidn;
          link->fids = fids;
          switch (atoi(val)) {
            case 1:
              /* Make list of masters */
              link->next = feature->master;
              feature->master = link;
              break;
            case 2:
              /* Make list of slaves */
              link->next = feature->slave;
              feature->slave = link;
              break;
            case 3:
              /* Make list of peers */
              link->next = feature->peer;
              feature->peer = link;
              break;
          }
        } else if (strcmp(ele, "ATTL") == 0) {
          
          /* Make list of attributes */
          
          if (att == NULL) {
            att = feature->att = calloc(1, sizeof(attribute_t));
          } else {
            att->next = calloc(1, sizeof(attribute_t));
            att = att->next;
          }
          att->atvl = NULL;
          att->next = NULL;
          att->attl = atoi(val);
        } else if (strcmp(ele, "ATVL") == 0) {
          val = strtok(val, "'`\n");
          if (val == NULL) {
            att->atvl = "";
          } else {
            att->atvl = calloc(1, strlen(val)+1);
            strcpy(att->atvl, val);
          }
        }
      }
    }
	}
  
  /* All input done - now convert S-57 features to OSM format */
  
  feature_t *fptr;
  for (fptr = features.next; fptr != NULL; fptr = fptr->next) {
    if ((fptr->prim == 1) && (fptr->slave != NULL)) {
      pointer_t *link = fptr->slave;
      do {
        feature_t *feature = findFeature(link);
        if (feature->prim == 1) feature->sflag = 1;
        link = link->next;
      } while (link != NULL);
    }
  }
  
  printf("<?xml version='1.0' encoding='UTF-8'?>\n");
  printf("<osm version='0.6' generator='s57toosm'>\n");
  printf("<bounds minlat='%g' minlon='%g' maxlat='%g' maxlon='%g'/>\n", minlat, minlon, maxlat, maxlon);
  
  long id = 0;
  int way = 0;
  enum {NONE, ALL, NODES, WAYS, LIST} match = NONE;
  if (argc > args) {
    if (strcmp(argv[args], "all") == 0) match = ALL;
    else if (strcmp(argv[args], "nodes") == 0) match = NODES;
    else if (strcmp(argv[args], "ways") == 0) match = WAYS;
    else match = LIST;
  }
  for (fptr = features.next; fptr != NULL; fptr = fptr->next) {
    char *type = decodeType(fptr->objl);
    reference_t *inners = NULL;
    long members[100];
    int multi = 0;
    if (strlen(type) > 0) {
      if (fptr->ref != NULL) {
        int matched = (!(fptr->sflag) && ((match == ALL) || ((match == NODES) && (fptr->prim == 1)) || ((match == WAYS) && ((fptr->prim == 2) || (fptr->prim == 3)))));
        if (match == LIST) {
          int i = 0;
          for (i = args; i < argc; i++) {
            if (strcmp(type, argv[i]) == 0)
              break;
          }
          matched = (i < argc);
        }
        if (!matched) continue;
        fptr->id = --id;
        if ((fptr->prim == 2) || (fptr->prim == 3)) {
          way = 1;
          reference_t *eref = fptr->ref;
          do {
            node_t *enode = findNode(eref->rcid, eref->rcnm);
            node_t *cnode = findNode(enode->ref->rcid, enode->ref->rcnm);
            if (cnode->id == 0) {
              cnode->id = --id;
              printf("<node id='%ld' lat='%f' lon='%f' version='1'/>\n", id, cnode->xy->lat, cnode->xy->lon);
            }
            coordinate_t *xy;
            for (xy = enode->xy; xy != NULL; xy = xy->next) {
              if (xy->id == 0) {
                xy->id = --id;
                printf("<node id='%ld' lat='%f' lon='%f' version='1'/>\n", id, xy->lat, xy->lon);
              }
            }
            cnode = findNode(enode->ref->next->rcid, enode->ref->next->rcnm);
            if (cnode->id == 0) {
              cnode->id = --id;
              printf("<node id='%ld' lat='%f' lon='%f' version='1'/>\n", id, cnode->xy->lat, cnode->xy->lon);
            }
            eref = eref->next;
          } while (eref != NULL);
          printf("<way id='%ld' version='1'>\n", fptr->id);
          eref = fptr->ref;
          long last = 0;
          do {
            node_t *enode = findNode(eref->rcid, eref->rcnm);
            node_t *cnode;
            if (eref->usag == 2) {
              if (inners == NULL) {
                inners = eref;
                members[multi++] = id;
              }
            } else {
              if (eref->ornt == 2) {
                long ids[10000];
                int i = 0;
                ids[i++] = findNode(enode->ref->rcid, enode->ref->rcnm)->id;
                coordinate_t *xy;
                for (xy = enode->xy; xy != NULL; xy = xy->next) {
                  ids[i++] = xy->id;
                }
                ids[i++] = findNode(enode->ref->next->rcid, enode->ref->next->rcnm)->id;
                do {
                  if (ids[--i] != last)
                    printf(" <nd ref='%ld'/>\n", ids[i]);
                } while (i > 0);
                last = ids[0];
              } else {
                cnode = findNode(enode->ref->rcid, enode->ref->rcnm);
                if (cnode->id != last)
                  printf(" <nd ref='%ld'/>\n", cnode->id);
                coordinate_t *xy;
                for (xy = enode->xy; xy != NULL; xy = xy->next) {
                  printf(" <nd ref='%ld'/>\n", xy->id);
                }
                cnode = findNode(enode->ref->next->rcid, enode->ref->next->rcnm);
                printf(" <nd ref='%ld'/>\n", cnode->id);
                last = cnode->id;
              }
            }
            eref = eref->next;
          } while (eref != NULL);
        } else if (fptr->prim == 1) {
          way = 0;
          node_t *fnode = findNode(fptr->ref->rcid, fptr->ref->rcnm);
          if ((fptr->prim == 1) && (fnode != NULL) && (fnode->xy != NULL))
            printf("<node id='%ld' lat='%f' lon='%f' version='1'>\n", id, fnode->xy->lat, fnode->xy->lon);
          else
            continue;
        }
        int lights = 0;
        if (strcmp(type, "light") == 0) {
          printf(" <tag k='seamark:type' v='light_minor'/>\n");
          lights = 1;
        } else {
          printf(" <tag k='seamark:type' v='%s'/>\n", type);
        }
        pointer_t *slaves = fptr->slave;
        while (slaves != NULL) {
          feature_t *feature = findFeature(slaves);
          type = decodeType(feature->objl);
          if ((strlen(type) > 0) && (strcmp(type, "light") == 0))
            lights++;
          att = feature->att;
          while (att != NULL) {
            char *attr = decodeAttribute(att->attl);
            if ((strlen(attr) > 0) && (strcmp(attr, "sector_end") == 0))
              lights++;
            att = att->next;
          }
          slaves = slaves->next;
        }
        lights = (lights > 1) ? 1 : 0;
        feature_t *aptr = fptr;
        slaves = fptr->slave;
        char sector[8];
        do {
          type = decodeType(aptr->objl);
          if ((lights > 0) && (strcmp(type, "light") == 0))
            sprintf(sector, ":%d", lights++);
          else {
            strcpy(sector, "");
          }
          if (strlen(type) > 0) {
            if (aptr->att == NULL)
              printf(" <tag k='seamark:%s%s' v='yes'/>\n", type, sector);
            else {
              att = aptr->att;
              while (att != NULL) {
                char *attr = decodeAttribute(att->attl);
                if ((strlen(attr) > 0) && (strlen(att->atvl) > 0)) {
                  char *attv = decodeValue(att->attl, att->atvl);
                  if (attv != NULL)
                    printf(" <tag k='seamark:%s%s:%s' v='%s'/>\n", type, sector, attr, attv);
                }
                att = att->next;
              }
            }
          }
          aptr = findFeature(slaves);
          slaves = slaves != NULL ? slaves->next : NULL;
        } while (!way && (aptr != NULL));
        if (args > 2)
          printf(" <tag k='seamark:source' v='%s'/>\n", argv[2]);
        if (way)
          printf("</way>\n");
        else
          printf("</node>\n");
      }
    }
    if (inners != NULL) {
      printf("<way id='%ld' version='1'>\n", --id);
      members[multi++] = id;
      long first = 0;
      long last = 0;
      do {
        node_t *enode = findNode(inners->rcid, inners->rcnm);
        node_t *cnode;
          if (inners->ornt == 2) {
            long ids[10000];
            int i = 0;
            ids[i++] = findNode(enode->ref->rcid, enode->ref->rcnm)->id;
            coordinate_t *xy;
            for (xy = enode->xy; xy != NULL; xy = xy->next) {
              ids[i++] = xy->id;
            }
            ids[i++] = findNode(enode->ref->next->rcid, enode->ref->next->rcnm)->id;
            do {
              if (first == 0) first = ids[i];
              if (ids[--i] != last)
                printf(" <nd ref='%ld'/>\n", ids[i]);
            } while (i > 0);
            last = ids[0];
          } else {
            cnode = findNode(enode->ref->rcid, enode->ref->rcnm);
            if (first == 0) first = cnode->id;
            if (cnode->id != last)
              printf(" <nd ref='%ld'/>\n", cnode->id);
            coordinate_t *xy;
            for (xy = enode->xy; xy != NULL; xy = xy->next) {
              printf(" <nd ref='%ld'/>\n", xy->id);
            }
            cnode = findNode(enode->ref->next->rcid, enode->ref->next->rcnm);
            printf(" <nd ref='%ld'/>\n", cnode->id);
            last = cnode->id;
          }
        if (last == first) {
          if (inners->next != NULL) {
            printf("</way>\n");
            printf("<way id='%ld' version='1'>\n", --id);
            members[multi++] = id;
          }
        }
        inners = inners->next;
      } while (inners != NULL);
      printf("</way>\n");
      printf("<relation id='%ld' version='1'>\n", --id);
      while (--multi > 0) {
        printf(" <member type='way' ref='%ld' role='inner'/>\n", members[multi]);
      }
      printf(" <member type='way' ref='%ld' role='outer'/>\n", members[multi]);
      printf(" <tag k='type' v='multipolygon'/>\n");
      printf("</relation>\n");
    }
  }
  
  printf("</osm>\n");
  
  return (EXIT_SUCCESS);
}
