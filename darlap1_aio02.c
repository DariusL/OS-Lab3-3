/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_aio02.c*/

#include <aio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

static const int bufferSize = 1048576;

int rd;

void dl_exit(int code){
	close(rd);
	exit(code);
}

int dl_read_bytes(int desc, struct aiocb *block, char *buf, int bufOffset, int bytes){
	memset((void*)block, 0, sizeof(struct aiocb));
	block->aio_fildes = desc;
	block->aio_buf = buf + bufOffset;
	block->aio_nbytes = bytes;
	block->aio_offset = 0;
	if(aio_read(block) != 0){
		printf("Kazkas sprogo pradedant skaityt\n");
		return -1;
	}
	return 0;
}

int dl_read_wait(struct aiocb *block){
	const struct aiocb *temp[1];
	temp[0] = block;
	if(aio_suspend(temp, 1, NULL) != 0){
		printf("Kazkas sprogo baigiant skaityt\n");
		return -1;
	}
	return aio_return(block);
}

int main(){
	struct aiocb block;
	char *buf;
	int n = 0, t;
	buf = malloc(bufferSize);
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	rd = open("/dev/urandom", O_RDONLY);
	if(rd == -1){
		printf("Kazkas sprogo atidarant faila\n");
		dl_exit(1);
	}
	while(n < bufferSize){
		if(dl_read_bytes(rd, &block, buf, n, bufferSize - n) != 0)
			dl_exit(1);
		t = dl_read_wait(&block);
		if(t > 0){
			n += t;
			printf("Perskaityta %d baitu\n", n);
		}else
			dl_exit(1);
	}
	dl_exit(0);
	return 0;
}
