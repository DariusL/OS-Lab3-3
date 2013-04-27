/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_mmap02.c*/

#include <aio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int rd;
int wd;

void dl_exit(int code){
	close(rd);
	close(wd);
	exit(code);
}

char *dl_mighty_map(int d, int size, int read){
	void *buf;
	if(!read){
		lseek(d, size - 1, SEEK_SET);
		write(d, &d, 1);
		buf = mmap(NULL, size, PROT_WRITE|PROT_READ, MAP_SHARED, d, 0);
	}else
		buf = mmap(NULL, size, PROT_READ, MAP_SHARED, d, 0);
	if(buf == MAP_FAILED){
		printf("mmap sprogo\n");
		dl_exit(1);
	}
	return buf;
}

void dl_mighty_unmap(void *buf, int size){
	if(munmap(buf, size) != 0){
		printf("munmap sprogo\n");
		dl_exit(1);
	}
}

int main(int argc, char *argv[]){
	int s;
	struct stat status;
	void *rm, *wm;
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	if(argc < 3){
		printf("Reikia 2 argumentu: skaitymui ir rasymui\n");
		dl_exit(1);
	}
	rd = open(argv[1], O_RDONLY);
	if(rd == -1){
		printf("Nepavyko atidaryti failo skaitymui\n");
		dl_exit(1);
	}
	wd = open(argv[2], O_RDWR|O_CREAT, S_IRWXU|S_IWGRP|S_IROTH);
	if(wd == -1){
		printf("Nepavyko sukurti/atidaryti antro failo\n");
		dl_exit(1);
	}
	fstat(rd, &status);
	s = status.st_size;
	printf("Dydis - %d baitu\n", s);
	rm = dl_mighty_map(rd, s, 1);
	wm = dl_mighty_map(wd, s, 0);
	memcpy(wm, rm, s);
	dl_mighty_unmap(rm, s);
	dl_mighty_unmap(wm, s);
	dl_exit(0);
	return 0;
}
