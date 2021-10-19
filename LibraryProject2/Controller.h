#pragma once
#include "User.h"
#include "View.h"
#include "Book.h"
#include "Record.h"
#include "Configuration.h"
#include <string>
#include <fstream>

constexpr auto USERFILE = "User.dat"; // �û��ļ�
constexpr auto BOOKFILE = "Book.dat"; // �鼮�ļ�
constexpr auto RECORDFILE = "Record.dat"; // ��ǰ���ļ�¼�ļ�
constexpr auto CONFIGURATIONFILE = "Configuration.dat"; // ������Ϣ�ļ�
 
class Controller
{
	/*
	�������ܣ�
	1����ѯ�û���ʷ�����¼
	2����ѯ�鼮��ʷ�����¼������Ա����
	3����ѯ�鼮��ǰ��˭�裬����Ա����
	4���鼮��ʧ��Ӧ���и����
	5�����鳬�ڣ�Ӧ���и���ܣ������ⷣ������
	6������Ա��������������
	7������Ա����ÿ�ν���ʱ��
	8������Ա�����������
	9�����蹦��
	10���鼮�ļ۸����Լ���ط���
	11�����ĸ��ļ����������������Ϣ������Ľṹ��ɶ������
	���Ҫʵ��������ܣ����ļ�¼�Ͳ�����Ϊ���Է���Controller�У�ÿ��ֱ�Ӷ�����������
	���ԣ����ǿ��Բ���vector��Ϊһ�����ԣ�����������Ϊ��С������
	���ԣ���������ᵼ���û��Ľ��ļ�¼��ʾ��ȫ
	12�����ϲ�ѯ
	13���û���������
	14������Ա�޸��鼮��ѧ����Ϣ
	15�����û����鼮��չʾ���洫��View���У���View��������û����鼮��������Ϣ����
	16����������һ���鿴���Ĺ����ѡ��
	17������ʱ��ע���⸺����������Ǹ��������Ǹĳ��޷����������Ǽ��б𣿣���
	18���黹�鼮ʱ�������б����������
	19��չʾ�鼮��¼������������ͨ������һ�������ķ�ʽ�ϲ�
	20������һ��BUG����¼ʱû�м���û�ID���Ѿ����
	21�����躯��������BUG����������õ���ʲô,�Ѿ���飬��BUG
	*/
private:
	User* user; // ��ǰ��¼�û������ڻ�ȡ���ù����û��ĸ�������
	View* view; // ��ͼ���ԣ����ڵ��ø�����ͼ����
	Configuration* cfg; // ������Ϣ���ԣ����ڻ�ȡ���ø���������Ϣ
	int userPosition; // �û���¼���ļ��е�λ��
	int year; // ��ǰ��
	int month; // ��ǰ��
	int day; // ��ǰ��
	
public:
	Controller(); // ���캯��
	~Controller(); // ��������
	void setTime(); // ���õ�ǰϵͳʱ��
	int dayOfMonth(int year, int month); // ���ض�Ӧ����µ�������ǣ�浽��������
	void start(); // ��ʼ�����ѡ��

	int searchBook(); // �����鼮
	int showBook(string bookName, string author, int choice); // չʾ�鵽���鼮
	int login(); // �û���¼
	int loadOverDueNum(User& user1); // ���ص�ǰ�����鼮����
	void userMenu(); // �û�����ѡ��
	void borrowBook(); // ����
	void returnBook(); // ����
	bool isOverdue(int curYear, int curMonth, int curDay, int returnYear, int returnMonth, int returnDay); // �ж��鼮�Ƿ�����
	
	void showCurBorrow(int userId); // ��ʾ��ǰ����
	void showPastBorrow(int userId); //�����ʷ�����¼��Ϣ
	void showBorrowRecord(int userId, char recordState);

	void managerMenu(); // ����Ա����ѡ��
	void searchUser(); // �����û�

	
	void addUser(); // ����û�
	void deleteUser(); //ע���û��˺ţ�������ɾ���û���Ϣ��
	
	void addBook(); // ����鼮
	void modifyBook(); // �޸��鼮��Ϣ�������鼮��ʧ������״̬��
	void addRecord(Record& record); // ���ӽ��ļ�¼
	void modifyConfiguration();     // �޸Ľ��Ĺ���
	void modifyPassword();          // �޸�����
	bool pay(double);              // ֧����֧���ɹ�����true��ʧ�ܷ���false��Ĭ�ϳɹ���
	void renew();                  // ����
	
	// ���ļ��м����û����鼮����ǰ������Ϣ�����Ĺ�����Ϣ
	void loadUser(int userId);
	Book loadBook(int bookId);
	Record loadRecord(int userId, int bookId);
	void loadConfiguration();

	// ���ļ��б����û����鼮����ǰ������Ϣ
	void saveUser(int position, User& user1);
	void saveBook(Book& book);
	void saveRecord(Record& record);
	void saveConfiguration(Configuration& cfg1);
};
