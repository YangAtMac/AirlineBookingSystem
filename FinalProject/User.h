#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>

#include "Flight.h"
#include "User.h"
#include "Seat.h"
#include "Database.h"
#include "FinalProject.h"
#include "Reservation.h"


//using namespace std;

class Flight;
struct UserReservation;

class User
{
private:
	int id;
	std::string username;
	std::string password;
	std::string hashed_password;
	std::string role;
public:
	User(void);
	User(int i, std::string u, std::string h, std::string r);
	User(std::string u, std::string p, std::string r);
	~User(void);
	static User Find(int id);
	static User User::FindByUsername(string username);
	static std::vector<User> All();
	void Create();
	void Update();
	void Delete();
	vector<Flight> GetFlights();
	int GetId();
	std::string GetUsername();
	string GetHashedPassword();
	void SetPassword(std::string p);
	std::string GetRole();
	void SetRole(std::string r);
	bool Authenticate();
	void reserveFlight(Flight flight, int user_id,  string seat_num);
	void reserveFlight(UserReservation *uReserve);
	void cancelFlight(int flight_id);
	void reserveSeat(Flight f);
	void reserveSeat(UserReservation *uReservation);

	vector<UserReservation> GetUserReservation(int userId);
	vector<Seat> reserveSeat(Flight f, vector<Seat> seats);
	vector<Seat> reserveSeat(UserReservation *uR, vector<Seat> seats);
	void showSeatMap( Flight f, vector<Seat> seats );
	void showSeatMap(UserReservation *uR, vector<Seat> seats);
};

