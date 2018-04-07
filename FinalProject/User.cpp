#include "stdafx.h"
#include "User.h"
#include "Flight.h"
#include <iostream>
#include "sha256.h"

using namespace std;

static int selectCallback(void *ptr, int count, char **data, char **columns);
static int flightSelectCallback(void *ptr, int count, char **data, char **columns);
static int userFlightSelectCb(void *ptr, int count, char **data, char **columns);
vector<User> Select(string sql);
bool isSeatValid(string seatNum, vector<Seat> seats);

User::User(void)
{
}

User::User(int i, string u, string h, string r)
{
	id = i;
	username = u;
	hashed_password = h;
	role = r;
}

User::User(string u, string p, string r)
{
	username = u;
	password = p;
	role = r;
}

User::~User(void)
{
}

User User::Find(int id)
{
	string sql = "SELECT * from USERS WHERE id=" + to_string(id) + ";";
	vector<User> users = Select(sql);
	if(users.size() == 0)
	{
		User user;
		return user;
	}
	User user = User(users[0].GetId(), users[0].GetUsername(), users[0].GetHashedPassword(), users[0].GetRole());
	return user;
};

User User::FindByUsername(string username)
{
	string sql = "SELECT * from USERS WHERE username='" + username + "';";
	vector<User> users = Select(sql);
	cout << users[0].GetUsername();
	if(users.size() == 0)
	{
		User user;
		return user;
	}
	User user = User(users[0].GetId(), users[0].GetUsername(), users[0].GetHashedPassword(), users[0].GetRole());
	return user;
}

std::vector<User> User::All()
{
	string sql = "SELECT * from USERS;";
	std::vector<User> users = Select(sql);
	return users;
};
void User::Create()
{
	int rc;
	char *error;
	sqlite3 *db = Database::openDb();
	string sql = "INSERT INTO USERS (username, hashed_password, role) VALUES('" + username + "', '" + sha256(password) + "', '" + role + "');";
	const char *sqlInsert = sql.c_str();
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
   if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
		Database::closeDb(db);
	} else {
		fprintf(stdout, "User created successfully\n");
		Database::closeDb(db);
		string sql = "SELECT * from USERS WHERE username='" + username + "';";
		vector<User> users = Select(sql);
		id = users[0].id;
	}
};
void User::Update()
{
	int rc;
	char *error;
	sqlite3 *db = Database::openDb();
	string sql = "UPDATE USERS SET username='" + username +  "', hashed_password='" + hashed_password +  "', role='" + role +  "';";
	const char *sqlUpdate = sql.c_str();
	rc = sqlite3_exec(db, sqlUpdate, NULL, NULL, &error);
   if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
		Database::closeDb(db);
	} else {
		fprintf(stdout, "User updated successfully\n");
		Database::closeDb(db);
	}
};
void User::Delete()
{
	int rc;
	char *error;
	sqlite3 *db = Database::openDb();
	string sql = "DELETE FROM USERS WHERE id=" + to_string(id) + ";";
	const char *sqlUpdate = sql.c_str();
	rc = sqlite3_exec(db, sqlUpdate, NULL, NULL, &error);
   if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
		Database::closeDb(db);
	} else {
		fprintf(stdout, "User updated successfully\n");
		Database::closeDb(db);
	}
};

vector<Flight> User::GetFlights()
{
  vector<Flight> flights;
  sqlite3 *db = Database::openDb();
	char *zErrMsg = 0;
	int rc;

	string sql = "SELECT f.id,	depart_city, destination, flight_no, depart_time, arrival_time, total_seat,	fare, plane_model  FROM RESERVATION fu INNER JOIN FLIGHTS f ON f.id = fu.flight_id WHERE fu.user_id = '" + to_string(id) + "';";
	int n = sql.length();
	const char *cstr = sql.c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, cstr, flightSelectCallback, &flights, &zErrMsg);

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
};

vector<UserReservation>  User::GetUserReservation(int userId)
{
	vector<UserReservation> userFlights;
	sqlite3 *db = Database::openDb();
	char *zErrMsg = 0;
	int rc;

	string sql = "SELECT * FROM RESERVATION fu INNER JOIN FLIGHTS f ON f.id = fu.flight_id WHERE fu.user_id = '" + to_string(userId) + "';";
	//string sql = "SELECT fu.id, fu.seat_no, user_id, user.username, user.role, f.depart_city, destination, depart_time, arrival_time, flight_id, flight_no" \
	//			"FROM FLIGHT_USER fu INNER JOIN FLIGHTS f ON f.id = fu.flight_id INNER JOIN USERS user ON user.id = fu.user_id WHERE fu.user_id = '" + to_string(userId) + "';";

	int n = sql.length();
	const char *cstr = sql.c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, cstr, userFlightSelectCb, &userFlights, &zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	Database::closeDb(db);
	return userFlights;
};

static int flightSelectCallback(void *ptr, int count, char **data, char **columns)
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

	Flight flight(id, depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model);
	flights->push_back(flight);
	return 0;
}

static int userFlightSelectCb(void *ptr, int count, char **data, char **columns)
{
	if(count == 0)
	{
		return 0;
	}

	std::vector<UserReservation>* userFlights = static_cast<std::vector<UserReservation>*>(ptr);

	int id;
	int reserve_id;
	int userId;
	int flight_id;
	string seat_num;
	string depart_city;
	string destination;
	string flight_no;
	string depart_time;
	string arrival_time;
	int total_seat;
	double fare;
	string plane_model;
	for(int i = 0; i < count; i++){
		string colName(columns[i]);
		if (colName == "reserve_id") {
			 reserve_id = atoi(data[i]);
		} else if(colName == "id") {
			 id = atoi(data[i]);
		} else if (colName == "seat_no") {
			seat_num = data[i];
		} else if(colName == "user_id"){
			userId = atoi(data[i]);
		} else if(colName == "flight_id"){
			flight_id = atoi(data[i]);
		} else if(colName == "depart_city"){
			depart_city = data[i];
		} else if(colName == "destination") {
			destination = data[i];
		} else if(colName == "flight_no") {
			flight_no = data[i];
		} else if(colName == "depart_time") {
			depart_time = data[i];
		} else if(colName == "plane_model") {
			plane_model = data[i];
		} else if(colName == "arrival_time") {
			arrival_time = data[i];
		} else if(colName == "total_seat") {
			total_seat = atoi(data[i]);
		} else if(colName == "fare") {
			fare = atof(data[i]);
		} 
	}

	
	//Flight flight(id, depart_city, destination, flight_no, depart_time, arrival_time, total_seat, fare, plane_model);
	Flight *flight = new Flight();
	flight->SetId(id);
	flight->SetDepartCity(depart_city);
	flight->SetDestination(destination);
	flight->SetFlightNo(flight_no);
	flight->SetDepartTime(depart_time);
	flight->SetArrivalTime(arrival_time);
	flight->SetTotalSeat(total_seat);
	flight->SetFare(fare);
	flight->SetPlaneModel(plane_model);
	UserReservation uReservation;
	uReservation.reserve_id = reserve_id;
	uReservation.flight = flight;
	uReservation.userId = userId;
	uReservation.seat_num = seat_num;
	userFlights->push_back(uReservation);
	return 0;
}

int User::GetId()
{
	return id;
};
string User::GetUsername()
{
	return username;
}
string User::GetHashedPassword()
{
	return hashed_password;
}
void User::SetPassword(string p)
{
	password = p;
};
std::string User::GetRole()
{
	return role;
};
void User::SetRole(string r)
{
	role = r;
};
bool User::Authenticate()
{
	if(hashed_password.compare(sha256(password)) == 0)
	{
		return true;
	} else {
		return false;
	}
};

static int selectCallback(void *ptr, int count, char **data, char **columns){
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
			hashed_password = data[i];
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

vector<User> Select(string sql)
{
	vector<User> users;
	sqlite3 *db = Database::openDb();
	char *zErrMsg = 0;
	int rc;
	const char* data = "Callback function called";
	User user;

	int n = sql.length();
	const char *cstr = sql.c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, cstr, selectCallback, &users, &zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		Database::closeDb(db);
		return users;
	} else {
		//fprintf(stdout, "Operation done successfully\n");
		Database::closeDb(db);
		return users;
	}
}

void User::reserveFlight(int flight_id, int user_id,  string seatNumber)
{
	Database db;
	Reservation reservation;
	reservation.flight_id = flight_id;
	reservation.seat_num = seatNumber;
	reservation.user_id = user_id;
	//reserveSeat();
	db.saveReservation(reservation);
};

void User::cancelFlight(int reservationId)
{
	Database db;
	db.cancelReservation(reservationId);
};

void User::reserveSeat(Flight f)
{
	char answer;
	bool resSeat;
	do {
		cout << endl << "Do you want to reserve your seat? [y/n]";
		cin >> answer;
	} while( !cin.fail() && toupper(answer) != 'Y' && toupper(answer) != 'N' );
	resSeat = (toupper(answer)=='Y') ? true : false;
	if (resSeat) {
		vector<Seat> seats = f.getAllSeats();
 		showSeatMap(f, seats);
		reserveSeat(f, seats);
	}
}

vector<Seat> User::reserveSeat(Flight f, vector<Seat> seats)
{
	string userAnswer;
	do {
		cout << "Please enter the seat number: ";
		cin >> userAnswer;
	} while (!isSeatValid(userAnswer, seats));
	//TODO reserve seat
	for ( vector<Seat>::iterator it = seats.begin(); it != seats.end(); ++it ){
		if ( userAnswer == it->getSeatNumber() ) {
			it->reserve();
		} 
	}
	cout << "Your seat is reserved successfully. display again." << endl;
	showSeatMap(f, seats);
	return seats;
}

bool isSeatValid(string seatNum, vector<Seat> seats)
{
	bool isValid = false;
	cout << "Your entry is: " << seatNum << endl;
	for ( vector<Seat>::iterator it = seats.begin(); it != seats.end(); ++it ){
		if ( seatNum == it->getSeatNumber() ) {
			isValid = true;
			// return index would be better.
		} 
	}
	if (!isValid)
			cout << "Your entry is invalid, please try again." <<endl;
	return isValid;
}

void User::showSeatMap(Flight f, vector<Seat> seats )
{ 
    //system( "cls" );
	cout << "Total seats: " << f.GetTotalSeat() << endl;;
    cout << "*********     Welcome to seat reservation system     *********\n\n";

	for ( vector<Seat>::iterator it = seats.begin(); it != seats.end(); ++it ) {
		cout << " ";
		if (it->isSeatAvailable()) {
			cout << it->getSeatNumber() << "\t";
		} else {
			cout << "X\t";
		}
		if ( it->seatCol == 'G') {
			cout << endl;
		}
	}
}
