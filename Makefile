all: *.rel myIO.lib final clean

final : myIO.lib final_project.c
	sdcc final_project.c myIO.lib
	packihx final_project.ihx > final_project.hex

myIO.lib : *.rel
	sdar -rc myIO.lib Keypad4x4.rel 
	sdranlib myIO.lib

*.rel : Keypad4x4.c 
	sdcc -c Keypad4x4.c
	
trash = *.lk *.lst *.rel *.asm *.ihx *.rst *.sym *.map *.mem

clean:
	del $(trash)
 