#include "typewise-alert.h"
#include <stdio.h>
#include <vector>
#include <string>

struct Limits{
  int lowerLimit = 0;
  int upperLimit = 0;
};
struct AlertMessage{
  std::string Warning;
  std::string Description;
};
/*Variable: CoolingLimits
**type: Dynamic array of structures
**description: Put here the lower and upper limits of your cooling systems
**note: lower cannot be greater than upper*/
std::vector<Limits> CoolingLimits =
                                    {{0 , 35},  //PASSIVE_COOLING
                                     {0 , 45},  //HI_ACTIVE_COOLING
                                     {0 , 40}}; //MED_ACTIVE_COOLING

std::vector<void (*)(BreachType)> AlertFunction =
                                                  {&sendToController, //TO_CONTROLLER
                                                   &sendToEmail};     //TO_EMAIL

std::vector<AlertMessage> AlertMessages =
                                    {{"No breach", "NO_BREACH"},                               //NO_BREACH
                                     {"Hi, the temperature is too low", "TOO_LOW"},          //TOO_LOW
                                     {"Hi, the temperature is too high", "TOO_HIGH"},         //TOO_HIGH
                                     {"The breach parameters are not possible", "NOT_POSSIBLE"}}; //NOT_POSSIBLE

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  BreachType breach = NO_BREACH;
  //This cannot be allowed to enter 2 separate if statements
  if((value <= lowerLimit) | (value >= upperLimit)) {
    breach = (value >= upperLimit) ? TOO_HIGH:TOO_LOW;
  }
  if(lowerLimit >= upperLimit) breach = NOT_POSSIBLE;
  return breach;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  Limits myCoolingLimits = CoolingLimits.at(coolingType);
  return inferBreach(temperatureInC, myCoolingLimits.lowerLimit, myCoolingLimits.upperLimit);
}
/*Function:   checkAndAlert
**decription: This is the highest level function, checks breach type, then alerts if necessary*/
bool checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  AlertFunction.at(alertTarget)(breachType);
  return breachType;
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %s\n", header, AlertMessages.at(breachType).Description.c_str());
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  if(breachType) {
      printf("To: %s\n", recepient);
      printf("%s\n", AlertMessages.at(breachType).Warning.c_str());
  }
}
