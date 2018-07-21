#pragma once
#ifndef SINGLETON
#define SINGLETON


class Singleton
{
public:
	int value;
	static Singleton * getInstance();
	static Singleton *uniqueInstance;

private:
	~Singleton() {}

	Singleton()
	{
		value = 0;
	}
};
Singleton *Singleton::uniqueInstance = 0;

Singleton * Singleton::getInstance()
{
	if (uniqueInstance == 0)
	{
		uniqueInstance = new Singleton();
	}
	return uniqueInstance;
}

#endif // !Singleton