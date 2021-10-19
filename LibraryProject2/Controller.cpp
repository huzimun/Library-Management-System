#include "Controller.h"


Controller::Controller()
{
	view = NULL;
	user = NULL;
	userPosition = -1;
	year = 0;
	month = 0;
	day = 0;
	// ��ʼ��������Ϣ
	cfg = NULL;
}

Controller::~Controller()
{
	delete user;
	delete view;
	delete cfg;
}

// ����ϵͳ��ǰʱ�䣬����������Էŵ�Controller���캯����ѽ
void Controller::setTime()
{
	time_t timep;
	struct tm p;

	time(&timep); //��ȡ��1970������˶����룬����time_t���͵�timep
	localtime_s(&p, &timep);//��localtime������ת��Ϊstruct tm�ṹ��

	year = 1900 + p.tm_year;
	month = 1 + p.tm_mon;
	day = p.tm_mday;
}

int Controller::dayOfMonth(int year, int month)
{
	// �ж�����
	bool isLeapYear = false;
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
		isLeapYear = true;
	}
	switch (month) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
		break;
	case 2:
		return isLeapYear ? 29 : 28;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

void Controller::start()
{
	setTime();	// ����ϵͳ��ǰʱ��
	loadConfiguration();
	if (cfg == NULL) { // ����ʧ�ܺ�ʹ��Ĭ�ϵ�������Ϣ
		cout << "ʹ�õ���Ĭ�Ϲ���" << endl;
		cfg = new Configuration();
	}
	view = new View(year, month, day); // ������Ϣ���Ա���Ҫ����֮ǰ����

	string choice; // ���ѡ��
	while (true) {
		view->startView();
		cin >> choice;

		if (choice == "1") { // �û�
			if (login()) { // ��¼�ɹ�
				if (user->getRole() == '0') {
					system("pause");
					system("cls");
					
					userMenu();
				}
				else {
					system("pause");
					system("cls");
					
					managerMenu();
				}
				
			}
		}
		else if (choice == "2") { // �οͷ��ʣ�ֻ�����˲����鼮�Ľӿڸ��ο�
			
			system("pause");
			system("cls");
			cout << "�ο�ֻ�ܲ����鼮����Ҫ�������ȵ�¼" << endl;
			while (searchBook()) {

			}
			system("pause");
			system("cls");
		}
		else if (choice == "0") {
			cout << "��ӭ�ٴ�ʹ��" << endl;
			exit(0);
		}
		else {
			cout << "�������������ѡ��" << endl;
			system("pause");
			system("cls");
		}
	}
}

int Controller::searchBook() { // ����
	while (true) {
		view->searchBookView();
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,����������:" << endl;
		}
		string bookName = "";
		string author = "";
		int id = -1;
		Book book;

		switch (choice) {
		case '1': // ������
			while (true) {
				cout << "��������(�ַ����Ȳ�����20��һ�����ֵ��������ַ�)��";
				cin >> bookName;
				if (bookName.length() > 20)
					cout << "�Ƿ����룬���������룡" << endl;
				else break;
			}
			if (showBook(bookName, author, 1)) { // �鵽��
				return 1;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case '2': // ��������
			while (true) {
				cout << "��������(�ַ����Ȳ�����20��һ�����ֵ��������ַ�)��";
				cin >> author;
				if (author.length() > 20)
					cout << "�Ƿ����룬���������룡" << endl;
				else break;
			}
			if (showBook(bookName, author, 2)) { // �鵽��
				return 1;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case '3': // ��ID
			// ����Ĵ�����Կ���ʹ��loadBook��������
			cout << "����ID��" << endl;
			while (!(cin >> id) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			book = loadBook(id);
			if (book.getId() == id && id != 0) {
				view->showBook(&book); // ��ӡ�����鼮��¼
				return 1;
			}
			else {
				cout << "���鼮ID�����ڣ�" << endl;
			}
			break;
		case '4': // ���������������ϲ�ѯ
			while (true) {
				cout << "��������(�ַ����Ȳ�����20��һ�����ֵ��������ַ�)��";
				cin >> bookName;
				if (bookName.length() > 20)
					cout << "�Ƿ����룬���������룡" << endl;
				else break;
			}
			while (true) {
				cout << "��������(�ַ����Ȳ�����20��һ�����ֵ��������ַ�)��";
				cin >> author;
				if (author.length() > 20)
					cout << "�Ƿ����룬���������룡" << endl;
				else break;
			}
			if (showBook(bookName, author, 3)) { // �鵽��
				return 1;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case '0':
			return 0;
		default:
			break;
		}
	}
}

int Controller::showBook(string bookName, string author, int choice)
{
	ifstream bookFile(BOOKFILE, ios::binary | ios::in);
	Book book;
	int num = 0; // �鵽���鼮����
	// ÿ�δ��ļ��ж�ȡ��������
	if (choice == 1) { // ����������
		while (bookFile.read((char*)&book, sizeof(book))) {
			string bookName1 = book.getBookName();
			// cout << "��������ѯչʾ�鼮" << book.getBookName() << "ID��" << book.getId() << endl;
			if (bookName1 == bookName) {
				cout << ++num << "-";
				view->showBook(&book);
			}
		}
	}
	else if (choice == 2) { // �����߲���
		while (bookFile.read((char*)&book, sizeof(book))) {
			string author1 = book.getAuthor();
			if (author1 == author) {
				cout << ++num << "-";
				view->showBook(&book);
			}
		}
	}
	else {
		while (bookFile.read((char*)&book, sizeof(book))) {
			string author1 = book.getAuthor();
			string bookName1 = book.getBookName();
			if (author1 == author && bookName1 == bookName) {
				cout << ++num << "-";
				view->showBook(&book);
			}
		}
	}
	bookFile.close();
	if (num == 0) {
		cout << "û���ҵ������������鼮��" << endl;
	}
	return num;
}

// Ӧ�ü����û��˺��Ƿ��ڼ���״̬
int Controller::login()
{
	while (true) {
		cout << "�������ID������0�˳�)��" << endl;
		int id = -1;
		while (!(cin >> id) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');//������뻺�����ĵ�ǰ��
			cout << "�������ݴ���,����������!" << endl;
		}
		if (id == 0) { // �˳�
			system("pause");
			system("cls");
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return 0;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');//������뻺������ǰ��

		loadUser(id); // �����û����󣬼���ʧ��user��NULL
		if (user == NULL || user->getId() != id) {
			cout << "���û������ڣ�" << endl;
			system("pause");
			system("cls");
			return 0;
		}
		else {
			// �û��˻�ͣ�ú���Ҫ����
			if (user->getState() == '1') {
				cout << "���˻�ͣ�ã����ȼ���" << endl;
				system("pause");
				system("cls");
				return 0;
			}
			while (true) {
				cout << "����������루0�˳�����" << endl;
				string password;
				cin >> password;
				if (password == "0") {
					system("pause");
					system("cls");
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					return 0;
				}
				if (password.compare(user->getPassword()) != 0) {
					cout << "��������, ���������룡" << endl;
				}
				else {
					cout << "��¼�ɹ���" << endl;
					return 1;
				}
			}
		}
	}
	return 0;
}

int Controller::loadOverDueNum(User& user1)
{
	Record record1;
	int overDueNum = 0; // ��¼�����鼮����
	int borrowNum = 0; // ��¼�Ѿ����������ڽ��鼮����
	ifstream recordFile(RECORDFILE, ios::binary);
	while (recordFile.read((char*)&record1, sizeof(record1))) {
		// ���û��ĵ�ǰ���ļ�¼
		if (record1.getUserId() == user1.getId() && record1.getState() == '0') {
			// �ж��Ƿ�����
			borrowNum++;
			if (isOverdue(year, month, day, record1.getReturnYear(), record1.getReturnMonth(), record1.getReturnDay()))//����{
			{
				overDueNum++;
			}
			if (borrowNum == user1.getCurBorrowNum()) { // �Ѿ���������û����еĵ�ǰ���ļ�¼
				break;
			}
		}
	}
	recordFile.close();
	return overDueNum;
}

void Controller::userMenu()
{
	while (true) {
		view->userView(user,cfg->getMaxBorrow());
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,����������:" << endl;
		}
		switch (choice) {
		case '1'://�����鼮
			searchBook();
			break;
		case '2':// �����鼮
			if (user->getCurBorrowNum() < cfg->getMaxBorrow()) {
				borrowBook();
			}
			else {
				cout << "�㵱ǰ�Ľ��������Ѿ��ﵽ�����ֵ��" << endl;
			}
			break;
		case '3': // �黹�鼮
			returnBook();
			break;
		case '4': // ��ʾ��ǰ����
			renew();
			break;
		case '5': // ��ʾ���ڽ���
			// showCurBorrow(user->getId());
			showBorrowRecord(user->getId(), '0');
			break; 
		case '6': // ��ʾ��ʷ����
			// showPastBorrow(user->getId());
			showBorrowRecord(user->getId(), '1');
			break;
		case '7':  // �޸ĵ�¼����
			modifyPassword();
			break;
		case '8': // չʾ���Ĺ���
			view->showConfiguration(cfg);
			break;
		case '0': // ������һ��
			cout << "�˳��ɹ�����ӭ�ٴε�¼��" << endl;
			system("pause");
			system("cls");
			return;
		default:
			cout << "��������" << endl;
			break;
		}
		system("pause");
		system("cls");
	}
}

void Controller::borrowBook()
{
	if (searchBook()) { // ����֮ǰ�Ȳ���
		while (true) {
			// û���ݴ�
			cout << "������Ҫ���ĵ��鼮��ID������0ȡ������" << endl;
			int bookId = 0;
			Book book;
			while (!(cin >> bookId) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			if (bookId != 0) {
				book = loadBook(bookId);
				if (book.getId() == bookId) { // �ҵ���Ӧ�鼮
					if (book.getState() == '0') { // �ɽ�
						cout << "ȷ��Ҫ���ĸ��鼮�������� 1 ȷ�������� 0 ȡ����" << endl;
						view->showBook(&book);
						char choice;
						while (true)
						{
							while (!(cin >> choice) || cin.peek() != '\n')
							{
								cin.clear();
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
								cout << "�������ݴ���,����������:" << endl;
							}
							if (choice == '1') { // ȷ�Ͻ��ĸ��鼮
								// ��δ�����Էŵ�һ�������ĺ�����
								int returnYear = 0;
								int returnMonth = 0;
								int returnDay = 0;
								// ���Ľ����¼��ǣ�浽���ڣ����׳������ݲ�һ�£����С��
								if (day + cfg->getBorrowDay() > dayOfMonth(year, month)) {
									if (month + 1 > 12) { // ��ݼ�һ
										returnYear = year + 1;
										returnMonth = month - 11;
										returnDay = day + cfg->getBorrowDay() - dayOfMonth(year, month);
									}
									else {
										returnYear = year;
										returnMonth = month + 1;
										returnDay = day + cfg->getBorrowDay() - dayOfMonth(year, month);
									}
								}
								else {
									returnYear = year;
									returnMonth = month;
									returnDay = day + cfg->getBorrowDay();
								}

								book.setState('1'); // �޸��鼮״̬Ϊ���ɽ�
								book.setYear(returnYear);
								book.setMonth(returnMonth);
								book.setDay(returnDay);

								// �����鼮��Ϣ�������˻������ں��鼮״̬
								saveBook(book);

								// ���Ӻͱ�������¼
								Record* record = new Record(user->getId(), book.getId(), year, month, day, returnYear, returnMonth, returnDay, '0', 0, book.getBookName());
								addRecord(*record);

								user->setCurBorrowNum(user->getCurBorrowNum() + 1);

								// �����û��������˽�������
								saveUser(userPosition, *user);

								cout << "����ɹ���" << endl;
								break;
							}
							else if (choice == '0') {
								break;
							}
							else {
								cout << "�����������������" << endl;
							}
						}
					}
					else {
						cout << "���鼮���ɽ裬Ӧ���黹����Ϊ��" << book.getYear() << "-" << book.getMonth() << "-" << book.getDay() << endl;
					}
				}
			}
			else { // �˳�
				return;
			}
		}
	}
}

void Controller::returnBook(){
	if (user->getCurBorrowNum() == 0) {
		cout << "���ڽ��鼮" << endl;
		return;
	}
	showCurBorrow(user->getId());
	while (true) {
		cout << "����Ҫ�黹�鼮��ID������0ȡ������" << endl;
		int bookId = 0;
		while (!(cin >> bookId) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,����������:" << endl;
		}
		if (bookId == 0) {
			return;
		}
		bool isFind = false; // �ҵ��ı�־

		Record record1;
		ifstream recordFile(RECORDFILE, ios::binary);
		while (recordFile.read((char*)&record1, sizeof(record1))) {
			// �û�Id���鼮Id���ϣ������ǳ��ڽ���״̬�ģ���ֹ��ʷ��¼�ĸ��ţ�
			if (record1.getUserId() == user->getId() && record1.getBookId() == bookId && record1.getState() == '0') {
				isFind = true;
				// �ж��Ƿ����ڣ�������ڲ��ҳ�������
				//�ж��Ƿ�����
				int overdue = 0;//��������
				/*
				����ж����ڣ�����
				����ж�û�����ڣ�����
				��ǰʱ���Ӧ�������ʱ��Ƚ�
				���С
				���һ���·�С
				����һ������С
				cout << "��ǰ�����գ�" << year << "-" << month << "-" << day << "\tӦ���黹�����գ�" << record1.getReturnYear() << "-" << record1.getReturnMonth() << "-" << record1.getReturnDay() << endl;
				cout << (year <= record1.getReturnYear()) << endl;
				cout << (month <= record1.getReturnMonth()) << endl;
				cout << (day <= record1.getReturnDay()) << endl;
				cout << (!(year <= record1.getReturnYear() && month <= record1.getReturnMonth() && day <= record1.getReturnDay())) << endl;
				*/
				if (isOverdue(year, month, day, record1.getReturnYear(), record1.getReturnMonth(), record1.getReturnDay()))//����
				{		
					if (year > record1.getReturnYear()) // ���ڿ���
					{
						int tempYear = record1.getReturnYear();
						overdue = dayOfMonth(record1.getReturnYear(), record1.getReturnMonth()) - record1.getReturnDay(); // ���ĵ�������
						for (int j = record1.getReturnMonth() + 1; j < 13; j++) // ���ĵ���������
							overdue += dayOfMonth(record1.getReturnYear(), j);

						while (true) 
						{
							tempYear++;
							if (tempYear < year)//tempYearΪһ���꣬������ͻ�����֮�������
								for (int j = 1; j < 13; j++)
									overdue += dayOfMonth(tempYear, j);
							else //year == tempYear�����������������
							{
								for (int j = 1; j < month; j++)
									overdue += dayOfMonth(year, j);
								overdue += day;
								break;
							}
						}
					}
					if (year == record1.getReturnYear())//���ڵ���
					{
						if (record1.getReturnMonth() == month)//���ڵ���
							overdue = day - record1.getReturnDay();
						else
						{
							overdue = dayOfMonth(record1.getReturnYear(), record1.getReturnMonth()) - record1.getReturnDay();
							for (int j = record1.getReturnMonth() + 1; j < month; j++)
								overdue += dayOfMonth(record1.getReturnYear(), j);
							overdue += day;
						}
					}
					cout << "����" << overdue << "�죡" << endl;
					// �ڿ������ڻ��鲻������������ں󣬴������գ��������ڣ������ۼƷ������һ�췣��moneyPerDayԪ
					if (overdue > cfg->getGracePeriod()) {
						cout << "Ӧ����" << overdue * cfg->getMoneyPerDay() << "Ԫ��" << endl;
						if (pay(overdue * cfg->getMoneyPerDay()))
						{
							system("pause");
							cout << "֧���ɹ���" << endl;
						}
					}
					
				}
				// �޸��鼮״̬
				Book book = loadBook(bookId);
				book.setDay(0);
				book.setMonth(0);
				book.setYear(0);
				book.setState('0');
				saveBook(book);
				// �޸��û���Ϣ
				if (overdue > 0) {
					user->setOverDueNum(user->getOverDueNum() - 1);
				}
				user->setCurBorrowNum(user->getCurBorrowNum() - 1);
				saveUser(userPosition, *user);
				// �޸Ľ��ļ�¼������ʱ������
				record1.setState('1'); // ����Ϊ��ʷ�����¼
				record1.setReturnYear(year); // ������ʱ������Ϊ��ǰʱ��
				record1.setReturnMonth(month);
				record1.setReturnDay(day);
				saveRecord(record1);
				cout << "�鼮�黹�ɹ���" << endl;
				break;
			}

		}
		recordFile.close();
		if (!isFind) // û�ҵ�
		{
			cout << "���鼮ID���ڽ��鼮�����ڣ�" << endl;
		}
	}
}

// �Ƿ����ڣ����ڷ���TRUE��
bool Controller::isOverdue(int curYear, int curMonth, int curDay, int returnYear, int returnMonth, int returnDay)
{
	if (curYear < returnYear) {
		return false;
	}
	else if (curYear == returnYear && curMonth < returnMonth) {
		return false;
	}
	else if (curYear == returnYear && curMonth == returnMonth && curDay <= returnDay) {
		return false;
	}
	else {
		return true;
	}
}

void Controller::showCurBorrow(int userId)
{
	Record record1;
	int recordNum = 0;
	ifstream recordFile(RECORDFILE, ios::binary);
	int num = 0;
	while (recordFile.read((char*)&record1, sizeof(record1))) {
		// ��鱨��cout << "���ļ�¼�е��û�ID��" << record1.getUserId() << endl;
		if (record1.getUserId() == userId && record1.getState() == '0') {
			
			printf("%d.", num + 1);
			view->showRecord(&record1);
			num++;
		}
	}
	if (num == 0) {
		cout << "���û����ڽ��鼮" << endl;
	}
	recordFile.close();
	return;
}

void Controller::showPastBorrow(int userId)
{
	Record record1;
	int recordNum = 0;
	ifstream recordFile(RECORDFILE, ios::binary);
	int num = 0;
	while (recordFile.read((char*)&record1, sizeof(record1))) {
		// ��鱨��cout << "���ļ�¼�е��û�ID��" << record1.getUserId() << endl;
		if (record1.getUserId() == userId && record1.getState() == '1') {

			printf("%d.", num + 1);
			view->showRecord(&record1);
			num++;
		}
		
	}
	if (num == 0) {
		cout << "���û�����ʷ�����鼮" << endl;
	}
	recordFile.close();
	return;
}

void Controller::showBorrowRecord(int userId, char recordState) {
	Record record1;
	int recordNum = 0;
	ifstream recordFile(RECORDFILE, ios::binary);
	int num = 0;
	while (recordFile.read((char*)&record1, sizeof(record1))) {
		// ��鱨��cout << "���ļ�¼�е��û�ID��" << record1.getUserId() << endl;
		if (record1.getUserId() == userId && record1.getState() == recordState) {

			printf("%d.", num + 1);
			view->showRecord(&record1);
			num++;
		}
	}
	if (num == 0) {
		if (recordState == '1') {
			cout << "���û�����ʷ�����鼮" << endl;
		}
		if (recordState == '0') {
			cout << "���û��޵�ǰ�����鼮" << endl;
		}
	}
	recordFile.close();
	return;
}
void Controller::managerMenu()
{
	while (true) {
		view->managerView();
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,���������룺" << endl;
		}

		switch (choice) {
		case '1':// �鿴�鼮��Ϣ
			searchBook();
			break;
		case '2':// �鿴�û���Ϣ
			searchUser();
			break;
		case '3':// �����鼮��Ϣ
			addBook();
			break;
		case '4': // �����û���Ϣ
			addUser();
			break;
		case '5': // ע���û��˺�
			deleteUser();
			break;
		case '6': // �޸ĵ�¼����
			modifyPassword();
			break;
		case '7': // �޸��鼮״̬��Ϣ
			modifyBook();
			break;
		case '8': // �޸Ľ��Ĺ���
			modifyConfiguration();
			break;
		case '0': // �˳�
			cout << "�˳��ɹ�����ӭ�ٴε�¼��" << endl;
			system("pause");
			system("cls");
			return;
		default:
			cout << "����������������룡" << endl;
			break;
		}
		system("pause");
		system("cls");
	}
}

void Controller::searchUser()
{
	while (true) {
		view->searchUserView();

		ifstream userFile(USERFILE, ios::in | ios::binary);
		if (!userFile) {
			cout << "�û��ļ���ʧ�ܣ�" << endl;
			userFile.close();
			return;
		}

		User user1;
		string name;
		int id = 0;
		int num = 0; // ͬ���û���
		int i = 0; // ��ǰ��¼��λ��
		int position = 0; // ���ҵ����û���λ��
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,���������룺" << endl;
		}

		switch (choice)
		{
		case '1':// ���û�����ѯ
			while (true)
			{
				cout << "�������������Ȳ�����10���ַ���һ�����ֵ���2���ַ�������0ȡ��)��" << endl;
				cin >> name;
				if (name.length() > 10)
				{
					cout << "�Ƿ����룬���������룡" << endl;
				}
				else
				{
					break;
				}
			}
			if (name[0] == '0' && name[1] == '\0')
			{
				return;
			}
			while (userFile.read((char*)&user1, sizeof(user1))) {
				if (user1.getName() == name) {
					// ����loadUser������һ��Ҫ���¼��ظ��û�����ᶪʧ
					// ÿ�ζ�Ҫ�����û��������鼮����
					position = i; // �����û���λ��
					cout << num + 1 << ".�û���Ϣ��" << endl;
					user1.setOverDueNum(loadOverDueNum(user1));
					saveUser(position, user1);
					view->showUser(&user1, cfg->getMaxBorrow());
					showCurBorrow(user1.getId());
					num++;
				}
				i++;
			}
			if (num == 0) {
				cout << "���޴��ˣ�" << endl;
			}
			break;
		case '2': // ��ID��ѯ
			cout << "�����û�ID������0ȡ������" << endl;
			while (!(cin >> id) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,���������룺" << endl;
			}
			while (userFile.read((char*)&user1, sizeof(user1))) {
				if (user1.getId() == id) {
					position = i;
					cout << "�û���Ϣ��" << endl;
					user1.setOverDueNum(loadOverDueNum(user1));// ÿ�ζ�Ҫ�����û��������鼮����
					saveUser(position, user1);
					view->showUser(&user1, cfg->getMaxBorrow());
					showCurBorrow(user1.getId());
					num++;
					break;
				}
				i++;
			}
			if (num == 0) {
				cout << "���޴��ˣ�" << endl;
			}
			break;
		case '0': // ȡ��
			return;
			break;
		default:
			cout << "��������" << endl;
			break;
		}
		userFile.close();
	}
}

void Controller::addUser()
{
	ifstream ifs(USERFILE, ios::binary | ios::in);
	if (!ifs)
		cerr << "�ļ���ʧ�ܣ�" << endl;

	User user1;
	int zone = sizeof(user1); // book1�Ĵ�С��ƫ����
	ifs.seekg(-zone, ios::end);
	ifs.read((char*)&user1, sizeof(user1));
	ifs.close();

	int id = user1.getId() + 1;

	char role1 = '0'; // Ĭ��Ϊ��ͨ�û�
	char state = '0'; // Ĭ������
	char name[11] = { '\0' };
	char password[11] = { '\0' };
	string name1;
	string password1;
	char choice;

	ofstream ofs(USERFILE, ios::binary | ios::app); // ����ģʽ���ļ�

	while (true)
	{
		while (true)
		{
			cout << "�������������Ȳ�����10���ַ���һ�����ֵ���2���ַ�������0ȡ��)��" << endl;
			cin >> name1;
			if (name1.length() > 10)
			{
				cout << "�Ƿ����룬���������룡" << endl;
			}
			else
			{
				break;
			}
		}
		if (name1[0] == '0' && name[1] == '\0')
		{
			return;
		}

		// �Ƿ�Ӧ�����û����������¼ʱ�������룿����
		while (true)
		{ // �����Ժ���Ըĳ��Զ����ɣ�����
			cout << "���������û������루���벻����10���ַ���һ�����ֵ���2���ַ�)��" << endl;
			cin >> password1;
			if (password1.length() > 10)
			{
				cout << "�Ƿ����룬���������룡" << endl;
			}
			else
				break;
		}

		while (true)
		{
			cout << "���������û�����ݣ�0������ͨ�û���1�������Ա)��" << endl;
			while (!(cin >> role1) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			if (role1 != '0' && role1 != '1')
			{
				cout << "�Ƿ����룬���������룡" << endl;
			}
			else
				break;
		}

		cout << "������" << name1 << "\t����" << password1 << "\t�û���ݣ�" << role1 << "\tȷ����Ϣ����" << endl;
		cout << "1---��" << endl;
		cout << "2---��" << endl;
		cout << "0---ȡ������" << endl;

		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,����������:" << endl;
		}

		if (choice == '1' || choice == '0')
		{
			break;
		}
		else
		{
			cout << "����������" << endl;
		}
	}
	if (choice == '1') //����
	{

		for (unsigned i = 0; i < name1.length(); ++i)
		{
			name[i] = name1[i];
		}

		for (unsigned i = 0; i < password1.length(); ++i)
		{
			password[i] = password1[i];
		}
		// Ӧ�������������û��Ĵ����������ɲ�������
		User newUser(id, password, name, 0, role1, '0', 0);

		ofs.write((char*)&newUser, sizeof(newUser));
		cout << "���û���ӳɹ����û���Ϣ���£�" << endl;
		view->showUser(&newUser, cfg->getMaxBorrow());

		ofs.close();
	}
	// choice == '0' �˳�
}

void Controller::deleteUser()
{
	int userId = 0;
	cout << "����Ҫɾ���û���ID������0ȡ������" << endl;
	while (!(cin >> userId) || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "�������ݴ���,����������:" << endl;
	}
	if (userId == 0) {
		return;
	}
	else if (userId == user->getId()) {
		cout << "����ɾ������" << endl;
		return;
	}
	ifstream userFile(USERFILE, ios::binary | ios::in);
	User user1;
	int flag = false; // ���ҵ����û��ı�־
	bool hasBorrowBook = false;
	int i = 0; // ��¼��ȡ��λ��
	int position = 0; // ��¼Ҫɾ���û���λ��
	while (userFile.read((char*)&user1, sizeof(user1))) {
		if (user1.getId() == userId) {
			cout << "���û���Ϣ��" << endl;
			if (user1.getState() == '1') {
				cout << "���û��Ѿ���ɾ��,�����ظ�������" << endl;
				return;
			}
			if (user1.getCurBorrowNum() > 0) {
				cout << "��ǰ�û������ڴ��ڽ����鼮������ɾ����" << endl;
				showCurBorrow(user1.getId());
				return;
			}
			user1.setOverDueNum(loadOverDueNum(user1));
			view->showUser(&user1, cfg->getMaxBorrow());
			showCurBorrow(user1.getId());
			flag = true;
			position = i;
			break;
		}
		i++;
	}
	if (!flag) {
		cout << "���޴��ˣ�" << endl;
		return;
	}
	
	// ��������ͼ���Ƿ�����ʱ����ɣ������ɻ������飬�����������û�м���
	// ����ɾ�����������û�״̬Ϊ˯��
	user1.setState('1');
	saveUser(position, user1);
}

void Controller::addBook()
{
	ifstream ifs(BOOKFILE, ios::binary | ios::in);
	if (!ifs)
		cerr << "�ļ���ʧ�ܣ�" << endl;
	Book book1;
	int zone = sizeof(book1); // book1�Ĵ�С��ƫ����
	ifs.seekg(-zone, ios::end);
	ifs.read((char*)&book1, sizeof(book1));
	ifs.close();

	int id = book1.getId() + 1;
	char state = '0'; // ��ʼ״̬Ĭ��Ϊ�ɽ�
	double price = 0.0;
	char name[21] = { 0 }, author[21] = { 0 };
	string name1, author1;

	ofstream ofs(BOOKFILE, ios::binary | ios::app); // ����ģʽ���ļ�

	while (true)//����Ϣ�ݴ�
	{
		cout << "��������Ҫ��ӵ��鼮�������Ϣ��0-ȡ������" << endl;
		//cout << "������";
		while (true) {
			cout << "����(�ַ����Ȳ�����20��һ�����ֵ��������ַ�)��";
			cin >> name1;
			if (name1.length() > 20)
				cout << "�Ƿ����룬���������룡" << endl;
			else break;
		}
		//cin >> name1;
		if (name1 == "0")//ȡ��
			return;
		//cin.getline( name,10);
		while (true) {
			cout << "����(�ַ����Ȳ�����20��һ�����ֵ��������ַ�)��";
			cin >> author1;
			if (author1.length() > 20)
				cout << "�Ƿ����룬���������룡" << endl;
			else break;
		}
		if (author1 == "0") {
			return;
		}
		while (true) {
			cout << "�۸�0��1000���ڣ���";
			while (!(cin >> price) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			if (price > 0 && price < 1000)
				break;
			else
				cout << "�۸������쳣��" << endl;
		}

		cout << "��ȷ������鼮��Ϣ��1-ȷ��  0-�񣩣�" << endl;
		cout << "������" << name1 << "\t���ߣ�" << author1 << "\t�۸�" << price << endl;
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,���������룺" << endl;
		}
		//cin >> choice;
		if (choice == '1')
			break;
		else {
			cout << "��������" << endl;
		}
	}

	for (unsigned i = 0; i < name1.length(); ++i)
		name[i] = name1[i];
	for (unsigned i = 0; i < author1.length(); ++i)
		author[i] = author1[i];

	// cout << "��ӳɹ���" << endl;
	// cout << "�����鼮��Ϣ��" << endl;
	cout << "����Ҫ��ӵ��鼮��������" << endl;
	int num = 0;
	while (!(cin >> num) || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "�������ݴ���,����������:" << endl;
	}
	for (int i = 0; i < num; ++i) {
		Book book2(id, name, author, state, 0, 0, 0, price);
		id++;
		ofs.write((char*)&book2, sizeof(book2));
		view->showBook(&book2);
	}
	cout << "���ӳɹ���" << endl;
	ofs.close();
}

// �����鼮��Ϣ���Ƿ����Ȿ�飿�Ƿ񱻽裿��
// ���ؽ����¼��Ϣ
// ���ؽ�������Ϣ
// �޸��鼮״̬
// �޸Ľ����¼��Ϣ
// �޸Ľ�������Ϣ
// �����鼮��Ϣ
// ��������¼��Ϣ
// �����������Ϣ
void Controller::modifyBook()
{
	cout << "����Ҫ�޸ĵ��鼮��ID" << endl;
	int bookId;
	while (!(cin >> bookId) || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "�������ݴ���,����������:" << endl;
	}
	
	Book book = loadBook(bookId);
	if (book.getId() == bookId) { // �ҵ�
		//cout << "���鼮��Ϣ��" << endl;
		//view->showBook(&book);
		if (book.getState() == '1') { // �鼮���ڱ�����״̬
			// ���ؽ����¼
			Record record1;
			int recordNum = 0;
			ifstream recordFile(RECORDFILE, ios::binary);
			while (recordFile.read((char*)&record1, sizeof(record1))) {
				// ƥ���Ȿ��ĵ�ǰ���ļ�¼
				if (record1.getBookId() == bookId && record1.getState() == '0') {
					recordNum = 1;
					break;
				}
			}
			recordFile.close();
			if (recordNum == 0) {
				cout << "δ�ҵ����鼮�ĵ�ǰ���ļ�¼" << endl;
				return;
			}
			//cout << "���鼮�ĵ�ǰ��������Ϣ��" << endl;
			//view->showRecord(&record1);
			// �����û���Ϣ
			User user1;
			fstream userFile(USERFILE, ios::binary | ios::in);
			int i = 0;
			int position = 0; // ��¼λ��
			while (userFile.read((char*)&user1, sizeof(user1))) {
				//cout << "�ҽ�����whileѭ����" << endl;
				//cout << "�û�ID��" << user1.getId() << endl;
				if (user1.getId() == record1.getUserId()) {
					// ÿ�ζ�Ҫ�����û��������鼮����
					position = i; // �����û���λ��
					user1.setOverDueNum(loadOverDueNum(user1));
					saveUser(position, user1);
					break;
				}
				i++;
		    }
			userFile.close();
			if (user1.getId() != record1.getUserId()) {
				cout << "δ�ҵ����û���Ϣ" << endl;
				return;
			}
			//cout << "���ĸ��鼮���û�����Ϣ��" << endl;
			//view->showUser(&user1, cfg->getMaxBorrow());
			cout << "�޸�ǰ���鼮��Ϣ" << endl;
			view->showBook(&book);
			cout << "�޸�ǰ�Ľ����û���Ϣ" << endl;
			view->showUser(&user1, cfg->getMaxBorrow());
			cout << "�޸�ǰ�ĵ�ǰ���ļ�¼" << endl;
			view->showRecord(&record1);
			// �޸��鼮״̬��ͼ
			view->modifyBookView();
			char newState;
			while (true) {
				while (!(cin >> newState) || cin.peek() != '\n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "�������ݴ���,���������룺" << endl;
				}
				if (newState == '1') {
					cout << "δ�ı��鼮״̬��" << endl;
					return;
				}
				else if (newState == '0' || newState == '3') { // �����û�����
					
					// �޸��鼮�ļ�
					book.setState(newState); // ���鼮״̬����Ϊ�ɽ�
					saveBook(book);
					// �޸ļ�¼�ļ�
					record1.setState('1'); // �����ļ�¼����Ϊ��ʷ�����¼
					record1.setReturnYear(year);
					record1.setReturnMonth(month);
					record1.setReturnDay(day);
					saveRecord(record1);
					// �޸��û��ļ�
					user1.setCurBorrowNum(user1.getCurBorrowNum() - 1); // ���õ�ǰ��������
					saveUser(position, user1);
					cout << "�޸ĺ���鼮��Ϣ" << endl;
					view->showBook(&book);
					cout << "�޸ĺ�Ľ����û���Ϣ" << endl;
					view->showUser(&user1, cfg->getMaxBorrow());
					cout << "�޸ĺ�Ľ��Ľ��ļ�¼" << endl;
					view->showRecord(&record1);
					return;
					
				}
				else if (newState == '2') { // �鼮��ʧ������ʱ������Ϊ0-0-0
					// �޸��鼮�ļ�
					book.setState('2'); // ���鼮״̬����Ϊ���ɽ趪ʧ
					saveBook(book);
					// �޸ļ�¼�ļ�
					record1.setState('1'); // �����ļ�¼����Ϊ��ʷ�����¼
					record1.setReturnYear(0);
					record1.setReturnMonth(0);
					record1.setReturnDay(0);
					saveRecord(record1);
					// �޸��û��ļ�
					user1.setCurBorrowNum(user1.getCurBorrowNum() - 1); // ���õ�ǰ��������
					saveUser(position, user1);
					cout << "�޸ĺ���鼮��Ϣ" << endl;
					view->showBook(&book);
					cout << "�޸ĺ�Ľ����û���Ϣ" << endl;
					view->showUser(&user1, cfg->getMaxBorrow());
					cout << "�޸ĺ�Ľ��Ľ��ļ�¼" << endl;
					view->showRecord(&record1);
					return;
				}
				else if (newState == '4') {
					cout << "���鼮�������У�����ɾ����" << endl;
					return;
				}
				else {
					cout << "�������ݴ�������������" << endl;
				}
			}
		}
		else if (book.getState() == '0' || book.getState() == '2' || book.getState() == '3'){ // �鼮���ڿɽ衢��ʧ����״̬��ֻ���޸��鼮�ļ�
			cout << "�޸�ǰ���鼮״̬���£�" << endl;
			view->showBook(&book);
			// �޸��鼮״̬��ͼ
			view->modifyBookView();
			char newState;
			while (true) {
				while (!(cin >> newState) || cin.peek() != '\n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "�������ݴ���,���������룺" << endl;
				}
				if (newState == '1') {
					cout << "�в��ܽ��鼮���鼮״̬��Ϊ�����ģ�" << endl;
					return;
				}
				else if (newState == '0' || newState == '2' || newState == '3' || newState == '4') {
					
					book.setState(newState);
					saveBook(book);
					cout << "�޸ĺ��鼮��Ϣ���£�" << endl;
					view->showBook(&book);
					return;
				}
				else {
					cout << "����������ݣ����������룺" << endl;
				}
			}
		}
		else if (book.getState() == '4') {
			cout << "�����Ѿ���ɾ�������ɽ���״̬�޸ģ�" << endl;
			return;
		}
	}
	else {
		cout << "��ID�鼮�����ڣ�" << endl;
		return;
	}
}


void Controller::addRecord(Record& record)
{
	ofstream ofs(RECORDFILE, ios::binary | ios::app); // ����ģʽ���ļ�
	ofs.write((char*)&record, sizeof(record));
	ofs.close();
}

void Controller::modifyConfiguration()
{
	char choice;
	while (true) {
		view->showConfiguration(cfg); // չʾ���ڵĹ���
		view->modifyConfigurationView(); // չʾ���ܽ���
		int newMaxBorrow = cfg->getMaxBorrow(); // ���ɽ��鼮����
		int newBorrowDay = cfg->getBorrowDay(); // ÿ�οɽ���ʱ��
		int newMaxRenew = cfg->getMaxRenew(); // ����������
		int newGracePeriod = cfg->getGracePeriod(); // ���ڿ�����
		double newMoneyPerDay = cfg->getMoneyPerDay(); // �������ڿ�����ÿ����Ҫ���ɵ�Ǯ���������ڿ����ڣ�
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,����������:" << endl;
		}
		switch (choice)
		{
		case '1':
			cout << "�����µ����ɽ��鼮������" << endl;
			while (!(cin >> newMaxBorrow) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			cfg->setMaxBorrow(newMaxBorrow);
			break;
		case '2':
			cout << "�����µ�ÿ�οɽ���������" << endl;
			while (!(cin >> newBorrowDay) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			cfg->setBorrowDay(newBorrowDay);
			break;
		case '3':
			cout << "�����µ�������������" << endl;
			while (!(cin >> newMaxRenew) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			cfg->setMaxRenew(newMaxRenew);
			break;
		case '4':
			cout << "�����µ����ڿ����ڣ�" << endl;
			while (!(cin >> newGracePeriod) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			cfg->setGracePeriod(newGracePeriod);
			break;
		case '5':
			cout << "�����µĳ������ڿ�����ÿ����Ҫ���ɵ�Ǯ��" << endl;
			while (!(cin >> newMoneyPerDay) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�������ݴ���,����������:" << endl;
			}
			cfg->setMoneyPerDay(newMoneyPerDay);
			break;
		case '0':
			return;
			break;
		default:
			cout << "����������ݣ����������룡" << endl;
			break;
		}
		// ���浱ǰ��������Ϣ
		saveConfiguration(*cfg);
	}
}

void Controller::modifyPassword()
{
	while (true)
	{
		string s;
		while (true) {
			cout << "�����������루���Ȳ��ɳ���10���ַ���һ�����ֵ���2���ַ�������0-ȡ������" << endl;
			cin >> s;
			if (s.length() > 10)
				cout << "�Ƿ����룬���������룡" << endl;
			else if (s == "0") return;
			else break;
		}
		char key1[11] = { '\0' };
		for (unsigned i = 0; i < s.length(); i++) {
			key1[i] = s[i];
		}
		cout << "���ٴ����������룺" << endl;
		char key2[11];
		cin >> key2;
		if (!strcmp(key1, key2))//������������һ��
		{
			user->setPassword(key1);
			cout << "�޸ĳɹ���" << endl;
			saveUser(userPosition, *user);
			break;
		}
		cout << "�������벻һ�£����������룡" << endl;
	}
}

bool Controller::pay(double)
{
	return true;
}

// ����Ľ����Ե�������������ͼ��������borrowDay
void Controller::renew()
{
	if (user->getCurBorrowNum() == 0)//���ڽ����鼮����δ0
	{
		cout << "�����ڽ����鼮��" << endl;
		return;
	}
	showCurBorrow(user->getId());
	
	int bookId;
	int flag = 0;//�Ƿ��ҵ�
	while (true)
	{
		cout << "��������Ҫ������鼮ID��0-ȡ������" << endl;
		while (!(cin >> bookId) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������ݴ���,����������:" << endl;
		}
		if (bookId == 0)//ȡ��
			return;

		// �����鼮id�ڽ����¼�в��ң�Ȼ��ƥ���û�id���Ƿ����ڽ��鼮
		/*
		��Ҫ�������¼����ļ�
		�鼮�Ĺ黹����
		��ǰ���ļ�¼��Ӧ���黹���ڣ��������
		*/
		Record record1 = loadRecord(user->getId(), bookId);
		//cout << record1.getUserId() << " " << record1.getBookId() << " " << record1.getState() << endl;
		//cout << user->getId() << " " << bookId << " " << '0' << endl;
		// ��Ҫ�б��ǲ����Ȿ������û��ĵ�ǰ���ļ�¼
		if (record1.getUserId() == user->getId() && record1.getBookId() == bookId && record1.getState() == '0') {
			// �ж��Ƿ��������鼮���ǵĻ���������
			if (isOverdue(year, month, day, record1.getReturnYear(), record1.getReturnMonth(), record1.getReturnDay()))
			{
				cout << "�����鼮�������裬�뾡�컹�飡" << endl;
			}
			// �ж��Ƿ��Ѿ����ڵ�������������
			else if (record1.getCurRenewNum() >= cfg->getMaxRenew()){
				cout << "�Ѿ��ﵽ�������������������" << endl;
			}
			else {
				// �����µĻ���ʱ��
				Book book = loadBook(bookId);
				// ��δ�����Էŵ�һ�������ĺ�����
				int returnYear = 0;
				int returnMonth = 0;
				int returnDay = 0;
				// ���Ľ����¼��ǣ�浽���ڣ����׳������ݲ�һ�£����С��
				if (record1.getReturnDay() + cfg->getBorrowDay() > dayOfMonth(record1.getReturnYear(), record1.getReturnMonth())) {
					if (record1.getReturnMonth() + 1 > 12) { // ��ݼ�һ
						returnYear = record1.getReturnYear() + 1;
						returnMonth = record1.getReturnMonth() - 11;
						returnDay = record1.getReturnDay() + cfg->getBorrowDay() - dayOfMonth(record1.getReturnYear(), record1.getReturnMonth());
					}
					else {
						returnYear = record1.getReturnYear();
						returnMonth = record1.getReturnMonth() + 1;
						returnDay = record1.getReturnDay() + cfg->getBorrowDay() - dayOfMonth(record1.getReturnYear(), record1.getReturnMonth());
					}
				}
				else {
					returnYear = record1.getReturnYear();
					returnMonth = record1.getReturnMonth();
					returnDay = record1.getReturnDay() + cfg->getBorrowDay();
				}

				book.setYear(returnYear);
				book.setMonth(returnMonth);
				book.setDay(returnDay);
				
				// �����鼮��Ϣ�������˻�������
				saveBook(book);

				// �޸Ľ��ļ�¼���µĹ黹ʱ��ͱ����Ĵ���
				record1.setReturnYear(returnYear);
				record1.setReturnMonth(returnMonth);
				record1.setReturnDay(returnDay);
				record1.setCurRenewNum(record1.getCurRenewNum() + 1);
				saveRecord(record1);

				cout << "����ɹ���" << endl;
			}
		}
		else {
			cout << "δ�ҵ��������ļ�¼" << endl;
		}
	}
}

void Controller::loadUser(int userId)
{
	fstream ifs(USERFILE, ios::binary | ios::in);
	if (!ifs)
		cerr << "�û��ļ���ʧ�ܣ�" << endl;
	User user1;
	int zone = sizeof(user1); // book1�Ĵ�С��ƫ����
	ifs.seekg(-zone, ios::end); // ֱ�Ӷ�λ���ļ�β
	ifs.read((char*)&user1, sizeof(user1));
	ifs.seekg(0, ios::beg); // ��λ���ļ���ͷ

	int lastId = user1.getId(); // �ļ������һ���û���ID
	int curId = 0;
	int i = 0;
	while (curId != lastId) {
		ifs.read((char*)&user1, sizeof(user1));
		if (user1.getId() == userId) {
			userPosition = i; // �û����ļ��е�λ��
			// �����û���Ϣ
			user = new User(user1.getId(), user1.getPassword(), user1.getName(), user1.getCurBorrowNum(), user1.getRole(), user1.getState(), user1.getOverDueNum());
			user->setOverDueNum(loadOverDueNum(*user)); // ��������һ�������鼮������
			saveUser(userPosition, *user); // ÿ�μ��ض�����һ�µ�ǰ�������鼮����
			ifs.close();
			return;
		}
		++i;
		curId = user1.getId();
	}
	return;
}

Book Controller::loadBook(int bookId)
{
	ifstream bookFile(BOOKFILE, ios::binary);
	Book book;
	while (bookFile.read((char*)&book, sizeof(book)))
	{
		if (book.getId() == bookId) {
			bookFile.close();
			return book;
		}
	} 
	bookFile.close();
	Book book1;
	return book1; // δ�ҵ��鼮��Ϣ
}

// ���ص�ǰ���ļ�¼
Record Controller::loadRecord(int userId, int bookId)
{
	Record record1;
	int recordNum = 0;
	ifstream recordFile(RECORDFILE, ios::binary);
	while (recordFile.read((char*)&record1, sizeof(record1))) {
		// �û�Id���鼮Id���ϲ����ǵ�ǰ���ļ�¼
		// cout << record1.getUserId() << " " << record1.getBookId() << " " << record1.getState()  << endl;
		if (record1.getUserId() == userId && record1.getBookId() == bookId && record1.getState() == '0') {
			return record1;
		}
	}
	recordFile.close();
	Record record;
	return record;
}

void Controller::loadConfiguration()
{
	fstream ifs(CONFIGURATIONFILE, ios::binary | ios::in | ios::out);
	Configuration tempCfg;
	if (ifs.read((char*)&tempCfg, sizeof(tempCfg))) {
		cfg = new Configuration(tempCfg.getMaxBorrow(), tempCfg.getBorrowDay(), tempCfg.getMaxRenew(), tempCfg.getGracePeriod(), tempCfg.getMoneyPerDay());
		return;
	}
	cout << "�����ļ�����ʧ�ܣ�" << endl;
	return;
}

void Controller::saveUser(int position, User& user1)
{
	// ���ļ�
	// �ҵ�λ��
	// ����ԭ��¼
	// �ر��ļ�
	fstream userFile(USERFILE, ios::binary | ios::in | ios::out);
	// �ƶ��ļ���дָ�룬seekp���ļ��з���Ϣ��seekg���ļ���ȡ��Ϣ��position�Ǵ�0��ʼ�Ĳ��ü�1
	userFile.seekp(static_cast<std::basic_ostream<char, std::char_traits<char>>::off_type>(sizeof(user1)) * position, ios::beg);
	// д�뵱ǰ�û���Ϣ������֮ǰ����Ϣ,ע���һ�������õ���ָ�룬�ڶ��������õ��������
	userFile.write((char*)&user1, sizeof(user1));

	userFile.close();
}

void Controller::saveBook(Book& book)
{
	long long i = 0; // ����鼮��¼���ļ��е�λ��
	int sign = 0; // �ҵ��ı�־
	Book book1;
	fstream bookFile(BOOKFILE, ios::binary | ios::out | ios::in);
	// ���ҵ��鼮���ļ��е�λ��
	while (bookFile.read((char*)&book1, sizeof(book1)))
	{
		i++;
		if (book1.getId() == book.getId()) {
			sign = 1;
			break;
		}
	}
	if (sign) {
		bookFile.seekg(static_cast<std::basic_istream<char, std::char_traits<char>>::off_type>(sizeof(book)) * (i - 1), ios::beg); // ��ס��������(i - 1)����Ϊ�Ǵ�ͷ���ǵ�
	}
	else {
		// û�ҵ����ƶ����ļ�β���������������𣬻ᣬ��������ʱ��
		cout << "δ�ҵ���Ҫ������鼮��Ϣ��" << endl;
		bookFile.seekg(0, ios::end);
	}
	bookFile.write((char*)&book, sizeof(book));
	bookFile.close();
}

// ֻ�������浱ǰ�����¼����������������ʷ�����¼�����ǿ��԰ѵ�ǰ�����¼����Ϊ��ʷ�����¼
void Controller::saveRecord(Record& record)
{
	long long i = 0; // ��Ǽ�¼���ļ��е�λ��
	int sign = 0;
	Record record1;
	fstream recordFile(RECORDFILE, ios::binary | ios::out | ios::in);
	while (recordFile.read((char*)&record1, sizeof(record))) {
		// ���ҵ�ǰ�����¼��ƥ���û�id���鼮id�Լ��鼮�Ľ���״̬
		i++;// ��������λ�ò��Ի����
		if (record1.getUserId() == record.getUserId() && record1.getBookId() == record.getBookId() && record1.getState() == '0')
		{
			sign = 1;
			break;
		}

	}
	if (sign) {
		recordFile.seekp(static_cast<std::basic_ostream<char, std::char_traits<char>>::off_type>(sizeof(record1)) * (i - 1), ios::beg);
	}
	else { // û���ҵ���¼������һ���µļ�¼
		recordFile.seekg(0, ios::end);
	}
	recordFile.write((char*)&record, sizeof(record));
	recordFile.close();
}

void Controller::saveConfiguration(Configuration& cfg1)
{
	fstream ifs(CONFIGURATIONFILE, ios::binary | ios::trunc);
	ifs.write((char*)&cfg1, sizeof(cfg1));
	ifs.close();
}
