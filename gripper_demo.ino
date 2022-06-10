#include <Servo.h>
#include <Wire.h>

// switch parameters
#define SWITCH_1_PIN 51
#define SWITCH_2_PIN 49
#define SWITCH_3_PIN 45
#define SWITCH_4_PIN 47
#define SWITCH_5_PIN 53

// Gripper 1 parameters (SOFT ROT)
#define GRIPPER_1_SERVO_1 9
#define GRIPPER_1_SERVO_2 10
#define GRIPPER_1_OPEN 5
#define GRIPPER_1_CLOSE 60

// Gripper 2 parameters (HARD ROT)
#define GRIPPER_2_SERVO_1 7
#define GRIPPER_2_SERVO_2 8
#define GRIPPER_2_OPEN 5
#define GRIPPER_2_CLOSE 60

// Gripper 3 parameters
#define GRIPPER_3_SERVO_1 11
#define GRIPPER_3_SERVO_2 12
#define GRIPPER_3_SERVO_3 6

// Sensor params
#define THRESHHOLD 150

// create servos
Servo g_1_s_1;
Servo g_1_s_2;

Servo g_2_s_1;
Servo g_2_s_2;

Servo g_3_s_1;
Servo g_3_s_2;
Servo g_3_s_3;

// Aashi code
const uint8_t smcDeviceNumber = 13;

// Required to allow motors to move.
// Must be called when controller restarts and after any error.
void exitSafeStart()
{
    Wire.beginTransmission(smcDeviceNumber);
    Wire.write(0x83); // Exit safe start
    Wire.endTransmission();
}

void setMotorSpeed(int16_t speed)
{
    uint8_t cmd = 0x85; // Motor forward
    if (speed < 0)
    {
        cmd = 0x86; // Motor reverse
        speed = -speed;
    }
    Wire.beginTransmission(smcDeviceNumber);
    Wire.write(cmd);
    Wire.write(speed & 0x1F);
    Wire.write(speed >> 5 & 0x7F);
    Wire.endTransmission();
}

uint16_t currentMeasure()
{
    Wire.beginTransmission(smcDeviceNumber);
    Wire.write(0xA1); // Command: Get variable
    Wire.write(44);   // Variable ID: Current in mA
    Wire.endTransmission();
    Wire.requestFrom(smcDeviceNumber, (uint8_t)2);
    uint16_t upTime = Wire.read();
    upTime |= Wire.read() << 8;
    return upTime;
}

void setup()
{
    // switch Pins
    pinMode(SWITCH_1_PIN, INPUT);
    pinMode(SWITCH_2_PIN, INPUT);
    pinMode(SWITCH_3_PIN, INPUT);
    pinMode(SWITCH_4_PIN, INPUT);
    pinMode(SWITCH_5_PIN, INPUT);

    // servo initialization
    g_1_s_1.attach(GRIPPER_1_SERVO_1);
    g_1_s_2.attach(GRIPPER_1_SERVO_2);

    g_2_s_1.attach(GRIPPER_2_SERVO_1);
    g_2_s_2.attach(GRIPPER_2_SERVO_2);

    g_3_s_1.attach(GRIPPER_3_SERVO_1);
    g_3_s_2.attach(GRIPPER_3_SERVO_2);
    g_3_s_3.attach(GRIPPER_3_SERVO_3);

    // serial for debugging
    Serial.begin(9600);

    // initialize LED's
    pinMode(32, OUTPUT);
    pinMode(34, OUTPUT);
    pinMode(36, OUTPUT);
    pinMode(38, OUTPUT);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);

    Wire.begin();
    exitSafeStart();

    // setMotorSpeed(1000);
    // delay(700);
}

void loop()
{
    // CURRRENT MEASURING - Read the up time from the controller and send it to the serial monitor.
    uint16_t current = currentMeasure();

    // FORCE SENSING
    if (analogRead(A0) >= THRESHHOLD)
    {
        digitalWrite(36, HIGH);
    }
    else
    {
        digitalWrite(36, LOW);
    }

    if (analogRead(A1) >= THRESHHOLD)
    {
        digitalWrite(38, HIGH);
    }
    else
    {
        digitalWrite(38, LOW);
    }

    if (analogRead(A2) >= THRESHHOLD)
    {
        digitalWrite(32, HIGH);
    }
    else
    {
        digitalWrite(32, LOW);
    }

    if (analogRead(A3) >= THRESHHOLD)
    {
        digitalWrite(34, HIGH);
    }
    else
    {
        digitalWrite(34, LOW);
    }

    Serial.print(analogRead(A0));
    Serial.print("\t");
    Serial.print(analogRead(A1));
    Serial.print("\t");
    Serial.print(analogRead(A2));
    Serial.print("\t");
    Serial.println(analogRead(A3));
    delay(100);

    // GRIPPER 1(ROT SOFT)
    if (digitalRead(SWITCH_1_PIN) == HIGH)
    {
        // open gripper
        g_1_s_1.write(40);
        g_1_s_2.write(100);
    }
    if (digitalRead(SWITCH_1_PIN) == LOW)
    {
        // close gripper
        g_1_s_1.write(100); // 100 -> 20
        g_1_s_2.write(160); // 80 -> 160
    }

    // GRIPPER 2 (ROT HARD)
    if (digitalRead(SWITCH_2_PIN) == HIGH)
    {
        // open gripper

        g_2_s_1.write(80);
        g_2_s_2.write(100);
    }
    if (digitalRead(SWITCH_2_PIN) == LOW)
    {
        // close gripper
        // NORMAL CLOSE
        g_2_s_1.write(20);  // 100 -> 20
        g_2_s_2.write(160); // 80 -> 160
        // FULL CLOSE
        // g_2_s_1.write(16);  // 100 -> 20
        // g_2_s_2.write(164); // 80 -> 160
    }
    // GRIPPER 3 (ROT LIN) [LINEAR]
    if (digitalRead(SWITCH_3_PIN) == HIGH)
    {
        g_3_s_3.write(5);
        // Serial.print("3, ");
    }
    if (digitalRead(SWITCH_3_PIN) == LOW)
    {

        g_3_s_3.write(130);
        // Serial.print("1, ");
    }
    // GRIPPER 4 (ROT LIN) [ROT]
    if (digitalRead(SWITCH_4_PIN) == HIGH)
    {
        // Serial.print("4, ");
        g_3_s_1.write(0);
        g_3_s_2.write(0);
        g_3_s_3.write(5); // optional
    }
    if (digitalRead(SWITCH_4_PIN) == LOW)
    {
        // Serial.print("1, ");
        g_3_s_1.write(70);
        g_3_s_2.write(70);
        g_3_s_3.write(60); // optional
    }
    // GRIPPER 5 (LIN V2)
    if (digitalRead(SWITCH_5_PIN) == HIGH)
    {
        if (current < 2200)
        {
            // delay(1000);
            setMotorSpeed(1000);
            // exitSafeStart(); // for current = 3200mA
            // Serial.print("5, ");
        }
    }
    if (digitalRead(SWITCH_5_PIN) == LOW)
    {
        // setMotorSpeed(-1000);
        // delay(200);
        // if (current <= 1400)
        // {
        // delay(1000);
        if (current < 2200)
        {
            setMotorSpeed(-1000);
        }
        // exitSafeStart(); // for current = 3200mA
        // Serial.print("1, ");
    }
}

/* LED TEST
    digitalWrite(32, HIGH);
    digitalWrite(34, LOW);
    digitalWrite(36, LOW);
    digitalWrite(38, LOW);
    delay(1000);
    digitalWrite(32, LOW);
    digitalWrite(34, HIGH);
    digitalWrite(36, LOW);
    digitalWrite(38, LOW);
    delay(1000);
    digitalWrite(32, LOW);
    digitalWrite(34, LOW);
    digitalWrite(36, HIGH);
    digitalWrite(38, LOW);
    delay(1000);
    digitalWrite(32, LOW);
    digitalWrite(34, LOW);
    digitalWrite(36, LOW);
    digitalWrite(38, HIGH);
    delay(1000);
    */