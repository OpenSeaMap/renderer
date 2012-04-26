/* Copyright 2011 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#include "seamaps.h"

const RegSTR_t RegSTR[] = {
  {A, "iala-a"},
  {B, "iala-b"},
  {C, "cevni"},
  {R, "riwr"},
  {X, "other"}
};

const ObjSTR_t ObjSTR[] = {
  {TYPTAG, "type"},
  {OBJNAM, "name"},
  {LIGHTS, "light"},
  {LITMIN, "light_minor"},
  {BOYLAT, "buoy_lateral"},
  {TOPMAR, "topmark"},
  {LITMAJ, "light_major"},
  {BCNLAT, "beacon_lateral"},
  {BOYSPP, "buoy_special_purpose"},
  {RTPBCN, "radar_transponder"},
  {BOYCAR, "buoy_cardinal"},
  {BCNCAR, "beacon_cardinal"},
  {FOGSIG, "fog_signal"},
  {BCNSPP, "beacon_special_purpose"},
  {LNDMRK, "landmark"},
  {BCNISD, "beacon_isolated_danger"},
  {BOYSAW, "buoy_safe_water"},
  {BOYISD, "buoy_isolated_danger"},
  {LITFLT, "light_float"},
  {BCNSAW, "beacon_safe_water"},
  {LITVES, "light_vessel"},
  {DAYMAR, "daymark"},
  {RADRFL, "radar_reflector"},
  {MORFAC, "mooring"},
  {SISTAW, "signal_station_warning"},
  {SISTAT, "signal_station_traffic"},
  {CGUSTA, "coastguard_station"},
  {PILBOP, "pilot_boarding"},
  {RSCSTA, "rescue_station"},
  {RDOSTA, "radio_station"},
  {RADSTA, "radar_station"},
  {OFSPLF, "platform"},
  {BUISGL, "building"},
  {NOTMRK, "notice"},
  {UNKOBJ, ""}
};

const AttSTR_t AttSTR[] = {
  {CATEGORY, "category"},
  {SHAPE, "shape"},
  {COLOUR, "colour"},
  {COLPAT, "colour_pattern"},
  {SYSTEM, "system"},
  {GROUP, "group"},
  {PERIOD, "period"},
  {RANGE, "range"},
  {CHANNEL, "channel"},
  {FUNCTION, "function"},
  {ADDMRK, "addition"},
  {UNKATT, ""}
};

const CatSTR_t CatSTR[] = {
  {LAM_PORT, "port"},
  {LAM_STBD, "starboard"},
  {LAM_PPORT, "preferred_channel_port"},
  {LAM_PSTBD, "preferred_channel_starboard"},
  {CAM_NORTH, "north"},
  {CAM_EAST, "east"},
  {CAM_SOUTH, "south"},
  {CAM_WEST, "west"},
  {SPM_UNKN, "unknown_purpose"},
  {SPM_WARN, "warning"},
  {SPM_CHBF, "channel_separation"},
  {SPM_YCHT, "yachting"},
  {SPM_CABL, "cable"},
  {SPM_OFAL, "outfall"},
  {SPM_ODAS, "odas"},
  {SPM_RECN, "recreational"},
  {SPM_MOOR, "mooring"},
  {SPM_LNBY, "lanby"},
  {SPM_LDNG, "leading"},
  {SPM_NOTC, "notice"},
  {SPM_TSS, "tss"},
  {SPM_FOUL, "foul"},
  {SPM_DIVE, "diving"},
  {SPM_FRRY, "ferry"},
  {SPM_ANCH, "anchorage"},
  {MOR_DLPN, "dolphin"},
  {MOR_DDPN, "deviation_dolphin"},
  {MOR_BLRD, "bollard"},
  {MOR_WALL, "wall"},
  {MOR_POST, "post"},
  {MOR_POST, "pile"},
  {MOR_CHWR, "chain"},
  {MOR_BUOY, "buoy"},
  {SIS_PTCL, "control"},
  {SIS_PTED, "entry"},
  {SIS_IPT, "ipt"},
  {SIS_BRTH, "berthing"},
  {SIS_DOCK, "dock"},
  {SIS_LOCK, "lock"},
  {SIS_FBAR, "barrage"},
  {SIS_BRDG, "bridge"},
  {SIS_DRDG, "dredging"},
  {SIS_TRFC, "traffic"},
  {SIS_DNGR, "danger"},
  {SIS_OBST, "obstruction"},
  {SIS_CABL, "cable"},
  {SIS_MILY, "military"},
  {SIS_DSTR, "distress"},
  {SIS_WTHR, "weather"},
  {SIS_STRM, "storm"},
  {SIS_ICE, "ice"},
  {SIS_TIME, "time"},
  {SIS_TIDE, "tide"},
  {SIS_TSTM, "stream"},
  {SIS_TGAG, "gauge"},
  {SIS_TSCL, "scale"},
  {SIS_DIVE, "diving"},
  {SIS_LGAG, "level"},
  {LMK_CHMY, "chimney"},
  {LMK_CARN, "cairn"},
  {LMK_DSHA, "dish_aerial"},
  {LMK_FLGS, "flagstaff"},
  {LMK_FLRS, "flare_stack"},
  {LMK_MNMT, "monument"},
  {LMK_TOWR, "tower"},
  {LMK_WNDM, "windmotor"},
  {LMK_WTRT, "water_tower"},
  {LMK_MAST, "mast"},
  {LMK_WNDS, "windsock"},
  {LMK_CLMN, "column"},
  {LMK_OBLK, "obelisk"},
  {LMK_STAT, "statue"},
  {LMK_CROS, "cross"},
  {LMK_DOME, "dome"},
  {LMK_SCNR, "radar_scanner"},
  {LMK_WNDL, "windmill"},
  {LMK_SPIR, "spire"},
  {LMK_MNRT, "minaret"},
  {NOCAT, ""}
};

const ShpSTR_t ShpSTR[] = {
  {BEACON, "beacon"},
  {BEACON, "pile"},
  {BEACON, "lattice"},
  {PILLAR, "pillar"},
  {SPAR, "spar"},
  {CAN, "can"},
  {CONI, "conical"},
  {SPHERI, "spherical"},
  {BARREL, "barrel"},
  {FLOAT, "float"},
  {SUPER, "super-buoy"},
  {BUOYANT, "buoyant"},
  {CAIRN, "cairn"},
  {PILE, "pile"},
  {LATTICE, "lattice"},
  {TOWER, "tower"},
  {STAKE, "stake"},
  {STAKE, "post"},
  {STAKE, "pole"},
  {PERCH, "perch"},
  {BOLLARD, "bollard"},
  {DOLPHIN, "dolphin"},
  {DEVDOLPHIN, "deviation_dolphin"},
  {POST, "post"},
  {SIGNAL, "signal_station"},
  {HOUSE, "lighthouse"},
  {MAJOR, "light_major"},
  {MINOR, "light_minor"},
  {CHIMNEY, "chimney"},
  {DISH, "dish_aerial"},
  {FLAGSTAFF, "flagstaff"},
  {FLARESTACK, "flare_stack"},
  {MONUMENT, "monument"},
  {LANDTOWER, "landtower"},
  {WINDMOTOR, "windmotor"},
  {WATERTOWER, "water_tower"},
  {MAST, "mast"},
  {WINDSOCK, "wind_sock"},
  {LANDCROSS, "cross"},
  {DOME, "dome"},
  {RADAR, "radar"},
  {WINDMILL, "windmill"},
  {SPIRE, "spire"},
  {PLATFORM, "platform"},
  {FPSO, "fpso"},
  {UNKSHP, ""}
};

const ColSTR_t ColSTR[] = {
  {WHITE, "white"},
  {RED, "red"},
  {ORANGE, "orange"},
  {AMBER, "amber"},
  {YELLOW, "yellow"},
  {GREEN, "green"},
  {BLUE, "blue"},
  {VIOLET, "violet"},
  {BLACK, "black"},
  {GREY, "grey"},
  {BROWN, "brown"},
  {MAGENTA, "magenta"},
  {PINK, "pink"},
  {UNKCOL, ""}
};

const ColSTR_t ColMAP[] = {
  {WHITE, "W"},
  {RED, "R"},
  {ORANGE, "Or"},
  {AMBER, "Am"},
  {YELLOW, "Y"},
  {GREEN, "G"},
  {BLUE, "Bu"},
  {VIOLET, "Vi"},
  {BLACK, "B"},
  {GREY, "Gr"},
  {BROWN, "Bn"},
  {MAGENTA, "Mg"},
  {PINK, "Pk"},
  {UNKCOL, ""}
};

const AtlSTR_t AtlSTR[] = {
  {COL, "colour"},
  {CHR, "character"},
  {GRP, "group"},
  {SEQ, "sequence"},
  {PER, "period"},
  {LIT, "category"},
  {BEG, "sector_start"},
  {END, "sector_end"},
  {RAD, "radius"},
  {HGT, "height"},
  {RNG, "range"},
  {VIS, "visibility"},
  {EXH, "exhibition"},
  {ORT, "orientation"},
  {MLT, "multiple"},
  {ALT, "colour"},
  {OBJ, "object"},
  {NOATL, ""}
};
  
  const ChrMAP_t ChrMAP[] = {
  {FIXED, "F"},
  {FLASH, "Fl"},
  {LFLASH, "LFl"},
  {QUICK, "Q"},
  {VQUICK, "VQ"},
  {UQUICK, "UQ"},
  {ISOPHASED, "Iso"},
  {OCCULTING, "Oc"},
  {IQUICK, "IQ"},
  {IVQUICK, "IVQ"},
  {IUQUICK, "IUQ"},
  {MORSE, "Mo"},
  {FIXED + FLASH, "FFl"},
  {FLASH + LFLASH, "FlLFl"},
  {OCCULTING + FLASH, "OcFl"},
  {FIXED + OCCULTING, "FOc"},
  {FIXED + LFLASH, "FLFl"},
  {QUICK + LFLASH, "Q+LFl"},
  {VQUICK + LFLASH, "VQ+LFl"},
  {UQUICK + LFLASH, "UQ+LFl"},
  {ALTERNATING, "Al"},
  {ALTERNATING + OCCULTING, "Al.Oc"},
  {ALTERNATING + LFLASH, "Al.LFl"},
  {ALTERNATING + FLASH, "Al.Fl"},
  {ALTERNATING + FIXED, "Al.F"},
  {ALTERNATING + FIXED + FLASH, "Al.FFl"},
  {ALTERNATING + ISOPHASED, "Al.Iso"},
  {UNKCHR, ""}
};

const VisSTR_t VisSTR[] = {
  {HIGH, "high"},
  {LOW, "low"},
  {FAINT, "faint"},
  {INTEN, "intensified"},
  {UNINTEN, "unintensified"},
  {REST, "restricted"},
  {OBS, "obscured"},
  {PARTOBS, "part_obscured"},
  {UNKVIS, ""}
};

const LitSTR_t LitSTR[] = {
  {VERT, "vertical"},
  {HORIZ, "horizontal"},
  {DIR, "directional"},
  {UPPER, "upper"},
  {LOWER, "lower"},
  {LEAD, "leading"},
  {REAR, "rear"},
  {FRONT, "front"},
  {AERO, "aero"},
  {AIROBS, "air_obstruction"},
  {FOGDET, "fog_detector"},
  {FLOOD, "floodlight"},
  {STRIP, "striplight"},
  {SUBS, "subsidairy"},
  {SPOT, "spotlight"},
  {MOIRE, "moire"},
  {EMERG, "emergency"},
  {BEAR, "bearing"},
  {UNKLIT, ""}
};

const LitSTR_t LitMAP[] = {
  {VERT, "vert"},
  {HORIZ, "hor"},
  {UPPER, "upper"},
  {LOWER, "lower"},
  {LEAD, "leading"},
  {REAR, "rear"},
  {FRONT, "front"},
  {UNKLIT, ""}
};

const ExhSTR_t ExhSTR[] = {
  {H24, "24h"},
  {DAY, "day"},
  {NIGHT, "night"},
  {FOG, "fog"},
  {WARN, "warning"},
  {STORM, "storm"},
  {UNKEXH, ""}
};

const PatSTR_t PatSTR[] = {
  {HSTR, "horizontal"},
  {VSTR, "vertical"},
  {DIAG, "diagonal"},
  {SQUARED, "squared"},
  {BORDER, "border"},
  {CROSS, "cross"},
  {SALT, "saltire"},
  {NOPAT, ""}
};

const PatSTR_t PatMAP[] = {
  {HSTR, "H"},
  {VSTR, "V"},
  {DIAG, "D"},
  {SQUARED, "S"},
  {BORDER, "B"},
  {CROSS, "C"},
  {SALT, "X"},
  {NOPAT, ""}
};

const TopSTR_t TopSTR[] = {
  {CYL, "cylinder"},
  {CONE, "cone, point up"},
  {CONE_INV, "cone, point down"},
  {SPHERE, "sphere"},
  {X_SHAPE, "x-shape"},
  {NORTH, "2 cones up"},
  {SOUTH, "2 cones down"},
  {EAST, "2 cones base together"},
  {WEST, "2 cones point together"},
  {SPHERES2, "2 spheres"},
  {BOARD, "board"},
  {DIAMOND, "diamond"},
  {CIRCLE, "circle"},
  {TRIANGLE, "triangle, point up"},
  {TRIANGLE_INV, "triangle, point down"},
  {SQUARE, "square"},
  {NOTOP, ""}
};

const RtbSTR_t RtbSTR[] = {
  {RACON, "racon"},
  {RAMARK, "ramark"},
  {LEADING, "leading"},
  {NORTB, ""}
};

const RtbSTR_t RtbMAP[] = {
  {RACON, "Racon"},
  {RAMARK, "Ramark"},
  {NORTB, ""}
};

const FogSTR_t FogSTR[] = {
  {UNKFOG, "yes"},
  {HORN, "horn"},
  {SIREN, "siren"},
  {DIA, "diaphone"},
  {BELL, "bell"},
  {WHIS, "whistle"},
  {GONG, "gong"},
  {EXPLOS, "explosion"},
  {NOFOG, ""}
};

const FogSTR_t FogMAP[] = {
  {UNKFOG, ""},
  {HORN, "Horn"},
  {SIREN, "Siren"},
  {DIA, "Dia"},
  {BELL, "Bell"},
  {WHIS, "Whis"},
  {GONG, "Gong"},
  {EXPLOS, "Explos"},
  {NOFOG, ""}
};

const CatSTR_t SisMAP[] = {
  {SIS_PTCL, "(Port Control)"},
  {SIS_PTED, "(Traffic)"},
  {SIS_IPT, "(INT)"},
  {SIS_BRTH, ""},
  {SIS_DOCK, ""},
  {SIS_LOCK, "(Lock)"},
  {SIS_FBAR, ""},
  {SIS_BRDG, "(Bridge)"},
  {SIS_DRDG, ""},
  {SIS_TRFC, "(Traffic)"},
  {SIS_DNGR, "(Danger)"},
  {SIS_OBST, ""},
  {SIS_CABL, ""},
  {SIS_MILY, "(Firing)"},
  {SIS_WTHR, "(Weather)"},
  {SIS_STRM, "(Storm)"},
  {SIS_ICE, "(Ice)"},
  {SIS_TIME, "(Time)"},
  {SIS_TIDE, "(Tide)"},
  {SIS_TSTM, "(Stream)"},
  {SIS_TGAG, ""},
  {SIS_TSCL, ""},
  {SIS_DIVE, ""},
  {SIS_LGAG, ""},
  {NOCAT, ""}
};

const StsSTR_t StsSTR[] = {
  {PERM, "permanent"},
  {OCC, "occasional"},
  {REC, "recommended"},
  {NIU, "not_in_use"},
  {INT, "intermittent"},
  {RESV, "reserved"},
  {TEMP, "tempory"},
  {PRIV, "private"},
  {MAND, "mandatory"},
  {DEST, "destroyed"},
  {EXT, "extinguished"},
  {ILLUM, "illuminated"},
  {HIST, "historic"},
  {PUB, "public"},
  {SYNC, "synchronized"},
  {WATCH, "watched"},
  {UNWAT, "unwatched"},
  {DOUBT, "existence_doubtful"},
  {UNKSTS, ""}
};

const CnsSTR_t CnsSTR[] = {
  {BRICK, "masonry"},
  {CONC, "concreted"},
  {BOULD, "boulders"},
  {HSURF, "hard_surfaced"},
  {USURF, "unsurfaced"},
  {WOOD, "wooden"},
  {METAL, "metal"},
  {GLAS, "grp"},
  {PAINT, "painted"},
  {UNKCNS, ""}
};

const ConSTR_t ConSTR[] = {
  {CONSP, "conspicuous"},
  {NCONS, "not_conspicuous"},
  {REFL, "reflector"},
  {UNKCON, ""}
};

const FncSTR_t FncSTR[] = {
  {HMO, "harbour-master"},
  {CSTM, "customs"},
  {HLTH, "health"},
  {HOSP, "hospital"},
  {POFF, "post_office"},
  {HOTEL, "hotel"},
  {RWSTA, "railway_station"},
  {POLICE, "police_station"},
  {WPOL, "water-police_station"},
  {PILO, "pilot_office"},
  {PILL, "pilot_lookout"},
  {BANK, "bank"},
  {DCHQ, "district_control"},
  {TRNS, "transit_shed"},
  {FACT, "factory"},
  {PWRS, "power_station"},
  {ADMIN, "administrative"},
  {EDUC, "educational"},
  {CHCH, "church"},
  {CHPL, "chapel"},
  {TMPL, "temple"},
  {PGDA, "pagoda"},
  {SHSH, "shinto_shrine"},
  {BTMP, "buddhist_temple"},
  {MOSQ, "mosque"},
  {MRBT, "marabout"},
  {LOOK, "lookout"},
  {COMS, "communication"},
  {TV, "television"},
  {RADO, "radio"},
  {RADR, "radar"},
  {LSUP, "light_support"},
  {MWAV, "microwave"},
  {COOL, "cooling"},
  {OBSV, "observation"},
  {TIMB, "time_ball"},
  {CLK, "clock"},
  {CTRL, "control"},
  {AMOR, "airship_mooring"},
  {STAD, "stadium"},
  {BUSS, "bus_station"},
  {UNKFNC, ""}
};

const AddSTR_t AddSTR[] = {
  {TBRD, "top_board"},
	{BBRD, "bottom_board"},
	{RTRI, "right_triangle"},
	{LTRI, "left_triangle"},
	{BTRI, "bottom_triangle"},
	{NOADD, ""}
};

Key_t getkey(Map_t map[], char *val) {
  int i;
  if (val != NULL) {
    for (i = 0; map[i].key != NULKEY; i++) {
      if (strcmp(map[i].val, val) == 0) {
        return (map[i].key);
      }
    }
  }
  return (0);
}

char *getval(Map_t map[], Key_t key) {
  int i;
  for (i = 0; map[i].key != NULKEY; i++) {
    if (map[i].key == key) {
      return (map[i].val);
    }
  }
  return ("");
}