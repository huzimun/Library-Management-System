#include "Controller.h"


Controller::Controller()
{
	view = NULL;
	user = NULL;
	userPosition = -1;
	year = 0;
	month = 0;
	day = 0;
	// 初始化配置信息
	cfg = NULL;
}

Controller::~Controller()
{
	delete user;
	delete view;
	delete cfg;
}

// 设置系统当前时间，这个函数可以放到Controller构造函数里呀
void Controller::setTime()
{
	time_t timep;
	struct tm p;

	time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
	localtime_s(&p, &timep);//用localtime将秒数转化为struct tm结构体

	year = 1900 + p.tm_year;
	month = 1 + p.tm_mon;
	day = p.tm_mday;
}

int Controller::dayOfMonth(int year, int month)
{
	// 判断闰年
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
	setTime();	// 设置系统当前时间
	loadConfiguration();
	if (cfg == NULL) { // 加载失败后使用默认的配置信息
		cout << "使用的是默认规则！" << endl;
		cfg = new Configuration();
	}
	view = new View(year, month, day); // 配置信息属性必须要在这之前加载

	string choice; // 身份选择
	while (true) {
		view->startView();
		cin >> choice;

		if (choice == "1") { // 用户
			if (login()) { // 登录成功
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
		else if (choice == "2") { // 游客访问，只开放了查找书籍的接口给游客
			
			system("pause");
			system("cls");
			cout << "游客只能查找书籍，如要借书请先登录" << endl;
			while (searchBook()) {

			}
			system("pause");
			system("cls");
		}
		else if (choice == "0") {
			cout << "欢迎再次使用" << endl;
			exit(0);
		}
		else {
			cout << "输入错误，请重新选择" << endl;
			system("pause");
			system("cls");
		}
	}
}

int Controller::searchBook() { // 查书
	while (true) {
		view->searchBookView();
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入数据错误,请重新输入:" << endl;
		}
		string bookName = "";
		string author = "";
		int id = -1;
		Book book;

		switch (choice) {
		case '1': // 按书名
			while (true) {
				cout << "输入书名(字符长度不超过20，一个汉字等于两个字符)：";
				cin >> bookName;
				if (bookName.length() > 20)
					cout << "非法输入，请重新输入！" << endl;
				else break;
			}
			if (showBook(bookName, author, 1)) { // 查到了
				return 1;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case '2': // 按作者名
			while (true) {
				cout << "输入作者(字符长度不超过20，一个汉字等于两个字符)：";
				cin >> author;
				if (author.length() > 20)
					cout << "非法输入，请重新输入！" << endl;
				else break;
			}
			if (showBook(bookName, author, 2)) { // 查到了
				return 1;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case '3': // 按ID
			// 这里的代码可以考虑使用loadBook函数代替
			cout << "输入ID：" << endl;
			while (!(cin >> id) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			book = loadBook(id);
			if (book.getId() == id && id != 0) {
				view->showBook(&book); // 打印这条书籍记录
				return 1;
			}
			else {
				cout << "该书籍ID不存在！" << endl;
			}
			break;
		case '4': // 按书名和作者联合查询
			while (true) {
				cout << "输入书名(字符长度不超过20，一个汉字等于两个字符)：";
				cin >> bookName;
				if (bookName.length() > 20)
					cout << "非法输入，请重新输入！" << endl;
				else break;
			}
			while (true) {
				cout << "输入作者(字符长度不超过20，一个汉字等于两个字符)：";
				cin >> author;
				if (author.length() > 20)
					cout << "非法输入，请重新输入！" << endl;
				else break;
			}
			if (showBook(bookName, author, 3)) { // 查到了
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
	int num = 0; // 查到的书籍数量
	// 每次从文件中读取定长数据
	if (choice == 1) { // 按书名查书
		while (bookFile.read((char*)&book, sizeof(book))) {
			string bookName1 = book.getBookName();
			// cout << "按书名查询展示书籍" << book.getBookName() << "ID：" << book.getId() << endl;
			if (bookName1 == bookName) {
				cout << ++num << "-";
				view->showBook(&book);
			}
		}
	}
	else if (choice == 2) { // 按作者查书
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
		cout << "没有找到符合条件的书籍！" << endl;
	}
	return num;
}

// 应该检验用户账号是否处于激活状态
int Controller::login()
{
	while (true) {
		cout << "输入你的ID（输入0退出)：" << endl;
		int id = -1;
		while (!(cin >> id) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');//清除输入缓冲区的当前行
			cout << "输入数据错误,请重新输入!" << endl;
		}
		if (id == 0) { // 退出
			system("pause");
			system("cls");
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return 0;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');//清除输入缓冲区当前行

		loadUser(id); // 加载用户对象，加载失败user是NULL
		if (user == NULL || user->getId() != id) {
			cout << "该用户不存在！" << endl;
			system("pause");
			system("cls");
			return 0;
		}
		else {
			// 用户账户停用后需要激活
			if (user->getState() == '1') {
				cout << "该账户停用，请先激活" << endl;
				system("pause");
				system("cls");
				return 0;
			}
			while (true) {
				cout << "输入你的密码（0退出）：" << endl;
				string password;
				cin >> password;
				if (password == "0") {
					system("pause");
					system("cls");
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					return 0;
				}
				if (password.compare(user->getPassword()) != 0) {
					cout << "密码有误, 请重新输入！" << endl;
				}
				else {
					cout << "登录成功！" << endl;
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
	int overDueNum = 0; // 记录逾期书籍数量
	int borrowNum = 0; // 记录已经遍历到的在借书籍数量
	ifstream recordFile(RECORDFILE, ios::binary);
	while (recordFile.read((char*)&record1, sizeof(record1))) {
		// 该用户的当前借阅记录
		if (record1.getUserId() == user1.getId() && record1.getState() == '0') {
			// 判断是否逾期
			borrowNum++;
			if (isOverdue(year, month, day, record1.getReturnYear(), record1.getReturnMonth(), record1.getReturnDay()))//逾期{
			{
				overDueNum++;
			}
			if (borrowNum == user1.getCurBorrowNum()) { // 已经遍历完该用户所有的当前借阅记录
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
			cout << "输入数据错误,请重新输入:" << endl;
		}
		switch (choice) {
		case '1'://查找书籍
			searchBook();
			break;
		case '2':// 借阅书籍
			if (user->getCurBorrowNum() < cfg->getMaxBorrow()) {
				borrowBook();
			}
			else {
				cout << "你当前的借阅数量已经达到了最大值！" << endl;
			}
			break;
		case '3': // 归还书籍
			returnBook();
			break;
		case '4': // 显示当前借阅
			renew();
			break;
		case '5': // 显示当期借阅
			// showCurBorrow(user->getId());
			showBorrowRecord(user->getId(), '0');
			break; 
		case '6': // 显示历史借阅
			// showPastBorrow(user->getId());
			showBorrowRecord(user->getId(), '1');
			break;
		case '7':  // 修改登录密码
			modifyPassword();
			break;
		case '8': // 展示借阅规则
			view->showConfiguration(cfg);
			break;
		case '0': // 返回上一级
			cout << "退出成功，欢迎再次登录！" << endl;
			system("pause");
			system("cls");
			return;
		default:
			cout << "输入有误" << endl;
			break;
		}
		system("pause");
		system("cls");
	}
}

void Controller::borrowBook()
{
	if (searchBook()) { // 借书之前先查书
		while (true) {
			// 没有容错
			cout << "输入想要借阅的书籍的ID（输入0取消）：" << endl;
			int bookId = 0;
			Book book;
			while (!(cin >> bookId) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			if (bookId != 0) {
				book = loadBook(bookId);
				if (book.getId() == bookId) { // 找到对应书籍
					if (book.getState() == '0') { // 可借
						cout << "确认要借阅该书籍？（输入 1 确定，输入 0 取消）" << endl;
						view->showBook(&book);
						char choice;
						while (true)
						{
							while (!(cin >> choice) || cin.peek() != '\n')
							{
								cin.clear();
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
								cout << "输入数据错误,请重新输入:" << endl;
							}
							if (choice == '1') { // 确认借阅该书籍
								// 这段代码可以放到一个单独的函数里
								int returnYear = 0;
								int returnMonth = 0;
								int returnDay = 0;
								// 更改借书记录，牵涉到日期，容易出现数据不一致，务必小心
								if (day + cfg->getBorrowDay() > dayOfMonth(year, month)) {
									if (month + 1 > 12) { // 年份加一
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

								book.setState('1'); // 修改书籍状态为不可借
								book.setYear(returnYear);
								book.setMonth(returnMonth);
								book.setDay(returnDay);

								// 保存书籍信息，更改了还书日期和书籍状态
								saveBook(book);

								// 增加和保存借书记录
								Record* record = new Record(user->getId(), book.getId(), year, month, day, returnYear, returnMonth, returnDay, '0', 0, book.getBookName());
								addRecord(*record);

								user->setCurBorrowNum(user->getCurBorrowNum() + 1);

								// 保存用户，更改了借书数量
								saveUser(userPosition, *user);

								cout << "借书成功！" << endl;
								break;
							}
							else if (choice == '0') {
								break;
							}
							else {
								cout << "输入错误，请重新输入" << endl;
							}
						}
					}
					else {
						cout << "该书籍不可借，应当归还日期为：" << book.getYear() << "-" << book.getMonth() << "-" << book.getDay() << endl;
					}
				}
			}
			else { // 退出
				return;
			}
		}
	}
}

void Controller::returnBook(){
	if (user->getCurBorrowNum() == 0) {
		cout << "无在借书籍" << endl;
		return;
	}
	showCurBorrow(user->getId());
	while (true) {
		cout << "输入要归还书籍的ID（输入0取消）：" << endl;
		int bookId = 0;
		while (!(cin >> bookId) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入数据错误,请重新输入:" << endl;
		}
		if (bookId == 0) {
			return;
		}
		bool isFind = false; // 找到的标志

		Record record1;
		ifstream recordFile(RECORDFILE, ios::binary);
		while (recordFile.read((char*)&record1, sizeof(record1))) {
			// 用户Id和书籍Id对上，并且是出于借阅状态的（防止历史记录的干扰）
			if (record1.getUserId() == user->getId() && record1.getBookId() == bookId && record1.getState() == '0') {
				isFind = true;
				// 判断是否逾期，如果逾期并且超出逾期
				//判断是否逾期
				int overdue = 0;//逾期天数
				/*
				如何判断逾期？？？
				如何判断没有逾期？？？
				当前时间和应当还书的时间比较
				年份小
				年份一致月份小
				年月一致天数小
				cout << "当前年月日：" << year << "-" << month << "-" << day << "\t应当归还年月日：" << record1.getReturnYear() << "-" << record1.getReturnMonth() << "-" << record1.getReturnDay() << endl;
				cout << (year <= record1.getReturnYear()) << endl;
				cout << (month <= record1.getReturnMonth()) << endl;
				cout << (day <= record1.getReturnDay()) << endl;
				cout << (!(year <= record1.getReturnYear() && month <= record1.getReturnMonth() && day <= record1.getReturnDay())) << endl;
				*/
				if (isOverdue(year, month, day, record1.getReturnYear(), record1.getReturnMonth(), record1.getReturnDay()))//逾期
				{		
					if (year > record1.getReturnYear()) // 逾期跨年
					{
						int tempYear = record1.getReturnYear();
						overdue = dayOfMonth(record1.getReturnYear(), record1.getReturnMonth()) - record1.getReturnDay(); // 借阅当月天数
						for (int j = record1.getReturnMonth() + 1; j < 13; j++) // 借阅当年其他月
							overdue += dayOfMonth(record1.getReturnYear(), j);

						while (true) 
						{
							tempYear++;
							if (tempYear < year)//tempYear为一整年，借阅年和还书年之间的天数
								for (int j = 1; j < 13; j++)
									overdue += dayOfMonth(tempYear, j);
							else //year == tempYear，还书年的逾期天数
							{
								for (int j = 1; j < month; j++)
									overdue += dayOfMonth(year, j);
								overdue += day;
								break;
							}
						}
					}
					if (year == record1.getReturnYear())//逾期当年
					{
						if (record1.getReturnMonth() == month)//逾期当月
							overdue = day - record1.getReturnDay();
						else
						{
							overdue = dayOfMonth(record1.getReturnYear(), record1.getReturnMonth()) - record1.getReturnDay();
							for (int j = record1.getReturnMonth() + 1; j < month; j++)
								overdue += dayOfMonth(record1.getReturnYear(), j);
							overdue += day;
						}
					}
					cout << "逾期" << overdue << "天！" << endl;
					// 在宽限期内还书不罚款，超过宽限期后，从逾期日（含宽限期）算起累计罚款，过期一天罚款moneyPerDay元
					if (overdue > cfg->getGracePeriod()) {
						cout << "应交费" << overdue * cfg->getMoneyPerDay() << "元！" << endl;
						if (pay(overdue * cfg->getMoneyPerDay()))
						{
							system("pause");
							cout << "支付成功！" << endl;
						}
					}
					
				}
				// 修改书籍状态
				Book book = loadBook(bookId);
				book.setDay(0);
				book.setMonth(0);
				book.setYear(0);
				book.setState('0');
				saveBook(book);
				// 修改用户信息
				if (overdue > 0) {
					user->setOverDueNum(user->getOverDueNum() - 1);
				}
				user->setCurBorrowNum(user->getCurBorrowNum() - 1);
				saveUser(userPosition, *user);
				// 修改借阅记录，借书时是新增
				record1.setState('1'); // 设置为历史借书记录
				record1.setReturnYear(year); // 将还书时间设置为当前时间
				record1.setReturnMonth(month);
				record1.setReturnDay(day);
				saveRecord(record1);
				cout << "书籍归还成功！" << endl;
				break;
			}

		}
		recordFile.close();
		if (!isFind) // 没找到
		{
			cout << "该书籍ID与在借书籍不存在！" << endl;
		}
	}
}

// 是否逾期，逾期返回TRUE，
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
		// 检查报错cout << "借阅记录中的用户ID：" << record1.getUserId() << endl;
		if (record1.getUserId() == userId && record1.getState() == '0') {
			
			printf("%d.", num + 1);
			view->showRecord(&record1);
			num++;
		}
	}
	if (num == 0) {
		cout << "该用户无在借书籍" << endl;
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
		// 检查报错cout << "借阅记录中的用户ID：" << record1.getUserId() << endl;
		if (record1.getUserId() == userId && record1.getState() == '1') {

			printf("%d.", num + 1);
			view->showRecord(&record1);
			num++;
		}
		
	}
	if (num == 0) {
		cout << "该用户无历史借阅书籍" << endl;
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
		// 检查报错cout << "借阅记录中的用户ID：" << record1.getUserId() << endl;
		if (record1.getUserId() == userId && record1.getState() == recordState) {

			printf("%d.", num + 1);
			view->showRecord(&record1);
			num++;
		}
	}
	if (num == 0) {
		if (recordState == '1') {
			cout << "该用户无历史借阅书籍" << endl;
		}
		if (recordState == '0') {
			cout << "该用户无当前借阅书籍" << endl;
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
			cout << "输入数据错误,请重新输入：" << endl;
		}

		switch (choice) {
		case '1':// 查看书籍信息
			searchBook();
			break;
		case '2':// 查看用户信息
			searchUser();
			break;
		case '3':// 增加书籍信息
			addBook();
			break;
		case '4': // 增加用户信息
			addUser();
			break;
		case '5': // 注销用户账号
			deleteUser();
			break;
		case '6': // 修改登录密码
			modifyPassword();
			break;
		case '7': // 修改书籍状态信息
			modifyBook();
			break;
		case '8': // 修改借阅规则
			modifyConfiguration();
			break;
		case '0': // 退出
			cout << "退出成功，欢迎再次登录！" << endl;
			system("pause");
			system("cls");
			return;
		default:
			cout << "输入错误，请重新输入！" << endl;
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
			cout << "用户文件打开失败！" << endl;
			userFile.close();
			return;
		}

		User user1;
		string name;
		int id = 0;
		int num = 0; // 同名用户数
		int i = 0; // 当前记录的位置
		int position = 0; // 查找到的用户的位置
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入数据错误,请重新输入：" << endl;
		}

		switch (choice)
		{
		case '1':// 按用户名查询
			while (true)
			{
				cout << "输入姓名（长度不超过10个字符，一个汉字等于2个字符，输入0取消)：" << endl;
				cin >> name;
				if (name.length() > 10)
				{
					cout << "非法输入，请重新输入！" << endl;
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
					// 利用loadUser函数，一定要重新加载该用户否则会丢失
					// 每次都要加载用户的逾期书籍数量
					position = i; // 当期用户的位置
					cout << num + 1 << ".用户信息：" << endl;
					user1.setOverDueNum(loadOverDueNum(user1));
					saveUser(position, user1);
					view->showUser(&user1, cfg->getMaxBorrow());
					showCurBorrow(user1.getId());
					num++;
				}
				i++;
			}
			if (num == 0) {
				cout << "查无此人！" << endl;
			}
			break;
		case '2': // 按ID查询
			cout << "输入用户ID（输入0取消）：" << endl;
			while (!(cin >> id) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入：" << endl;
			}
			while (userFile.read((char*)&user1, sizeof(user1))) {
				if (user1.getId() == id) {
					position = i;
					cout << "用户信息：" << endl;
					user1.setOverDueNum(loadOverDueNum(user1));// 每次都要加载用户的逾期书籍数量
					saveUser(position, user1);
					view->showUser(&user1, cfg->getMaxBorrow());
					showCurBorrow(user1.getId());
					num++;
					break;
				}
				i++;
			}
			if (num == 0) {
				cout << "查无此人！" << endl;
			}
			break;
		case '0': // 取消
			return;
			break;
		default:
			cout << "输入有误！" << endl;
			break;
		}
		userFile.close();
	}
}

void Controller::addUser()
{
	ifstream ifs(USERFILE, ios::binary | ios::in);
	if (!ifs)
		cerr << "文件打开失败！" << endl;

	User user1;
	int zone = sizeof(user1); // book1的大小是偏移量
	ifs.seekg(-zone, ios::end);
	ifs.read((char*)&user1, sizeof(user1));
	ifs.close();

	int id = user1.getId() + 1;

	char role1 = '0'; // 默认为普通用户
	char state = '0'; // 默认在线
	char name[11] = { '\0' };
	char password[11] = { '\0' };
	string name1;
	string password1;
	char choice;

	ofstream ofs(USERFILE, ios::binary | ios::app); // 附加模式打开文件

	while (true)
	{
		while (true)
		{
			cout << "输入姓名（长度不超过10个字符，一个汉字等于2个字符，输入0取消)：" << endl;
			cin >> name1;
			if (name1.length() > 10)
			{
				cout << "非法输入，请重新输入！" << endl;
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

		// 是否应该在用户输入密码登录时隐藏密码？？？
		while (true)
		{ // 密码以后可以改成自动生成？？？
			cout << "输入新增用户的密码（密码不超过10个字符，一个汉字等于2个字符)：" << endl;
			cin >> password1;
			if (password1.length() > 10)
			{
				cout << "非法输入，请重新输入！" << endl;
			}
			else
				break;
		}

		while (true)
		{
			cout << "输入新增用户的身份（0代表普通用户，1代表管理员)：" << endl;
			while (!(cin >> role1) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			if (role1 != '0' && role1 != '1')
			{
				cout << "非法输入，请重新输入！" << endl;
			}
			else
				break;
		}

		cout << "姓名：" << name1 << "\t密码" << password1 << "\t用户身份：" << role1 << "\t确认信息无误？" << endl;
		cout << "1---是" << endl;
		cout << "2---否" << endl;
		cout << "0---取消新增" << endl;

		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入数据错误,请重新输入:" << endl;
		}

		if (choice == '1' || choice == '0')
		{
			break;
		}
		else
		{
			cout << "请重新输入" << endl;
		}
	}
	if (choice == '1') //新增
	{

		for (unsigned i = 0; i < name1.length(); ++i)
		{
			name[i] = name1[i];
		}

		for (unsigned i = 0; i < password1.length(); ++i)
		{
			password[i] = password1[i];
		}
		// 应当有批量增加用户的代码用于生成测试用例
		User newUser(id, password, name, 0, role1, '0', 0);

		ofs.write((char*)&newUser, sizeof(newUser));
		cout << "新用户添加成功，用户信息如下：" << endl;
		view->showUser(&newUser, cfg->getMaxBorrow());

		ofs.close();
	}
	// choice == '0' 退出
}

void Controller::deleteUser()
{
	int userId = 0;
	cout << "输入要删除用户的ID（输入0取消）：" << endl;
	while (!(cin >> userId) || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "输入数据错误,请重新输入:" << endl;
	}
	if (userId == 0) {
		return;
	}
	else if (userId == user->getId()) {
		cout << "不可删除自身！" << endl;
		return;
	}
	ifstream userFile(USERFILE, ios::binary | ios::in);
	User user1;
	int flag = false; // 查找到该用户的标志
	bool hasBorrowBook = false;
	int i = 0; // 记录读取的位置
	int position = 0; // 记录要删除用户的位置
	while (userFile.read((char*)&user1, sizeof(user1))) {
		if (user1.getId() == userId) {
			cout << "该用户信息：" << endl;
			if (user1.getState() == '1') {
				cout << "该用户已经被删除,请勿重复操作！" << endl;
				return;
			}
			if (user1.getCurBorrowNum() > 0) {
				cout << "当前用户还存在存在借阅书籍，不可删除！" << endl;
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
		cout << "查无此人！" << endl;
		return;
	}
	
	// 关于逾期图书的欠款，还书时会缴纳，不缴纳还不上书，所以在这里就没有检验
	// 可以删除就设置其用户状态为睡眠
	user1.setState('1');
	saveUser(position, user1);
}

void Controller::addBook()
{
	ifstream ifs(BOOKFILE, ios::binary | ios::in);
	if (!ifs)
		cerr << "文件打开失败！" << endl;
	Book book1;
	int zone = sizeof(book1); // book1的大小是偏移量
	ifs.seekg(-zone, ios::end);
	ifs.read((char*)&book1, sizeof(book1));
	ifs.close();

	int id = book1.getId() + 1;
	char state = '0'; // 初始状态默认为可借
	double price = 0.0;
	char name[21] = { 0 }, author[21] = { 0 };
	string name1, author1;

	ofstream ofs(BOOKFILE, ios::binary | ios::app); // 附加模式打开文件

	while (true)//书信息容错
	{
		cout << "请输入你要添加的书籍的相关信息（0-取消）：" << endl;
		//cout << "书名：";
		while (true) {
			cout << "书名(字符长度不超过20，一个汉字等于两个字符)：";
			cin >> name1;
			if (name1.length() > 20)
				cout << "非法输入，请重新输入！" << endl;
			else break;
		}
		//cin >> name1;
		if (name1 == "0")//取消
			return;
		//cin.getline( name,10);
		while (true) {
			cout << "作者(字符长度不超过20，一个汉字等于两个字符)：";
			cin >> author1;
			if (author1.length() > 20)
				cout << "非法输入，请重新输入！" << endl;
			else break;
		}
		if (author1 == "0") {
			return;
		}
		while (true) {
			cout << "价格（0到1000以内）：";
			while (!(cin >> price) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			if (price > 0 && price < 1000)
				break;
			else
				cout << "价格数据异常！" << endl;
		}

		cout << "请确认添加书籍信息（1-确认  0-否）：" << endl;
		cout << "书名：" << name1 << "\t作者：" << author1 << "\t价格：" << price << endl;
		char choice;
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入数据错误,请重新输入：" << endl;
		}
		//cin >> choice;
		if (choice == '1')
			break;
		else {
			cout << "数据有误！" << endl;
		}
	}

	for (unsigned i = 0; i < name1.length(); ++i)
		name[i] = name1[i];
	for (unsigned i = 0; i < author1.length(); ++i)
		author[i] = author1[i];

	// cout << "添加成功！" << endl;
	// cout << "新增书籍信息：" << endl;
	cout << "输入要添加的书籍的数量：" << endl;
	int num = 0;
	while (!(cin >> num) || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "输入数据错误,请重新输入:" << endl;
	}
	for (int i = 0; i < num; ++i) {
		Book book2(id, name, author, state, 0, 0, 0, price);
		id++;
		ofs.write((char*)&book2, sizeof(book2));
		view->showBook(&book2);
	}
	cout << "增加成功！" << endl;
	ofs.close();
}

// 加载书籍信息（是否有这本书？是否被借？）
// 加载借书记录信息
// 加载借书人信息
// 修改书籍状态
// 修改借书记录信息
// 修改借书人信息
// 保存书籍信息
// 保存借书记录信息
// 保存借书人信息
void Controller::modifyBook()
{
	cout << "输入要修改的书籍的ID" << endl;
	int bookId;
	while (!(cin >> bookId) || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "输入数据错误,请重新输入:" << endl;
	}
	
	Book book = loadBook(bookId);
	if (book.getId() == bookId) { // 找到
		//cout << "该书籍信息：" << endl;
		//view->showBook(&book);
		if (book.getState() == '1') { // 书籍处于被借阅状态
			// 加载借书记录
			Record record1;
			int recordNum = 0;
			ifstream recordFile(RECORDFILE, ios::binary);
			while (recordFile.read((char*)&record1, sizeof(record1))) {
				// 匹配这本书的当前借阅记录
				if (record1.getBookId() == bookId && record1.getState() == '0') {
					recordNum = 1;
					break;
				}
			}
			recordFile.close();
			if (recordNum == 0) {
				cout << "未找到该书籍的当前借阅记录" << endl;
				return;
			}
			//cout << "该书籍的当前被借阅信息：" << endl;
			//view->showRecord(&record1);
			// 加载用户信息
			User user1;
			fstream userFile(USERFILE, ios::binary | ios::in);
			int i = 0;
			int position = 0; // 记录位置
			while (userFile.read((char*)&user1, sizeof(user1))) {
				//cout << "我进入了while循环！" << endl;
				//cout << "用户ID：" << user1.getId() << endl;
				if (user1.getId() == record1.getUserId()) {
					// 每次都要加载用户的逾期书籍数量
					position = i; // 当期用户的位置
					user1.setOverDueNum(loadOverDueNum(user1));
					saveUser(position, user1);
					break;
				}
				i++;
		    }
			userFile.close();
			if (user1.getId() != record1.getUserId()) {
				cout << "未找到该用户信息" << endl;
				return;
			}
			//cout << "借阅该书籍的用户的信息：" << endl;
			//view->showUser(&user1, cfg->getMaxBorrow());
			cout << "修改前的书籍信息" << endl;
			view->showBook(&book);
			cout << "修改前的借书用户信息" << endl;
			view->showUser(&user1, cfg->getMaxBorrow());
			cout << "修改前的当前借阅记录" << endl;
			view->showRecord(&record1);
			// 修改书籍状态视图
			view->modifyBookView();
			char newState;
			while (true) {
				while (!(cin >> newState) || cin.peek() != '\n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "输入数据错误,请重新输入：" << endl;
				}
				if (newState == '1') {
					cout << "未改变书籍状态！" << endl;
					return;
				}
				else if (newState == '0' || newState == '3') { // 帮助用户还书
					
					// 修改书籍文件
					book.setState(newState); // 将书籍状态设置为可借
					saveBook(book);
					// 修改记录文件
					record1.setState('1'); // 将借阅记录设置为历史借书记录
					record1.setReturnYear(year);
					record1.setReturnMonth(month);
					record1.setReturnDay(day);
					saveRecord(record1);
					// 修改用户文件
					user1.setCurBorrowNum(user1.getCurBorrowNum() - 1); // 设置当前借书数量
					saveUser(position, user1);
					cout << "修改后的书籍信息" << endl;
					view->showBook(&book);
					cout << "修改后的借书用户信息" << endl;
					view->showUser(&user1, cfg->getMaxBorrow());
					cout << "修改后的借阅借阅记录" << endl;
					view->showRecord(&record1);
					return;
					
				}
				else if (newState == '2') { // 书籍挂失，还书时间设置为0-0-0
					// 修改书籍文件
					book.setState('2'); // 将书籍状态设置为不可借丢失
					saveBook(book);
					// 修改记录文件
					record1.setState('1'); // 将借阅记录设置为历史借书记录
					record1.setReturnYear(0);
					record1.setReturnMonth(0);
					record1.setReturnDay(0);
					saveRecord(record1);
					// 修改用户文件
					user1.setCurBorrowNum(user1.getCurBorrowNum() - 1); // 设置当前借书数量
					saveUser(position, user1);
					cout << "修改后的书籍信息" << endl;
					view->showBook(&book);
					cout << "修改后的借书用户信息" << endl;
					view->showUser(&user1, cfg->getMaxBorrow());
					cout << "修改后的借阅借阅记录" << endl;
					view->showRecord(&record1);
					return;
				}
				else if (newState == '4') {
					cout << "该书籍被借阅中，不可删除！" << endl;
					return;
				}
				else {
					cout << "输入数据错误，请重新输入" << endl;
				}
			}
		}
		else if (book.getState() == '0' || book.getState() == '2' || book.getState() == '3'){ // 书籍处于可借、丢失、损坏状态，只需修改书籍文件
			cout << "修改前的书籍状态如下：" << endl;
			view->showBook(&book);
			// 修改书籍状态视图
			view->modifyBookView();
			char newState;
			while (true) {
				while (!(cin >> newState) || cin.peek() != '\n')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "输入数据错误,请重新输入：" << endl;
				}
				if (newState == '1') {
					cout << "尚不能将书籍将书籍状态改为被借阅！" << endl;
					return;
				}
				else if (newState == '0' || newState == '2' || newState == '3' || newState == '4') {
					
					book.setState(newState);
					saveBook(book);
					cout << "修改后书籍信息如下：" << endl;
					view->showBook(&book);
					return;
				}
				else {
					cout << "输入错误数据，请重新输入：" << endl;
				}
			}
		}
		else if (book.getState() == '4') {
			cout << "该书已经被删除，不可进行状态修改！" << endl;
			return;
		}
	}
	else {
		cout << "该ID书籍不存在！" << endl;
		return;
	}
}


void Controller::addRecord(Record& record)
{
	ofstream ofs(RECORDFILE, ios::binary | ios::app); // 附加模式打开文件
	ofs.write((char*)&record, sizeof(record));
	ofs.close();
}

void Controller::modifyConfiguration()
{
	char choice;
	while (true) {
		view->showConfiguration(cfg); // 展示当期的规则
		view->modifyConfigurationView(); // 展示功能界面
		int newMaxBorrow = cfg->getMaxBorrow(); // 最多可借书籍数量
		int newBorrowDay = cfg->getBorrowDay(); // 每次可借书时长
		int newMaxRenew = cfg->getMaxRenew(); // 最大续借次数
		int newGracePeriod = cfg->getGracePeriod(); // 逾期宽限期
		double newMoneyPerDay = cfg->getMoneyPerDay(); // 超出逾期宽限期每日需要缴纳的钱（包括逾期宽限期）
		while (!(cin >> choice) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入数据错误,请重新输入:" << endl;
		}
		switch (choice)
		{
		case '1':
			cout << "输入新的最多可借书籍数量：" << endl;
			while (!(cin >> newMaxBorrow) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			cfg->setMaxBorrow(newMaxBorrow);
			break;
		case '2':
			cout << "输入新的每次可借书天数：" << endl;
			while (!(cin >> newBorrowDay) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			cfg->setBorrowDay(newBorrowDay);
			break;
		case '3':
			cout << "输入新的最大续借次数：" << endl;
			while (!(cin >> newMaxRenew) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			cfg->setMaxRenew(newMaxRenew);
			break;
		case '4':
			cout << "输入新的逾期宽限期：" << endl;
			while (!(cin >> newGracePeriod) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			cfg->setGracePeriod(newGracePeriod);
			break;
		case '5':
			cout << "输入新的超出逾期宽限期每日需要缴纳的钱：" << endl;
			while (!(cin >> newMoneyPerDay) || cin.peek() != '\n')
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入数据错误,请重新输入:" << endl;
			}
			cfg->setMoneyPerDay(newMoneyPerDay);
			break;
		case '0':
			return;
			break;
		default:
			cout << "输入错误数据，请重新输入！" << endl;
			break;
		}
		// 保存当前的配置信息
		saveConfiguration(*cfg);
	}
}

void Controller::modifyPassword()
{
	while (true)
	{
		string s;
		while (true) {
			cout << "请输入新密码（长度不可超过10个字符，一个汉字等于2个字符，输入0-取消）：" << endl;
			cin >> s;
			if (s.length() > 10)
				cout << "非法输入，请重新输入！" << endl;
			else if (s == "0") return;
			else break;
		}
		char key1[11] = { '\0' };
		for (unsigned i = 0; i < s.length(); i++) {
			key1[i] = s[i];
		}
		cout << "请再次输入新密码：" << endl;
		char key2[11];
		cin >> key2;
		if (!strcmp(key1, key2))//两次密码输入一致
		{
			user->setPassword(key1);
			cout << "修改成功！" << endl;
			saveUser(userPosition, *user);
			break;
		}
		cout << "两次输入不一致，请重新输入！" << endl;
	}
}

bool Controller::pay(double)
{
	return true;
}

// 续借的借期自到期日算起，所有图书续借期borrowDay
void Controller::renew()
{
	if (user->getCurBorrowNum() == 0)//正在借阅书籍数量未0
	{
		cout << "无正在借阅书籍！" << endl;
		return;
	}
	showCurBorrow(user->getId());
	
	int bookId;
	int flag = 0;//是否找到
	while (true)
	{
		cout << "请输入想要续借的书籍ID（0-取消）：" << endl;
		while (!(cin >> bookId) || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入数据错误,请重新输入:" << endl;
		}
		if (bookId == 0)//取消
			return;

		// 根据书籍id在借书记录中查找，然后匹配用户id，是否是在借书籍
		/*
		需要更改如下几个文件
		书籍的归还日期
		当前借阅记录的应当归还日期，续借次数
		*/
		Record record1 = loadRecord(user->getId(), bookId);
		//cout << record1.getUserId() << " " << record1.getBookId() << " " << record1.getState() << endl;
		//cout << user->getId() << " " << bookId << " " << '0' << endl;
		// 需要判别是不是这本书这个用户的当前借阅记录
		if (record1.getUserId() == user->getId() && record1.getBookId() == bookId && record1.getState() == '0') {
			// 判断是否是逾期书籍，是的话不能续借
			if (isOverdue(year, month, day, record1.getReturnYear(), record1.getReturnMonth(), record1.getReturnDay()))
			{
				cout << "逾期书籍不可续借，请尽快还书！" << endl;
			}
			// 判断是否已经大于等于最大续借次数
			else if (record1.getCurRenewNum() >= cfg->getMaxRenew()){
				cout << "已经达到最大借书次数，不可续借" << endl;
			}
			else {
				// 计算新的还书时间
				Book book = loadBook(bookId);
				// 这段代码可以放到一个单独的函数里
				int returnYear = 0;
				int returnMonth = 0;
				int returnDay = 0;
				// 更改借书记录，牵涉到日期，容易出现数据不一致，务必小心
				if (record1.getReturnDay() + cfg->getBorrowDay() > dayOfMonth(record1.getReturnYear(), record1.getReturnMonth())) {
					if (record1.getReturnMonth() + 1 > 12) { // 年份加一
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
				
				// 保存书籍信息，更改了还书日期
				saveBook(book);

				// 修改借阅记录，新的归还时间和被借阅次数
				record1.setReturnYear(returnYear);
				record1.setReturnMonth(returnMonth);
				record1.setReturnDay(returnDay);
				record1.setCurRenewNum(record1.getCurRenewNum() + 1);
				saveRecord(record1);

				cout << "借书成功！" << endl;
			}
		}
		else {
			cout << "未找到该条借阅记录" << endl;
		}
	}
}

void Controller::loadUser(int userId)
{
	fstream ifs(USERFILE, ios::binary | ios::in);
	if (!ifs)
		cerr << "用户文件打开失败！" << endl;
	User user1;
	int zone = sizeof(user1); // book1的大小是偏移量
	ifs.seekg(-zone, ios::end); // 直接定位到文件尾
	ifs.read((char*)&user1, sizeof(user1));
	ifs.seekg(0, ios::beg); // 定位到文件开头

	int lastId = user1.getId(); // 文件中最后一个用户的ID
	int curId = 0;
	int i = 0;
	while (curId != lastId) {
		ifs.read((char*)&user1, sizeof(user1));
		if (user1.getId() == userId) {
			userPosition = i; // 用户在文件中的位置
			// 加载用户信息
			user = new User(user1.getId(), user1.getPassword(), user1.getName(), user1.getCurBorrowNum(), user1.getRole(), user1.getState(), user1.getOverDueNum());
			user->setOverDueNum(loadOverDueNum(*user)); // 单独加载一下逾期书籍的数量
			saveUser(userPosition, *user); // 每次加载都更新一下当前的逾期书籍数量
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
	return book1; // 未找到书籍信息
}

// 加载当前借阅记录
Record Controller::loadRecord(int userId, int bookId)
{
	Record record1;
	int recordNum = 0;
	ifstream recordFile(RECORDFILE, ios::binary);
	while (recordFile.read((char*)&record1, sizeof(record1))) {
		// 用户Id和书籍Id对上并且是当前借阅记录
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
	cout << "配置文件加载失败！" << endl;
	return;
}

void Controller::saveUser(int position, User& user1)
{
	// 打开文件
	// 找到位置
	// 覆盖原纪录
	// 关闭文件
	fstream userFile(USERFILE, ios::binary | ios::in | ios::out);
	// 移动文件读写指针，seekp往文件中放信息，seekg往文件外取信息，position是从0开始的不用减1
	userFile.seekp(static_cast<std::basic_ostream<char, std::char_traits<char>>::off_type>(sizeof(user1)) * position, ios::beg);
	// 写入当前用户信息，覆盖之前的信息,注意第一个参数用的是指针，第二个参数用的是类对象
	userFile.write((char*)&user1, sizeof(user1));

	userFile.close();
}

void Controller::saveBook(Book& book)
{
	long long i = 0; // 标记书籍记录在文件中的位置
	int sign = 0; // 找到的标志
	Book book1;
	fstream bookFile(BOOKFILE, ios::binary | ios::out | ios::in);
	// 先找到书籍在文件中的位置
	while (bookFile.read((char*)&book1, sizeof(book1)))
	{
		i++;
		if (book1.getId() == book.getId()) {
			sign = 1;
			break;
		}
	}
	if (sign) {
		bookFile.seekg(static_cast<std::basic_istream<char, std::char_traits<char>>::off_type>(sizeof(book)) * (i - 1), ios::beg); // 记住，这里是(i - 1)，因为是从头覆盖的
	}
	else {
		// 没找到就移动到文件尾，这种情况会存在吗，会，添加新书的时候
		cout << "未找到需要保存的书籍信息！" << endl;
		bookFile.seekg(0, ios::end);
	}
	bookFile.write((char*)&book, sizeof(book));
	bookFile.close();
}

// 只用来保存当前借书记录而不能用来保存历史借书记录，但是可以把当前借书记录保存为历史借书记录
void Controller::saveRecord(Record& record)
{
	long long i = 0; // 标记记录在文件中的位置
	int sign = 0;
	Record record1;
	fstream recordFile(RECORDFILE, ios::binary | ios::out | ios::in);
	while (recordFile.read((char*)&record1, sizeof(record))) {
		// 查找当前借书记录，匹配用户id和书籍id以及书籍的借阅状态
		i++;// 该条语句的位置不对会出错
		if (record1.getUserId() == record.getUserId() && record1.getBookId() == record.getBookId() && record1.getState() == '0')
		{
			sign = 1;
			break;
		}

	}
	if (sign) {
		recordFile.seekp(static_cast<std::basic_ostream<char, std::char_traits<char>>::off_type>(sizeof(record1)) * (i - 1), ios::beg);
	}
	else { // 没有找到记录，这是一条新的记录
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
