//
// Arduino Uno project
//
// Game to practice numbers and speed
// - display shows 1..9 lines. User needs to select same number with keypad
// - program displays the speed for the number selection
// - intended for child who is learning basic numbers
//
// circuit: https://www.tinkercad.com/things/cR6FBUxwbxK-knowyournumber

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <IRremoteInt.h>

enum GameState {AskNumber, ShowResults};

const int lcd_RS_PIN = 2;
const int lcd_E_PIN = 3;
const int lcd_D4_PIN = 4;
const int lcd_D5_PIN = 5;
const int lcd_D6_PIN = 6;
const int lcd_D7_PIN = 7;

const int ir_receiver_PIN = 9;

const int led_red_PIN = 11;
const int led_green_PIN = 12;

LiquidCrystal lcd(lcd_RS_PIN, lcd_E_PIN, lcd_D4_PIN, lcd_D5_PIN, lcd_D6_PIN, lcd_D7_PIN);
IRrecv irReceiver(ir_receiver_PIN);
decode_results irResults;
GameState gameState = GameState::AskNumber;
int numberOfLines;
unsigned long startTime;

void StartGame() {
    gameState = GameState::AskNumber;
    lcd.clear();
    startTime = millis();

    numberOfLines = random(1, 9);
    if(numberOfLines == 1) lcd.print("I");
    else if(numberOfLines == 2) lcd.print("II");
    else if(numberOfLines == 3) lcd.print("III");
    else if(numberOfLines == 4) lcd.print("IIII");
    else if(numberOfLines == 5) lcd.print("IIIII");
    else if(numberOfLines == 6) lcd.print("I IIIII");
    else if(numberOfLines == 7) lcd.print("II IIIII");
    else if(numberOfLines == 8) lcd.print("III IIIII");
    else if(numberOfLines == 9) lcd.print("IIII IIIII");
}

int GetNumberFromKeypad() {
    int keyNumber;

    switch (irResults.value) {
        case 0xFF30CF: keyNumber = 1; break;
        case 0xFF18E7: keyNumber = 2; break;
        case 0xFF7A85: keyNumber = 3; break;
        case 0xFF10EF: keyNumber = 4; break;
        case 0xFF38C7: keyNumber = 5; break;
        case 0xFF5AA5: keyNumber = 6; break;
        case 0xFF42BD: keyNumber = 7; break;
        case 0xFF4AB5: keyNumber = 8; break;
        case 0xFF52AD: keyNumber = 9; break;
        default: keyNumber = 0;
    }

    return keyNumber;
}

void CorrectAnswer()
{
    lcd.clear();
    lcd.print("OIKEIN!!!\ntime: ");
    lcd.print(millis() - startTime);
    lcd.print(" msec");
    digitalWrite(led_green_PIN, HIGH);

    gameState = GameState::ShowResults;

    delay(1000);
}

void WrongAnswer()
{
    digitalWrite(led_red_PIN, HIGH);
    delay(1000);
    digitalWrite(led_red_PIN, LOW);
}

void setup()
{
    pinMode(led_red_PIN, OUTPUT);
    pinMode(led_green_PIN, OUTPUT);

    lcd.begin(16, 2);
    irReceiver.enableIRIn();

    randomSeed(analogRead(0));
    StartGame();
}

void loop()
{
    if(irReceiver.decode(&irResults)) {
        if(gameState == GameState::AskNumber){
            if(GetNumberFromKeypad() == numberOfLines){
                CorrectAnswer();
            } else {
                WrongAnswer();
            }
        }
        else {
            StartGame();
        }
      irReceiver.resume();
    }
}
