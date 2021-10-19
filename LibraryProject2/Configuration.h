#pragma once

class Configuration
{
private:
	// �����ļ��е���Ϣ
	int maxBorrow; // ���ɽ��鼮����
	int borrowDay; // ÿ�οɽ���ʱ��
	int maxRenew; // ����������
	int gracePeriod; // ���ڿ�����
	double moneyPerDay; // �������ڿ�����ÿ����Ҫ���ɵ�Ǯ���������ڿ����ڣ�

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
