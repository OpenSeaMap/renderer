/* Copyright 2011 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#ifndef searender_render_h
#define searender_render_h

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

typedef enum {CC, TL, TR, TC, LC, RC, BL, BR, BC} Handle_t;
typedef enum {POINT, LINE, AREA} Feature_t;

extern void render();
extern int renderColourSymbol(Item_t*, Obja_t, char*, char*, char*, Handle_t, double, double, double);
extern int renderSymbol(Item_t*, Obja_t, char*, char*, char*, Handle_t, double, double, double);
extern void renderCluster(Item_t*, char*);
extern void renderNotice(Item_t*);
extern void renderSector(Item_t*, int, char*, char*, double, int);
extern void renderFlare(Item_t*);

extern int drawLine(Item_t*, char*);
extern void drawLineSymbols(Item_t*, char*, double, char*, int);
extern int drawLineText(Item_t*, char*, char*, double, double, int);
extern int drawWayText(Item_t*, char*, char*, double, double, int);
extern int drawArea(Item_t*, char*);
extern int drawText(Item_t*, char*, char*, double, double);

extern bool compareTypes(Item_t*, char*);
extern bool compareObjects(Item_t*, char*);
extern int countObjects(Item_t*, char*);
extern bool compareAttributes(Obj_t*, char*);

extern int countValues(Att_t*);
extern bool compareValues(Att_t*, char*);
extern bool compareLiterals(char*, char*);
extern char *charString(Item_t*, char*, int);
extern Feature_t testFeature(Item_t*);
extern double calcArea(Item_t*);

bool testArea(Item_t*);
bool testLine(Item_t*);
bool testNode(Item_t*);
int countNodes(Item_t*);

extern Item_t *findItem(char*);
extern Item_t *findNext();

extern int zoom;

#endif
