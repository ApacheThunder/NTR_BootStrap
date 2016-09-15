/*
    NitroHax -- Cheat tool for the Nintendo DS
    Copyright (C) 2008  Michael "Chishm" Chisholm

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <nds.h>

#include "bios_decompress_callback.h"

#include "fade00.h"
#include "fade01.h"
#include "fade02.h"
#include "fade03.h"
#include "fade04.h"

#include "suberror00.h"
#include "suberror01.h"
#include "suberror02.h"
#include "suberror03.h"
#include "suberror04.h"
#include "suberror05.h"
#include "suberror06.h"

#include "errorsplash.h"

#define CONSOLE_SCREEN_WIDTH 32
#define CONSOLE_SCREEN_HEIGHT 24

void FadeFX() {

 	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
	
	swiDecompressLZSSVram ((void*)fade00Tiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
	swiDecompressLZSSVram ((void*)fade00Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE[0], fade00Pal, fade00PalLen);
	vramcpy_ui (&BG_PALETTE_SUB[0], fade00Pal, fade00PalLen);

	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
	
 	swiDecompressLZSSVram ((void*)fade01Tiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
	swiDecompressLZSSVram ((void*)fade01Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE[0], fade01Pal, fade01PalLen);
	vramcpy_ui (&BG_PALETTE_SUB[0], fade01Pal, fade01PalLen);

	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
	
 	swiDecompressLZSSVram ((void*)fade02Tiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
	swiDecompressLZSSVram ((void*)fade02Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE[0], fade02Pal, fade02PalLen);
	vramcpy_ui (&BG_PALETTE_SUB[0], fade02Pal, fade02PalLen);

	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }

 	swiDecompressLZSSVram ((void*)fade03Tiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
	swiDecompressLZSSVram ((void*)fade03Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE[0], fade03Pal, fade03PalLen);
	vramcpy_ui (&BG_PALETTE_SUB[0], fade03Pal, fade03PalLen);

	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }

 	swiDecompressLZSSVram ((void*)fade04Tiles, (void*)CHAR_BASE_BLOCK(2), 0, &decompressBiosCallback);
	swiDecompressLZSSVram ((void*)fade04Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE[0], fade04Pal, fade04PalLen);
	vramcpy_ui (&BG_PALETTE_SUB[0], fade04Pal, fade04PalLen);	

	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
}

void GenericError() {
	
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
	
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
	
	FadeFX();

	swiDecompressLZSSVram ((void*)suberror00Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], suberror00Pal, suberror00PalLen);
	
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
	
	swiDecompressLZSSVram ((void*)suberror01Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], suberror01Pal, suberror01PalLen);
	
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }

	swiDecompressLZSSVram ((void*)suberror02Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], suberror02Pal, suberror02PalLen);
	
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }

	swiDecompressLZSSVram ((void*)suberror03Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], suberror03Pal, suberror03PalLen);
	
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
	
	// Wait for input, then fade out
	do { swiWaitForVBlank(); scanKeys(); } while (!keysDown());

 	swiDecompressLZSSVram ((void*)suberror04Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], suberror04Pal, suberror04PalLen);
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }

 	swiDecompressLZSSVram ((void*)suberror05Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], suberror05Pal, suberror05PalLen);
	
 	swiDecompressLZSSVram ((void*)suberror06Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], suberror06Pal, suberror06PalLen);
	
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
	
	swiDecompressLZSSVram ((void*)fade04Tiles, (void*)CHAR_BASE_BLOCK_SUB(2), 0, &decompressBiosCallback);
	vramcpy_ui (&BG_PALETTE_SUB[0], fade04Pal, fade04PalLen);
	
	for (int i = 0; i < 2; i++) { swiWaitForVBlank(); }
}

