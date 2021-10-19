#pragma once
#include <string.h>

class Book
{
private:
	int id; // �鼮���
	char bookName[21]; // ������������20���ַ���һ�����ֵ���2���ַ�
	char author[21]; // ���ߣ�������20���ַ���һ�����ֵ���2���ַ�
	char state; // 0�ɽ裬1���ɽ豻���ģ�2���ɽ趪ʧ��3���ɽ��𻵣�4���ɽ豻ɾ��
	double price; // �鼮�۸�
	// ����������Ӧ���黹�����ڣ�Ĭ��Ϊ0��0��0�գ�
	int year;
	int month;
	int day;
public:
	// �������캯��
	Book();
	Book(int id, char bookName[21], char author[21], char state, int year, int month, int day, double price);
	Book(const Book&); // �����еĶ���ʵ�����µĶ���

	int getId(); // ��ȡ�鼮id
	char* getBookName(); // ��ȡ����
	char* getAuthor(); // ��ȡ����
	char getState(); // ��ȡ�鼮״̬
	double getPrice(); // ��ȡ�鼮�۸�
	// ��ȡӦ���黹��������
	int getYear();
	int getMonth();
	int getDay();
	void setBookName(char bookName[21]);
	void setAuthor(char author[21]);
	void setState(char state);  // �����鼮״̬
	double setPrice(double price); // �����鼮�۸�
	// ���ù黹��������
	void setYear(int year);
	void setMonth(int month);
	void setDay(int year);
	
};

