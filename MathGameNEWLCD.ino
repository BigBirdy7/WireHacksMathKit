#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,  16, 2);
#include <Keypad.h>
#include <math.h>

//determing which pins corespond to which keypad keys
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {7,8,9,10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {11,12,13}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//setting up screen

int operation;
int num1;
int num2;
char operationchar = "+";
int answer;
int answerentered =0;
int numberplace = 0;
int inputarray[3];
int typer;
int points = 0;

void setup(){
  lcd.init(); //initialize the lcd
  lcd.backlight();
  randomSeed(analogRead(1));
   Serial.begin(9600);
  
}

int addition(){
  num1 = random(0,101);
  num2 = random(0,101);
  answer = num1+num2;
  operationchar = '+';
}

int subtraction(){
  num1 = random(0,101);
  num2 = random(0,num1+1);
  answer = num1-num2;
  operationchar = '-';
}

int multiplication(){

  num1 = random(0,11);
  num2 = random(0,11);
  answer = num1*num2;
  operationchar = 'x';
}




void loop(){
  operation = random(0,3); // 0 = addition 1 = subtraction 2= multiplication

  if (operation == 0){
    addition();
  }

  if(operation == 1){
    subtraction();
  }

  if (operation == 2){
    multiplication();
  }
  
  //output to monitor
  Serial.println("operation");
  Serial.println(operation);
  Serial.println("num1");
  Serial.println(num1);
  Serial.println("num2");
  Serial.println(num2);
  Serial.println("answer");
  Serial.println(answer);
  lcd.setCursor(0, 0);
  lcd.print(num1);
  lcd.setCursor(3, 0);
  lcd.print(operationchar);
  lcd.setCursor(5, 0);
  lcd.print(num2);
  

while (answerentered == 0) {
    
    char key = keypad.waitForKey();
    //Serial.print("Key Input =>");
    Serial.println(key);
    if (key) {
      if (key == '#') {
          answerentered = 1;}
        }
      if (key == '*'){
        if (numberplace > 0) {
          numberplace--; // Move back one position
          typer /= 10;   // Remove the last digit
          lcd.setCursor(0, 1);
          lcd.print("   "); // Clear the second line
          lcd.setCursor(0, 1);
          if (typer == 0){
            lcd.print("   ");
          }
          else{
            Serial.print("backspaceKey0clear");
            lcd.print(typer);
          }
        }
      }
      else{
        if (numberplace < 3 && key != '#') { // Prevent buffer overflow
          inputarray[numberplace] = key - '0'; // Convert char to int
          numberplace++;
          typer = typer * 10 + (key - '0'); // Build the number

          // Display the current input
          lcd.setCursor(0, 1);
          lcd.print(typer);
          Serial.print("normalkey:");
          Serial.println(typer);
      }
    }
  }

  if (answer == typer){
    lcd.setCursor(5, 1);
    lcd.print("Correct!");
    delay(800);
    answerentered = 0;
    points++;
    if (points == 99){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You win!!! Show");
      lcd.setCursor(0, 1);
      lcd.print("ivan for prize");
      delay(20000);
      points = 0;
    }
  }
  else {
    lcd.setCursor(5, 1);
    lcd.print("Wrong");
    delay(800);
    answerentered = 0;
    points--;
  }
  lcd.clear();
  lcd.setCursor(14, 1);
  lcd.print(points);
  numberplace = 0;
  typer = 0;
}//i want to sleep