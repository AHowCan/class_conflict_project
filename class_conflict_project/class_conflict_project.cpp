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

int main()
{
	node* head = NULL;
	node* second = NULL;
	head = new node();
	head->data = 5;
	head->next = second;
	int const C = 10; // Number of courses
	int const S = 10;  // Number of students
	int const K = 5;  // Number of courses per student
	std::string dist = "linear";
	int M = 0;	// Distinct pair-wise course conflicts
	int T = 0;	// Total pair-wise course conflicts
	std::list<int> E[1];	// Adj list of distinct course conflicts
	std::list<int> P[1];	// Pointing to each course conflicts...???

	std::list<int> student_list[S];
	std::list<int> conflict_list[C+1];

	srand(time(NULL));

	for (int j = 0; j < S; j++) {
		std::vector<int> dup_check(C+1);
		for (int i = 0; i < K; i++) {
			int class_number = rand() % C;
			while(dup_check.at(class_number) == 1){
				class_number++;
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

	for (int j = 0; j < S; j++) {
		std::cout << "Student " << j << " is taking courses ";
		std::list<int>::iterator classes;
		for (classes = student_list[j].begin(); classes != student_list[j].end(); classes++) {
			std::cout << *classes << " ";
		}
		std::cout << std::endl;
	}

	for (int j = 0; j < S; j++) {
		std::list<int>::iterator class_x;
		std::list<int>::iterator class_y;
		//std::vector<int[20][30]> dup_check(0)(0);
		std::vector<std::vector<int>> dup_check(C+1,std::vector<int>(C+1,0));
		for (class_x = student_list[j].begin(); class_x != student_list[j].end(); class_x++) {
			class_y = std::next(class_x);
			while (class_y != student_list[j].end()) {
				if (dup_check[*class_x][*class_y] != 1) {
					conflict_list[*class_x].push_back(*class_y);
					dup_check[*class_x][*class_y] = 1;
					M++;
				}
				else
					T++;
				class_y++;
			}
			T += M;
			//class_y = std::next(class_x);
			//while (dup_check[*class_x][*class_y] != 1) {
			//	conflict_list[*class_x].push_back(*class_y);
			//	dup_check[*class_x][*class_y] = 1;
			//	M++;
			//}
		}
		

	}
	std::cout << "The amount of conflicts is " << M << std::endl;
	std::cout << "Hello World!\n";
}

