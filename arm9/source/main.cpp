/*-----------------------------------------------------------------

 Copyright (C) 2010  Dave "WinterMute" Murphy

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

------------------------------------------------------------------*/

#include <nds.h>
#include <fat.h>
#include <limits.h>

#include <stdio.h>
#include <stdarg.h>

#include <nds/fifocommon.h>

#include "nds_loader_arm9.h"
#include "inifile.h"
#include "bootsplash.h"
#include "errorsplash.h"

using namespace std;

void runFile(string filename) {
	vector<char*> argarray;

	if ( strcasecmp (filename.c_str() + filename.size() - 5, ".argv") == 0) {
		FILE *argfile = fopen(filename.c_str(),"rb");
		char str[PATH_MAX], *pstr;
		const char seps[]= "\n\r\t ";

		while( fgets(str, PATH_MAX, argfile) ) {
			// Find comment and end string there
			if( (pstr = strchr(str, '#')) )
				*pstr= '\0';

			// Tokenize arguments
			pstr= strtok(str, seps);

			while( pstr != NULL ) {
				argarray.push_back(strdup(pstr));
				pstr= strtok(NULL, seps);
			}
		}
		fclose(argfile);
		filename = argarray.at(0);
	} else {
		argarray.push_back(strdup(filename.c_str()));
	}

	if ( strcasecmp (filename.c_str() + filename.size() - 4, ".nds") != 0 || argarray.size() == 0 ) {
	} else {
		int err = runNdsFile (argarray[0], argarray.size(), (const char **)&argarray[0]);
	}
}

#define REG_SCFG_CLK	(*(vu32*)0x4004004)
#define REG_SCFG_EXT	(*(vu32*)0x4004008)
#define REG_SCFG_MC		(*(vu32*)0x4004010)

int main(int argc, char **argv) {

	REG_SCFG_CLK = 0x85;
	
	bool UseNTRSplash = true;
	bool TriggerExit = false;
	
	if (fatInitDefault()) {

		CIniFile bootstrapini( "sd:/nds/ntr_bootstrap.ini" );

		std::string	ndsPath = bootstrapini.GetString( "NTR-BOOTSTRAP", "NDS", "");

		if(bootstrapini.GetInt("NTR-BOOTSTRAP","NTRCLOCK",0) == 0) { UseNTRSplash = false; }

		if(bootstrapini.GetInt("NTR-BOOTSTRAP","DISABLEANIMATION",0) == 0) { BootSplashInit(UseNTRSplash); }

		if(bootstrapini.GetInt("NTR-BOOTSTRAP","NTRCLOCK",0) == 1) {
		REG_SCFG_CLK = 0x80;
		fifoSendValue32(FIFO_USER_04, 1);
		}

		if(bootstrapini.GetInt("NTR-BOOTSTRAP","RESETSLOT1",0) == 1) {
			if(REG_SCFG_MC == 0x11) { 
				consoleDemoInit();
				printf("Please insert a cartridge...\n");
				do { swiWaitForVBlank(); } 
				while (REG_SCFG_MC == 0x11);
			}
			fifoSendValue32(FIFO_USER_02, 1);
		}

		fifoSendValue32(FIFO_USER_01, 1);
		fifoWaitValue32(FIFO_USER_03);
		for (int i = 0; i < 20; i++) { swiWaitForVBlank(); }

		if(bootstrapini.GetInt("NTR-BOOTSTRAP","LOCKSCFG",0) == 1) {
			fifoSendValue32(FIFO_USER_05, 1); 
		} else {
			// REG_SCFG_EXT = 0x8307F100;
			REG_SCFG_EXT = 0x83000000;
		}

		runFile(ndsPath.c_str());
	} else {
		if( REG_SCFG_CLK = 0x80 ) { REG_SCFG_CLK = 0x85; }
		GenericError();
		TriggerExit = true;
	}
	
	while(1) { 
		swiWaitForVBlank(); 
		if(TriggerExit) { break; }
	}
	return 0;
}

