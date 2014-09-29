#pragma warning(disable:4996)
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#define MAX_N 1000

void fail(const char* str, const bool failing = true){
	puts("[AN ERROR HAS OCCURED]:");
	puts(str);
	
	if(failing) {
		getch();
		exit(1);
	}
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
		printf("Set %s has %d elements, while expected %d. %s", array_name , strlen(Arr), control_num, (strlen(Arr) > control_num) ? "" : "Closing...");
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
	} while(strcmp(buf, "\r\n"));
	//read the numbers
	num_success = fscanf(file, "%d", &a);
	if (!num_success)
		fail("Failed to read array sizes from input string. Closing...");
	fgets(buf, 256, file);
	fgets(buf, 256, file); //array a
	read_array(buf, A, a, "A");
	num_success = fscanf(file, "%d", &b);
	if (!num_success)
		fail("Failed to read array sizes from input string. Closing...");
	fgets(buf, 256, file);
	fgets(buf, 256, file); //array b
	read_array(buf, B, b, "B");


	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	fread(header, MAX_N, 1, file);
	header[fsize] = 0;
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

void file_output(FILE* file, const char* header,const char C[MAX_N][2], const char A[MAX_N], const char B[MAX_N]){
	size_t i, u, a, b, sA = strlen(A), sB = strlen(B);
	//header print
	fprintf(file,"%s \n", header);
	//post header/pre array
	//array
	fputs("\nA x B={\n", file);
	a = ceil((double)9*sA*sB/80);
	b = ceil((double)sA*sB/a);
	for (i = 0; i < a; i++){
		//fputs("{", file);
		for (u = 0; u < b && i*b+u<sA*sB; u++) 
			fprintf(file,"\t( %c %c )%s ", C[i*b + u][0], C[i*b + u][1], u < b - 1 ? "," : "");
		fputs("\n", file);
		//fputs("}\n", file);
	}
	//post array
	fputs("}\n", file);
};



int main(const int argc, char** argv){
	char A[MAX_N], B[MAX_N], C[MAX_N][2];
	char header[MAX_N];
	FILE *input, *output;
	if(argc<3) {
		fail("Argument haven't passed, using predefined file", false);
		input = fopen("in.txt", "rb");
		output = fopen("out.txt", "w");
	}
	else {
		input = fopen(argv[1], "rb");
		output = fopen(argv[2], "w");
		printf("Reading from file: %s", argv[1]);
	}
	
	if (!input)
		fail("Failed to open input file. Closing...");
	if (!output)
		fail("Failed to open output file for writing. Closing...");
	file_input(input, header,A,B );
	decart_magic(A,B,C);
	file_output(output, header, C, A, B);
	return 0;
};
