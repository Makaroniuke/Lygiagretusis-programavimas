// Lab1b.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include "Plane.h"
#include "Monitor.h"





using namespace std;

string strings[100];
void writeToFile(string fname, vector<Plane> planes);
vector<Plane> readFromFile(string fname);
void writeResultsToFile(string fname, Monitor planes, int sumOfInts, float sumOfFloats);

int main()
{
	Monitor resultMonitor(25);

	//vector<Plane> planes = readFromFile("IF01_CapaiteLaura_L1_dat_1.txt");
	//vector<Plane> planes = readFromFile("IF01_CapaiteLaura_L1_dat_2.txt");
	vector<Plane> planes = readFromFile("IF01_CapaiteLaura_L1_dat_3.txt");
	writeToFile("rez.txt", planes);
	int sumOfInts = 0;
	float sumOfFloats = 0;
	//omp_set_num_threads(4);

	#pragma omp parallel num_threads(4) reduction(+:sumOfInts, sumOfFloats)  shared(planes, resultMonitor) //default(none)
	{
		auto total_threads = omp_get_num_threads();
		auto chunk_size = planes.size() / total_threads;
		auto thread_number = omp_get_thread_num();
		auto start_index = chunk_size * thread_number;
		cout << omp_get_thread_num() << ' ' << start_index << endl;
		auto end_index = thread_number == total_threads - 1 ? planes.size() : ((thread_number + 1) * chunk_size);
		cout << omp_get_thread_num() << ' ' << end_index << endl;
		
				
		for (int i = start_index; i < end_index; i++) {
			planes[i].addResult(planes[i].CountFibonacci(planes[i].getQuantity()));
			if (planes[i].getResult() >= 1000000) {
				resultMonitor.Add(planes[i]);
				sumOfInts += planes[i].getQuantity();
				sumOfFloats += planes[i].getPrice();
				cout << "done" << endl;
			}
		}
					
			
	
	}


	writeResultsToFile("rez.txt", resultMonitor, sumOfInts, sumOfFloats);
	cout << "Program finished execution" << endl;



}

int len(string str)
{
	int length = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		length++;

	}
	return length;
}

void split(string str, char seperator)
{

	int currIndex = 0, i = 0;
	int startIndex = 0, endIndex = 0;
	while (i <= len(str))
	{
		if (str[i] == seperator || i == len(str))
		{
			endIndex = i;
			string subStr = "";
			subStr.append(str, startIndex, endIndex - startIndex);
			strings[currIndex] = subStr;
			currIndex += 1;
			startIndex = endIndex + 1;
		}
		i++;
	}
}

vector<Plane> readFromFile(string fname) {
	vector<Plane> planes;
	fstream myFile;
	myFile.open(fname, ios::in); // read mode
	if (myFile.is_open()) {
		string line;
		size_t pos = 0;
		
		char delim = ',';
		// reads from file and stores in 'line' variable
		while (getline(myFile, line)) {
			string brand, quantity, price;
			vector<string> str;
			split(line, delim);
			
			brand = strings[0];
			quantity = strings[1];
			price = strings[2];
			
			Plane plane(brand, stoi(quantity), stoi(price));
			planes.push_back(plane);
		}
			
		myFile.close();
	}
	return planes;
}

void writeToFile(string fname, vector<Plane> planes) {
	fstream myFile;

	myFile.open(fname, ios::out); //write mode
	if (myFile.is_open())
	{
		myFile << "Pradiniai duomenys: " << endl
			<< "------------------------------------------------------------------------------------------------------------------------"
			<< endl
			<< setw(30) << "Pavadinimas |" << setw(11) << "Kiekis |" << setw(20) << "Kaina |" << setw(15)
			<< "Rezultatas |" << endl
			<< "------------------------------------------------------------------------------------------------------------------------"
			<< endl;
		for (int i = 0; i < planes.size(); i++) {
			myFile << setw(30) << planes[i].getBrand() + " |" << setw(10) << to_string(planes[i].getQuantity()) + " |" << setw(20)
				<< to_string(planes[i].getPrice()) + " |" << setw(15) << to_string(planes[i].getResult()) + " |"
				<< endl;
		}
		myFile.close();
	}
}


void writeResultsToFile(string fname, Monitor planes, int sumOfInts, float sumOfFloats) {
	fstream myFile;

	myFile.open(fname, ios::app); //write mode
	if (myFile.is_open())
	{
		myFile << "Rezultatai: " << endl
			<< "------------------------------------------------------------------------------------------------------------------------"
			<< endl
			<< setw(30) << "Pavadinimas |" << setw(11) << "Kiekis |" << setw(20) << "Kaina |" << setw(15)
			<< "Rezultatas |" << endl
			<< "------------------------------------------------------------------------------------------------------------------------"
			<< endl;
		for (int i = 0; i < planes.getSize(); i++) {
			myFile << setw(30) << planes.getItem(i).getBrand() + " |" << setw(10) << to_string(planes.getItem(i).getQuantity()) + " |" << setw(20)
				<< to_string(planes.getItem(i).getPrice()) + " |" << setw(15) << to_string(planes.getItem(i).getResult()) + " |"
				<< endl;
		}
		myFile << "Suma int: " << sumOfInts << endl;
		myFile << "Suma float: " << sumOfFloats << endl;
		myFile.close();
	}

}



