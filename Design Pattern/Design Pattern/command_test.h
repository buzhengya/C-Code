#pragma once
#include "command.h"

void CommandTest()
{
	CLight oLight;
	CTV oTV;
	CLightOnCommand oLightOnCommand(&oLight);
	CLightOffCommand oLightOffCommand(&oLight);
	CTVOnCommand oCTVOnCommand(&oTV);
	CTVOffCommand oCTVOffCommand(&oTV);
	CRemoteControl oRemoteControl;

	oRemoteControl.SetCommand(&oLightOnCommand);
	oRemoteControl.CommandExecute();

	oRemoteControl.SetCommand(&oLightOffCommand);
	oRemoteControl.CommandExecute();

	oRemoteControl.SetCommand(&oCTVOnCommand);
	oRemoteControl.CommandExecute();

	oRemoteControl.SetCommand(&oCTVOffCommand);
	oRemoteControl.CommandExecute();
}