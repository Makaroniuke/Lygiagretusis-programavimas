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
	string getBrand();
	int getQuantity();
	float getPrice();
	int getResult();

	void addResult(int res);
	int CountFibonacci(int quantity);

	Plane();

};

