/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_rw01.c*/

#include <aio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int rd;
int wd;

int dl_parse_char(char *nr){
	int ret = 0;
	int tmp = 0;
	int i;
	for(i = 0; nr[i] != '\0'; i++){
		tmp = nr[i] - '0';
		if(tmp > 9 || tmp < 0)
			return 0;
		ret *= 10;
		ret += tmp;
	}
	return ret;
}

void dl_exit(int code){
	close(wd);
	close(rd);
	exit(code);
}

int main(int argc, char *argv[]){
	int b;
	int br;
	char *arr;
	rd = 0;
	wd = 0;
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	if(argc != 4){
		printf("1 argumentas - failas skaitymui\n2 argumentas - failas rasymui\n\
3 argumentas - kiek baitu kopijuot\n");
		dl_exit(1);
	}
	rd = open(argv[1], O_RDONLY);
	if(rd == -1){
		printf("Nepavyko atidaryti pirmo failo\n");
		dl_exit(1);
	}
	wd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IWGRP|S_IROTH);
	if(wd == -1){
		printf("Nepavyko sukurti antro failo\n");
		dl_exit(1);
	}
	b = dl_parse_char(argv[3]);
	if(b == 0){
		printf("Netinkamas baitu skaicius\n");
		dl_exit(1);
	}
	arr = malloc(b);
	br = read(rd, (void*)arr, b);
	if(br == 0){
		printf("Failas tuscias\n");
		dl_exit(1);
	}
	else if(br == -1){
		printf("Nepavyko perskaityti failo\n");
		dl_exit(1);
	}
	write(wd, (void*)arr, br);
	dl_exit(0);
	return 0;
}

