/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_seek01.c*/

#include <aio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int wd;
static const int mb = 1024 * 1024;
static const char *file = "test file";

void dl_exit(int code){
	close(wd);
	exit(code);
}

int main(int argc, char *argv[]){
	char b;
	b = 0;
	wd = 0;
	wd = open(file, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IWGRP|S_IROTH);
	if(wd == -1){
		printf("Nepavyko sukurti failo\n");
		dl_exit(1);
	}
	lseek(wd, mb, SEEK_SET);
	write(wd, &b, 1);
	dl_exit(0);
	return 0;
}
