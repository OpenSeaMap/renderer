/* Copyright 2012 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */


#ifndef map_h
#define map_h

#ifndef system_h
#define system_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#endif

#include "s57obj.h"
#include "s57att.h"
#include "s57val.h"

typedef int bool;
#define true (1)
#define false (0)

typedef enum {EMPTY, NODE, WAY, RELN} Flag_t;

typedef struct ITEM Item_t;
typedef struct RELN Reln_t;
typedef struct MSHP Mshp_t;
typedef struct MEMB Memb_t;
typedef struct WAY Way_t;
typedef struct REF Ref_t;
typedef struct NODE Node_t;
typedef struct WMBS Wmbs_t;
typedef struct OBJ Obj_t;
typedef struct TAG Tag_t;
typedef struct ATT Att_t;

struct ATT {
  Att_t *next;
  Val_t val;
};

struct OBJ {
  Obj_t *next;
  Obja_t obj;
  int idx;
  Att_t *atts;
};

struct WMBS {
  Wmbs_t *next;
  Item_t *way;
};

struct NODE {
  double lat;
  double lon;
  Wmbs_t *ways;
};

struct REF {
  Ref_t *flink;
  Ref_t *blink;
  Item_t *ref;
};

struct WAY {
  Ref_t *flink;
  Ref_t *blink;
};

struct MEMB {
  Memb_t *flink;
  Memb_t *blink;
  Item_t *memb;
  char *role;
};

struct MSHP {
  Mshp_t *next;
  Item_t *reln;
};

struct RELN {
  Memb_t *flink;
  Memb_t *blink;
};

struct TAG {
  Tag_t *next;
  char *key;
  char *val;
};

struct ITEM {
  Item_t *items[10];
  Item_t *next;
  char *id;
  Tag_t *tags;
  Mshp_t *mshp;
  Obj_t objs;
  Flag_t flag;
  union {
    Node_t node;
    Way_t way;
    Reln_t reln;
  } type;
};

extern Item_t map;
extern Item_t *addItem(Flag_t);
extern Item_t *getItem(char*);
extern Tag_t *addTag(Item_t*, char*, char*);
extern Obj_t *addObj(Item_t*, Obja_t, int);
extern Obj_t *getObj(Item_t*, Obja_t, int);
extern Ref_t *addRef(Item_t*, char*);
extern Memb_t *addMemb(Item_t*, char*, char*);
extern Att_t *addAtt(Obj_t*, char*, char*);
extern Att_t *getAtt(Obj_t*, Atta_t);
extern Enum_t getAttEnum(Obj_t*, Atta_t, int);
extern bool testAtt(Att_t*, Enum_t);

#endif
