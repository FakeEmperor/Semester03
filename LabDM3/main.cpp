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

size_t file_input(FILE* file, bool*** input, char** task){
	size_t num;
	size_t i, s, t = 0;
	bool working = true;
	char buf[256];
	if (!fscanf(file, "%d", &num))
		fail("Failed to read number of arguments. Closing...");
	fgets(buf, 256, file);
	//allocate memory
	*input = (bool**)malloc(num*sizeof(bool*));
	for (i = 0; i < num; ++i)
		(*input)[i] = (bool*)malloc(num*sizeof(bool));
	do {
		fgets(buf, 256, file);
		s = strlen(buf);
		//copy to task
		if (!*task) {
			*task = (char*)realloc(*task, s + 1);
			*task[0] = 0;
		}
		else
			*task = (char*)realloc(*task, strlen(*task) + s + 1);
		strcat(*task + strlen(*task), buf);
		
		//reset buffer
		for (i = 0; i < s; ++i) {
			if (buf[i] == '1' || buf[i] == '0')
				if (t < num*num) {
					(*input)[t / num ][ t%num ] = buf[i] - '0';
					++t;
				}
				else {
					fail("Number of vertices is greater than declared. Stopping input...");
					working = false;
					break;
				}
		};
		buf[0] = 0;
	} while (!feof(file)&&working);
	if ( t != num*num)
		fail("Number of arguments does not match number of bits, that have been read by the program. Closing...");
	return num;
};

void recFunc(bool **graph, size_t verts, bool* visited, size_t  &numLinks, size_t index){
	visited[index] = true;
	numLinks++;
	for (size_t i = 0; i < verts; ++i) {
		if (!visited[i] && graph[index][i])
			//go there
			recFunc(graph, verts, visited, numLinks, i);
	}

};
void recFuncBuild(bool **graph, size_t verts, bool* visited, size_t index, char** subvert) {
	char buf[20];
	sprintf(buf, "%d-", index+1);
	//build string
	if (!(*subvert)) {
		*subvert = (char*)malloc(3);
		*subvert[0] = 0;
	}
	else {
		size_t s = strlen(*subvert);
		*subvert = (char*)realloc(*subvert, s+3);
		(*subvert)[s] = 0;
	}
	strcat(*subvert + strlen(*subvert), buf);
	//usual in-depth search algorithm
	visited[index] = true;
	for (size_t i = 0; i < verts; ++i) {
		if (!visited[i] && graph[index][i])
			//go there
			recFuncBuild(graph, verts, visited, i, subvert);
	}
	
}

void walkThroughGraph(bool **graph, size_t verts, bool& is_linked) {
	bool *visited = new bool[verts];
	size_t numLinks = 0, maxL = 0;
	for (size_t i = 0; i < verts; ++i)
		visited[i] = false;
	recFunc(graph, verts, visited, numLinks, 0);
	if (numLinks < verts)
		is_linked = false;
	else
		is_linked = true;
	delete[] visited;
};

size_t walkAndNoticeGraphComponents(bool **graph, size_t verts, char*** subverts){
	size_t num_subverts = 0, i = 0;
	bool *visited = new bool[verts];
	for (size_t i = 0; i < verts; ++i)
		visited[i] = false;
	do {
		num_subverts++;
		if (!(*subverts)) {
			*subverts = (char**)malloc(sizeof(char*)*num_subverts);
		}
		else
			*subverts = (char**)realloc(*subverts , sizeof(char*)*num_subverts);
		(*subverts)[num_subverts - 1] = NULL;
		recFuncBuild(graph, verts, visited, i, &(*subverts)[num_subverts-1] );
		//eat last character
		(*subverts)[num_subverts-1][ strlen( (*subverts)[num_subverts-1]) - 1] = 0;
		for (i = 0; i < verts; ++i)
			if (!visited[i])
				break;
		
	} while (i!=verts);
	return num_subverts;
};


void file_output(FILE* file, const char* header, const char* input, const size_t num_verts,
				 const bool is_linked, char** subgraphs, size_t num_subgraphs ){
	fprintf(file, "%s\r\n%d\r\n%s\r\n", header, num_verts, input);
	fprintf(file, "Граф %sсвязан.\r\n", (is_linked? "" : "не"));
	fprintf(file, "Граф состоит из %d связанных компонент:\r\n", num_subgraphs);
	for (size_t i = 0; i < num_subgraphs; ++i)
		fprintf(file, "%s\r\n", subgraphs[i]);
};

int main(int argc, char** argv){
	const char* header = "Коряковцев Алишер, ИУ8-34\r\n"
		"Внутримашинные представления графов. Выявление связности графа\r\n";
	char* task = NULL;
	char** subgraphs = NULL;
	bool **graph = NULL;
	bool is_linked = false;
	FILE *in, *out;
	int num_vertices, num_subgraphs;

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

	num_vertices = file_input(in, &graph, &task);
	walkThroughGraph(graph, num_vertices, is_linked);
	num_subgraphs = walkAndNoticeGraphComponents(graph, num_vertices, &subgraphs);

	file_output(out, header, task, num_vertices, is_linked, subgraphs, num_subgraphs);
	flushall();
	getch();
	return 0;
};
