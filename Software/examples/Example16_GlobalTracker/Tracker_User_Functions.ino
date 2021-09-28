// Artemis Global Tracker: User Functions

#include <pu2clr_mcp23008.h>

bool cutter1_fired = false;
bool cutter2_fired = false;
bool heating_state = false;

/**
 * Operate pin on MCP23008.
 * @param pin pin number (0, ..., 2)
 * @param state new state (true for high, false for low)
 * @param switch_time time to leave pin in changed state (0 = forever)
 * @return false if MCP is not found or pin is in wrong range, else true
 */
bool operateMcp(uint8_t pin, bool state, uint16_t switch_time = 0)
{
  if (pin > 2) {
    debugPrint(F("Wrong cutter GPIO number: "));
    debugPrintln(pin);
    return false;
  }
  #ifdef USE_QWIIC
  MCP mcp(&qwiic);
  #else
  setAGTWirePullups(1); // MCP23008 needs pull-ups
  MCP mcp(&agtWire);
  #endif
  mcp.setup();
  if (!mcp.checkDevice()) {
    Serial.println(F("Couldn't connect to MCP23008, trying again."));
    mcp.setup();
    if (!mcp.checkDevice()) {
      debugPrintln(F("Error connecting to MCP23008."));
      Serial.println(F("Error connecting to MCP23008."));
      return false;
    }
  }
  mcp.gpioWrite(pin, state);
  if (switch_time > 0) {
    delay(switch_time);
    mcp.gpioWrite(pin, !state);
  }
  return true;
}

/**
 * Operate cutter.
 * @param pin MCP23008 pin
 */
bool operateCutter(uint8_t pin)
{
  Serial.print("Activating cutter ");
  Serial.println(pin);
  return operateMcp(pin, true, 15000);
}

/**
 * Operate heating.
 * @param state new state of heating (true = on, false = off)
 */
bool operateHeating(bool state)
{
  if (operateMcp(2, state, 0)) {
    heating_state = state;
    return true;
  }
  else {
    return false;
  }
}

// Add your own code to these functions to return USERVAL's or execute USERFUNC's

/**
 * Function executed when the Tracker receives a "USERFUNC1" (0x58) message field.
 * Used to trigger cutter 1 (MCP23008 GPIO 0).
 */
void USER_FUNC_1()
{
  debugPrintln(F("Executing USERFUNC1..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC1" (0x58) message field
  if (operateCutter(0))
    cutter1_fired = true;
}

/**
 * Function executed when the Tracker receives a "USERFUNC2" (0x59) message field.
 * Used to trigger cutter 2 (MCP23008 GPIO 1).
 */
void USER_FUNC_2()
{
  debugPrintln(F("Executing USERFUNC2..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC2" (0x59) message field
  if (operateCutter(1))
    cutter2_fired = true;
}

/**
 * Function executed when the Tracker receives a "USERFUNC3" (0x5a) message field.
 * Currently unused.
 */
void USER_FUNC_3()
{
  debugPrintln(F("Executing USERFUNC3..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC3" (0x5a) message field
}

/**
 * Function executed when the Tracker receives a "USERFUNC4" (0x5b) message field.
 * Currently unused.
 */
void USER_FUNC_4()
{
  debugPrintln(F("Executing USERFUNC4..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC4" (0x5b) message field
}

/**
 * Function executed when the Tracker receives a "USERFUNC5" (0x5c) message field.
 * Currently unused.
 */
void USER_FUNC_5(uint16_t myVar)
{
  debugPrintln(F("Executing USERFUNC5..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC5" (0x5c) message field
}

/**
 * Function executed when the Tracker receives a "USERFUNC6" (0x5d) message field.
 * Currently unused.
 */
void USER_FUNC_6(uint16_t myVar)
{
  debugPrintln(F("Executing USERFUNC6..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC6" (0x5d) message field
}

/**
 * Function executed when the Tracker receives a "USERFUNC7" (0x5e) message field.
 * Currently unused.
 */
void USER_FUNC_7(uint32_t myVar)
{
  debugPrintln(F("Executing USERFUNC7..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC7" (0x5e) message field
}

/**
 * Function executed when the Tracker receives a "USERFUNC8" (0x5f) message field.
 * Currently unused.
 */
void USER_FUNC_8(uint32_t myVar)
{
  debugPrintln(F("Executing USERFUNC8..."));
  // Add your own code here - it will be executed when the Tracker receives a "USERFUNC8" (0x5f) message field
}

/**
 * Determine "USERVAL1" (0x20) message field.
 * Used for cutter and heating status:
 *   bit 0 (0x01): cutter 1 has fired
 *   bit 1 (0x02): cutter 2 has fired
 *   bit 2 (0x04): heating is currently on
 */
byte USER_VAL_1()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL1" (0x20) message field
  byte retVal = 0; // The return value.
  if (cutter1_fired)
    retVal |= 0x01;
  if (cutter2_fired)
    retVal |= 0x02;
  if (heating_state)
    retVal |= 0x04;
  return (retVal);
}

/**
 * Determine "USERVAL2" (0x21) message field.
 * Currently unused.
 */
byte USER_VAL_2()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL2" (0x21) message field
  byte retVal; // The return value.
  retVal = 2; // Set retVal to 2 for testing - delete this line if you add your own code
  return (retVal);
}

/**
 * Determine "USERVAL3" (0x22) message field.
 * Currently unused.
 */
uint16_t USER_VAL_3()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL3" (0x22) message field
  uint16_t retVal; // The return value.
  retVal = 3; // Set retVal to 3 for testing - delete this line if you add your own code
  return (retVal);
}

/**
 * Determine "USERVAL4" (0x23) message field.
 * Currently unused.
 */
uint16_t USER_VAL_4()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL4" (0x23) message field
  uint16_t retVal; // The return value.
  retVal = 4; // Set retVal to 4 for testing - delete this line if you add your own code
  return (retVal);
}

/**
 * Determine "USERVAL5" (0x24) message field
 */
uint32_t USER_VAL_5()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL5" (0x24) message field
  uint32_t retVal; // The return value.
  retVal = 5; // Set retVal to 5 for testing - delete this line if you add your own code
  return (retVal);
}

/**
 * Determine "USERVAL6" (0x25) message field.
 * Currently unused.
 */
uint32_t USER_VAL_6()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL6" (0x25) message field
  uint32_t retVal; // The return value.
  retVal = 6; // Set retVal to 6 for testing - delete this line if you add your own code
  return (retVal);
}

/**
 * Determine "USERVAL7" (0x26) message field.
 * Currently unused.
 */
float USER_VAL_7()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL7" (0x26) message field
  float retVal; // The return value.
  retVal = 7.0; // Set retVal to 7.0 for testing - delete this line if you add your own code
  return (retVal);
}

/**
 * Determine "USERVAL8" (0x27) message field.
 * Currently unused.
 */
float USER_VAL_8()
{
  // Add your own code here - e.g. read an external sensor. The return value will be sent as a "USERVAL8" (0x27) message field
  float retVal; // The return value.
  retVal = 8.0E8; // Set retVal to 8.0E8 for testing - delete this line if you add your own code
  return (retVal);
}

/**
 * User alarm function executed when a PHT alarm is set off.
 * @param alarmType the type of the alarm (bit mask with values defined in Tracker_Message_Fields.h)
 *
 * Used to trigger cutter 1 on ALARM_LOPRESS and to switch heating on ALARM_LOTEMP / ALARM_HITEMP.
 */
void ALARM_FUNC(uint8_t alarmType)
{
  debugPrintln(F("Executing ALARM_FUNC ..."));
  // Add your own code to be executed when an alarm is set off.
  Serial.println(F("Executing alarm user function."));
  // LOPRESS alarm
  if ( ((alarmType & ALARM_LOPRESS) == ALARM_LOPRESS) && ((myTrackerSettings.FLAGS1 & FLAGS1_LOPRESS) == FLAGS1_LOPRESS) ) { // If it's a LOPRESS alarm and it is enabled
    if (!cutter1_fired) {
      debugPrintln(F("Activating cutter 1."));
      USER_FUNC_1();
    }
  }
  // LOTEMP / HITEMP alarms
  if ((alarmType & ALARM_LOTEMP) == ALARM_LOTEMP) { // If LOTEMP limit has been exceeded
    Serial.println(F("LOTEMP alarm, activating heating."));
    operateHeating(true);
  }
  if ((alarmType & ALARM_HITEMP) == ALARM_HITEMP) { // If HITEMP limit has been exceeded
    Serial.println(F("HITEMP alarm, deactivating heating."));
    operateHeating(false);
  }
}
