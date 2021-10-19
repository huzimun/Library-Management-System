#pragma once
#include "User.h"
#include "View.h"
#include "Book.h"
#include "Record.h"
#include "Configuration.h"
#include <string>
#include <fstream>

constexpr auto USERFILE = "User.dat"; // 用户文件
constexpr auto BOOKFILE = "Book.dat"; // 书籍文件
constexpr auto RECORDFILE = "Record.dat"; // 当前借阅记录文件
constexpr auto CONFIGURATIONFILE = "Configuration.dat"; // 配置信息文件
 
class Controller
{
	/*
	新增功能：
	1、查询用户历史借书记录
	2、查询书籍历史被借记录，管理员功能
	3、查询书籍当前被谁借，管理员功能
	4、书籍挂失，应当有付款功能
	5、还书超期，应当有付款功能，还有免罚宽限期
	6、管理员更改最大借书数量
	7、管理员更改每次借书时间
	8、管理员更改续借次数
	9、续借功能
	10、书籍的价格属性及相关方法
	11、第四个文件，保存参数配置信息，里面的结构是啥？？？
	如果要实现这个功能，借阅记录就不能作为属性放在Controller中，每次直接读出来？？？
	不对，我们可以采用vector作为一个属性，借阅数量作为大小？？？
	不对，如果这样会导致用户的借阅记录显示不全
	12、联合查询
	13、用户更改密码
	14、管理员修改书籍、学生信息
	15、把用户和书籍的展示界面传入View类中，给View类新添加用户、书籍、配置信息属性
	16、可以增加一个查看借阅规则的选项
	17、检测的时候注意检测负数的情况，非负的数据是改成无符号整数还是加判别？？？
	18、归还书籍时的逾期判别出现了问题
	19、展示书籍记录两个函数可以通过增加一个参数的方式合并
	20、发现一个BUG，登录时没有检测用户ID，已经解决
	21、续借函数可能有BUG，续借次数用的是什么,已经检查，无BUG
	*/
private:
	User* user; // 当前登录用户，用于获取设置关于用户的各项属性
	View* view; // 视图属性，用于调用各个视图界面
	Configuration* cfg; // 配置信息属性，用于获取设置各种配置信息
	int userPosition; // 用户记录在文件中的位置
	int year; // 当前年
	int month; // 当前月
	int day; // 当前日
	
public:
	Controller(); // 构造函数
	~Controller(); // 析构函数
	void setTime(); // 设置当前系统时间
	int dayOfMonth(int year, int month); // 返回对应年和月的天数，牵涉到闰年问题
	void start(); // 开始，身份选择

	int searchBook(); // 查找书籍
	int showBook(string bookName, string author, int choice); // 展示查到的书籍
	int login(); // 用户登录
	int loadOverDueNum(User& user1); // 加载当前逾期书籍数量
	void userMenu(); // 用户功能选择
	void borrowBook(); // 借书
	void returnBook(); // 还书
	bool isOverdue(int curYear, int curMonth, int curDay, int returnYear, int returnMonth, int returnDay); // 判断书籍是否逾期
	
	void showCurBorrow(int userId); // 显示当前借阅
	void showPastBorrow(int userId); //输出历史借书记录信息
	void showBorrowRecord(int userId, char recordState);

	void managerMenu(); // 管理员功能选择
	void searchUser(); // 查找用户

	
	void addUser(); // 添加用户
	void deleteUser(); //注销用户账号（并不会删除用户信息）
	
	void addBook(); // 添加书籍
	void modifyBook(); // 修改书籍信息（设置书籍丢失、破损状态）
	void addRecord(Record& record); // 增加借阅记录
	void modifyConfiguration();     // 修改借阅规则
	void modifyPassword();          // 修改密码
	bool pay(double);              // 支付（支付成功返回true，失败返回false，默认成功）
	void renew();                  // 续借
	
	// 从文件中加载用户、书籍、当前借阅信息、借阅规则信息
	void loadUser(int userId);
	Book loadBook(int bookId);
	Record loadRecord(int userId, int bookId);
	void loadConfiguration();

	// 在文件中保存用户、书籍、当前借阅信息
	void saveUser(int position, User& user1);
	void saveBook(Book& book);
	void saveRecord(Record& record);
	void saveConfiguration(Configuration& cfg1);
};
