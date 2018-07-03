/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "Precompiled.h"
#include "globaldata.h"


#include <string.h>

typedef struct  {
	int exp;
	int pos;
}argpos;

argpos dpos[3];
//GK:Exclusive parameters per game
void M_initParam() {
	int		i;
	for (int j = 0; j < 3; j++) {
		dpos[j].pos = -1;
	}
	int o = 2;
	for (i = 0; i < ::g->myargc; i++)
	{
		if (!idStr::Icmp("-doom", ::g->myargv[i])) {
			dpos[o].pos = i;
			dpos[o].exp = retail;
			o--;
			continue;
		}

		if (!idStr::Icmp("-doom2", ::g->myargv[i])) {
			dpos[o].pos = i;
			dpos[o].exp = commercial;
			o--;
			continue;
		}

		if (!idStr::Icmp("-both", ::g->myargv[i])) {
			dpos[o].pos = i;
			o--;
		}
	}

}
//GK end



//
// M_CheckParm
// Checks for the given parameter
// in the program's command line arguments.
// Returns the argument number (1 to argc-1)
// or 0 if not present
int M_CheckParm (const char *check,bool offset)
{
    int		i;
	int c = 0;
    for (i = 1; i < ::g->myargc; i++)
    {
		if (!idStr::Icmp(check, ::g->myargv[i])) {
			if (offset && !c) {
					c++;
					continue;
			}
			//GK begin
			int p = 0;
			for (int j = 0; j < 3; j++) {
				if (dpos[j].pos == -1)
					continue;
				int res = i - dpos[j].pos;
				if (res == 0)
					return i;

				if (res > 0) {
					if (dpos[j].exp == ::g->gamemode || !dpos[j].exp) {
						p = i;
						break;
					}
					else {
						p = -1;
						break;
					}
				}
			}
			if (p > 0) {
				return p;
			}
			else if (p==-1){
				continue;
			}
			if (!offset) {
				return i;
			}
			else {
				continue;
			}

			}
		}
		//GK end
	return 0;
    }






