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
In dieser Auflistung sind die jeweiligen Zustände des Subsystems enthalten.
Je nach dem welchen Zustand das System gerade hat, kann es sich anders verhalten.
Zudem kann damit überprüft werden, ob sich das System Fehlerhaft verhält, bzw
eine Warnung ausgibt, z.B. bei einem geringen Füllstand des Wassertanks.
*/
enum OPERATION_MODE{
	ACTIVE,PASSIVE,ACTIVE_WARNING,PASSIVE_WARNING,ACTIVE_ERROR,PASSIVE_ERROR
};