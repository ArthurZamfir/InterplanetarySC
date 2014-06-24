#pragma once
#include "Battery.h"
#include "FuelCell.h"
#include "HydrogenTank.h"
#include "OxygenTank.h"
#include "Radiator.h"
#include "ThermalFissionGenerator.h"
#include "Thruster.h"
#include "WaterTank.h"
#include "Link.h"

/*
In dieser Auflistung sind die jeweiligen Zust�nde des Subsystems enthalten.
Je nach dem welchen Zustand das System gerade hat, kann es sich anders verhalten.
Zudem kann damit �berpr�ft werden, ob sich das System Fehlerhaft verh�lt, bzw
eine Warnung ausgibt, z.B. bei einem geringen F�llstand des Wassertanks.
*/
enum OPERATION_MODE{
	ACTIVE,PASSIVE,ACTIVE_WARNING,PASSIVE_WARNING,ACTIVE_ERROR,PASSIVE_ERROR
};