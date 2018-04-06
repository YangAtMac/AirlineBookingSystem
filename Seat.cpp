#include "stdafx.h"
#include "Seat.h"

using namespace std;

Seat::Seat(void)
{
	isAvailable = true;
	Seat::status = Seat::seatState::Available;
}

Seat::Seat(int seatRow, char seatCol)
{
	isAvailable = true;
	Seat::status = Seat::seatState::Available;

	Seat::seatRow = seatRow;
	Seat::seatCol = seatCol;
	seatNumber = std::to_string(seatRow) + seatCol;
}

string Seat::getSeatNumber()
{
	return seatNumber;
}

void Seat::setSeatNo(string seatNo)
{
	seatNumber = seatNo;
}

void Seat::setSeatStatus(Seat::seatState status)
{
	Seat::status = status;
}

void Seat::reserve()
{
	Seat::status = Seat::seatState::Reserved;
	isAvailable = false;
}

void Seat::cancel()
{
	Seat::status = Seat::seatState::Available;
	isAvailable = true;
}

Seat::~Seat(void)
{
}

