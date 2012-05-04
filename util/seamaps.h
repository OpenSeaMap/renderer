/* Copyright 2011 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEAMAPS_H
#define SEAMAPS_H

#include <string.h>

#define TRUE  1
#define FALSE 0
typedef int boolean;

typedef enum { A, B, C, R, X } Reg_t;
typedef struct { Reg_t key; char *val; } RegSTR_t;

typedef enum {
  UNKOBJ, MASTER, OBJNAM, BCNCAR, BCNISD, BCNLAT, BCNSAW, BCNSPP,
  BOYCAR, BOYISD, BOYLAT, BOYSAW, BOYSPP, LITMAJ, LITMIN, LNDMRK,
  LITFLT, LITVES, TOPMAR, DAYMAR, FOGSIG, LIGHTS, RADRFL, RTPBCN,
  MORFAC, SISTAW, SISTAT, CGUSTA, PILBOP, RSCSTA, RDOSTA, RADSTA,
  OFSPLF, BUISGL, NOTMRK, PILPNT, BCNWTW, BOYWTW, RESARE
} Obj_t;
typedef struct { Obj_t key; char *val; } ObjSTR_t;

typedef enum {
  UNKATT, CATEGORY, SHAPE, COLOUR, COLPAT, SYSTEM, GROUP, PERIOD, RANGE, CHANNEL, FUNCTION, ADDMRK
} Att_t;
typedef struct { Att_t key; char *val; } AttSTR_t;

typedef enum {
  NOCAT, LAM_PORT, LAM_STBD, LAM_PPORT, LAM_PSTBD, CAM_NORTH, CAM_EAST, CAM_SOUTH, CAM_WEST,
  ACH_URST, ACH_DEEP, ACH_TANK, ACH_EXPL, ACH_QUAR, ACH_SPLN, ACH_SCAN, ACH_SCMO, ACH_T24H, ACH_TLIM,
  SPM_UNKN, SPM_WARN, SPM_CHBF, SPM_YCHT, SPM_CABL, SPM_OFAL, SPM_ODAS, SPM_RECN, SPM_MOOR, SPM_LNBY,
  SPM_LDNG, SPM_NOTC, SPM_TSS, SPM_FOUL, SPM_DIVE, SPM_FRRY, SPM_ANCH,
  MOR_DLPN, MOR_DDPN, MOR_BLRD, MOR_WALL, MOR_POST, MOR_CHWR, MOR_BUOY,
  SIS_PTCL, SIS_PTED, SIS_IPT, SIS_BRTH, SIS_DOCK, SIS_LOCK, SIS_FBAR, SIS_BRDG, SIS_DRDG, SIS_TRFC,
  SIS_DNGR, SIS_OBST, SIS_CABL, SIS_MILY, SIS_DSTR, SIS_WTHR, SIS_STRM, SIS_ICE, SIS_TIME, SIS_TIDE,
  SIS_TSTM, SIS_TGAG, SIS_TSCL, SIS_DIVE, SIS_LGAG, LIT_DIRF, LIT_LEDG,
  LMK_CHMY, LMK_CARN, LMK_DSHA, LMK_FLGS, LMK_FLRS, LMK_MNMT, LMK_TOWR, LMK_WNDM, LMK_WTRT, LMK_MNRT,
  LMK_MAST, LMK_WNDS, LMK_CLMN, LMK_OBLK, LMK_STAT, LMK_CROS, LMK_DOME, LMK_SCNR, LMK_WNDL, LMK_SPIR
} Cat_t;
typedef struct { Cat_t key; char *val; } CatSTR_t;

typedef enum {
  UNKSHP, BEACON, PILLAR, SPAR, CAN, CONI, SPHERI, BARREL, FLOAT, SUPER, BUOYANT, CAIRN, PILE, LATTICE, TOWER, STAKE, POLE, POST, PERCH,
  BOLLARD, DOLPHIN, DEVDOLPHIN, SIGNAL, PILOT, MAJOR, MINOR, HOUSE, CHIMNEY, DISH, FLAGSTAFF, FLARESTACK, MONUMENT, LANDTOWER, WINDMOTOR,
  WATERTOWER, MAST, WINDSOCK, LANDCROSS, DOME, RADAR, WINDMILL, SPIRE, PLATFORM, FPSO
} Shp_t;
typedef struct { Shp_t key; char *val; } ShpSTR_t;

typedef enum {
  UNKCOL=0, RED=1, WHITE=2, YELLOW=4, GREEN=8, ORANGE=16, AMBER=32,  BLUE=64, VIOLET=128, BLACK=256, GREY=512, BROWN=1024, MAGENTA=2048, PINK=4096
} Col_t;
typedef struct { Col_t key; char *val; } ColSTR_t;

typedef enum {
  UNKCHR=0, FIXED=1, FLASH=2, LFLASH=4, QUICK=8, VQUICK=16, UQUICK=32, ISOPHASED=64, OCCULTING=128, MORSE=256, ALTERNATING=512, IQUICK=1024, IVQUICK=2048, IUQUICK=4096
} Chr_t;
typedef struct { Chr_t key; char *val; } ChrMAP_t;

typedef enum {
  UNKVIS, HIGH, LOW, FAINT, INTEN, UNINTEN, REST, OBS, PARTOBS
} Vis_t;
typedef struct { Vis_t key; char *val; } VisSTR_t;

typedef enum {
  UNKLIT, DIR, VERT, HORIZ, UPPER, LOWER, LEAD, REAR, FRONT, AERO, AIROBS, FOGDET, FLOOD, STRIP, SUBS, SPOT, MOIRE, EMERG, BEAR
} Lit_t;
typedef struct { Lit_t key; char *val; } LitSTR_t;

typedef enum {
  UNKEXH, H24, DAY, NIGHT, FOG, WARN, STORM
} Exh_t;
typedef struct { Exh_t key; char *val; } ExhSTR_t;

typedef enum {
  NOATL, COL, CHR, GRP, SEQ, PER, LIT, BEG, END, RAD, HGT, RNG, VIS, EXH, ORT, MLT, ALT, OBJ
} Atl_t;
typedef struct { Atl_t key; char *val; } AtlSTR_t;

typedef enum {
  NOPAT, HSTR, VSTR, DIAG, SQUARED, BORDER, CROSS, SALT
} Pat_t;
typedef struct { Pat_t key; char *val; } PatSTR_t;

typedef enum {
  NOTOP, CYL, CONE, CONE_INV, SPHERE, X_SHAPE, NORTH, SOUTH, EAST, WEST, SPHERES2, BOARD, DIAMOND, CIRCLE, TRIANGLE, TRIANGLE_INV, SQUARE
} Top_t;
typedef struct { Top_t key; char *val; } TopSTR_t;

typedef enum {
  NORTB, REFLECTOR, RACON, RAMARK, LEADING
} Rtb_t;
typedef struct { Rtb_t key; char *val; } RtbSTR_t;

typedef enum {
  NOFOG, UNKFOG, HORN, SIREN, DIA, BELL, WHIS, GONG, EXPLOS
} Fog_t;
typedef struct { Fog_t key; char *val; } FogSTR_t;

typedef enum {
  UNKSTS, PERM, OCC, REC, NIU, INT, RESV, TEMP, PRIV, MAND, DEST, EXT, ILLUM, HIST, PUB, SYNC, WATCH, UNWAT, DOUBT
} Sts_t;
typedef struct { Sts_t key; char *val; } StsSTR_t;

typedef enum {
  UNKCNS, BRICK, CONC, BOULD, HSURF, USURF, WOOD, METAL, GLAS, PAINT
} Cns_t;
typedef struct { Cns_t key; char *val; } CnsSTR_t;

typedef enum {
  UNKCON, CONSP, NCONS, REFL
} Con_t;
typedef struct { Con_t key; char *val; } ConSTR_t;

typedef enum {
  UNKFNC, HMO, CSTM, HLTH, HOSP, POFF, HOTEL, RWSTA, POLICE, WPOL, PILO, PILL, BANK, DCHQ, TRNS, FACT, PWRS, ADMIN, EDUC, CHCH, CHPL,
  TMPL, PGDA, SHSH, BTMP, MOSQ, MRBT, LOOK, COMS, TV, RADO, RADR, LSUP, MWAV, COOL, OBSV, TIMB, CLK, CTRL, AMOR, STAD, BUSS
} Fnc_t;
typedef struct { Fnc_t key; char *val; } FncSTR_t;

typedef enum {
  NOADD, TBRD, BBRD, RTRI, LTRI, BTRI
} Add_t;
typedef struct { Add_t key; char *val; } AddSTR_t;

typedef enum { NULKEY } Key_t;
typedef struct { Key_t key; char *val; } Map_t;

extern const RegSTR_t RegSTR[];
extern const ObjSTR_t ObjSTR[];
extern const AttSTR_t AttSTR[];
extern const CatSTR_t CatSTR[];
extern const ShpSTR_t ShpSTR[];
extern const ColSTR_t ColSTR[];
extern const ColSTR_t ColMAP[];
extern const ChrMAP_t ChrMAP[];
extern const AtlSTR_t AtlSTR[];
extern const VisSTR_t VisSTR[];
extern const LitSTR_t LitSTR[];
extern const LitSTR_t LitMAP[];
extern const ExhSTR_t ExhSTR[];
extern const PatSTR_t PatSTR[];
extern const PatSTR_t PatMAP[];
extern const TopSTR_t TopSTR[];
extern const RtbSTR_t RtbSTR[];
extern const RtbSTR_t RtbMAP[];
extern const FogSTR_t FogSTR[];
extern const FogSTR_t FogMAP[];
extern const StsSTR_t StsSTR[];
extern const CnsSTR_t CnsSTR[];
extern const ConSTR_t ConSTR[];
extern const CatSTR_t SisMAP[];
extern const FncSTR_t FncSTR[];
extern const AddSTR_t AddSTR[];

extern Key_t getkey(Map_t[], char*);
extern char *getval(Map_t[], Key_t);

#endif
