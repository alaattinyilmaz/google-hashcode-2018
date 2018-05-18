#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;


struct ride {
	int rideid;
	int x,y,z,m,d,e,f,instant;
	bool isvisited;
	ride(int rideid,int x, int y, int z, int m, int d, int e, int f, int instant):rideid(rideid),x(x),y(y),z(z),m(m),d(d),e(e),f(f),instant(instant),isvisited(false){}
};

struct car {
	int carid,instant,x,y;
	vector <int> rides;
	car(int carid,int x, int y, int instant):carid(carid),x(x),y(y),instant(instant){}
};

struct valuecomp
{
	int carid, rideid, dur;
	double value;
	valuecomp(int carid, int rideid, double value, int dur):carid(carid),rideid(rideid),value(value),dur(dur){}
};

vector <ride> rides;
vector <car> cars;
vector <car> active_cars;

int takeMax(vector<valuecomp> & values)
{
	int max = values[0].value;
	int index = 0;
	for (int i= 0; i < values.size(); i++)
	{
		if (values[i].value > max)
		{
			max = values[i].value;
			index = i;
		}
	}

	return index;
}

int rideNum, ridecheck, bonus, points;

void rideAssign()
{

	vector <valuecomp> values;
	int d0, d, e, dur, t0, f, carid, rideid, xpos, ypos;

	double val;
	int maxindex = 0;
	int ridesize = rides.size();
	int passivecheck;
	valuecomp * newvalue;
	// bool passive_car = false;

	for (int i = 0; i < cars.size(); i++)
	{
		t0 = cars[i].instant;
		passivecheck = t0;

		for (int j = 0; j < ridesize; j++)
		{
			if (rides[j].isvisited == false)
			{

				d0 = abs(rides[j].x - cars[i].x) +  abs(rides[j].y - cars[i].y); // distance to rider
				d = rides[j].d; // distance of ride
				e = rides[j].e;
				f = rides[j].f;

				//val = (double) d / (d0 + (t0 + d0 - e) + d); // We want to maximize this value

				if ((d0+t0) >= e && (t0 + d0 + d) <= f)
				{		
					dur = d0 + d; // time to pass
					newvalue = new valuecomp(i,j,0,dur);
					values.push_back(*newvalue);
					break;
				}

				else if ((t0 + d0 < e) && (t0 + d0 + abs(t0 + d0 - e) + d) <= f) // erken gitmiþ
				{
					dur = d0 + abs(t0 + d0 - e) + d; // It is waiting this time to pass
					newvalue = new valuecomp(i,j,0,dur);
					values.push_back(*newvalue);
					break;
				}

			}
		}

		if (passivecheck == 0)
		{ break; }

	}	

	if (values.size() != 0)
	{

		maxindex = 0;		
		carid = values[maxindex].carid;
		rideid = values[maxindex].rideid;
		dur = values[maxindex].dur;

		xpos = rides[rideid].z;
		ypos = rides[rideid].m;
		rides[rideid].isvisited = true; // This ride is done

		cars[carid].instant = cars[carid].instant + dur; // Time instant of car changed
		cars[carid].x = xpos; // Car moved to x pos
		cars[carid].y = ypos; // Car moved to y pos


		// Points calculation

		if (cars[carid].instant == rides[rideid].e)
		{
			points = points + bonus;
			// cout << "BONUS" << endl;
		}

		d = abs(rides[rideid].z - rides[rideid].x) + abs(rides[rideid].m - rides[rideid].y);
		points = points + d; // distance of ride

		//	cout << "rideid: " <<rideid << " assigned to car"<< carid << endl;
		cars[carid].rides.push_back(rideid); // Adding ride to this car

		ridecheck++;

	}
}

int main ()
{

	clock_t tStart = clock();

	ifstream input;
	input.open("input.txt");

	string line;
	int r,c,v, steps, x,y,z,m,d,e,f;

	input >> r >> c >> v >> rideNum >> e >> f;

	for (int i = 0; i < v; i++)
	{
		car newcar(i, 0, 0, 0); // carid, x,y, instant
		cars.push_back(newcar);
	}

	// vector <ride> rides;
	int rideid = 0;
	ridecheck = 0;

	int UNTIL_HERE = 6000;

	while(rideid < UNTIL_HERE)
	{
		input >> x >> y >> z >> m >> e >> f; 
		d = abs(z - x) + abs(m - y);
		ride newride(rideid, x,y,z,m,d,e,f,0); // 0 -> instant
		rides.push_back(newride);
		rideid++;
	}
	rides.pop_back();
	rideid--;

	for (int k = 0; k < rideNum; k++)
	{
		//	cout<< k << ". Ride is assigning" << endl;
		rideAssign();
	}

	ofstream output;
	int ridesize;

	output.open("output.txt");
	for (int i = 0; i < cars.size(); i++)
	{
		ridesize = cars[i].rides.size();

		output << ridesize << " ";
		cout << ridesize << " ";

		for(int j = 0; j < cars[i].rides.size(); j++)
		{ 
			output << cars[i].rides[j] << " ";
			cout << cars[i].rides[j] << " ";
		}
		output << endl;
		cout << endl;
	}
	/*
	for (int i = 0; i < 5000 - cars.size(); i++)
	{

	output << 0 << " ";
	//	cout << 0 << " ";
	output << endl;
	//	cout << endl;
	}
	*/

	cout << endl << endl << "RIDES DONE: " << ridecheck << "/" << rideNum << " %" << (int)(((double)ridecheck/rideNum)*100) << endl;
	cout << "Points: " << points << endl;	
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


}