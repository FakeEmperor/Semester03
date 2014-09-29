#pragma warning(disable:4996)
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>

#define MAX_N 1000

void fail(const char* str){
	puts(str);
	getch();
	exit(1);
}

void read_array(const char* buf, char Arr[MAX_N], const size_t control_num, const char* array_name){
	size_t cnt = 0;
	while (*buf){
		if (isalnum(*buf)) {
			Arr[cnt] = *buf;
			++cnt;
		}
		++buf;
	}
	Arr[cnt] = 0;
	if (strlen(Arr) != control_num) {
		printf("Array has %d elements, while expected %d. %s", strlen(Arr), control_num, (strlen(Arr) > control_num) ? "" : "Closing...");
		if (strlen(Arr) < control_num){
			getch();
			exit(1);
		}
	}
}

void file_input(FILE* file, char* header, char A[MAX_N], char B[MAX_N]  ){
	char buf[256];
	int a, b, num_success;
	header[0] = 0;
	do{
		fgets(buf, MAX_N, file);
		strcat(header, buf);
	} while(strcmp(buf, "\n"));
	//read the numbers
	num_success = fscanf(file, "%d %d", &a, &b);
	if (num_success < 2)
		fail("Failed to read array sizes from input string. Closing...");
	fgets(buf, 256, file);
	fgets(buf, 256, file); //array a
	read_array(buf, A, a, "A");
	fgets(buf, 256, file); //array b
	read_array(buf, B, b, "B");
};

size_t decart_magic(const char A[MAX_N], const char B[MAX_N], char C[MAX_N][2]) {
	size_t i = 0, u = 0, z = 0, a = strlen(A), b = strlen(B);
	for (i = 0; i < a; ++i){
		for (u = 0; u < b; ++u) {
			C[z][0] = A[i];
			C[z][1] = B[u];
			++z;
		}
	}
	return strlen(A)*strlen(B);
};

void file_output(FILE* file, const char* header,const char C[MAX_N][2],const size_t a, const size_t b){
	size_t i, u;
	//header print
	fprintf(file,"%s \n", header);
	//post header/pre array
	//array
	fputs("{\n", file);
	for (i = 0; i < a; i++){
		//fputs("{", file);
		for (u = 0; u < b; u++) 
			fprintf(file,"\t( %c %c )%s ", C[i*b + u][0], C[i*b + u][1], u < b - 1 ? "," : "");
		fputs("\n", file);
		//fputs("}\n", file);
	}
	//post array
	fputs("\n}", file);
};



int main(){
	char A[MAX_N], B[MAX_N], C[MAX_N][2];
	char header[MAX_N];
	FILE *input, *output;
	input = fopen("in.txt", "r");
	output = fopen("out.txt", "w");
	if (!input)
		fail("Failed to open input file. Closing...");
	if (!output)
		fail("Failed to open output file for writing. Closing...");
	file_input(input, header,A,B );
	decart_magic(A,B,C);
	file_output(output, header, C, strlen(A), strlen(B));
	return 0;
};
