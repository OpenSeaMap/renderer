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
    literal_case("tower|beacon|stake") {
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

object_rules(lights) {
  if (is_type("light_major|light_minor|light")) {
    if (is_type("light_major")) {
      symbol("light_major");
      if (has_object("fog_signal")) object(fogs);
    }
    if (is_type("light_minor|light")) {
      if ((zoom >= 14) || ((zoom >= 11) && (has_object("radar_transponder"))))
        symbol("light_minor");
      if (has_object("fog_signal")) object(fogs);
    }
    if (has_object("radar_transponder")) object(rtbs);
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
        ((zoom >= 11) && (has_object("radar_transponder"))))
      light_flare;
    if (zoom >= 15) {
      make_char_string("light");
      text(string, "font-family:Arial; font-weight:normal; font-size:70; text-anchor:start", 60, -10);
      free_string;
      if (has_item_attribute("name") && is_type("light_major|light_minor"))
        text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -70);
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
    literal_case("tower|beacon|stake") object(top_colours, attribute("shape"), 0, 73, 0);
    literal_case("float|super-buoy") object(top_colours, attribute("shape"), 0, 44, 0);
    literal_case("can|conical|spherical|barrel") object(top_colours, attribute("shape"), -15.3, 37, 18.5);
    end_switch
  }
}

object_rules(mark_colours, char* default_shape) {
  make_string("");
  if (attribute_test("shape", "tower|pillar|spar|barrel|can|conical|spherical|super-buoy")) {
    add_string(attribute("shape"));
  } else {
    add_string(default_shape)
  }
  if (has_attribute("colour")) {
    int n = attribute_count("colour");
    char panel[4] = {'0'+n, 0, 0, 0};
    attribute_switch("colour_pattern")
    attribute_case("horizontal") panel[1] = 'H';
    attribute_case("vertical") panel[1] = 'V';
    attribute_case("diagonal") panel[1] = 'D';
    attribute_case("squared") panel[1] = 'S';
    attribute_case("cross") panel[1] = 'C';
    attribute_case("saltire") panel[1] = 'X';
    end_switch;
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
  if (has_object("radar_reflector")) object(refls, string);
  if (has_object("light")) object(lights);
  if ((zoom >= 15) && has_item_attribute("name")) {
    literal_switch(string)
    literal_case("beacon") text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 20, -50);
    literal_case("pillar|spar") text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 50, -50);
    literal_case("float|tower") text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 60, -50);
    literal_case("spherical|can|conical|float|tower|barrel") {
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
      if ((zoom >= 15) && (has_item_attribute("name"))) text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 20, -50);
    }
    attribute_case("perch") {
      attribute_switch("category")
      attribute_case("port") symbol("perchP");
      attribute_case("starboard") symbol("perchS");
      attribute_default symbol("stake");
      end_switch
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
  if (zoom >= 12) {
    attribute_switch("category")
    attribute_case("training_wall") {}
    if (attribute_test("water_level", "covers")) {
      ref = line("stroke-width:10; stroke-dasharray:40,40; stroke-linecap:butt; stroke-linejoin:round; stroke:#000000; fill:none");
      if (zoom >= 15) line_text("(covers)", "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.5, 50, ref);
    } else line("stroke-width:10; stroke-linecap:butt; stroke-linejoin:round; stroke:#000000; fill:none");
    if (zoom >= 15) line_text("Training Wall", "font-family:Arial; font-weight:normal; font-size:80; text-anchor:middle", 0.5, -20, ref);
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
    line_symbols("lane_arrow", 0.5);
  } else if (is_type("separation_boundary")) {
    line("stroke-width: 20; stroke-dasharray:40,40; stroke-linecap:butt; stroke-linejoin:round; fill:none; stroke:#c480ff; stroke-opacity:0.5");
  }
}

object_rules(transits) {
  int ref;
  if (zoom >= 12) {
    if (is_type("recommended_track")) ref = line("stroke-width:8; stroke:#000000; stroke-linecap:butt; fill:none");
    else if (is_type("navigation_line")) ref = line("stroke-width:8; stroke-dasharray:20,20; stroke:#000000; stroke-linecap:butt; fill:none");
  }
  if (zoom >= 15) {
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
  if (has_object("light")) object(lights);
}

object_rules(gauge) {
  if (zoom >= 14) symbol("tide_gauge");
}

object_rules(landmarks) {
  if ((zoom >= 12) && (has_attribute("function"))) {
    attribute_switch("function")
    attribute_case("pagoda") symbol("temple");
    attribute_case("mosque") symbol("minaret");
    attribute_case("marabout") symbol("spire");
    attribute_default symbol(attribute("function"));
    end_switch
  }
  if ((zoom >= 12) && (has_attribute("category"))) {
    attribute_switch("category")
    attribute_case("statue|column|obelisk") symbol("monument");
    attribute_case("tower") {
      if (attribute_test("function", "church"))
        symbol("church_tower");
      else
        symbol("land_tower");
    }
    attribute_case("cross") symbol("land_cross");
    attribute_case("radar_scanner") symbol("signal_station");
    attribute_default symbol(attribute("category"));
    end_switch
  }
  if (!has_attribute("function") && !has_attribute("category") && has_object("light")) {
    symbol("lighthouse");
    if ((zoom >= 15) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -70);
  } else {
    if ((zoom >= 15) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:start", 60, -50);
  }
  if (has_object("fog_signal")) object(fogs);
  if (has_object("radar_transponder")) object(rtbs);
  if (has_object("radar_station") && (zoom >= 12)) symbol("radar_station");
  if (has_object("light")) object(lights);
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
  if (has_object("light")) object(lights);
}

object_rules(notices) {
  if (zoom >= 14)
    symbol_notice;
}

object_rules(marinas) {
  if (zoom >= 16) symbol_cluster("small_craft_facility");
}

object_rules(locks) {
  if ((zoom>=13) && is_type("lock_basin|lock_basin_part")) symbol("lock");
  if ((zoom>=15) && is_type("gate")) symbol("lock_gate");
}

object_rules(distances) {
  if ((zoom>=16) && (has_attribute("category"))) {
    attribute_switch("category")
    attribute_case("installed") symbol("distance_i");
    attribute_default symbol("distance_u");
    end_switch
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
  }
}

object_rules(harbours) {
  if (is_type("anchorage")) symbol("anchorage");
  if ((zoom >= 16) && is_type("anchor_berth")) symbol("anchor_berth");
  if ((zoom >= 12) && is_type("harbour") && attribute_test("category", "marina|yacht")) {
    symbol("marina");
    if ((zoom >= 15) && (has_item_attribute("name")))
      text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, -90);
  }
}

object_rules(areas) {
  if (is_type("fairway")) {
    if (zoom < 16) area("stroke:#c480ff;stroke-width:8;stroke-dasharray:50,50;fill:#ffffff;fill-opacity:0.25");
    else area("stroke:#c480ff;stroke-width:8;stroke-dasharray:50,50;fill:none");
  }
  if (is_type("dredged_area")) {
    if (zoom < 16 ) area("stroke:#000000;stroke-width:10;stroke-dasharray:50,50;fill:#ffffff;fill-opacity:0.25");
    else area("stroke:#000000;stroke-width:10;stroke-dasharray:50,50;fill:none");
    if ((zoom >= 12) && has_item_attribute("name"))
      text(item_attribute("name"), "font-family:Arial;font-weight:normal;font-size:100;text-anchor:middle", 0, 0);
  }
  if (is_type("restricted_area")) line_symbols("restricted_line", 1.0);
  if (is_type("production_area")) {
    if (attribute_test("category", "wind_farm")) {
      symbol("wind_farm");
      area("stroke-width:20;stroke-dasharray:40,40;fill:none;stroke:#000000");
      if ((zoom >= 15) && (has_item_attribute("name")))
        text(item_attribute("name"), "font-family:Arial; font-weight:bold; font-size:80; text-anchor:middle", 0, 70);
    }
  }
  if (is_type("sea_area") && has_item_attribute("name")) {
    if (has_attribute("category")) {
      make_string("");
      attribute_switch("category")
      attribute_case("reach") { if (zoom >= 10) add_string("font-family:Arial;font-weight:normal;font-style:italic;font-size:150;text-anchor:middle") }
      attribute_case("bay") { if (zoom >= 12) add_string("font-family:Arial;font-weight:normal;font-style:italic;font-size:150;text-anchor:middle") }
      attribute_case("shoal") { if (zoom >= 14) add_string("font-family:Arial;font-weight:normal;font-style:italic;font-size:150;text-anchor:middle") }
      attribute_case("gat|narrows") { if (zoom >= 12) add_string("font-family:Arial;font-weight:normal;font-style:italic;font-size:100;text-anchor:middle") }
      end_switch
      if (strlen(string) > 0) {
        int ref = line("stroke:none;fill:none");
        if (ref != 0) {
          line_text(item_attribute("name"), string, 0.5, 0, ref);
          if (attribute_test("category", "shoal"))
            line_text("(Shoal)", "font-family:Arial;font-weight:normal;font-size:100;text-anchor:middle", 0.5, 75, ref);
        } else {
          text(item_attribute("name"), string, 0, 0);
          if (attribute_test("category", "shoal"))
            text("(Shoal)", "font-family:Arial;font-weight:normal;font-size:100;text-anchor:middle", 0, 75);
        }
      }
      free_string
    }
  }
}

object_rules(waterways) {
  if (has_item_attribute("name") && (zoom >= 14)) {
    int ref = line("stroke:none;fill:none");
    line_text(item_attribute("name"), "font-family:Arial;font-weight:bold;font-size:80;text-anchor:middle", 0.5, 15, ref);
  }
}

object_rules(pipelines) {
  line_symbols("pipeline", 1.0);
}

object_rules(cables) {
  line_symbols("cable", 0.0);
}

rules {
  
  type("shoreline_construction") object(shoreline);
  type("pipeline_submarine") object(pipelines);
//  type("cable_submarine") object(cables);
  type("separation_zone") object(separation);
  type("separation_crossing") object(separation);
  type("separation_roundabout") object(separation);
  type("separation_line") object(separation);
  type("separation_lane") object(separation);
  type("separation_boundary") object(separation);
  type("sandwaves") object(areas);
  type("production_area") object(areas);
  type("fairway") object(areas);
  type("dredged_area") object(areas);
  type("restricted_area") object(areas);
  type("sea_area") object(areas);
  type("waterway_axis") object(waterways);

  type("recommended_track") object(transits);
  type("navigation_line") object(transits);

  type("harbour") object(harbours);
  type("anchorage") object(harbours);
  type("anchor_berth") object(harbours);
  type("lock_basin") object(locks);
  type("lock_basin_part") object(locks);
  type("gate") object(locks);
  type("distance_mark") object(distances);
  type("landmark") object(landmarks);
  type("crane") object(ports);
  type("mooring") object(moorings);
  type("notice") object(notices);
  type("small_craft_facility") object(marinas);
  type("bridge") object(bridges);
  type("light_major") object(lights);
  type("light_minor") object(lights);
  type("light") object(lights);
  type("light_vessel") object(floats);
  type("light_float") object(floats);
  type("signal_station_traffic") object(signals);
  type("signal_station_warning") object(signals);
  type("waterway_gauge") object(gauge);
  type("coastguard_station") object(signals);
  type("platform") object(platforms);
  type("radio_station") object(signals);
  type("radar_station") object(signals);
  type("rescue_station") object(signals);
  type("pilot_boarding") object(signals);
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

