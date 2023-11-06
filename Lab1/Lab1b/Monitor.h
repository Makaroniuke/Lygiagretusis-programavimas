



#include "Plane.h"
using namespace std;

class Monitor {
private:
	int count;
	int capacity;
	Plane planes[30];

public:
	Monitor(int size);
	void Add(Plane plane);
	//Plane Remove();
	int findIndex(Plane plane);
	void shiftArray(int index);

	int getSize();
	int getCapacity();
	Plane getItem(int i);
};

