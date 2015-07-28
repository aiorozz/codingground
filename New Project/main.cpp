/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/
#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include <string>
#include <psapi.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "script.h"
#include <string>
#include <ctime>

struct Buttons {
	int Clutch, FirstGear, SecondGear, ThirdGear, FourthGear, Reverse, EnableDisable;
};
Buttons Pressed;

void loadConfig(Buttons& config) {
	ifstream fin("MTconfig.ini");
	string line;
	while (getline(fin, line)) {
		istringstream sin(line.substr(line.find("=") + 1));
		if (line.find("Clutch") != -1)
			sin >> config.Clutch;
		else if (line.find("FirstGear") != -1)
			sin >> config.FirstGear;
		else if (line.find("SecondGear") != -1)
			sin >> config.SecondGear;
		else if (line.find("ThirdGear") != -1)
			sin >> config.ThirdGear;
		else if (line.find("FourthGear") != -1)
			sin >> config.FourthGear;
		else if (line.find("Reverse") != -1)
			sin >> config.Reverse;
		else if (line.find("Enable/Disable") != 1)
			sin >> config.EnableDisable;
	}
}

ifstream ConfigFile("MTconfig.ini");
void LB()
{
	if (ConfigFile)
	{
		loadConfig(Pressed);
	}
}

void max_speed(int entity, float speed)
{
	ENTITY::SET_ENTITY_MAX_SPEED(entity, speed);
}

bool MT = true;
bool Gear1 = true, Gear2, Gear3, Gear4, Reverse;
bool isClutchDown = false;
bool ConfigLoaded = false;
bool ShiftNow;
bool Gear3Launch = false;
void ManualTransmission()
{
	reset_globals();
	while (true)
	{

		if (!ConfigLoaded)
		{
			LB();
			ConfigLoaded = true;
		}

		unsigned int model = GetHash("taxi");

		if (is_control_pressed(Gamepad::Right) || get_key_pressed(Keys::RIGHT))
		{
			MT = !MT;
			MT ? ShowText("Manual Transmission ~g~Enabled~w~!") : ShowText("Manual Transmission ~r~Disabled~w~!");
			WAIT(50);
		}

		if (MT)
		{			
			if (is_ped_in_any_vehicle(player_ped_id()))
			{
				float speed = ENTITY::GET_ENTITY_SPEED(vehid());
				float acceleration = VEHICLE::GET_VEHICLE_ACCELERATION(vehid());
				float FirstMax = 21.0f;
				float SecondMax = 31.0f;
				float ThirdMax = 40.5f;
				if (get_key_pressed(Pressed.Clutch))
				{
					isClutchDown = true;
					CONTROLS::DISABLE_CONTROL_ACTION(2, 71, true);
				}
				else
				{
					isClutchDown = false;
				}

				if (isClutchDown)
				{
					GearSc("Clutch", 0.080, 0.015);
				}
				else
				{
					Gear("Clutch", 0.080, 0.015);
				}

				if (ShiftNow)
				{
					GearSc("Shift", 0.155, 0.015);
				}
				else
				{
					Gear("Shift", 0.155, 0.015);
				}

				if (speed < 1)
				{
					CONTROLS::DISABLE_CONTROL_ACTION(2, 72, true);
				}

				if (!Reverse && get_key_pressed(Keys::S))
				{
					if (speed > 0)
					{
						speed -= 1.2f;
					}
					else if (speed < 1)
					{
						CONTROLS::DISABLE_CONTROL_ACTION(2, 72, true);
					}
				}

				if (Gear1)
				{
					GearSc("1", 0.015, 0.015);
					if (speed > FirstMax)
					{
						float curr = ENTITY::GET_ENTITY_SPEED(vehid());
						max_speed(vehid(), curr);
					}
					else
					{
						max_speed(vehid(), FirstMax);
					}
					if (speed > FirstMax - 1)
					{
						ShiftNow = true;
					}
					else
					{
						ShiftNow = false;
					}
				}
				else if (Gear2)
				{
					GearSc("2", 0.015, 0.015);
					if (speed > SecondMax)
					{
						float curr = ENTITY::GET_ENTITY_SPEED(vehid());
						max_speed(vehid(), curr);
					}
					else
					{
						max_speed(vehid(), SecondMax);
					}
					if (speed > SecondMax - 1)
					{
						ShiftNow = true;
					}
					else
					{
						ShiftNow = false;
					}
				}
				else if (Gear3)
				{
					GearSc("3", 0.015, 0.015);
					if (speed > ThirdMax)
					{
						float curr = ENTITY::GET_ENTITY_SPEED(vehid());
						max_speed(vehid(), curr);
					}
					else
					{
						max_speed(vehid(), ThirdMax);
					}
					if (speed > ThirdMax - 1)
					{
						ShiftNow = true;
					}
					else
					{
						ShiftNow = false;
					}
				}
				else if (Gear4)
				{
					GearSc("4", 0.015, 0.015);
					max_speed(vehid(), 9999);
					ShiftNow = false;
				}
				else if (Reverse)
				{
					GearSc("R", 0.015, 0.015);
					max_speed(vehid(), 8.0f);
					CONTROLS::DISABLE_CONTROL_ACTION(2, 71, true);
					CONTROLS::DISABLE_CONTROL_ACTION(2, 72, true);
					if (get_key_pressed(Keys::W))
					{
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehid(), -4.1f);
					}
				}

				if (isClutchDown)
				{
					if (get_key_pressed(Pressed.FirstGear))
					{
						Gear1 = true;
						Gear2 = false;
						Gear3 = false;
						Gear4 = false;
						Reverse = false;
						WAIT(50);
					}
					else if (get_key_pressed(Pressed.SecondGear))
					{
						Gear1 = false;
						Gear2 = true;
						Gear3 = false;
						Gear4 = false;
						Reverse = false;
						WAIT(50);
					}
					else if (get_key_pressed(Pressed.ThirdGear))
					{
						Gear1 = false;
						Gear2 = false;
						Gear3 = true;
						Gear4 = false;
						Reverse = false;
						WAIT(50);
					}
					else if (get_key_pressed(Pressed.FourthGear))
					{
						Gear1 = false;
						Gear2 = false;
						Gear3 = false;
						Gear4 = true;
						Reverse = false;
						WAIT(50);
					}
					else if (get_key_pressed(Pressed.Reverse))
					{
						Gear1 = false;
						Gear2 = false;
						Gear3 = false;
						Gear4 = false;
						Reverse = true;
						WAIT(50);
					}
				}


				if (Gear2 && speed <= FirstMax - 2.1f)
				{
					if (is_control_pressed(Gamepad::R2) || get_key_pressed(Keys::W))
					{
						VEHICLE::SET_VEHICLE_ENGINE_ON(vehid(), 0, 0);
					}
				}
				else if (Gear3 && speed <= SecondMax - 2.1f)
				{
					if (is_control_pressed(Gamepad::R2) || get_key_pressed(Keys::W))
					{
						VEHICLE::SET_VEHICLE_ENGINE_ON(vehid(), 0, 0);
					}
				}
				else if (Gear4 && speed <= ThirdMax - 2.1f)
				{
					if (is_control_pressed(Gamepad::R2) || get_key_pressed(Keys::W))
					{
						VEHICLE::SET_VEHICLE_ENGINE_ON(vehid(), 0, 0);
					}
				}

				if (!Gear1 && VEHICLE::IS_VEHICLE_STOPPED(vehid()))
				{
					if (is_control_pressed(Gamepad::R2) || get_key_pressed(Keys::W))
					{
						VEHICLE::SET_VEHICLE_ENGINE_ON(vehid(), 0, 0);
					}
				}


			}
		}
		else if (!MT || ENTITY::GET_ENTITY_MODEL(vehid()) == model)
		{
			max_speed(vehid(), 9999);
			Gear1 = true, Gear2 = false, Gear3 = false, Gear4 = false, Reverse = false;
		}
		/*--------------*/
		update_features();
		WAIT(0);
	}
}