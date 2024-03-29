// CS7350_acanter_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <chrono>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h> 
#include <list>
#include <iterator>
#include <vector>
#include <random>
#include <iomanip>

long const C = 5; // Number of courses
long const S = 15;  // Number of students
long const K = 3;  // Number of courses per student
std::list<long> student_list[S];
std::list<long> E[C + 1]; // My E[]
std::list<long> P[C + 1]; // My P[]

void part1();
void part2();

struct vertex {
	int name;
	int degree = 0;
	int originalDegree = 0;
	int degreeWhenDeleted = 0;
	std::list<long>::iterator adjList;
	std::list<vertex *> padjList;
	struct vertex *degListNext = NULL;
	struct vertex *degListPrev = NULL;
	int deleted = 0; 
	int color = 0;
};

void main()
{
	part1();
	part2();
}

void part2() 
{
	std::string method = "m1";

	// Read in file
	/*
	std::ifstream e_list("p_list.txt");
	if (!e_list)
		return 1;
	char str[255];
	std::vector<int> c_adj; 
	int value;
	
	while (e_list) {
		e_list.getline(str, 255);
		std::istringstream iss(str);
		std::string string;

		//std::cout << str << std::endl;
		std::stringstream ss;
		while(ss << str){
			ss >> value;
			std::cout << value << " " << std::endl;
		}
	}
	*/

	// Building vertex struct array
	std::list<long>::iterator course_x;
	vertex courses[C+1];
	for (int i = 0; i <= C; i++) {
		courses[i].name = i;
		courses[i].adjList = P[i].begin();

		course_x = courses[i].adjList;
		int counter = 0;
		for (course_x; course_x != P[i].end();course_x++) {
			counter++;
		}
		courses[i].degree = counter;
		courses[i].originalDegree = counter;
	}
	// create adj pointer list in vertex
	for (int i = 0; i <= C; i++) {
		for (course_x = P[i].begin(); course_x != P[i].end();course_x++) {
			courses[i].padjList.push_back(&courses[*course_x]);			
		}
	}

	//// Create degree list with add next/prev degree for a doubly linked list
	//std::cout << "Creating degList:" << std::endl;
	//std::list<vertex *> degList[C]; // List of ptr to vertices
	//for (int i = 0; i <= C; i++) {
	//	// Locate degList index by using course i degree and pushing the name on the degList
	//	degList[courses[i].degree].push_front(&courses[i]);
	//	// Assign next and prev vertex. Default values are null. i=0 needs to stay null will assign nextdeg on n=1
	//	if (i > 0) {
	//		courses[i].degListPrev = &courses[i - 1];
	//		courses[i - 1].degListNext = &courses[i];
	//	}
	//}

	// Print adjacency list
	/*std::list<vertex *>::iterator c_deg; // going to be a ptr to a ptr
	for (int i = 0; i < c; i++) {
		std::cout << i;
		for (c_deg = deglist[i].begin(); c_deg != deglist[i].end(); c_deg++) {
			vertex temp = **c_deg;
			std::cout << " " << temp.name;
		}
		std::cout << std::endl;
	}*/

	if (method == "m1") {
		// Method 1 - Smallest Last Vertex ordering

		auto t1 = std::chrono::high_resolution_clock::now();
		// Create degree list with add next/prev degree for a doubly linked list
		std::cout << "Creating degList:" << std::endl;
		std::list<vertex *> degList[C]; // List of ptr to vertices
		for (int i = 0; i <= C; i++) {
			// Locate degList index by using course i degree and pushing the name on the degList
			degList[courses[i].degree].push_front(&courses[i]);
			// Assign next and prev vertex. Default values are null. i=0 needs to stay null will assign nextdeg on n=1
			if (i > 0) {
				courses[i].degListPrev = &courses[i - 1];
				courses[i - 1].degListNext = &courses[i];
			}
		}

		vertex* deletedCourse[C+1]; // List of vertex pointers
		std::list<vertex *>::iterator course_x_deg; // Going to be a ptr to a ptr
		std::list<vertex *>::iterator course_y_deg; // Going to be a ptr to a ptr
		int deleteCount = C;
		int terminal_clique = 0;
		bool fallback = false;
		bool found_terminal_clique = false;
		for (int i = 0; i < C; i++) {
			// If degList is not empty and a delete will happen, check the previous degList for new vertex
			if(fallback){
				fallback = false;
				if (i > 2) i -= 2;
			}
			for (course_x_deg = degList[i].begin(); course_x_deg != degList[i].end(); course_x_deg++) {
				vertex* temp_x = *course_x_deg;

				// Check for clique by checking for all non-delete vertices having the same degree
				// More efficent to check orderlist(O(n) if none have been deleted) or go through all courses to check(O(n))
				// Going for easy implementation since same big-O
				if (!found_terminal_clique) {
					int current_degree = 0;
					for (int k = 1; k <= C; k++) {
						if (courses[k].deleted != 1) {
							current_degree = courses[k].degree;
							break;
						}
					}
					for (int k = 1; k <= C; k++) {
						if (courses[k].deleted != 1) {
							if (current_degree != courses[k].degree) {
								break;
							}
							if (k == C) {
								found_terminal_clique = true;
								terminal_clique = current_degree;
							}
						}
						if (k == C) {
							found_terminal_clique = true;
							terminal_clique = current_degree;
						}
					}
				}
				if (temp_x->deleted == 0) {
					vertex* temp_y = std::next(*course_x_deg);
					temp_x->deleted = 1;
					temp_x->degreeWhenDeleted = temp_x->degree;
					//temp_x->degree = 0;
					deletedCourse[deleteCount] = temp_x;
					deleteCount--;

					// Need to remove from doubly link degList and move next vertex over to Null (always Null?)
					temp_y->degListPrev = NULL;

					// Update Deleted course class conflicts degrees
					for (course_x = P[temp_x->name].begin(); course_x != P[temp_x->name].end();course_x++) {
						// Check degPrev and degNext
						if (courses[*course_x].degListPrev != NULL && courses[*course_x].degListNext != NULL) {
							vertex* temp_x = courses[*course_x].degListPrev;
							vertex* temp_y = courses[*course_x].degListNext;
							temp_x->degListNext = temp_y;
							temp_y->degListPrev = temp_x;
						}
						else if (courses[*course_x].degListPrev != NULL && courses[*course_x].degListNext == NULL) {
							vertex* temp_x = courses[*course_x].degListPrev;
							temp_x->degListNext = NULL;
						}
						else if (courses[*course_x].degListPrev == NULL && courses[*course_x].degListNext != NULL) {
							vertex* temp_x = courses[*course_x].degListNext;
							temp_x->degListPrev = NULL;
						}
						else { // Should only happen when course degListPrev == NULL and degListNext == NULL
							// Nothing needs to happen..........
						}

						//// Delete from degList
						//std::list<vertex *>::iterator ptemp_v;
						//ptemp_v = degList[courses[*course_x].degree].begin();
						//vertex *pvertex = *ptemp_v;
						//while (courses[pvertex->name].name != courses[*course_x].name) {
						//	ptemp_v++;
						//	pvertex = *ptemp_v;
						//}
						//ptemp_v = degList[courses[*course_x].degree].erase(ptemp_v);

						// Decrease course_x degree int
						if(courses[*course_x].degree != 0)
							courses[*course_x].degree--;

						// Add to end of adjList lower degree row and add new degListPrev and remove any degListNext
						std::list<vertex *>::iterator ptemp_course;
						if (degList[courses[*course_x].degree].begin() != degList[courses[*course_x].degree].end()) {
							ptemp_course = degList[courses[*course_x].degree].begin();
							while (std::next(ptemp_course) != degList[courses[*course_x].degree].end()) {
								ptemp_course++;
							}
							degList[courses[*course_x].degree].push_back(&courses[*course_x]);
							vertex* temp = *ptemp_course;
							temp->degListNext = &courses[*course_x];
							courses[*course_x].degListNext = NULL;
							courses[*course_x].degListPrev = *ptemp_course;
							//ptemp_course = degList[courses[*course_x].degree].erase(ptemp_course);
						}
						else {
							degList[courses[*course_x].degree].push_back(&courses[*course_x]);
							courses[*course_x].degListNext = NULL;
							courses[*course_x].degListPrev = NULL;
							//ptemp_course = degList[courses[*course_x].degree].erase(ptemp_course);
						}
						// if statement to breakout out multiple for loops if needing to go a row back in degList
						if (courses[*course_x].degree < i) {
							while (std::next(course_x_deg) != degList[i].end()) {
								course_x_deg++;
							}
							fallback = true;
							break;
						}
					}
				}
			}
		}// Stop execution time and compare
		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration_M = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		std::cout << "The execution time for M1 in milliseconds is " << duration_M << std::endl;
		// Color the deleted vertices
		for (int i = 0; i <= C; i++) {
			int colorArr[C + 1] = { 0 };
			vertex* temp_x = deletedCourse[i];
			std::list<vertex *>::iterator ptemp_course;
			for (ptemp_course = deletedCourse[i]->padjList.begin(); ptemp_course != deletedCourse[i]->padjList.end(); ptemp_course++) {
				vertex* temp = *ptemp_course;
				if (temp->color != 0) {
					colorArr[temp->color] = 1;
				}
			}
			for (int j = 1; j < C + 1; j++) {
				if (colorArr[j] == 0) {
					temp_x->color = j;
					break;
				}
			}
		}
		// Output Details
		int max_degree_deleted = 0;
		std::cout << std::endl << "Ordered colored | Degree value when deleted" << std::endl;
		std::cout << "---------------------------------------------------" << std::endl;
		for (int i = 0; i < C; i++) {
			std::cout << std::setw(4) << deletedCourse[i]->name << "  |";
			if (deletedCourse[i]->degreeWhenDeleted > max_degree_deleted) max_degree_deleted = deletedCourse[i]->degreeWhenDeleted;
			for (int j = 1; j <= deletedCourse[i]->degreeWhenDeleted; j++) {
				std::cout << "*" << j;
			}
			std::cout << "*" << std::endl;
		}
		std::cout << std::endl << "The largest degree when deleted was " << max_degree_deleted;
		std::cout << std::endl << "The terminal clique is " << terminal_clique;
		std::cout << std::endl;

		std::ofstream file;
		file.open("vertex_info.txt");
		file << std::setw(15) << "Course Number " << std::setw(15) << "Color " << std::setw(20) << "Original Degree " << std::setw(15) << " Deg when deleted" << std::endl;
		for (int i = 0; i < C; i++) {
			file << std::setw(15) << deletedCourse[i]->name << std::setw(15) << deletedCourse[i]->color << std::setw(15) << deletedCourse[i]->originalDegree << std::setw(15) << deletedCourse[i]->degreeWhenDeleted << std::endl;
		}
		file << "The largest degree when deleted was " << max_degree_deleted;
		file.close();

	}
	else if (method == "m2") {
		// Method-2 Welsh-Powell
		
		//Make degree list
		std::cout << "Creating degList:" << std::endl;
		std::list<vertex *> degList_M2[C]; // List of ptr to vertices
		for (int i = 0; i <= C; i++) {
			// Locate degList index by using course i degree and pushing the name on the degList
			degList_M2[courses[i].degree].push_front(&courses[i]);
			// Assign next and prev vertex. Default values are null. i=0 needs to stay null will assign nextdeg on n=1
			if (i > 0) {
				courses[i].degListPrev = &courses[i - 1];
				courses[i - 1].degListNext = &courses[i];
			}
		}
		// Create vertex list from greatest to least
		vertex* vertexList[C + 1];
		int listIndex = 0;
		for (int i = C-1; i >= 0; i--) {
			std::list<vertex *>::iterator course_x_deg;
			for (course_x_deg = degList_M2[i].begin(); course_x_deg != degList_M2[i].end(); course_x_deg++) {
				vertexList[listIndex] = *course_x_deg;
				listIndex++;
			}
		}
		// Start coloring the vertices
		int colorIndex = 1;
		for (int i = 0; i < C; i++) { // Used for vertex
			if (vertexList[i]->color == 0) {
				vertexList[i]->color = colorIndex;
				std::list<vertex *>::iterator adjCourse;
				int adjacencyArray[C+1] = { 0 };
				for (adjCourse = vertexList[i]->padjList.begin(); adjCourse != vertexList[i]->padjList.end(); adjCourse++) {
					vertex* pvertex = *adjCourse;
					adjacencyArray[pvertex->name] = 1;
				}
				adjacencyArray[vertexList[i]->name] = 1;
				for (int j = 0; j <= C; j++) {
					if (adjacencyArray[j] == 0 && courses[j].color == 0) {
						courses[j].color = colorIndex;
						// Unsure how to prevent coloring of an adj vertex to the colored vertex if it's also not a neighbor
					}
				}
				colorIndex++;
			}
		}
	}
	else if (method == "m3") {
		// Method-3 Uniform Random ordering
		
		// Take courses and randomly select a number and place in the array
		vertex* randomOrder[C+1] = { NULL };
		int used[C+1] = { 0 };
		int class_number = 0;
		for (int i = 0; i < C; i++) {
			class_number = rand() % C + 1;
			while (used[class_number] != 0) {
				class_number++;
				if (class_number >= C) class_number = 1;
			}
			used[class_number] = 1;
			randomOrder[i] = &courses[class_number];
		}
		// Color vertices
		for (int i = 0; i < C; i++) {
			int colorArr[C + 1] = { 0 };
			vertex* temp_x = randomOrder[i];
			std::list<vertex *>::iterator ptemp_course;
			for (ptemp_course = randomOrder[i]->padjList.begin(); ptemp_course != randomOrder[i]->padjList.end(); ptemp_course++) {
				vertex* temp = *ptemp_course;
				if (temp->color != 0) {
					colorArr[temp->color] = 1;
				}
			}
			for (int j = 1; j < C + 1; j++) {
				if (colorArr[j] == 0) {
					temp_x->color = j;
					break;
				}
			}
		}
	}
	else if (method == "m4") {
		// Method-4 My own choice
		
		// Numeric ordering based on vertice name (number)
		vertex* numericOrder[C + 1] = { NULL };
		for (int i = 0; i <= C; i++) {
			numericOrder[i] = &courses[i];
		}
		// Color Vertices
		for (int i = 0; i <= C; i++) {
			int colorArr[C + 1] = { 0 };
			vertex* temp_x = numericOrder[i];
			std::list<vertex *>::iterator ptemp_course;
			for (ptemp_course = numericOrder[i]->padjList.begin(); ptemp_course != numericOrder[i]->padjList.end(); ptemp_course++) {
				vertex* temp = *ptemp_course;
				if (temp->color != 0) {
					colorArr[temp->color] = 1;
				}
			}
			for (int j = 1; j < C + 1; j++) {
				if (colorArr[j] == 0) {
					temp_x->color = j;
					break;
				}
			}
		}
	}


	std::cout << "win";
	//for (int i = 0; i <= C; i++) {
	//	std::cout << i << "=" << deletedCourse[i]->name << " ";
	//}

	// Print All vertex deleted state
	//std::cout << std::endl << "Vertex Delete state";
	//for (int i = 0; i < C; i++) {
	//	std::cout << i << " " << courses[i].deleted << std::endl;
	//}
}

void part1()
{

	std::string dist = "uniform";
	long M = 0;	// Distinct pair-wise course conflicts
	long T = 0;	// Total pair-wise course conflicts
	//std::list<long> E[C + 1];
	//std::list<long> P[C + 1];

	long class_count[C + 1] = { 0 }; // For graphing distributions

	srand(time(NULL));
	auto t1 = std::chrono::high_resolution_clock::now();
	// Calc and create distribution
	for (int j = 0; j < S; j++) {
		std::vector<long> dup_check(C + 1);
		int class_number = 0;
		// Create distribution
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
				if (percentage < 40) {
					class_number = rand() % (C / 4) + 1;
				}
				else if (percentage < 70) {
					int random = rand();
					class_number = random % (C / 4) + ((C / 4) + 1);
				}
				else if (percentage < 90) {
					int random = rand();
					class_number = random % (C / 4) + (2 * (C / 4) + 1);
				}
				else {
					int random = rand();
					class_number = random % (C / 4) + (3 * (C / 4) + 1);
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
						class_number = c_2 + c_1;
					else
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
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::wcout << "Duration is = " << duration << "ms" << std::endl;

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
	auto t3 = std::chrono::high_resolution_clock::now();
	std::vector<std::vector<long>> dup_check(C + 1, std::vector<long>(C + 1, 0));
	std::vector<std::vector<long>> dup_check_P(C + 1, std::vector<long>(C + 1, 0));
	for (int j = 0; j < S; j++) {
		std::list<long>::iterator class_x;
		std::list<long>::iterator class_y;
		for (class_x = student_list[j].begin(); class_x != student_list[j].end(); class_x++) {
			class_y = std::next(class_x);
			while (class_y != student_list[j].end()) {
				// Create conflict list - has duplicates, used for p[]
				if (dup_check_P[*class_x][*class_y] != 1) {
					P[*class_x].push_back(*class_y);
					dup_check_P[*class_x][*class_y] = 1;
				}
				if (dup_check_P[*class_y][*class_x] != 1) {
					P[*class_y].push_back(*class_x);
					dup_check_P[*class_y][*class_x] = 1;
				}
				// Used to prevent dup entries - Student1 = {5,9}, Student = {9,5}
				// Always use smaller number first for dup entry table, 
				// Calculate distinct conflict list using lowest number first method
				if (*class_x >= *class_y) {
					std::list<long>::iterator placeholder_x;
					std::list<long>::iterator placeholder_y;
					placeholder_x = class_y;
					placeholder_y = class_x;

					if (dup_check[*placeholder_x][*placeholder_y] != 1) {
						E[*placeholder_x].push_back(*placeholder_y);
						dup_check[*placeholder_x][*placeholder_y] = 1;
						M++;
					}

				}
				else {
					if (dup_check[*class_x][*class_y] != 1) {
						E[*class_x].push_back(*class_y);
						dup_check[*class_x][*class_y] = 1;
						M++;
					}
				}
				// Could just use k*(k-1) / 2 to calculate T, but use this to help ensure the correct amount of loops
				T++;
				class_y++;
			}
		}
	}
	auto t4 = std::chrono::high_resolution_clock::now();
	auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
	std::wcout << "Duration is = " << duration_2 << "micro-seconds" << std::endl;
	std::ofstream file;
	file.open("student_count_per_course.txt");
	for (int i = 1; i < C + 1; i++)
		file << i << "\t" << class_count[i] << std::endl;
	file.close();
	std::list<long>::iterator class_x;
    file.open("course_conflicts.txt");
	for (int i = 0; i < C + 1; i++) {
		file << i;
		for (class_x = E[i].begin(); class_x != E[i].end(); class_x++)
			file << "->" << *class_x;
		file << std::endl;
	}
	file.close();

	//std::list<long>::iterator class_x;
	file.open("student_list_of_courses_taking.txt");
	for (int i = 0; i < S; i++) {
		file << i+1;
		for (class_x = student_list[i].begin(); class_x != student_list[i].end(); class_x++)
			file << "->" << *class_x;
		file << std::endl;
	}
	file.close();

	file.open("p_list.txt");
	for (int i = 0; i < C + 1; i++) {
		file << i;
		for (class_x = P[i].begin(); class_x != P[i].end(); class_x++)
			file << " " << *class_x;
		file << std::endl;
	}
	file.close();

	file.open("e_list.txt");
	for (int i = 0; i < C + 1; i++) {
		file << i;
		for (class_x = E[i].begin(); class_x != E[i].end(); class_x++)
			file << " " << *class_x;
		file << std::endl;
	}
	file.close();

	//std::cout << "The amount of conflicts is " << M << std::endl;
	//std::cout << "The total amount of conflicts is " << T << std::endl;
	//std::cin.get();
}

