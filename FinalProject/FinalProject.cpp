#include "stdafx.h"
#include <ios>
#include <iostream>
#include "sqlite3.h"
#include <string>

#include "FinalProject.h"
#include "Flight.h"
#include "User.h"
#include "Database.h"
#include "Reservation.h"

#include <iomanip>

using namespace std;

void printMenu(void);
void reserveFlight(void);
void displayUserReservation(int userId);
void displayReservation(void);
void cancalUserFlight(int userId);
void cancalFlight(void);
void printFlights(vector<Flight> flights);
void printReservation(vector<UserReservation> uReservation);
bool login(User &user);

Database db;

int _tmain(int argc, _TCHAR* argv[])
{
	Database::initDB();
	Database::insertFlight();
	User user;
	bool authenticated = login(user);

	cin.ignore();
	cin.get();

	int choic;
	
	while (authenticated) {
		system("CLS");
		printMenu();
		cin >> choic;
		switch(choic){
			case 1:
				displayReservation();
				break;
			case 2:
				reserveFlight();
				break;
			case 3:
				cancalFlight();
				break;
			case 4: 
				printFlights(Flight::All());
				break;
			case 6:
				cout << "construction.....";
				//flights.getFlights();
				break;
			case 7:
				db.debug();
				break;
			case 9:
			default: 
				exit(0);
		}
		system("PAUSE");
	}

	cout << "Please press any key to exit the program ..." << endl;
	cin.ignore();
	cin.get();

	return 0;
}

void printMenu(void)
{
	cout << "\tAIRLINE RESERVATION SYSTEM\n" << endl;
	cout << endl;
	cout << "1. View My Flights." << endl;
	cout << "2. Reserve your flight." << endl;
	cout << "3. Cancel your flights." << endl;
	cout << "4. Display all flights." << endl;
	cout << "9. Exit." << endl;
	cout << endl;
	cout << "Please select option: ";
}

void reserveFlight(void)
{
	User passenger = User::Find(2);
	Flight f;
	printFlights(Flight::All());

	cout << "Enter the flight id to reserve: ";
	int flight_id;
	cin >> flight_id;
	//f = f.getFlight(flight_id);
	passenger.reserveFlight(flight_id, 2, "11B");
//	passenger.reserveSeat(f);
}

void cancalFlight()
{
	unsigned int userId;
	do{
		cout << "Please enter your USER ID to login: ";
		//cout << "Please enter your password: ";
		cin >> userId;
	}while(userId < 0); // validate if it's exists

	cancalUserFlight(userId);
}

bool isValidId(vector<Flight> flights, int id)
{
	bool isValid = false;
	for (Flight f:flights){
		if (f.GetId() == id)
			isValid = true;
	}
	return isValid;
}

bool isValidId(vector<UserReservation> rEservations, int id)
{
	bool isValid = false;
	for (UserReservation r:rEservations){
		if (r.reserve_id == id)
			isValid = true;
	}
	return isValid;
}

void cancalUserFlight(int userId)
{
	User user = User::Find(userId);
	vector<UserReservation> uReservations;
	uReservations = user.GetUserReservation(userId);
	printReservation(uReservations);
	cout << "Your have [" << uReservations.size() << "] reservation." << endl;
	unsigned int selectId;
	do{
		cout << "Please enter flight ID you to Cancel: ";
		cin >> selectId;
	}while(!isValidId(uReservations, selectId));
	db.cancelReservation(selectId);
}

void displayReservation(void)
{
	unsigned int userId;
	do{
		cout << "Please enter your USER ID: ";
		cin >> userId;
	}while(userId < 0); // TODO: validate if User ID is exists
	displayUserReservation(userId);
}

void displayUserReservation(int userId)
{
	User user = User::Find(userId);
	vector<UserReservation> uReservations;
	uReservations = user.GetUserReservation(userId);
	printReservation(uReservations);
}

void printFlights(vector<Flight> flights)
{
	if (flights.size() > 0) {
	cout << setw(5) << right << "ID";
	cout << setw(16) << right << "Depart City";
	cout << setw(16) << right << "Destination";
	cout << setw(14) << right << "Flight No";
	cout << setw(16) << right << "Depart Time";
	cout << setw(17) << right << "Arrival Time";
	cout << setw(14) << right << "Num Seats";
	cout << setw(8) << right << "Fare";
	cout << setw(6) << right << "Plane" << endl;
	for(Flight flight : flights)
	{
		cout << setw(3) << right << "[" << flight.GetId() << "]";
		cout << setw(16) << right << flight.GetDepartCity();
		cout << setw(16) << right << flight.GetDestination();
		cout << setw(14) << right << flight.GetFlightNo();
		cout << setw(16) << right << flight.GetDepartTime();
		cout << setw(17) << right << flight.GetArrivalTime();
		cout << setw(14) << right << flight.GetTotalSeat();
		cout << setw(8) << right << flight.GetFare();
		cout << setw(6) << right << flight.GetPlaneModel() << endl;
		}
	} else {
		cout << "\nNo data found." << endl;
	}
}

void printReservation(vector<UserReservation> uReservations)
{
	if (uReservations.size() > 0) {
	cout << setw(7) << right << "ID";
	cout << setw(19) << right << "Seats Number";
	cout << setw(15) << right << "Flight ID";
	cout << setw(16) << right << "Depart City";
	cout << setw(16) << right << "Destination";
	cout << setw(14) << right << "Flight No";
	cout << setw(16) << right << "Depart Time";
	cout << setw(17) << right << "Arrival Time" << endl;
	for(UserReservation r : uReservations)
	{
		cout << setw(6) << right << "[" << r.reserve_id << "]";
		cout << setw(14) << right << r.seat_num;
		cout << setw(14) << right << r.flight->GetId();
		cout << setw(16) << right << r.flight->GetDepartCity();
		cout << setw(16) << right << r.flight->GetDestination();
		cout << setw(17) << right << r.flight->GetFlightNo();
		cout << setw(14) << right << r.flight->GetDepartTime();
		cout << setw(17) << right << r.flight->GetArrivalTime() << endl;
		}
	} else {
		cout << "\nNo data found." << endl;
	}
}

bool login(User &user)
{
	string username;
	string password;
	cout << "Please enter your username: ";
	cin >> username;
	user = User::FindByUsername(username);
	if(username.compare(user.GetUsername()) != 0)
	{
		cout << "User not found." << endl;
		return false;
	}
	cout << "Please enter your password: ";
	cin >> password;

	user.SetPassword(password);
	bool authenticated = user.Authenticate();
	if(!authenticated)
	{
		cout << "Password is not correct." << endl;
	}
}