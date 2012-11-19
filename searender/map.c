/* Copyright 2012 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */


#include "map.h"
#include "s57val.h"

extern double minlat, minlon, maxlat, maxlon;
extern int zoom;
extern bool bb;

Item_t *addItem(Flag_t flag) {
  char *id = NULL;
  double lat = 0.0;
  double lon = 0.0;
  char *token = strtok(NULL, " =");
  do {
    char *val = strtok(NULL, "'\"");
    if (strcmp(token, "id") == 0) {
      id = strdup(val);
    } else if (strcmp(token, "lat") == 0) {
      lat = atof(val);
      if (!bb) {
        if (lat < minlat) minlat = lat;
        if (lat > maxlat) maxlat = lat;
      }
    } else if (strcmp(token, "lon") == 0) {
      lon = atof(val);
      if (!bb) {
        if (lon < minlon) minlon = lon;
        if (lon > maxlon) maxlon = lon;
      }
    }
    token = strtok(NULL, " =");
  } while (token != NULL);
  int i;
  Item_t *link;
  char *digit = id;
  if (*digit == '-') *digit = '0';
  for (link = &map, i = (*digit - '0'); *digit++ != 0; link = link->items[i], i = (*digit - '0')) {
    if (link->items[i] == NULL) {
      link->items[i] = calloc(1, sizeof(Item_t));
    }
  }
  link->id = id;
  link->flag = flag;
  if (flag == NODE) {
    link->type.node.lat = lat;
    link->type.node.lon = lon;
  }
  link->next = map.next;
  map.next = link;
  return link;
}

Item_t *getItem(char *id) {
  int i;
  Item_t *link;
  char *digit = id;
  if (*digit == '-') *digit = '0';
  for (link = &map, i = (*digit - '0'); *digit++ != 0; link = link->items[i], i = (*digit - '0')) {
    if (link->items[i] == NULL) {
      return NULL;
    }
  }
  return link;
}

Tag_t *addTag(Item_t *item, char *key, char *val) {
  Tag_t *link = calloc(1, sizeof(Tag_t));
  link->key = strdup(key);
  link->val = strdup(val);
  link->next = item->tags;
  item->tags = link;
  return link;
}

Obj_t *addObj(Item_t *item, Obja_t obj, int idx) {
  Obj_t *link = calloc(1, sizeof(Obj_t));
  link->obj = obj;
  link->idx = idx;
  link->next = item->objs.next;
  item->objs.next = link;
  return link;
}

Obj_t *getObj(Item_t *item, Obja_t obj, int idx) {
  Obj_t *link = item->objs.next;
   while (link != NULL) {
    if ((link->obj == obj) && (link->idx == idx)) break;
    link = link->next;
  }
  return link;
}

Att_t *addAtt(Obj_t *obj, char *key, char *val) {
  Att_t *link = calloc(1, sizeof(Att_t));
  link->val = convertValue(val, enumAttribute(key, obj->obj));
  link->next = obj->atts;
  obj->atts = link;
  return link;
}

Att_t *getAtt(Obj_t *obj, Atta_t key) {
  Att_t *link = obj != NULL ? obj->atts : NULL;
  while (link != NULL) {
    if (link->val.key == key) break;
    link = link->next;
  }
  return link;
}

Enum_t getAttEnum(Obj_t *obj, Atta_t key, int idx) {
  Att_t *att = getAtt(obj, key);
  if (att == NULL) return 0;
  if (att->val.type == E)
    return att->val.val.e;
  else if (att->val.type == L) {
    Lst_t *val = att->val.val.l;
    for (int i = 0; i < idx; i++) {
      if (val->next == NULL) return 0;
      val = val->next;
    }
    return val->val;
  } else return 0;
}

bool testAtt(Att_t *att, Enum_t val) {
  if ((att->val.type == E) && (att->val.val.e == val)) return true;
  else if (att->val.type == L) {
    Lst_t *lst = att->val.val.l;
    while (lst != NULL) {
      if (lst->val == val) break;
      lst = lst->next;
    }
    if (lst != NULL) return true;
  }
  return false;
}

Ref_t *addRef(Item_t *item, char *ref) {
  Ref_t *link = calloc(1, sizeof(Ref_t));
  link->ref = getItem(ref);
  if (link->ref == NULL) return NULL;
  Wmbs_t *node = calloc(1, sizeof(Wmbs_t));
  node->way = item;
  node->next = link->ref->type.node.ways;
  link->ref->type.node.ways = node;
  link->flink = item->type.way.flink;
  if (item->type.way.blink == NULL)
    item->type.way.blink = link;
  else
    item->type.way.flink->blink = link;
  item->type.way.flink = link;
  return link;
}

Memb_t *addMemb(Item_t *item, char *id, char *role) {
  Memb_t *link = calloc(1, sizeof(Memb_t));
  link->memb = getItem(id);
  Mshp_t *memb = calloc(1, sizeof(Mshp_t));
  memb->reln = item;
  memb->next = link->memb->mshp;
  link->memb->mshp = memb;
  link->role = strdup(role);
  link->flink = item->type.reln.flink;
  if (item->type.reln.blink == NULL)
    item->type.reln.blink = link;
  else
    item->type.reln.flink->blink = link;
  item->type.reln.flink = link;
  return link;
}
