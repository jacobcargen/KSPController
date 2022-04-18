/*
 Name:		KSPController.ino
 Created:	04/13/2022 21:27:40
 Author:	jacob
*/

#include <PayloadStructs.h>
#include <KerbalSimpitMessageTypes.h>
#include <KerbalSimpit.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Degree character for the LCD
const char degreeChar = 223;

#pragma region Input

//const int POWER_SWITCH;
//const int DEBUG_MODE_SWITCH;
//const int SPEAKER_ENABLE_SWITCH;
// Display Controls
//const int VERTICAL_VELOCITY_SWITCH;
//const int CYCLE_REFERENCE_MODE_BUTTON;
//const int ALT_RADAR_MODE_SWITCH;
//const int STAGE_VIEW_SWITCH;
// Info Modes(1-12)
//const int INFO_O2_N_TIME_OPTION;
//const int INFO_H2O_FOOD_TIME_OPTION;
//const int INFO_EC_STRESS_TIME_OPTION;
//const int INFO_EVAS_RADS_OPTION;
//const int INFO_STORAGE_TRANSFER_RATE_OPTION;
//const int INFO_TEMP_HUMIDITY_OPTION;
//const int INFO_PRESSURE__OPTION;
//const int INFO_EC_PRODUCE_CONSUME_OPTION;
//const int INFO_ORBIT_OPTION;
//const int INFO_MANEUVER_OPTION;
//const int INFO_RENDEVOUS_OPTION;
//const int INFO_EXTRA_OPTION;
// Direction Modes(9 Options)
//const int DIR_MANEUVER_OPTION;
//const int DIR_PROGRADE_OPTION;
//const int DIR_RETROGRADE_OPTION;
//const int DIR_NORMAL_OPTION;
//const int DIR_ANTI_NORMAL_OPTION;
//const int DIR_RADIAL_OPTION;
//const int DIR_ANTI_RADIAL_OPTION;
//const int DIR_TARGET_OPTION;
//const int DIR_ANTI_TARGET_OPTION;
// Warnings
//const int GEE_WARN_BUTTON;
//const int PITCH_WARN_BUTTON;
//const int TEMP_WARN_BUTTON;
//const int WARP_WARN_BUTTON;
//const int BRAKE_WARN_BUTTON;
//const int FUEL_WARN_BUTTON;
//const int VOLT_WARN_BUTTON;
//const int RADS_WARN_BUTTON;
//const int CO2_WARN_BUTTON;
//const int FAULT_WARN_BUTTON;
// Stage & Abort
//const int STAGE_BUTTON;
//const int STAGE_LOCK_SWITCH;
//const int ABORT_BUTTON;
//const int ABORT_LOCK_SWITCH;
// Action Group 1-10
//const int AG1_BUTTON;
//const int AG2_BUTTON;
//const int AG3_BUTTON;
//const int AG4_BUTTON;
//const int AG5_RADIATORS_BUTTON;
//const int AG6_CHUTES_BUTTON;
//const int AG7_LADDERS_BUTTON;
//const int AG8_SOLAR_BUTTON;
//const int AG9_COMMS_BUTTON;
//const int AG10_SCIENCE_BUTTON;
// Control
//const int DOCKING_MODE_SWITCH;
//const int PERCISION_MODE_SWITCH;
//const int GEAR_SWITCH;
//const int LIGHTS_SWITCH;
//const int BRAKE_SWITCH;
//const int SAS_SWITCH;
//const int RCS_SWITCH;
//const int THROTTLE_LOCK_SWITCH;
//const int SET_TRIM_TRANSLATION;
//const int RESET_TRIM_TRANSLATION;
//const int SET_TRIM_ROTATION;
//const int RESET_TRIM_ROTATION;
// EVA
//const int EXTRA_1_BUTTON;
//const int EXTRA_2_BUTTON;
// SAS Modes
//const int SAS_MODE_STABILITY_ASSIST_BUTTON;
//const int SAS_MODE_MANEUVER_BUTTON;
//const int SAS_MODE_PROGRADE_BUTTON;
//const int SAS_MODE_RETROGRADE_BUTTON;
//const int SAS_MODE_NORMAL_BUTTON;
//const int SAS_MODE_ANTI_NORMAL_BUTTON;
//const int SAS_MODE_RADIAL_BUTTON;
//const int SAS_MODE_ANTI_RADIAL_BUTTON;
//const int SAS_MODE_TARGET_BUTTON;
//const int SAS_MODE_ANTI_TARGET_BUTTON;
// Warping & Time Control
//const int WARP_LOCK_SWITCH;
//const int PAUSE_BUTTON;
//const int CANCEL_WARP_BUTTON;
//const int PHYSICS_WARP_SWITCH;
//const int DECREASE_WARP_BUTTON;
//const int INCREASE_WARP_BUTOTN;
// View
//const int CYCLE_FOCUS_BUTTON;
//const int HIDE_UI_SWITCH;
//const int SCREENSHOT_BUTTON;
//const int MAP_FLIGHT_SWITCH;
//const int EXT_IVA_SWITCH;
//const int CYCLE_CAMERA_MODE_BUTTON;
//const int RESET_CAMERA_BUTTON;

#pragma region Analog

// Rotation Joystick X-Axis(Roll)
const int ROTATION_X_AXIS = A0;
// Rotation Joystick Y-Axis(Pitch)
const int ROTATION_Y_AXIS = A1;
// Rotation Joystick Z-Axis(Yaw)
const int ROTATION_Z_AXIS = A2;
// Rotation Joystick Button
const int ROTATION_BUTTON = A3;

// Translation Joystick X-Axis(Left/Right)
const int TRANSLATION_X_AXIS = A4;
// Translation Joystick Y-Axis(Forward/Back)
const int TRANSLATION_Y_AXIS = A5;
// Translation Joystick Z-Axis(Up/Down)
const int TRANSLATION_Z_AXIS = A6;
// Translation Joystick ButtonTRAN
const int TRANSLATION_BUTTON = A7;

// Throttle Axis
const int THROTTLE_AXIS = A8;

#pragma endregion

#pragma endregion

#pragma region Output

// Shift out
const int SHIFT_OUT_A_DATA_PIN = 8;
const int SHIFT_OUT_A_LATCH_PIN = 9;
const int SHIFT_OUT_A_CLOCK_PIN = 10;

//const int POWER_LED;
// Warings
//const int GEE_WARN_LED;
//const int PITCH_WARN_LED;
//const int TEMP_WARN_LED;
//const int WARP_WARN_LED;
//const int BRAKE_WARN_LED;
//const int FUEL_WARN_LED;
//const int VOLT_WARN_LED;
//const int RADS_WARN_LED;
//const int CO2_WARN_LED;
//const int FAULT_WARN_LED;

#pragma region LED_BARS

// SF
bool sfLeds[20];
// LF
bool lfLeds[20];
// OX
bool oxLeds[20];
// MP
bool mpLeds[20];
// EC
bool ecLeds[20];

#pragma endregion

#pragma region Communication

// Heading LCD
LiquidCrystal_I2C headingLCD(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

#pragma endregion

// Stage & Abort
//const int STAGE_LED = 2;
//const int STAGE_LOCK_LED;
//const int ABORT_LED;
//const int ABORT_LOCK_LED;
// Action Group 1-10
//const int AG1_LED;
//const int AG2_LED;
//const int AG3_LED;
//const int AG4_LED;
//const int AG5_RADIATORS_LED;
//const int AG6_CHUTES_LED;
//const int AG7_LADDERS_LED;
//const int AG8_SOLAR_LED;
//const int AG9_COMMS_LED;
//const int AG10_SCIENCE_LED;
// Control
//const int DOCKING_MODE_LED;
//const int PERCISION_MODE_LED;
//const int GEAR_LED;
//const int LIGHTS_LED;
//const int BRAKE_LED;
//const int SAS_LED;
//const int RCS_LED;
//const int THROTTLE_LOCK_LED;
// SAS Modes
//const int SAS_MODE_STABILITY_ASSIST_LED;
//const int SAS_MODE_MANEUVER_LED;
//const int SAS_MODE_PROGRADE_LED;
//const int SAS_MODE_RETROGRADE_LED;
//const int SAS_MODE_NORMAL_LED;
//const int SAS_MODE_ANTI_NORMAL_LED;
//const int SAS_MODE_RADIAL_LED;
//const int SAS_MODE_ANTI_RADIAL_LED;
//const int SAS_MODE_TARGET_LED;
//const int SAS_MODE_ANTI_TARGET_LED;
// Warping & Time Control
//const int WARP_LOCK_LED;

#pragma endregion

#pragma region Simpit

KerbalSimpit mySimpit(Serial);

#pragma endregion

#pragma region KspInfo

// SOI
String soi = "";
// Heading
int heading;
// Pitch
int pitch;
// Roll 
int roll;
// 
int surfaceVelocity;
int orbitalVelocity;
int verticalVelocity;
int targetVelocity;

#pragma endregion


int lcdLine2Delay = 0;


// Analog inputs
int rotXRaw, rotYRaw, rotZRaw;

// Switch states
bool rotButtonState, stageButtonState = false;


#pragma region Methods

///<summary> Initialize the inputs and outputs.</summary>
void initIO()
{

    #pragma region Inputs



    #pragma endregion

    #pragma region Outputs

    // Initialize the LCD
    headingLCD.begin();

    pinMode(SHIFT_OUT_A_LATCH_PIN, OUTPUT);
    pinMode(SHIFT_OUT_A_DATA_PIN, OUTPUT);
    pinMode(SHIFT_OUT_A_CLOCK_PIN, OUTPUT);

    #pragma endregion

}

/// <summary>Records every analog input on the controller.</summary>
void recordAnalogInputs()
{
    // Get the raw axis values
    rotXRaw = analogRead(ROTATION_X_AXIS);
    rotYRaw = analogRead(ROTATION_Y_AXIS);
    rotZRaw = analogRead(ROTATION_Z_AXIS);
    // Checks if the button is pressed and sets the result
    rotButtonState = analogRead(ROTATION_BUTTON) > 50 ? true : false;
}

// Will I need a record input method??

/// <summary>Updates every output on the controller.</summary>
void updateController()
{
    updateHeadingLCD();
    //updateSpeedLCD();
    shiftOutA();
}

#pragma region Simpit

/// <summary>Register all the needed channels for receiving simpit messages.</summary>
void registerChannels()
{
    mySimpit.registerChannel(ALTITUDE_MESSAGE);
    mySimpit.registerChannel(ROTATION_DATA);
    mySimpit.registerChannel(VELOCITY_MESSAGE);
    mySimpit.registerChannel(SOI_MESSAGE);
}

/// <summary>Simpit messages from ksp are received here.</summary>
void myCallbackHandler(byte messageType, byte msg[], byte msgSize)
{
    switch (messageType)
    {
    case ROTATION_DATA:
        if (msgSize == sizeof(vesselPointingMessage))
        {
            vesselPointingMessage vpm;
            vpm = parseMessage<vesselPointingMessage>(msg);
            heading = vpm.heading;
            pitch = vpm.pitch;
            roll = vpm.roll;
        }
        break;
    case VELOCITY_MESSAGE:
        if (msgSize == sizeof(velocityMessage))
        {
            velocityMessage vm;
            vm = parseMessage<velocityMessage>(msg);
            surfaceVelocity = vm.surface;
            orbitalVelocity = vm.orbital;
            verticalVelocity = vm.vertical;
        }
        break;
    case TARGETINFO_MESSAGE:
        if (msgSize == sizeof(targetMessage))
        {
            targetMessage tm;
            tm = parseMessage<targetMessage>(msg);
            targetVelocity = tm.velocity;
        }
        break;
    case SOI_MESSAGE:
        soi = (char*)msg;
        //soi[msgSize] = '\0';
        soi[soi.length()] = '\0';
        mySimpit.printToKSP("SOI:'" + soi + "'", PRINT_TO_SCREEN);
        break;
    }
}

#pragma endregion

#pragma region LCDs

/// <summary>Update the heading LCD.</summary>
void updateHeadingLCD()
{
    String topTxt, botTxt;
    // Calculate gap
    // No SOI names are more than 7 char, which is good because that is the esact amount of room at max on the lcd.
    // SOI txt
    //mySimpit.printToKSP("", PRINT_TO_SCREEN);
    topTxt += calculateGap(soi, 7);
    // Heading txt
    topTxt += " HDG+";
    topTxt += formatNumber(heading, 3, false, false);
    topTxt += degreeChar;
    // Pitch txt
    botTxt += "PTH";
    botTxt += formatNumber(pitch, 3, true, false);
    botTxt += degreeChar;
    // Roll txt
    botTxt += " RLL";
    botTxt += formatNumber(roll, 4, true, true);
    botTxt += degreeChar;
    // Clear LCD
    headingLCD.clear();
    // Delay
    delay(lcdLine2Delay);
    // Print to bottom line
    headingLCD.setCursor(0, 1);
    headingLCD.print(botTxt);
    // Print to top line
    headingLCD.setCursor(0, 0);
    headingLCD.print(topTxt);
}

// Speed lcd (WIP)
/*
void updateSpeedLCD()
{
    String topTxt, botTxt;

    // Reference txt
    topTxt += "REF:";
    topTxt += reference;
    // SPD txt
    botTxt += "SPD ";
    botTxt += formatNumber(spd, 4, true, true);
    botTxt += "m/s";

    // Clear LCD
    speedLCD.clear();
    // Print to top line
    speedLCD.setCursor(0, 0);
    speedLCD.print(topTxt);
    // Delay
    delay(lcdLine2Delay);
    // Print to bottom line
    speedLCD.setCursor(0, 1);
    speedLCD.print(botTxt);
}
*/

#pragma endregion

#pragma region ShiftOut

void shiftOutA()
{
    // Define the output
    unsigned long output = 0;
    // Pins on registers
    int shiftOutAPins[32];
    shiftOutAPins[0] = (int)sfLeds[0]; // A:0
    shiftOutAPins[1] = (int)sfLeds[1]; // A:1
    shiftOutAPins[2] = (int)sfLeds[2]; // A:2
    shiftOutAPins[3] = (int)sfLeds[3]; // A:3
    shiftOutAPins[4] = (int)sfLeds[4]; // A:4
    shiftOutAPins[5] = (int)sfLeds[5]; // A:5
    shiftOutAPins[6] = (int)sfLeds[6]; // A:6
    shiftOutAPins[7] = (int)sfLeds[7]; // A:7
    shiftOutAPins[8] = (int)sfLeds[8]; // B:0
    shiftOutAPins[9] = (int)sfLeds[9]; // B:1
    shiftOutAPins[10] = (int)sfLeds[10]; // B:2
    shiftOutAPins[11] = (int)sfLeds[11]; // B:3
    shiftOutAPins[12] = (int)sfLeds[12]; // B:4
    shiftOutAPins[13] = (int)sfLeds[13]; // B:5
    shiftOutAPins[14] = (int)sfLeds[14]; // B:6
    shiftOutAPins[15] = (int)sfLeds[15]; // B:7
    shiftOutAPins[16] = (int)sfLeds[16]; // C:0
    shiftOutAPins[17] = (int)sfLeds[17]; // C:1
    shiftOutAPins[18] = (int)sfLeds[18]; // C:2
    shiftOutAPins[19] = (int)sfLeds[19]; // C:4
    // For each pin/bit
    for (int pin = 0; pin < 32; pin++)
    {
        // If should be enabled
        if (shiftOutAPins[pin] == 1)
        {
            // Set the value for THIS pin/bit to 1
            bitSet(output, pin);
        }
    }
    // Update the changes
    updateShiftOut(output, 'A');
}

void updateShiftOut(unsigned long pins, char reg, BitOrder order = MSBFIRST)
{
    int data, latch, clock;

    switch (reg)
    {
    case 'A':
        data = SHIFT_OUT_A_DATA_PIN;
        latch = SHIFT_OUT_A_LATCH_PIN;
        clock = SHIFT_OUT_A_CLOCK_PIN;
        break;
        //case 'B':
            //break;
    default:
        return;
    }

    unsigned int leds16 = int(pins);
    unsigned int leds32 = int(pins >> 16);
    byte low16LED = lowByte(leds16);
    byte high16LED = highByte(leds16);
    byte low32LED = lowByte(leds32);
    byte high32LED = highByte(leds32);

    // Disable
    digitalWrite(latch, LOW);
    shiftOut(data, clock, order, high32LED);
    shiftOut(data, clock, order, low32LED);
    shiftOut(data, clock, order, high16LED);
    shiftOut(data, clock, order, low16LED);
    // Enable
    digitalWrite(latch, HIGH);
}

#pragma endregion

#pragma region ShiftIn



#pragma endregion

#pragma region Tools

/// <summary>Format numbers for LCD. Length max = 4.</summary>
/// <returns>Returns a formated number at a specific length.</returns>
String formatNumber(int number, byte lengthReq, bool canBeNegative, bool flipNegative)
{
    int num = abs(number);

    bool isNegative = number < 0 ? true : false;
    if (flipNegative && number != 0) isNegative = !isNegative;

    if (num < 10) lengthReq -= 1;               // 1 characters
    else if (num < 100) lengthReq -= 2;         // 2 characters
    else if (num < 1000) lengthReq -= 3;        // 3 characters
    else if (num < 10000) lengthReq -= 4;       // 4 characters

    String str;
    for (size_t i = 0; i < lengthReq; i++)
    {
        if (canBeNegative)
        {
            if (i == 0 && isNegative) str += "-";
            else if (i == 0 && !isNegative) str += "+";
            else str += "0";
        }
        else str += "0";
    }

    return str + (String)num;
}

/// <summary>Calculate a gap. The txt length cannot be more than the ideal length.</summary>
String calculateGap(String includedTxt, int idealLength)
{
    // Calculate gap
    int gap = idealLength - includedTxt.length();
    if (gap < 0) return "";
    String str;
    for (size_t i = 0; i < gap; i++)
    {
        str += " ";
    }
    return includedTxt + str;
}

#pragma endregion

#pragma endregion

void setup()
{
    initIO();

    Serial.begin(115200);
    while (!mySimpit.init());
    mySimpit.inboundHandler(myCallbackHandler);
    registerChannels();
}

void loop()
{
    mySimpit.update();
    recordAnalogInputs();
    updateController();
}
