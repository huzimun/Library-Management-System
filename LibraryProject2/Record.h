#pragma once
#include <string.h>
// �ȿ���������ѯ�û��Ľ����¼��Ҳ���Բ�ѯ�鼮�ı����¼
class Record
{
private:
	int userId;       // �û�ID
	int bookId;       // �鼮ID
	char bookName[21];// ������������20���ַ���һ�����ֵ���2���ַ�
	int curRenewNum;  // �鼮��ǰ���������
	int borrowYear;   // �鼮���������
	int borrowMonth;  // �鼮���������
	int borrowDay;    // �鼮���������
	int returnYear;  // �鼮Ӧ���黹����
	int returnMonth; // �鼮Ӧ���黹����
	int returnDay;   // �鼮Ӧ���黹����
	char state;      // 0����ǰ���ļ�¼��1������ʷ���ļ�¼

public:
	// �������캯��
	Record();
	Record(int userId, int bookId, int borrowYear, int borrowMonth, int borrowDay, int returnYear, int returnMonth, int returnDay, char state, int curRenewNum, char bookName[21]);
	// ��ȡ�û�ID���鼮ID���������鼮������Ĵ������鼮������͹黹�������ա����ļ�¼״̬
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
	// �����û�ID���鼮ID���������鼮������Ĵ������鼮������͹黹�������ա����ļ�¼״̬
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

