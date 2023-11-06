#include "Monitor.h"
#include "Plane.h"

Monitor::Monitor(int size)
{
	//this->planes = new Plane[size];
	this->planes[size];
	this->count = 0;
	this->capacity = size;
}

void Monitor::Add(Plane plane)
{
#pragma omp critical
{
	int index = findIndex(plane);
	shiftArray(index);
	planes[index] = plane;
	count++;
	}

}


int Monitor::findIndex(Plane plane)
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

void Monitor::shiftArray(int index)
{
	for (int i = count; i > index; i--) {
		planes[i] = planes[i - 1];
	}
}

int Monitor::getSize()
{
	return this->count;
}

int Monitor::getCapacity()
{
	return this->capacity;
}

Plane Monitor::getItem(int i)
{
	return planes[i];
}

