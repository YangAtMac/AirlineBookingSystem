#pragma once
#include "sqlite3.h"

class Database
{
public:
	Database(void);
	sqlite3 *openDb();
	int initDB();
	int insertFlight();
	void getFlights();
	void closeDb(sqlite3 *db);
	~Database(void);
};

