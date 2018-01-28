#include "ff.h"
#include "stdio.h"

extern void *malloc(unsigned int);
extern void free(void*);

FILE *fopen(const char *__filename,
			const char *__modes)
{
	printf("fopen(\"%s\",\"%s\")\n", __filename,__modes);
	FIL* stream=(FIL*)malloc(sizeof (FIL));
	if (!f_open(stream, __filename, FA_OPEN_EXISTING | FA_READ))
	{
		return (FILE *)stream;
	}
	return NULL;
}

size_t fread(void * __ptr, size_t __size, size_t __n,
	  FILE * stream)
{
	printf("fread(%d,%d,%d)\n", __size, __n,(size_t)stream);
	UINT read_cnt;
	FRESULT res = f_read((FIL *)stream, __ptr, __size * __n, &read_cnt);
	if (res == FR_OK && read_cnt)
	{
		return read_cnt;
	}
	else
	{
		return 0;
	}
}

int fgetc(FILE *stream)
{
	printf("fgetc(%d)\n",(size_t)stream);
	char ch = 0;
	FRESULT res;
	UINT cou_sd;
	res = f_read((FIL *)stream, &ch, 1, &cou_sd);
	if (res != FR_OK || cou_sd == 0)
	{
		return EOF;
	}
	return ch;
}

int ferror(FILE *stream)
{
	printf("ferror(%d)\n",(size_t)stream);
	return f_error((FIL*)stream);
}

int feof(FILE *stream)
{
	printf("feof(%d)\n",(size_t)stream);
	return f_eof((FIL*)stream);
}

int fclose(FILE *stream)
{
	printf("fclose(%d)\n",(size_t)stream);
	return f_close((FIL*)stream);
}
