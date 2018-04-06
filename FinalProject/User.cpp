#include "stdafx.h"
#include "User.h"
#include <iostream>

using namespace std;

static int selectCallback(void *ptr, int count, char **data, char **columns);
vector<User> Select(string sql);

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
	string sql = "SELECT * from Users WHERE user_id=" + to_string(id) + ";";
	vector<User> users = Select(sql);
	User user;
	if(users.size() == 0)
	{
		return user;
	}
	user = User(users[0].GetId(), users[0].GetUsername(), users[0].GetHashedPassword(), users[0].GetRole());
	return user;
};
std::vector<User> User::All()
{
	string sql = "SELECT * from Users;";
	return Select(sql);
};
void User::Create()
{
};
void User::Update()
{
};
void User::Delete()
{
};	

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
};
std::string User::GetRole()
{
	return role;
};
void User::SetRole(string r)
{
};
bool User::Authenticate()
{
	return true;
};

void User::reserveFlight()
{
	int selectId;
	bool bookIt;
	char ans;
	vector<Flight> flights;
	Flight flight, f;
	flights = flight.getFlights();
	
	// validate input - SQL injection
	do {
		cout << "Please enter flight ID you want to reserve: ";
		cin >> selectId;
	}
	while ( (selectId < 0 ) || (selectId > flights.size()));

	f = flight.getFlight(selectId);
	cout << "From: " << f.depart_city << endl;
	cout << "To: " << f.destination << endl;
	cout << "Depart time: " << f.depart_time << endl;
	cout << "Landing time: " << f.arrival_time << endl;
	cout << "Flight No: " << f.flight_no << endl;
	cout << "Flight fare: " << f.fare << endl;
	do {
		cout << endl << "Are you sure you want to book this flight? [y/n]: ";
		cin >> ans;
	} while( !cin.fail() && toupper(ans) != 'Y' && toupper(ans) != 'N' );
	bookIt = (toupper(ans)=='Y') ? true : false;
		
	if (bookIt) {
		cout << endl << "Reserve and saved successful." << endl;
		} 
}

static int selectCallback(void *ptr, int count, char **data, char **columns){
	if(count == 0) 
	{
		return 0;
	}

	std::vector<User>* users = static_cast<std::vector<User>*>(ptr);

	string username, hashed_password, role;
	int id;
	for(int i = 0; i<count; i++){
		cout << columns[i] << endl;
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
			id = (int)data[i];
		}
	}

	User user(id, username, hashed_password, role);
	users->push_back(user);
	return 0;
}

vector<User> Select(string sql)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char* data = "Callback function called";
	User user;
	std::vector<User> users;

	/* Open database */
	rc = sqlite3_open("FinalProject.db", &db);

	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return users;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}

	int n = sql.length();
	const char *cstr = sql.c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, cstr, selectCallback, &users, &zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return users;
	} else {
		fprintf(stdout, "Operation done successfully\n");
		sqlite3_close(db);
		return users;
	}
}
