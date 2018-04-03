#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>

#include "FinalProject.h"

using namespace std; 

class Flight
{
	

public:
	Flight(void);
	Flight(int id, std::string depart_city, std::string destination, std::string flight_no, 
		std::string depart_time, std::string arrival_time, int total_seat, double fare, std::string plane_model);
	~Flight(void);
	int id;
	string depart_city;
	string destination;
	string flight_no; 
	string depart_time;
	string arrival_time; 
	int total_seat; 
	double fare; 
	string plane_model;
	
	vector<Flight> display(const char *query);
	Flight getFlight(int id);
	vector<Flight> Flight::getFlights();
};

