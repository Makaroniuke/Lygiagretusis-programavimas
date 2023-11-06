
#include <string>
#include "Plane.h"



Plane::Plane(string b, int q, float p)
{
	this->brand = b;
	this->quantity = q;
	this->price = p;
	this->result = 0;
}

string Plane::getBrand()
{
	return this->brand;
}

int Plane::getQuantity()
{
	return this->quantity;
}

float Plane::getPrice()
{
	return this->price;
}

int Plane::getResult()
{
	return this->result;
}

void Plane::addResult(int res)
{
	this->result = res;
}

int Plane::CountFibonacci(int quantity)
{
	if (quantity <= 1)
		return quantity;
	return CountFibonacci(quantity - 1) + CountFibonacci(quantity - 2);
}

Plane::Plane()
{
}

