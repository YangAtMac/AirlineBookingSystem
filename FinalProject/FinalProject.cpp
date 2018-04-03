#include "stdafx.h"
#include <ios>
#include <iostream>
#include "sqlite3.h"
#include <string>

#include "User.h"
#include "Database.h"
#include "FinalProject.h"
#include "Flight.h"
 
using namespace std;

void printMenu();
void bookFlight();

int _tmain(int argc, _TCHAR* argv[])
{
	int choic;
	Database db;
	//db.initDB();
	//db.insertFlight();
	Flight flights;
	while (true) {
		system("CLS");
		printMenu();
		cin >> choic;
		switch(choic){
		case 1:
			bookFlight();
			break;
		case 4:
			//Flight::All();
			break;
		case 6: 

			flights.getFlights();
			break;
		case 9:
			exit(0);
		}
		system("PAUSE");
	}

	//vector<User> users = User::All();
	// cout << users[0].GetId() << endl;
	//vector<Flight> flights = Flight::All();
 
	cout << "Please press any key to exit the program ..." << endl;
	cin.get();
	
	return 0;
}

void printMenu()
{
	cout << "\tAIRLINE RESERVATION SYSTEM\n" << endl;
	cout << endl;
	cout << "1. Book a ticket." << endl;
	cout << "2. Reserve passenger's seat." << endl;
	cout << "3. Cancel a ticket." << endl;
	cout << "4. Dispaly all flights." << endl;
	cout << "5. Dispaly a flight seat." << endl;
	cout << "6. Dispaly a flights." << endl;
	cout << "9. Exit." << endl;
	cout << endl;
	cout << "Please select option: ";
}

void bookFlight()
{
	Flight *flight, f;
	//int selectId;
	//flight->getFlights();
	User passenger;
	passenger.reserveFlight();
}
