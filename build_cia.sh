GAME_TITLE="NTR BOOTSTRAP"
GAME_SUBTITLE1="Stage3 BootStrap"
GAME_SUBTITLE2="Modification of NDS BootStrap"
GAME_INFO="BNTE 01 NTRBOOTSTRAP"

$DEVKITARM/bin/ndstool	-c ntr_bootstrap.nds -7 ntr_bootstrap.arm7.elf -9 ntr_bootstrap.arm9.elf -g $GAME_INFO -b icon.bmp  "$GAME_TITLE;$GAME_SUBTITLE1;$GAME_SUBTITLE2"  -r9 0x2000000 -r7 0x2380000 -e9 0x2000000 -e7 0x2380000
python patch_ndsheader_dsiware.py --mode dsi ntr_bootstrap.nds

./makerom.exe -srl ntr_bootstrap.nds