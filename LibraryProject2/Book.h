#pragma once
#include <string.h>

class Book
{
private:
	int id; // 书籍编号
	char bookName[21]; // 书名，不超过20个字符，一个汉字等于2个字符
	char author[21]; // 作者，不超过20个字符，一个汉字等于2个字符
	char state; // 0可借，1不可借被借阅，2不可借丢失，3不可借损坏，4不可借被删除
	double price; // 书籍价格
	// 如果被借出，应当归还的日期（默认为0年0月0日）
	int year;
	int month;
	int day;
public:
	// 三个构造函数
	Book();
	Book(int id, char bookName[21], char author[21], char state, int year, int month, int day, double price);
	Book(const Book&); // 用已有的对象实例化新的对象

	int getId(); // 获取书籍id
	char* getBookName(); // 获取书名
	char* getAuthor(); // 获取作者
	char getState(); // 获取书籍状态
	double getPrice(); // 获取书籍价格
	// 获取应当归还的年月日
	int getYear();
	int getMonth();
	int getDay();
	void setBookName(char bookName[21]);
	void setAuthor(char author[21]);
	void setState(char state);  // 设置书籍状态
	double setPrice(double price); // 设置书籍价格
	// 设置归还的年月日
	void setYear(int year);
	void setMonth(int month);
	void setDay(int year);
	
};

