/* Copyright 2011 Malcolm Herring
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#include "rules.h"

object_rules(refls, char *body_shape) {
  if (zoom >= 14) {
    char *shape = strdup(body_shape);
    literal_switch(body_shape)
    literal_case("pillar|spar") {
      if (has_object("topmark")) symbol_orientation("radar_reflector", BC, -60, 180, 18.5);
      else symbol_orientation("radar_reflector", BC, -35, 100, 18.5);
    }
    literal_case("can|conical|spherical|barrel") {
      if (has_object("topmark")) symbol_orientation("radar_reflector", BC, -45, 120, 18.5);
      else symbol_orientation("radar_reflector", BC, -15, 50, 18.5);
    }
    literal_case("lattice|tower|beacon|stake") {
      if (has_object("topmark")) symbol_position("radar_reflector", BC, 0, 160);
      else symbol_position("radar_reflector", BC, 0, 80);
    }
    literal_case("float|super-buoy") {
      if (has_object("topmark")) symbol_position("radar_reflector", BC, 0, 130);
      else symbol_position("radar_reflector", BC, 0, 50);
    }
    end_switch
    free(shape);
  }
}

object_rules(fogs) {
  if (zoom >= 11) symbol("fog_signal");
  if (zoom >= 15) {
    make_char_string("fog_signal");
    text(string, "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -60, -10);
    free_string
  }
}

object_rules(rtbs) {
  if (zoom >= 11) symbol("radar_station");
  if (zoom >= 15) {
    make_char_string("radar_transponder");
    text(string, "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -40, -40);
    free_string
  }
}

object_rules(rdos) {
  if (zoom >= 11) symbol("radar_station");
  if (zoom >= 15) {
    use_object("radio_station");
    attribute_switch("category")
    attribute_case("ais|s-ais") text("AIS", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("omnidirectional") text("RC", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("directional") text("RD", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("rotating_pattern") text("RW", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("consol") text("Consol", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("rdf") text("RG", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("qtg") text("R", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("aeronautical") text("AeroRC", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("decca") text("Decca", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("loran") text("Loran", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("dgps") text("DGPS", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("toran") text("Toran", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("omega") text("Omega", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("syledis") text("Syledis", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    attribute_case("chiaka") text("Chiaka", "font-family:Arial; font-weight:normal; font-size:70; text-anchor:end", -30, -70);
    end_switch
    used
  }
}

object_rules(lights) {
  if (has_object("topmark")) {
    use_object("topmark")
    if (attribute_test("status", "illuminated") && (zoom >= 14)) {
      symbol("floodlit");
    }
    used
  }
  if (is_type("light_major|light_minor|light|pile")) {
    if (is_type("light_major")) {
      symbol("light_major");
      if (has_object("fog_signal")) object(fogs);
    } else if (is_type("light_minor|light")) {
      if ((zoom >= 14) || ((zoom >= 11) && (has_object("radar_transponder")))) {
        if (has_object("light")) {
          use_object("light")
          if (attribute_test("category", "floodlight") && (zoom >= 14)) {
            symbol("signal_station");
          } else {
            symbol("light_minor");
          }
          used
        }
        if (has_object("fog_signal")) object(fogs);
      }
    } else if (is_type("pile")) {
      if (has_object("light") && (zoom >= 14)) {
        use_object("light")
        if (attribute_test("category", "floodlight") && (zoom >= 14)) {
          symbol("signal_station");
        } else {
          symbol("light_minor");
        }
        used
      } else if (zoom >= 16) symbol("post");
    }
    if (has_object("radar_transponder")) object(rtbs);
    if (has_object("radio_station")) object(rdos);
  }
  if (has_object("light")) {
    int n = object_count("light");
    if (n > 0) {
      for (int i = 1; i <= n; i++) {
        if (zoom >= 15) {
          make_char_string_indexed("light", i);
          light_sector_caption(i, string, "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.5, -20);
          free_string;
        } else {
          if ((zoom >= 14) || ((zoom >= 12) && (is_type("light_major|landmark")))) {
            light_sector(i);
          }
        }
      }
    }
    if ((zoom >= 14) || is_type("light_major|landmark") ||
        ((zoom >= 11) && (has_object("radar_transponder")))) {
      light_flare;
    }
    if (zoom >= 15) {
      make_char_string("light");
      text(string, "font-family:Arial; font-weight:normal; font-size:70; text-anchor:start", 60, -10);
      free_string;
      if (has_item_attribute("name") && !has_item_attribute("fixme") && is_type("light_major"))
        text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -100);
    }
  }
}

object_rules(top_colours, char *shape, double dx, double dy, double th) {
  make_string(shape);
  if (has_attribute("colour")) {
    int n = attribute_count("colour");
    char panel[4] = {'0'+n, 0, 0, 0};
    if (has_attribute("colour_pattern")) {
      attribute_switch("colour_pattern")
      attribute_case("horizontal") panel[1] = 'H';
      attribute_case("vertical") panel[1] = 'V';
      attribute_case("border") panel[1] = 'B';
      end_switch;
    } else {
      if (n > 1) panel[1] = '_';
    }
    attribute_sequence("colour");
    int i; for(i = 1; i <= n; i++) {
      panel[2] = '0' + i;
      colour_symbol_orientation(string, panel, attribute_next, BC, dx, dy, th);
    }
    end_sequence
  }
  symbol_orientation(string, BC, dx, dy, th);
  free_string;
}

object_rules(topmarks, char *body_shape) {
  if (has_attribute("shape")) {
    literal_switch(body_shape)
    literal_case("pillar|spar") object(top_colours, attribute("shape"), -33.5, 100, 18.5);
    literal_case("lattice|tower|beacon|stake") object(top_colours, attribute("shape"), 0, 73, 0);
    literal_case("float|super-buoy") object(top_colours, attribute("shape"), 0, 44, 0);
    literal_case("can|conical|spherical|barrel") object(top_colours, attribute("shape"), -15.3, 37, 18.5);
    end_switch
  }
}

object_rules(mark_colours, char* default_shape) {
  make_string("");
  if (attribute_test("shape", "lattice|tower|pillar|spar|barrel|can|conical|spherical|super-buoy")) {
    add_string(attribute("shape"));
  } else {
    add_string(default_shape)
  }
  if (has_attribute("colour")) {
    int n = attribute_count("colour");
    char panel[4] = {'0'+n, 0, 0, 0};
    if (has_attribute("colour_pattern")) {
      attribute_switch("colour_pattern")
      attribute_case("horizontal") panel[1] = 'H';
      attribute_case("vertical") panel[1] = 'V';
      attribute_case("diagonal") panel[1] = 'D';
      attribute_case("squared") panel[1] = 'S';
      attribute_case("cross") panel[1] = 'C';
      attribute_case("saltire") panel[1] = 'X';
      end_switch;
    }
    attribute_sequence("colour");
    int i; for(i = 1; i <= n; i++) {
      panel[2] = '0' + i;
      colour_symbol(string, panel, attribute_next);
    }
    end_sequence
  } 
  symbol(string);
  if (has_object("topmark")) {
    use_object("topmark")
    object(topmarks, string);
    used
  } else if (has_object("daymark")) {
    use_object("daymark")
    object(topmarks, string);
    used
  }
  if (has_object("fog_signal")) object(fogs);
  if (has_object("radar_transponder")) object(rtbs);
  if (has_object("radio_station")) object(rdos);
  if (has_object("radar_reflector")) object(refls, string);
  if (has_object("light")) object(lights);
  if ((zoom >= 15) && has_item_attribute("name")) {
    literal_switch(string)
    literal_case("lattice|beacon") text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 20, -50);
    literal_case("pillar|spar") text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 50, -50);
    literal_case("float|tower") text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 60, -50);
    literal_case("spherical|can|conical|float|super-buoy|tower|barrel") {
      if (has_object("topmark") || has_object("daymark") || is_type("mooring"))
        text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 60, -50);
      else
        text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 0, -50);
    }
    end_switch
  }
  free_string;
}

object_rules(mark_shapes, char* default_shape) {
  if (has_attribute("shape")) {
    attribute_switch("shape")
    attribute_case("stake") {
      if (has_attribute("colour")) {
      attribute_switch("colour")
      attribute_case("red") symbol("stakeR");
      attribute_case("green") symbol("stakeG");
      attribute_case("yellow") symbol("stakeY");
      attribute_default symbol("stake");
      end_switch
      } else {
        symbol("stake");
      }
      if (has_object("topmark")) { 
        use_object("topmark")
        object(topmarks, "stake");
        used
      } else if (has_object("daymark")) {
        use_object("daymark")
        object(topmarks, "stake");
        used
      }
      if (has_object("fog_signal")) object(fogs);
      if (has_object("radar_transponder")) object(rtbs);
      if (has_object("radio_station")) object(rdos);
      if (has_object("radar_reflector")) object(refls, "stake");
      if (has_object("light")) object(lights);
      if ((zoom >= 15) && (has_item_attribute("name"))) text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 20, -50);
    }
    attribute_case("perch|withy") {
      if (has_attribute("category")) {
        attribute_switch("category")
        attribute_case("port") {
          if (attribute_test("shape", "perch"))
            symbol("perchP");
          else
            symbol("withyP");
        }
        attribute_case("starboard") {
          if (attribute_test("shape", "perch"))
            symbol("perchS");
          else
            symbol("withyS");
        }
        attribute_default symbol("stake");
        end_switch
      }
      if ((zoom >= 15) && has_item_attribute("name")) text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 20, -50);
    }
    attribute_case("cairn") {
      symbol("cairn");
      if ((zoom >= 15) && has_item_attribute("name")) text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 30, -50);
    }
    attribute_default object(mark_colours, default_shape);
    end_switch
  } else {
    object(mark_colours, default_shape);
  }
}

object_rules(buoys) {
  switch (zoom) {
    case 9:
    case 10:
      break;
    case 11:
      if (is_type("buoy_safe_water") || has_object("radar_transponder") || attribute_test("shape", "super-buoy"))
        object(mark_shapes, "pillar");
      break;
    case 12:
    case 13:
      if (!is_type("buoy_special_purpose") || has_object("radar_transponder") || attribute_test("shape", "super-buoy"))
        object(mark_shapes, "pillar");
      break;
    default:
      object(mark_shapes, "pillar");
      break;
  }
}

object_rules(beacons) {
  if (attribute_test("status", "illuminated") && (zoom >= 14)) {
    symbol("floodlit");
  }
  switch (zoom) {
    case 9:
    case 10:
      break;
    case 11:
      if (is_type("beacon_safe_water") || has_object("radar_transponder"))
        object(mark_shapes, "beacon");
      break;
    case 12:
    case 13:
      if (!is_type("beacon_special_purpose") || has_object("radar_transponder"))
        object(mark_shapes, "beacon");
      break;
    default:
      object(mark_shapes, "beacon");
      if (has_object("notice"))
        symbol_notice;
      break;
  }
}

object_rules(floats) {
  switch (zoom) {
    case 9:
    case 10:
      break;
    case 11:
      if (is_type("light_vessel") || is_type("buoy_installation"))
        object(mark_shapes, "super-buoy");
      else if (has_object("radar_transponder"))
        object(mark_shapes, "float");
      break;
    default:
      if (is_type("light_vessel") || is_type("buoy_installation"))
        object(mark_shapes, "super-buoy");
      else
        object(mark_shapes, "float");
      break;
  }
}

object_rules(shoreline) {
  int ref;
  if ((zoom >= 12) && has_attribute("category")) {
    attribute_switch("category")
    attribute_case("slipway") {
      area("fill:#ffe000;fill-opacity:1;stroke:#000000;stroke-width:2;stroke-opacity:1");
      if ((zoom >= 16) && has_object("small_craft_facility")) {
        use_object("small_craft_facility")
        if (attribute_test("category", "slipway")) symbol_cluster("small_craft_facility");
        used
      }
    }
    attribute_case("training_wall") {
      if (attribute_test("water_level", "covers")) {
        ref = line("stroke-width:10; stroke-dasharray:40,40; stroke-linecap:butt; stroke-linejoin:round; stroke:#000000; fill:none");
        if (zoom >= 15) line_text("(covers)", "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.5, 50, ref);
      } else ref = line("stroke-width:10; stroke-linecap:butt; stroke-linejoin:round; stroke:#000000; fill:none");
      if (zoom >= 15) line_text("Training Wall", "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.5, -20, ref);
    }
    end_switch
  }
}

object_rules(separation) {
  if (is_type("separation_zone|separation_crossing|separation_roundabout")) {
    if (zoom <= 15)
      area("stroke:none; fill:#c480ff; fill-opacity:0.5");
    else
      area("stroke:#c480ff; stroke-width:20; stroke-opacity:0.5; fill:none");
    if ((zoom >= 10) && has_item_attribute("name")) {
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:150; text-anchor:middle", 0, 0);
    }
  } else if (is_type("separation_line")) {
    line("stroke-width:20; stroke-linecap:butt; stroke-linejoin:round; fill:none; stroke:#c480ff; stroke-opacity:0.5");
  } else if (is_type("separation_lane")) {
    line_symbols("lane_arrow", 0.5, NULL, 0);
  } else if (is_type("separation_boundary")) {
    line("stroke-width: 20; stroke-dasharray:40,40; stroke-linecap:butt; stroke-linejoin:round; fill:none; stroke:#c480ff; stroke-opacity:0.5");
  } else if (is_type("inshore_traffic_zone")) {
    line_symbols("restricted_line", 1.0, NULL, 0);
  }
}

object_rules(transits) {
  int ref = 0;
  if (zoom >= 14) {
    if (is_type("recommended_track") && is_line) {
      if (nodes == 2)ref = line("stroke-width:8; stroke:#000000; stroke-linecap:butt; fill:none");
      else line("stroke-width:8; stroke-dasharray:10,10; stroke:#000000; stroke-linecap:butt; fill:none");
    }
    else if (is_type("navigation_line") && is_line) ref = line("stroke-width:8; stroke-dasharray:20,20; stroke:#000000; stroke-linecap:butt; fill:none");
  }
  if ((ref != 0) && (zoom >= 15)) {
    make_string("");
    if (has_object("name")) {
      add_string(item_attribute("name"));
      add_string(" ");
    }
    if (has_attribute("orientation")) {
      add_string(attribute("orientation"));
      add_string("°");
    }
    if (has_attribute("category")) {
      add_string(" (");
      add_string(attribute("category"));
      add_string(")");
    }
    line_text(string, "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.5, -20, ref);
    free_string
  }
}

object_rules(moorings) {
  if ((zoom >= 11) && (zoom < 14) && attribute_test("shape", "super-buoy"))
    object(mark_shapes, "super-buoy");
  if ((zoom >= 14) && (has_attribute("category"))) {
    attribute_switch("category")
    attribute_case("buoy") {
      object(mark_shapes, "spherical");
      if (has_attribute("shape")) {
        attribute_switch("shape")
        attribute_case("float|super-buoy") object(top_colours, "top_mooring", 0, 44, 0);
        attribute_case("can|conical|spherical|barrel") object(top_colours, "top_mooring", -15.3, 37, 18.5);
        end_switch
      } else {
        object(top_colours, "top_mooring", -15.3, 37, 18.5);
      }
    }
    attribute_case("dolphin") { if (zoom >= 16) symbol("dolphin"); }
    attribute_case("pile|post") { if (zoom >= 16) symbol("post"); }
    attribute_case("bollard") { if (zoom >= 16) symbol("bollard"); }
    attribute_case("deviation_dolphin") { if (zoom >= 16)symbol("deviation_dolphin"); }
    end_switch
  }
  if (has_object("fog_signal")) object(fogs);
  if (has_object("radar_transponder")) object(rtbs);
  if (has_object("radio_station")) object(rdos);
  if (has_object("light")) object(lights);
}

object_rules(signals) {
  if (zoom >= 14) {
    if (is_type("signal_station_traffic|signal_station_warning|coastguard_station")) {
      symbol("signal_station");
      if (zoom >= 15) {
        make_char_string(this_type);
        text(string, "font-family:Arial; font-weight:normal; font-size:70; text-anchor:middle", 0.0, 65);
        free_string
      }
    } else if (is_type("radio_station|radar_station")) {
      symbol("signal_station");
      symbol("radar_station");
    } else if (is_type("rescue_station"))
      symbol("rescue");
    else if (is_type("pilot_boarding"))
      symbol("pilot");
  }
  if (has_object("fog_signal")) object(fogs);
  if (has_object("radar_transponder")) object(rtbs);
  if (has_object("radio_station")) object(rdos);
  if (has_object("light")) object(lights);
}

object_rules(radios) {
  if (zoom >= 12) {
    symbol("signal_station");
    if (is_type("radio_station") && has_attribute("category")) {
      if (attribute_test("category", "v-ais|v-ais_north_cardinal|v-ais_south_cardinal|v-ais_east_cardinal|v-ais_west_cardinal|v-ais_port_lateral") ||
          attribute_test("category", "v-ais_starboard_lateral|v-ais_isolated_danger|v-ais_safe_water|v-ais_special_purpose|v-ais_wreck")) {
        text("V-AIS", "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.0, 80);
        attribute_switch("category")
        attribute_case("v-ais_north_cardinal") symbol_orientation("top_north", BC, 0, 25, 0);
        attribute_case("v-ais_south_cardinal") symbol_orientation("top_south", BC, 0, 25, 0);
        attribute_case("v-ais_east_cardinal") symbol_orientation("top_east", BC, 0, 25, 0);
        attribute_case("v-ais_west_cardinal") symbol_orientation("top_west", BC, 0, 25, 0);
        attribute_case("v-ais_port_lateral") symbol_orientation("top_can", BC, 0, 25, 0);
        attribute_case("v-ais_starboard_lateral") symbol_orientation("top_cone_up", BC, 0, 25, 0);
        attribute_case("v-ais_isolated_danger") symbol_orientation("top_isol", BC, 0, 25, 0);
        attribute_case("v-ais_safe_water") symbol_orientation("top_sphere", BC, 0, 25, 0);
        attribute_case("v-ais_special_purpose") symbol_orientation("top_saltire", BC, 0, 25, 0);
        attribute_case("v-ais_wreck") symbol_orientation("top_cross", BC, 0, 25, 0);
        end_switch
      }
    }
    if ((zoom >= 15) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -140);
    if (has_object("fog_signal")) object(fogs);
    if (has_object("light")) object(lights);
    if (has_object("radar_transponder")) object(rtbs);
    if (has_object("radio_station")) object(rdos);
  }
}

object_rules(gauge) {
  if (zoom >= 14) {
    if (attribute_test("category", "recording|recording_remote_access")) {
      symbol("signal_station");
    } else {
      symbol("tide_gauge");
    }
    if (has_object("light")) object(lights);
    if ((zoom >= 15) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:60; text-anchor:middle", 0, -100);
  }
}

object_rules(landmarks) {
  if (attribute_test("status", "illuminated") && (zoom >= 14)) {
    symbol("floodlit");
  }
  if ((zoom >= 12) && (has_attribute("function"))) {
    attribute_switch("function")
    attribute_case("pagoda") symbol("temple");
    attribute_case("mosque") symbol("minaret");
    attribute_case("marabout") symbol("spire");
    attribute_case("chapel") symbol("church");
    attribute_default symbol(attribute("function"));
    end_switch
  }
  if (((zoom >= 12) || (has_object("light"))) && has_attribute("category")) {
    attribute_switch("category")
    attribute_case("statue|column|obelisk") symbol("monument");
    attribute_case("tower") {
      if (attribute_test("function", "church|chapel"))
        symbol("church_tower");
      else
        symbol("land_tower");
    }
    attribute_case("mast") {
      if (attribute_test("function", "communication|television|radio|radar"))
        symbol("radio_mast");
      else
        symbol("mast");
    }
    attribute_case("cross") symbol("land_cross");
    attribute_case("radar_scanner") symbol("radio_mast");
    attribute_default symbol(attribute("category"));
    end_switch
  }
  if (!has_attribute("function") && !has_attribute("category") && has_object("light")) {
    symbol("lighthouse");
    if ((zoom >= 15) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -100);
  } else {
    if ((zoom >= 15) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 60, -50);
  }
  if (has_object("fog_signal")) object(fogs);
  if (has_object("radar_transponder")) object(rtbs);
  if (has_object("radio_station")) object(rdos);
  if (has_object("radar_station") && (zoom >= 12)) symbol("radar_station");
  if (has_object("light")) object(lights);
}

object_rules(buildings) {
  if (attribute_test("status", "illuminated") && (zoom >= 14)) {
    symbol("floodlit");
  }
  if ((zoom >= 16) && (has_attribute("function"))) {
    attribute_switch("function")
    attribute_case("harbour_master") symbol("harbour_master");
    attribute_case("health|hospital") symbol("hospital");
    attribute_case("customs") symbol("customs");
    attribute_case("pagoda") symbol("temple");
    attribute_case("mosque") symbol("minaret");
    attribute_case("marabout") symbol("spire");
    attribute_default symbol(attribute("function"));
    end_switch
  }
}

object_rules(platforms) {
  if (has_attribute("category")) {
    attribute_switch("category")
    attribute_case("fpso") symbol("storage");
    attribute_default symbol("platform");
    end_switch
  } else {
    symbol("platform");
  }
  if ((zoom >= 15) && has_item_attribute("name"))
    text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 60, -50);
  if (has_object("fog_signal")) object(fogs);
  if (has_object("radar_transponder")) object(rtbs);
  if (has_object("radio_station")) object(rdos);
  if (has_object("light")) object(lights);
}

object_rules(notices) {
  if (zoom >= 14) {
    symbol_notice;
    if (has_object("light")) object(lights);
  }
}

object_rules(marinas) {
  if (has_object("shoreline_construction")) {
    use_object("shoreline_construction")
    if (attribute_test("category", "slipway")) area("fill:#ffe000;fill-opacity:1;stroke:#000000;stroke-width:2;stroke-opacity:1");
    used
  }
  if (zoom >= 16) symbol_cluster("small_craft_facility");
}

object_rules(distances) {
  if (zoom >= 14) {
    if (has_attribute("category") && !(attribute_test("category", "not_installed")))
      symbol("distance_i");
    else
      symbol("distance_u");
    if (has_attribute("distance") && (zoom >= 15)) {
      make_string("");
      if (has_attribute("units")) {
        attribute_switch("units")
        attribute_case("metres") add_string("m ")
        attribute_case("feet") add_string("ft ")
        attribute_case("hectometres") add_string("hm ")
        attribute_case("kilometres") add_string("km ")
        attribute_case("statute_miles") add_string("M ")
        attribute_case("nautical_miles") add_string("NM ")
        end_switch
      }
      add_string(attribute("distance"));
      text(string, "font-family:Arial; font-weight:normal; font-size:60; text-anchor:middle", 0, 45);
      free_string;
    }
  }
}

object_rules(bridges) {
  if (zoom >= 16) symbol_cluster("bridge");
}

object_rules(ports) {
  if (zoom>=14) {
    if (is_type("crane")) {
      if (attribute_test("category", "container_crane")) symbol("container_crane");
      else symbol("port_crane");
    }
    if (is_type("hulk")) {
      area("fill:#ffe000;fill-opacity:1;stroke:#000000;stroke-width:2;stroke-opacity:1");
      if ((zoom >= 15) && (has_item_attribute("name")))
        text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:70; text-anchor:middle", 0, 0);
    }
  }
}

object_rules(harbours) {
  if ((zoom >= 12) && is_type("anchorage")) {
    symbol("anchorage");
    if ((zoom >= 15) && (has_item_attribute("name")))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -90);
    if ((zoom >= 12) && (is_area)) line_symbols("restricted_line", 0.5, "line_anchor", 10);
  }
  if ((zoom >= 16) && is_type("anchor_berth")) symbol("anchor_berth");
  if ((zoom >= 16) && is_type("berth")) {
    symbol("berth");
    if (has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, 15);
  }
  if ((zoom >= 12) && is_type("harbour")) {
    if (has_attribute("category")) {
      attribute_switch("category")
      attribute_case("fishing") symbol("fishing_harbour");
      attribute_case("roro|ferry|naval|tanker|passenger|container|bulk|cargo") symbol("harbour");
      attribute_case("marina|yacht") symbol("marina");
      attribute_case("marina_no_facilities") symbol("marina_nf");
      attribute_default symbol("harbour");
      end_switch
    } else symbol("harbour");
    if ((zoom >= 15) && (has_item_attribute("name")))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -90);
  }
}

object_rules(areas) {
  if ((zoom >= 12) && is_type("seaplane_landing_area")) {
    symbol("seaplane");
    if ((zoom >= 15) && (has_item_attribute("name")))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -90);
    if (is_area) line_symbols("restricted_line", 0.5, "line_plane", 10);
  }
  if (is_type("marine_farm")) {
    if (zoom >= 14) {
      symbol("marine_farm");
      if (!is_node) line("stroke:#000000;stroke-width:5;stroke-dasharray:20,20;fill:none");
    }
  }
  if (is_type("fairway")) {
    if (extent > 2.0) {
      if (zoom < 16) area("stroke:#c480ff;stroke-width:8;stroke-dasharray:50,50;fill:#ffffff;fill-opacity:0.25");
      else area("stroke:#c480ff;stroke-width:8;stroke-dasharray:50,50;fill:none");
    } else {
      if (zoom >= 14) area("stroke:none;fill:#ffffff;fill-opacity:0.25");
    }
  }
  if (is_type("dredged_area")) {
    if (zoom < 16 ) area("stroke:#000000;stroke-width:6;stroke-dasharray:25,25;fill:#ffffff;fill-opacity:0.25");
    else area("stroke:#000000;stroke-width:6;stroke-dasharray:25,25;fill:none");
    if ((zoom >= 12) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial;font-weight:normal;font-size:100;text-anchor:middle", 0, 0);
  }
  if (is_type("restricted_area|military_area") && (zoom >= 12)) {
    line_symbols("restricted_line", 1.0, NULL, 0);
    if (attribute_test("category", "no_wake")) {
      symbol("no_wake");
    }
  }
  if (is_type("precautionary_area") && (zoom >= 12)) {
    area("stroke-width:10;stroke-dasharray:40,40;fill:none;stroke:#c480ff");
  }
  if (is_type("production_area")) {
    if (attribute_test("category", "wind_farm")) {
      symbol("wind_farm");
      area("stroke-width:20;stroke-dasharray:40,40;fill:none;stroke:#000000");
      if ((zoom >= 15) && (has_item_attribute("name")))
        text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, 70);
    }
  }
  if (is_type("sea_area")) {
    if (has_attribute("category")) {
      make_string("");
      attribute_switch("category")
      attribute_case("reach") { if (zoom >= 10) add_string("font-family:Arial;font-weight:normal;font-style:italic;font-size:150;text-anchor:middle") }
      attribute_case("bay") { if (zoom >= 12) add_string("font-family:Arial;font-weight:normal;font-style:italic;font-size:150;text-anchor:middle") }
      attribute_case("shoal") { if (zoom >= 14) {
        if (is_area) {
          area("stroke:#c480ff;stroke-width:4;stroke-dasharray:25,25;fill:none");
          if (has_item_attribute("name")) text(item_attribute("name"), "font-family:Arial;font-weight:normal;font-style:italic;font-size:75;text-anchor:middle", 0, -40);
          text("(Shoal)", "font-family:Arial;font-weight:normal;font-size:60;text-anchor:middle", 0, 0);
        } else if (is_line) {
          if (has_item_attribute("name")) way_text(item_attribute("name"), "font-family:Arial;font-weight:normal;font-style:italic;font-size:75;text-anchor:middle", 0.5, -40, line("stroke:none;fill:none"));
          way_text("(Shoal)", "font-family:Arial;font-weight:normal;font-size:60;text-anchor:middle", 0.5, 0, line("stroke:none;fill:none"));
        } else {
          if (has_item_attribute("name")) text(item_attribute("name"), "font-family:Arial;font-weight:normal;font-style:italic;font-size:75;text-anchor:middle", 0, -40);
          text("(Shoal)", "font-family:Arial;font-weight:normal;font-size:60;text-anchor:middle", 0, 0);
        }
      }
      }
      attribute_case("gat|narrows") { if (zoom >= 12) add_string("font-family:Arial;font-weight:normal;font-style:italic;font-size:100;text-anchor:middle") }
      end_switch
      if ((strlen(string) > 0) && !attribute_test("category", "shoal")) {
        int ref = line("stroke:none;fill:none");
        if (ref != 0) {
          if (has_item_attribute("name")) way_text(item_attribute("name"), string, 0.5, 0, ref);
        } else {
          if (has_item_attribute("name")) text(item_attribute("name"), string, 0, 0);
        }
      }
      free_string
    }
  }
//  if (is_type("sandwaves") && (zoom>=12)) area("fill:url(#sandwaves)");
  if (is_type("weed") && attribute_test("category", "kelp") && (zoom>=12)) {
    if (is_node) symbol("kelp_p");
    else if (is_area) area("fill:url(#kelp_a)");
  }
}

object_rules(obstructions) {
  if (is_type("obstruction") && (zoom >= 12)) {
    if (has_attribute("category")) {
      attribute_switch("category")
      attribute_case("boom") {
        int ref = line("stroke:#000000;stroke-width:5;stroke-dasharray:20,20;fill:none");
        if (zoom >= 15) line_text("Boom", "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.5, -20, ref);
      }
      end_switch;
    }
  } else if ((is_type("rock")) && (zoom>=14)) {
    if (has_attribute("water_level")) {
      attribute_switch("water_level")
      attribute_case("covers") symbol("rock_c");
      attribute_case("awash") symbol("rock_a");
      attribute_default symbol("rock");
      end_switch;
    } else symbol("rock");
  }
}

object_rules(waterways) {
  if (has_item_attribute("name") && (zoom >= 14)) {
    int ref = line("stroke:none;fill:none");
    line_text(item_attribute("name"), "font-family:Arial;font-weight:bold;font-size:80;text-anchor:middle", 0.5, 15, ref);
  }
}

object_rules(pipelines) {
  if (is_type("pipeline_submarine") && (zoom >= 14)) line_symbols("pipeline", 1.0, NULL, 0);
  if (is_type("pipeline_overhead") && (zoom >= 14)) {
//   line_symbols("pipeline", 1.0, NULL, 0);
  }
}

object_rules(cables) {
  if (is_type("cable_submarine") && (zoom >= 14)) line_symbols("cable", 0.0, NULL, 0);
  if (is_type("cable_overhead") && (zoom >= 14)) {
    if (attribute_test("category", "power|transmission")) {
    } else {
      
    }
//   line_symbols("cable", 0.0, NULL, 0);
  }
}

object_rules(wrecks) {
  if (zoom >= 14) {
    if (has_attribute("category")) {
      attribute_switch("category")
      attribute_case("dangerous|mast_showing") symbol("wreck_d");
      attribute_case("hull_showing") symbol("wreck_s");
      attribute_default symbol("wreck_nd");
      end_switch
    } else {
      symbol("wreck_nd");
    }
  }
}

rules {
  
  type("") ;
  type("shoreline_construction") object(shoreline);
  //  type("pipeline_submarine") object(pipelines);
  //  type("cable_submarine") object(cables);
  type("pipeline_overhead") object(pipelines);
  type("cable_overhead") object(cables);
  type("separation_zone") object(separation);
  type("separation_crossing") object(separation);
  type("separation_roundabout") object(separation);
  type("separation_line") object(separation);
  type("separation_lane") object(separation);
  type("separation_boundary") object(separation);
  type("inshore_traffic_zone") object(separation);
  type("sandwaves") object(areas);
  type("production_area") object(areas);
  type("fairway") object(areas);
  type("dredged_area") object(areas);
  type("restricted_area") object(areas);
  type("precautionary_area") object(areas);
  type("military_area") object(areas);
  type("seaplane_landing_area") object(areas);
  type("sea_area") object(areas);
  type("weed") object(areas);
  type("obstruction") object(obstructions);
  type("rock") object(obstructions);
  type("marine_farm") object (areas);
  type("waterway_axis") object(waterways);

  type("recommended_track") object(transits);
  type("navigation_line") object(transits);

  type("harbour") object(harbours);
  type("anchorage") object(harbours);
  type("anchor_berth") object(harbours);
  type("distance_mark") object(distances);
  type("hulk") object(ports);
  type("landmark") object(landmarks);
  type("building") object(buildings);
  type("crane") object(ports);
  type("berth") object(harbours);
  type("mooring") object(moorings);
  type("notice") object(notices);
  type("small_craft_facility") object(marinas);
  type("bridge") object(bridges);
  type("pile") object(lights);
  type("light_minor") object(lights);
  type("light") object(lights);
  type("light_major") object(lights);
  type("light_vessel") object(floats);
  type("light_float") object(floats);
  type("signal_station_traffic") object(signals);
  type("signal_station_warning") object(signals);
  type("waterway_gauge") object(gauge);
  type("coastguard_station") object(signals);
  type("platform") object(platforms);
  type("radio_station") object(radios);
  type("radar_station") object(radios);
  type("rescue_station") object(signals);
  type("pilot_boarding") object(signals);
  type("wreck") object(wrecks);
  type("buoy_lateral") object(buoys);
  type("buoy_cardinal") object(buoys);
  type("buoy_isolated_danger") object(buoys);
  type("buoy_safe_water") object(buoys);
  type("buoy_special_purpose") object(buoys);
  type("buoy_waterway") object(buoys);
  type("buoy_installation") object(floats);
  type("beacon_lateral") object(beacons);
  type("beacon_cardinal") object(beacons);
  type("beacon_isolated_danger") object(beacons);
  type("beacon_safe_water") object(beacons);
  type("beacon_special_purpose") object(beacons);
  type("beacon_waterway") object(beacons);
  
}

