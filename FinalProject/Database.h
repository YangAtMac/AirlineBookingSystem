#pragma once
#include "sqlite3.h"
#include <string>

//#include "FinalProject.h"
#include "Reservation.h"

class Database
{
public:
	Database(void);
	static sqlite3 *openDb();
	static int initDB();
	static int insertFlight();
	void getFlights();
	static void closeDb(sqlite3 *db);
	void sqlCommand( std::string sqlQuery);
	void saveReservation( Reservation reservation);
	void cancelReservation( int id);
	void debug();
	~Database(void);
};

