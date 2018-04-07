#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>

#include "Seat.h"
#include "User.h"
#include "Reservation.h"
#include "Database.h"


class User;

class Flight
{
private:
	int id;
	std::string depart_city;
	std::string destination;
	std::string flight_no;
	std::string depart_time;
	std::string arrival_time;
	int total_seat;
	double fare;
	std::string plane_model;
	int MAX_SEAT_PER_ROW;

public:
	Flight(void);
	Flight(int i, std::string depart_city, std::string destination, std::string flight_no,
		std::string depart_time, std::string arrival_time, int total_seat, double fare, std::string plane_model);
	Flight(std::string depart_city, std::string destination, std::string flight_no,
		std::string depart_time, std::string arrival_time, int total_seat, double fare, std::string plane_model);
	~Flight(void);
	void SetId( int Id){id = Id;};
	static Flight Find(int id);
	static std::vector<Flight> All();
	std::vector<Flight *> Flight::getAllFlights();
	void Create();
	void Update();
	void Delete();
	vector<User> GetUsers();
	int GetId();
	std::string GetDepartCity();
	void SetDepartCity(std::string d);
	std::string GetDestination();
	void SetDestination(std::string d);
	std::string GetFlightNo();
	void SetFlightNo(std::string f);
	std::string GetDepartTime();
	void SetDepartTime(std::string d);
	std::string GetArrivalTime();
	void SetArrivalTime(std::string a);
	Flight getFlight(int id);
	int GetTotalSeat();
	void SetTotalSeat(int i) {total_seat = i;};
	void SetFare(double i) {fare = i;};
	void SetPlaneModel(std::string model) { plane_model = model; };
	double GetFare();
	std::string GetPlaneModel();
	std::vector<Seat> getAllSeats();
};
