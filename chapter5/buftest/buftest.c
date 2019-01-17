#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

char buf[100];

void myprintf(const char *ptr, int mode)
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	switch(mode) {
		case 0:
			printf("\n[ %ld ] NBF: ", time.tv_sec);
			break;
		case 1:
			printf("\n[ %ld ] LBF: ", time.tv_sec);
			break;
		case 2:
			printf("\n[ %ld ] FBF: ", time.tv_sec);
			break;
		case 3:
			printf("\n[ %ld ] TFS: ", time.tv_sec);
			break;
	}

	printf(ptr);
}

void myfprintf(FILE *fp)
{
	printf("\n fp bufbase: %p", fp->_IO_buf_base);
	printf("\n fp bufend: %p", fp->_IO_buf_end);
	printf("\n fp bufsize: %d", fp->_IO_buf_end -fp->_IO_buf_base);
	printf("\n fp readbase: %p", fp->_IO_read_base);
	printf("\n fp readend: %p", fp->_IO_read_end);
	printf("\n fp readsize: %d", fp->_IO_read_end - fp->_IO_read_base);
	printf("\n fp readptr: %p", fp->_IO_read_ptr);
	printf("\n fp writebase: %p", fp->_IO_write_base);
	printf("\n fp writeend: %p", fp->_IO_write_end);
	printf("\n fp writesize: %d", fp->_IO_write_end - fp->_IO_write_base);
	printf("\n fp writeptr: %p", fp->_IO_write_ptr);
	
	printf("\n stdout bufbase: %p", stdout->_IO_buf_base);
	printf("\n stdout bufend: %p", stdout->_IO_buf_end);
	printf("\n stdout bufsize: %d", stdout->_IO_buf_end - stdout->_IO_buf_base);
	printf("\n stdout readbase: %p", stdout->_IO_read_base);
	printf("\n stdout readend: %p", stdout->_IO_read_end);
	printf("\n stdout readsize: %d", stdout->_IO_read_end - stdout->_IO_read_base);
	printf("\n stdout readptr: %p", stdout->_IO_read_ptr);
	printf("\n stdout writebase: %p", stdout->_IO_write_base);
	printf("\n stdout writeend: %p", stdout->_IO_write_end);
	printf("\n stdout writesize: %d", stdout->_IO_write_end - stdout->_IO_write_base);
	printf("\n stdout writeptr: %p\n", stdout->_IO_write_ptr);

}

void testforBF(int mode)
{
	memset(buf, 0, sizeof(buf)/sizeof(buf[0]));
	FILE *fp = fopen("testfile1", "r");
	if(fp == NULL)
		myprintf("open error\n", mode);

	freopen("/dev/fd/1", "a", stdout);
	
	switch(mode) {
		case 0:
			if(setvbuf(fp, NULL, _IONBF, 0))
				myprintf("setvbuf error!\n", mode);
			if(setvbuf(stdout, NULL, _IONBF, 0))
				myprintf("setvbuf error!\n", mode);
			break;
		case 1:
			if(setvbuf(fp, NULL, _IOLBF, 0))
				myprintf("setvbuf error!\n", mode);
			if(setvbuf(stdout, NULL, _IOLBF, 0))
				myprintf("setvbuf error!\n", mode);
			break;
		case 2:
			if(setvbuf(fp, NULL, _IOFBF, 0))
				myprintf("setvbuf error!\n", mode);
			if(setvbuf(stdout, NULL, _IOFBF, 0))
				myprintf("setvbuf error!\n", mode);
			break;
	}

	myprintf("before fgetc and fputc:\n", mode);
	myfprintf(fp);
	int c = 0;
	if((c = fgetc(fp)) == EOF)
		myprintf("fgetc error!\n", mode);

	myprintf("after fgetc, before fputc:\n", mode);
	myfprintf(fp);

	sleep(5);
	
	if(fputc(c, stdout) == EOF)
		myprintf("fputc error!\n", mode);

	//myprintf("after fgetc and fputc:\n", mode);
	//myfprintf(fp);
	
	sleep(5);

	myprintf("after sleep.\n", mode);
	myprintf("before fgets and fputs:\n", mode);
	myfprintf(fp);

	if((fgets(buf, sizeof(buf)/sizeof(buf[0]), fp)) == NULL) {
		if(ferror(fp))
			myprintf("fgets error!\n", mode);
	}
	
	myprintf("after fgets, before fputs:\n", mode);
	myfprintf(fp);

	sleep(5);

	if(fputs(buf, stdout) == EOF)
		myprintf("fputs error!\n", mode);
	myprintf("after fgets and fputs:\n", mode);
	myfprintf(fp);

	sleep(5);

	myprintf("after sleep. before close.\n", mode);

	if(fclose(fp) == EOF)
		myprintf("close error\n", mode);
}

void testforstyle()
{
	memset(buf, 0, sizeof(buf)/sizeof(buf[0]));
	FILE *fp = fopen("testfile2", "r+");
	if(fp == NULL)
		myprintf("open error\n", 3);

	freopen("/dev/fd/1", "a", stdout);

	myprintf("before fgets and fputs:\n", 3);
	myfprintf(fp);
	
	if((fgets(buf, sizeof(buf)/sizeof(buf[0]), fp)) == NULL) {
		if(ferror(fp))
			myprintf("fgets error!\n", 3);
	}
	
	myprintf("after fgets, before fputs:\n", 3);
	myfprintf(fp);

	sleep(5);
	fseek(fp, 0, SEEK_END);

	if(fputs(buf, fp) == EOF)
		myprintf("fputs error!\n", 3);

	sleep(5);

	myprintf("after sleep.\n", 3);
	myprintf("before fputs and fgets:\n", 3);
	myfprintf(fp);

	if(fputs("tttsss\n", fp) == EOF)
		myprintf("fputs error!\n", 3);
	
	fflush(fp);

	myprintf("after fputs, before fgets:\n", 3);
	myfprintf(fp);

	sleep(5);
	
	if((fgets(buf, sizeof(buf)/sizeof(buf[0]), fp)) == NULL) {
		if(ferror(fp))
			myprintf("fgets error!\n", 3);
		else
			myprintf(buf, 3);
	}

	myprintf("after fputs and fgets:\n", 3);
	myfprintf(fp);

	sleep(5);

	myprintf("after sleep. before close.\n", 3);

	if(fclose(fp) == EOF)
		myprintf("close error\n", 3);
}

int main(void)
{
	testforBF(0);
	testforBF(1);
	testforBF(2);
	testforstyle();
}
