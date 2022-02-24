#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, {20, 30}) == TOO_LOW);
  REQUIRE(inferBreach(36, {20, 30}) == TOO_HIGH);
  REQUIRE(inferBreach(25, {20, 30}) == NO_BREACH);
  REQUIRE(inferBreach(12, {35, 30}) == NOT_POSSIBLE);
  REQUIRE(inferBreach(12, {20, 10}) == NOT_POSSIBLE);
  REQUIRE(inferBreach(20, {20, 20}) == NOT_POSSIBLE);
}
TEST_CASE("infers the breach according to cooling type") {
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, -1.0) == TOO_LOW);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING,  1.0) == NO_BREACH);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 40) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1.0) == TOO_LOW);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING,  1.0) == NO_BREACH);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1.0) == TOO_LOW);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING,  1.0) == NO_BREACH);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(COOLING_TYPE_TEST, 21) == NOT_POSSIBLE);
}
TEST_CASE("checks temperature and alert according to configured parameter"){
  BatteryCharacter battery = {PASSIVE_COOLING, ""};
  REQUIRE(checkAndAlert(TO_CONTROLLER, battery, 100 ) == true);
  REQUIRE(checkAndAlert(TO_CONTROLLER, battery, -1 ) == true);
  REQUIRE(checkAndAlert(TO_EMAIL, battery, 100 ) == true);
  REQUIRE(checkAndAlert(TO_EMAIL, battery, -1 ) == true);
}
