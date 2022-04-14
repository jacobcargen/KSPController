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
//const int SF_A_1_LED;
//const int SF_A_2_LED;
//const int SF_A_3_LED;
//const int SF_A_4_LED;
//const int SF_A_5_LED;
//const int SF_A_6_LED;
//const int SF_A_7_LED;
//const int SF_A_8_LED;
//const int SF_A_9_LED;
//const int SF_A_10_LED;
//const int SF_B_1_LED;
//const int SF_B_2_LED;
//const int SF_B_3_LED;
//const int SF_B_4_LED;
//const int SF_B_5_LED;
//const int SF_B_6_LED;
//const int SF_B_7_LED;
//const int SF_B_8_LED;
//const int SF_B_9_LED;
//const int SF_B_10_LED;
// LF
//const int LF_A_1_LED;
//const int LF_A_2_LED;
//const int LF_A_3_LED;
//const int LF_A_4_LED;
//const int LF_A_5_LED;
//const int LF_A_6_LED;
//const int LF_A_7_LED;
//const int LF_A_8_LED;
//const int LF_A_9_LED;
//const int LF_A_10_LED;
//const int LF_B_1_LED;
//const int LF_B_2_LED;
//const int LF_B_3_LED;
//const int LF_B_4_LED;
//const int LF_B_5_LED;
//const int LF_B_6_LED;
//const int LF_B_7_LED;
//const int LF_B_8_LED;
//const int LF_B_9_LED;
//const int LF_B_10_LED;
// OX
//const int OX_A_1_LED;
//const int OX_A_2_LED;
//const int OX_A_3_LED;
//const int OX_A_4_LED;
//const int OX_A_5_LED;
//const int OX_A_6_LED;
//const int OX_A_7_LED;
//const int OX_A_8_LED;
//const int OX_A_9_LED;
//const int OX_A_10_LED;
//const int OX_B_1_LED;
//const int OX_B_2_LED;
//const int OX_B_3_LED;
//const int OX_B_4_LED;
//const int OX_B_5_LED;
//const int OX_B_6_LED;
//const int OX_B_7_LED;
//const int OX_B_8_LED;
//const int OX_B_9_LED;
//const int OX_B_10_LED;
// MP
//const int MP_A_1_LED;
//const int MP_A_2_LED;
//const int MP_A_3_LED;
//const int MP_A_4_LED;
//const int MP_A_5_LED;
//const int MP_A_6_LED;
//const int MP_A_7_LED;
//const int MP_A_8_LED;
//const int MP_A_9_LED;
//const int MP_A_10_LED;
//const int MP_B_1_LED;
//const int MP_B_2_LED;
//const int MP_B_3_LED;
//const int MP_B_4_LED;
//const int MP_B_5_LED;
//const int MP_B_6_LED;
//const int MP_B_7_LED;
//const int MP_B_8_LED;
//const int MP_B_9_LED;
//const int MP_B_10_LED;
// EC
//const int EC_A_1_LED;
//const int EC_A_2_LED;
//const int EC_A_3_LED;
//const int EC_A_4_LED;
//const int EC_A_5_LED;
//const int EC_A_6_LED;
//const int EC_A_7_LED;
//const int EC_A_8_LED;
//const int EC_A_9_LED;
//const int EC_A_10_LED;
//const int EC_B_1_LED;
//const int EC_B_2_LED;
//const int EC_B_3_LED;
//const int EC_B_4_LED;
//const int EC_B_5_LED;
//const int EC_B_6_LED;
//const int EC_B_7_LED;
//const int EC_B_8_LED;
//const int EC_B_9_LED;
//const int EC_B_10_LED;

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

int lcdLine2Delay = 100;

String soi = "";

// Analog inputs
int rotXRaw, rotYRaw, rotZRaw;

// Switch states
bool rotButtonState, stageButtonState = false;

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

void setup()
{
    // initialize the LCD
    headingLCD.begin();

    Serial.begin(115200);
    while (!mySimpit.init());
    mySimpit.inboundHandler(myCallbackHandler);
    registerChannels();
}

void loop()
{
    mySimpit.update();
    recordInputs();
    updateController();

    if (stageButtonState)
    {
        mySimpit.activateAction(STAGE_ACTION);
        stageButtonState = false;
    }

    delay(100);
}

#pragma region Simpit

void registerChannels()
{
    //mySimpit.registerChannel(ALTITUDE_MESSAGE);
    mySimpit.registerChannel(ROTATION_DATA);
    mySimpit.registerChannel(VELOCITY_MESSAGE);
    //mySimpit.registerChannel(SOI_MESSAGE);
}

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
    case TARGETINFO_MESSAGE:
        if (msgSize == sizeof(targetMessage))
        {
            targetMessage tm;
            tm = parseMessage<targetMessage>(msg);
            targetVelocity = tm.velocity;
        }
    case SOI_MESSAGE:
            soi = (char *) msg;
            soi[msgSize] = '\0';
      
            mySimpit.printToKSP("I'm orbitting " + soi, PRINT_TO_SCREEN);
        break;
    }
}

#pragma endregion

void recordInputs()
{
    // Get the raw axis values
    rotXRaw = analogRead(ROTATION_X_AXIS);
    rotYRaw = analogRead(ROTATION_Y_AXIS);
    rotZRaw = analogRead(ROTATION_Z_AXIS);
    // Checks if the button is pressed and sets the result
    rotButtonState = analogRead(ROTATION_BUTTON) > 50 ? true : false;
}

void updateController()
{
    updateHeadingLCD();
    //updateSpeedLCD();
}

void updateHeadingLCD()
{
    String topTxt, botTxt;
    topTxt += soi;

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
    // Print to top line
    headingLCD.setCursor(0, 0);
    headingLCD.print(topTxt);
    // Delay
    delay(lcdLine2Delay);
    // Print to bottom line
    headingLCD.setCursor(0, 1);
    headingLCD.print(botTxt);
}
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
/// <summary>
/// Format numbers for LCD
/// </summary>
/// <param name=""></param>
/// <param name="length"></param>
/// <returns>Returns a formated number for a specific length</returns>
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
