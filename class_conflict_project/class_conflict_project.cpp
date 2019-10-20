// CS7350_acanter_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h> 
#include <list>
#include <iterator>

//void add_conflict(std::list<int> conflict_list[], int v1, int v2) {
//	conflict_list[v1].push_back[v2];
//	conflict_list[v2].push_back[v1];
//}

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
	int const C = 10; // Number of courses
	int const S = 20;  // Number of students
	int const K = 3;  // Number of courses per student
	std::string dist = "linear";
	int M = 0;	// Distinct pair-wise course conflicts
	int T = 0;	// Total pair-wise course conflicts
	std::list<int> E[1];	// Adj list of distinct course conflicts
	std::list<int> P[1];	// Pointing to each course conflicts...???

	std::list<int> adj_list[S];
	std::cout << " Declaring bankrupcy " << std::endl;
	srand(time(NULL));
	for (int j = 0; j < S; j++) {
		for (int i = 0; i < K; i++) {
			int class_number = rand() % C + 1;

			// Discussion point 1
			// Need to remove duplicate courses entries. How should it be done?
			// Generating a new number is changing the distribution...maybe?
			// Initial number could be 2 out of 100 but then after having to re-generate
			//   a number, it could be 99.
			// Is adding 1 to the inital value better?
			int count = 1;
			while (check_distinct(adj_list, j, class_number)) {
				std::cout << " While Loop" << std::endl;

				// if (count % 2 == 0)
				//	count = -count;
				// else
				//	count = abs(count);
				class_number++;
			}
			adj_list[j].push_back(class_number);
		}
	}

	for (int j = 0; j < S; j++) {
		std::cout << "Student " << j << " is taking courses ";
		std::list<int>::iterator classes;
		for (classes = adj_list[j].begin(); classes != adj_list[j].end(); classes++) {
			std::cout << *classes << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Hello World!\n";
}

