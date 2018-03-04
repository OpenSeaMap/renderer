/* Copyright 2012 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

/* Conversion of S-57 ATTL & ATVL values to OSeaM attribute tag values */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iconv.h>
#include <unistd.h>

#include "s57val.h"

typedef const struct {
  Enum_t atvl;
  char *val;
} s57val_t;

typedef const struct {
  Attl_t attl;
  Conv_t type;
  s57val_t *val;
} s57key_t;
 
s57val_t bcnshp[] = { [BCN_UNKN]={0, ""}, [BCN_STAK]={1, "stake"}, [BCN_WTHY]={2, "withy"}, [BCN_TOWR]={3, "tower"}, [BCN_LATT]={4, "lattice"}, [BCN_PILE]={5, "pile"}, [BCN_CARN]={6, "cairn"}, [BCN_BUOY]={7, "buoyant"}, [BCN_POLE]={1, "pole"}, [BCN_PRCH]={1, "perch"}, [BCN_POST]={1, "post"}, {0, NULL} };
s57val_t buishp[] = { {1, "non-specific"}, {2, "tower"}, {3, "spire"}, {4, "cupola"}, {5, "high-rise"}, {6, "pyramid"}, {7, "cylindrical"}, {8, "spherical"}, {9, "cubic"}, {0, NULL} };
s57val_t boyshp[] = { [BOY_UNKN]={0, ""}, [BOY_CONE]={1, "conical"}, [BOY_CAN]={2, "can"}, [BOY_SPHR]={3, "spherical"}, [BOY_PILR]={4, "pillar"}, [BOY_SPAR]={5, "spar"}, [BOY_BARL]={6, "barrel"},
  [BOY_SUPR]={7, "super-buoy"}, [BOY_ICE]={8, "ice_buoy"}, {0, NULL} };
s57val_t catair[] = { {1, "military"}, {2, "civil"}, {3, "military_heliport"}, {4, "civil_heliport"}, {5, "glider"}, {6, "small_planes"}, {8, "emergency"}, {0, NULL} };
s57val_t catach[] = { {1, "unrestricted"}, {2, "deep_water"}, {3, "tanker"}, {4, "explosives"}, {5, "quarantine"}, {6, "seaplane"}, {7, "small_craft"}, {8, "small_craft_mooring"},
  {9, "24_hour"}, {10, "limited_period"}, {0, NULL} };
s57val_t catbrg[] = { {1, "fixed"}, {2, "opening"}, {3, "swing"}, {4, "lifting"}, {5, "bascule"}, {6, "pontoon"}, {7, "drawbridge"}, {8, "transporter"}, {9, "footbridge"}, {10, "viaduct"},
  {11, "aqueduct"}, {12, "suspension"}, {0, NULL} };
s57val_t catbua[] = { {1, "urban"}, {2, "settlement"}, {3, "village"}, {4, "town"}, {5, "city"}, {6, "holiday_village"}, {0, NULL} };
s57val_t catcbl[] = { {1, "power"}, {2, "telecom"}, {3, "transmission"}, {4, "telephone"}, {5, "telegraph"}, {6, "mooring"}, {7, "optical"}, {8, "ferry"}, {0, NULL} };
s57val_t catcan[] = { {1, "transportation"}, {2, "drainage"}, {3, "irrigation"}, {0, NULL} };
s57val_t catcam[] = { {1, "north"}, {2, "east"}, {3, "south"}, {4, "west"}, {0, NULL} };
s57val_t catchp[] = { {1, "custom"}, {2, "border"}, {0, NULL} };
s57val_t catcoa[] = { {1, "steep"}, {2, "flat"}, {3, "sandy"}, {4, "stony"}, {5, "shingly"}, {6, "glacier"}, {7, "mangrove"}, {8, "marshy"}, {9, "coral_reef"}, {10, "ice"}, {11, "shelly"}, {0, NULL} };
s57val_t catctr[] = { {1, "triangulation"}, {2, "observation"}, {3, "fixed"}, {4, "benchmark"}, {5, "boundary"}, {6, "horizontal_main"}, {7, "horizontal_secondary"}, {0, NULL} };
s57val_t catcon[] = { {1, "aerial"}, {2, "belt"}, {0, NULL} };
s57val_t catcov[] = { {1, "coverage"}, {2, "no_coverage"}, {0, NULL} };
s57val_t catcrn[] = { {1, "non-specific"}, {2, "container"}, {3, "sheerlegs"}, {4, "travelling"}, {5, "a-frame"}, {0, NULL} };
s57val_t catdam[] = { {1, "weir"}, {2, "dam"}, {3, "flood_barrage"}, {0, NULL} };
s57val_t catdis[] = { [DIS_UNKN]={0, ""}, [DIS_NINS]={1, "not_installed"}, [DIS_POLE]={2, "pole"}, [DIS_BORD]={3, "board"}, [DIS_SHUK]={4, "unknown_shape"}, {0, NULL} };
s57val_t catdoc[] = { {1, "tidal"}, {2, "non-tidal"}, {0, NULL} };
s57val_t catdpg[] = { {1, "general"}, {2, "chemical"}, {3, "nuclear"}, {4, "explosives"}, {5, "spoil"}, {6, "vessel"}, {0, NULL} };
s57val_t catfnc[] = { {1, "fence"}, {2, "muir"}, {3, "hedge"}, {4, "wall"}, {0, NULL} };
s57val_t catfry[] = { {1, "free"}, {2, "cable"}, {3, "ice"}, {4, "swinging_wire_ferry"}, {0, NULL} };
s57val_t catfif[] = { {1, "stake"}, {2, "trap"}, {3, "weir"}, {4, "tunny"}, {0, NULL} };
s57val_t catfog[] = { [FOG_UNKN]={0, ""}, [FOG_EXPL]={1, "explosive"}, [FOG_DIA]={2, "diaphone"}, [FOG_SIRN]={3, "siren"}, [FOG_NAUT]={4, "nautophone"}, [FOG_REED]={5, "reed"},
  [FOG_TYPH]={6, "tyfon"}, [FOG_BELL]={7, "bell"}, [FOG_WHIS]={8, "whistle"}, [FOG_GONG]={9, "gong"}, [FOG_HORN]={10, "horn"}, {0, NULL} };
s57val_t catfor[] = { {1, "castle"}, {2, "fort"}, {3, "battery"}, {4, "blockhouse"}, {5, "martello_tower"}, {6, "redoubt"}, {0, NULL} };
s57val_t catgat[] = { {1, "general"}, {2, "flood_barrage"}, {3, "caisson"}, {4, "lock"}, {5, "dyke"}, {6, "sluice"}, {0, NULL} };
s57val_t cathaf[] = { {1, "roro"}, {3, "ferry"}, {4, "fishing"}, {5, "marina"}, {6, "naval"}, {7, "tanker"}, {8, "passenger"}, {9, "shipyard"}, {10, "container"}, {11, "bulk"},
  {12, "syncrolift"}, {13, "straddle_carrier"}, {14, "lay_up"}, {15, "timber"}, {16, "service_repair"}, {17, "quarantine"}, {18, "seaplane"}, {19, "cargo"}, {20, "offshore_support"},
  {21, "port_support_base"}, {99, "marina_no_facilities"}, {0, NULL} };
s57val_t cathlk[] = { {1, "floating_restaurant"}, {2, "historic"}, {3, "museum"}, {4, "accommodation"}, {5, "floating_breakwater"}, {6, "casino_boat"}, {0, NULL} };
s57val_t catice[] = { {1, "fast"}, {2, "sea"}, {3, "growler"}, {4, "pancake"}, {5, "glacier"}, {6, "peak"}, {7, "pack"}, {8, "polar"}, {0, NULL} };
s57val_t catinb[] = { {1, "calm"}, {2, "sbm"}, {0, NULL} };
s57val_t catlnd[] = { {1, "fen"}, {2, "marsh"}, {3, "bog"}, {4, "heathland"}, {5, "mountain"}, {6, "lowlands"}, {7, "canyon"}, {8, "paddy"}, {9, "agricultural"}, {10, "savanna"}, {11, "parkland"},
  {12, "swamp"}, {13, "landslide"}, {14, "lava"}, {15, "salt_pan"}, {16, "moraine"}, {17, "crater"}, {18, "cave"}, {19, "rock_pinnacle"}, {20, "cay"}, {0, NULL} };
s57val_t catlmk[] = { [LMK_UNKN]={0, ""}, [LMK_CARN]={1, "cairn"}, [LMK_CMTY]={2, "cemetery"}, [LMK_CHMY]={3, "chimney"}, [LMK_DISH]={4, "dish_aerial"}, [LMK_FLAG]={5, "flagstaff"}, [LMK_FLAR]={6, "flare_stack"},
  [LMK_MAST]={7, "mast"}, [LMK_WNDS]={8, "windsock"}, [LMK_MNMT]={9, "monument"}, [LMK_CLMN]={10, "column"}, [LMK_MEML]={11, "memorial"}, [LMK_OBLK]={12, "obelisk"}, [LMK_STAT]={13, "statue"},
  [LMK_CROS]={14, "cross"}, [LMK_DOME]={15, "dome"}, [LMK_RADR]={16, "radar_scanner"}, [LMK_TOWR]={17, "tower"}, [LMK_WNDM]={18, "windmill"}, [LMK_WNDG]={19, "windmotor"}, [LMK_SPIR]={20, "spire"},
  [LMK_BLDR]={21, "boulder"}, [LMK_MNRT]={22, "minaret"}, [LMK_WTRT]={23, "water_tower"}, {0, NULL} };
s57val_t catlam[] = { {1, "port"}, {2, "starboard"}, {3, "preferred_channel_starboard"}, {4, "preferred_channel_port"}, {5, "waterway_right"}, {6, "waterway_left"}, {7, "channel_right"}, {8, "channel_leftt"},
  {9, "waterway_separation"}, {10, "channel_separation"}, {11, "channel_right_bank"}, {12, "channel_left_bank"}, {13, "crossover_right"}, {14, "crossover_left"}, {15, "danger_right"}, {16, "danger_left"},
  {17, "turnoff_right"}, {18, "turnoff_left"}, {19, "junction_right"}, {20, "junction_left"}, {21, "harbour_right"}, {22, "harbour_left"}, {23, "bridge_pier"}, {0, NULL} };
s57val_t catlit[] = { [LIT_NONE]={0, ""}, [LIT_DIR]={1, "directional"}, [LIT_LEAD]={4, "leading"}, [LIT_AERO]={5, "aero"}, [LIT_AIR]={6, "air_obstruction"}, [LIT_FOG]={7, "fog_detector"}, [LIT_FLDL]={8, "floodlight"}, [LIT_STRP]={9, "strip_light"}, [LIT_SUBS]={10, "subsidiary"}, [LIT_SPOT]={11, "spotlight"}, [LIT_FRNT]={12, "front"}, [LIT_REAR]={13, "rear"}, [LIT_LOWR]={14, "lower"}, [LIT_UPPR]={15, "upper"}, [LIT_MOIR]={16, "moire"}, [LIT_EMRG]={17, "emergency"}, [LIT_BRNG]={18, "bearing"}, [LIT_HORI]={19, "horizontal"}, [LIT_VERT]={20, "vertical"}, {0, NULL} };
s57val_t catmfa[] = { {1, "crustaceans"}, {2, "oysters_mussels"}, {3, "fish"}, {4, "seaweed"}, {5, "pearl_culture"}, {0, NULL} };
s57val_t catmpa[] = { {1, "practice"}, {2, "torpedo"}, {3, "submarine"}, {4, "firing"}, {5, "mine-laying"}, {6, "small_arms"}, {0, NULL} };
s57val_t catmor[] = { {1, "dolphin"}, {2, "deviation_dolphin"}, {3, "bollard"}, {4, "wall"}, {5, "pile"}, {6, "chain"}, {7, "buoy"}, {8, "shore_ropes"}, {9, "automatic"}, {10, "post"}, {11, "wire"}, {12, "cable"}, {0, NULL} };
s57val_t catnav[] = { {1, "clearing"}, {2, "transit"}, {3, "leading"}, {0, NULL} };
s57val_t catobs[] = { {1, "stump"}, {2, "wellhead"}, {3, "diffuser"}, {4, "crib"}, {5, "fish_haven"}, {6, "foul_area"}, {7, "foul_ground"}, {8, "ice_boom"}, {9, "ground_tackle"}, {10, "boom"}, {0, NULL} };
s57val_t catofp[] = { {1, "oil"}, {2, "production"}, {3, "observation"}, {4, "alp"}, {5, "salm"}, {6, "mooring"}, {7, "artificial_island"}, {8, "fpso"}, {9, "accommodation"}, {10, "nccb"}, {0, NULL} };
s57val_t catolb[] = { {1, "retention"}, {2, "floating"}, {0, NULL} };
s57val_t catple[] = { {1, "stake"}, {2, "snag"}, {3, "post"}, {4, "tripodal"}, {0, NULL} };
s57val_t catpil[] = { {1, "cruising_vessel"}, {2, "helicopter"}, {3, "from_shore"}, {0, NULL} };
s57val_t catpip[] = { {2, "outfall"}, {3, "intake"}, {4, "sewer"}, {5, "bubbler"}, {6, "supply"}, {0, NULL} };
s57val_t catpra[] = { {1, "quarry"}, {2, "mine"}, {3, "stockpile"}, {4, "power_station"}, {5, "refinery"}, {6, "timber_yard"}, {7, "factory"}, {8, "tank_farm"}, {9, "wind_farm"}, {10, "slag"}, {0, NULL} };
s57val_t catpyl[] = { {1, "power"}, {2, "telecom"}, {3, "aerial"}, {4, "bridge"}, {5, "bridge_pier"}, {0, NULL} };
s57val_t catqua[] = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"}, {6, "not_evaluated"}, {0, NULL} };
s57val_t catras[] = { {1, "surveillance"}, {2, "coast"}, {0, NULL} };
s57val_t catrtb[] = { [RTB_UNKN]={0, ""}, [RTB_RAMK]={1, "ramark"}, [RTB_RACN]={2, "racon"}, [RTB_LDG]={3, "leading"}, {0, NULL} };
s57val_t catros[] = { [ROS_UNKN]={0, ""}, [ROS_OMNI]={1, "omnidirectional"}, [ROS_DIRL]={2, "directional"}, [ROS_ROTP]={3, "rotating_pattern"}, [ROS_CNSL]={4, "consol"}, [ROS_RDF]={5, "rdf"}, [ROS_QTG]={6, "qtg"},
  [ROS_AERO]={7, "aeronautical"}, [ROS_DECA]={8, "decca"}, [ROS_LORN]={9, "loran"}, [ROS_DGPS]={10, "dgps"}, [ROS_TORN]={11, "toran"}, [ROS_OMGA]={12, "omega"}, [ROS_SYLD]={13, "syledis"}, [ROS_CHKA]={14, "chaika"},
  [ROS_PCOM]={15, "public_communication"}, [ROS_COMB]={16, "commercial_broadcast"}, [ROS_FACS]={17, "facsimile"}, [ROS_TIME]={18, "time_signal"}, [ROS_PAIS]={19, "ais"}, [ROS_SAIS]={20, "s-ais"}, [ROS_VAIS]={21, "v-ais"},
  [ROS_VANC]={22, "v-ais_north_cardinal"}, [ROS_VASC]={23, "v-ais_south_cardinal"}, [ROS_VAEC]={24, "v-ais_east_cardinal"}, [ROS_VAWC]={25, "v-ais_west_cardinal"}, [ROS_VAPL]={26, "v-ais_port_lateral"},
  [ROS_VASL]={27, "v-ais_starboard_lateral"}, [ROS_VAID]={28, "v-ais_isolated_danger"}, [ROS_VASW]={29, "v-ais_safe_water"}, [ROS_VASP]={30, "v-ais_special_purpose"}, [ROS_VAWK]={31, "v-ais_wreck"}, {0, NULL} };
s57val_t cattrk[] = { {1, "fixed_marks"}, {2, "no_fixed_marks"}, {0, NULL} };
s57val_t catrsc[] = { {1, "lifeboat"}, {2, "rocket"}, {3, "lifeboat_rocket"}, {4, "refuge_shipwrecked"}, {5, "refuge_intertidal"}, {6, "lifeboat_on_mooring"}, {7, "radio"}, {8, "first_aid"}, {0, NULL} };
s57val_t catrea[] = { {1, "safety"}, {2, "no_anchoring"}, {3, "no_fishing"}, {4, "nature_reserve"}, {5, "bird_sanctuary"}, {6, "game_reserve"}, {7, "seal_sanctuary"}, {8, "degaussing_range"}, {9, "military"},
  {10, "historic_wreck"}, {11, "inshore_traffic"}, {12, "navigational_aid_safety"}, {13, "stranding_danger"}, {14, "minefield"}, {15, "no_diving"}, {16, "to_be_avoided"}, {17, "prohibited"}, {18, "swimming"},
  {19, "waiting"}, {20, "research"}, {21, "dredging"}, {22, "fish_sanctuary"}, {23, "ecological_reserve"}, {24, "no_wake"}, {25, "swinging"}, {26, "water_skiing"}, {0, NULL} };
s57val_t catrod[] = { {1, "motorway"}, {2, "major_road"}, {3, "minor_road"}, {4, "track"}, {5, "major_street"}, {6, "minor_street"}, {7, "crossing"}, {8, "path"}, {0, NULL} };
s57val_t catrun[] = { {1, "aeroplane"}, {2, "helicopter"}, {0, NULL} };
s57val_t catsea[] = { {1, "general"}, {2, "gat"}, {3, "bank"}, {4, "deep"}, {5, "bay"}, {6, "trench"}, {7, "basin"}, {8, "mud_flats"}, {9, "reef"}, {10, "ledge"}, {11, "canyon"}, {12, "narrows"}, {13, "shoal"},
  {14, "knoll"}, {15, "ridge"}, {16, "seamount"}, {17, "pinnacle"}, {18, "abyssal_plain"}, {19, "plateau"}, {20, "spur"}, {21, "shelf"}, {22, "trough"}, {23, "saddle"}, {24, "abyssal_hills"}, {25, "apron"},
  {26, "archipelagic_apron"}, {27, "borderland"}, {28, "continental_margin"}, {29, "continental_rise"}, {30, "escarpment"}, {31, "fan"}, {32, "fracture_zone"}, {33, "gap"}, {34, "guyot"}, {35, "hill"}, {36, "hole"},
  {37, "levee"}, {38, "median valley"}, {39, "moat"}, {40, "mountains"}, {41, "peak"}, {42, "province"}, {43, "rise"}, {44, "sea channel"}, {45, "seamount chain"}, {46, "shelf-edge"}, {47, "sill"}, {48, "slope"},
  {49, "terrace"}, {50, "valley"}, {51, "canal"}, {52, "lake"}, {53, "river"}, {54, "reach"}, {0, NULL} };
s57val_t catslc[] = { {1, "breakwater"}, {2, "groyne"}, {3, "mole"}, {4, "pier"}, {5, "promenade_pier"}, {6, "wharf"}, {7, "training_wall"}, {8, "rip_rap"}, {9, "revetment"}, {10, "sea_wall"},
  {11, "landing_steps"}, {12, "ramp"}, {13, "slipway"}, {14, "fender"}, {15, "solid_face_wharf"}, {16, "open_face_wharf"}, {17, "log_ramp"}, {18, "lock_wall"}, {0, NULL} };
s57val_t  catsit[] = { [SIT_UNKN]={0, ""}, [SIT_PRTC]={1, "port_control"}, [SIT_PRTE]={2, "port_entry_departure"}, [SIT_IPT]={3, "ipt"}, [SIT_BRTH]={4, "berthing"}, [SIT_DOCK]={5, "dock"},
  [SIT_LOCK]={6, "lock"}, [SIT_FLDB]={7, "flood_barrage"}, [SIT_BRDG]={8, "bridge_passage"}, [SIT_DRDG]={9, "dredging"}, [SIT_TCLT]={10, "traffic_control_light"}, {0, NULL} };
s57val_t  catsiw[] = { [SIW_UNKN]={0, ""}, [SIW_DNGR]={1, "danger"}, [SIW_OBST]={2, "maritime_obstruction"}, [SIW_CABL]={3, "cable"}, [SIW_MILY]={4, "military"}, [SIW_DSTR]={5, "distress"},
  [SIW_WTHR]={6, "weather"}, [SIW_STRM]={7, "storm"}, [SIW_ICE]={8, "ice"}, [SIW_TIME]={9, "time"}, [SIW_TIDE]={10, "tide"}, [SIW_TSTR]={11, "tidal_stream"}, [SIW_TIDG]={12, "tide_gauge"},
  [SIW_TIDS]={13, "tide_scale"}, [SIW_DIVE]={14, "diving"}, [SIW_WTLG]={15, "water_level_gauge"}, [SIW_VRCL]={16, "vertical_clearance"}, [SIW_DPTH]={18, "depth"}, {0, NULL} };
s57val_t catsil[] = { {1, "silo"}, {2, "tank"}, {3, "grain_elevator"}, {4, "water_tower"}, {0, NULL} };
s57val_t catslo[] = { {1, "cutting"}, {2, "embankment"}, {3, "dune"}, {4, "hill"}, {5, "pingo"}, {6, "cliff"}, {7, "scree"}, {0, NULL} };
s57val_t catscf[] = { [SCF_UNKN]={0, ""}, [SCF_VBTH]={1, "visitor_berth"}, [SCF_CLUB]={2, "nautical_club"}, [SCF_BHST]={3, "boat_hoist"}, [SCF_SMKR]={4, "sailmaker"}, [SCF_BTYD]={5, "boatyard"},
  [SCF_INN]={6, "public_inn"}, [SCF_RSRT]={7, "restaurant"}, [SCF_CHDR]={8, "chandler"}, [SCF_PROV]={9, "provisions"}, [SCF_DCTR]={10, "doctor"}, [SCF_PHRM]={11, "pharmacy"}, [SCF_WTRT]={12, "water_tap"},
  [SCF_FUEL]={13, "fuel_station"}, [SCF_ELEC]={14, "electricity"}, [SCF_BGAS]={15, "bottle_gas"}, [SCF_SHWR]={16, "showers"}, [SCF_LAUN]={17, "laundrette"}, [SCF_WC]={18, "toilets"},
  [SCF_POST]={19, "post_box"},  [SCF_TELE]={20, "telephone"}, [SCF_REFB]={21, "refuse_bin"}, [SCF_CARP]={22, "car_park"}, [SCF_BTPK]={23, "boats_trailers_park"}, [SCF_CRVN]={24, "caravan_site"},
  [SCF_CAMP]={25, "camping_site"},  [SCF_PMPO]={26, "pump-out"}, [SCF_EMRT]={27, "emergency_telephone"}, [SCF_SLPW]={28, "slipway"}, [SCF_VMOR]={29, "visitors_mooring"},
  [SCF_SCRB]={30, "scrubbing_berth"}, [SCF_PCNC]={31, "picnic_area"},  [SCF_MECH]={32, "mechanics_workshop"}, [SCF_SECS]={33, "security_service"}, {0, NULL} };
s57val_t catspm[] = { {1, "firing_danger_area"}, {2, "target"}, {3, "marker_ship"}, {4, "degaussing_range"}, {5, "barge"}, {6, "cable"}, {7, "spoil_ground"}, {8, "outfall"}, {9, "odas"}, {10, "recording"},
  {11, "seaplane_anchorage"}, {12, "recreation_zone"}, {13, "private"}, {14, "mooring"}, {15, "lanby"}, {16, "leading"}, {17, "measured_distance"}, {18, "notice"}, {19, "tss"}, {20, "no_anchoring"}, {21, "no_berthing"},
  {22, "no_overtaking"}, {23, "no_two-way_traffic"}, {24, "reduced_wake"}, {25, "speed_limit"}, {26, "stop"}, {27, "warning"}, {28, "sound_ship_siren"}, {29, "restricted_vertical_clearance"}, {30, "maximum_vessel_draught"},
  {31, "restricted_horizontal_clearance"}, {32, "strong_current"}, {33, "berthing"}, {34, "overhead_power_cable"}, {35, "channel_edge_gradient"}, {36, "telephone"}, {37, "ferry_crossing"}, {38, "marine_traffic_lights"},
  {39, "pipeline"}, {40, "anchorage"}, {41, "clearing"}, {42, "control"}, {43, "diving"}, {44, "refuge_beacon"}, {45, "foul_ground"}, {46, "yachting"}, {47, "heliport"}, {48, "gps"}, {49, "seaplane_landing"},
  {50, "no_entry"}, {51, "work_in_progress"}, {52, "unknown_purpose"}, {53, "wellhead"}, {54, "channel_separation"}, {55, "marine_farm"}, {56, "artificial_reef, "}, {0, NULL} };
s57val_t cattss[] = { {1, "imo_adopted"}, {2, "not_imo_adopted"}, {0, NULL} };
s57val_t catveg[] = { {1, "grass"}, {2, "paddy"}, {3, "bush"}, {4, "deciduous_wood"}, {5, "coniferous_wood"}, {6, "wood"}, {7, "mangroves"}, {8, "park"}, {9, "parkland"}, {10, "mixed_crops"}, {11, "reed"},
  {12, "moss"}, {13, "tree"}, {14, "evergreen_tree"}, {15, "coniferous_tree"}, {16, "palm_tree"}, {17, "nipa_palm_tree"}, {18, "casuarina_tree"}, {19, "eucalypt_tree"}, {20, "deciduous_tree"}, {21, "mangrove_tree"},
  {22, "filao_tree"}, {0, NULL} };
s57val_t catwat[] = { {1, "breakers"}, {2, "eddies"}, {3, "overfalls"}, {4, "tide_rips"}, {5, "bombora"}, {0, NULL} };
s57val_t catwed[] = { {1, "kelp"}, {2, "sea_weed"}, {3, "sea_grass"}, {4, "saragasso"}, {0, NULL} };
s57val_t catwrk[] = { {1, "non-dangerous"}, {2, "dangerous"}, {3, "distributed_remains"}, {4, "mast_showing"}, {5, "hull_showing"}, {0, NULL} };
s57val_t catzoc[] = { {1, "a1"}, {2, "a2"}, {3, "b"}, {4, "c"}, {5, "d"}, {6, "u"}, {0, NULL} };
s57val_t colour[] = { [COL_UNK]={0, ""}, [COL_WHT]={1, "white"}, [COL_BLK]={2, "black"}, [COL_RED]={3, "red"}, [COL_GRN]={4, "green"}, [COL_BLU]={5, "blue"}, [COL_YEL]={6, "yellow"}, [COL_GRY]={7, "grey"},
  [COL_BRN]={8, "brown"}, [COL_AMB]={9, "amber"}, [COL_VIO]={10, "violet"}, [COL_ORG]={11, "orange"}, [COL_MAG]={12, "magenta"}, [COL_PNK]={13, "pink"}, {0, NULL} };
s57val_t colpat[] = { [PAT_UNKN]={0, ""}, [PAT_HORI]={1, "horizontal"}, [PAT_VERT]={2, "vertical"}, [PAT_DIAG]={3, "diagonal"}, [PAT_SQUR]={4, "squared"}, [PAT_STRP]={5, "stripes"}, [PAT_BRDR]={6, "border"},
  [PAT_CROS]={7, "cross"}, [PAT_SALT]={8, "saltire"}, {0, NULL} };
s57val_t condtn[] = { {1, "under_construction"}, {2, "ruined"}, {3, "under_reclamation"}, {4, "wingless"}, {5, "planned_construction"}, {0, NULL} };
s57val_t conrad[] = { {1, "conspicuous"}, {2, "not_conspicuous"}, {3, "reflector"}, {0, NULL} };
s57val_t convis[] = { {1, "conspicuous"}, {2, "not_conspicuous"}, {0, NULL} };
s57val_t dunits[] = { {1, "metres"}, {2, "fathoms_feet"}, {3, "fathoms"}, {4, "fathoms_fractions"}, {0, NULL} };
s57val_t exclit[] = { {1, "24h"}, {2, "day"}, {3, "fog"}, {4, "night"}, {5, "warning"}, {6, "storm"}, {0, NULL} };
s57val_t expsou[] = { {1, "within"}, {2, "shoaler"}, {3, "deeper"}, {0, NULL} };
s57val_t functn[] = { [FNC_UNKN]={0, ""}, [FNC_HBRM]={2, "harbour_master"}, [FNC_CSTM]={3, "customs"}, [FNC_HLTH]={4, "health"}, [FNC_HOSP]={5, "hospital"}, [FNC_POST]={6, "post_office"}, [FNC_HOTL]={7, "hotel"},
  [FNC_RAIL]={8, "railway_station"}, [FNC_POLC]={9, "police_station"}, [FNC_WPOL]={10, "water-police_station"}, [FNC_PILO]={11, "pilot_office"}, [FNC_PILL]={12, "pilot_lookout"}, [FNC_BANK]={13, "bank"},
  [FNC_DIST]={14, "district_control"}, [FNC_TRNS]={15, "transit_shed"}, [FNC_FCTY]={16, "factory"}, [FNC_POWR]={17, "power_station"}, [FNC_ADMIN]={18, "administrative"}, [FNC_EDUC]={19, "educational"},
  [FNC_CHCH]={20, "church"}, [FNC_CHPL]={21, "chapel"}, [FNC_TMPL]={22, "temple"}, [FNC_PGDA]={23, "pagoda"}, [FNC_SHSH]={24, "shinto_shrine"}, [FNC_BTMP]={25, "buddhist_temple"}, [FNC_MOSQ]={26, "mosque"},
  [FNC_MRBT]={27, "marabout"}, [FNC_LOOK]={28, "lookout"}, [FNC_COMM]={29, "communication"}, [FNC_TV]={30, "television"}, [FNC_RADO]={31, "radio"}, [FNC_RADR]={32, "radar"}, [FNC_LGHT]={33, "light_support"},
  [FNC_MCWV]={34, "microwave"}, [FNC_COOL]={35, "cooling"}, [FNC_OBS]={36, "observation"}, [FNC_TMBL]={37, "time_ball"}, [FNC_CLOK]={38, "clock"}, [FNC_CTRL]={39, "control"}, [FNC_ASHM]={40, "airship_mooring"},
  [FNC_STAD]={41, "stadium"}, [FNC_BUSS]={42, "bus_station"}, {0, NULL} };
s57val_t hunits[] = { [UNI_UNKN]={0, ""}, [UNI_MTR]={1, "metres"}, [UNI_FEET]={2, "feet"}, [UNI_KMTR]={3, "kilometres"}, [UNI_HMTR]={4, "hectometres"}, [UNI_SMIL]={5, "statute_miles"}, [UNI_NMIL]={6, "nautical_miles"}, {0, NULL} };
s57val_t jrsdtn[] = { {1, "international"}, {2, "national"}, {3, "national_sub-division"}, {0, NULL} };
s57val_t litchr[] = { [CHR_UNKN]={0, ""}, [CHR_F]={1, "F"}, [CHR_FL]={2, "Fl"}, [CHR_LFL]={3, "LFl"}, [CHR_Q]={4, "Q"}, [CHR_VQ]={5, "VQ"}, [CHR_UQ]={6, "UQ"}, [CHR_ISO]={7, "Iso"}, [CHR_OC]={8, "Oc"},
  [CHR_IQ]={9, "IQ"}, [CHR_IVQ]={10, "IVQ"}, [CHR_IUQ]={11, "IUQ"}, [CHR_MO]={12, "Mo"}, [CHR_FFL]={13, "FFl"}, [CHR_FLLFL]={14, "FlLFl"}, [CHR_OCFL]={15, "OcFl"}, [CHR_FLFL]={16, "FLFl"}, [CHR_ALOC]={17, "Al.Oc"},
  [CHR_ALLFL]={18, "Al.LFl"}, [CHR_ALFL]={19, "Al.Fl"}, [CHR_ALGR]={20, "Al.Gr"}, [CHR_QLFL]={25, "Q+LFl"}, [CHR_VQLFL]={26, "VQ+LFl"}, [CHR_UQLFL]={27, "UQ+LFl"}, [CHR_AL]={28, "Al"}, [CHR_ALFFL]={29, "Al.FFl"}, {0, NULL} };
s57val_t litvis[] = { {1, "high"}, {2, "low"}, {3, "faint"}, {4, "intensified"}, {5, "unintensified"}, {6, "restricted"}, {7, "obscured"}, {8, "part_obscured"}, {0, NULL} };
s57val_t marsys[] = { [SYS_UNKN]={0, ""}, [SYS_IALA]={1, "iala-a"}, [SYS_IALB]={2, "iala-b"}, [SYS_NONE]={9, "none"}, [SYS_OTHR]={10, "other"}, [SYS_CEVN]={11, "cevni"}, [SYS_RIWR]={12, "riwr"},
  [SYS_BWR2]={13, "bniwr2"}, [SYS_BNWR]={14, "bniwr"}, [SYS_PPWB]={15, "ppwbc"}, {0, NULL} };
s57val_t natcon[] = { {1, "masonry"}, {2, "concreted"}, {3, "loose_boulders"}, {4, "hard-surfaced"}, {5, "unsurfaced"}, {6, "wooden"}, {7, "metal"}, {8, "grp"}, {9, "painted"}, {0, NULL} };
s57val_t natsur[] = { {1, "mud"}, {2, "clay"}, {3, "silt"}, {4, "sand"}, {5, "stone"}, {6, "gravel"}, {7, "pebbles"}, {8, "cobbles"}, {9, "rock"}, {11, "lava"}, {14, "coral"}, {17, "shells"}, {18, "boulder"}, {0, NULL} };
s57val_t natqua[] = { {1, "fine"}, {2, "medium"}, {3, "coarse"}, {4, "broken"}, {5, "sticky"}, {6, "soft"}, {7, "stiff"}, {8, "volcanic"}, {9, "calcareous"}, {10, "hard"}, {0, NULL} };
s57val_t prodct[] = { {1, "oil"}, {2, "gas"}, {3, "water"}, {4, "stone"}, {5, "coal"}, {6, "ore"}, {7, "chemicals"}, {8, "drinking_water"}, {9, "milk"}, {10, "bauxite"}, {11, "coke"}, {12, "iron_ingots"}, {13, "salt"},
  {14, "sand"}, {15, "timber"}, {16, "sawdust"}, {17, "scrap"}, {18, "lng"}, {19, "lpg"}, {20, "wine"}, {21, "cement"}, {22, "grain"}, {0, NULL} };
s57val_t quasou[] = { {1, "known"}, {2, "unknown"}, {3, "doubtful"}, {4, "unreliable"}, {5, "no_bottom_found"}, {6, "least_known"}, {7, "least_unknown"}, {8, "not_surveyed"}, {9, "not_confirmed"}, {10, "maintained"},
  {11, "not_maintained"}, {0, NULL} };
s57val_t restrn[] = { {1, "no_anchoring"}, {2, "restricted_anchoring"}, {3, "no_fishing"}, {4, "restricted_fishing"}, {5, "no_trawling"}, {6, "restricted_trawling"}, {7, "no_entry"}, {8, "restricted_entry"},
  {9, "no_dredging"}, {10, "restricted_dredging"}, {11, "no_diving"}, {12, "restricted_diving"}, {13, "no_wake"}, {14, "to_be_avoided"}, {15, "no_construction"}, {16, "no_discharging"}, {17, "restricted_discharging"},
  {18, "no_ exploration_development"}, {19, "restricted_exploration_development"}, {20, "no_drilling"}, {21, "restricted_drilling"}, {22, "no_historical_artifacts_removal"}, {23, "no_lightering"}, {24, "no_dragging"},
  {25, "no_stopping"}, {26, "no_landing"}, {27, "restricted_speed"}, {28, "no_overtaking"}, {29, "no_convoy_overtaking"}, {30, "no_passing_overtaking"},
  {31, "no_berthing"}, {32, "restricted_berthing"}, {33, "no_making_fast"}, {34, "restricted_making_fast"}, {35, "no_turning"}, {36, "restricted_fairway_depth"}, {0, NULL} };
s57val_t siggen[] = { {1, "automatic"}, {2, "wave"}, {3, "hand"}, {4, "wind"}, {0, NULL} };
s57val_t status[] = { {1, "permanent"}, {2, "occasional"}, {3, "recommended"}, {4, "not_in_use"}, {5, "intermittent"}, {6, "reserved"}, {7, "temporary"}, {8, "private"}, {9, "mandatory"}, {11, "extinguished"},
  {12, "illuminated"}, {13, "historic"}, {14, "public"}, {15, "synchronised"}, {16, "watched"}, {17, "unwatched"}, {18, "existence_doubtful"}, {19, "on_request"}, {20, "drop_away"}, {21, "rising"}, {22, "increasing"},
  {23, "decreasing"}, {24, "strong"}, {25, "good"}, {26, "moderately"}, {27, "poor"}, {0, NULL} };
s57val_t surtyp[] = { {1, "sketch"}, {2, "controlled"}, {4, "examination"}, {5, "passage"}, {6, "remote"}, {0, NULL} };
s57val_t tecsou[] = { {1, "echo-sounder"}, {2, "side-scan_sonar"}, {3, "multi-beam"}, {4, "diver"}, {5, "lead-line"}, {6, "wire-drag"}, {7, "laser"}, {8, "vertical_acoustic"}, {9, "electromagnetic"},
  {10, "photogrammetry"}, {11, "satellite"}, {12, "levelling"}, {13, "side-scan_sonar"}, {14, "computer"}, {0, NULL} };
s57val_t topshp[] = { [TOP_UNKN]={0, ""}, [TOP_CONE]={1, "cone, point up"}, [TOP_ICONE]={2, "cone, point down"}, [TOP_SPHR]={3, "sphere"}, [TOP_ISD]={4, "2 spheres"}, [TOP_CAN]={5, "cylinder"}, [TOP_BORD]={6, "board"},
  [TOP_SALT]={7, "x-shape"}, [TOP_CROS]={8, "cross"}, [TOP_CUBE]={9, "cube, point up"}, [TOP_WEST]={10, "2 cones point together"}, [TOP_EAST]={11, "2 cones base together"}, [TOP_RHOM]={12, "rhombus"},
  [TOP_NORTH]={13, "2 cones up"}, [TOP_SOUTH]={14, "2 cones down"}, [TOP_BESM]={15, "besom, point up"}, [TOP_IBESM]={16, "besom, point down"}, [TOP_FLAG]={17, "flag"}, [TOP_SPRH]={18, "sphere over rhombus"},
  [TOP_SQUR]={19, "square"}, [TOP_HRECT]={20, "rectangle, horizontal"}, [TOP_VRECT]={21, "rectangle, vertical"}, [TOP_TRAP]={22, "trapezium, up"}, [TOP_ITRAP]={23, "trapezium, down"}, [TOP_TRI]={24, "triangle, point up"},
  [TOP_ITRI]={25, "triangle, point down"}, [TOP_CIRC]={26, "circle"}, [TOP_CRSS]={27, "2 upright crosses"}, [TOP_T]={28, "t-shape"}, [TOP_TRCL]={29, "triangle, point up over circle"}, [TOP_CRCL]={30, "upright cross over circle"},
  [TOP_RHCL]={31, "rhombus over circle"}, [TOP_CLTR]={32, "circle over triangle, point up"}, [TOP_OTHR]={33, "other"}, [TOP_CYSP]={34, "cylinder over sphere"}, [TOP_COSP]={35, "cone, point up over sphere"},
  [TOP_ARRW]={99, "arrow"}, {0, NULL} };
s57val_t trafic[] = { {1, "inbound"}, {2, "outbbound"}, {3, "one-way"}, {4, "two-way"}, {0, NULL} };
s57val_t watlev[] = { {1, "part-submerged"}, {2, "dry"}, {3, "submerged"}, {4, "covers"}, {5, "awash"}, {6, "floods"}, {7, "floating"}, {8, "above_mwl"}, {9, "below_mwl"}, {0, NULL} };
s57val_t cat_ts[] = { {1, "flood"}, {2, "ebb"}, {3, "other"}, {0, NULL} };
s57val_t punits[] = { {1, "metres"}, {2, "degrees"}, {3, "millimetres"}, {4, "feet"}, {5, "cables"}, {0, NULL} };
s57val_t quapos[] = { {1, "surveyed"}, {2, "unsurveyed"}, {3, "part-surveyed"}, {4, "approximate"}, {5, "doubtful"}, {6, "unreliable"}, {7, "reported_unsurveyd"}, {8, "unconfirmed"}, {9, "estimated"},
  {10, "precise"}, {11, "calculated"}, {0, NULL} };
s57val_t catachi[] = { {1, "unrestricted"}, {2, "deep-water"}, {3, "tanker"}, {4, "explosives"}, {5, "quarantine"}, {6, "seaplane"}, {7, "small_craft"}, {9, "24_hours"}, {10, "pushing-navigation_vessels"},
  {11, "non-pushing-navigation_vessels"}, {0, NULL} };
s57val_t verdat[] = { {1, "mlws"}, {2, "mllws"}, {3, "msl"}, {4, "llw"}, {5, "mlw"}, {6, "llws"}, {7, "amlws"}, {8, "islw"}, {9, "lws"}, {10, "alat"},  {11, "nllw"}, {12, "mllw"}, {13, "lw"}, {14, "amlw"}, {15, "amllw"},
  {16, "mhw"}, {17, "mhws"}, {18, "hw"}, {19, "amsl"}, {20, "hws"}, {21, "mhhw"}, {22, "eslw"}, {23, "lat"}, {24, "local"}, {25, "igld1985"}, {26, "mlw"}, {27, "llwlt"}, {28, "hhwlt"}, {29, "nhhw"}, {30, "hat"},
  {31, "llwrl"}, {32, "lhwrl"}, {33, "lmwrl"}, {34, "ehw_dglw"}, {35, "hshw_dhsw"}, {36, "rlwl_donau"}, {37, "hshw_donau"}, {38, "drlwrl_olr"}, {39, "rpwl"}, {40, "rnbl"}, {41, "ohio_rd"}, {0, NULL} };
s57val_t addmrk[] = { [MRK_UNKN]={0, ""}, [MRK_TOPB]={1, "top_board"}, [MRK_BOTB]={2, "bottom_board"}, [MRK_RTRI]={3, "right_triangle"}, [MRK_LTRI]={4, "left_triangle"}, [MRK_BTRI]={5, "bottom_triangle"}, {0, NULL} };
s57val_t bnkwtw[] = { [BWW_UNKN]={0, ""}, [BWW_LEFT]={1, "left"}, [BWW_RGHT]={2, "right"}, {0, NULL} };
s57val_t catbnk[] = { {1, "steep"}, {2, "flat"}, {3, "fastened"}, {4, "unfastened"}, {0, NULL} };
s57val_t catnmk[] = { [NMK_UNKN]={0, ""}, [NMK_NENT]={1, "no_entry"}, [NMK_CLSA]={2, "closed_area"}, [NMK_NOVK]={3, "no_overtaking"}, [NMK_NCOV]={4, "no_convoy_overtaking"}, [NMK_NPAS]={5, "no_passing"},
  [NMK_NBRT]={6, "no_berthing"}, [NMK_NBLL]={7, "no_berthing_lateral_limit"}, [NMK_NANK]={8, "no_anchoring"}, [NMK_NMOR]={9, "no_mooring"}, [NMK_NTRN]={10, "no_turning"}, [NMK_NWSH]={11, "no_wash"},
  [NMK_NPSL]={12, "no_passage_left"}, [NMK_NPSR]={13, "no_passage_right"}, [NMK_NMTC]={14, "no_motor_craft"}, [NMK_NSPC]={15, "no_sports_craft"}, [NMK_NWSK]={16, "no_waterskiing"}, [NMK_NSLC]={17, "no_sailing_craft"},
  [NMK_NUPC]={18, "no_unpowered_craft"}, [NMK_NSLB]={19, "no_sailboards"}, [NMK_NWBK]={20, "no_waterbikes"}, [NMK_NHSC]={21, "no_high_speeds"}, [NMK_NLBG]={22, "no_launching_beaching"}, [NMK_MVTL]={23, "move_to_left"},
  [NMK_MVTR]={24, "move_to_right"}, [NMK_MVTP]={25, "move_to_port"}, [NMK_MVTS]={26, "move_to_starboard"}, [NMK_KPTP]={27, "keep_to_port"}, [NMK_KPTS]={28, "keep_to_starboard"}, [NMK_CSTP]={29, "cross_to_port"},
  [NMK_CSTS]={30, "cross_to_starboard"}, [NMK_STOP]={31, "stop"}, [NMK_SPDL]={32, "speed_limit"}, [NMK_SHRN]={33, "sound_horn"}, [NMK_KPLO]={34, "keep_lookout"}, [NMK_GWJN]={35, "give_way_junction"},
  [NMK_GWCS]={36, "give_way_crossing"}, [NMK_MKRC]={37, "make_radio_contact"}, [NMK_LMDP]={38, "limited_depth"}, [NMK_LMHR]={39, "limited_headroom"}, [NMK_LMWD]={40, "limited_width"}, [NMK_NAVR]={41, "navigation_restrictions"},
  [NMK_CHDL]={42, "channel_distance_left"}, [NMK_CHDR]={43, "channel_distance_right"}, [NMK_CHTW]={44, "channel_two_way"}, [NMK_CHOW]={45, "channel_one_way"}, [NMK_OPTR]={46, "opening_to_right"},
  [NMK_OPTL]={47, "opening_to_left"}, [NMK_PRTL]={48, "proceed_to_left"}, [NMK_PRTR]={49, "proceed_to_right"}, [NMK_ENTP]={50, "entry_permitted"}, [NMK_OVHC]={51, "overhead_cable"}, [NMK_WEIR]={52, "weir"},
  [NMK_FERN]={53, "ferry_non_independent"},  [NMK_FERI]={54, "ferry_independent"}, [NMK_BRTP]={55, "berthing_permitted"}, [NMK_BTLL]={56, "berthing_lateral_limit"}, [NMK_BTLS]={57, "berthing_lateral_limits"},
  [NMK_BTRL]={58, "berth_rafting_limit"},  [NMK_BTUP]={59, "berthing_unmarked_pushing"}, [NMK_BTP1]={60, "berthing_marked_pushing_1"}, [NMK_BTP2]={61, "berthing_marked_pushing_2"}, [NMK_BTP3]={62, "berthing_marked_pushing_3"},
  [NMK_BTUN]={63, "berthing_unmarked_non-pushing"}, [NMK_BTN1]={64, "berthing_marked_non-pushing_1"}, [NMK_BTN2]={65, "berthing_marked_non-pushing_2"}, [NMK_BTN3]={66, "berthing_marked_non-pushing_3"},
  [NMK_BTUM]={67, "berthing_unmarked"}, [NMK_BTU1]={68, "berthing_marked_1"}, [NMK_BTU2]={69, "berthing_marked_2"}, [NMK_BTU3]={70, "berthing_marked_3"}, [NMK_ANKP]={71, "anchoring_permitted"},[NMK_MORP]={72, "mooring_permitted"},
  [NMK_VLBT]={73, "vehicle_loading_berth"}, [NMK_TRNA]={74, "turning_area"}, [NMK_SWWC]={75, "secondary_waterway_crossing"}, [NMK_SWWR]={76, "secondary_waterway_right"}, [NMK_SWWL]={77, "secondary_waterway_left"},
  [NMK_WRSA]={78, "main_waterway_right_secondary_ahead"}, [NMK_WLSA]={79, "main_waterway_left_secondary_ahead"}, [NMK_WRSL]={80, "main_waterway_right_secondary_left"}, [NMK_WLSR]={81, "main_waterway_left_secondary_right"},
  [NMK_WRAL]={82, "main_waterway_right_secondary_ahead_left"}, [NMK_WLAR]={83, "main_waterway_left_secondary_ahead_right"}, [NMK_MWWC]={84, "main_waterway_crossing"}, [NMK_MWWJ]={85, "main_waterway_junction"},
  [NMK_MWAR]={86, "main_waterway_ahead_right"}, [NMK_MWAL]={87, "main_waterway_ahead_left"}, [NMK_WARL]={88, "main_waterway_ahead_right_secondary_left"}, [NMK_WALR]={89, "main_waterway_ahead_left_secondary_right"},
  [NMK_PEND]={90, "prohibition_ends"}, [NMK_DWTR]={91, "drinking_water"}, [NMK_TELE]={92, "telephone"}, [NMK_MTCP]={93, "motor_craft_permitted"}, [NMK_SPCP]={94, "sport_craft_permitted"}, [NMK_WSKP]={95, "waterskiing_permitted"},
  [NMK_SLCP]={96, "sailing_craft_permitted"}, [NMK_UPCP]={97, "unpowered_craft_permitted"}, [NMK_SLBP]={98, "sailboards_permitted"}, [NMK_RADI]={99, "radio_information"},
  [NMK_WTBP]={100, "waterbikes_permitted"}, [NMK_HSCP]={101, "high_speeds_permitted"}, [NMK_LBGP]={102, "launching_beaching_permitted"},
  [NMK_KTPM]={103, "keep_to_port_margin"}, [NMK_KTSM]={104, "keep_to_starboard_margin"}, [NMK_KTMR]={105, "keep_to_mid-river"}, [NMK_CRTP]={106, "cross_river_to_port"}, [NMK_CRTS]={107, "cross_river_to_starboard"},
  [NMK_TRBM]={108, "traffic_between_margins"}, [NMK_RSPD]={109, "reduce_speed"}, {0, NULL} };
s57val_t clsdng[] = { {1, "one_blue"}, {2, "two_blue"}, {3, "three_blue"}, {4, "no_blue"}, {0, NULL} };
s57val_t dirimp[] = { {1, "upstream"}, {2, "downstream"}, {3, "left_bank"}, {4, "right_bank"}, {5, "to_harbour"}, {0, NULL} };
s57val_t fnctnm[] = { {1, "prohibition"}, {2, "regulation"}, {3, "restriction"}, {4, "recommendation"}, {5, "information"}, {0, NULL} };
s57val_t bunves[] = { {1, "bunker_vessel_available"}, {2, "no_bunker_vessel_available"}, {0, NULL} };
s57val_t catbrt[] = { {1, "loading"}, {2, "unloading"}, {3, "overnight_accommodation"}, {4, "pushing-navigation"}, {5, "non-pushing-navigation"}, {6, "fleeting"}, {7, "first_class"}, {8, "second_class"}, {0, NULL} };
s57val_t catbun[] = { {1, "diesel oil"}, {2, "water"}, {3, "ballast"}, {0, NULL} };
s57val_t catccl[] = { {1, "small_vessels"}, {2, "peniche"}, {3, "campine_barge"}, {4, "dortmund-ems_barge"}, {5, "rhine-herne_barge"}, {6, "1-barge_push-tow"},
  {7, "2-barge_push-tow_long"}, {8, "2-barge_push-tow_wide"}, {9, "4-barge_push-tow"}, {10, "6-barge_push-tow"}, {11, "no_cemt_class"}, {0, NULL} };
s57val_t catcom[] = { {1, "vts_centre"}, {2, "vts_sector"}, {3, "ivs_point"}, {4, "mid"}, {5, "lock"}, {6, "bridge"}, {7, "custom"}, {8, "harbour"}, {0, NULL} };
s57val_t cathbr[] = { {1, "custom"}, {2, "refuge"}, {3, "marina"}, {4, "fishing"}, {5, "private"}, {0, NULL} };
s57val_t catrfd[] = { {1, "cargo_residue"}, {2, "waste_oil"}, {3, "grey_black_water"}, {4, "domestic_refuse"}, {0, NULL} };
s57val_t cattml[] = { {1, "passenger"}, {2, "ferry"}, {3, "transhipment"}, {4, "roro"}, {0, NULL} };
s57val_t trshgd[] = { {1, "containers"}, {2, "bulk"}, {3, "oil"}, {4, "fuel"}, {5, "chemicals"}, {6, "liquid"}, {7, "explosive"}, {8, "fish"}, {9, "cars"}, {10, "general"}, {0, NULL} };
s57val_t catgag[] = { [GAG_UNKN]={0, ""}, [GAG_STAF]={1, "staff"}, [GAG_RCRD]={2, "recording"}, [GAG_RCRA]={3, "recording_remote_access"}, [GAG_RCEI]={4, "recording_external_indicator"}, [GAG_RRAI]={5, "recording_remote_access_indicator"}, {0, NULL} };
s57val_t reflev[] = { {1, "baltic"}, {2, "adriatic"}, {3, "amsterdam"}, {4, "msl"}, {5, "other"}, {6, "ngvd29"}, {7, "navd88"}, {8, "msl1912"}, {9, "msl1929"}, {0, NULL} };
s57val_t catvtr[] = { {1, "official"}, {2, "private"}, {3, "car_cranes"}, {4, "car_planks"}, {5, "permission_required"}, {6, "locked_gate"}, {0, NULL} };
s57val_t cattab[] = { {1, "operational_period"}, {2, "non-operational_period"}, {0, NULL} };
s57val_t useshp[] = { {1, "liner_trade"}, {2, "occasional_professional_shipping"}, {3, "leisure"}, {0, NULL} };
s57val_t catexs[] = { {1, "lift-lock"}, {2, "aqueduct"}, {3, "sloping_plane_lock"}, {4, "water_slope_lock"}, {5, "other"}, {0, NULL} };
s57val_t catwwm[] = { {1, "waterway_right"}, {2, "waterway_left"}, {3, "waterway_separation"}, {4, "channel_right"}, {5, "channel_left"}, {6, "channel_separation"}, {7, "channel_right_bank"}, {8, "channel_left_bank"},
  {9, "crossover_right"}, {10, "crossover_left"}, {11, "danger_right"}, {12, "danger_left"}, {13, "turnoff_right"}, {14, "turnoff_left"}, {15, "junction_right"}, {16, "junction_left"}, {17, "harbour_right"},
  {18, "harbour_left"}, {19, "bridge_pier"}, {0, NULL} };
s57val_t lg_spr[] = { {1, "other"}, {2, "speed_over_ground"}, {3, "speed_through_water"}, {0, NULL} };
s57val_t lg_wdu[] = { {1, "other"}, {2, "cubic_metres"}, {3, "tonnes"}, {0, NULL} };
s57val_t lg_rel[] = { {1, "other"}, {2, "usage_of_waterway"}, {3, "carriage_of_equipment"}, {4, "task_operation"}, {0, NULL} };
s57val_t lg_fnc[] = { {1, "other"}, {2, "prohibited"}, {3, "prohibited_with_exceptions"}, {4, "permitted"}, {5, "permitted_with_exceptions"}, {6, "recommended"}, {7, "not_recommended"}, {0, NULL} };
s57val_t lc_csi[] = { {1, "all"}, {2, "other"}, {3, "non-motorized"}, {5, "craft"}, {6, "vessel"}, {7, "inland_waterway"}, {8, "sea-going"}, {9, "motor"}, {10, "motor_tanker"}, {11, "motor_cargo"}, {12, "canal_barge"},
  {13, "tug"}, {14, "pusher"}, {15, "barge"}, {16, "tank_barge"}, {17, "dumb_barge"}, {18, "lighter"}, {19, "tank_lighter"}, {20, "cargo_lighter"}, {21, "ship_borne_lighter"}, {22, "passenger"}, {23, "passenger_sailing"},
  {24, "day_trip"}, {25, "cabin"}, {26, "high-speed"}, {27, "floating_equipment"}, {28, "worksite"}, {29, "recreational"}, {30, "dinghy"}, {31, "floating_establishment"}, {32, "floating_object"}, {0, NULL} };
s57val_t lc_cse[] = { {1, "all"}, {2, "other"}, {3, "non-motorized"}, {5, "craft"}, {6, "vessel"}, {7, "inland_waterway"}, {8, "sea-going"}, {9, "motor"}, {10, "motor_tanker"}, {11, "motor_cargo"}, {12, "canal_barge"},
  {13, "tug"}, {14, "pusher"}, {15, "barge"}, {16, "tank_barge"}, {17, "dumb_barge"}, {18, "lighter"}, {19, "tank_lighter"}, {20, "cargo_lighter"}, {21, "ship_borne_lighter"}, {22, "passenger"}, {23, "passenger_sailing"},
  {24, "day_trip"}, {25, "cabin"}, {26, "high-speed"}, {27, "floating_equipment"}, {28, "worksite"}, {29, "recreational"}, {30, "dinghy"}, {31, "floating_establishment"}, {32, "floating_object"}, {0, NULL} };
s57val_t lc_asi[] = { {1, "all"}, {2, "other"}, {3, "single"}, {5, "convoy"}, {6, "formation"}, {7, "rigid_convoy"}, {8, "pushed_convoy"}, {9, "breasted"}, {10, "towed_convoy"}, {0, NULL} };
s57val_t lc_ase[] = { {1, "all"}, {2, "other"}, {3, "single"}, {5, "convoy"}, {6, "formation"}, {7, "rigid_convoy"}, {8, "pushed_convoy"}, {9, "breasted"}, {10, "towed_convoy"}, {0, NULL} };
s57val_t lc_cci[] = { {1, "all"}, {2, "other"}, {4, "bulk"}, {5, "dry"}, {6, "liquid"}, {7, "liquid_n"}, {8, "liquid_c"}, {9, "gas"}, {0, NULL} };
s57val_t lc_cce[] = { {1, "all"}, {2, "other"}, {4, "bulk"}, {5, "dry"}, {6, "liquid"}, {7, "liquid_n"}, {8, "liquid_c"}, {9, "gas"}, {0, NULL} };
s57val_t shptyp[] = { {1, "cargo"}, {2, "container"}, {3, "tanker"}, {4, "sailing"}, {5, "fishing"}, {6, "special_purpose."}, {7, "man_of_war"}, {8, "submarine"}, {9, "high-speed"}, {10, "bulk_carrier"},
  {11, "seaplane"}, {12, "tugboat"}, {13, "passenger"}, {14, "ferry"}, {15, "boat"}, {0, NULL} };

s57key_t keys[] = {
  [UNKATT]={0, S, NULL}, [AGENCY]={1, A, NULL}, [BCNSHP]={2, E, bcnshp}, [BUISHP]={3, E, buishp}, [BOYSHP]={4, E, boyshp}, [BURDEP]={5, F, NULL}, [CALSGN]={6, S, NULL}, [CATAIR]={7, L, catair},
  [CATACH]={8, L, catach}, [CATBRG]={9, L, catbrg}, [CATBUA]={10, E, catbua}, [CATCBL]={11, E, catcbl}, [CATCAN]={12, E, catcan}, [CATCAM]={13, E, catcam}, [CATCHP]={14, E, catchp}, [CATCOA]={15, E, catcoa},
  [CATCTR]={16, E, catctr}, [CATCON]={17, E, catcon}, [CATCOV]={18, E, catcov}, [CATCRN]={19, E, catcrn}, [CATDAM]={20, E, catdam}, [CATDIS]={21, E, catdis}, [CATDOC]={22, E, catdoc}, [CATDPG]={23, L, catdpg},
  [CATFNC]={24, E, catfnc}, [CATFRY]={25, E, catfry}, [CATFIF]={26, E, catfif}, [CATFOG]={27, E, catfog}, [CATFOR]={28, E, catfor}, [CATGAT]={29, E, catgat}, [CATHAF]={30, L, cathaf}, [CATHLK]={31, L, cathlk},
  [CATICE]={32, E, catice}, [CATINB]={33, E, catinb}, [CATLND]={34, L, catlnd}, [CATLMK]={35, L, catlmk}, [CATLAM]={36, E, catlam}, [CATLIT]={37, L, catlit}, [CATMFA]={38, E, catmfa}, [CATMPA]={39, L, catmpa},
  [CATMOR]={40, E, catmor}, [CATNAV]={41, E, catnav}, [CATOBS]={42, E, catobs}, [CATOFP]={43, L, catofp}, [CATOLB]={44, E, catolb}, [CATPLE]={45, E, catple}, [CATPIL]={46, E, catpil}, [CATPIP]={47, L, catpip},
  [CATPRA]={48, E, catpra}, [CATPYL]={49, E, catpyl}, [CATQUA]={50, E, catqua}, [CATRAS]={51, E, catras}, [CATRTB]={52, E, catrtb}, [CATROS]={53, L, catros}, [CATTRK]={54, E, cattrk}, [CATRSC]={55, L, catrsc},
  [CATREA]={56, L, catrea}, [CATROD]={57, E, catrod}, [CATRUN]={58, E, catrun}, [CATSEA]={59, E, catsea}, [CATSLC]={60, E, catslc}, [CATSIT]={61, L, catsit}, [CATSIW]={62, L, catsiw}, [CATSIL]={63, E, catsil},
  [CATSLO]={64, E, catslo}, [CATSCF]={65, L, catscf}, [CATSPM]={66, L, catspm}, [CATTSS]={67, E, cattss}, [CATVEG]={68, L, catveg}, [CATWAT]={69, E, catwat}, [CATWED]={70, E, catwed}, [CATWRK]={71, E, catwrk},
  [CATZOC]={72, E, catzoc}, [$SPACE]={73, E, NULL}, [$CHARS]={74, A, NULL}, [COLOUR]={75, L, colour}, [COLPAT]={76, L, colpat}, [COMCHA]={77, A, NULL}, [$CSIZE]={78, F, NULL}, [CPDATE]={79, A, NULL},
  [CSCALE]={80, I, NULL}, [CONDTN]={81, E, condtn}, [CONRAD]={82, E, conrad}, [CONVIS]={83, E, convis}, [CURVEL]={84, F, NULL}, [DATEND]={85, A, NULL}, [DATSTA]={86, A, NULL}, [DRVAL1]={87, F, NULL},
  [DRVAL2]={88, F, NULL}, [DUNITS]={89, E, dunits}, [ELEVAT]={90, F, NULL}, [ESTRNG]={91, F, NULL}, [EXCLIT]={92, E, exclit}, [EXPSOU]={93, E, expsou}, [FUNCTN]={94, L, functn}, [HEIGHT]={95, A, NULL},
  [HUNITS]={96, E, hunits}, [HORACC]={97, F, NULL}, [HORCLR]={98, F, NULL}, [HORLEN]={99, F, NULL}, [HORWID]={100, F, NULL}, [ICEFAC]={101, F, NULL}, [INFORM]={102, S, NULL}, [JRSDTN]={103, E, jrsdtn},
  [$JUSTH]={104, E, NULL}, [$JUSTV]={105, E, NULL}, [LIFCAP]={106, F, NULL}, [LITCHR]={107, E, litchr}, [LITVIS]={108, L, litvis}, [MARSYS]={109, E, marsys}, [MLTYLT]={110, I, NULL}, [NATION]={111, A, NULL},
  [NATCON]={112, L, natcon}, [NATSUR]={113, L, natsur}, [NATQUA]={114, L, natqua}, [NMDATE]={115, A, NULL}, [OBJNAM]={116, S, NULL}, [ORIENT]={117, F, NULL}, [PEREND]={118, A, NULL}, [PERSTA]={119, A, NULL},
  [PICREP]={120, S, NULL}, [PILDST]={121, S, NULL}, [PRCTRY]={122, A, NULL}, [PRODCT]={123, L, prodct}, [PUBREF]={124, S, NULL}, [QUASOU]={125, L, quasou}, [RADWAL]={126, A, NULL}, [RADIUS]={127, F, NULL},
  [RECDAT]={128, A, NULL}, [RECIND]={129, A, NULL}, [RYRMGV]={130, A, NULL}, [RESTRN]={131, L, restrn}, [SCAMAX]={132, I, NULL}, [SCAMIN]={133, I, NULL}, [SCVAL1]={134, I, NULL}, [SCVAL2]={135, I, NULL},
  [SECTR1]={136, F, NULL}, [SECTR2]={137, F, NULL}, [SHIPAM]={138, A, NULL}, [SIGFRQ]={139, I, NULL}, [SIGGEN]={140, E, siggen}, [SIGGRP]={141, A, NULL}, [SIGPER]={142, F, NULL}, [SIGSEQ]={143, A, NULL},
  [SOUACC]={144, F, NULL}, [SDISMX]={145, I, NULL}, [SDISMN]={146, I, NULL}, [SORDAT]={147, A, NULL}, [SORIND]={148, A, NULL}, [STATUS]={149, L, status}, [SURATH]={150, S, NULL}, [SUREND]={151, A, NULL},
  [SURSTA]={152, A, NULL}, [SURTYP]={153, L, surtyp}, [$SCALE]={154, F, NULL}, [$SCODE]={155, A, NULL}, [TECSOU]={156, L, tecsou}, [$TXSTR]={157, S, NULL}, [TXTDSC]={158, S, NULL}, [TS_TSP]={159, A, NULL},
  [TS_TSV]={160, A, NULL}, [T_ACWL]={161, E, NULL}, [T_HWLW]={162, A, NULL}, [T_MTOD]={163, E, NULL}, [T_THDF]={164, A, NULL}, [T_TINT]={165, I, NULL}, [T_TSVL]={166, A, NULL}, [T_VAHC]={167, A, NULL},
  [TIMEND]={168, A, NULL}, [TIMSTA]={169, A, NULL}, [$TINTS]={170, E, NULL}, [TOPSHP]={171, E, topshp}, [TRAFIC]={172, E, trafic}, [VALACM]={173, F, NULL}, [VALDCO]={174, F, NULL}, [VALLMA]={175, F, NULL},
  [VALMAG]={176, F, NULL}, [VALMXR]={177, F, NULL}, [VALNMR]={178, F, NULL}, [VALSOU]={179, F, NULL}, [VERACC]={180, F, NULL}, [VERCLR]={181, F, NULL}, [VERCCL]={182, F, NULL}, [VERCOP]={183, F, NULL},
  [VERCSA]={184, F, NULL}, [VERDAT]={185, E, verdat}, [VERLEN]={186, F, NULL}, [WATLEV]={187, E, watlev}, [CAT_TS]={188, E, cat_ts}, [PUNITS]={189, E, punits}, [NINFOM]={300, S, NULL}, [NOBJNM]={301, S, NULL},
  [NPLDST]={302, S, NULL}, [$NTXST]={303, S, NULL}, [NTXTDS]={304, S, NULL}, [HORDAT]={400, E, NULL}, [POSACC]={401, F, NULL}, [QUAPOS]={402, E, quapos}, [CLSDNG]={17055, L, clsdng}, [DIRIMP]={17056, L, dirimp},
  [DISBK1]={17057, F, NULL}, [DISBK2]={17058, F, NULL}, [DISIPU]={17059, F, NULL}, [DISIPD]={17060, F, NULL}, [ELEVA1]={17061, F, NULL}, [ELEVA2]={17062, F, NULL}, [FNCTNM]={17063, E, fnctnm}, [WTWDIS]={17064, F, NULL},
  [BUNVES]={17065, E, bunves}, [COMCTN]={17073, S, NULL}, [HORCLL]={17074, F, NULL}, [HORCLW]={17075, F, NULL}, [TRSHGD]={17076, L, trshgd}, [UNLOCD]={17077, S, NULL}, [HIGWAT]={17080, F, NULL}, [HIGNAM]={17081, S, NULL},
  [LOWWAT]={17082, F, NULL}, [LOWNAM]={17083, S, NULL}, [MEAWAT]={17084, F, NULL}, [MEANAM]={17085, S, NULL}, [OTHWAT]={17086, F, NULL}, [OTHNAM]={17087, S, NULL}, [REFLEV]={17088, E, reflev}, [SDRLEV]={17089, S, NULL},
  [VCRLEV]={17090, S, NULL}, [SCHREF]={17093, S, NULL}, [USESHP]={17094, E, useshp}, [CURVHW]={17095, F, NULL}, [CURVLW]={17096, F, NULL}, [CURVMW]={17097, F, NULL}, [CURVOW]={17098, F, NULL}, [APTREF]={17099, S, NULL},
  [SHPTYP]={33066, E, shptyp}, [UPDMSG]={40000, S, NULL}, [ADDMRK]={17050, L, addmrk}, [BNKWTW]={99999, E, bnkwtw}, [CATBNK]={17051, E, catbnk}, [CATNMK]={17052, E, catnmk}, [CATBRT]={17066, L, catbrt}, [CATBUN]={17067, L, catbun},
  [CATCCL]={17068, L, catccl}, [CATCOM]={17069, L, catcom}, [CATHBR]={17070, L, cathbr}, [CATRFD]={17071, L, catrfd}, [CATTML]={17072, L, cattml}, [CATGAG]={17078, E, catgag}, [CATVTR]={17091, L, catvtr}, [CATTAB]={17092, E, cattab},
  [CATEXS]={17100, E, catexs}, [CATWWM]={17112, E, catwwm}, [LG_SPD]={18001, F, NULL}, [LG_SPR]={18002, L, lg_spr}, [LG_BME]={18003, F, NULL}, [LG_LGS]={18004, F, NULL}, [LG_DRT]={18005, F, NULL}, [LG_WDP]={18006, F, NULL},
  [LG_WDU]={18007, E, lg_wdu}, [LG_REL]={18008, L, lg_rel}, [LG_FNC]={18009, L, lg_fnc}, [LG_DES]={18010, S, NULL}, [LG_PBR]={18011, S, NULL}, [LC_CSI]={18012, L, lc_csi}, [LC_CSE]={18013, L, lc_cse}, [LC_ASI]={18014, L, lc_asi},
  [LC_ASE]={18015, L, lc_ase}, [LC_CCI]={18016, L, lc_cci}, [LC_CCE]={18017, L, lc_cce}, [LC_BM1]={18018, F, NULL}, [LC_BM2]={18019, F, NULL}, [LC_LG1]={18020, F, NULL}, [LC_LG2]={18021, F, NULL}, [LC_DR1]={18022, F, NULL},
  [LC_DR2]={18023, F, NULL}, [LC_SP1]={18024, F, NULL}, [LC_SP2]={18025, F, NULL}, [LC_WD1]={18026, F, NULL}, [LC_WD2]={18027, F, NULL}, [LITRAD]={0, A, NULL},
  {17000, L, catach}, {17002, L, catsit}, {17003, L, catsiw}, {17004, L, restrn}, {17005, E, verdat}, {17006, L, catbrg}, {17007, E, catfry}, {17008, L, cathaf}, {17009, E, marsys}, {17010, L, catchp},
  {17011, E, catlam}, {17012, E, catslc}, {17101, E, catcbl}, {17102, L, cathlk}, {17103, E, hunits}, {17104, E, watlev},
  {-1, S, NULL}
};

iconv_t conv;

void set_conv(char *code) {
  conv = iconv_open("UTF-8", code);
}

s57key_t *findKey(Attl_t attl) {
  for (int i = 0; keys[i].attl >= 0; i++) {
    if (keys[i].attl == attl)
      return (&keys[i]);
  }
  return NULL;
}

char *findVal(s57key_t *key, Enum_t atvl) {
  if (key != NULL) {
    for (int i = 0; key->val[i].val != NULL; i++) {
      if (key->val[i].atvl == atvl)
        return (key->val[i].val);
    }
  }
  return "";
}

char *findEnum(s57key_t *key, Enum_t val) {
  if (key != NULL) {
    return (key->val[val].val);
  }
  return "";
}

Enum_t getEnum(s57val_t *key, char *val) {
  if (key != NULL) {
    for (int i = 0; key[i].val != NULL; i++) {
      if (strcmp(key[i].val, val) == 0)
        return i;
    }
  }
  return 0;
}

Enum_t getVal(s57val_t *key, char *val) {
  return (key[getEnum(key, val)].atvl);
}

char instr[1000];
char outstr[1000];

char *decodeValue(Attl_t attl, char *atvl) {
  s57key_t *key = findKey(attl);
  if (key == NULL)
    return NULL;
  switch (key->type) {
    case A:
    case S: {
      size_t in = strlen(atvl);
      size_t out = 999;
      bzero(outstr, out);
      char *to = outstr;
      iconv(conv, &atvl, &in, &to, &out);
      for (int i = 0; outstr[i] != 0; i++) {
        if ((outstr[i] == '<') || (outstr[i] == '>'))
          outstr[i] = '_';
      }
    }
      break;
    case E:
    case L:
      strncpy(instr, atvl, 999);
      strcpy(outstr, "");
      char *item = strtok(instr, ",");
      while (item != NULL) {
        if (strlen(outstr) > 0)
          strcat(outstr, ";");
        char *val = findVal(key, atoi(item));
        if (val == NULL)
          return NULL;
        strcat(outstr, val);
        if (key->type == E)
          break;
        item = strtok(NULL, ",");
      }
      break;
    case I:
      sprintf(outstr, "%d", atoi(atvl));
      break;
    case F:
      sprintf(outstr, "%g", atof(atvl));
      break;
  }
  char *str = outstr;
  while (isspace(*str)) str++;
  if (strlen(str) == 0)
    return NULL;
  return outstr;
}

char *stringValue(Val_t val) {
  s57key_t *key = &keys[val.key];
  strcpy(outstr, "");
  switch (val.type) {
    case A:
    case S:
      strcpy(outstr, val.val.a);
      break;
    case E:
      if (findVal(key, val.val.e) == NULL)
        break;
      strcpy(outstr, findEnum(key, val.val.e));
      break;
    case L: {
      strcpy(outstr, "");
      Lst_t *lst = val.val.l;
      while (lst != NULL) {
        strcat(outstr, findEnum(key, lst->val));
        lst = lst->next;
        if (lst != NULL)
          strcat(outstr, ";");
      }
    }
      break;
    case I:
      sprintf(outstr, "%ld", val.val.i);
      break;
    case F:
      sprintf(outstr, "%g", val.val.f);
      break;
  }
  return outstr;
}

Val_t convertValue(char *val, Atta_t key) {
  Val_t value = {0, S, 0};
  char *tok;
  value.key = key;
  s57key_t *valkey = &keys[value.key];
  if (value.key != 0) {
    value.type = valkey->type;
    switch (value.type) {
      case S:
      case A:
        value.val.s = strdup(val);
        break;
      case L:
        value.val.l = NULL;
        Lst_t *llst = NULL;
        for (tok = strtok(val, ";"); tok != NULL; tok = strtok(NULL, ";")) {
          Lst_t *lst = malloc(sizeof(Lst_t));
          lst->val = getEnum(valkey->val, tok);
          lst->next = NULL;
          if (llst == NULL)
            value.val.l = lst;
          else llst->next = lst;
          llst = lst;
        }
        break;
      case E:
        tok = strtok(val, ";");
        value.val.e = getEnum(valkey->val, tok);
        break;
      case F:
        value.val.f = atof(val);
        break;
      case I:
        value.val.i = atoi(val);
        break;
    }

  }
  return value;
}

Enum_t enumValue(char *val, Atta_t att) {
  if (val == NULL) return 0;
  s57val_t *values = keys[att].val;
  if (values == NULL) return 0;
  for (int i = 0; values[i].val != NULL; i++) {
    if (strcmp(values[i].val, val) == 0)
      return i;
  }
  return 0;
}
