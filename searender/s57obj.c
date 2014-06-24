/* Copyright 2012 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

/* Conversion of S57 OBJL values to OSeaM object type tag names */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "s57obj.h"

Objl_t s57types[OBJSIZ] = {
  [UNKOBJ]=0, [ADMARE]=1, [AIRARE]=2, [ACHBRT]=3, [ACHARE]=4, [BCNCAR]=5, [BCNISD]=6, [BCNLAT]=7, [BCNSAW]=8, [BCNSPP]=9, [BERTHS]=10, [BRIDGE]=11, [BUISGL]=12, [BUAARE]=13,
  [BOYCAR]=14, [BOYINB]=15, [BOYISD]=16, [BOYLAT]=17, [BOYSAW]=18, [BOYSPP]=19, [CBLARE]=20, [CBLOHD]=21, [CBLSUB]=22, [CANALS]=23, [CANBNK]=24, [CTSARE]=25, [CAUSWY]=26,
  [CTNARE]=27, [CHKPNT]=28, [CGUSTA]=29, [COALNE]=30, [CONZNE]=31, [COSARE]=32, [CTRPNT]=33, [CONVYR]=34, [CRANES]=35, [CURENT]=36, [CUSZNE]=37, [DAMCON]=38, [DAYMAR]=39,
  [DWRTCL]=40, [DWRTPT]=41, [DEPARE]=42, [DEPCNT]=43, [DISMAR]=44, [DOCARE]=45, [DRGARE]=46, [DRYDOC]=47, [DMPGRD]=48, [DYKCON]=49, [EXEZNE]=50, [FAIRWY]=51, [FNCLNE]=52,
  [FERYRT]=53, [FSHZNE]=54, [FSHFAC]=55, [FSHGRD]=56, [FLODOC]=57, [FOGSIG]=58, [FORSTC]=59, [FRPARE]=60, [GATCON]=61, [GRIDRN]=62, [HRBARE]=63, [HRBFAC]=64, [HULKES]=65,
  [ICEARE]=66, [ICNARE]=67, [ISTZNE]=68, [LAKARE]=69, [LAKSHR]=70, [LNDARE]=71, [LNDELV]=72, [LNDRGN]=73, [LNDMRK]=74, [LIGHTS]=75, [LITFLT]=76, [LITVES]=77, [LOCMAG]=78,
  [LOKBSN]=79, [LOGPON]=80, [MAGVAR]=81, [MARCUL]=82, [MIPARE]=83, [MORFAC]=84, [NAVLNE]=85, [OBSTRN]=86, [OFSPLF]=87, [OSPARE]=88, [OILBAR]=89, [PILPNT]=90, [PILBOP]=91,
  [PIPARE]=92, [PIPOHD]=93, [PIPSOL]=94, [PONTON]=95, [PRCARE]=96, [PRDARE]=97, [PYLONS]=98, [RADLNE]=99, [RADRNG]=100, [RADRFL]=101, [RADSTA]=102, [RTPBCN]=103,
  [RDOCAL]=104, [RDOSTA]=105, [RAILWY]=106, [RAPIDS]=107, [RCRTCL]=108, [RECTRC]=109, [RCTLPT]=110, [RSCSTA]=111, [RESARE]=112, [RETRFL]=113, [RIVERS]=114, [RIVBNK]=115,
  [ROADWY]=116, [RUNWAY]=117, [SNDWAV]=118, [SEAARE]=119, [SPLARE]=120, [SBDARE]=121, [SLCONS]=122, [SISTAT]=123, [SISTAW]=124, [SILTNK]=125, [SLOTOP]=126, [SLOGRD]=127,
  [SMCFAC]=128, [SOUNDG]=129, [SPRING]=130, [SQUARE]=131, [STSLNE]=132, [SUBTLN]=133, [SWPARE]=134, [TESARE]=135, [TS_PRH]=136, [TS_PNH]=137, [TS_PAD]=138, [TS_TIS]=139,
  [T_HMON]=140, [T_NHMN]=141, [T_TIMS]=142, [TIDEWY]=143, [TOPMAR]=144, [TSELNE]=145, [TSSBND]=146, [TSSCRS]=147, [TSSLPT]=148, [TSSRON]=149, [TSEZNE]=150, [TUNNEL]=151,
  [TWRTPT]=152, [UWTROC]=153, [UNSARE]=154, [VEGATN]=155, [WATTUR]=156, [WATFAL]=157, [WEDKLP]=158, [WRECKS]=159, [TS_FEB]=160, [M_ACCY]=300, [M_CSCL]=301, [M_COVR]=302,
  [M_HDAT]=303, [M_HOPA]=304, [M_NPUB]=305, [M_NSYS]=306, [M_PROD]=307, [M_QUAL]=308, [M_SDAT]=309, [M_SREL]=310, [M_UNIT]=311, [M_VDAT]=312, [C_AGGR]=400, [C_ASSO]=401,
  [C_STAC]=402, [$AREAS]=500, [$LINES]=501, [$CSYMB]=502, [$COMPS]=503, [$TEXTS]=504
};
Objl_t ienctypes[OBJSIZ] = {
  [UNKOBJ]=0, [ACHBRT]=17000, [ACHARE]=17001, [CANBNK]=17002, [DEPARE]=17003, [DISMAR]=17004, [RESARE]=17005, [RIVBNK]=17006, [SISTAT]=17007, [SISTAW]=17008,
  [TOPMAR]=17009, [BERTHS]=17010, [BRIDGE]=17011, [CBLOHD]=17012, [FERYRT]=17013, [HRBARE]=17014, [HRBFAC]=17015, [LOKBSN]=17016, [RDOCAL]=17017, [M_NSYS]=17018,
  [CURENT]=17019, [HULKES]=17020, [PONTON]=17021, [M_SDAT]=17022, [M_VDAT]=17023, [PIPOHD]=17024, [FLODOC]=17025, [CHKPNT]=17027, [BCNLAT]=17028, [BOYLAT]=17029,
  [CRANES]=17030, [GATCON]=17031, [SLCONS]=17032, [UWTROC]=17033, [CONVYR]=17034, [NOTMRK]=17050, [WTWAXS]=17051, [WTWPRF]=17052, [BRGARE]=17053, [BUNSTA]=17054,
  [COMARE]=17055, [HRBBSN]=17056, [LOKARE]=17057, [LKBSPT]=17058, [PRTARE]=17059, [BCNWTW]=17060, [BOYWTW]=17061, [REFDMP]=17062, [RTPLPT]=17063, [TERMNL]=17064,
  [TRNBSN]=17065, [WTWARE]=17066, [WTWGAG]=17067, [TISDGE]=17068, [VEHTRF]=17069, [EXCNST]=17070, [LG_SDM]=18001, [LG_VSP]=18002
};

char *osmtypes[OBJSIZ] = {
  [UNKOBJ]="", [ADMARE]="administration", [AIRARE]="airfield", [ACHBRT]="anchor_berth", [ACHARE]="anchorage", [BCNCAR]="beacon_cardinal", [BCNISD]="beacon_isolated_danger",
  [BCNLAT]="beacon_lateral", [BCNSAW]="beacon_safe_water", [BCNSPP]="beacon_special_purpose", [BERTHS]="berth", [BRIDGE]="bridge", [BUISGL]="building", [BUAARE]="built-up_area",
  [BOYCAR]="buoy_cardinal", [BOYINB]="buoy_installation", [BOYISD]="buoy_isolated_danger", [BOYLAT]="buoy_lateral", [BOYSAW]="buoy_safe_water", [BOYSPP]="buoy_special_purpose",
  [CBLARE]="cable_area", [CBLOHD]="cable_overhead", [CBLSUB]="cable_submarine", [CANALS]="canal", [CANBNK]="canal_bank", [CTSARE]="cargo_area", [CAUSWY]="causeway",
  [CTNARE]="caution_area", [CHKPNT]="checkpoint", [CGUSTA]="coastguard_station", [COALNE]="coastline", [CONZNE]="contiguous_zone", [COSARE]="continental_shelf",
  [CTRPNT]="control_point", [CONVYR]="conveyor", [CRANES]="crane", [CURENT]="current", [CUSZNE]="custom_zone", [DAMCON]="dam", [DAYMAR]="daymark",
  [DWRTCL]="deep_water_route_centreline", [DWRTPT]="deep_water_route", [DEPARE]="depth_area", [DEPCNT]="depth_contour", [DISMAR]="distance_mark", [DOCARE]="dock",
  [DRGARE]="dredged_area", [DRYDOC]="dry_dock", [DMPGRD]="dumping_ground", [DYKCON]="dyke", [EXEZNE]="exclusive_economic_zone", [FAIRWY]="fairway", [FNCLNE]="wall",
  [FERYRT]="ferry_route", [FSHZNE]="fishery_zone", [FSHFAC]="fishing_facility", [FSHGRD]="fishing_ground", [FLODOC]="floating_dock", [FOGSIG]="fog_signal",
  [FORSTC]="fortified_structure", [FRPARE]="free_port_area", [GATCON]="gate", [GRIDRN]="gridiron", [HRBARE]="harbour_area", [HRBFAC]="harbour", [HULKES]="hulk",
  [ICEARE]="ice_area", [ICNARE]="incineration_zone", [ISTZNE]="inshore_traffic_zone", [LAKARE]="lake", [LAKSHR]="lake_shore", [LNDARE]="land_area", [LNDELV]="land_elevation",
  [LNDRGN]="land_region", [LNDMRK]="landmark", [LIGHTS]="light", [LITFLT]="light_float", [LITVES]="light_vessel", [LOCMAG]="local_magnetic_anomaly", [LOKBSN]="lock_basin",
  [LOGPON]="log_pond", [MAGVAR]="magnetic_variation", [MARCUL]="marine_farm", [MIPARE]="military_area", [MORFAC]="mooring", [NAVLNE]="navigation_line", [OBSTRN]="obstruction",
  [OFSPLF]="platform", [OSPARE]="production_area", [OILBAR]="oil_barrier", [PILPNT]="pile", [PILBOP]="pilot_boarding", [PIPARE]="pipeline_area", [PIPOHD]="pipeline_overhead",
  [PIPSOL]="pipeline_submarine", [PONTON]="pontoon", [PRCARE]="precautionary_area", [PRDARE]="land_production_area", [PYLONS]="pylon", [RADLNE]="radar_line", [RADRNG]="radar_range",
  [RADRFL]="radar_reflector", [RADSTA]="radar_station", [RTPBCN]="radar_transponder", [RDOCAL]="calling-in_point", [RDOSTA]="radio_station", [RAILWY]="railway", [RAPIDS]="rapids",
  [RCRTCL]="recommended_route_centreline", [RECTRC]="recommended_track", [RCTLPT]="recommended_traffic_lane", [RSCSTA]="rescue_station", [RESARE]="restricted_area",
  [RETRFL]="retro_reflector", [RIVERS]="river", [RIVBNK]="river_bank", [ROADWY]="road", [RUNWAY]="runway", [SNDWAV]="sand_waves", [SEAARE]="sea_area", [SPLARE]="seaplane_landing_area",
  [SBDARE]="seabed_area", [SLCONS]="shoreline_construction", [SISTAT]="signal_station_traffic", [SISTAW]="signal_station_warning", [SILTNK]="tank", [SLOTOP]="slope_topline",
  [SLOGRD]="sloping_ground", [SMCFAC]="small_craft_facility", [SOUNDG]="sounding", [SPRING]="spring", [SQUARE]="square", [STSLNE]="territorial_baseline", [SUBTLN]="submarine_transit_lane",
  [SWPARE]="swept_area", [TESARE]="territorial_area", [TS_PRH]="", [TS_PNH]="", [TS_PAD]="", [TS_TIS]="", [T_HMON]="", [T_NHMN]="", [T_TIMS]="", [TIDEWY]="tideway", [TOPMAR]="topmark",
  [TSELNE]="separation_line", [TSSBND]="separation_boundary", [TSSCRS]="separation_crossing", [TSSLPT]="separation_lane", [TSSRON]="separation_roundabout", [TSEZNE]="separation_zone",
  [TUNNEL]="tunnel", [TWRTPT]="two-way_route", [UWTROC]="rock", [UNSARE]="unsurveyed_area", [VEGATN]="vegetation", [WATTUR]="water_turbulence", [WATFAL]="waterfall", [WEDKLP]="weed",
  [WRECKS]="wreck", [TS_FEB]="tidal_stream", [M_ACCY]="", [M_CSCL]="", [M_COVR]="coverage", [M_HDAT]="", [M_HOPA]="", [M_NPUB]="", [M_NSYS]="", [M_PROD]="", [M_QUAL]="data_quality",
  [M_SDAT]="", [M_SREL]="", [M_UNIT]="", [M_VDAT]="", [C_AGGR]="", [C_ASSO]="", [C_STAC]="", [$AREAS]="", [$LINES]="", [$CSYMB]="", [$COMPS]="", [$TEXTS]="",
  [NOTMRK]="notice", [WTWAXS]="waterway_axis", [WTWPRF]="waterway_profile", [BRGARE]="bridge_area", [BUNSTA]="bunker_station", [COMARE]="communication_area", [HRBBSN]="harbour_basin",
  [LOKARE]="lock_area", [LKBSPT]="lock_basin_part", [PRTARE]="port_area", [BCNWTW]="beacon_waterway", [BOYWTW]="buoy_waterway", [REFDMP]="refuse_dump", [RTPLPT]="route_planning_point",
  [TERMNL]="terminal", [TRNBSN]="turning_basin", [WTWARE]="waterway_area", [WTWGAG]="waterway_gauge", [TISDGE]="time_schedule", [VEHTRF]="vehicle_transfer", [EXCNST]="exceptional_structure",
  [LG_SDM]="", [LG_VSP]="", [LITMAJ]="light_major", [LITMIN]="light_minor"
};

Obja_t lookupType(Objl_t objl) {
  if (objl < 10000)
    for (int i = 0; i < OBJSIZ; i++) {
      if (s57types[i] == objl)
        return i;
    }
  else 
    for (int i = 0; i < OBJSIZ; i++) {
      if (ienctypes[i] == objl)
        return i;
    }
  return 0;
}

char *decodeType(Objl_t objl) {
  return osmtypes[lookupType(objl)];
}

Objl_t encodeType(char *type) {
  int i = enumType(type);
  return s57types[i] != 0 ? s57types[i] : ienctypes[i];
}

char *stringType(Obja_t obja) {
  return osmtypes[obja] != NULL ? osmtypes[obja] : ""; 
}

Obja_t enumType(char *type) {
  if ((type != NULL) && (strlen(type) > 0)) {
    for (int i = 0; i < OBJSIZ; i++) {
      if (strcmp(osmtypes[i], type) == 0)
        return i;
    }
  }
  return UNKOBJ;
}
