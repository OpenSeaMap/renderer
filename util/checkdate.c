//
//  Utility to scan OSM files for timestamps and return the latest one
//  
//
//  Created by Malcolm Herring on 04/08/2011.
//  Copyright 2011 Malcolm Herring. Some rights reserved.
//  This program, and all its integral parts, are released under
//  the GNU General Public License v2 or later.
//
//  The GPL v3 is accessible here:
//  http://www.gnu.org/licenses/gpl.html
//
//  The GPL v2 is accessible here:
//  http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	FILE *inf;
	char prev[] = "0000-00-00T00:00:00Z";
	char line[200];
	char *token;
	char *timestamp;
	
	inf = fopen(argv[1], "r");
	while (!feof(inf)) {
		fgets(line, 199, inf);
		token = strtok(line, " '\"");
		timestamp = NULL;
		while (token != NULL) {
			if (strcmp(token, "timestamp=") == 0) {
				timestamp = strtok(NULL, " '\"");
				break;
			}
			token = strtok(NULL, " '\"");
		}
		if ((timestamp != NULL) && (strcmp(timestamp, prev) > 0)) {
      strcpy(prev, timestamp);
    }
	}
	printf("%s\n", prev);
}
