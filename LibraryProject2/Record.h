#pragma once
#include <string.h>
// 既可以用来查询用户的借书记录，也可以查询书籍的被借记录
class Record
{
private:
	int userId;       // 用户ID
	int bookId;       // 书籍ID
	char bookName[21];// 书名，不超过20个字符，一个汉字等于2个字符
	int curRenewNum;  // 书籍当前被续借次数
	int borrowYear;   // 书籍被借出的年
	int borrowMonth;  // 书籍被借出的月
	int borrowDay;    // 书籍被借出的日
	int returnYear;  // 书籍应当归还的年
	int returnMonth; // 书籍应当归还的月
	int returnDay;   // 书籍应当归还的日
	char state;      // 0代表当前借阅记录，1代表历史借阅记录

public:
	// 两个构造函数
	Record();
	Record(int userId, int bookId, int borrowYear, int borrowMonth, int borrowDay, int returnYear, int returnMonth, int returnDay, char state, int curRenewNum, char bookName[21]);
	// 获取用户ID、书籍ID、书名、书籍被续借的次数、书籍被借出和归还的年月日、借阅记录状态
	int getUserId();
	int getBookId();
	int getBorrowYear();
	int getBorrowMonth();
	int getBorrowDay();
	int getReturnYear();
	int getReturnMonth();
	int getReturnDay();
	char getState();
	int getCurRenewNum();
	// 设置用户ID、书籍ID、书名、书籍被续借的次数、书籍被借出和归还的年月日、借阅记录状态
	char* getBookName();
	void setBookName(char bookName[21]);
	void setCurRenewNum(int renewNum);
	void setBorrowYear(int borrowYear);
	void setBorrowMonth(int borrowMonth);
	void setBorrowDay(int borrowDay);
	void setReturnYear(int returnYear);
	void setReturnMonth(int returnMonth);
	void setReturnDay(int returnDay);
	void setState(char state);
};

