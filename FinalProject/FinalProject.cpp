#include "stdafx.h"
#include <ios>
#include <iostream>
#include "sqlite3.h"
#include <string>
#include <conio.h>

#include "FinalProject.h"
#include "Flight.h"
#include "User.h"
#include "Database.h"
#include "Reservation.h"

#include <iomanip>

using namespace std;

void printMenu(void);
void printUserMenu(void);
void reserveFlight(int userId);
void displayUserReservation(int userId);
void displayReservation(void);
void cancalUserFlight(int userId);
void cancalFlight(void);
void printFlights(vector<Flight *> flights);
void printReservation(vector<UserReservation> uReservation);
bool loginOrCreate(User &user);
bool login(User &user);
bool createAccount(User &user);
void initDB();

Database db;

int _tmain(int argc, _TCHAR* argv[])
{

	User user;
	bool authenticated = loginOrCreate(user);
	int userId = user.GetId();

	int choic;
	Flight f;

	while (authenticated) {
		system("CLS");
		printMenu();
		cin >> choic;
		switch(choic){
			case 1:
				displayUserReservation(userId);
				break;
			case 2:
				reserveFlight(userId);
				break;
			case 3:
				cancalUserFlight(userId);
				break;
			case 4:
				printFlights(f.getAllFlights());
				break;
			case 6:
				break;
			case 7:
				//db.debug();
				break;
			case 9:
				exit(0);
		}
		cin.clear();
		cin.ignore();
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

void printUserMenu(void)
{
	cout << "\tUSER MENU\n" << endl;
	cout << endl;
	cout << "1. Login." << endl;
	cout << "2. Create account." << endl;
	cout << "3. Initialize database." << endl;
	cout << "9. Exit." << endl;
	cout << endl;
	cout << "Please select option: ";
}

void reserveFlight(int userId)
{
	Flight f;
	UserReservation *uR;
	int flight_id;
	uR = new UserReservation();
	User passenger = User::Find(userId);
	uR->userId = userId;
	vector<Flight*> flights = f.getAllFlights();
	printFlights(flights);

	cout << "Enter the flight id to reserve: ";

	cin >> flight_id;
	for (Flight *flight : flights) {
		if(flight->GetId() == flight_id) {
			uR->flight = flight;
			break;
		}
	}
	passenger.reserveFlight(uR);
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
	int resId;
	do {
		system("CLS");
		printReservation(uReservations);
		cout << "Enter 'q' back to main meun " << endl;
		cout << "Enter reservation ID to show seat map: " ;
		cin.clear();
		cin >> resId;
		if (cin.fail()){
			cout << "\nInput incorrect ID back to main menu." << endl;
			break;
		} else if ( isValidId(uReservations, resId)){
			for ( UserReservation r : uReservations) {
				if ( r.reserve_id == resId) {
					user.dispalyUserSeat(r);
					break;
				} 
			} 
		} else {
			cout << endl << "No matched reservation found!" << endl;
		}
		system("PAUSE");
	} while ( !cin.fail() );

}

void printFlights(vector<Flight *> flights)
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
		for (Flight * flight : flights) {
			cout << setw(3) << right << "[" << flight->GetId() << "]";
			cout << setw(16) << right << flight->GetDepartCity();
			cout << setw(16) << right << flight->GetDestination();
			cout << setw(14) << right << flight->GetFlightNo();
			cout << setw(16) << right << flight->GetDepartTime();
			cout << setw(17) << right << flight->GetArrivalTime();
			cout << setw(14) << right << flight->GetTotalSeat();
			cout << setw(8) << right << flight->GetFare();
			cout << setw(6) << right << flight->GetPlaneModel() << endl;
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

void viewSeatMap()
{

}

bool loginOrCreate(User &user)
{
	int choice = 0;
	system("CLS");
	printUserMenu();
	cin >> choice;
	switch(choice){
			case 1:
				return login(user);
				break;
			case 2:
				return createAccount(user);
				break;
			case 3:
				initDB();
				break;
			case 9:
				exit(0);
		}
}

bool login(User &user)
{
	string username;
	string password;
	bool authenticated;
	cout << "Please enter your username: ";
	cin >> username;
	user = User::FindByUsername(username);
	if(username.compare(user.GetUsername()) != 0)
	{
		cout << "User not found." << endl;
		return false;
	}
	int count = 0;
	do{
		int ch;
		cout << "Please enter your password: ";
		/*cin >> password;*/
		while ( ch = _getch()){
			if ( ch == 13){
				break;
			} else if (ch == 8) {
				if (password.length() > 0){
					cout << "\b \b";
					password.erase( password.length() - 1);
				}
			} else {
				cout << "*";
				password += ch;
			}
		}

		user.SetPassword(password);
		authenticated = user.Authenticate();
		password = "";
		count++;
		if (count == 5){
			cout << "Too many attemption, exit."<<endl;
			system("PAUSE");
			exit(0);
		}
		if(!authenticated)
		{
			cout << "tried " << count <<", password is not correct." << endl;
		}
	} while (!authenticated);
	return authenticated;
	return authenticated;
}

bool createAccount(User &user)
{
	string username;
	string password;
	string role = "user";
	cout << "Please enter your username: ";
	cin >> username;
	cout << "Please enter your password: ";
	int ch;
	while ( ch = _getch()){
		if ( ch == 13){
			break;
		} else if (ch == 8) {
			if (password.length() > 0){
					cout << "\b \b";
					password.erase( password.length() - 1);
			}
		} else {
			cout << "*";
			password += ch;
		}
	}
	user = User(username, password, role);
	user.Create();
	return true;
}

void initDB(){
	char choice = 'n';
	cout<< "\nWARNING: This will delete all exist data in database, you will have to "
		"recrate user/password and reservation" << endl;
	cout << "Would you like to initialize the database? (y/n): ";
	cin >> choice;
	if(choice == 'y')
	{
		Database::initDB();
		Database::insertFlight();
	} else {
		exit(0);
	}

}
