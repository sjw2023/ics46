// main.cpp
//
// ICS 46 Spring 2017
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.
#include <iostream>
#include "RoadMapReader.hpp"
#include "RoadMapWriter.hpp"
#include "TripReader.hpp"
#include <map>

using namespace std;


std::function<double(const RoadSegment&)> fastestpath = [](RoadSegment rs){return rs.miles;};
std::function<double(const RoadSegment&)> fastesttime = [](RoadSegment rs){return rs.milesPerHour/ rs.miles;};

void outputTime(double t)
{
	int hr = 0, min = 0;
	float sec = 0;
	hr = t / 3600;
	min = ((t / 3600 - hr) * 60);
	sec = ((t/60 - min) * 60);

	if(min != 0 or hr != 0)
	{
		cout << min << " mins ";
	}
	if(hr != 0)
	{
		cout << hr << " hrs ";
	}

	printf("%2.2f secs", sec );
}

int main()
{
	struct vertex_information
	{
		int vertex;
		string name;
		RoadSegment info;
	};
	InputReader r  = InputReader(cin);
	RoadMapReader rm;
	RoadMap rmap = rm.readRoadMap(r);
	
	TripReader t;
	std::vector<Trip> trips = t.readTrips(r);

	map<int, int> info;
	for (std::vector<Trip>::iterator i = trips.begin(); i != trips.end(); i++)
	{
		vector<vertex_information> rTrip;
		if (i->metric == TripMetric::Time)
		{
			info = rmap.findShortestPaths(i->startVertex, fastesttime);
		}
		else
		{
			info = rmap.findShortestPaths(i->startVertex, fastestpath);
		}

		int next = i->endVertex;
		vertex_information end = vertex_information{i->endVertex, rmap.vertexInfo(i->endVertex), rmap.edgeInfo(info[i->endVertex], i->endVertex)};
		rTrip.push_back(end);

		float totaldistance = end.info.miles;
		double totalmph = 0;
		while(next != i->startVertex)
		{
			int temp = next;
			next = info[next];
			rTrip.push_back(vertex_information{next, rmap.vertexInfo(next), rmap.edgeInfo(next, temp)});
			if (next != i->startVertex)
			{
				totaldistance += rmap.edgeInfo(next, temp).miles;
			}
		}

		if (i->metric == TripMetric::Time)
		{
			cout << "Shortest driving time from " << rmap.vertexInfo(rTrip.back().vertex) <<
			 " to " << rmap.vertexInfo(rTrip.front().vertex) << endl;

			for(int i = rTrip.size() - 1; i >= 0; i--)
			{
				double m = rTrip[i].info.miles;
				double mph = rTrip[i].info.milesPerHour;
				double t = m/mph * 3600; //In seconds

				if (i != rTrip.size() - 1)
					cout << "\t Continue to ";	
				else
				{
					cout << "\t Begin at " <<  rTrip[i].name << endl;
					continue;
				}
					
				totalmph += t;
				cout << rTrip[i].name << " (" << m << " miles @ " << mph << " mph = ";
				outputTime(t);
				printf(")\n");
			}
			std::cout<<"Total time: "<<std::endl;
			outputTime(totalmph);
			std::cout<<"\n";
		}
		else
		{
			cout << "Shortest distance from " << rmap.vertexInfo(rTrip.back().vertex) <<
			 " to " << rmap.vertexInfo(rTrip.front().vertex) << endl;

			for(int i = rTrip.size() - 1; i >= 0; i--)
			{
				if (i != rTrip.size() - 1)
					cout << "\t Continue to "<< rTrip[i].name << " (" << rTrip[i].info.miles << " miles)" << endl;
				else
					cout << "\t Begin at "<< rTrip[i].name << endl;
					
			}
			printf("Total Distance: %2.1f miles\n", totaldistance);
		}
		std::cout<<"\n";
	}

    return 0;
}

