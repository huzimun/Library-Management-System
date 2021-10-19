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
	// ��¼ϵͳʱ������ʱ��
	int year;
	int month;
	int day;

public:
	// ���캯��
	View();
	View(int year, int month, int day);

	void startView();      // ���ѡ�����
	void searchBookView(); // ���鼮����
	void searchUserView(); // ���û�����
	void userView(User* user, int maxBorrow); // ��ͨ�û�����ѡ�����
	void managerView();	// ����Ա����ѡ�����
	void showBook(Book* book); // ��ӡ�鼮��Ϣ
	void showUser(User* user, int maxBorrow); // ��ӡ��ǰ�û���Ϣ
	void modifyConfigurationView(); // �޸Ľ��Ĺ������
	void showConfiguration(Configuration* cfg); // չʾ��ǰ���Ĺ������
	void modifyBookView(); // �޸��鼮״̬
	void showRecord(Record* record1);
};