/* Copyright 2014 Malcolm Herring
 * Copyright 2016 Consilium Marine & Safety AB, Palle Raabjerg <palle.raabjerg@consilium.se>
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * For a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

package jsearch;

import java.io.*;
import java.util.*;

public class Jsearch {
	public static class MapBB {
		public double minlat;
		public double minlon;
		public double maxlat;
		public double maxlon;
	}
	
	public static void main(String[] args) throws Exception {
		HashMap<Long, Boolean> cnodes = new HashMap<Long, Boolean>();
		HashMap<Long, Boolean> cways = new HashMap<Long, Boolean>();
		HashMap<Long, Boolean> crels = new HashMap<Long, Boolean>();
		HashMap<Long, Boolean> nnodes = new HashMap<Long, Boolean>();
		HashMap<Long, Boolean> nways = new HashMap<Long, Boolean>();
		HashMap<Long, Boolean> nrels = new HashMap<Long, Boolean>();
		String dir = args[0];
		long id = 0;
		HashMap<Integer, Boolean> z9s = new HashMap<Integer, Boolean>();
		HashMap<Integer, Boolean> z10s = new HashMap<Integer, Boolean>();
		HashMap<Integer, Boolean> z11s = new HashMap<Integer, Boolean>();
		HashMap<Integer, Boolean> z12s = new HashMap<Integer, Boolean>();
		BufferedReader in = new BufferedReader(new FileReader(dir + "diffs"));
		String ln;
		while ((ln = in.readLine()) != null) {
			for (String token : ln.split("[ ]+")) {
				if (token.matches("^id=.+")) {
					id = Long.parseLong(token.split("[\"\']")[1]);
					break;
				}
			}
			if (ln.matches("^<.+")) {
				if (ln.contains("<node")) {
					cnodes.put(id, true);
				} else if (ln.contains("<way")) {
					cways.put(id, true);
				} else if (ln.contains("<relation")) {
					crels.put(id, true);
				}
			} else if (ln.matches("^>.+")) {
				if (ln.contains("<node")) {
					nnodes.put(id, true);
				} else if (ln.contains("<way")) {
					nways.put(id, true);
				} else if (ln.contains("<relation")) {
					nrels.put(id, true);
				}
			}
		}
		in.close();
		
		boolean next = false;
		do {
			if (next) {
				in = new BufferedReader(new FileReader(dir + "next.osm"));
			} else {
				in = new BufferedReader(new FileReader(dir + "world.osm"));
			}
			boolean inOsm = false;
			boolean inNode = false;
			boolean inWay = false;
			boolean inRel = false;
			ArrayList<String> buf = new ArrayList<String>();
			while ((ln = in.readLine()) != null) {
				if (inOsm) {
					if (inNode) {
						if (ln.contains("</node")) {
							buf.add(ln);
							inNode = false;
						}
					} else if (ln.contains("<node")) {
						buf.add(ln);
						if (!ln.contains("/>")) {
							inNode = true;
						}
					} else if (inWay) {
						if (ln.contains("<nd")) {
							buf.add(ln);
						}
						if (ln.contains("</way")) {
							buf.add(ln);
							inWay = false;
						}
					} else if (ln.contains("<way")) {
						buf.add(ln);
						if (!ln.contains("/>")) {
							inWay = true;
						}
					} else if (inRel) {
						if (ln.contains("<member")) {
							String type = "";
							String role = "";
							long ref = 0;
							for (String token : ln.split("[ ]+")) {
								if (token.matches("^ref=.+")) {
									ref = Long.parseLong(token.split("[\"\']")[1]);
								} else if (token.matches("^type=.+")) {
									type = (token.split("[\"\']")[1]);
								} else if (token.matches("^role=.+")) {
									role = (token.split("[\"\']")[1]);
								}
							}
							if ((role.equals("outer") || role.equals("inner")) && type.equals("way")) {
								if (next) {
									nways.put(ref, true);
								} else {
									cways.put(ref, true);
								}
							}
						}
						if (ln.contains("</relation")) {
							inRel = false;
						}
					} else if (ln.contains("<relation")) {
						for (String token : ln.split("[ ]+")) {
							if (token.matches("^id=.+")) {
								id = Long.parseLong(token.split("[\"\']")[1]);
							}
						}
						if (((next && nrels.containsKey(id)) || (!next && crels.containsKey(id))) && !ln.contains("/>")) {
							inRel = true;
						}
					} else if (ln.contains("</osm")) {
						buf.add(ln);
						inOsm = false;
						break;
					}
				} else if (ln.contains("<osm")) {
					buf.add(ln);
					inOsm = true;
				}
			}
			in.close();
			inOsm = false;
			inWay = false;
			for (String line : buf) {
				ln = line;
				if (inOsm) {
					if (inWay) {
						if (ln.contains("<nd")) {
							for (String token : ln.split("[ ]+")) {
								if (token.matches("^ref=.+")) {
									id = Long.parseLong(token.split("[\"\']")[1]);
								}
							}
							if (next) {
								nnodes.put(id, true);
							} else {
								cnodes.put(id, true);
							}
						}
						if (ln.contains("</way")) {
							inWay = false;
						}
					} else if (ln.contains("<way")) {
						for (String token : ln.split("[ ]+")) {
							if (token.matches("^id=.+")) {
								id = Long.parseLong(token.split("[\"\']")[1]);
							}
						}
						if (((next && nways.containsKey(id)) || (!next && cways.containsKey(id))) && !ln.contains("/>")) {
							inWay = true;
						}
					} else if (ln.contains("</osm")) {
						inOsm = false;
						break;
					}
				} else if (ln.contains("<osm")) {
					inOsm = true;
				}
			}
			for (String line : buf) {
				ln = line;
				if (ln.contains("<node")) {
					Double lat = 0.0;
					Double lon = 0.0;
					for (String token : ln.split("[ ]+")) {
						if (token.matches("^id=.+")) {
							id = Long.parseLong(token.split("[\"\']")[1]);
						} else if (token.matches("^lat=.+")) {
							lat = Double.parseDouble(token.split("[\"\']")[1]);
						} else if (token.matches("^lon=.+")) {
							lon = Double.parseDouble(token.split("[\"\']")[1]);
						}
					}
					if ((next && nnodes.containsKey(id)) || (!next && cnodes.containsKey(id))) {
						int xtile = TileConversion.lon2xtile(lon, 12);
						int ytile = TileConversion.lat2ytile(lat, 12);
						z9s.put(((xtile / 8) * 512) + (ytile / 8), true);
						z10s.put(((xtile / 4) * 1024) + (ytile / 4), true);
						z11s.put(((xtile / 2) * 2048) + (ytile / 2), true);
						for (int x = xtile - 1; x <= xtile + 1; x++) {
							for (int y = ytile - 1; y <= ytile + 1; y++) {
								if ((y >= 0) && (y <= 4095))
									z12s.put((((x < 0) ? 4095 : (x > 4095) ? 0 : x) * 4096) + y, true);
							}
						}
					}
				}
			}
			next = !next;
		} while (next);
		
		HashMap<Integer, Boolean> z9sClone = new HashMap<Integer, Boolean>(z9s);
		for (int t : z9sClone.keySet()) {
			String z9nam = dir + "tmp/" + (t / 512) + "-" + (t % 512) + "-9.osm";
			if (!(new File(z9nam).exists())) {
				makez9osm(t, dir, z9nam);
			}

			// Add additional nodes to ways
			File inputOSM = new File(z9nam);
			File tempOutput = new File(z9nam + ".output");
			NodeAdder nodeAdder = new NodeAdder(z9s, z10s, z11s, z12s);
			HashMap<Integer, Boolean> addedz9s = nodeAdder.addNodes(inputOSM, tempOutput);
			tempOutput.renameTo(inputOSM);

			// Build any additional z9 osm files
			for (int u : addedz9s.keySet()) {
				String z9name = dir + "tmp/" + (u / 512) + "-" + (u % 512) + "-9.osm";
			
				if (!(new File(z9name).exists())) {
					makez9osm(u, dir, z9name);
				}
			}
		}
		for (int t : z10s.keySet()) {
			int x = (t / 1024) * 4;
			int y = (t % 1024) * 4;
			MapBB bb = new MapBB();
			bb.minlon = TileConversion.tile2lon((x + 4094) % 4096, 12);
			bb.maxlon = TileConversion.tile2lon((x + 6) % 4095, 12);
			bb.minlat = TileConversion.tile2lat(Math.min((y + 6), 4095), 12);
			bb.maxlat = TileConversion.tile2lat(Math.max((y - 2), 0), 12);
			ArrayList<String> ext = Extract.extractData(dir + "tmp/" + ((t / 1024) / 2) + "-" + ((t % 1024) / 2) + "-9.osm", bb);
			PrintStream out = new PrintStream(dir + "tmp/" + (t / 1024) + "-" + (t % 1024) + "-10.osm");
			for (String line : ext) {
				out.println(line);
			}
			out.close();
		}
		for (int t : z11s.keySet()) {
			int x = (t / 2048) * 2;
			int y = (t % 2048) * 2;
			MapBB bb = new MapBB();
			bb.minlon = TileConversion.tile2lon((x + 4094) % 4096, 12);
			bb.maxlon = TileConversion.tile2lon((x + 4) % 4095, 12);
			bb.minlat = TileConversion.tile2lat(Math.min((y + 4), 4095), 12);
			bb.maxlat = TileConversion.tile2lat(Math.max((y - 2), 0), 12);
			String z11nam = dir + "tmp/" + (t / 2048) + "-" + (t % 2048) + "-11.osm";
			String z10nam = dir + "tmp/" + ((t / 2048) / 2) + "-" + ((t % 2048) / 2) + "-10.osm";
			ArrayList<String> ext = Extract.extractData(z10nam, bb);
			PrintStream out = new PrintStream(z11nam);
			for (String line : ext) {
				out.println(line);
			}
			out.close();
			for (int i = (x+4095)%4096; i < x+3; i = i+1) {
				for (int j = Math.max(y-1, 0); j < y+3; j = Math.min(j+1, 4095)) {
					if (z12s.containsKey(i*4096+j)) {
						z12s.remove(i*4096+j);
						bb = new MapBB();
						bb.minlon = TileConversion.tile2lon((i + 4095) % 4096, 12);
						bb.maxlon = TileConversion.tile2lon((i + 2) % 4095, 12);
						bb.minlat = TileConversion.tile2lat(Math.min((j + 2), 4095), 12);
						bb.maxlat = TileConversion.tile2lat(Math.max((j - 1), 0), 12);
						String z12nam = dir + "tmp/" + i + "-" + j + "-12.osm";
						ext = Extract.extractData(z11nam, bb);
						out = new PrintStream(z12nam);
						for (String line : ext) {
							out.println(line);
						}
						out.close();
					}
				}
			}
		}
		System.exit(0);
	}

	static private void makez9osm(int t, String dir, String z9nam) throws IOException {
		int x = (t / 512) * 8;
		int y = (t % 512) * 8;
		MapBB bb = new MapBB();
		bb.minlon = TileConversion.tile2lon((x + 4094) % 4096, 12);
		bb.maxlon = TileConversion.tile2lon((x + 10) % 4095, 12);
		bb.minlat = TileConversion.tile2lat(Math.min((y + 10), 4095), 12);
		bb.maxlat = TileConversion.tile2lat(Math.max((y - 2), 0), 12);
		ArrayList<String> ext = Extract.extractData(dir + "next.osm", bb);
		PrintStream out = new PrintStream(z9nam);
		for (String line : ext) {
			out.println(line);
		}
		out.close();
	}
	
	MapBB tile2bb(final int x, final int y, final int zoom) {
		MapBB bb = new MapBB();
		bb.maxlat = TileConversion.tile2lat(y, zoom);
		bb.minlat = TileConversion.tile2lat(y + 1, zoom);
		bb.minlon = TileConversion.tile2lon(x, zoom);
		bb.maxlon = TileConversion.tile2lon(x + 1, zoom);
		return bb;
	}
}
