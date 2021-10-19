#pragma once
#include "User.h"
#include "Book.h"
#include "Record.h"
#include "Configuration.h"
#include <iostream>
using namespace std;

class View
{
private:
	// 登录系统时所处的时间
	int year;
	int month;
	int day;

public:
	// 构造函数
	View();
	View(int year, int month, int day);

	void startView();      // 身份选择界面
	void searchBookView(); // 查书籍界面
	void searchUserView(); // 查用户界面
	void userView(User* user, int maxBorrow); // 普通用户功能选择界面
	void managerView();	// 管理员功能选择界面
	void showBook(Book* book); // 打印书籍信息
	void showUser(User* user, int maxBorrow); // 打印当前用户信息
	void modifyConfigurationView(); // 修改借阅规则界面
	void showConfiguration(Configuration* cfg); // 展示当前借阅规则界面
	void modifyBookView(); // 修改书籍状态
	void showRecord(Record* record1);
};