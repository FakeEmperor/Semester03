#pragma warning(disable:4996)
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <time.h>



void fail(const char* str, bool close = true){
	puts(str);
	if (close){
		getch();
		exit(1);
	}
};

size_t lof(FILE* in) {
	size_t cur = ftell(in), end;
	fseek(in, 0, SEEK_END);
	end = ftell(in);
	fseek(in, cur, SEEK_SET);
	return end;
};

int getnum(FILE* in){
	char buf[65];
	int conv = -1, i = 0;
	char c;
	bool broken = false, ended = false, negative = false;
	do {
		c = fgetc(in);
		if (isdigit(c))
			buf[i++] = c;
		else if (c == '-' && i == 0)
			negative = !negative;
		else if (isspace(c) && i != 0)
			ended = true;
		else if (i != 0)
			broken = true;
	} while (!broken && !ended && i<64);
	buf[i] = 0;
	if (!broken)
		conv = atoi(buf);

	return conv;
}

char* word_input(FILE* in, int* start_state) {
	if (!start_state || !in)
		return NULL;
	if (!fscanf(in, "%d", start_state))
		fail("Can\'t get the start state for DFA");
	while (isspace(fgetc(in)));
	fseek(in, -1, SEEK_CUR);
	
	size_t st = ftell(in) , sp = lof(in);

	char* word = (char*)malloc(sp - st + 1);
	fread(word, sp - st, 1, in);
	word[sp - st] = 0;
	return word;
}


char** out_matrix_input(FILE* in, char** dict_out_alph, int** dict_out_stat, int* r, int* l ) {
	if (!in || !dict_out_stat || !dict_out_alph || !r || !l)
		return NULL;
	char** matrix;
	//read lines and rows
	int num_read = fscanf(in, "%d %d", l, r);
	if (num_read < 2)
		fail("Failed to obtain DFA's output matrix sizes. Closing...");
	while (isspace(fgetc(in)));
	fseek(in, -1, SEEK_CUR );
	//expand
	(*dict_out_stat) = (int*)malloc(sizeof(int)*(*r));
	(*dict_out_alph) = (char*)malloc(sizeof(char)*(*l));
	int i, u, t;
	matrix = (char**)malloc(sizeof(char*)*(*l));
	for (i = 0; i < (*l); ++i)
		matrix[i] = (char*)malloc(sizeof(char)*(*r));

	//now read the matrix
	
	char c;
	bool head_vert_read = false, head_horz_read = false;
	for (i = 0; i < *l;) {
		if (feof(in))
			break;
		head_vert_read = false;
		
		for (u = 0; u < *r;) {
			//first line - headers
			if (!head_horz_read) {
				if ((t = getnum(in)))
					(*dict_out_stat)[u++] = t;
				else
					fail("Failed to get the header. Closing...");
			}
			//first element in non-first line - line header
			else {
				c = fgetc(in);
				if (!isspace(c)) {
					if (!head_vert_read) {
						head_vert_read = true;
						(*dict_out_alph)[i] = c;
					}
					else
						matrix[i][u++] = c;
				}		
			}
		}
		if (!head_horz_read)
			head_horz_read = true;
		else
			++i;
	}
	if (i != *l)
		fail("Failed to get enough matrix lines. Closing...");

	return matrix;
}

int** state_matrix_input(FILE* in, char** dict_state_alph, int** dict_state_stat, int* r, int* l) {
	if (!in || !dict_state_stat || !dict_state_alph || !r || !l)
		return NULL;
	int** matrix;
	//read lines and rows
	int num_read = fscanf(in, "%d %d", l, r);
	if (num_read < 2)
		fail("Failed to obtain DFA's output matrix sizes. Closing...");
	while (isspace(fgetc(in)));
	fseek(in, -1, SEEK_CUR);
	//expand
	(*dict_state_stat) = (int*)malloc(sizeof(int)*(*r));
	(*dict_state_alph) = (char*)malloc(sizeof(char)*(*l));
	int i, u, t;
	matrix = (int**)malloc(sizeof(int*)*(*l));
	for (i = 0; i < (*l); ++i)
		matrix[i] = (int*)malloc(sizeof(int)*(*r));

	//now read the matrix

	char c;
	bool head_vert_read = false, head_horz_read = false;
	for (i = 0; i < *l;) {
		if (feof(in))
			break;
		head_vert_read = false;
		for (u = 0; u < *r;) {
			//first line - headers
			if (!head_horz_read) {
				if ((t = getnum(in)))
					(*dict_state_stat)[u++] = t;
				else
					fail("Failed to get the header. Closing...");
			}
			//first element in non-first line - line header
			else {
				c = fgetc(in);
				if (!isspace(c)) {
					if (!head_vert_read) {
						head_vert_read = true;
						(*dict_state_alph)[i] = c;
					}
					else {
						fseek(in, -1,  SEEK_CUR );
						matrix[i][u++] = getnum(in);
					}
						
				}
			}
		}
		if (!head_horz_read)
			head_horz_read = true;
		else
			++i;
	}
	if (i != *l)
		fail("Failed to get enough matrix lines. Closing...");
	return matrix;
}

char* readall(FILE* in) {
	fseek(in, 0, SEEK_SET);
	size_t len = lof(in);
	char* instr;
	instr = (char*)malloc(len+1);
	
	fread(instr, 1, len, in);
	instr[len] = 0;
	return instr;
}

bool open_cautios(FILE** file, int argc, char** argv, const char* const def_path, int a_num,
				  const char* mode, const char* file_name, const char* msg_warn, bool _warned_already)
{
	char *msg_buf;
	const char* msg = "Failed to open %s file. Closing...";
	if (!file)
		return false;
	msg_buf = (char*)malloc(strlen(file_name) + strlen(msg) + 1);
	*file = fopen(argc < a_num + 1 ? def_path : argv[a_num], mode);
	if (argc < a_num + 1 && !_warned_already) {
		fail(msg_warn, false);
		_warned_already = true;
	}
		
	
	if (!*file) {
		sprintf(msg_buf, msg, file_name);
		fail(msg_buf);
	}
	free(msg_buf);
	return _warned_already;
}


int find_index_state(int target, int* dict_rows, int rows){
	for (int i = 0; i < rows; ++i)
		if (target == dict_rows[i])
			return i;

	return -1;
};
int find_index_char(char target, char* dict_lines, int lines){
	for (int i = 0; i < lines; ++i)
		if (tolower(target) == tolower(dict_lines[i]))
			return i;

	return -1;
};

char newchar(char ch, int current_state, char** out_matrix, char* dict_out_lines, int* dict_out_rows, int lines, int rows) {
	int l = find_index_char(ch, dict_out_lines, lines),
		r = find_index_state(current_state, dict_out_rows, rows);
	if (l == -1 || r == -1)
		fail("Failed to find a char in DFA with given old char and state. Closing...");

	return out_matrix[l][r];
};

int newstate(char ch, int current_state, int** state_matrix, char* dict_stat_lines, int* dict_stat_rows, int lines, int rows) {
	int l = find_index_char(ch, dict_stat_lines, lines),
		r = find_index_state(current_state, dict_stat_rows, rows);
	if (l == -1 || r == -1)
		fail("Failed to find a new state in DFA with given old char and state. Closing...");
	return state_matrix[l][r];
};

char* automate_transform(const char* word,char** out_matrix,int* dict_out_rows,char* dict_out_lines,
	int** state_matrix,int* dict_state_rows,char* dict_state_lines,int lines,int rows,int start_state){

	size_t s = strlen(word);
	char* newword = (char*)malloc(s + 1);
	int current_state = start_state;
	for (int i = 0; i < s; ++i) {
		newword[i] = newchar(word[i], current_state, out_matrix, dict_out_lines, dict_out_rows, lines, rows);
		current_state = newstate(word[i], current_state, state_matrix, dict_state_lines, dict_state_rows, lines, rows );
	}
	newword[s] = 0;
	return newword;
}

void print_result(FILE* out, FILE* in_state, FILE* in_out, FILE* in_word, const char* newword){
	char *header = "Коряковцев Алишер. ЛАБ ДМК №4\r\n"
				   "Начальные условия:\r\n"
				   "Таблица переходов:\r\n%s\r\n"
				   "Таблица выходов:\r\n%s\r\n"
				   "Входное слово и состояние:\r\n%s\r\n\r\n"
				   "Результат: %s";
	char *ins = readall(in_state), *ino = readall(in_out), *inw = readall(in_word);
	fprintf(out, header, ins, ino, inw, newword);
	free(ins); free(ino); free(inw);
};

void free_matrix(void** arr, size_t lines) {
	for (int i = 0; i < lines; ++i)
		free(arr[i]);
	free(arr);
}

void main(int argc, char** argv) {
	char** out_matrix;
	char* dict_out_lines, *dict_state_lines;
	int* dict_out_rows, *dict_state_rows;
	int** state_matrix;

	const char  *def_in_state_path = "in_state.txt", *def_in_out_path = "in_out.txt",
				*def_in_word_path = "in_word.txt", *def_out_path = "out.txt";

	int lines, rows;
	FILE *in_state, *in_out, *in_word, *out;
	bool _warned =false;
	if (argc < 5)
		fail("Some filepaths are not passed. Using default ones...", false);
	/*HINDU CODE BEGIN*/
	_warned = open_cautios(&in_state, argc, argv, def_in_state_path, 1, 
							"rb", "<in_state>" , "U NO PASS ANY FILENAME! BAD, BAD USER!", _warned);
	_warned = open_cautios(&in_out, argc, argv, def_in_out_path, 2,
							"rb", "<in_out>", "U NO PASS THE <IN_OUT> FILANAME! BAD, BAD USER!", _warned);
	_warned = open_cautios(&in_word, argc, argv, def_in_word_path, 3,
							"rb", "<in_word>", "U NO PASS THE <IN_WORDT> FILANAME! BAD, BAD USER!", _warned);
	_warned = open_cautios(&out, argc, argv, def_out_path, 4,
							"w", "<out>", "U NO PASS THE <OUT> FILANAME! BAD, BAD USER!", _warned);
	/*HINDU CODE END*/
	
	char *newword, *word;
	int start_state = 0;
	//read words
	word = word_input(in_word, &start_state);
	//read out matrix with state matrix
	out_matrix = out_matrix_input(in_out, &dict_out_lines, &dict_out_rows, &rows, &lines);
	int tl, tr;
	state_matrix = state_matrix_input(in_state, &dict_state_lines, &dict_state_rows, &tr, &tl);
	if (tl != lines || tr != rows)
		fail("Sizes do not match. Closing...");
	//make a new word
	newword = automate_transform(word, out_matrix, dict_out_rows, dict_out_lines,
								 state_matrix, dict_state_rows, dict_state_lines, lines, rows, start_state );

	print_result(out, in_state, in_out, in_word, newword);
	fflush(out);
	//free memory
	free(newword);
	free_matrix((void**)out_matrix, lines);
	free_matrix((void**)state_matrix, lines);
	free(dict_out_lines);
	free(dict_out_rows);
	free(dict_state_lines);
	free(dict_state_rows);
	free(word);


	getch();

}