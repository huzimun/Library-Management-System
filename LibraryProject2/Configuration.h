#pragma once

class Configuration
{
private:
	// 配置文件中的信息
	int maxBorrow; // 最多可借书籍数量
	int borrowDay; // 每次可借书时长
	int maxRenew; // 最大续借次数
	int gracePeriod; // 逾期宽限期
	double moneyPerDay; // 超出逾期宽限期每日需要缴纳的钱（包括逾期宽限期）

public:
	Configuration();
	Configuration(int maxBorrow, int borrowDay, int maxRenew, int gracePeriod, double moneyPerDay);
	int getMaxBorrow();
	int getBorrowDay();
	int getMaxRenew();
	int getGracePeriod();
	double getMoneyPerDay();

	void setMoneyPerDay(double moneyPerDay);
	void setMaxBorrow(int maxBorrow);
	void setBorrowDay(int borrowDay);
	void setMaxRenew(int maxRenew);
	void setGracePeriod(int gracePeriod);
};
