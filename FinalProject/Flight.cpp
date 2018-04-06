#include "stdafx.h"
#include <iostream>

#include "Flight.h"
#include "Database.h"

using namespace std;


Flight::Flight(void)
{
}

Flight::Flight(int id, string depart_city, string destination, string flight_no, 
		string depart_time, string arrival_time, int total_seat, double fare, 
		string plane_model)
{
	id = id;
	depart_city = depart_city;
	destination = destination;
	flight_no = flight_no; 
	depart_time = depart_time;
	arrival_time = arrival_time; 
	total_seat = total_seat; 
	fare = fare; 
	plane_model = plane_model;
}

Flight::~Flight(void)
{
}

vector<Flight> Flight::display(const char *query)
{
	Database DB;
	sqlite3 *db = DB.openDb();
	char *zErrMsg = 0;
	sqlite3_stmt *statement;    

	Flight flight;
	std::vector<Flight> flights;
	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK ) 
    {
        int colums = sqlite3_column_count(statement);
        int res = 0;

        while ( true )
        {
            res = sqlite3_step(statement);
            if ( res == SQLITE_ROW ) 
            {
				int id;
                id = sqlite3_column_int(statement, 0);
				//string colName = sqlite3_column_name(statement, 0);
				//cout << colName << endl;
				cout << endl;
				cout << "---------------------------------------------------------------"<<endl;
				cout << "[" << id << "]: ";
				flight.id = id;
				for ( int i = 1; i < colums; i++ ) 
                {
                    string ColName = sqlite3_column_name(statement, i);
					if (ColName == "depart_city") {
						string s = (char*)sqlite3_column_text(statement, i);
						flight.depart_city = s;
					} else if ( ColName == "destination" ) {
						string s = (char*)sqlite3_column_text(statement, i);
						flight.destination = s;
					} else if ( ColName == "flight_no" ) {
						string s = (char*)sqlite3_column_text(statement, i);
						flight.flight_no = s;
					} else if ( ColName == "depart_time" ) {
						string s = (char*)sqlite3_column_text(statement, i);
						flight.depart_time = s;
					} else if ( ColName == "arrival_time" ) {
						string s = (char*)sqlite3_column_text(statement, i);
						flight.arrival_time = s;
					} else if ( ColName == "total_seat" ) {
						int s = sqlite3_column_int(statement, i);
						flight.total_seat = s;
					} else if ( ColName == "fare" ) {
						double s = sqlite3_column_double(statement, i);
						flight.fare = s;
					} else if ( ColName == "plane_model" ) {
						string s = (char*)sqlite3_column_text(statement, i);
						flight.plane_model = s;
					}
					
					string d = (char*)sqlite3_column_text(statement, i);
                    // print or format the output as you want 
                    cout << d << " | " ;
                }
				flights.push_back(flight);
				cout << "\n---------------------------------------------------------------";
				//cout << endl;
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)    
            {
                cout << "\ndone " << endl;
                break;
            }
			
        }
    }
	sqlite3_close(db);
	
	return flights;
}


Flight Flight::getFlight(int id)
{
    Flight f;
	string str = "select * from FLIGHTS WHERE id=" + to_string(id) + ";";
	const char *query = str.c_str();
	vector<Flight> flights = display(query);
	if (flights.size() == 0) {
		cerr << "Could not fine flight ID: " << id;
	} else {
		f = flights.front();
	}

	/*for(vector<Flight>::iterator it = flights.begin(); it != flights.end(); ++it){
		cout << " " << it->id << " " << it->depart_city << " " << it->destination << " "
			<< it->depart_time << " " << it->arrival_time << " " << it->flight_no << endl;
	}
*/
	/*cout << "getFlights.id: " << f.id << endl;
	cout << "getFlight.depart_city: " << f.depart_city << endl;
	cout << "getFlight.destination: " << f.destination << endl;
	cout << "getFlights.fare: " << f.fare << endl;
	cout << "getFlights.flight_no: " << f.flight_no << endl;
	cout << "getFlights.plane_model: " << f.plane_model << endl;
*/
	return f;
}

vector<Flight> Flight::getFlights()
{
    char *query = "select * from FLIGHTS;";
	vector<Flight> flights = display(query);

	/*for(vector<Flight>::iterator it = flights.begin(); it != flights.end(); ++it){
		cout << " " << it->id << " " << it->depart_city << " " << it->destination << " "
			<< it->depart_time << " " << it->arrival_time << " " << it->flight_no << endl;
	}*/

	return flights;
}
