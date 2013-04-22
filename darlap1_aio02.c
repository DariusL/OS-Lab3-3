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

#define chunkSize 1024
#define chunkCount 1024
#define bufferSize 1048576

int rd;

void dl_exit(int code){
	close(rd);
	exit(code);
}

int dl_read_chunks(int desc, struct aiocb *blocks[], char *buf){
	struct aiocb *temp;
	int i;
	memset((void*)blocks, 0, sizeof(struct aiocb) * chunkCount);
	for(i = 0; i < chunkCount; i++){
		temp = blocks[i];
		temp->aio_fildes = desc;
		temp->aio_buf = buf + i * chunkSize;
		temp->aio_nbytes = chunkSize;
		temp->aio_offset = i * chunkSize;
		if(aio_read(temp) != 0){
			printf("Kazkas sprogo pradedant skaityt\n");
			return -1;
		}
	}
	return 0;
}

int dl_read_wait(struct aiocb *blocks[]){
	int i, n;
	struct aiocb *temp;
	const struct aiocb *arr[chunkCount];
	memcpy(arr, blocks, sizeof(struct aiocb) * chunkCount);
	n = 0;
	if(aio_suspend(arr, chunkCount, NULL) != 0){
		printf("Kazkas sprogo baigiant skaityt\n");
		return -1;
	}
	for(i = 0; i < chunkCount; i++){
		temp = blocks[i];
		n += aio_return(temp);
	}
	return n;
}

int main(){
	struct aiocb **blocks;
	char buf[bufferSize];
	int n;
	blocks = malloc(sizeof(struct aiocb) * chunkCount);
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	rd = open("/dev/urandom", O_RDONLY);
	if(rd == -1){
		printf("Kazkas sprogo atidarant faila\n");
		dl_exit(1);
	}
	if(dl_read_chunks(rd, blocks, buf) == -1)
		dl_exit(1);
	n = dl_read_wait(blocks);
	if(n > 0)
		printf("Perskaityta %d baitu\n", n);
	else
		dl_exit(1);
	dl_exit(0);
	return 0;
}
