#pragma once

#include <iostream>
#include <string>

using namespace std;

class Seat
{
public:
	Seat(void);
	Seat(int seatRow, char seatCol);
	~Seat(void);

	enum seatState {
		Available = 0,
		Reserved = 1,
		Locked = 2,
	};

	int seatRow;
	char seatCol;
	string getSeatNumber();
	void setSeatNo(string seatNo);
	bool isSeatAvailable() {return isAvailable; };
	void setSeatStatus(Seat::seatState status);
	seatState getStatus() { return status;}
	void reserve();
	void cancel();

private:
	
	string seatNumber;
	bool isAvailable;
	seatState status;
	

};

