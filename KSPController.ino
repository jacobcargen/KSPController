/*
 Name:		KSPController.ino
 Created:	04/13/2022 21:27:40
 Author:	Jacob Cargen
*/

#include <PayloadStructs.h>
#include <KerbalSimpitMessageTypes.h>
#include <KerbalSimpit.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Degree character for the lcd
const char DEGREE_CHAR_LCD = 223;
// Create insatance of Simpit
KerbalSimpit mySimpit(Serial);

enum directionMode
{
    maneuver, // Player depicted
    prograde, // Towards
    retrograde, // Away
    normal, // Up
    antiNormal, // Down
    radial, // In
    antiRadial, // Out
    target, // Towards
    antiTarget // Away
};

enum speedMode
{
    surface,
    orbit,
    targetVel,
    vertical,
};

#pragma region InputPinsAndStates

// Shift in A pins (8 registers)
const int SHIFT_IN_A_DATA_PIN = 11;
const int SHIFT_IN_A_CLOCK_ENABLE_PIN = 12;
const int SHIFT_IN_A_CLOCK_PIN = 13;
const int SHIFT_IN_A_LOAD_PIN = 14;
// Shift in B pins (2 registers)
const int SHIFT_IN_B_DATA_PIN = 15;
const int SHIFT_IN_B_CLOCK_ENABLE_PIN = 16;
const int SHIFT_IN_B_CLOCK_PIN = 17;
const int SHIFT_IN_B_LOAD_PIN = 18;

const int POWER_SWITCH_PIN = 53;
bool powerSwitch;
const int DEBUG_MODE_SWITCH_PIN = 52;
bool debugSwitch;
const int SPEAKER_ENABLE_SWITCH_PIN = 51;
bool speakerEnableSwitch;
// Display Controls
const int STAGE_VIEW_SWITCH_PIN = 50;
bool stageViewSwitch;
const int VERTICAL_VELOCITY_SWITCH_PIN = 49;
bool verticalVelocitySwitch;
const int CYCLE_REFERENCE_MODE_BUTTON_PIN = 48;
bool cycleRefModeButton;
const int ALT_RADAR_MODE_SWITCH_PIN = 47;
bool altRadarModeSwitch;
// Stage & Abort
const int STAGE_BUTTON_PIN = 46;
bool stageButton;
const int STAGE_LOCK_SWITCH_PIN = 45;
bool stageLockSwitch;
const int ABORT_BUTTON_PIN = 44;
bool abortButton;
const int ABORT_LOCK_SWITCH_PIN = 43;
bool abortLockSwitch;
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

// Direction Modes(9 Main Options)
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
bool dockingModeSwitch;
bool percisionModeSwitch;
bool gearSwitch;
bool lightsSwitch;
bool brakeSwitch;
bool sasSwitch;
bool rcsSwitch;
bool throttleLockSwitch;
bool setTrimTranslationButton;
bool resetTrimTranslationButton;
bool setTrimRotationButton;
bool resetTrimRotationButton;
// SAS Modes
bool sasButtons[10];
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
bool warpLockSwitch;
bool pauseButton;
bool cancelWarpButton;
bool enablePhysWarpSwitch;
bool decreaseWarpButton;
bool increaseWarpButton;
// Extras
const int EXTRA_BUTTON_1_PIN = 42;
bool extraButton1;
bool extraButton2;
// View
bool cycleFocusButton;
bool hideUIButton;
bool screenshotButton;
bool mapFlightSwitch;
bool extIvaSwitch;
bool cycleCamModeButton;
bool resetCamButton;

// AnalogPins \\

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

// Joystick analog
int rotXRaw, rotYRaw, rotZRaw;
int transXRaw, transYRaw, transZRaw;
// Joystick buttons
bool rotButton, transButton;
// Throttle analog
int throttleRaw;

#pragma endregion

//*#pragma region OutputPinsAndStates

// Shift out A pins (8 registers)
const int SHIFT_OUT_A_DATA_PIN = 2;
const int SHIFT_OUT_A_LATCH_PIN = 3;
const int SHIFT_OUT_A_CLOCK_PIN = 4;
// Shift out B pins (8 registers)
const int SHIFT_OUT_B_DATA_PIN = 5;
const int SHIFT_OUT_B_LATCH_PIN = 6;
const int SHIFT_OUT_B_CLOCK_PIN = 7;
// Shift out C pins (4 registers)
const int SHIFT_OUT_C_DATA_PIN = 8;
const int SHIFT_OUT_C_LATCH_PIN = 9;
const int SHIFT_OUT_C_CLOCK_PIN = 10;

// Pins on registers out A
int shiftOutA[64];
// Pins on registers out B
int shiftOutB[64];
// Pins on registers out C
int shiftOutC[32];

int shiftInA[64];
int shiftInB[16];

// Power led
bool pwrLed;
// Warings leds
bool warningLeds[10];
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

// LED_BARS \\

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

// Communication \\

// Heading lcd
LiquidCrystal_I2C headingLCD(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
// Speed lcd
LiquidCrystal_I2C speedLCD(0x26, 16, 2);
// Altitude lcd
LiquidCrystal_I2C altitudeLCD(0x25, 16, 2);
// Info lcd
LiquidCrystal_I2C infoLCD(0x23, 16, 2);
// Direction lcd
LiquidCrystal_I2C directionLCD(0x22, 16, 2);

// Stage & Abort
bool stageLed;
bool stageLockLed;
bool abortLed;
bool abortLockLed;
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
bool dockingModeLed;
bool PercisionModeLed;
bool gearLed;
bool lightsLed;
bool brakeLed;
bool sasLed;
bool rcsLed;
// SAS Modes
bool sasModeLeds[10];
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

#pragma endregion

#pragma region KspInfo

// SOI
String soi = "";
// Altitude
int altitude;
// Velocities
int surfaceVelocity;
int orbitalVelocity;
int verticalVelocity;
int targetVelocity;
// Ship Direction and orientation
int heading;
int pitch;
int roll;
// Maneuver Direction
int maneuverHeading;
int maneuverPitch;
// Prograde Direction
int progradeHeading;
int progradePitch;
// Retrograde Direction
int retrogradeHeading;
int retorgradePitch;
// Normal Direction
int normalHeading;
int normalPitch;
// Anti-Normal Direction
int antiNormalHeading;
int antiNormalPitch;
// Radial Direction
int radialHeading;
int radialPitch;
// Anti-Radial Direction
int antiRadialHeading;
int antiRadialPitch;
// Target Direction
int targetHeading;
int targetPitch;
// Anti-Target Direction
int antiTargetHeading;
int antiTargetPitch;

#pragma endregion

// Current speed mode to show on speed LCD
speedMode currentSpeedMode;
// Time between the top and bottom of the lcd prints
int lcdLineDelay = 0;
// Text for speed lcd
String speedLCDTopTxt, speedLCDBotTxt;
// Text for altitude lcd
String altLCDTopTxt, altLCDBotTxt;
// Text for info lcd
String infoLCDTopTxt, infoLCDBotTxt;
// Text for heading lcd
String headingLCDTopTxt, headingLCDBotTxt;
// Text for direction lcd
String dirLCDTopTxt, dirLCDBotTxt;

// Create rotation msg
rotationMessage rotMsg;
// Create translation msg
translationMessage transMsg;
// Create new throttle msg
throttleMessage throttleMsg;


///<summary>Initialize the inputs and outputs.</summary>
void initIO()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
{
    ////////////////
    /*---INPUTS---*/
    ////////////////
    
    pinMode(POWER_SWITCH_PIN, INPUT);
    pinMode(DEBUG_MODE_SWITCH_PIN, INPUT);
    pinMode(SPEAKER_ENABLE_SWITCH_PIN, INPUT);
    pinMode(STAGE_VIEW_SWITCH_PIN, INPUT);
    pinMode(VERTICAL_VELOCITY_SWITCH_PIN, INPUT);
    pinMode(CYCLE_REFERENCE_MODE_BUTTON_PIN, INPUT);
    pinMode(ALT_RADAR_MODE_SWITCH_PIN, INPUT);
    pinMode(STAGE_BUTTON_PIN, INPUT);
    pinMode(STAGE_LOCK_SWITCH_PIN, INPUT);
    pinMode(ABORT_BUTTON_PIN, INPUT);
    pinMode(ABORT_LOCK_SWITCH_PIN, INPUT);
    pinMode(EXTRA_BUTTON_1_PIN, INPUT);
    // Shift register pins
    pinMode(SHIFT_IN_A_DATA_PIN, INPUT);
    pinMode(SHIFT_IN_B_DATA_PIN, INPUT);

    /////////////////
    /*---OUTPUTS---*/
    /////////////////

    // LCDs
    headingLCD.begin();
    speedLCD.begin();
    altitudeLCD.begin();
    infoLCD.begin();
    directionLCD.begin();
    // Shift register pins
    pinMode(SHIFT_OUT_A_LATCH_PIN, OUTPUT);
    pinMode(SHIFT_OUT_A_DATA_PIN, OUTPUT);
    pinMode(SHIFT_OUT_A_CLOCK_PIN, OUTPUT);
    pinMode(SHIFT_OUT_B_LATCH_PIN, OUTPUT);
    pinMode(SHIFT_OUT_B_DATA_PIN, OUTPUT);
    pinMode(SHIFT_OUT_B_CLOCK_PIN, OUTPUT);
    pinMode(SHIFT_OUT_C_LATCH_PIN, OUTPUT);
    pinMode(SHIFT_OUT_C_DATA_PIN, OUTPUT);
    pinMode(SHIFT_OUT_C_CLOCK_PIN, OUTPUT);
    pinMode(SHIFT_IN_A_CLOCK_PIN, OUTPUT);
    pinMode(SHIFT_IN_A_CLOCK_ENABLE_PIN, OUTPUT);
    pinMode(SHIFT_IN_A_LOAD_PIN, OUTPUT);
    pinMode(SHIFT_IN_B_CLOCK_PIN, OUTPUT);
    pinMode(SHIFT_IN_B_CLOCK_ENABLE_PIN, OUTPUT);
    pinMode(SHIFT_IN_B_LOAD_PIN, OUTPUT);
}
/// <summary>Register all the needed channels for receiving simpit messages.</summary>
void registerSimpitChannels()
{
    mySimpit.registerChannel(SCENE_CHANGE_MESSAGE);
    mySimpit.registerChannel(ALTITUDE_MESSAGE);
    mySimpit.registerChannel(APSIDES_MESSAGE);
    // LED Bars
    mySimpit.registerChannel(SF_MESSAGE);
    mySimpit.registerChannel(SF_STAGE_MESSAGE);
    mySimpit.registerChannel(LF_STAGE_MESSAGE);
    mySimpit.registerChannel(LF_MESSAGE);
    mySimpit.registerChannel(OX_MESSAGE);
    mySimpit.registerChannel(OX_STAGE_MESSAGE);
    mySimpit.registerChannel(MONO_MESSAGE);
    mySimpit.registerChannel(EVA_MESSAGE);
    mySimpit.registerChannel(ELECTRIC_MESSAGE);

    mySimpit.registerChannel(VELOCITY_MESSAGE);
    mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
    mySimpit.registerChannel(APSIDESTIME_MESSAGE);
    mySimpit.registerChannel(TARGETINFO_MESSAGE);
    mySimpit.registerChannel(SOI_MESSAGE);
    mySimpit.registerChannel(ROTATION_DATA);
    mySimpit.registerChannel(AIRSPEED_MESSAGE);
}

////////////////
/*---INPUTS---*/
////////////////

/// <summary>Records every input on the controller.</summary>
void getInputs()
{
    getShiftIn(SHIFT_IN_A_DATA_PIN, SHIFT_IN_A_CLOCK_ENABLE_PIN, SHIFT_IN_A_CLOCK_PIN, SHIFT_IN_A_LOAD_PIN,
        SHIFT_IN_B_DATA_PIN, SHIFT_IN_B_CLOCK_ENABLE_PIN, SHIFT_IN_B_CLOCK_PIN, SHIFT_IN_B_LOAD_PIN);
    getNonShiftRegInputs();
}
/// <summary>Set input values before sending them to ksp.</summary>
void setInputs()
{
    setJoystickValues();
    setThrottleValues();
    setInputValues();
}
/// <summary>Send input data to ksp.</summary>
void sendInputs()
{
    // Rotation
    mySimpit.send(ROTATION_MESSAGE, rotMsg);
    // Translation
    mySimpit.send(TRANSLATION_MESSAGE, transMsg);
    // Throttle
    mySimpit.send(THROTTLE_MESSAGE, throttleMsg);
}

/////////////////
/*---OUTPUTS---*/
/////////////////

/// <summary>Get info to send to the controller. Ex: SPD, ALT, SAS state.</summary>
void getOutputs()
{

}
/// <summary>Set values to prepare them for being sent.</summary>
void setOutputs()
{
    // Lcd screen value setting 
    setSpeedLCD();
    setAltitufeLCD();
    setInfoLCD();
    setHeadingLCD();
    setDirectionLCD;
    // Set the output registers
    setOutputValues();
}
/// <summary>Updates every output on the controller.</summary>
void sendOutputs()
{
    // Shift register out group A
    sendShiftOut(shiftOutA, SHIFT_OUT_A_DATA_PIN, SHIFT_OUT_A_LATCH_PIN, SHIFT_OUT_A_CLOCK_PIN);
    // Shift register out group B
    sendShiftOut(shiftOutB, SHIFT_OUT_B_DATA_PIN, SHIFT_OUT_B_LATCH_PIN, SHIFT_OUT_B_CLOCK_PIN);
    // Shift register out group C
    sendShiftOut(shiftOutC, SHIFT_OUT_C_DATA_PIN, SHIFT_OUT_C_LATCH_PIN, SHIFT_OUT_C_CLOCK_PIN);
    // Update screens
    sendSpeedLCD();
    sendAltitufeLCD();
    sendInfoLCD();
    sendHeadingLCD();
    sendDirectionLCD();
}

#pragma region Getting

////////////////
/*---INPUTS---*/
////////////////

/// <summary>Gets shift register inputs.(MSBFIRST)</summary>
void getShiftIn(int dataA, int clockEnableA, int clockA, int loadA,
                int dataB, int clockEnableB, int clockB, int loadB)
{
    // Pulse to A
    digitalWrite(loadA, LOW);
    delayMicroseconds(5);
    digitalWrite(loadA, HIGH);
    delayMicroseconds(5);
    byte inputA[8];
    // Get input A data
    digitalWrite(clockA, HIGH);
    digitalWrite(clockEnableA, LOW);
    inputA[0] = shiftIn(dataA, clockA, MSBFIRST);
    inputA[1] = shiftIn(dataA, clockA, MSBFIRST);
    inputA[2] = shiftIn(dataA, clockA, MSBFIRST);
    inputA[3] = shiftIn(dataA, clockA, MSBFIRST);
    inputA[4] = shiftIn(dataA, clockA, MSBFIRST);
    inputA[5] = shiftIn(dataA, clockA, MSBFIRST);
    inputA[6] = shiftIn(dataA, clockA, MSBFIRST);
    inputA[7] = shiftIn(dataA, clockA, MSBFIRST);
    digitalWrite(clockEnableA, HIGH);
    
    for (size_t i = 0; i < 64; i++)
    {
        if (i < 8)
            if (1 == bitRead(inputA[0], i))
                shiftInA[i] = 1;
        else if (i < 16)
            if (1 == bitRead(inputA[1], i - 8))
                shiftInA[i] = 1;
        else if (i < 24)
            if (1 == bitRead(inputA[2], i - 16))
                shiftInA[i] = 1;
        else if (i < 32)
            if (1 == bitRead(inputA[3], i - 24))
                shiftInA[i] = 1;
        else if (i < 40)
            if (1 == bitRead(inputA[4], i - 32))
                shiftInA[i] = 1;
        else if (i < 48)
            if (1 == bitRead(inputA[5], i - 40))
                shiftInA[i] = 1;
        else if (i < 56)
            if (1 == bitRead(inputA[6], i - 48))
                shiftInA[i] = 1;
        else
            if (1 == bitRead(inputA[7], i - 56))
                shiftInA[i] = 1;
    }

    // Pulse to B
    digitalWrite(loadB, LOW);
    delayMicroseconds(5);
    digitalWrite(loadB, HIGH);
    delayMicroseconds(5);
    byte inputB[2];
    // Get input B data
    digitalWrite(clockB, HIGH);
    digitalWrite(clockEnableB, LOW);
    inputB[0] = shiftIn(dataB, clockB, MSBFIRST);
    inputB[1] = shiftIn(dataB, clockB, MSBFIRST);
    digitalWrite(clockEnableB, HIGH);
    
    for (size_t i = 0; i < 16; i++)
    {
        if (i < 8)
            if (1 == bitRead(inputB[0], i))
                shiftInB[i] = 1;
        else
            if (1 == bitRead(inputB[1], i - 8))
                shiftInB[i] = 1;
    }
}

void getNonShiftRegInputs()
{
    // Get the raw axis values
    rotXRaw = analogRead(ROTATION_X_AXIS_PIN);
    rotYRaw = analogRead(ROTATION_Y_AXIS_PIN);
    rotZRaw = analogRead(ROTATION_Z_AXIS_PIN);
    transXRaw = analogRead(TRANSLATION_X_AXIS_PIN);
    transYRaw = analogRead(TRANSLATION_Y_AXIS_PIN);
    transZRaw = analogRead(TRANSLATION_Z_AXIS_PIN);
    throttleRaw = analogRead(THROTTLE_AXIS_PIN);
    // If button less then 50, then set to true, otherwise set false;
    rotButton = analogRead(ROTATION_BUTTON_PIN) > 50 ? false : true;
    transButton = analogRead(TRANSLATION_BUTTON_PIN) > 50 ? false : true;

    // Digital VV
    // Booleans vars
    powerSwitch = (bool)digitalRead(POWER_SWITCH_PIN);
    debugSwitch = (bool)digitalRead(DEBUG_MODE_SWITCH_PIN);
    speakerEnableSwitch = (bool)digitalRead(SPEAKER_ENABLE_SWITCH_PIN);
    stageViewSwitch = (bool)digitalRead(STAGE_VIEW_SWITCH_PIN);
    verticalVelocitySwitch = (bool)digitalRead(VERTICAL_VELOCITY_SWITCH_PIN);
    stageButton = (bool)digitalRead(STAGE_BUTTON_PIN);
    stageLockSwitch = (bool)digitalRead(STAGE_LOCK_SWITCH_PIN);
    abortButton = (bool)digitalRead(ABORT_BUTTON_PIN);
    abortLockSwitch = (bool)digitalRead(ABORT_LOCK_SWITCH_PIN);
    extraButton1 = (bool)digitalRead(EXTRA_BUTTON_1_PIN);
}

/////////////////
/*---OUTPUTS---*/
/////////////////

/// <summary>Info from ksp.</summary>
void myCallbackHandler(byte messageType, byte msg[], byte msgSize)
{
    switch (messageType)
    {
    case AB_MESSAGE:break;
    case AB_STAGE_MESSAGE:break;
    case ACTIONSTATUS_MESSAGE:
        if (msgSize == sizeof(msg))
        {
            if (msg[0] & STAGE_ACTION);
            if (msg[1] & GEAR_ACTION);
            if (msg[2] & LIGHT_ACTION);
            if (msg[3] & RCS_ACTION);
            if (msg[4] & SAS_ACTION);
            if (msg[5] & BRAKES_ACTION);
            /*
            switch (msg[0])
            {
            case STAGE_ACTION:

                break;
            case GEAR_ACTION:

                break;
            case LIGHT_ACTION:

                break;
            case RCS_ACTION:

                break;
            case SAS_ACTION:

                break;
            case BRAKES_ACTION:

                break;
            case ABORT_ACTION:

                break;
            default:
                break;
            }
            // OR
            for (size_t i = 0; i < sizeof(msg); i++)
            {
                switch (msg[i])
                {
                case STAGE_ACTION:

                    break;
                case GEAR_ACTION:

                    break;
                case LIGHT_ACTION:

                    break;
                case RCS_ACTION:

                    break;
                case SAS_ACTION:

                    break;
                case BRAKES_ACTION:

                    break;
                case ABORT_ACTION:

                    break;
                default:
                    break;
                }
            }
            */
        }
        break;
    case AIRSPEED_MESSAGE:break;
    case ALTITUDE_MESSAGE:
        if (msgSize == sizeof(altitudeMessage))
        {
            altitudeMessage am;
            am = parseMessage<altitudeMessage>(msg);
        }
        break;
    case APSIDESTIME_MESSAGE:break;
    case APSIDES_MESSAGE:
        if (msgSize == sizeof(apsidesMessage))
        {
            apsidesMessage am;
            am = parseMessage<apsidesMessage>(msg);
        }
        break;
    case ATMO_CONDITIONS_MESSAGE:break;
    case BURNTIME_MESSAGE:break;
    case CAGSTATUS_MESSAGE:break;
    case DELTAVENV_MESSAGE:break;
    case DELTAV_MESSAGE:break;
    case ELECTRIC_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage ec;
            ec = parseMessage<resourceMessage>(msg);
        }
        break;
    case EVA_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage evaMP;
            evaMP = parseMessage<resourceMessage>(msg);
        }
        break;
    case FLIGHT_STATUS_MESSAGE:break;
    case LF_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage lf;
            lf = parseMessage<resourceMessage>(msg);
        }
        break;
    case LF_STAGE_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage lfStage;
            lfStage = parseMessage<resourceMessage>(msg);
        }
        break;
    case MANEUVER_MESSAGE:break;
    case MONO_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage mp;
            mp = parseMessage<resourceMessage>(msg);
        }
        break;
    case ORBIT_MESSAGE:break;
    case ORE_MESSAGE:break;
    case OX_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage ox;
            ox = parseMessage<resourceMessage>(msg);
        }
        break;
    case OX_STAGE_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage oxStage;
            oxStage = parseMessage<resourceMessage>(msg);
        }
        break;
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
    case SAS_MODE_INFO_MESSAGE:break;
    case SCENE_CHANGE_MESSAGE:
        if (msgSize == sizeof(msg))
        {

        }
        break;
    case SF_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage sf;
            sf = parseMessage<resourceMessage>(msg);
        }
        break;
    case SF_STAGE_MESSAGE:
        if (msgSize == sizeof(resourceMessage))
        {
            resourceMessage sfStage;
            sfStage = parseMessage<resourceMessage>(msg);
        }
        break;
    case SOI_MESSAGE: // WIP
        soi = (char*)msg;
        //soi[msgSize] = '\0'; 
        soi[soi.length()] = '\0';
        mySimpit.printToKSP("SOI:'" + soi + "'", PRINT_TO_SCREEN);
        break;
    case TARGETINFO_MESSAGE:
        if (msgSize == sizeof(targetMessage))
        {
            targetMessage tm;
            tm = parseMessage<targetMessage>(msg);
            targetVelocity = tm.velocity;
        }
        break;
    case TEMP_LIMIT_MESSAGE:break;
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
    case XENON_GAS_MESSAGE:break;
    case XENON_GAS_STAGE_MESSAGE:break;
    default:
        break;
    }
}

#pragma endregion

#pragma region Setting

////////////////
/*---INPUTS---*/
////////////////

void setInputValues()
{
    // Shift registers in A
    infoModes[0] = (bool)shiftInA[0]; // A:0
    infoModes[1] = (bool)shiftInA[1]; // A:1
    infoModes[2] = (bool)shiftInA[2]; // A:2
    infoModes[3] = (bool)shiftInA[3]; // A:3
    infoModes[4] = (bool)shiftInA[4]; // A:4
    infoModes[5] = (bool)shiftInA[5]; // A:5
    infoModes[6] = (bool)shiftInA[6]; // A:6
    infoModes[7] = (bool)shiftInA[7]; // A:7
    infoModes[8] = (bool)shiftInA[8]; // B:0
    infoModes[9] = (bool)shiftInA[9]; // B:1
    infoModes[10] = (bool)shiftInA[10]; // B:2
    infoModes[11] = (bool)shiftInA[11]; // B:3
    dirModes[0] = (bool)shiftInA[12]; // B:4
    dirModes[1] = (bool)shiftInA[13]; // B:5
    dirModes[2] = (bool)shiftInA[14]; // B:6
    dirModes[3] = (bool)shiftInA[15]; // B:7
    dirModes[4] = (bool)shiftInA[16]; // C:0
    dirModes[5] = (bool)shiftInA[17]; // C:1
    dirModes[6] = (bool)shiftInA[18]; // C:2
    dirModes[7] = (bool)shiftInA[19]; // C:3
    dirModes[8] = (bool)shiftInA[20]; // C:4
    dirModes[9] = (bool)shiftInA[21]; // C:5
    dirModes[10] = (bool)shiftInA[22]; // C:6
    dirModes[11] = (bool)shiftInA[23]; // C:7
    warnButtons[0] = (bool)shiftInA[24]; // D:0
    warnButtons[1] = (bool)shiftInA[25]; // D:1
    warnButtons[2] = (bool)shiftInA[26]; // D:2
    warnButtons[3] = (bool)shiftInA[27]; // D:3
    warnButtons[4] = (bool)shiftInA[28]; // D:4
    warnButtons[5] = (bool)shiftInA[29]; // D:5
    warnButtons[6] = (bool)shiftInA[30]; // D:6
    warnButtons[7] = (bool)shiftInA[31]; // D:7
    warnButtons[8] = (bool)shiftInA[32]; // E:0
    warnButtons[9] = (bool)shiftInA[33]; // E:1
    agButtons[0] = (bool)shiftInA[34]; // E:2
    agButtons[1] = (bool)shiftInA[35]; // E:3
    agButtons[2] = (bool)shiftInA[36]; // E:4
    agButtons[3] = (bool)shiftInA[37]; // E:5
    agButtons[4] = (bool)shiftInA[38]; // E:6
    agButtons[5] = (bool)shiftInA[39]; // E:7
    agButtons[6] = (bool)shiftInA[40]; // F:0
    agButtons[7] = (bool)shiftInA[41]; // F:1
    agButtons[8] = (bool)shiftInA[42]; // F:2
    agButtons[9] = (bool)shiftInA[43]; // F:3
    dockingModeSwitch = (bool)shiftInA[44]; // F:4
    percisionModeSwitch = (bool)shiftInA[45]; // F:5
    gearSwitch = (bool)shiftInA[46]; // F:6
    lightsSwitch = (bool)shiftInA[47]; // F:7
    brakeSwitch = (bool)shiftInA[48]; // G:0
    sasSwitch = (bool)shiftInA[49]; // G:1
    rcsSwitch = (bool)shiftInA[50]; // G:2
    throttleLockSwitch = (bool)shiftInA[51]; // G:3
    setTrimTranslationButton = (bool)shiftInA[52]; // G:4
    resetTrimTranslationButton = (bool)shiftInA[53]; // G:5
    setTrimRotationButton = (bool)shiftInA[54]; // G:6
    resetTrimRotationButton = (bool)shiftInA[55]; // G:7
    sasButtons[0] = (bool)shiftInA[56]; // H:0
    sasButtons[1] = (bool)shiftInA[57]; // H:1
    sasButtons[2] = (bool)shiftInA[58]; // H:2
    sasButtons[3] = (bool)shiftInA[59]; // H:3
    sasButtons[4] = (bool)shiftInA[60]; // H:4
    sasButtons[5] = (bool)shiftInA[61]; // H:5
    sasButtons[6] = (bool)shiftInA[62]; // H:6
    sasButtons[7] = (bool)shiftInA[63]; // H:7
    // Shift registers in B
    sasButtons[8] = (bool)shiftInB[0]; // A:0
    sasButtons[9] = (bool)shiftInB[1]; // A:1
    warpLockSwitch = (bool)shiftInB[2]; // A:2
    pauseButton = (bool)shiftInB[3]; // A:3
    cancelWarpButton = (bool)shiftInB[4]; // A:4
    enablePhysWarpSwitch = (bool)shiftInB[5]; // A:5
    decreaseWarpButton = (bool)shiftInB[6]; // A:6
    increaseWarpButton = (bool)shiftInB[7]; // A:7
    extraButton2 = (bool)shiftInB[8]; // B:0
    cycleFocusButton = (bool)shiftInB[9]; // B:1
    hideUIButton = (bool)shiftInB[10]; // B:2
    screenshotButton = (bool)shiftInB[11]; // B:3
    mapFlightSwitch = (bool)shiftInB[12]; // B:4
    extIvaSwitch = (bool)shiftInB[13]; // B:5
    cycleCamModeButton = (bool)shiftInB[14]; // B:6
    resetCamButton = (bool)shiftInB[15]; // B:7
    // More values (non-shift register)

}

// Analog \\

/// <summary>Send joystick values to ksp for rotation and translation.</summary>
void setJoystickValues()
{
    // Smoothing and mapping
    int16_t rotX = smoothAndMapAxis(rotXRaw);
    int16_t rotY = smoothAndMapAxis(rotYRaw);
    int16_t rotZ = smoothAndMapAxis(rotZRaw);
    int16_t transX = smoothAndMapAxis(transXRaw);
    int16_t transY = smoothAndMapAxis(transYRaw);
    int16_t transZ = smoothAndMapAxis(transZRaw);
    // Flip some values the right way
    rotX *= -1;
    // Set msg values
    rotMsg.setPitchRollYaw(rotY, rotX, rotZ);
    transMsg.setXYZ(transX, transZ, transY);
}
/// <summary>Send throttle values to ksp.</summary>
void setThrottleValues()
{
    // Smooth and map the raw input
    int16_t throttle = smoothAndMapAxis(throttleRaw);
    // Set the throttle value
    throttleMsg.throttle = throttle;
}
/// <summary>Give the raw analog some smoothing.</summary>
/// <returns>Returns a smoothed and mapped value.</returns>
int16_t smoothAndMapAxis(int raw)
{
    // Smooth the raw input (NO SMOOTHING YET)
    int smooth = raw;
    // Map the smoothed data for simpit
    int16_t newMap = map(smooth, 0, 1023, INT16_MIN, INT16_MAX);
    // Return the smoothed mapped data
    return newMap;
}

/////////////////
/*---OUTPUTS---*/
/////////////////

void setOutputValues()
{
    // Shift register out A
    shiftOutA[0] = 0;//(int)sfLeds[0]; // A:0
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
    shiftOutA[19] = (int)sfLeds[19]; // C:3      
    shiftOutA[20] = (int)lfLeds[0]; // C:4      
    shiftOutA[21] = (int)lfLeds[1]; // C:5      
    shiftOutA[22] = (int)lfLeds[2]; // C:6      
    shiftOutA[23] = (int)lfLeds[3]; // C:7      
    shiftOutA[24] = (int)lfLeds[4]; // D:0      
    shiftOutA[25] = (int)lfLeds[5]; // D:1      
    shiftOutA[26] = (int)lfLeds[6]; // D:2      
    shiftOutA[27] = (int)lfLeds[7]; // D:3      
    shiftOutA[28] = (int)lfLeds[8]; // D:4      
    shiftOutA[29] = (int)lfLeds[9]; // D:5      
    shiftOutA[30] = (int)lfLeds[10]; // D:6      
    shiftOutA[31] = (int)lfLeds[11]; // D:7      
    shiftOutA[32] = (int)lfLeds[12]; // E:0      
    shiftOutA[33] = (int)lfLeds[13]; // E:1      
    shiftOutA[34] = (int)lfLeds[14]; // E:2      
    shiftOutA[35] = (int)lfLeds[15]; // E:3      
    shiftOutA[36] = (int)lfLeds[16]; // E:4      
    shiftOutA[37] = (int)lfLeds[17]; // E:5      
    shiftOutA[38] = (int)lfLeds[18]; // E:6      
    shiftOutA[39] = (int)lfLeds[19]; // E:7      
    shiftOutA[40] = (int)oxLeds[0]; // F:0      
    shiftOutA[41] = (int)oxLeds[1]; // F:1      
    shiftOutA[42] = (int)oxLeds[2]; // F:2      
    shiftOutA[43] = (int)oxLeds[3]; // F:3      
    shiftOutA[44] = (int)oxLeds[4]; // F:4      
    shiftOutA[45] = (int)oxLeds[5]; // F:5      
    shiftOutA[46] = (int)oxLeds[6]; // F:6      
    shiftOutA[47] = (int)oxLeds[7]; // F:7      
    shiftOutA[48] = (int)oxLeds[8]; // G:0      
    shiftOutA[49] = (int)oxLeds[9]; // G:1      
    shiftOutA[50] = (int)oxLeds[10]; // G:2      
    shiftOutA[51] = (int)oxLeds[11]; // G:3      
    shiftOutA[52] = (int)oxLeds[12]; // G:4      
    shiftOutA[53] = (int)oxLeds[13]; // G:5      
    shiftOutA[54] = (int)oxLeds[14]; // G:6      
    shiftOutA[55] = (int)oxLeds[15]; // G:7      
    shiftOutA[56] = (int)oxLeds[16]; // H:0      
    shiftOutA[57] = (int)oxLeds[17]; // H:1      
    shiftOutA[58] = (int)oxLeds[18]; // H:2      
    shiftOutA[59] = (int)oxLeds[19]; // H:3      
    shiftOutA[60] = (int)mpLeds[0]; // H:4      
    shiftOutA[61] = (int)mpLeds[1]; // H:5      
    shiftOutA[62] = (int)mpLeds[2]; // H:6
    shiftOutA[63] = (int)mpLeds[3]; // H:7
    // Shift register out B
    shiftOutB[0] = (int)mpLeds[4]; // A:0
    shiftOutB[1] = (int)mpLeds[5]; // A:1
    shiftOutB[2] = (int)mpLeds[6]; // A:2
    shiftOutB[3] = (int)mpLeds[7]; // A:3
    shiftOutB[4] = (int)mpLeds[8]; // A:4
    shiftOutB[5] = (int)mpLeds[9]; // A:5
    shiftOutB[6] = (int)mpLeds[10]; // A:6
    shiftOutB[7] = (int)mpLeds[11]; // A:7
    shiftOutB[8] = (int)mpLeds[12]; // B:0
    shiftOutB[9] = (int)mpLeds[13]; // B:1
    shiftOutB[10] = (int)mpLeds[14]; // B:2
    shiftOutB[11] = (int)mpLeds[15]; // B:3
    shiftOutB[12] = (int)mpLeds[16]; // B:4
    shiftOutB[13] = (int)mpLeds[17]; // B:5
    shiftOutB[14] = (int)mpLeds[18]; // B:6
    shiftOutB[15] = (int)mpLeds[19]; // B:7
    shiftOutB[16] = (int)ecLeds[0]; // C:0
    shiftOutB[17] = (int)ecLeds[1]; // C:1
    shiftOutB[18] = (int)ecLeds[2]; // C:2
    shiftOutB[19] = (int)ecLeds[3]; // C:3      
    shiftOutB[20] = (int)ecLeds[4]; // C:4      
    shiftOutB[21] = (int)ecLeds[5]; // C:5      
    shiftOutB[22] = (int)ecLeds[6]; // C:6      
    shiftOutB[23] = (int)ecLeds[7]; // C:7      
    shiftOutB[24] = (int)ecLeds[8]; // D:0      
    shiftOutB[25] = (int)ecLeds[9]; // D:1      
    shiftOutB[26] = (int)ecLeds[10]; // D:2      
    shiftOutB[27] = (int)ecLeds[11]; // D:3      
    shiftOutB[28] = (int)ecLeds[12]; // D:4      
    shiftOutB[29] = (int)ecLeds[13]; // D:5      
    shiftOutB[30] = (int)ecLeds[14]; // D:6      
    shiftOutB[31] = (int)ecLeds[15]; // D:7      
    shiftOutB[32] = (int)ecLeds[16]; // E:0      
    shiftOutB[33] = (int)ecLeds[17]; // E:1      
    shiftOutB[34] = (int)ecLeds[18]; // E:2      
    shiftOutB[35] = (int)ecLeds[19]; // E:3      
    shiftOutB[36] = (int)pwrLed; // E:4      
    shiftOutB[37] = (int)warningLeds[0]; // E:5      
    shiftOutB[38] = (int)warningLeds[1]; // E:6      
    shiftOutB[39] = (int)warningLeds[2]; // E:7      
    shiftOutB[40] = (int)warningLeds[3]; // F:0      
    shiftOutB[41] = (int)warningLeds[4]; // F:1      
    shiftOutB[42] = (int)warningLeds[5]; // F:2      
    shiftOutB[43] = (int)warningLeds[6]; // F:3      
    shiftOutB[44] = (int)warningLeds[7]; // F:4      
    shiftOutB[45] = (int)warningLeds[8]; // F:5      
    shiftOutB[46] = (int)warningLeds[9]; // F:6      
    shiftOutB[47] = (int)stageLed; // F:7      
    shiftOutB[48] = (int)stageLockLed; // G:0      
    shiftOutB[49] = (int)abortLed; // G:1      
    shiftOutB[50] = (int)abortLockLed; // G:2      
    shiftOutB[51] = (int)agLeds[0]; // G:3      
    shiftOutB[52] = (int)agLeds[1]; // G:4      
    shiftOutB[53] = (int)agLeds[2]; // G:5      
    shiftOutB[54] = (int)agLeds[3]; // G:6      
    shiftOutB[55] = (int)agLeds[4]; // G:7      
    shiftOutB[56] = (int)agLeds[5]; // H:0      
    shiftOutB[57] = (int)agLeds[6]; // H:1      
    shiftOutB[58] = (int)agLeds[7]; // H:2      
    shiftOutB[59] = (int)agLeds[8]; // H:3      
    shiftOutB[60] = (int)agLeds[9]; // H:4      
    shiftOutB[61] = (int)dockingModeLed; // H:5      
    shiftOutB[62] = (int)PercisionModeLed; // H:6
    shiftOutB[63] = (int)lightsLed; // H:7
    // Shift register out C
    shiftOutC[0] = (int)gearLed; // A:0
    shiftOutC[1] = (int)brakeLed; // A:1
    shiftOutC[2] = (int)sasLed; // A:2
    shiftOutC[3] = (int)rcsLed; // A:3
    shiftOutC[4] = (int)sasModeLeds[0]; // A:4
    shiftOutC[5] = (int)sasModeLeds[1]; // A:5
    shiftOutC[6] = (int)sasModeLeds[2]; // A:6
    shiftOutC[7] = (int)sasModeLeds[3]; // A:7
    shiftOutC[8] = (int)sasModeLeds[4]; // B:0
    shiftOutC[9] = (int)sasModeLeds[5]; // B:1
    shiftOutC[10] = (int)sasModeLeds[6]; // B:2
    shiftOutC[11] = (int)sasModeLeds[7]; // B:3
    shiftOutC[12] = (int)sasModeLeds[8]; // B:4
    shiftOutC[13] = (int)sasModeLeds[9]; // B:5
    shiftOutC[14] = 0; // B:6
    shiftOutC[15] = 0; // B:7
    shiftOutC[16] = 0; // C:0
    shiftOutC[17] = 0; // C:1
    shiftOutC[18] = 0; // C:2
    shiftOutC[19] = 0; // C:3      
    shiftOutC[20] = 0; // C:4      
    shiftOutC[21] = 0; // C:5      
    shiftOutC[22] = 0; // C:6      
    shiftOutC[23] = 0; // C:7      
    shiftOutC[24] = 0; // D:0      
    shiftOutC[25] = 0; // D:1      
    shiftOutC[26] = 0; // D:2      
    shiftOutC[27] = 0; // D:3      
    shiftOutC[28] = 0; // D:4      
    shiftOutC[29] = 0; // D:5      
    shiftOutC[30] = 0; // D:6      
    shiftOutC[31] = 0; // D:7
}

// Serial \\

/// <summary>Set the speed lcd values.</summary>
void setSpeedLCD()
{
    // Txt for the speed mode
    String mode;
    // Speed
    int speed;
    // Clear the strings
    speedLCDTopTxt = "";
    speedLCDBotTxt = "";
    // Check the current speed mode to use and set the values for that mode
    switch (currentSpeedMode)
    {
    case surface:
        speed = surfaceVelocity;
        mode = "Surface";
        break;
    case orbit:
        speed = orbitalVelocity;
        mode = "Orbit";
        break;
    case targetVel:
        speed = targetVelocity;
        mode = "Target";
        break;
    case vertical:
        speed = verticalVelocity;
        mode = "Vertical";
        break;
    default:
        break;
    }
    speedLCDTopTxt += mode;// Alt 
    // Speed txt
    speedLCDBotTxt += "SPD ";
    // Speed
    speedLCDBotTxt += formatNumber(speed, 9, false, false);
    // Add unit measurement
    speedLCDBotTxt += "m/s";

}
/// <summary>Set the altitude lcd values.</summary>
void setAltitufeLCD()
{
    // Check the alt mode (NEEDS TO CHECK MODE STILL)(UPDATE THIS) 
    bool radarMode = true;
    // Alt
    int alt;
    // Clear the strings
    altLCDTopTxt = "";
    altLCDBotTxt = "";
    // Calculate gap for soi name
    // No SOI names are more than 7 char, which is good because that is the exact amount of room at max on the lcd.
    altLCDTopTxt += calculateGap(soi, 7);
    // Check altitude mode
    if (!radarMode) // SEA
    {
        altLCDTopTxt += "      Sea";
    }
    else // LAND
    {
        altLCDTopTxt += "     Land";
    }
    // Alt txt
    altLCDBotTxt += "ALT";
    if (altitude >= 1000000) alt = getKilos(altitude);
    // Alt 
    altLCDBotTxt += formatNumber(alt, 12, true, false);
    // Add distance unit
    if (altitude >= 1000000) altLCDBotTxt += "k";
    else altLCDBotTxt += "m";
}
/// <summary>Set the info lcd values.</summary>
void setInfoLCD()
{
    // Just do this last..
}
/// <summary>Set the heading lcd values.</summary>
void setHeadingLCD()
{
    // Clear the strings
    headingLCDTopTxt = "";
    headingLCDBotTxt = "";
    // Gap? 
    headingLCDTopTxt += "        ";
    // Heading txt
    headingLCDTopTxt += " HDG+";
    headingLCDTopTxt += formatNumber(heading, 3, false, false);
    headingLCDTopTxt += DEGREE_CHAR_LCD;
    // Pitch txt
    headingLCDBotTxt += "PTH";
    headingLCDBotTxt += formatNumber(pitch, 3, true, false);
    headingLCDBotTxt += DEGREE_CHAR_LCD;
    // Roll txt
    headingLCDBotTxt += " RLL";
    headingLCDBotTxt += formatNumber(roll, 4, true, true);
    headingLCDBotTxt += DEGREE_CHAR_LCD;
}
/// <summary>Set the direction lcd values.</summary>
void setDirectionLCD()
{
    directionMode dirMode;
    // Clear the strings
    dirLCDTopTxt = "";
    dirLCDBotTxt = "";
    // Gap after the mode name
    String gap = "    "; // Default gap
    // Declare the values
    int hdg, pth; // No need for roll
    // Check for which mode the use and set the values for that mode
    switch (dirMode)
    {
    case maneuver:
        dirLCDTopTxt += "MVR";
        hdg = maneuverHeading;
        pth = maneuverPitch;
        break;
    case prograde:
        dirLCDTopTxt += "PGD";
        hdg = progradeHeading;
        pth = progradePitch;
        break;
    case retrograde:
        dirLCDTopTxt += "RGD";
        hdg = retrogradeHeading;
        pth = retorgradePitch;
        break;
    case normal:
        dirLCDTopTxt += "NOR";
        hdg = normalHeading;
        pth = normalPitch;
        break;
    case antiNormal:
        dirLCDTopTxt += "A-NOR";
        hdg = antiNormalHeading;
        pth = antiNormalPitch;
        gap = "  ";
        break;
    case radial:
        dirLCDTopTxt += "RAD";
        hdg = radialHeading;
        pth = radialPitch;
        break;
    case antiRadial:
        dirLCDTopTxt += "A-RAD";
        hdg = antiRadialHeading;
        pth = antiRadialPitch;
        gap = "  ";
        break;
    case target:
        dirLCDTopTxt += "TAR";
        hdg = targetHeading;
        pth = targetPitch;
        break;
    case antiTarget:
        dirLCDTopTxt += "A-TAR";
        hdg = antiTargetHeading;
        pth = antiTargetPitch;
        gap = "  ";
        break;
    default:
        break;
    }
    // Gap
    dirLCDTopTxt += gap;
    // Heading txt
    dirLCDTopTxt += " HDG+";
    dirLCDTopTxt += formatNumber(hdg, 3, false, false);
    dirLCDTopTxt += DEGREE_CHAR_LCD;
    // Pitch txt
    dirLCDBotTxt += "PTH";
    dirLCDBotTxt += formatNumber(pth, 3, true, false);
    dirLCDBotTxt += DEGREE_CHAR_LCD;
}
/// <summary>Format numbers for lcd. Length max is 16 characters.</summary>
/// <returns>Returns a formated number at a specific length.</returns>
/// <remarks>This will fit a number to a character range,
/// the number will be to the right of the excess characters. 
/// The excess will become zeros (to the left of the number).</remarks>
String formatNumber(int number, byte lengthReq, bool canBeNegative, bool flipNegative)
{
    // Makes the number a positive
    int num = abs(number);
    // Check if the number is negative 
    bool isNegative = number < 0 ? true : false;
    // If should flip the polarity, Does not flip if the number is a zero (fix for causing zero to go negative)
    if (flipNegative && number != 0) isNegative = !isNegative;
    // Check length
    if (num < 10) lengthReq -= 1; // 1 characters
    else if (num < 100) lengthReq -= 2; // 2 characters
    else if (num < 1000) lengthReq -= 3; // 3 characters
    else if (num < 10000) lengthReq -= 4; // 4 characters
    else if (num < 100000) lengthReq -= 5; // 5 characters
    else if (num < 1000000) lengthReq -= 6; // 6 characters
    else if (num < 10000000) lengthReq -= 7; // 7 characters
    else if (num < 100000000) lengthReq -= 8; // 8 characters
    else if (num < 1000000000) lengthReq -= 9; // 9 characters
    else if (num < 10000000000) lengthReq -= 10; // 10 characters
    else if (num < 100000000000) lengthReq -= 11; // 11 characters
    else if (num < 1000000000000) lengthReq -= 12; // 12 characters
    else if (num < 10000000000000) lengthReq -= 13; // 13 characters
    else if (num < 100000000000000) lengthReq -= 14; // 14 characters
    else if (num < 1000000000000000) lengthReq -= 15; // 15 characters
    else if (num < 10000000000000000) lengthReq -= 16; // 16 characters

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
/// <summary>Input meters and receive it converted and rounded into kilos.</summary>
/// <param name="meters"></param>
/// <returns>Meters conveted into kilos.</returns>
int getKilos(int meters) 
{
    // Convert
    int kilos = meters / 1000;
    // Round and return
    return round(kilos);
}

#pragma endregion

#pragma region Sending

////////////////
/*---INPUTS---*/
////////////////



/////////////////
/*---OUTPUTS---*/
/////////////////

/// <summary>Updates a shift register group. (MSBFIRST) (MAX OF 64 pins)</summary>
void sendShiftOut(int pins[], int dataPin, int latchPin, int clockPin)
{
    // Define outputA (4bytes)
    unsigned long outputA = 0;
    // Define outputB (4bytes)
    unsigned long outputB = 0;
    // For each pin/bit
    for (int pin = 0; pin < sizeof(pins); pin++)
    {
        // First 4 bytes
        if (pin < 31 && pins[pin] == 1)
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
    // Break down the bytes 4-2bytes
    unsigned int b0_1 = int(outputA);
    unsigned int b2_3 = int(outputA >> 16);
    unsigned int b4_5 = int(outputB);
    unsigned int b6_7 = int(outputB >> 16);
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

/// <summary>Update the speed.</summary>
void sendSpeedLCD()
{
    // Clear LCD
    speedLCD.clear();
    // Print to top line
    speedLCD.setCursor(0, 0);
    speedLCD.print(speedLCDTopTxt);
    // Delay
    delay(lcdLineDelay);
    // Print to bottom line
    speedLCD.setCursor(0, 1);
    speedLCD.print(speedLCDBotTxt);
}
/// <summary>Update the altitude lcd.</summary>
void sendAltitufeLCD()
{
    // Clear LCD
    altitudeLCD.clear();
    // Print to top line
    altitudeLCD.setCursor(0, 0);
    altitudeLCD.print(altLCDTopTxt);
    // Delay
    delay(lcdLineDelay);
    // Print to bottom line
    altitudeLCD.setCursor(0, 1);
    altitudeLCD.print(altLCDBotTxt);
}
/// <summary>Update the info lcd.</summary>
void sendInfoLCD()
{
    // Clear LCD
    infoLCD.clear();
    // Print to top line
    infoLCD.setCursor(0, 0);
    infoLCD.print(infoLCDTopTxt);
    // Delay
    delay(lcdLineDelay);
    // Print to bottom line
    infoLCD.setCursor(0, 1);
    infoLCD.print(infoLCDBotTxt);
}
/// <summary>Update the heading lcd.</summary>
void sendHeadingLCD()
{
    // Clear LCD
    headingLCD.clear();
    // Print to top line
    headingLCD.setCursor(0, 0);
    headingLCD.print(headingLCDTopTxt);
    // Delay
    delay(lcdLineDelay);
    // Print to bottom line
    headingLCD.setCursor(0, 1);
    headingLCD.print(headingLCDBotTxt);
}
/// <summary>Update the direction lcd.</summary>
void sendDirectionLCD()
{
    // Clear LCD
    directionLCD.clear();
    // Print to top line
    directionLCD.setCursor(0, 0);
    directionLCD.print(dirLCDTopTxt);
    // Delay
    delay(lcdLineDelay);
    // Print to bottom line
    directionLCD.setCursor(0, 1);
    directionLCD.print(dirLCDBotTxt);
}

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
    mySimpit.printToKSP("KSP Controller Connected!", PRINT_TO_SCREEN);
    // Register a method for receiving simpit messages from ksp
    mySimpit.inboundHandler(myCallbackHandler);
    // Register the simpit channels
    registerSimpitChannels();
}

/// <summary>Loops while the arduino is on.</summary>
void loop()
{
    // Update simpit
    mySimpit.update();

    // Record analog inputs
    getInputs();
    // Set input values
    setInputs();
    // Update the inputs to ksp
    sendInputs();

    // Get info for outputs
    getOutputs();
    // Set outputs
    setOutputs();
    // Update the controllers outputs
    sendOutputs();

    // Delay 100ms
    delay(100);
}
