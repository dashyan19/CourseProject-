#include <iostream>
#include "windows.h"
#include "time.h"
#include <stack>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

void execution(int** mass_vertex, int size);

void print_matrix(int** matrix, int size);
void zero_matrix(int** matrix, int size);

int** generation_matrix(int size);
int** manual_matrix(int size);
int** file_matrix(int size);

int** edge_matrix(int** matrix, int** edge_coord, int size, int* num);

int maximal_independent_set(int** matrix, int* vis, int* set, int* num, int* vis_prnt, int start, int action, int** edge_coord);

string file_name;
ofstream file_write;

void execution(int** mass_vertex, int size)
{
	int kolvo_edge = 0, zero, action = 0;
	int* num = &kolvo_edge, * vis, * set, * vis_prnt;
	int** edge, ** edge_coord;
	int** matrix;

	printf("\nМатрица смежности вершин:\n\n");
	file_write << "\nМатрица смежности вершин:\n\n";
	print_matrix(mass_vertex, size);
	zero_matrix(mass_vertex, size);

	for (int i = 0; i < size; i++) 
	{
		for (int j = i; j < size; j++) 
		{
			if (mass_vertex[i][j] == 1) 
			{
				(*num)++;
			}
		}
	}

	edge_coord = (int**)malloc(3 * sizeof(int*));
	for (int i = 0; i < 3; i++) {
		edge_coord[i] = (int*)malloc(*num * sizeof(int));
	}
	edge = edge_matrix(mass_vertex, edge_coord, size, num);
	printf("\nМатрица смежности ребер:\n\n");
	file_write << "\nМатрица смежности ребер:\n\n";
	print_matrix(edge, *num);
	printf("\n");

	vis = (int*)malloc(*num * sizeof(int));
	set = (int*)malloc(*num * sizeof(int));
	vis_prnt = (int*)malloc(*num * sizeof(int));
	for (int i = 0; i < *num; i++) {
		vis_prnt[i] = 0;
	}

	printf("Максимальные независимые множества реберного графа и максиамльные паросочетания исходного графа:\n\n");
	file_write << "\nМаксиамльные паросочетания исходного графа:\n\n";
	for (int i = 0; i < *num; i++) 
	{
		for (int j = 0; j < *num; j++) 
		{
			vis[j] = 0;
		}
		vis[i] = 1;
		set[action] = i;
		action = maximal_independent_set(edge, vis, set, num, vis_prnt, i, action, edge_coord);
		action = 0;
	}
}

void print_matrix(int** matrix, int size) 
{
	for (int i = 0; i < size; i++) 
	{
		for (int j = 0; j < size; j++) 
		{
			printf("%d ", matrix[i][j]);
			file_write << " " << matrix[i][j];
		}
		printf("\n");
		file_write << "\n";
	}
}

void zero_matrix(int** matrix, int size) 
{
	int zero = 0;
	for (int i = 0; i < size; i++) 
	{
		for (int j = 0; j < size; j++) 
		{
			if (matrix[i][j] == 0) zero++;
		}
	}
	if (zero == size * size) 
	{
		printf("\nРебра отсутствуют!\n");
		exit(1);
	}
}

int** generation_matrix(int size) 
{
	int** matrix;
	matrix = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) 
	{
		matrix[i] = (int*)malloc(size * sizeof(int));
	}
	for (int i = 0; i < size; i++) 
	{
		for (int j = 0; j < size; j++) 
		{
			if (j > i) 
			{
				matrix[i][j] = rand() % 100;
				matrix[j][i] = matrix[i][j];
			}
			else if (i == j) 
			{
				matrix[i][j] = 0;
			}
			if (0 <= matrix[i][j] && matrix[i][j] <= 50) 
			{
				matrix[i][j] = 0;
			}
			else if (matrix[i][j] >= 51 && matrix[i][j] <= 100)
			{
				matrix[i][j] = 1;
			}
		}
	}
	return matrix;
}

int** manual_matrix(int size)
{
	int** matrix = (int**)malloc(sizeof(int*) * (size));

	int i = 0;
	int j = 0;

	for (i = 0; i < size; ++i)
	{
		matrix[i] = (int*)malloc(sizeof(int) * size);
	}

	for (i = 0; i < size; ++i)
	{
		matrix[i][i] = 0;
		for (j = i + 1; j < size; ++j)
		{
			do {
				printf("Введите существование (0 или 1) ребра для вершин %d - %d: ", i + 1, j + 1);
				scanf_s("%d", &matrix[i][j]);
				if ((matrix[i][j] < 0) || (matrix[i][j] > 1))
					printf("Неверный ввод! Матрица смежности включает в себя 0 или 1!\n\n");
			} while ((matrix[i][j] < 0) || (matrix[i][j] > 1));
			matrix[j][i] = matrix[i][j];
		}
	}
	return matrix;
}

int** file_matrix(int size) 
{
	int** matrix;

	stack <int> graph;
	int buffer = 0;
	ifstream file_read("read.txt");
	if (!file_read.is_open())
	{
		_cputws(L"Не удалось открыть файл.");
	}

	while (!file_read.eof())
	{
		file_read >> buffer;
		graph.push(buffer);
	}

	file_read.close();
	size = (int)sqrt(graph.size());

	matrix = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++)
		matrix[i] = (int*)malloc(size * sizeof(int));

	for (int i = size - 1; i > -1; i--)
	{
		for (int j = size - 1; j > -1; j--)
		{
			matrix[i][j] = graph.top();
			graph.pop();
		}
	}
	return matrix;
}

int** edge_matrix(int** matrix, int** edge_coord, int size, int* num) {
	int** edge;
	// выделение памяти для хранения матрицы смежности ребер и заполнение ее нулями
	edge = (int**)malloc(*num * sizeof(int*));
	for (int i = 0; i < *num; i++) {
		edge[i] = (int*)malloc(*num * sizeof(int));
	}
	for (int i = 0; i < *num; i++) {
		for (int j = 0; j < *num; j++) {
			edge[i][j] = 0;
		}
	}
	// цикл прохода по исходной матрице с целью задания им порядка и определения их координат
	int iter = 0;
	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			if (matrix[i][j] == 1) {
				edge_coord[0][iter] = iter;
				edge_coord[1][iter] = i;
				edge_coord[2][iter] = j;
				iter++;
			}
		}
	}
	// цикл прохода по массиву координат с целью заполнения реберной матрицы смежности (если находится хотя бы одна одинаковая координата у двух ребер, то они смежные)
	for (int i = 0; i < *num; i++) {
		for (int j = i + 1; j < *num; j++) {
			if ((edge_coord[1][i] == edge_coord[1][j]) || (edge_coord[1][i] == edge_coord[2][j]) || (edge_coord[2][i] == edge_coord[1][j]) || (edge_coord[2][i] == edge_coord[2][j])) {
				edge[i][j] = 1;
				edge[j][i] = 1;
			}
		}
	}
	return edge;
}

int maximal_independent_set(int** matrix, int* vis, int* set, int* num, int* vis_prnt, int start, int action, int** edge_coord) {

	int control = 0;

	for (int i = start; i < *num; i++) {
		if (matrix[start][i] == 0 && vis[i] == 0) {
			int test = 0;
			for (int j = 0; j < action + 1; j++) {
				if (matrix[set[j]][i] == 0) {
					test++;
				}
			}
			if (test == action + 1) {
				control = 1;
				action++;
				set[action] = i;
				vis[i] = 1;
				vis_prnt[i] = 1;
				action = maximal_independent_set(matrix, vis, set, num, vis_prnt, start, action, edge_coord);
			}
		}
	}

	if (control == 0 && (vis_prnt[set[0]] == 0 || action != 0)) {
		printf("[ ");
		for (int i = 0; i < action + 1; i++) {
			printf("%d ", set[i] + 1);
		}
		printf("]			[ ");
		file_write << "[ ";

		for (int i = 0; i < action + 1; i++) {
			printf("%d-%d ", edge_coord[1][set[i]] + 1, edge_coord[2][set[i]] + 1);
			file_write << edge_coord[1][set[i]] + 1 << "-" << edge_coord[2][set[i]] + 1 << " ";
		}
		printf("]\n");
		file_write << "]\n";
	}

	vis[set[action]] = 0;
	action--;
	return action;

}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));

	int size;
	int** mass_vertex;

	char item = '\0';

	cout << "\n\t~~~ М Е Н Ю ~~~\n\n";
	cout << "1. Сгенерировать матрицу смежности.\n";
	cout << "2. Задать матрицу смежности с клавиатуры.\n";
	cout << "3. Вывести матрицу смежности из файла.\n";

	cout << "\nВыведите имя файла результатов: ";
	cin >> file_name;
	file_write.open(file_name);

	cout << "\nПункт меню: ";
	cin >> item;

	if (item == '1')
	{
		do 
		{
			printf("\nВведите порядок исходного графа: ");
			scanf_s("%d", &size);
			if (size < 1)
			{
				printf("Невозможно считать данные! Попробуйте заново!\n");
			}
		} while (size < 1);
		mass_vertex = generation_matrix(size);
		execution(mass_vertex, size);
	}

	if (item == '2')
	{
		do 
		{
			printf("\nВведите порядок исходного графа: ");
			scanf_s("%d", &size);
			if (size < 1)
			{
				printf("Невозможно считать данные! Попробуйте заново!\n");
			}
		} while (size < 1);
		mass_vertex = manual_matrix(size);
		execution(mass_vertex, size);
	}

	if (item == '3')
	{
		do 	
		{
			printf("\nВведите порядок исходного графа: ");
			scanf_s("%d", &size);
			if (size < 1)
			{
				printf("Невозможно считать данные! Попробуйте заново!\n");
			}
		} while (size < 1);
		mass_vertex = file_matrix(size);
		execution(mass_vertex, size);
	}

	if ((item != '1') && (item != '2') && (item != '3'))
	{
		printf("\nНеверный ввод! Попробуйте заново!\n\n");
	}

	file_write.close();
}