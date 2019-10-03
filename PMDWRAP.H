//PMD interrupt call wrapper functions 0.1

//For use with Turbo C/C++ 4.0J. MASTER.LIB is required

//Written by: spaztron64, 2019
//Special thanks to nmlgc for teaching me how to do the most basic stuff
//None of this would be possible without him. Give the man some love (and cash)

//IMPORTANT:
//When loading a new song file, call pmd_get_song_address() first. Only then
//call pmd_load_song_data(<file>) and pmd_play_loaded_song();


//Not all functions are implemented. More are coming soon

union REGS inregs, outregs, regs;
struct SREGS segregs;

void far *fpmd;

void pmd_set_volume(int volume){
	regs.h.ah= 0x19;
	regs.h.al= volume; //NOTE: Higher values lead to LOWER volumes
	int86(0x60, &regs, &regs);
}

void pmd_pause_song(){
	regs.h.ah= 0x1A;
	int86(0x60, &regs, &regs);
}

void pmd_unpause_song(){
	regs.h.ah= 0x1B;
	int86(0x60, &regs, &regs);
}

void pmd_get_song_address(){

	regs.h.ah = 0x06; //Get song address
	int86x(0x60, &regs, &regs, &segregs);
}


void pmd_play_loaded_song(){
	regs.h.ah = 0x00; //Start playing
	regs.w.bx = 0x00;
	int86(0x60, &regs, &regs);
}

void pmd_stop_song(){
	regs.h.ah = 0x01;
	int86(0x60, &regs, &regs);
	file_close();
}

void pmd_fade_song(int fadespeed){
	 regs.h.ah = 0x02;
	 regs.h.al = fadespeed;
	 //fadespeed is a signed value. This means that values from -127 to -1 are fade in
	 //while values from 1 to 127 are fade out.
	 //The further the value is from 0, the faster the fade operation.
	 int86(0x60, &regs, &regs);
}

void pmd_load_song_data(char* file){
	fpmd = MK_FP(segregs.ds,regs.w.dx);
	file_ropen(file);
	long size = file_size();
	file_lread(fpmd, size);
}
