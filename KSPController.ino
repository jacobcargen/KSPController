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
// Create insatance of Simpit
KerbalSimpit mySimpit(Serial);

#pragma region Input

const int POWER_SWITCH = 53;
const int DEBUG_MODE_SWITCH = 52;
const int SPEAKER_ENABLE_SWITCH = 51;

// Display Controls
const int STAGE_VIEW_SWITCH = 50;
const int VERTICAL_VELOCITY_SWITCH = 49;
const int CYCLE_REFERENCE_MODE_BUTTON = 48;
const int ALT_RADAR_MODE_SWITCH = 47;

// Info Modes(1-12)
bool infoModes[12];
/*
O2 time left + amt
N time left + amt
H20 time left + amt
Food time left + amt
EC time left + amt
EC consume/produce rate
Stress time left + amt (worst kerbal)
Rads rate + amt (worst kerbal)
Comms rate + storage(Hard drive storage for science)
Science sample capcity
Temp + humidity + pressure
Orbit mode
Maneuver mode
*/

// Direction Modes(9 Options)
bool dirModes[12];
/*
0:Maneuver
1:Prograde
2:Retrograde
3:Normal
4:Anti-Normal
5:Radial
6:Anti-Radial
7:Target
8:Anti-Target
9:Custom0
10:Custom1
11:Custom2
*/

// Warnings (Press to disable warning(New alerts will still trigger))
bool warnButtons[10];
/*
0:GEE
1:PITCH
2:TEMP
3:WARP
4:BRAKE
5:FUEL
6:VOLT
7:RADS
8:CO2
9:FAULT
*/

// Stage & Abort
const int STAGE_BUTTON = 46;
const int ABORT_BUTTON = 45;

// Action Group 1-10
bool agButtons[10];
/*
0:ag1
1:ag2
2:ag3
3:ag4
4:ag5 Radiators
5:ag6 Chutes
6:ag7 Ladders
7:ag8 Solar
8:ag9 Comms
9:ag10 Science
*/

// Control
int dockingModeSwitch;
int percisionModeSwitch;
int gearSwitch;
int lightsSwitch;
int brakeSwitch;
int sasSwitch;
int rcsSwitch;
int throttleLockSwitch;
int setTrimTranslationButton;
int resetTrimTranslationButton;
int setTrimRotationButton;
int resetTrimRotationButton;

// EVA
const int EXTRA_1_BUTTON = 44;
const int EXTRA_2_BUTTON = 43;

// SAS Modes
int sasButtons[10];
/*
0:Stability Assist
1:Maneuver
2:Prograde
3:Retrograde
4:Normal
5:Anti-Normal
6:Radial
7:Anti-Radial
8:Target
9:Anti-Target
*/

// Warping & Time Control
int warpLockSwitch;
int pauseButton;
int cancelWarpButton;
int enablePhysWarpSwitch;
int decreaseWarpButton;
int increaseWarpButton;

// View
int cycleFocusButton;
int hideUIButton;
int screenshotButton;
int mapFlightSwitch;
int extIvaSwitch;
int cycleCamModeButton;
int resetCamButton;

#pragma region AnalogPins

// Rotation Joystick X-Axis(Roll)
const int ROTATION_X_AXIS_PIN = A0;
// Rotation Joystick Y-Axis(Pitch)
const int ROTATION_Y_AXIS_PIN = A1;
// Rotation Joystick Z-Axis(Yaw)
const int ROTATION_Z_AXIS_PIN = A2;
// Rotation Joystick Button
const int ROTATION_BUTTON_PIN = A3;

// Translation Joystick X-Axis(Left/Right)
const int TRANSLATION_X_AXIS_PIN = A4;
// Translation Joystick Y-Axis(Forward/Back)
const int TRANSLATION_Y_AXIS_PIN = A5;
// Translation Joystick Z-Axis(Up/Down)
const int TRANSLATION_Z_AXIS_PIN = A6;
// Translation Joystick ButtonTRAN
const int TRANSLATION_BUTTON_PIN = A7;

// Throttle Axis
const int THROTTLE_AXIS_PIN = A8;

#pragma endregion

// Joystick analog
int rotXRaw, rotYRaw, rotZRaw;
int transXRaw, transYRaw, transZRaw;
// Joystick buttons
bool rotButtonState, transButtonState;
// Throttle analog
int throttleRaw;

#pragma endregion

#pragma region Output

// Shift out A pins
const int SHIFT_OUT_A_DATA_PIN = 8;
const int SHIFT_OUT_A_LATCH_PIN = 9;
const int SHIFT_OUT_A_CLOCK_PIN = 10;

//const int POWER_LED;

// Warings
int warningLeds[10];
/*
0:GEE
1:PITCH
2:TEMP
3:WARP
4:BRAKE
5:FUEL
6:VOLT
7:RADS
8:CO2
9:FAULT
*/

#pragma region LED_BARS // True for on

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
//const int STAGE_LED = ;
//const int ABORT_LED;

// Action Group 1-10
bool agLeds[10];
/*
0:ag1
1:ag2
2:ag3
3:ag4
4:ag5 Radiators
5:ag6 Chutes
6:ag7 Ladders
7:ag8 Solar
8:ag9 Comms
9:ag10 Science
*/

// Control
int dockingModeLed;
int PercisionModeLed;
int gearLed;
int lightsLed;
int brakeLed;
int sasLed;
int rcsLed;

// SAS Modes
int sasModeLeds[10]
/*
0:Stability Assist
1:Maneuver
2:Prograde
3:Retrograde
4:Normal
5:Anti-Normal
6:Radial
7:Anti-Radial
8:Target
9:Anti-Target
*/

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

#pragma endregion

// Time between the top and bottom of the lcd prints
int lcdLineDelay = 0;

// Pins on registers out A
int shiftOutA[64];


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

/// <summary>Records every input on the controller.</summary>
void recordInputs()
{

    #pragma region Analog

    // Get the raw axis values
    rotXRaw = analogRead(ROTATION_X_AXIS_PIN);
    rotYRaw = analogRead(ROTATION_Y_AXIS_PIN);
    rotZRaw = analogRead(ROTATION_Z_AXIS_PIN);
    transXRaw = analogRead(TRANSLATION_X_AXIS_PIN);
    transYRaw = analogRead(TRANSLATION_Y_AXIS_PIN);
    transZRaw = analogRead(TRANSLATION_Z_AXIS_PIN);
    throttleRaw = analogRead(THROTTLE_AXIS_PIN);
    // If button less then 50, then set to true, otherwise set false;
    rotButtonState = analogRead(ROTATION_BUTTON_PIN) > 50 ? false : true;
    transButtonState = analogRead(TRANSLATION_BUTTON_PIN) > 50 ? false : true;

    #pragma endregion

    #pragma region Digital



    #pragma endregion

}

/// <summary>Updates every output on the controller.</summary>
void updateController()
{
    updateHeadingLCD();
    //updateSpeedLCD();
    shiftOutA(shiftOutA[], SHIFT_OUT_A_DATA_PIN, SHIFT_OUT_A_LATCH_PIN, SHIFT_OUT_A_CLOCK_PIN);
    
    
    // Set pin values here
    shiftOutA[0] = (int)sfLeds[0]; // A:0
    shiftOutA[1] = (int)sfLeds[1]; // A:1
    shiftOutA[2] = (int)sfLeds[2]; // A:2
    shiftOutA[3] = (int)sfLeds[3]; // A:3
    shiftOutA[4] = (int)sfLeds[4]; // A:4
    shiftOutA[5] = (int)sfLeds[5]; // A:5
    shiftOutA[6] = (int)sfLeds[6]; // A:6
    shiftOutA[7] = (int)sfLeds[7]; // A:7
    shiftOutA[8] = (int)sfLeds[8]; // B:0
    shiftOutA[9] = (int)sfLeds[9]; // B:1
    shiftOutA[10] = (int)sfLeds[10]; // B:2
    shiftOutA[11] = (int)sfLeds[11]; // B:3
    shiftOutA[12] = (int)sfLeds[12]; // B:4
    shiftOutA[13] = (int)sfLeds[13]; // B:5
    shiftOutA[14] = (int)sfLeds[14]; // B:6
    shiftOutA[15] = (int)sfLeds[15]; // B:7
    shiftOutA[16] = (int)sfLeds[16]; // C:0
    shiftOutA[17] = (int)sfLeds[17]; // C:1
    shiftOutA[18] = (int)sfLeds[18]; // C:2
    shiftOutA[19] = (int)sfLeds[19]; // C:4
}

/// <summary>Send input data to ksp.</summary>
void sendInputs()
{
    sendJoystickValues('A');
    sendJoystickValues('B');
    sendThrottleValues();
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
        default:
            break;
    }
}

#pragma endregion

#pragma region Analog

/// <summary>Send joystick values to ksp.</summary>
void sendJoystickValues(char option)
{
    int x, y, z;
    rotationMessage rotMsg;
    translationMessage transMsg;

    // Smoothing
    //int pitch = smoothAxis(rotYRaw);
    switch (option)
    {
    case 'A':
        x = smoothAxis(rotXRaw);
        y = smoothAxis(rotYRaw);
        z = smoothAxis(rotZRaw);
        break;
    case 'B':
        x = smoothAxis(transXRaw);
        y = smoothAxis(transYRaw);
        z = smoothAxis(transZRaw);
        break;
    default:
        break;
    }
    int16_t xMap = map(x, 0, 1023, INT16_MIN, INT16_MAX);
    int16_t yMap = map(y, 0, 1023, INT16_MIN, INT16_MAX);
    int16_t zMap = map(z, 0, 1023, INT16_MIN, INT16_MAX);
    switch (option)
    {
    case 'A': // Rotation
        xMap *= -1;
        rotMsg.setPitchRollYaw(yMap, xMap, zMap);
        mySimpit.send(ROTATION_MESSAGE, rotMsg);
        break;
    case 'B':
        zMap *= -1;
        transMsg.setXYZ(xMap, zMap, yMap);
        mySimpit.send(TRANSLATION_MESSAGE, transMsg);
        break;
    default:
        break;
    }
}

/// <summary>Send throttle values to ksp.</summary>
void sendThrottleValues()
{
    // Create new throttle msg
    throttleMessage throttleMsg;
    // Smooth the raw input
    int thr = smoothAxis(throttleRaw);
    // Convert the smoothed number
    int16_t thrMap = map(thr, 0, 1023, INT16_MIN, INT16_MAX);
    // Set the throttle value
    throttleMsg.throttle = thrMap;
}

/// <summary>Give the raw analog some smoothing.</summary>
/// <returns>Returns a smoothed value.</returns>
int smoothAxis(int raw)
{
    return raw;
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
    delay(lcdLineDelay);
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

#pragma region ShiftOut

/// <summary>Call to shift 8 registers.</summary>
void shiftOut8Reg(int pins[], int dataPin, int latchPin, int clockPin)
{
    // Define outputA (4bytes)
    unsigned long outputA = 0;
    // Define outputB (4bytes)
    unsigned long outputB = 0;
    // For each pin/bit
    for (int pin = 0; pin < pins.Length(); pin++)
    {
        // First 4 bytes
        if (pin > 31 && pins[pin] == 1)
        {
            // Set the value for THIS pin/bit to 1
            bitSet(outputA, pin);
        }
        // Last 4 bytes
        else if (pins[pin] == 1)
        {
            // Set the value for THIS pin/bit to 1
            bitSet(outputB, pin);
        }
    }
    // Update the changes
    shiftOut8Reg(outputA, outputB, dataPin, latchPin, clockPin);
}

/// <summary>Updates a shift register out group. (MSBFIRST)</summary>
void shiftOut8Reg(unsigned long b0_1_2_3, unsigned long b4_5_6_7, int dataPin, int latchPin, int clockPin)
{
    // Break down the bytes 4-2bytes
    unsigned int b0_1 = int(b0_1_2_3);
    unsigned int b2_3 = int(b0_1_2_3 >> 16)
    unsigned int b4_5 = int(b4_5_6_7);
    unsigned int b6_7 = int(b4_5_6_7 >> 16);
    // Break down the bytes 2-1bytes
    byte b0 = lowByte(b0_1);
    byte b1 = highByte(b0_1);
    byte b2 = lowByte(b2_3);
    byte b3 = highByte(b2_3);
    byte b4 = lowByte(b4_5);
    byte b5 = highByte(b4_5);
    byte b6 = lowByte(b6_7);
    byte b7 = highByte(b6_7);
    // Disable
    digitalWrite(latchPin, LOW);
    // Shift the values into the register starting from the last register
    shiftOut(dataPin, clockPin, MSBFIRST, b7);
    shiftOut(dataPin, clockPin, MSBFIRST, b6);
    shiftOut(dataPin, clockPin, MSBFIRST, b5);
    shiftOut(dataPin, clockPin, MSBFIRST, b4);
    shiftOut(dataPin, clockPin, MSBFIRST, b3);
    shiftOut(dataPin, clockPin, MSBFIRST, b2);
    shiftOut(dataPin, clockPin, MSBFIRST, b1);
    shiftOut(dataPin, clockPin, MSBFIRST, b0);
    // Enable
    digitalWrite(latchPin, HIGH);
}

#pragma endregion

#pragma region ShiftIn



#pragma endregion

#pragma endregion

/// <summary>Called at the start of the program.</summary>
void setup()
{
    // Initialize the i/o
    initIO();
    // Open up the serial port
    Serial.begin(115200);
    // Wait for a connection to ksp
    while (!mySimpit.init());
    // Show that the controller has connected
    mySimpit.printToKSP("Controller Connected!", PRINT_TO_SCREEN);
    // Register a method for receiving simpit messages from ksp
    mySimpit.inboundHandler(myCallbackHandler);
    // Register the simpit channels
    registerChannels();
}

/// <summary>Loops while the arduino is on.</summary>
void loop()
{
    // Update simpit
    mySimpit.update();
    // Record analog inputs
    recordInputs();
    // Update the inputs to ksp
    sendInputs();
    // Update the controllers outputs
    updateController();

    delay(100);
}
