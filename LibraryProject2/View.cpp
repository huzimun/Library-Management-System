#include "View.h"

View::View()
{
	year = 0;
	month = 0;
	day = 0;
}

View::View(int year, int month, int day)
{
	this->year = year;
	this->month = month;
	this->day = day;
}

void View::startView()
{
	cout << "��ǰʱ��: " << year << "-" << month << "-" << day << endl;
	cout << "��ӭ����ͼ�����ϵͳ" << endl;
	cout << "1---�û���¼" << endl;
	cout << "2---�ο͵�¼" << endl;
	cout << "0---�˳�" << endl;
	cout << "����һ��ѡ��" << endl;
}

void View::searchBookView()
{
	cout << "ѡ���ѯ��ʽ" << endl;
	cout << "1---����������" << endl;
	cout << "2---�����߲���" << endl;
	cout << "3---���鼮ID����" << endl;
	cout << "4---���������������ϲ���" << endl;
	cout << "0---ȡ��" << endl;
	cout << "ѡ��������" << endl;
}

void View::searchUserView()
{
	cout << "��ѡ���ѯ�ķ�ʽ" << endl;
	cout << "1---���û���" << endl;
	cout << "2---���û�ID" << endl;
	cout << "0---ȡ��" << endl;
	cout << "����������" << endl;
}

void View::userView(User* user, int maxBorrow)
{
	cout << "��ͨ�û���¼" << endl;
	cout << "���ɽ����鼮����: " << maxBorrow << "\t��ǰ�鼮����������" << user->getCurBorrowNum() << "\t��ǰ�����鼮������" << user->getOverDueNum() << endl;
	cout << "�û������б�" << endl;
	cout << "1---�����鼮" << endl;
	cout << "2---�����鼮" << endl;
	cout << "3---�黹�鼮" << endl; // �黹�鼮ʱ��ѯ�Ƿ��ڣ������ⷣ���ڽ���ΥԼ��
	cout << "4---�鼮����" << endl;
	cout << "5---��ʾ��ǰ����" << endl;
	cout << "6---��ʾ��ʷ����" << endl;
	cout << "7---�޸ĵ�¼����" << endl;
	cout << "8---չʾ���Ĺ���" << endl;
	cout << "0---�˳���¼" << endl;
	cout << "��ѡ����Ҫ���еĲ�����" << endl;
}

void View::managerView()
{
	cout << "����Ա��¼" << endl;
	cout << "����Ա�����б�" << endl;
	cout << "1---�鿴�鼮��Ϣ" << endl;
	cout << "2---�鿴�û���Ϣ" << endl; // ����Ӧ�����ò鿴��ǰ���ĺ���ʷ���ĵ�ѡ����ת
	cout << "3---�����鼮" << endl;
	cout << "4---�����û�" << endl;
	cout << "5---ע���û�" << endl;
	cout << "6---�޸ĵ�¼����" << endl;
	cout << "7---�޸��鼮״̬" << endl;
	cout << "8---�޸Ľ��Ĺ���" << endl;
	cout << "0---�˳���¼" << endl;
	cout << "��ѡ����Ҫ���еĲ�����" << endl;
}

void View::showBook(Book* book)
{
	cout << "ID��" << book->getId() << "\t������" << book->getBookName() << "\t���ߣ�" << book->getAuthor() << "\t�鼮�۸�" << book->getPrice() << "\t�鼮״̬��";
	if (book->getState() == '0') {
		cout << "�ɽ�" << endl;
	}
	else if (book->getState() == '1') {
		cout << "���ɽ裬Ӧ���黹���ڣ�" << book->getYear() << "-" << book->getMonth() << "-" << book->getDay() << endl;
	}
	else if (book->getState() == '2') {
		cout << "���ɽ裬�鼮��ʧ" << endl;
	}
	else if (book->getState() == '3') {
		cout << "���ɽ裬�鼮����" << endl;
	}
	else if (book->getState() == '4') {
		cout << "���ɽ裬���鼮��ɾ��" << endl;
	}
	else {
		cout << "���鼮״̬��Ϣ����" << endl;
	}
}

void View::showUser(User* user, int maxBorrow)
{
	if (user->getRole() == '0') {
		cout << "�û�ID��" << user->getId() << "\t�û�����" << user->getName()<< "\t�û����룺" << user->getPassword() << "\t���ɽ���������" << maxBorrow << "\t��ǰ����������" << user->getCurBorrowNum() << "\t�����鼮������" << user->getOverDueNum() << "\t�û���ݣ���ͨ�û�";
	}
	else {
		cout << "�û�ID��" << user->getId() << "\t�û�����" << user->getName() << "\t�û����룺" << user->getPassword()  << "\t�û���ݣ�����Ա";
	}
	if (user->getState() == '0') {
		cout << "\t�˺�״̬���Ѽ���" << endl;
	}
	else {
		cout << "\t�˺�״̬��ͣ����" << endl;
	}
}

void View::modifyConfigurationView()
{
	cout << "�����޸ĵĹ������£�" << endl;
	cout << "1---�޸����ɽ��鼮����" << endl;
	cout << "2---�޸�ÿ�οɽ�������" << endl;
	cout << "3---�޸�����������" << endl;
	cout << "4---�޸����ڿ�����" << endl;
	cout << "5---�޸ĳ������ڿ�����ÿ����Ҫ���ɵ�Ǯ" << endl;
	cout << "0---ȡ��" << endl;
	cout << "������Ҫ�޸ĵĹ���" << endl;
}

void View::showConfiguration(Configuration* cfg)
{
	cout << "���ɽ��鼮������" << cfg->getMaxBorrow() << "\tÿ�οɽ���������" << cfg->getBorrowDay() << "\t�������������" << cfg->getMaxRenew() << "\t���ڿ����ڣ�" << cfg->getGracePeriod() << "\t�������ڿ����ں�ÿ�շ��" << cfg->getMoneyPerDay() << endl;
}

void View::modifyBookView()
{
	cout << "�鼮״̬���£�" << endl;
	cout << "0---�ɽ���" << endl;
	cout << "1---���ɽ裬������" << endl;
	cout << "2---���ɽ裬�鼮��ʧ" << endl;
	cout << "3---���ɽ裬�鼮����" << endl;
	cout << "4---���ɽ裬�鼮�ѱ�ɾ��" << endl;
	cout << "�����Խ������²�����" << endl;
	cout << "1. �޸��Ѿ���ɾ���鼮��״̬" << endl;
	cout << "2. ɾ�����ڱ����ĵ��鼮" << endl;
	cout << "3. ���鼮״̬����Ϊ������" << endl;
	cout << "4. ���鼮״̬����Ϊ����5�������״̬" << endl;
	cout << "�����µ��鼮״̬�����뵱ǰ���鼮״̬�˳�����" << endl;
}

void View::showRecord(Record* record1)
{
	cout <<"�û�ID��" <<record1->getUserId() << "\t�鼮ID��" << record1->getBookId() << "\t������" << record1->getBookName()
		<< "\t�������ڣ�" << record1->getBorrowYear() << "-" << record1->getBorrowMonth() << "-" << record1->getBorrowDay()
		<< "\t�������ڣ�" << record1->getReturnYear() << "-" << record1->getReturnMonth() << "-" << record1->getReturnDay() << endl;
}
