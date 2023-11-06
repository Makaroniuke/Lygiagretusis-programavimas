
#pragma once


#include <string>

using namespace std;


class Plane {
private:
	string brand;
	int quantity;
	float price;
	int result;

public:
	Plane(string b, int q, float p);
	Plane(string b, int q, float p, int result);
	string getBrand();
	int getQuantity();
	string to_json();
	Plane from_json(const string& json_string);
	float getPrice();
	int getResult();

	void addResult(int res);
	int CountFibonacci(int quantity);

	Plane();

};

