/*---------------------------------------------------------------------------------

default ARM7 core

Copyright (C) 2005 - 2010
	Michael Noland (joat)
	Jason Rogers (dovoto)
	Dave Murphy (WinterMute)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you
	must not claim that you wrote the original software. If you use
	this software in a product, an acknowledgment in the product
	documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source
	distribution.

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <nds/arm7/input.h>
#include <nds/system.h>

#include <maxmod7.h>

#include "fifocheck.h"

//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

void VblankHandler(void) {
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	// Switch to NTR Mode
	REG_SCFG_ROM = 0x703;
	REG_SCFG_CLK = 0x0187;
	REG_SCFG_EXT = 0x93AF0100;

	irqInit();
	fifoInit();

	// read User Settings from firmware
	readUserSettings();

	// Start the RTC tracking IRQ
	initClockIRQ();

	mmInstall(FIFO_MAXMOD);

	SetYtrigger(80);

	installSoundFIFO();
	installSystemFIFO();
	
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);   

	fifoWaitValue32(FIFO_USER_01);
	if(fifoCheckValue32(FIFO_USER_02)) { dsi_resetSlot1(); }
	fifoSendValue32(FIFO_USER_03, 1);

	while (1) { swiWaitForVBlank(); fifocheck(); }
}

