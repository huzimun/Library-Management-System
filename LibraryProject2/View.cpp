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
	cout << "当前时间: " << year << "-" << month << "-" << day << endl;
	cout << "欢迎来到图书管理系统" << endl;
	cout << "1---用户登录" << endl;
	cout << "2---游客登录" << endl;
	cout << "0---退出" << endl;
	cout << "输入一个选项" << endl;
}

void View::searchBookView()
{
	cout << "选择查询方式" << endl;
	cout << "1---按书名查找" << endl;
	cout << "2---按作者查找" << endl;
	cout << "3---按书籍ID查找" << endl;
	cout << "4---按书名和作者联合查找" << endl;
	cout << "0---取消" << endl;
	cout << "选择查找类别：" << endl;
}

void View::searchUserView()
{
	cout << "请选择查询的方式" << endl;
	cout << "1---按用户名" << endl;
	cout << "2---按用户ID" << endl;
	cout << "0---取消" << endl;
	cout << "输入查找类别" << endl;
}

void View::userView(User* user, int maxBorrow)
{
	cout << "普通用户登录" << endl;
	cout << "最大可借阅书籍数量: " << maxBorrow << "\t当前书籍借阅数量：" << user->getCurBorrowNum() << "\t当前逾期书籍数量：" << user->getOverDueNum() << endl;
	cout << "用户功能列表" << endl;
	cout << "1---查找书籍" << endl;
	cout << "2---借阅书籍" << endl;
	cout << "3---归还书籍" << endl; // 归还书籍时查询是否超期，超出免罚日期缴纳违约金
	cout << "4---书籍续借" << endl;
	cout << "5---显示当前借阅" << endl;
	cout << "6---显示历史借阅" << endl;
	cout << "7---修改登录密码" << endl;
	cout << "8---展示借阅规则" << endl;
	cout << "0---退出登录" << endl;
	cout << "请选择你要进行的操作：" << endl;
}

void View::managerView()
{
	cout << "管理员登录" << endl;
	cout << "管理员功能列表" << endl;
	cout << "1---查看书籍信息" << endl;
	cout << "2---查看用户信息" << endl; // 里面应该设置查看当前借阅和历史借阅的选项跳转
	cout << "3---增加书籍" << endl;
	cout << "4---增加用户" << endl;
	cout << "5---注销用户" << endl;
	cout << "6---修改登录密码" << endl;
	cout << "7---修改书籍状态" << endl;
	cout << "8---修改借阅规则" << endl;
	cout << "0---退出登录" << endl;
	cout << "请选择你要进行的操作：" << endl;
}

void View::showBook(Book* book)
{
	cout << "ID：" << book->getId() << "\t书名：" << book->getBookName() << "\t作者：" << book->getAuthor() << "\t书籍价格：" << book->getPrice() << "\t书籍状态：";
	if (book->getState() == '0') {
		cout << "可借" << endl;
	}
	else if (book->getState() == '1') {
		cout << "不可借，应当归还日期：" << book->getYear() << "-" << book->getMonth() << "-" << book->getDay() << endl;
	}
	else if (book->getState() == '2') {
		cout << "不可借，书籍丢失" << endl;
	}
	else if (book->getState() == '3') {
		cout << "不可借，书籍破损" << endl;
	}
	else if (book->getState() == '4') {
		cout << "不可借，该书籍被删除" << endl;
	}
	else {
		cout << "该书籍状态信息错误" << endl;
	}
}

void View::showUser(User* user, int maxBorrow)
{
	if (user->getRole() == '0') {
		cout << "用户ID：" << user->getId() << "\t用户名：" << user->getName()<< "\t用户密码：" << user->getPassword() << "\t最大可借阅数量：" << maxBorrow << "\t当前借阅数量：" << user->getCurBorrowNum() << "\t逾期书籍数量：" << user->getOverDueNum() << "\t用户身份：普通用户";
	}
	else {
		cout << "用户ID：" << user->getId() << "\t用户名：" << user->getName() << "\t用户密码：" << user->getPassword()  << "\t用户身份：管理员";
	}
	if (user->getState() == '0') {
		cout << "\t账号状态：已激活" << endl;
	}
	else {
		cout << "\t账号状态：停用中" << endl;
	}
}

void View::modifyConfigurationView()
{
	cout << "可以修改的规则如下：" << endl;
	cout << "1---修改最多可借书籍数量" << endl;
	cout << "2---修改每次可借书天数" << endl;
	cout << "3---修改最大续借次数" << endl;
	cout << "4---修改逾期宽限期" << endl;
	cout << "5---修改超出逾期宽限期每日需要缴纳的钱" << endl;
	cout << "0---取消" << endl;
	cout << "输入你要修改的规则：" << endl;
}

void View::showConfiguration(Configuration* cfg)
{
	cout << "最多可借书籍数量：" << cfg->getMaxBorrow() << "\t每次可借书天数：" << cfg->getBorrowDay() << "\t最大续借天数：" << cfg->getMaxRenew() << "\t逾期宽限期：" << cfg->getGracePeriod() << "\t超过逾期宽限期后每日罚款：" << cfg->getMoneyPerDay() << endl;
}

void View::modifyBookView()
{
	cout << "书籍状态如下：" << endl;
	cout << "0---可借阅" << endl;
	cout << "1---不可借，被借阅" << endl;
	cout << "2---不可借，书籍丢失" << endl;
	cout << "3---不可借，书籍破损" << endl;
	cout << "4---不可借，书籍已被删除" << endl;
	cout << "不可以进行如下操作：" << endl;
	cout << "1. 修改已经被删除书籍的状态" << endl;
	cout << "2. 删除正在被借阅的书籍" << endl;
	cout << "3. 将书籍状态设置为被借阅" << endl;
	cout << "4. 讲书籍状态设置为以上5种以外的状态" << endl;
	cout << "输入新的书籍状态（输入当前的书籍状态退出）：" << endl;
}

void View::showRecord(Record* record1)
{
	cout <<"用户ID：" <<record1->getUserId() << "\t书籍ID：" << record1->getBookId() << "\t书名：" << record1->getBookName()
		<< "\t借书日期：" << record1->getBorrowYear() << "-" << record1->getBorrowMonth() << "-" << record1->getBorrowDay()
		<< "\t还书日期：" << record1->getReturnYear() << "-" << record1->getReturnMonth() << "-" << record1->getReturnDay() << endl;
}
