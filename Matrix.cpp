#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

/* Lab #6 Matrix Manipulation
 * Timo Alejandro Aranjo
 * ID: 3057380
 * 10.20.2023
 */

/*max_Dimension is really an artifical limit
 *Only included this because the instructions
*/

static const int max_Dimension = 100;

struct Matrix {
	int dimension;
	int ** array;
};

struct Data {
	off_t bytes;
	char * data;
};

void free_Data(struct Data d);

void allocate_Matrix_For_Dimension(struct Matrix * m);

void deallocate_Matrix(struct Matrix * m);

off_t get_File_Byte_Count(FILE * file);

struct Data read_Entire_File(FILE * file);

struct Data append_Null(struct Data d);

int string_Contains_Number(char * string);

void read_Matrices(const char * filename, struct Matrix * m, struct Matrix * m2);

void add_Matrices(struct Matrix * a, struct Matrix * b, struct Matrix * c);

void subtract_Matrices(struct Matrix * a, struct Matrix * b, struct Matrix * c);

void mulitply_Matrices(struct Matrix * a, struct Matrix * b, struct Matrix * c);

void print_Matrix(struct Matrix * m, const char * optional_Label);

int main() {
	struct Matrix a, b, sum, difference, product;
	
	read_Matrices("matrix_input.txt", &a, &b);

	printf("\nTimo Alejandro Aranjo (ID: 3057380)\n");
	printf("Lab #6: Matrix Manipulation\n\n");

	print_Matrix(&a, "Matrix A:");
	print_Matrix(&b, "Matrix B:");
	
	add_Matrices(&a, &b, &sum);
	print_Matrix(&sum, "Matrix Sum (A + B):");

	mulitply_Matrices(&a, &b, &product);
	print_Matrix(&product, "Matrix Product (A * B):");

	subtract_Matrices(&a, &b, &difference);
	print_Matrix(&difference, "Matrix Difference (A - B):");

	
	
	deallocate_Matrix(&a);
	deallocate_Matrix(&b);
	deallocate_Matrix(&sum);
	deallocate_Matrix(&difference);
}

void allocate_Matrix_For_Dimension(struct Matrix * m) {
	m->array = (int **) malloc(sizeof(int *) * m->dimension);

	for (int i = 0; i < m->dimension; i++) {
		m->array[i] = (int *) malloc(sizeof(int) * m->dimension);
	}
}

void deallocate_Matrix(struct Matrix * m) {
	for (int i = 0; i < m->dimension; i++) {
		free((void *) m->array[i]);
	}

	free((void *) m->array);
}

off_t get_File_Byte_Count(FILE * file) {
	int fd = fileno(file);
	struct stat s;
	
	fstat(fd, &s);
	return s.st_size;
}

struct Data read_Entire_File(FILE * file) {
	struct Data d;

	off_t byte_Count = get_File_Byte_Count(file);
	
	d.data = (char *) malloc(byte_Count);
	
	fread((void *) d.data, 1, byte_Count, file);
	d.bytes = byte_Count;

	return d;
}

void free_Data(struct Data d) {
	free((void *) d.data);
}

struct Data append_Null(struct Data d) {
	struct Data c;
	c.data = (char *) malloc(d.bytes + 1);
	strncpy(c.data, d.data, d.bytes);
	c.data[d.bytes] = '\0';

	return c;
}

int string_Contains_Number(char * string) {
	int length = strlen(string);
	
	
	for (int i = 0; i < length; i++) {
		if (string[i] >= 49 && string[i] <= 57) return 1;
	}

	return 0;
}

void read_Matrices(const char * filename, struct Matrix * m, struct Matrix * m2) {
	char * token;
	FILE * file = fopen(filename, "r");

	struct Data d = read_Entire_File(file);
	struct Data d_With_Null = append_Null(d);

	free_Data(d);
	
	token = strtok(d_With_Null.data, " \n");
	
	sscanf(token, "%d", &m->dimension);
	//Remove this assertion to allow matrices of any size
	assert(m->dimension <= max_Dimension);
	m2->dimension = m->dimension;

	allocate_Matrix_For_Dimension(m);
	allocate_Matrix_For_Dimension(m2);

	for (int r = 0; r < m->dimension; r++) {
		for (int c = 0; c < m->dimension; c++) {	
			token = strtok(NULL, " \n");
			
			if (!string_Contains_Number(token)) {c--; continue;};
			
			sscanf(token, "%d", &(m->array[r][c]));
		}
	}

	for (int r = 0; r < m->dimension; r++) {
		for (int c = 0; c < m->dimension; c++) {	
			token = strtok(NULL, " \n");
			if (!string_Contains_Number(token)) {c--; continue;};
			
			sscanf(token, "%d", &(m2->array[r][c]));
		}
	}

	free_Data(d_With_Null);

	fclose(file);
}

void add_Matrices(struct Matrix * a, struct Matrix * b, struct Matrix * c) {
	c->dimension = a->dimension;
	allocate_Matrix_For_Dimension(c);

	for (int r = 0; r < c->dimension; r++) {
		for (int col = 0; col < c->dimension; col++) {
			c->array[r][col] = a->array[r][col] + b->array[r][col];
		}
	}
}

void subtract_Matrices(struct Matrix * a, struct Matrix * b, struct Matrix * c) {
	c->dimension = a->dimension;
	allocate_Matrix_For_Dimension(c);

	for (int r = 0; r < c->dimension; r++) {
		for (int col = 0; col < c->dimension; col++) {
			c->array[r][col] = a->array[r][col] - b->array[r][col];
		}
	}
}

void mulitply_Matrices(struct Matrix * a, struct Matrix * b, struct Matrix * c) {
	c->dimension = a->dimension;
	allocate_Matrix_For_Dimension(c);

	for (int r = 0; r < c->dimension; r++) {
		for (int col = 0; col < c->dimension; col++) {
			int accumulator = 0;
			for (int i = 0; i < c->dimension; i++) {
				accumulator += a->array[r][i] * b->array[i][col];
			}
			c->array[r][col] = accumulator;
		}
	}
}

void print_Matrix(struct Matrix * m, const char * optional_Label) {
	if (optional_Label != (char *) 0) {printf("%s\n", optional_Label);}

	for (int r = 0; r < m->dimension; r++) {
		for (int col = 0; col < m->dimension; col++) {
			printf("%10d", m->array[r][col]);
		}
		printf("\n\n");
	}
}
