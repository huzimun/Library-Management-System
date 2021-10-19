#pragma once
#include <string.h>

class User
{
private:
	int id;            // �û�ID
	char password[11]; // �û����룬������10���ַ���һ�����ֵ���2���ַ�
	char name[11];     // �û�����������10���ַ���һ�����ֵ���2���ַ�
	int curBorrowNum;  // ��ǰ��������
	int overdueNum;    // �����鼮����
	char role;         // �û���ݣ�0������ͨ�û���1�������Ա
	char state;        // �û�״̬��0�������ߣ�1����ע��
	
public:
	// ���캯��
	User();
	User(int id, char password[11], char name[11], int curBorrowNum, char role, char state, int overdueNum);
	// ��ȡ�û�ID���û������û����롢��ǰ�����������û���ݡ��û�״̬���û������鼮����
	int getId();
	char* getName();
	char* getPassword();
	int getCurBorrowNum();
	char getRole();
	char getState();
	int getOverDueNum();
	// ��ȡ�û�ID���û������û����롢��ǰ�����������û���ݡ��û���ݡ��û�״̬���û������鼮����
	void setId(int id);
	void setName(char name[11]);
	void setPassword(char password[11]);
	void setCurBorrowNum(int num);
	void setRole(char role);
	void setState(char state);
	void setOverDueNum(int overdueNum);
};

