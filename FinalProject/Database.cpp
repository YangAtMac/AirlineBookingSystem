#include "stdafx.h"
#include <iostream>
#include "Database.h"
#include <string>
#include "sha256.h"


using namespace std;

Database::Database(void)
{

}

sqlite3* Database::openDb()
{
	sqlite3 *db;
	// Open Database
	//cout << "Opening FinalProject.db ..." << endl;
	int rc = 0;
	rc = sqlite3_open("FinalProject.db", &db);
	if (rc)
	{
		cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl << endl;
		closeDb(db);
	}
	else
	{
		//cout << "Opened FinalProject.db." << endl << endl;
	}
	return db;
}

void Database::closeDb(sqlite3 *db)
{
	sqlite3_close(db);
}

int Database::initDB()
{
	int rc;
	char *error;
	sqlite3 *db = openDb();

	// Execute SQL
	cout << "Creating USERS Table ..." << endl;
	const char *sqlDropUserTable = "DROP TABLE IF EXISTS USERS;";
	rc = sqlite3_exec(db, sqlDropUserTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}

	const char *sqlCreateUserTable = "CREATE TABLE IF NOT EXISTS USERS (id INTEGER PRIMARY KEY, username STRING, hashed_password STRING, role STRING);";
	rc = sqlite3_exec(db, sqlCreateUserTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Created USERS." << endl << endl;
	}

	cout << "Creating FLIGHTS ..." << endl;
	const char *sqlDropFlightTable = "DROP TABLE IF EXISTS FLIGHTS;";
	rc = sqlite3_exec(db, sqlDropFlightTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	const char *sqlCreateFlightsTable = "CREATE TABLE IF NOT EXISTS FLIGHTS ( "
		"id INTEGER PRIMARY KEY NOT NULL, depart_city STRING, destination STRING, flight_no STRING,"
		"depart_time DATETIME, arrival_time DATETIME, total_seat SMALLINT, fare DOUBLE, plane_model STRING);";
	rc = sqlite3_exec(db, sqlCreateFlightsTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Created FLIGHTS." << endl << endl;
	}

	cout << "Creating FLIGHT_USER ..." << endl;
	const char *sqlDropFlightUserTable = "DROP TABLE IF EXISTS FLIGHT_USER;";
	rc = sqlite3_exec(db, sqlDropFlightUserTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	const char *sqlCreateFlightUserTable = "CREATE TABLE IF NOT EXISTS FLIGHT_USER (id INTEGER PRIMARY KEY, seat_no STRING, user_id INTEGER, flight_id INTEGER);";
	rc = sqlite3_exec(db, sqlCreateFlightUserTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Created FLIGHT_USER." << endl << endl;
	}

	cout << "Inserting a value into User ..." << endl;
	string hashed_password = sha256("password");
	string sql = "INSERT INTO Users (username, hashed_password, role) VALUES('bob', '" + hashed_password + "', 'user');";
	const char *cstr = sql.c_str();
	rc = sqlite3_exec(db, cstr, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Inserted a value into User." << endl << endl;
	}
	closeDb(db);
	return 1;
}

int Database::insertFlight()
{
	char *error;
	int rc = 0;
	sqlite3 *db = openDb();
	sqlite3_open("FinalProject.db", &db);
	const char *sqlInsertFlight =
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('ATL', 'YUL', 'AC015', '12:35', '15:45', '270', '650.00', 'A320');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YUL', 'ATL', 'AC016', '16:50', '19:55', '270', '550.00', 'A320');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('AUS', 'YYZ', 'AC018', '15:35', '23:45', '350', '1550.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YYZ', 'AUS', 'AC019', '12:50', '20:55', '350', '1550.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('ANC', 'YYZ', 'AC020', '14:35', '19:45', '350', '1650.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YYZ', 'ANC', 'AC021', '12:50', '17:55', '350', '1650.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('ORD', 'YUL', 'AC221', '11:35', '14:45', '270', '2650.00', 'B787');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YUL', 'ORD', 'AC222', '15:50', '18:55', '270', '2650.00', 'B787');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('AUS', 'CVG', 'MU056', '15:35', '23:45', '350', '1350.00', 'B777');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('CVG', 'AUS', 'MU057', '13:50', '21:55', '350', '1350.00', 'B777');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('ANC', 'DEN', 'CE067', '15:35', '20:45', '350', '1250.00', 'B747');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('DEN', 'ANC', 'CE068', '13:55', '18:59', '350', '1250.00', 'B747');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('CLE', 'DFW', 'AC025', '11:45', '15:45', '270', '3650.00', 'A320');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('DFW', 'CLE', 'AC026', '15:20', '19:55', '270', '3650.00', 'A320');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('FLL', 'YYZ', 'NY038', '13:15', '23:45', '350', '1150.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YYZ', 'FLL', 'NY039', '15:50', '20:55', '350', '1150.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YVR', 'YYZ', 'AC120', '16:45', '19:45', '350', '1650.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YYZ', 'YVR', 'AC121', '13:20', '17:55', '350', '1650.00', 'A380');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('FLL', 'YUL', 'FL221', '12:25', '14:45', '270', '2150.00', 'B767');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YUL', 'FLL', 'FL222', '15:45', '18:55', '270', '2150.00', 'B767');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('HNL', 'CVG', 'DE056', '14:35', '23:45', '350', '2350.00', 'B777');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('CVG', 'HNL', 'DE057', '15:40', '21:55', '350', '2350.00', 'B777');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('JFK', 'YYZ', 'MU167', '11:15', '20:45', '350', '2250.00', 'B767');"
		"INSERT INTO FLIGHTS (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) values ('YYZ', 'JFK', 'MU168', '10:25', '18:59', '350', '2250.00', 'B767');";
	rc = sqlite3_exec(db, sqlInsertFlight, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error fail to insert flight info: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Inserted flight info into FLIGHT table." << endl << endl;
	}
	closeDb(db);
	return rc;
}

void Database::getFlights()
{
    sqlite3_stmt *statement;
	sqlite3 *db = openDb();

    char *query = "select * from FLIGHTS";

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        while ( true )
        {
            res = sqlite3_step(statement);
            if ( res == SQLITE_ROW )
            {
				int id;
                id = sqlite3_column_int(statement, 0);
				cout << id << ": ";
				for ( int i = 1; i < ctotal; i++ )
                {
                    string s = (char*)sqlite3_column_text(statement, i);
                    // print or format the output as you want
                    cout << s << " " ;
                }
                cout << endl;
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                cout << "done " << endl;
                break;
            }
        }
    }
}

/* this is debug function to modify DB*/
//void Database::debug()
//{
//	string sqlDebug = "INSERT INTO RESERVATION (seat_no, user_id, flight_id) VALUES('12B', 2, 12)";
//	/*
//	string sqlDebug  = "ALTER TABLE FLIGHT_USER RENAME TO RESERVATION";
//	string sqlDebug  = "ALTER TABLE FLIGHT_USER RENAME TO RESERVATION";
//	*/
//	sqlCommand(sqlDebug);
//}


//void Database::saveReservation( Reservation reservation)
//{
//
//	string sqlSave = "INSERT INTO RESERVATION (seat_no, user_id, flight_id) VALUES('" +
//		reservation.seat_num + "'," + to_string(reservation.user_id) + ", " + to_string(reservation.flight_id) + ");";
//	cout  << "------> sql command: " << sqlSave << endl;
//	sqlCommand(sqlSave);
//}

void Database::saveReservation( UserReservation *uReservation)
{

	string sqlSave = "INSERT INTO RESERVATION (seat_no, user_id, flight_id) VALUES('" +
		uReservation->seat_num + "'," + to_string(uReservation->userId) + ", " + to_string(uReservation->flight->GetId()) + ");";
	//cout  << "------> sql command: " << sqlSave << endl;
	sqlCommand(sqlSave);
}

void Database::cancelReservation(int reservationId)
{
	string sqlRemove = "DELETE FROM RESERVATION WHERE reserve_id='" + to_string(reservationId)  + "';";
	sqlCommand(sqlRemove);
}

void Database::sqlCommand( string sqlQuery)
{
	int rc;
	char *error;
	sqlite3 *db = Database::openDb();
	const char *sqlQueryStr = sqlQuery.c_str();
	rc = sqlite3_exec(db, sqlQueryStr, NULL, NULL, &error);
   if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
	} else {
		fprintf(stdout, "SQL command executed successfully\n");
	}
   Database::closeDb(db);
}

Database::~Database(void)
{
}
