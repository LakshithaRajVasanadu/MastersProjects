#include <stdio.h>
#include <string.h>

int main() {
	FILE *fp;
   	size_t len;
	char str[6];
	strcpy(str, "hello");
	len =strlen(str);	
	fp = fopen("file1.txt", "wb");
	fwrite(&len, sizeof(len), 1, fp);
	fwrite(str, strlen(str), 1, fp);
	fclose(fp);
}
