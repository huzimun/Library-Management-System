#pragma once
#include <string.h>

class User
{
private:
	int id;            // 用户ID
	char password[11]; // 用户密码，不超过10个字符，一个汉字等于2个字符
	char name[11];     // 用户名，不超过10个字符，一个汉字等于2个字符
	int curBorrowNum;  // 当前借书数量
	int overdueNum;    // 逾期书籍数量
	char role;         // 用户身份，0代表普通用户，1代表管理员
	char state;        // 用户状态，0代表在线，1代表被注销
	
public:
	// 构造函数
	User();
	User(int id, char password[11], char name[11], int curBorrowNum, char role, char state, int overdueNum);
	// 获取用户ID、用户名、用户密码、当前借书数量、用户身份、用户状态、用户逾期书籍数量
	int getId();
	char* getName();
	char* getPassword();
	int getCurBorrowNum();
	char getRole();
	char getState();
	int getOverDueNum();
	// 获取用户ID、用户名、用户密码、当前借书数量、用户身份、用户身份、用户状态、用户逾期书籍数量
	void setId(int id);
	void setName(char name[11]);
	void setPassword(char password[11]);
	void setCurBorrowNum(int num);
	void setRole(char role);
	void setState(char state);
	void setOverDueNum(int overdueNum);
};

