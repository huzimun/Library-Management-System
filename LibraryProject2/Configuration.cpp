#include "Configuration.h"


Configuration::Configuration()
{
	// 默认情况如下
	maxBorrow = 30;
	borrowDay = 20;
	maxRenew = 1;
	moneyPerDay = 0.1;
	gracePeriod = 5;
}

int Configuration::getMaxBorrow()
{
	return maxBorrow;
}

int Configuration::getBorrowDay()
{
	return borrowDay;
}

int Configuration::getMaxRenew()
{
	return maxRenew;
}

Configuration::Configuration(int maxBorrow, int borrowDay, int maxRenew, int gracePeriod, double moneyPerDay)
{
	this->borrowDay = borrowDay;
	this->maxBorrow = maxBorrow;
	this->maxRenew = maxRenew;
	this->gracePeriod = gracePeriod;
	this->moneyPerDay = moneyPerDay;
}

void Configuration::setMaxBorrow(int maxBorrow)
{
	this->maxBorrow = maxBorrow;
}

void Configuration::setBorrowDay(int borrowDay)
{
	this->borrowDay = borrowDay;
}

void Configuration::setMaxRenew(int maxRenew)
{
	this->maxRenew = maxRenew;
}

int Configuration::getGracePeriod()
{
	return gracePeriod;
}

double Configuration::getMoneyPerDay()
{
	return moneyPerDay;
}

void Configuration::setGracePeriod(int gracePeriod)
{
	this->gracePeriod = gracePeriod;
}

void Configuration::setMoneyPerDay(double moneyPerDay)
{
	this->moneyPerDay = moneyPerDay;
}
