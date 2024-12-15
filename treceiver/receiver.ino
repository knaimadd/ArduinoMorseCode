#include <LiquidCrystal.h> // library for display

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // initializing display pins

int unit = 300; // base unit
int smallUnit = unit/10; // length of 1 loop iteration

int modePin = 8;
int modeDiodePin = 12;
int buttonPin = 10;
int diodePin = 13;

int photoBoundary = 450; // boundary of changing analog signal from photoresistor to digital signal (below - on; above - off)

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // display initialization
  pinMode(modePin, INPUT);
  pinMode(modeDiodePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(diodePin, OUTPUT);
  pinMode(A0, INPUT);
  lcd.blink(); // blinking cursor on display
}

int source = -1; // mode; 1 - button, -1 - photoresistor 
int signalState; // morse signal; 0 - off, 1 - on
int onState = 0; // time of on signal
int offState = 0; // time of off signal

String morse = ""; // morse code for current letter
String sentance = ""; // morse code for current sentance

int lcdRowLen = 0; // current column on display
int lcdRow = 0; // current row on display
bool wasSpace = 1; // checks if last sign wast space

void loop() {
  int modeLen = 0; // length of holding mode changing button
  while (digitalRead(modePin)) {
    modeLen++;
    if (modeLen >= 100) { // calibration if holdign for long enough (1 second)
      Serial.println("Calibration.");
      calibration();
      modeLen = 0;
    } 
    delay(10);
  }
  if (modeLen >= 1) { // else changing mode/source
    source = -source;
    Serial.println("mode change");  
  }
  digitalWrite(modeDiodePin, -(source-1)/2);
  
  signalState = getSignal(source); // getting signal from button or photoresistor
  if (signalState == 1) { // signal on
    digitalWrite(diodePin, 1);
    offState = 0;
    onState++;
    if (onState == 6) { // auxilary prints to use while using button as signal source
      Serial.println('.');
    }
    else if (onState == 23) {
      Serial.println('-');
    }
    else if (onState == 15 || onState == 38){
      Serial.println("none");
    }
    else if (onState == 96) { // reset sentance if held long enough
      morse = "";
      sentance = "";
      lcdRowLen = 0;
      lcdRow = 0;
      Serial.println("Reset");
      lcd.clear();
      lcd.setCursor(0, 0);
    }
  }
  else { // signal off
    digitalWrite(diodePin, 0);
    if (offState == 0) { // auxilary prints
      Serial.print("Sign: ");
      morse += signalToMorse(onState);
      morse.replace(" ", "");
      Serial.println(morse);
    }
    if (offState == 26) { // end of character
      String letter = morseToLetter(morse);
      if (letter != ""){ // if morse code string is valid
        Serial.println(letter);
        offState = 0;
        sentance += letter;
        lcdRowLen++;
        char l = letter[0];
        lcd.write(l); // writing character on display
        lcd.setCursor(lcdRowLen, lcdRow);
        morse = ""; // clearing variables 
        wasSpace = 0; 
      }
      morse = "";
    }
    if (offState == 66) { // space characters
      offState = 0;
      sentance += " ";
      if (!wasSpace) { // checks if last character was space; if yes then writes nothing and waits for new signal
        lcdRowLen++;
        lcd.write(' ');
        lcd.setCursor(lcdRowLen, lcdRow);
      }
      morse = " ";
      wasSpace = 1;
    }
    onState = 1;
    offState++;
  }

  if (lcdRow == 0 && lcdRowLen == 16) { // if display run out of space on row 0, go to row 1
    lcd.setCursor(0, 1);
    lcdRow = 1;
    lcdRowLen = 0;
  }
  if (lcdRow == 1 && lcdRowLen >= 16) { // if display run out of space on row 1, clear display and go to row 0
    lcd.setCursor(0, 0);
    lcd.clear();
    lcdRow = 0;
    lcdRowLen = 0;
  }
  delay(smallUnit);
}

// reading signal from given source
int getSignal(int source) {
  if (source == 1) {
    return digitalRead(buttonPin);
  }
  else {
    int photoSignal = analogRead(A0);
    if (photoSignal >= photoBoundary) {
      return 0;
    }
    else {
      return 1;
    }
  }
}

// calibration; waiting for 3 short signals
void calibration() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("calibration:");
  int s1 = 0;
  while (!getSignal(source)) {}
  while (getSignal(source)) {
    s1++;
    digitalWrite(diodePin, 1);
    delay(10);
  }
  digitalWrite(diodePin, 0);
  lcd.write('1');
  int s2 = 0;
  while (!getSignal(source)) {}
  while (getSignal(source)) {
    s2++;
    digitalWrite(diodePin, 1);
    delay(10);
  }
  digitalWrite(diodePin, 0);
  lcd.write('2');
  int s3 = 0;
  while (!getSignal(source)) {}
  while (getSignal(source)) {
    s3++;
    digitalWrite(diodePin, 1);
    delay(10);
  }
  digitalWrite(diodePin, 0);
  lcd.write('3');
  unit = (s1 + s2 + s3 - 3)/3*10;
  smallUnit = unit/10;
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcdRow = 0;
  lcdRowLen = 0;
  Serial.println(unit);
}

// translating digital signals from button or photoresistor into morse ("." or "-")
char signalToMorse(int onState) {
  if (onState >= 6 && onState <= 14) {
    return '.';
  }
  else if (onState >= 23 && onState <= 37) {
    return '-';
  }
  else {
    return ' ';
  }
}

// translating morse code strings into letters
String morseToLetter(String morse){
  if (morse == ".-") {
    return "A";
  }
  else if (morse == "-...") {
    return "B";
  }
  else if (morse == "-.-.") {
    return "C";
  }
  else if (morse == "-..") {
    return "D";
  }
  else if (morse == ".") {
    return "E";
  }
  else if (morse == "..-.") {
    return "F";
  }
  else if (morse == "--.") {
    return "G";
  }
  else if (morse == "....") {
    return "H";
  }
  else if (morse == "..") {
    return "I";
  }
  else if (morse == ".---") {
    return "J";
  }
  else if (morse == "-.-") {
    return "K";
  }
  else if (morse == ".-..") {
    return "L";
  }
  else if (morse == "--") {
    return "M";
  }
  else if (morse == "-.") {
    return "N";
  }
  else if (morse == "---") {
    return "O";
  }
  else if (morse == ".--.") {
    return "P";
  }
  else if (morse == "--.-") {
    return "Q";
  }
  else if (morse == ".-.") {
    return "R";
  }
  else if (morse == "...") {
    return "S";
  }
  else if (morse == "-") {
    return "T";
  }
  else if (morse == "..-") {
    return "U";
  }
  else if (morse == "...-") {
    return "V";
  }
  else if (morse == ".--") {
    return "W";
  }
  else if (morse == "-..-") {
    return "X";
  }
  else if (morse == "-.--") {
    return "Y";
  }
  else if (morse == "--..") {
    return "Z";
  }
  else if (morse == ".----") {
    return "1";
  }
  else if (morse == "..---") {
    return "2";
  }
  else if (morse == "...--") {
    return "3";
  }
  else if (morse == "....-") {
    return "4";
  }
  else if (morse == ".....") {
    return "5";
  }
  else if (morse == "-....") {
    return "6";
  }
  else if (morse == "--...") {
    return "7";
  }
  else if (morse == "---..") {
    return "8";
  }
  else if (morse == "----.") {
    return "9";
  }
  else if (morse == "-----") {
    return "0";
  }
  else {
    return "";
  }
}


