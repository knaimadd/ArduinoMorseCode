int unit = 300; //length of base unit; short signal = unit; long signal = 3*unit; space = 7 unit 

int diodePin = 13;
int soundPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  String word =  readWord(); //reading word from serial monitor
  if (word == "?") { // calibration
    Serial.println("Calibration, set base unit length [ms]:");
    while (Serial.available() == 0) {} // waiting on base unit length 
    unit = Serial.readString().toInt();
    Serial.print("Base unit set to: ");
    Serial.print(unit);
    Serial.println(" ms");
    delay(500);
    Serial.println("Begin sending calibration signal?");
    while (Serial.available() == 0) {} //waiting on confirmation to begin sending signal
    delay(200);
    morseToSound(".", 400);
    delay(unit);
    morseToSound(".", 400);
    delay(unit);
    morseToSound(".", 400);
    delay(unit);
    readWord();
    Serial.println("Calibration ended succesfully!");
  }
  else {
    wordToSound(word, 400); // translating word from serial monitor to signal on diode and speaker
  }
}

// function that reads string from Serial monitor
String readWord() {
  while (Serial.available() == 0) {}
  String word = Serial.readString();
  word.trim();
  word.toUpperCase(); 
  return word;
}

// translating morse signal as string into signal from speaker and diode
void morseToSound(String morse, int frequency) {
  for (auto sign: morse) {
    switch (sign) {
      case '.':
        tone(soundPin, frequency);
        digitalWrite(diodePin, 1);
        delay(unit);
        break;
      case '-':
        tone(soundPin, frequency);
        digitalWrite(diodePin, 1);
        delay(3*unit);
        break;
      case ' ':
        delay(4*unit);
        break;
    }
    noTone(soundPin);
    digitalWrite(diodePin, 0);
    delay(unit);
  }
}

// translation of word as string into letters and then morse signals from speaker and diode
void wordToSound(String word, int frequency) {
  for (auto letter: word) {
    String morse = letterToMorse(letter);
    if (letter == ' ') {
      Serial.println();
    }
    else {
      Serial.print(letter);
      Serial.print(": ");
      Serial.println(morse);
    }
    morseToSound(morse, frequency);
    delay(2*unit);
  }
}

// translation of letters into morse signals as strings
String letterToMorse(char letter){
  switch (letter) {
    case 'A':
      return ".-";
    case 'B':
      return "-...";
    case 'C':
      return "-.-.";
    case 'D':
      return "-..";
    case 'E':
      return ".";
    case 'F':
      return "..-.";
    case 'G':
      return "--.";
    case 'H':
      return "....";
    case 'I':
      return "..";
    case 'J':
      return ".---";
    case 'K':
      return "-.-";
    case 'L':
      return ".-..";
    case 'M':
      return "--";
    case 'N':
      return "-.";
    case 'O':
      return "---";
    case 'P':
      return ".--.";
    case 'Q':
      return "--.-";
    case 'R':
      return ".-.";
    case 'S':
      return "...";
    case 'T':
      return "-";
    case 'U':
      return "..-";
    case 'V':
      return "...-";
    case 'W':
      return ".--";
    case 'X':
      return "-..-";
    case 'Y':
      return "-.--";
    case 'Z':
      return "--..";
    case '1':
      return ".----";
    case '2':
      return "..---";
    case '3':
      return "...--";
    case '4':
      return "....-";
    case '5':
      return ".....";
    case '6':
      return "-....";
    case '7':
      return "--...";
    case '8':
      return "---..";
    case '9':
      return "----.";
    case '0':
      return "-----";
    case ' ':
      return " ";
  }
}

