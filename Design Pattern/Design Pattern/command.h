#pragma once
#include<string>
#include<iostream>
#include<stack>

using namespace std;

class CLight
{
public:
	void On()
	{
		cout << "Light On" << endl;
	}

	void Off()
	{
		cout << "Light Off" << endl;
	}
};

class CTV
{
public:
	void On()
	{
		cout << "TV on" << endl;
	}

	void Off()
	{
		cout << "TV off" << endl;
	}
};

class CCommand
{
public:
	virtual void execute() = 0;
	virtual void undo() = 0;
};

class CLightOnCommand :public CCommand
{
private:
	CLight* m_poLight;
public:
	CLightOnCommand(CLight* pLight)
	{
		m_poLight = pLight;
	}
	void execute ()
	{
		m_poLight->On();
	}
	void undo()
	{
		m_poLight->Off();
	}
};

class CLightOffCommand :public CCommand
{
private:
	CLight* m_poLight;
public:
	CLightOffCommand(CLight* pLight)
	{
		m_poLight = pLight;
	}
	void execute()
	{
		m_poLight->Off();
	}
	void undo()
	{
		m_poLight->On();
	}
};

class CTVOnCommand :public CCommand
{
private:
	CTV* m_poTV;
public:
	CTVOnCommand(CTV* pTV)
	{
		m_poTV = pTV;
	}
	void execute()
	{
		m_poTV->On();
	}
	void undo()
	{
		m_poTV->Off();
	}
};

class CTVOffCommand :public CCommand
{
private:
	CTV* m_poTV;
public:
	CTVOffCommand(CTV* pTV)
	{
		m_poTV = pTV;
	}
	void execute()
	{
		m_poTV->Off();
	}
	void undo()
	{
		m_poTV->On();
	}
};

class CRemoteControl
{
private:
	CCommand* m_poCommand;
public:
	void SetCommand(CCommand* pCommand)
	{
		m_poCommand = pCommand;
	}

	void CommandExecute()
	{
		m_poCommand->execute();
	}
};