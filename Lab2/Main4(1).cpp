// Lab1b.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>

#include <fstream>
#include <string>
#include <list> 
#include <vector>
#include <iomanip>
#include <stdio.h>
#include "Plane.h"
#include "Monitor.h"
#include <mpi.h>
#include <array>
#include <bits/stdc++.h>






using namespace std;

string strings[100];

void split(string str, char seperator);
void writeToFile(string fname, vector<Plane> planes);
vector<Plane> readFromFile(string fname);
string makePlaneToString(Plane plane);
int findIndex(Plane plane, int count, Plane planes[]);
void shiftArray(int index, int count, Plane planes[]);
void writeResultsToFile(string fname, Plane planes[], int length);
//void writeResultsToFile(string fname, Monitor planes, int sumOfInts, float sumOfFloats);

int main()
{
	MPI::Init();
	
	//Monitor resultMonitor(25);

	//vector<Plane> planes = readFromFile("IF01_CapaiteLaura_L1_dat_1.txt");
	//vector<Plane> planes = readFromFile("IF01_CapaiteLaura_L1_dat_2.txt");
	//vector<Plane> planes = readFromFile("IF01_CapaiteLaura_L1_dat_3.txt");
	


	auto rank = MPI::COMM_WORLD.Get_rank();
	
	// pagrindinis procesas
	if(rank == 0){
		vector<Plane> planes = readFromFile("IF01_CapaiteLaura_L1_dat_1.txt");
		writeToFile("rez.txt", planes);
		//vector<Plane> results;
		Plane results[25];
		int count = 0;
		for(int i = 0; i < planes.size(); i++){					
			auto plane = planes[i];
			string serialized = makePlaneToString(plane);
			auto serialized_size = serialized.length();
			const char* serialized_chars = serialized.c_str();
			MPI::COMM_WORLD.Send(serialized_chars,serialized_size, MPI::CHAR, 1, 1);
			
		}
		// issiusti baigimo signala
		int end_signal = 1;
		
		char received_message[60];
		while(true){
			MPI::Status statuss;
			MPI::COMM_WORLD.Probe(2, 1, statuss);
			MPI::COMM_WORLD.Recv(received_message, statuss.Get_count(MPI::CHAR), MPI::CHAR, statuss.Get_source(), 1);
			string message(&received_message[0], &received_message[statuss.Get_count(MPI::CHAR)]);
			if(message.length() == 1) break;
			
			split(message, ',');
			auto brand = strings[0];
			auto quantity = strings[1];
			auto price = strings[2];
			auto result = strings[3];			
			Plane plane(brand, stoi(quantity), stoi(price), stoi(result));
			//results.push_back(plane);
			int index = findIndex(plane, count, results);
			shiftArray(index, count, results);
			results[index] = plane;
			count++;
		}
		int length = sizeof(results)/sizeof(results[0]);
		writeResultsToFile("rez.txt", results, length);
		
		
	// duomenu procesas
	}else if(rank == 1){
		string numbers[12];
		//array<string, 12> numbers;
		
		int elementsInBuffer = 0;
		//char msgBuffer[30];
		char received_message[60];
		int received_items = 0;
		while(received_items < 25){
			int len = sizeof(numbers)/sizeof(numbers[0]);

			
			
			if(len == elementsInBuffer){
				// nera vietos prideti
				// tik siusti darbininkui
				string num = numbers[elementsInBuffer];
				numbers[elementsInBuffer] = "";
				
				auto message_size = num.length();
				const char* message_chars = num.c_str();
				char received_signal[1];
				
				MPI::Status status;
				MPI::COMM_WORLD.Probe(MPI::ANY_SOURCE, 2, status);
				MPI::COMM_WORLD.Recv(received_signal, status.Get_count(MPI::CHAR), MPI::CHAR, status.Get_source(), 2);
				MPI::COMM_WORLD.Send(message_chars, message_size, MPI::CHAR, status.Get_source(), 1);
				elementsInBuffer--;
				//cout << "siunciama i darbini procesa " << message << endl;
			}
			else if(elementsInBuffer == 0){
				// nera ko issimti
				// tik priimti is pagrindinio			
				MPI::Status status;
				MPI::COMM_WORLD.Probe(0, 1, status);
				MPI::COMM_WORLD.Recv(received_message, status.Get_count(MPI::CHAR), MPI::CHAR, 0, 1);
				string received(&received_message[0], &received_message[status.Get_count(MPI::CHAR)]);
				numbers[elementsInBuffer] = received;
				elementsInBuffer++;
				received_items++;
			} else{			
				MPI::Status status;
				MPI::COMM_WORLD.Probe(0, 1, status);
				MPI::COMM_WORLD.Recv(received_message, status.Get_count(MPI::CHAR), MPI::CHAR, 0, 1);
				string received(&received_message[0], &received_message[status.Get_count(MPI::CHAR)]);
				received_items++;
				numbers[elementsInBuffer] = received;
				
				// ir priimti ir siusti
				string num = numbers[elementsInBuffer];
				numbers[elementsInBuffer] = "";
				
				auto send_length = num.length();
				const char* send_chars = num.c_str();
				
				MPI::Status statuss;
				char received_signal[1];
				MPI::COMM_WORLD.Probe(MPI::ANY_SOURCE, 2, statuss);
				MPI::COMM_WORLD.Recv(received_signal, statuss.Get_count(MPI::CHAR), MPI::CHAR, statuss.Get_source(), 2);
				MPI::COMM_WORLD.Send(send_chars,send_length, MPI::CHAR, statuss.Get_source(), 1);
				//cout << "siunciama i darbini procesa" << send << endl;
				
			}
		}
		while(elementsInBuffer != 0){
			string num = numbers[elementsInBuffer];
				numbers[elementsInBuffer] = "";
				
				auto send_length = num.length();
				const char* send_chars = num.c_str();
				
				MPI::Status statuss;
				char received_signal[1];
				MPI::COMM_WORLD.Probe(MPI::ANY_SOURCE, 2, statuss);
				MPI::COMM_WORLD.Recv(received_signal, statuss.Get_count(MPI::CHAR), MPI::CHAR, statuss.Get_source(), 2);
				MPI::COMM_WORLD.Send(send_chars,send_length, MPI::CHAR, statuss.Get_source(), 1);
				elementsInBuffer--;
		}
		// siusti baigimo signala
		char str[] = "a";
		char* end_signal = &str[0];
		MPI::COMM_WORLD.Send(&end_signal, 1, MPI::CHAR, 3, 1);
		MPI::COMM_WORLD.Send(&end_signal, 1, MPI::CHAR, 4, 1);
		MPI::COMM_WORLD.Send(&end_signal, 1, MPI::CHAR, 5, 1);
		cout << "DUOMENYS BAIGE SIUNTIMA  " << endl;
		
	// rezultatu procesas	
	}else if(rank == 2){
		int received_items = 0;
		int workers_signals = 0;
		vector<string> result_buffer;
		//string result_buffer[25];
		int count_results = 0;
		while(true){
			
			char buffer[60];
			
			MPI::Status status;
			MPI::COMM_WORLD.Probe(MPI::ANY_SOURCE, 3, status);
			MPI::COMM_WORLD.Recv(buffer, status.Get_count(MPI::CHAR), MPI::CHAR, status.Get_source(), 3);
			string message(&buffer[0], &buffer[status.Get_count(MPI::CHAR)]);
			if(message.length() == 1) workers_signals++;
			else result_buffer.push_back(message);
			if(workers_signals ==  MPI::COMM_WORLD.Get_size() - 3) break;
			received_items++;
				
			
		}
		cout << "REZULTATAI BAIGE SIUNTIMA  " << endl;
		for(int i = 0; i < result_buffer.size(); i++){
			string serialized = result_buffer[i];
			auto serialized_size = serialized.length();
			const char* serialized_chars = serialized.c_str();
			MPI::COMM_WORLD.Send(serialized_chars,serialized_size, MPI::CHAR, 0, 1);
		}
		char str[] = "a";
		char* end_signal = &str[0];
		MPI::COMM_WORLD.Send(&end_signal, 1, MPI::CHAR, 0, 1);
		
		
		
	// darbiniai procesai
	}else{
		// siuncia uzklausa trynimui
		
		while(true){
		
			char str[] = "a";
			char* request = &str[0];
			char buffer[60];
			MPI::COMM_WORLD.Send(&request, 1, MPI::CHAR, 1, 2);
			
			
			MPI::Status status;
			MPI::COMM_WORLD.Probe(1, 1, status);
			MPI::COMM_WORLD.Recv(&buffer, status.Get_count(MPI::CHAR), MPI::CHAR, 1, 1);
			string message(&buffer[0], &buffer[status.Get_count(MPI::CHAR)]);
			if(message.length() == 1){
				char str[] = "a";
				char* end_signal = &str[0];
				MPI::COMM_WORLD.Send(&end_signal, 1, MPI::CHAR, 2, 3);
				break;	
			}
			
			//cout << "Darbinis gauta " << message << endl;
			
			
			split(message, ',');
			auto brand = strings[0];
			auto quantity = strings[1];
			auto price = strings[2];			
			Plane plane(brand, stoi(quantity), stoi(price));
						
			plane.addResult(plane.CountFibonacci(plane.getQuantity()));			
			if (plane.getResult() >= 1000000) {
				// siunciama rezultatu procesui
				string serialized = makePlaneToString(plane);
				auto serialized_size = serialized.size();
				const char* serialized_chars = serialized.c_str();
				MPI::COMM_WORLD.Send(serialized_chars,serialized_size, MPI::CHAR, 2, 3);
			}
			
		}
		// siusti signala resultatams kad baigta	

			
			cout << "DARBINIAI BAIGE SIUNTIMA  " << endl;
	}
						
	
	
					
	MPI::Finalize();	
	
	
		//MPI::Status status;
		//MPI::COMM_WORLD.Probe(0, 1, status);
		//MPI::COMM_WORLD.Recv(&msgBuffer, status.Get_count(MPI::CHAR), MPI::CHAR, status.Get_source(), 1);

	//writeResultsToFile("rez.txt", resultMonitor);
	cout << "Program finished execution" << endl;



}




int findIndex(Plane plane, int count, Plane planes[])
{
	int condition = plane.getResult();
	int index = 0;
	for (int i = 0; i < count; i++) {
		if (condition < planes[i].getResult()) {
			return i;
		}
		index++;
	}
	return index;
}

void shiftArray(int index, int count, Plane planes[])
{
	for (int i = count; i > index; i--) {
		planes[i] = planes[i - 1];
	}
}


int length(const char* arr){
	return sizeof(arr)/sizeof(arr[0]);
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

string makePlaneToString(Plane plane){
	string line = "";
	line.append(plane.getBrand());
	line.append(",");
	line.append(to_string(plane.getQuantity()));
	line.append(",");
	line.append(to_string(plane.getPrice()));
	line.append(",");
	line.append(to_string(plane.getResult()));
	return line;

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

void writeResultsToFile(string fname, Plane planes[], int length) {
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
		for (int i = 0; i < length; i++) {
		if(planes[i].getResult() != 0){
			myFile << setw(30) << planes[i].getBrand() + " |" << setw(10) << to_string(planes[i].getQuantity()) + " |" << setw(20)
				<< to_string(planes[i].getPrice()) + " |" << setw(15) << to_string(planes[i].getResult()) + " |"
				<< endl;
		}
		}
		myFile.close();
	}
}






