/* Copyright 2012 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

/* Conversion of S-57 ATTL values to OSeaM attribute tag key names */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "s57att.h"

Attl_t s57attributes[ATTSIZ] = {
  [UNKATT]=0, [AGENCY]=1, [BCNSHP]=2, [BUISHP]=3, [BOYSHP]=4, [BURDEP]=5, [CALSGN]=6, [CATAIR]=7, [CATACH]=8, [CATBRG]=9, [CATBUA]=10, [CATCBL]=11, [CATCAN]=12, [CATCAM]=13,
  [CATCHP]=14, [CATCOA]=15, [CATCTR]=16, [CATCON]=17, [CATCOV]=18, [CATCRN]=19, [CATDAM]=20, [CATDIS]=21, [CATDOC]=22, [CATDPG]=23, [CATFNC]=24, [CATFRY]=25, [CATFIF]=26,
  [CATFOG]=27, [CATFOR]=28, [CATGAT]=29, [CATHAF]=30, [CATHLK]=31, [CATICE]=32, [CATINB]=33, [CATLND]=34, [CATLMK]=35, [CATLAM]=36, [CATLIT]=37, [CATMFA]=38, [CATMPA]=39,
  [CATMOR]=40, [CATNAV]=41, [CATOBS]=42, [CATOFP]=43, [CATOLB]=44, [CATPLE]=45, [CATPIL]=46, [CATPIP]=47, [CATPRA]=48, [CATPYL]=49, [CATQUA]=50, [CATRAS]=51, [CATRTB]=52,
  [CATROS]=53, [CATTRK]=54, [CATRSC]=55, [CATREA]=56, [CATROD]=57, [CATRUN]=58, [CATSEA]=59, [CATSLC]=60, [CATSIT]=61, [CATSIW]=62, [CATSIL]=63, [CATSLO]=64, [CATSCF]=65,
  [CATSPM]=66, [CATTSS]=67, [CATVEG]=68, [CATWAT]=69, [CATWED]=70, [CATWRK]=71, [CATZOC]=72, [COLOUR]=75, [COLPAT]=76, [COMCHA]=77, [CONDTN]=81, [CONRAD]=82, [CONVIS]=83,
  [CURVEL]=84, [DATEND]=85, [DATSTA]=86, [DRVAL1]=87, [DRVAL2]=88, [DUNITS]=89, [ELEVAT]=90, [ESTRNG]=91, [EXCLIT]=92, [EXPSOU]=93, [FUNCTN]=94, [HEIGHT]=95, [HUNITS]=96,
  [HORACC]=97, [HORCLR]=98, [HORLEN]=99, [HORWID]=100, [ICEFAC]=101, [INFORM]=102, [JRSDTN]=103, [$JUSTH]=104, [$JUSTV]=105, [LIFCAP]=106, [LITCHR]=107, [LITVIS]=108,
  [MARSYS]=109, [MLTYLT]=110, [NATION]=111, [NATCON]=112, [NATSUR]=113, [NATQUA]=114, [NMDATE]=115, [OBJNAM]=116, [ORIENT]=117, [PEREND]=118, [PERSTA]=119, [PICREP]=120,
  [PILDST]=121, [PRCTRY]=122, [PRODCT]=123, [PUBREF]=124, [QUASOU]=125, [RADWAL]=126, [RADIUS]=127, [RECDAT]=128, [RECIND]=129, [RYRMGV]=130, [RESTRN]=131, [SECTR1]=136,
  [SECTR2]=137, [SHIPAM]=138, [SIGFRQ]=139, [SIGGEN]=140, [SIGGRP]=141, [SIGPER]=142, [SIGSEQ]=143, [SOUACC]=144, [SDISMX]=145, [SDISMN]=146, [SORDAT]=147, [SORIND]=148,
  [STATUS]=149, [SURATH]=150, [SUREND]=151, [SURSTA]=152, [SURTYP]=153, [TECSOU]=156, [TXTDSC]=158, [TIMEND]=168, [TIMSTA]=169, [TOPSHP]=171, [TRAFIC]=172, [VALACM]=173,
  [VALDCO]=174, [VALLMA]=175, [VALMAG]=176, [VALMXR]=177, [VALNMR]=178, [VALSOU]=179, [VERACC]=180, [VERCLR]=181, [VERCCL]=182, [VERCOP]=183, [VERCSA]=184, [VERDAT]=185,
  [VERLEN]=186, [WATLEV]=187, [CAT_TS]=188, [PUNITS]=189, [NINFOM]=300, [NOBJNM]=301, [NPLDST]=302, [NTXTDS]=304, [HORDAT]=400, [POSACC]=401, [QUAPOS]=402
};
Attl_t iencattributes[ATTSIZ] = {
  [CATACH]=17000, [CATDIS]=17001, [CATSIT]=17002, [CATSIW]=17003, [RESTRN]=17004, [VERDAT]=17005, [CATBRG]=17006, [CATFRY]=17007, [CATHAF]=17008, [MARSYS]=17009, [CATCHP]=17010,
  [CATLAM]=17011, [CATSLC]=17012, [ADDMRK]=17050, [CATBNK]=17051, [CATNMK]=17052, [CLSDNG]=17055, [DIRIMP]=17056, [DISBK1]=17057, [DISBK2]=17058, [DISIPU]=17059, [DISIPD]=17060,
  [ELEVA1]=17061, [ELEVA2]=17062, [FNCTNM]=17063, [WTWDIS]=17064, [BUNVES]=17065, [CATBRT]=17066, [CATBUN]=17067, [CATCCL]=17069, [CATHBR]=17070, [CATRFD]=17071, [CATTML]=17072,
  [COMCTN]=17073, [HORCLL]=17074, [HORCLW]=17075, [TRSHGD]=17076, [UNLOCD]=17077, [CATGAG]=17078, [HIGWAT]=17080, [HIGNAM]=17081, [LOWWAT]=17082, [LOWNAM]=17083, [MEAWAT]=17084,
  [MEANAM]=17085, [OTHWAT]=17086, [OTHNAM]=17087, [REFLEV]=17088, [SDRLEV]=17089, [VCRLEV]=17090, [CATVTR]=17091, [CATTAB]=17092, [SCHREF]=17093, [USESHP]=17094, [CURVHW]=17095,
  [CURVLW]=17096, [CURVMW]=17097, [CURVOW]=17098, [APTREF]=17099, [CATEXS]=17100, [CATCBL]=17101, [CATHLK]=17102, [HUNITS]=17103, [WATLEV]=17104, [CATWWM]=17112, [BNKWTW]=17999,
  [LG_SPD]=18001, [LG_SPR]=18002, [LG_BME]=18003, [LG_LGS]=18004, [LG_DRT]=18005, [LG_WDP]=18006, [LG_WDU]=18007, [LG_REL]=18008, [LG_FNC]=18009, [LG_DES]=18010, [LG_PBR]=18011,
  [LC_CSI]=18012, [LC_CSE]=18013, [LC_ASI]=18014, [LC_ASE]=18015, [LC_CCI]=18016, [LC_CCE]=18017, [LC_BM1]=18018, [LC_BM2]=18019, [LC_LG1]=18020, [LC_LG2]=18021, [LC_DR1]=18022,
  [LC_DR2]=18023, [LC_SP1]=18024, [LC_SP2]=18025, [LC_WD1]=18026, [LC_WD2]=18027,
  [SHPTYP]=33066, [UPDMSG]=40000
};

char *osmattributes[ATTSIZ] = {
  [UNKATT]="", [AGENCY]="agency", [BCNSHP]="shape", [BUISHP]="shape", [BOYSHP]="shape", [BURDEP]="depth_buried", [CALSGN]="callsign", [CATAIR]="category", [CATACH]="category", [CATBRG]="category",
  [CATBUA]="category", [CATCBL]="category", [CATCAN]="category", [CATCAM]="category", [CATCHP]="category", [CATCOA]="category", [CATCTR]="category", [CATCON]="category", [CATCOV]="category",
  [CATCRN]="category", [CATDAM]="category", [CATDIS]="category", [CATDOC]="category", [CATDPG]="category", [CATFNC]="category", [CATFRY]="category", [CATFIF]="category", [CATFOG]="category",
  [CATFOR]="category", [CATGAT]="category", [CATHAF]="category", [CATHLK]="category", [CATICE]="category", [CATINB]="category", [CATLND]="category", [CATLMK]="category", [CATLAM]="category",
  [CATLIT]="category", [CATMFA]="category", [CATMPA]="category", [CATMOR]="category", [CATNAV]="category", [CATOBS]="category", [CATOFP]="category", [CATOLB]="category", [CATPLE]="category",
  [CATPIL]="category", [CATPIP]="category", [CATPRA]="category", [CATPYL]="category", [CATQUA]="category", [CATRAS]="category", [CATRTB]="category", [CATROS]="category", [CATTRK]="category",
  [CATRSC]="category", [CATREA]="category", [CATROD]="category", [CATRUN]="category", [CATSEA]="category", [CATSLC]="category", [CATSIT]="category", [CATSIW]="category", [CATSIL]="category",
  [CATSLO]="category", [CATSCF]="category", [CATSPM]="category", [CATTSS]="category", [CATVEG]="category", [CATWAT]="category", [CATWED]="category", [CATWRK]="category", [CATZOC]="category",
  [COLOUR]="colour", [COLPAT]="colour_pattern", [COMCHA]="channel", [CONDTN]="condition", [CONRAD]="reflectivity", [CONVIS]="conspicuity", [CURVEL]="velocity", [DATEND]="end_date",
  [DATSTA]="start_date", [DRVAL1]="minimum_depth", [DRVAL2]="maximum_depth", [DUNITS]="depth_units", [ELEVAT]="elevation", [ESTRNG]="estimated_range", [EXCLIT]="exhibition", [EXPSOU]="exposition",
  [FUNCTN]="function", [HEIGHT]="height", [HUNITS]="units", [HORACC]="accuracy", [HORCLR]="clearance", [HORLEN]="length", [HORWID]="width", [ICEFAC]="factor", [INFORM]="information",
  [JRSDTN]="jurisdiction", [$JUSTH]="", [$JUSTV]="", [LIFCAP]="maximum_load", [LITCHR]="character", [LITVIS]="visibility", [MARSYS]="system", [MLTYLT]="multiple", [NATION]="nationality",
  [NATCON]="construction", [NATSUR]="surface", [NATQUA]="surface_qualification", [NMDATE]="nm_date", [OBJNAM]="name", [ORIENT]="orientation", [PEREND]="end_date", [PERSTA]="start_date",
  [PICREP]="representation", [PILDST]="pilot_district", [PRCTRY]="producing_country", [PRODCT]="product", [PUBREF]="reference", [QUASOU]="quality", [RADWAL]="wavelength", [RADIUS]="radius",
  [RECDAT]="date", [RECIND]="indication", [RYRMGV]="year", [RESTRN]="restriction", [SECTR1]="sector_start", [SECTR2]="sector_end", [SHIPAM]="shift", [SIGFRQ]="frequency", [SIGGEN]="generation",
  [SIGGRP]="group", [SIGPER]="period", [SIGSEQ]="sequence", [SOUACC]="accuracy", [SDISMX]="maximum_sounding", [SDISMN]="minimum_sounding", [SORDAT]="source_date", [SORIND]="source",
  [STATUS]="status", [SURATH]="authority", [SUREND]="end_date", [SURSTA]="start_date", [SURTYP]="survey", [TECSOU]="technique", [TXTDSC]="description", [TIMEND]="end_time", [TIMSTA]="start_time",
  [TOPSHP]="shape", [TRAFIC]="flow", [VALACM]="variation_change", [VALDCO]="depth", [VALLMA]="anomaly", [VALMAG]="variation", [VALMXR]="maximum_range", [VALNMR]="range", [VALSOU]="sounding",
  [VERACC]="vertical_accuracy", [VERCLR]="clearance_height", [VERCCL]="clearance_height_closed", [VERCOP]="clearance_height_open", [VERCSA]="clearance_height_safe", [VERDAT]="vertical_datum",
  [VERLEN]="vertical_length", [WATLEV]="water_level", [CAT_TS]="category", [PUNITS]="positional_units", [NINFOM]="national_information", [NOBJNM]="national_name", [NPLDST]="national_pilot_district",
  [NTXTDS]="national_description", [HORDAT]="horizontal_datum", [POSACC]="positional_accuracy", [QUAPOS]="position_quality", [ADDMRK]="addition", [BNKWTW]="bank", [CATBNK]="category",
  [CATNMK]="category", [CLSDNG]="class", [DIRIMP]="impact", [DISBK1]="distance_start", [DISBK2]="distance_end", [DISIPU]="distance_up", [DISIPD]="distance_down", [ELEVA1]="minimum_elevation",
  [ELEVA2]="maximum_elevation", [FNCTNM]="function", [WTWDIS]="distance", [BUNVES]="availibility", [CATBRT]="category", [CATBUN]="category", [CATCCL]="category", [CATHBR]="category",
  [CATRFD]="category", [CATTML]="category", [COMCTN]="communication", [HORCLL]="clearance_length", [HORCLW]="clearance_width", [TRSHGD]="goods", [UNLOCD]="locode", [CATGAG]="category",
  [HIGWAT]="high_value", [HIGNAM]="high_name", [LOWWAT]="low_value", [LOWNAM]="low_name", [MEAWAT]="mean_value", [MEANAM]="mean_name", [OTHWAT]="local_value", [OTHNAM]="local_name",
  [REFLEV]="gravity_reference", [SDRLEV]="sounding_name", [VCRLEV]="vertical_name", [CATVTR]="category", [CATTAB]="operation", [SCHREF]="schedule", [USESHP]="use", [CURVHW]="high_velocity",
  [CURVLW]="low_velocity", [CURVMW]="mean_velocity", [CURVOW]="other_velocity", [APTREF]="passing_time", [CATEXS]="category", [CATWWM]="category", [SHPTYP]="ship", [UPDMSG]="message",
  [LITRAD]="radius"
};

typedef struct {
  Obja_t obj;
  Atta_t att;
} ObjAtt_t;

ObjAtt_t objattributes[] = {
  {ACHARE, CATACH}, {ACHBRT, CATACH}, {AIRARE, CATAIR}, {BCNCAR, BCNSHP}, {BCNCAR, CATCAM}, {BCNISD, BCNSHP}, {BCNLAT, BCNSHP}, {BCNLAT, CATLAM}, {BCNSAW, BCNSHP}, {BCNSPP, BCNSHP},
  {BCNSPP, CATSPM}, {BCNWTW, BCNSHP}, {BCNWTW, CATWWM}, {BOYCAR, BOYSHP}, {BOYCAR, CATCAM}, {BOYISD, BOYSHP}, {BOYLAT, BOYSHP}, {BOYLAT, CATLAM}, {BOYSAW, BOYSHP}, {BOYSPP, BOYSHP},
  {BOYSPP, CATSPM}, {BOYWTW, BOYSHP}, {BOYWTW, CATWWM}, {BOYINB, BOYSHP}, {BOYINB, CATINB}, {BRIDGE, CATBRG}, {BUAARE, CATBUA}, {BUNSTA, CATBUN}, {BUISGL, FUNCTN}, {BUISGL, BUISHP},
  {CANALS, CATCAN}, {CANBNK, CATBNK}, {CBLARE, CATCBL}, {CBLOHD, CATCBL}, {CBLSUB, CATCBL}, {CHKPNT, CATCHP}, {COALNE, CATCOA}, {COMARE, CATCCL}, {CONVYR, CATCON}, {CTRPNT, CATCTR},
  {CRANES, CATCRN}, {DAMCON, CATDAM}, {DAYMAR, TOPSHP}, {DISMAR, CATDIS}, {DMPGRD, CATDPG}, {DOCARE, CATDOC}, {EXCNST, CATEXS}, {FERYRT, CATFRY}, {FNCLNE, CATFNC}, {FOGSIG, CATFOG},
  {FORSTC, CATFOR}, {FSHFAC, CATFIF}, {GATCON, CATGAT}, {HRBARE, CATHBR}, {HRBBSN, CATHBR}, {HRBFAC, CATHAF}, {HRBFAC, CATHBR}, {HULKES, CATHLK}, {ICEARE, CATICE}, {LIGHTS, CATLIT},
  {LNDRGN, CATLND}, {LNDMRK, FUNCTN}, {LNDMRK, CATLMK}, {MARCUL, CATMFA}, {MIPARE, CATMPA}, {MORFAC, CATMOR}, {MORFAC, BOYSHP}, {M_COVR, CATCOV}, {M_QUAL, CATQUA}, {M_QUAL, CATZOC},
  {NAVLNE, CATNAV}, {NOTMRK, CATNMK}, {NOTMRK, FNCTNM}, {OBSTRN, CATOBS}, {OFSPLF, CATOFP}, {OILBAR, CATOLB}, {OSPARE, CATPRA}, {PILBOP, CATPIL}, {PILPNT, CATPLE}, {PIPARE, CATPIP},
  {PIPOHD, CATPIP}, {PIPSOL, CATPIP}, {PRDARE, CATPRA}, {PYLONS, CATPYL}, {RADSTA, CATRAS}, {RCRTCL, CATTRK}, {RCTLPT, CATTRK}, {RDOSTA, CATROS}, {RECTRC, CATTRK}, {REFDMP, CATRFD},
  {RESARE, CATREA}, {RIVBNK, CATBNK}, {ROADWY, CATROD}, {RSCSTA, CATRSC}, {RTPBCN, CATRTB}, {RUNWAY, CATRUN}, {SEAARE, CATSEA}, {SILTNK, CATSIL}, {SILTNK, BUISHP}, {SISTAT, CATSIT},
  {SISTAW, CATSIW}, {SLCONS, CATSLC}, {SLOTOP, CATSLO}, {SLOGRD, CATSLO}, {SMCFAC, CATSCF}, {TERMNL, CATTML}, {TOPMAR, TOPSHP}, {TSELNE, CATTSS}, {TSEZNE, CATTSS}, {TSSBND, CATTSS},
  {TSSCRS, CATTSS}, {TSSLPT, CATTSS}, {TSSRON, CATTSS}, {TWRTPT, CATTRK}, {VEGATN, CATVEG}, {VEHTRF, CATVTR}, {WATTUR, CATWAT}, {WEDKLP, CATWED}, {WRECKS, CATWRK}, {TS_FEB, CAT_TS},
  {BERTHS, CATBRT}, {WTWGAG, CATGAG}, {UNKOBJ, RADIUS}, {LIGHTS, LITRAD}, {UNKOBJ, UNKATT}
};

typedef enum {NON, NOT, YES} Ver_t;

Ver_t verifyAttribute(Obja_t obja, Atta_t atta) {
  Ver_t ver = NOT;
  for (int i = 0; objattributes[i].att != UNKATT; i++) {
    if (objattributes[i].att == atta) {
      if (objattributes[i].obj == obja)
        return YES;
      else
        ver = NON;
    }
  }
  return ver;
}

Atta_t lookupAttribute(Attl_t attl) {
  if (attl < 10000)
    for (int i = 0; i < ATTSIZ; i++) {
      if (s57attributes[i] == attl)
        return i;
    }
  else
    for (int i = 0; i < ATTSIZ; i++) {
      if (iencattributes[i] == attl)
        return i;
    }
  return 0;
}

char *decodeAttribute(Attl_t attl) {
  return osmattributes[lookupAttribute(attl)];
}

Attl_t encodeAttribute(char *attr, Objl_t objl) {
  Atta_t atta = enumAttribute(attr, lookupType(objl));
  return s57attributes[atta] != 0 ? s57attributes[atta] : iencattributes[atta];
}

char *stringAttribute(Atta_t atta) {
  return osmattributes[atta] != NULL ? osmattributes[atta] : "";
}

Atta_t enumAttribute(char *attr, Obja_t obja) {
  if ((attr != NULL) && (strlen(attr) > 0)) {
    for (int i = 0; i < ATTSIZ; i++) {
      if ((osmattributes[i] != NULL) && (strcmp(osmattributes[i], attr) == 0) && verifyAttribute(obja, i))
        return i;
    }
  }
  return 0;
}
