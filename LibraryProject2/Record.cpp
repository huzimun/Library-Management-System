#include "Record.h"


Record::Record()
{
    userId = 0;
    bookId = 0;
    borrowYear = 0;
    borrowMonth = 0;
    borrowDay = 0;
    returnYear = 0;
    returnMonth = 0;
    returnDay = 0;
    state = '0';
    curRenewNum = 0;
    for (int i = 0; i < 21; ++i) {
        bookName[i] = ' ';
    }
}

Record::Record(int userId, int bookId, int borrowYear, int borrowMonth, int borrowDay, int returnYear, int returnMonth, int returnDay, char state, int curRenewNum, char bookName[21])
{
    this->userId = userId;
    this->bookId = bookId;
    this->borrowDay = borrowDay;
    this->borrowMonth = borrowMonth;
    this->borrowYear = borrowYear;
    this->returnDay = returnDay;
    this->returnMonth = returnMonth;
    this->returnYear = returnYear;
    this->state = state;
    this->curRenewNum = curRenewNum;
    strcpy_s(this->bookName, bookName);
}

int Record::getUserId()
{
    return userId;
}

int Record::getBookId()
{
    return bookId;
}

int Record::getBorrowYear()
{
    return borrowYear;
}

int Record::getBorrowMonth()
{
    return borrowMonth;
}

int Record::getBorrowDay()
{
    return borrowDay;
}

int Record::getReturnYear()
{
    return returnYear;
}

int Record::getReturnMonth()
{
    return returnMonth;
}

int Record::getReturnDay()
{
    return returnDay;
}

char Record::getState()
{
    return state;
}

int Record::getCurRenewNum()
{
    return curRenewNum;
}

void Record::setBorrowYear(int year)
{
    borrowYear = year;
}

void Record::setBorrowMonth(int month)
{
    borrowMonth = month;
}

void Record::setBorrowDay(int day)
{
    borrowDay = day;
}

void Record::setReturnYear(int year)
{
    returnYear = year;
}

void Record::setReturnMonth(int month)
{
    returnMonth = month;
}

void Record::setReturnDay(int day)
{
    returnDay = day;
}

void Record::setState(char state)
{
    this->state = state;
}

void Record::setCurRenewNum(int renewNum)
{
    this->curRenewNum = renewNum;
}

char* Record::getBookName()
{
    return bookName;
}

void Record::setBookName(char bookName[21])
{
    strcpy_s(this->bookName, bookName);
}