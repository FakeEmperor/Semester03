#pragma warning(disable:4996)
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <conio.h>

static const bool WriteToFile = false;

void fail(const char* str, bool close = true){
	puts(str);
	if (close){
		getch();
		exit(1);
	}
}


void print_numbers(FILE* file, bool* vec, size_t s) {
	const int maxChars = 80, newLineChars = 2, spacesPerUnit = 1;
	char buf[2 * maxChars] = "", tempS[maxChars/2];
	size_t curChars = 0, toWrite = 0;
	for (size_t i = 0; i < s; ++i) {
		if (vec[i]) {
			itoa(i, tempS, 10);
			toWrite = strlen(tempS) + spacesPerUnit;
			if (curChars + toWrite > maxChars - newLineChars) {
				fputs(buf, file);
				fputs("\r\n", file);
				curChars = toWrite;
				strcpy(buf, tempS);
			}
			else {
				curChars += toWrite;
				strcat(buf, tempS);
			}

			strcat(buf, " ");
		}
		
	}
	//write remains
	fputs(buf, file);
	fputs("\r\n", file);
}

bool test(int number, long long* vec, size_t &s) {
	for (size_t u = 0; u < s; ++u)
		if (!(number%vec[u]))
			return false;
	vec[s++] = number;
	return true;
}

void main(int argc, char** argv) {
	size_t n = 0, m = 1;
	if (argc < 2 && WriteToFile )
		fail("No file, closing...");
	std::cin >> n;
	
	long long* delims = (long long*)malloc(sqrt(n) + 1);
	bool* simple = (bool*)calloc(n, sizeof(char));
	delims[0] = 2;
	
	for (size_t i = 3; i <= n; ++i)
		if (test(i, delims, m))
			simple[i] = 1;
	print_numbers(stdout, simple, n);
	getch();
	free(simple);
}