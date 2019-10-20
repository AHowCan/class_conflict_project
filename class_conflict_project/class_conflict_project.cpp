// CS7350_acanter_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h> 
#include <list>
#include <iterator>
#include <vector>

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
	long const C = 10000; // Number of courses
	long const S = 1000;  // Number of students
	long const K = 100;  // Number of courses per student
	std::string dist = "uniform";
	long M = 0;	// Distinct pair-wise course conflicts
	long T = 0;	// Total pair-wise course conflicts
	std::list<long> E[1];	// Adj list of distinct course conflicts
	std::list<long> P[1];	// Pointing to each course conflicts...???

	std::list<long> student_list[S];
	std::list<long> conflict_list[C+1];

	srand(time(NULL));



	for (int j = 0; j < S; j++) {
		std::vector<long> dup_check(C+1);
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

			}
			else if (dist == "new") {

			}
			class_number = rand() % C + 1;
			// std::cout << " C = " << C << " and class_number = " << class_number << std::endl;
			
			while(dup_check.at(class_number) == 1){
				if (class_number != 10)
					class_number++;
				else
					class_number = rand() % 10 + 1;
			}
			dup_check.at(class_number) = 1;
			
			student_list[j].push_back(class_number);
			// Discussion point 1

			// Need to remove duplicate courses entries. How should it be done?
			// Generating a new number is changing the distribution...maybe?
			// Initial number could be 2 out of 100 but then after having to re-generate
			//   a number, it could be 99.
			// Is adding 1 to the inital value better?

			//while (check_distinct(student_list, j, class_number)) {
				// if (count % 2 == 0)
				//	count = -count;
				// else
				//	count = abs(count);
				//class_number++;
			//}	
		}
	}

	// Displaying class for students
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
				if (dup_check[*class_x][*class_y] != 1) {
					conflict_list[*class_x].push_back(*class_y);
					dup_check[*class_x][*class_y] = 1;
					M++;
				}
				T++;
				class_y++;
			}
		}
	}
	std::cout << "The amount of conflicts is " << M << std::endl;
	std::cout << "The total amount of conflicts is " << T << std::endl;
	std::cout << "Hello World!\n";
}

