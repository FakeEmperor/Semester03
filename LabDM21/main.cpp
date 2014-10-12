#pragma warning(disable:4996)
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <time.h>

//OBJECTIVE-C style code
typedef struct BooleanFunction {
	unsigned char* func;
	size_t num_bits;
	bool status; //false - not working, true - working

	BooleanFunction() {
		status = false;
		func = NULL;
		this->num_bits = 0;
	};
	BooleanFunction(const char* str){
		size_t i, u, nn;
		size_t s = strlen(str);
		double n = (double)s / CHAR_BIT;
		nn = (size_t)(ceil(n));
		this->num_bits = s;
		func = new unsigned char[nn+1];
		//from left to right ( last bit in memory - first bit in real function set )
		for (i = 0; i < nn ; ++i){
			func[i] = 0;
			for (u = 0; u < CHAR_BIT && s > i*CHAR_BIT+u; ++u)
					func[i] |= ((str[i*CHAR_BIT + u] - '0') << (CHAR_BIT - 1 - u));

		};
		func[nn] = 0;
		status = 1;

	};
	~BooleanFunction(){
		delete func;
	};

	// true - in range, false - out of range
	bool checkX(size_t x) {
		return 1 << x <= num_bits;
	};
	//-1 - b1 < b2, 0 - b1 == b2, 1 - b1 > b2 
	char getBit(size_t b) {
		return (func[ b/CHAR_BIT ] & (1 << (CHAR_BIT - b%CHAR_BIT - 1))) >> (CHAR_BIT - b%CHAR_BIT - 1);
	};
	char compareBits(size_t b1, size_t b2) {
		char _b1 = getBit(b1),
			_b2 = getBit(b2);
		if (_b1 > _b2)
			return 1;
		else if (_b1 < _b2)
			return -1;
		return 0;
	};

	size_t getNeighboor( size_t b , size_t x){
		return b ^ (1 << x-1);
	};
	// -1 - error, 0 - false, 1 - true

	int checkImportance(size_t x) {
		if (!checkX(x))
			return -1;
		size_t half = num_bits;
		for (size_t i = 0; i < half; ++i)
			if (compareBits(i, getNeighboor(i, x)))
				return 1;
		return 0;
	};
	int checkMonotoneness(size_t x) {
		if (!checkX(x))
			return -1;
		size_t half = num_bits, nb;
		int val;
		for (size_t i = 0; i < half; ++i) {
			nb = getNeighboor(i, x);
			val = compareBits(i, nb);
			if ((val > 0 && i < nb) || (i > nb && val < 0))
				return 0;
		}
				return 1;
		return 0;
	};
	int checkLinearity(size_t x){
		if (!checkX(x))
			return -1;
		size_t half = num_bits ;
		for (size_t i = 0; i < half; ++i){
			if (!compareBits(i, getNeighboor(i, x)))
				return 0;
		}
		return 1;
	};
	void checkAll(bool valar[3], size_t x, bool checkL = true, bool checkI = true, bool checkM = true) {

	};

} BooleanFunction;


void fail(const char* str, bool close = true){
	puts(str);
	if (close){
		getch();
		exit(1);
	}	
}
void addchar(char* str, const char ch){
	size_t pos = strlen(str);
	str[pos] = ch;
	str[pos + 1] = 0;
};
size_t file_input(FILE* file, char** input, char** task ){
	size_t num;
	size_t i, s;
	char buf[256];
	if (!fscanf(file, "%d", &num ))
		fail("Failed to read number of arguments. Closing...");
	fgets(buf, 256, file);
	do {
		fgets(buf, 256, file);
		s = strlen(buf);
		if (!*input) {
			*input = (char*)realloc(*input, s + 1);
			*input[0] = 0;
		}
		else
			*input = (char*)realloc(*input, strlen(*input) + s + 1);
		//copy to task
		if (!*task) {
			*task = (char*)realloc(*task, s + 1);
			*task[0] = 0;
		}
		else
			*task = (char*)realloc(*task, strlen(*task) + s + 1);
		strcat(*task + strlen(*task), buf);
		//slow
		for (i = 0; i < s; ++i)
			if (buf[i]=='1'||buf[i]=='0')
				addchar(*input, buf[i]);

		buf[0] = 0;
	} 
	while (!feof(file));
	if (strlen(*input) != 1 << num)
		fail("Number of arguments does not match number of bits, that have been read by the program. Closing...");
	return num;
};

void file_output(FILE* file, const char* header, const char* input, const bool * const props[3]  , const size_t num_arguments){
	fprintf(file, "%s\r\n%d\r\n%s\r\n", header, num_arguments, input);
	fputs("\t (существенность, монотонность, линейность)\r\n", file);
	size_t l = 0, m = 0;
	for (size_t i = 0; i < num_arguments; ++i) {
		fprintf(file, "x%d\t%c\t%c\t%c\r\n", (i + 1), (props[i][0] ? '+' : '-'), (props[i][1] ? '+' : '-'), (props[i][2] ? '+' : '-'));
		if (props[i][2] || !props[i][0])
			l++;
		if (props[i][1] || !props[i][0])
			m++;
	}
	fprintf(file, "%sмонотонна, %sлинейна", (m == num_arguments ? "" : "не"), (l == num_arguments ? "" : "не"));
	
};



int main(int argc, char** argv){
	const char* header = "Коряковцев Алишер, ИУ8-34\r\n"
						 "Выявление существенности, линейности и монотонности булевых функций\r\n"
						 "по переменным с использованием побитовых операций.";
	char* input = NULL, *task = NULL;
	bool **props;
	FILE *in, *out;
	size_t num_arguments;
	BooleanFunction *f;

	if (argc < 3) {
		fail("Arguments to the program are: {input file path} {output file path}. Trying to apply defaults...", false);
		in = fopen("in.txt", "r");
		out = fopen("out.txt", "w");
		
	}
	else {
		in = fopen(argv[1], "r");
		out = fopen(argv[2], "w");
	}

	if (!in)
		fail("Failed to open input file. Closing...");
	if (!out)
		fail("Failed to open output file for writing. Closing...");
	
	num_arguments = file_input(in, &input, &task);
	f = new BooleanFunction(input);
	props = new bool*[num_arguments];

	for (size_t i = 0; i < num_arguments; ++i) {
		props[i] = new bool[3];
		props[i][0] = f->checkImportance(i+1);
		props[i][1] = f->checkMonotoneness(i+1);
		props[i][2] = f->checkLinearity(i+1);
	}

	file_output(out, header, task, props, num_arguments);
	flushall();
	getch();
	return 0;
};
