#include <Servo.h>

// switch parameters
#define SWITCH_1_PIN 51
#define SWITCH_2_PIN 49
#define SWITCH_3_PIN 45
#define SWITCH_4_PIN 47
#define SWITCH_5_PIN 53

// Gripper 1 parameters
#define GRIPPER_1_SERVO_1 2
#define GRIPPER_1_SERVO_2 3
#define GRIPPER_1_OPEN 5
#define GRIPPER_1_CLOSE 60

Servo g_1_s_1;
Servo g_1_s_2;
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

    // serial for debugging
    // Serial.begin(9600);
}

void loop()
{
    // GRIPPER 1 (ROT HARD)
    if (digitalRead(SWITCH_1_PIN) == HIGH)
    {
        // close gripper
        g_1_s_1.write(GRIPPER_1_CLOSE);
        g_1_s_2.write(180 - GRIPPER_1_CLOSE);
    }
    if (digitalRead(SWITCH_1_PIN) == LOW)
    {
        // open gripper
        g_1_s_1.write(GRIPPER_1_OPEN);
        g_1_s_2.write(180 - GRIPPER_1_OPEN);
    }
    // GRIPPER 1 (ROT SOFT)
    if (digitalRead(SWITCH_2_PIN) == HIGH)
    {
        Serial.print("2, ");
    }
    if (digitalRead(SWITCH_2_PIN) == LOW)
    {
        Serial.print("1, ");
    }
    // GRIPPER 1 (ROT LIN)
    if (digitalRead(SWITCH_3_PIN) == HIGH)
    {
        Serial.print("3, ");
    }
    if (digitalRead(SWITCH_3_PIN) == LOW)
    {
        Serial.print("1, ");
    }
    // GRIPPER 1 (LIN)
    if (digitalRead(SWITCH_4_PIN) == HIGH)
    {
        Serial.print("4, ");
    }
    if (digitalRead(SWITCH_4_PIN) == LOW)
    {
        Serial.print("1, ");
    }
    // GRIPPER 1 (LIN V2)
    if (digitalRead(SWITCH_5_PIN) == HIGH)
    {
        Serial.print("5, ");
    }
    if (digitalRead(SWITCH_5_PIN) == LOW)
    {
        Serial.print("1, ");
    }
}