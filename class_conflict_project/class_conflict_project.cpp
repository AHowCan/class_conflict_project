// CS7350_acanter_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h> 
#include <list>
#include <iterator>
#include <vector>
#include <random>

//void add_conflict(std::list<int> conflict_list[], int v1, int v2) {
//	conflict_list[v1].push_back[v2];
//	conflict_list[v2].push_back[v1];
//}
/*
struct node {
	int data;
	struct node* next;
	struct node* prev;
	std::vector<int> conflict;
};

void create_list(struct node** head, int data) {
	struct node* newNode = new node;
	newNode->data = data;

	newNode->next = (*head);
	newNode->prev = NULL;
}


bool check_distinct(std::list<int> conflict_list[], int index, int value) {
	std::list<int>::iterator check = conflict_list[index].begin();
	std::list<int>::iterator last = conflict_list[index].end();
	while (check != last) {
		if (*check == value) {
			return true;
		}
		check++;
	}
	return false;
}
*/

int main()
{
	long const C = 30; // Number of courses
	long const S = 500;  // Number of students
	long const K = 3;  // Number of courses per student
	std::string dist = "4-tiered";
	long M = 0;	// Distinct pair-wise course conflicts
	long T = 0;	// Total pair-wise course conflicts
	std::list<long> E[C+1];
	std::list<long> P[C+1];

	std::list<long> student_list[S];
	std::list<long> conflict_list_DISTINCT[C + 1]; // My E[]
	std::list<long> conflict_list_ALL[C + 1]; // My P[]
	long class_count[C+1] = { 0 };

	srand(time(NULL));

	for (int j = 0; j < S; j++) {
		std::vector<long> dup_check(C + 1);
		int class_number = 0;
		for (int i = 0; i < K; i++) {
			if (dist == "uniform") {
				class_number = rand() % C + 1;
			}
			else if (dist == "skewed") {
				int c_1 = rand() % C + 1;
				int c_2 = rand() % C + 1;
				if (c_1 < c_2)
					class_number = c_1;
				else
					class_number = c_2;

			}
			else if (dist == "4-tiered") {
				int percentage = rand() % 100 + 1;
				if (percentage <= 40) {
					class_number = rand() % (C / 4) + 1;
				}
				else if (percentage <= 70) {
					int random = rand();
					class_number = random % (C / 4) + ((C / 4));
				}
				else if (percentage <= 90) {
					int random = rand();
					class_number = random % (C / 4) + (2 * (C / 4));
				}
				else {
					int random = rand();
					class_number = random % (C / 4) + (3 * (C / 4));
				}
			}
			else if (dist == "new") {
				// Discussion Point : Value of 1 is impossible 
				// Creates leaning bell curve towards higher numbers
				int c_1 = rand() % C / 2 + 1;
				int c_2 = rand() % C / 2 + 1;
				int c_3 = rand() % C / 2 + 1;
				if (c_1 >= c_2) {
					if (c_3 > c_2)
						class_number = c_1 + c_3;
					else
						class_number = c_1 + c_2;
				}
				else {
					if (c_1 >= c_3)
						class_number = c_2 + c_3;
				}
			}

			// Check for duplicates by using a 1d array, if =0, new value, if =1 add one to the value
			while (dup_check.at(class_number) == 1) {
				if (class_number != C)
					class_number++;
				else
					class_number = 1;
			}
			dup_check.at(class_number) = 1;
			class_count[class_number]++;
			student_list[j].push_back(class_number);
			// Discussion point 1

			// Need to remove duplicate courses entries. How should it be done?
			// Generating a new number is changing the distribution...maybe?
			// Initial number could be 2 out of 100 but then after having to re-generate
			//   a number, it could be 99.
			// Is adding 1 to the inital value better? Eventually will get empty spot in linear time.	
		}
	}

	// Displaying classes for students
	/*
	for (int j = 0; j < S; j++) {
		std::cout << "Student " << j << " is taking courses ";
		std::list<long>::iterator classes;
		for (classes = student_list[j].begin(); classes != student_list[j].end(); classes++) {
			std::cout << *classes << " ";
		}
		std::cout << std::endl;
	}
	*/

	std::cout << "Before the conflict check" << std::endl;
	std::vector<std::vector<long>> dup_check(C + 1, std::vector<long>(C + 1, 0));
	for (int j = 0; j < S; j++) {
		std::list<long>::iterator class_x;
		std::list<long>::iterator class_y;
		for (class_x = student_list[j].begin(); class_x != student_list[j].end(); class_x++) {
			class_y = std::next(class_x);
			while (class_y != student_list[j].end()) {
				// Create conflict list - has duplicates, used for p[]
				conflict_list_ALL[*class_x].push_back(*class_y);

				// Used to prevent dup entries - Student1 = {5,9}, Student = {9,5}
				// Always use smaller number first for dup entry table, 
				// Calculate distinct conflict list using lowest number first method
				if (*class_x >= *class_y) {
					std::list<long>::iterator placeholder_x;
					std::list<long>::iterator placeholder_y;
					placeholder_x = class_y;
					placeholder_y = class_x;

					if (dup_check[*placeholder_x][*placeholder_y] != 1) {
						conflict_list_DISTINCT[*placeholder_x].push_back(*placeholder_y);
						dup_check[*placeholder_x][*placeholder_y] = 1;
						M++;
					}

				}
				else {
					if (dup_check[*class_x][*class_y] != 1) {
						conflict_list_DISTINCT[*class_x].push_back(*class_y);
						dup_check[*class_x][*class_y] = 1;
						M++;
					}
				}
				// Could just use k*(k-1) / 2 to calculate T, but use this to help ensure the proper amount of loops
				T++;
				class_y++;
			}
		}
	}
	std::ofstream file;
	file.open("student_count.txt");
	for (int i = 1; i < C+1; i++)
		file << i << "\t" << class_count[i] << std::endl;
	file.close();

	std::cout << "The amount of conflicts is " << M << std::endl;
	std::cout << "The total amount of conflicts is " << T << std::endl;
}

