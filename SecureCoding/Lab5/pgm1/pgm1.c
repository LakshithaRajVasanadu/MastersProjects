#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

char* readFile(FILE* fp);

int main(void)  {
	FILE *fp;
	char *buf;

	//rb is enough. rb+ not needed
	fp = fopen("file1.txt", "rb");
	if(fp == NULL)  {
		printf("File Could not be opened\n");
		abort();
	}
	buf = readFile(fp);
	if(buf != NULL) { 
		printf("%s\n", buf);
		free(buf);
		buf = NULL;
	} else
		printf("Buffer is NULL\n");
	fclose(fp);

	return 0;
}

char* readFile(FILE* fp)  {
  	char *buf=NULL ;
	size_t len;
	int bytes_read;

	bytes_read = fread(&len, sizeof(int), 1, fp);
	printf("len=%u, bytes_read=%d\n", len, bytes_read);
	if(bytes_read > 0 && bytes_read <= INT_MAX && bytes_read == sizeof(int))  {
		printf("len=%u, bytes_read=%d\n", len, bytes_read);
		if(len > 0 && len <= SIZE_MAX-1)  {
			buf =(char *)malloc((unsigned)(len+1));
			if(!buf)  {
				printf("Memory could not be allocated\n");
			} else {
				bytes_read = fread(buf, sizeof(char), len, fp);
				if(bytes_read < 0 || bytes_read > INT_MAX || bytes_read != len) {
					printf("Invalid data read\n");
					buf = NULL;
				}
			}
		}
		else  {
			printf("Length value is invalid\n");
		}
		
	} else {
		printf("Incorrect Value for Length read\n");
	}
	return buf;
}
	
