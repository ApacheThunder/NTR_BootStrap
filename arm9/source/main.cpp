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

#include "inifile.h"
#include "nds_loader_arm9.h"

#include "bios_decompress_callback.h"

#include "topLoad.h"
#include "subLoad.h"
#include "topError.h"
#include "subError.h"
#include "subPrompt.h"

#define CONSOLE_SCREEN_WIDTH 32
#define CONSOLE_SCREEN_HEIGHT 24

void vramcpy_ui (void* dest, const void* src, int size) 
{
	u16* destination = (u16*)dest;
	u16* source = (u16*)src;
	while (size > 0) {
		*destination++ = *source++;
		size-=2;
	}
}

void BootSplashInit() {

	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	vramSetBankA (VRAM_A_MAIN_BG_0x06000000);
	vramSetBankC (VRAM_C_SUB_BG_0x06200000);
	REG_BG0CNT = BG_MAP_BASE(0) | BG_COLOR_256 | BG_TILE_BASE(2);
	REG_BG0CNT_SUB = BG_MAP_BASE(0) | BG_COLOR_256 | BG_TILE_BASE(2);
	BG_PALETTE[0]=0;
	BG_PALETTE[255]=0xffff;
	u16* bgMapTop = (u16*)SCREEN_BASE_BLOCK(0);
	u16* bgMapSub = (u16*)SCREEN_BASE_BLOCK_SUB(0);
	for (int i = 0; i < CONSOLE_SCREEN_WIDTH*CONSOLE_SCREEN_HEIGHT; i++) {
		bgMapTop[i] = (u16)i;
		bgMapSub[i] = (u16)i;
	}

}

void LoadScreen() {

		// Display Load Screen
		swiDecompressLZSSVram ((void*)topLoadTiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
		swiDecompressLZSSVram ((void*)subLoadTiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
		vramcpy_ui (&BG_PALETTE[0], topLoadPal, topLoadPalLen);
		vramcpy_ui (&BG_PALETTE_SUB[0], subLoadPal, subLoadPalLen);

}

void CartridgePrompt() {

	// Display Load Screen
	swiDecompressLZSSVram ((void*)topLoadTiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
	swiDecompressLZSSVram ((void*)subPromptTiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE[0], topLoadPal, topLoadPalLen);
	vramcpy_ui (&BG_PALETTE_SUB[0], subPromptPal, subPromptPalLen);
	
	for (int i = 0; i < 20; i++) { swiWaitForVBlank(); }

}

int main( int argc, char **argv) {

	defaultExceptionHandler();

	BootSplashInit();

	if (fatInitDefault()) {
		
		LoadScreen();
		
		CIniFile NTRBootstrap( "sd:/nds/NTR_Bootstrap.ini" );
		
		std::string	ndsPath = NTRBootstrap.GetString( "NTR_BOOTSTRAP", "NDS", "");
		
		if(NTRBootstrap.GetInt("NTR_BOOTSTRAP","RESETSLOT1",0) == 0) {
			fifoSendValue32(FIFO_USER_01, 1);
		} else {
			if (REG_SCFG_MC == 0x11) {
				do { CartridgePrompt(); }
				while (REG_SCFG_MC == 0x11);
			}
			LoadScreen();
			fifoSendValue32(FIFO_USER_02, 1);
			fifoSendValue32(FIFO_USER_01, 1);
			fifoWaitValue32(FIFO_USER_03);
		}
		
		for (int i = 0; i < 60; i++) { swiWaitForVBlank(); }
		
		if(NTRBootstrap.GetInt("NTR_BOOTSTRAP","NTRTOUCH",0) == 0) { /* Do Nothing */ } else { fifoSendValue32(FIFO_USER_05, 1); }
		
		fifoSendValue32(FIFO_USER_04, 1);
		
		for (int i = 0; i < 60; i++) { swiWaitForVBlank(); }

		runNdsFile(ndsPath.c_str(), 0, NULL);
	} else {
		// Display Error Screen
		swiDecompressLZSSVram ((void*)topErrorTiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
		swiDecompressLZSSVram ((void*)subErrorTiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
		vramcpy_ui (&BG_PALETTE[0], topErrorPal, topErrorPalLen);
		vramcpy_ui (&BG_PALETTE_SUB[0], subErrorPal, subErrorPalLen);
		}
	while(1) { swiWaitForVBlank(); }
}

