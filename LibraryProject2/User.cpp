#include "User.h"

User::User() {
	id = 0;
	role = '0';
	state = '0';
	for (int i = 0; i < 10; ++i) {
		password[i] = ' ';
		name[i] = ' ';
	}
	password[10] = '\0';
	name[10] = '\0';
	curBorrowNum = 0;
	overdueNum = 0;
}

User::User(int id, char password[11], char name[11], int curBorrowNum, char role, char state, int overdueNum)
{
	this->id = id;
	strcpy_s(this->password, password);
	strcpy_s(this->name, name);
	this->curBorrowNum = curBorrowNum;
	this->role = role;
	this->state = state;
	this->overdueNum = overdueNum;
}



int User::getId()
{
	return id;
}

void User::setId(int id)
{
	this->id = id;
}

char* User::getName()
{
	return name;
}

char* User::getPassword()
{
	return password;
}

int User::getCurBorrowNum()
{
	return curBorrowNum;
}

char User::getRole()
{
	return role;
}

char User::getState()
{
	return state;
}

int User::getOverDueNum()
{
	return overdueNum;
}

void User::setRole(char role)
{
	this->role = role;
}

void User::setState(char state)
{
	this->state = state;
}

void User::setOverDueNum(int overdueNum)
{
	this->overdueNum = overdueNum;
}


void User::setCurBorrowNum(int num)
{
	curBorrowNum = num;
}

void User::setName(char name[11])
{
	strcpy_s(this->name, name);
}

void User::setPassword(char password[11])
{
	strcpy_s(this->password, password);
}
