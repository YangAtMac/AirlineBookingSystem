#pragma once

#include "Flight.h"
#include "User.h"
#include "Seat.h"

class User;
class Flight;

struct UserReservation {
	int reserve_id;
	int userId;
	Flight *flight;
	string seat_num;
	//seat seat;  //todo change seat_num to seat object
};

struct Reservation {
	int flight_id;
	int user_id;
	std::string seat_num;
};

