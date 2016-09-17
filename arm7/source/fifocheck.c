#include <nds.h>

void fifocheck (void) {
	if(fifoCheckValue32(FIFO_USER_06)) {
		if(fifoCheckValue32(FIFO_USER_04)) { REG_SCFG_CLK = 0x0181; }
		if(fifoCheckValue32(FIFO_USER_05)) { REG_SCFG_EXT = 0x13A50000; }
		// Tell arm9 it has finished
		fifoSendValue32(FIFO_USER_07, 1);
		fifoSendValue32(FIFO_USER_06, 0);
	}
}

