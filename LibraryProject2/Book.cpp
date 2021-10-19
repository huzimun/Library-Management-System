#include "Book.h"

Book::Book() {
	id = 0;
	for (int i = 0; i < 20; ++i) {
		bookName[i] = ' ';
		author[i] = ' ';
	}
	// 两个结束符也要初始化
	bookName[20] = '\0';
	author[20] = '\0';

	state = '0';
	year = 0;
	month = 0;
	day = 0;
	price = 0.0;
}

Book::Book(int id, char bookName[21], char author[21], char state, int year, int month, int day, double price)
{
	this->id = id;
	strcpy_s(this->bookName, bookName);
	strcpy_s(this->author, author);
	this->state = state;
	this->year = year;
	this->month = month;
	this->day = day;
	this->price = price;
}

Book::Book(const Book& book) {
	id = book.id;
	strcpy_s(bookName, book.bookName);
	strcpy_s(author, book.author);
	state = book.state;
	year = book.year;
	month = book.month;
	day = book.day;
	price = book.price;
}

int Book::getId() // 获取书籍id
{
	return id;
}

char* Book::getBookName() // 获取书籍名字
{
	return bookName;
}
char* Book::getAuthor() {
	return author;
}
char Book::getState() {
	return state;
}
double Book::getPrice()
{
	return price;
}
int Book::getYear() {
	return year;
}
int Book::getMonth() {
	return month;
}
int Book::getDay() {
	return day;
}

void Book::setBookName(char bookName[21])
{
	strcpy_s(this->bookName, bookName);
}

void Book::setAuthor(char author[21])
{
	strcpy_s(this->author, author);
}

void Book::setState(char state)
{
	this->state = state;
}

double Book::setPrice(double price)
{
	return price;
}


void Book::setYear(int year) {
	this->year = year;
}
void Book::setMonth(int month) {
	this->month = month;
}
void Book::setDay(int day) {
	this->day = day;
}


