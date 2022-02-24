#include "typewise-alert.h"
#include <stdio.h>
#include <vector>



struct AlertMessage{
  std::string Warning;
  std::string Description;
};
/*Variable: CoolingLimits
**type: Dynamic array of structures
**description: Put here the lower and upper limits of your cooling systems
**note: lower cannot be greater than upper*/
const std::vector<Limits> CoolingLimits =
                                    {{0 , 35},  //PASSIVE_COOLING
                                     {0 , 45},  //HI_ACTIVE_COOLING
                                     {0 , 40}, //MED_ACTIVE_COOLING
                                     {40 , 40}}; //COOLING_TYPE_TEST

const std::vector<void (*)(BreachType)> AlertFunction =
                                                  {&sendToController, //TO_CONTROLLER
                                                   &sendToEmail};     //TO_EMAIL

const std::vector<AlertMessage> AlertMessages =
                                    {{"No breach", "NO_BREACH"},                               //NO_BREACH
                                     {"Hi, the temperature is too low", "TOO_LOW"},          //TOO_LOW
                                     {"Hi, the temperature is too high", "TOO_HIGH"},         //TOO_HIGH
                                     {"ERROR: The breach parameters are not possible", "NOT_POSSIBLE"}}; //NOT_POSSIBLE

BreachType inferBreach(double value, Limits Limit) {
  BreachType breach = Limit.checkBreach(value);
  if(Limit.check_Invalidity()) breach = NOT_POSSIBLE;
  return breach;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  Limits myCoolingLimits = CoolingLimits.at(coolingType);
  return inferBreach(temperatureInC, myCoolingLimits);
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
  if(breachType != NO_BREACH) {
      printf("To: %s\n", recepient);
      printf("%s\n", AlertMessages.at(breachType).Warning.c_str());
  }
}

bool Limits::check_Invalidity(void){
  return lowerLimit >= upperLimit;
};
BreachType Limits::checkBreach(double value){
  BreachType breach = NO_BREACH;
  //3 possible outcomes means using 2 conditions
  if((value <= lowerLimit) | (value >= upperLimit)) {
    breach = (value >= upperLimit) ? TOO_HIGH:TOO_LOW;
  }
  return breach;
};
