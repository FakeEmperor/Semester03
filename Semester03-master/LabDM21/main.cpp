#pragma warning(disable:4996)
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>

#define MAX_N 1000
#define MAX_ARRAYS 10

void fail(const char* str){
	puts(str);
	getch();
	exit(1);
}


BOOL RAND_INITED = FALSE;
int random(const int max){
	if(!RAND_INITED) {
		srand(time(NULL));
		RAND_INITED = TRUE;
	}
	return rand()%max;
};


void read_array(const char* buf, char Arr[MAX_N], const size_t control_num, const char* array_name){
	size_t i,s,cnt = 0;

	while (*buf){
		if (isalnum(*buf)) {
			Arr[cnt] = *buf;
			++cnt;
		}
		++buf;
	}
	Arr[cnt] = 0;
	if (strlen(Arr) != control_num) {
		printf("Array \"%s\" has %d elements, while expected %d. %s\n", array_name, strlen(Arr), control_num, (strlen(Arr) > control_num) ? "" : "Closing...\n");
		if (strlen(Arr) < control_num){
			getch();
			exit(1);
		}
	}
	//next post process
	s = strlen(Arr);
	for( i=0; i<s-1; ++i)
		if(strchr(Arr+i+1, Arr[i]))
			printf("Array \"%s\" has duplicate element - %c\r\n",array_name, Arr[i]);

}

void file_input(FILE* file, char* header, char A[MAX_N], char B[MAX_N]  ){
	char buf[256];
	int a, b, inc = 0, num_success;
	header[0] = 0;
	do{
		fgets(buf, MAX_N, file);
		strcat(header, buf);
		if(!strcmp(buf, "\n"))
			inc++;
		else
			inc = 0;
	} while(inc<2);
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

void addchar(char* str, const char ch){
	size_t pos = strlen(str);
	str[pos] =ch;
	str[pos+1] = 0;
};

// A\X = B
// X = A\B+(Something not A)
void xor_magic(const char A[MAX_N], const char B[MAX_N], char C[MAX_ARRAYS][MAX_N]) {
	size_t pos, s = strlen(A), i = 0, u = 0, offset;
	unsigned char ch;
	for(; i<MAX_ARRAYS; ++i){
		//build array
		*C[i] = 0;
		for( u = 0; u<s; ++u)
			if(!strchr(B , A[u])) 
				addchar(C[i],A[u]);
		//add something not
		u = 0;
		while(u<i){
			ch = random(256);
			if( isalnum(ch)&&!strchr(A, ch)) {
				addchar(C[i],ch);
				++u;
			}			
		};
	}
};

void file_output(FILE* file, const char* header,const char C[MAX_ARRAYS][MAX_N]){
	size_t i, u, s;
	//header print
	fprintf(file,"%s \n", header);
	//post header/pre array
	//array

	for( i = 0; i<MAX_ARRAYS; ++i ){
		fprintf(file ,"ARRAY X [%d additional chars]:\n", i);
		fputs("{\n", file);
		s = strlen(C[i]);
		for (u = 0; u < s; u++){
			fprintf(file,"\t( %c )%s ", C[i][u], u < s - 1 ? "," : "");
		}
		//post array
		fputs("\n}\n", file);
	}
	
};



int main(){
	char A[MAX_N], B[MAX_N], C[MAX_ARRAYS][MAX_N];
	bool F[MAX_N];

	char header[MAX_N];
	FILE *input, *output;
	input = fopen("in.txt", "r");
	output = fopen("out.txt", "w");
	if (!input)
		fail("Failed to open input file. Closing...");
	if (!output)
		fail("Failed to open output file for writing. Closing...");
	file_input(input, header,A,B );
	xor_magic(A,B,C);
	file_output(output, header, C);
	flushall();
	getch();
	return 0;
};
