#include "stdafx.h"
#include "Flight.h"
#include "User.h"

vector<Flight> Select(string sql);
static int selectCallback(void *ptr, int count, char **data, char **columns);
static int userSelectCallback(void *ptr, int count, char **data, char **columns);

Flight::Flight(int i, string dc, string d, string fn,
		string dt, string at, int ts, double f, string pm)
{
	id = i;
	depart_city = dc;
	destination = d;
	flight_no = fn;
	depart_time = dt;
	arrival_time = at;
	total_seat = ts;
	fare = f;
	plane_model = pm;
	MAX_SEAT_PER_ROW = 7;
	getAllSeats();
};
Flight::Flight(string dc, string d, string fn,
		string dt, string at, int ts, double f, string pm)
{
	depart_city = dc;
	destination = d;
	flight_no = fn;
	depart_time = dt;
	arrival_time = at;
	total_seat = ts;
	fare = f;
	plane_model = pm;
	MAX_SEAT_PER_ROW = 7;
	getAllSeats();
};
Flight::Flight(void)
{
	MAX_SEAT_PER_ROW = 7;
	getAllSeats();
}


Flight::~Flight(void)
{
}

Flight Flight::Find(int id)
{
	string sql = "SELECT * from Flights WHERE id=" + to_string(id) + ";";
	vector<Flight> flights = Select(sql);
	if(flights.size() == 0)
	{
		Flight flight;
		return flight;
	}
	Flight flight = Flight();
	return flight;
};
std::vector<Flight> Flight::All()
{
	string sql = "SELECT * from Flights;";
	std::vector<Flight> flights = Select(sql);
	return flights;
};

void Flight::Create()
{
	int rc;
	char *error;
	sqlite3 *db = Database::openDb();
	string sql = "INSERT INTO Flights (depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model) VALUES('" + depart_city + "','" + destination + "','" + flight_no + "','" + depart_time + "','" + arrival_time + "','" + to_string(total_seat) + "','" + to_string(fare) + "','" + plane_model + "');";
	const char *sqlInsert = sql.c_str();
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
   if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
		Database::closeDb(db);
	} else {
		fprintf(stdout, "Flight created successfully\n");
		Database::closeDb(db);
		string sql = "SELECT * from Flights WHERE flight_no='" + flight_no + "';";
		vector<Flight> flights = Select(sql);
		id = flights[0].id;
	}
};
void Flight::Update()
{
	int rc;
	char *error;
	sqlite3 *db = Database::openDb();
	string sql = "UPDATE Flights SET depart_city='" + depart_city + "', destination='" + destination + "', flight_no='" + flight_no + "', depart_time='" + depart_time + "', arrival_time='" + arrival_time +  "' total_seat='" + to_string(total_seat) + "', fare='" + to_string(fare) + "', plane_model='" + plane_model + "';";
	const char *sqlUpdate = sql.c_str();
	rc = sqlite3_exec(db, sqlUpdate, NULL, NULL, &error);
   if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
		Database::closeDb(db);
	} else {
		fprintf(stdout, "Flight updated successfully\n");
		Database::closeDb(db);
	}
};
void Flight::Delete()
{
	int rc;
	char *error;
	sqlite3 *db = Database::openDb();
	string sql = "DELETE FROM Flights WHERE id=" + to_string(id) + ";";
	const char *sqlUpdate = sql.c_str();
	rc = sqlite3_exec(db, sqlUpdate, NULL, NULL, &error);
   if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
		Database::closeDb(db);
	} else {
		fprintf(stdout, "Flight updated successfully\n");
		Database::closeDb(db);
	}
};

static int selectCallback(void *ptr, int count, char **data, char **columns)
{
	if(count == 0)
	{
		return 0;
	}

	std::vector<Flight>* flights = static_cast<std::vector<Flight>*>(ptr);

	int id;
	string depart_city;
	string destination;
	string flight_no;
	string depart_time;
	string arrival_time;
	int total_seat;
	double fare;
	string plane_model;
	for(int i = 0; i<count; i++){
		string colName(columns[i]);

		if(colName == "depart_city")
		{
			depart_city = data[i];
		} else if(colName == "destination") {
			destination = data[i];
		} else if(colName == "flight_no")
		{
			flight_no = data[i];
		} else if(colName == "depart_time")
		{
			depart_time = data[i];
		} else if(colName == "plane_model")
		{
			plane_model = data[i];
		} else if(colName == "arrival_time")
		{
			arrival_time = data[i];
		} else if(colName == "total_seat")
		{
			total_seat = atoi(data[i]);
		} else if(colName == "fare")
		{
			fare = atof(data[i]);
		} else if(colName == "id")
		{
			id = atoi(data[i]);
		}
	}

	Flight flight(id, depart_city, destination, flight_no,
		depart_time, arrival_time, total_seat, fare, plane_model);
	flights->push_back(flight);
	return 0;
}

vector<Flight> Select(string sql)
{
	vector<Flight> flights;
	sqlite3 *db = Database::openDb();
	char *zErrMsg = 0;
	int rc;
	const char* data = "Callback function called";
	User user;

	int n = sql.length();
	const char *cstr = sql.c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, cstr, selectCallback, &flights, &zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		Database::closeDb(db);
		return flights;
	} else {
		fprintf(stdout, "Operation done successfully\n");
		Database::closeDb(db);
		return flights;
	}
}

vector<User> Flight::GetUsers()
{
  vector<User> users;
  sqlite3 *db = Database::openDb();
	char *zErrMsg = 0;
	int rc;

	const char* data = "Callback function called";
  string sql = "SELECT u.id, username, password, hashed_password, role FROM FLIGHT_USER fu INNER JOIN Users u ON u.id = fu.user_id WHERE fu.flight_id = '" + to_string(id) + "';";
	int n = sql.length();
	const char *cstr = sql.c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, cstr, userSelectCallback, &users, &zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		Database::closeDb(db);
		return users;
	} else {
		fprintf(stdout, "Operation done successfully\n");
		Database::closeDb(db);
		return users;
	}
};

static int userSelectCallback(void *ptr, int count, char **data, char **columns){
	if(count == 0)
	{
		return 0;
	}

	std::vector<User>* users = static_cast<std::vector<User>*>(ptr);

	string username, hashed_password, role;
	int id;
	for(int i = 0; i<count; i++){
		string colName(columns[i]);

		if(colName == "username")
		{
			username = data[i];
		} else if(colName == "hashed_password") {
			hashed_password == data[i];
		} else if(colName == "role")
		{
			role = data[i];
		} else if(colName == "id")
		{
			id = atoi(data[i]);
		}
	}

	User user(id, username, hashed_password, role);
	users->push_back(user);
	return 0;
}

int Flight::GetId()
{
    return id;
};
string Flight::GetDepartCity()
{
    return depart_city;
};
void Flight::SetDepartCity(string d)
{
	depart_city = d;
};
string Flight::GetDestination()
{
    return destination;
};
void Flight::SetDestination(string d)
{
  destination = d;
};
string Flight::GetFlightNo()
{
    return flight_no;
};
void Flight::SetFlightNo(string f)
{
  flight_no = f;
};
string Flight::GetDepartTime()
{
    return depart_time;
};
void Flight::SetDepartTime(string d)
{
  depart_time = d;
};
string Flight::GetArrivalTime()
{
    return arrival_time;
};
void Flight::SetArrivalTime(string a)
{
  arrival_time = a;
};

int Flight::GetTotalSeat()
{
	return total_seat;
}

double Flight::GetFare()
{
	return fare;
}

string Flight::GetPlaneModel()
{
	return plane_model;
}

vector<Seat>Flight::getAllSeats()
{
	vector<Seat>allSeats;
	Seat seat;
	char col[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', '\0'};
	for (int i = 1; i < Flight::total_seat/MAX_SEAT_PER_ROW; i++) {
		for (int x = 0; x < MAX_SEAT_PER_ROW; x++) {
			Seat seat(i, col[x]);
			allSeats.push_back(seat);
		}
		cout << endl;
	}
	return allSeats;
}
