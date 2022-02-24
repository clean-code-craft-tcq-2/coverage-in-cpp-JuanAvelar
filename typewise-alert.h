#pragma once
#include <string>
typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING,
  COOLING_TYPE_TEST//should be not possible from wrong parameters
} CoolingType;

typedef enum {
  NO_BREACH,
  TOO_LOW,
  TOO_HIGH,
  NOT_POSSIBLE
} BreachType;

struct Limits{
  int lowerLimit = 0;
  int upperLimit = 0;
  bool check_Invalidity(void);
  BreachType checkBreach(double value);
};

BreachType inferBreach(double value, Limits Limit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

struct BatteryCharacter{
  CoolingType coolingType;
  std::string brand;
} ;


bool checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

void sendToController(BreachType breachType);
void sendToEmail(BreachType breachType);
