#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//Declaring Varibles
int chooseFloor = 0;
char button_state;
int count = 0;
int count1 = 0;
int count2 = 0;

bool activateButton = false;
bool activateScroll = false;
bool hourSelect = false;
bool minuteSelect = false;


int menuNumber = 0;
int menuSelected = 0;

int chooseRoom = 0;
int chooseRoom1 = 0;
int chooseItem1 = 0;
int chooseItem2 = 0;
int chooseItem3 = 0;
int chooseItem4 = 0;
int chooseItem5 = 0;
int chooseItem6 = 0;

int levelControl[12];

int chosenElement;
int chosenElement2;
int levelValue;
int chooseOption;

int Hour;
int Minute;
int ON;
int mark = 0;


#ifdef __arm__
extern"C" char* sbrk(int incr);
#else// __ARM__
extern char *__brkval;
#endif// __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Home Controller");
  homeMenu();
  Serial.println("ENHANCED,QUERY,MEMORY,");



  levelControl[0] = EEPROM.read(0);

  levelControl[1] = EEPROM.read(1);

  levelControl[2] = EEPROM.read(2);

  levelControl[3] = EEPROM.read(3);

  levelControl[4] = EEPROM.read(4);

  levelControl[5] = EEPROM.read(5);

  levelControl[6] = EEPROM.read(6);

  levelControl[7] = EEPROM.read(7);

  levelControl[8] = EEPROM.read(8);

  levelControl[9] = EEPROM.read(9);

  levelControl[10] = EEPROM.read(10);

  levelControl[11] = EEPROM.read(11);

}


void loop() {

  while (Serial.available() > 0) {
    String input = Serial.readString();

    if (input.equals("M\n")) {
      Serial.println(freeMemory());
    } else {
      if (input.equals("Q\n")) {
        storedData();
      }
    }
  }


  button_state = lcd.readButtons();

  if (button_state & BUTTON_UP) {
    if (hourSelect == true) {
      if (count1 > 0) {
        count1 = count1 - 1;
        allMenus();
        delay(100);
      }
    }
    if (minuteSelect == true) {
      if (count2 > 0) {
        count2 = count2 - 1;
        allMenus();
        delay(100);
      }
    }
    if ( activateScroll == true) {
      count--;
      allMenus();
      delay(100);
    }
  }

  if (button_state & BUTTON_DOWN) {
    if (hourSelect == true) {
      if (count1 < 23) {
        count1 = count1 + 1;
        allMenus();
        delay(100);
      }
    }
    if (minuteSelect == true) {
      if (count2 < 59) {
        count2 = count2 + 1;
        allMenus();
        delay(100);
      }
    }
    if ( activateScroll == true) {
      count++;
      allMenus();
      delay(100);
    }
  }

  if (activateButton == true) {
    if (button_state & BUTTON_RIGHT) {
      if (levelControl[chosenElement] < 100) {
        levelControl[chosenElement] = (levelControl[chosenElement]) + 1;
        EEPROM.update(chosenElement, levelControl[chosenElement]);
        allMenus();
        delay(300);
      }
    }
  }
  if (activateButton == true) {
    if (button_state & BUTTON_LEFT) {
      if (levelControl[chosenElement] > 0) {
        levelControl[chosenElement] = (levelControl[chosenElement]) - 1;
        EEPROM.update(chosenElement, levelControl[chosenElement]);
        allMenus();
        delay(300);
      }
    }
  }

  if (button_state & BUTTON_SELECT) {
    if (activateButton == true) {
      menuSelected = 0;
      activateButton = false;
    }
    if (minuteSelect == true) {
      minuteSelect = false;
    }
    if ( hourSelect == true && mark == 1) {
      hourSelect = false;
      minuteSelect = true;
    }

    menuNumber = menuSelected;
    activateScroll = false;
    count = 1;
    delay(300);
    allMenus();

  }

  if (count > 3) {
    count = 3;
  }
  if (count < 1) {
    count = 1;
  }
}



void allMenus() {

  switch (menuNumber) {
    case 0:
      homeMenu();
      break;

    case 1:
      groundFloorMenu();
      break;

    case 2:
      firstFloorMenu();
      break;

    case 3:
      kitchen();
      break;

    case 4:
      hall();
      break;

    case 5:
      livingRoom();
      break;

    case 6:
      bedRoom1();
      break;

    case 7:
      bedRoom2();
      break;

    case 8:
      bathRoom();
      break;

    case 9:
      storedData();
      break;

    case 10:
      options();
      break;

    case 11:
      level(chosenElement);
      break;

    case 12:
      Time(ON, chosenElement);
      break;

  }
}



void homeMenu () {
  activateScroll = true;
  chooseFloor = count;
  switch (chooseFloor) {

    case 0:
      chooseFloor = 1;
      count = 1;
      break;

    case 1: //Ground Floor
      lcd.clear();
      lcd.print(">Ground Floor<");
      lcd.setCursor(0, 1);
      lcd.print(" First Floor");
      menuSelected = 1;
      break;

    case 2: //Second Floor
      lcd.clear();
      lcd.print(">First Floor<");
      lcd.setCursor(0, 1);
      lcd.print(" Stored Data");
      menuSelected = 2;
      break;


    case 3:
      lcd.clear();
      lcd.print(" First Floor");
      lcd.setCursor(0, 1);
      lcd.print(">Stored Data<");
      menuSelected = 9;
      break;

    case 4:
      chooseFloor = 3;
      count = 3;
      break;
  }
}

void groundFloorMenu() {
  activateScroll = true;
  chooseRoom = count;
  switch (chooseRoom) {

    case 0:
      chooseRoom = 1;
      count = 1;
      break;

    case 1: //Kitchen
      lcd.clear();
      lcd.print(">Kitchen<");
      lcd.setCursor(0, 1);
      lcd.print(" Hall");
      menuSelected = 3;
      break;

    case 2: //Hall
      lcd.clear();
      lcd.print(">Hall<");
      lcd.setCursor(0, 1);
      lcd.print(" Living Room");
      menuSelected = 4;
      break;

    case 3: //Living Room
      lcd.clear();
      lcd.print(" Hall");
      lcd.setCursor(0, 1);
      lcd.print(">Living Room<");
      menuSelected = 5;
      break;

    case 4:
      chooseRoom = 3;
      count = 3;
      break;
  }

}

void firstFloorMenu() {
  activateScroll = true;
  chooseRoom1 = count;

  switch (chooseRoom1) {

    case 0:
      chooseFloor = 1;
      count = 1;
      break;

    case 1: //Bedroom1
      lcd.clear();
      lcd.print(">Bedroom 1<");
      lcd.setCursor(0, 1);
      lcd.print(" Bedroom 2");
      menuSelected = 6;
      break;

    case 2: //Bedroom2
      lcd.clear();
      lcd.print(">Bedroom 2<");
      lcd.setCursor(0, 1);
      lcd.print(" Bathroom");
      menuSelected = 7;
      break;

    case 3: //Bathroom
      lcd.clear();
      lcd.print(" Bedroom 2");
      lcd.setCursor(0, 1);
      lcd.print(">Bathroom<");
      menuSelected = 8;
      break;

    case 4:
      chooseRoom1 = 3;
      count = 3;
      break;
  }
}

void kitchen () {
  activateScroll = true;
  chooseItem1 = count;
  switch (chooseItem1) {

    case 0:
      chooseItem1 = 1;
      count = 1;
      break;

    case 1: //Kitchen Light
      lcd.clear();
      lcd.print(">Light<");
      lcd.setCursor(0, 1);
      lcd.print(" Heat");
      menuSelected = 10;
      chosenElement = 0;
      chosenElement2 = 12;

      break;

    case 2: //Kitchen Heat
      lcd.clear();
      lcd.print(" Light");
      lcd.setCursor(0, 1);
      lcd.print(">Heat<");
      menuSelected = 10;
      chosenElement = 1;
      chosenElement2 = 15;

      break;
    case 3:
      chooseItem1 = 2;
      count = 2;
      break;

  }
}

void hall() {
  activateScroll = true;
  chooseItem2 = count;
  switch (chooseItem2) {


    case 0:
      chooseItem2 = 1;
      break;

    case 1: //Hall Light
      lcd.clear();
      lcd.print(">Light<");
      lcd.setCursor(0, 1);
      lcd.print(" Heat");
      menuSelected = 10;
      chosenElement = 2;
      chosenElement2 = 18;
      break;

    case 2: //Hall Heat
      lcd.clear();
      lcd.print(" Light");
      lcd.setCursor(0, 1);
      lcd.print(">Heat<");
      menuSelected = 10;
      chosenElement = 3;
      chosenElement2 = 21;
      break;

    case 3:
      chooseItem2 = 2;
      count = 2;
      break;

  }
}

void livingRoom() {
  activateScroll = true;
  chooseItem3 = count;
  switch (chooseItem3) {
    case 0:
      chooseItem3 = 1;
      break;

    case 1: //Livingroom Light
      lcd.clear();
      lcd.print(">Light<");
      lcd.setCursor(0, 1);
      lcd.print(" Heat");
      menuSelected = 10;
      chosenElement = 4;
      chosenElement2 = 24;
      break;

    case 2: //Livingroom Heat
      lcd.clear();
      lcd.print(" Light");
      lcd.setCursor(0, 1);
      lcd.print(">Heat<");
      menuSelected = 10;
      chosenElement = 5;
      chosenElement2 = 27;
      break;

    case 3:
      chooseItem3 = 2;
      count = 2;
      break;

  }
}



void bedRoom1() {
  activateScroll = true;
  chooseItem4 = count;
  switch (chooseItem4) {
    case 0:
      chooseItem4 = 1;
      break;

    case 1: //Bedroom 1 Light
      lcd.clear();
      lcd.print(">Light<");
      lcd.setCursor(0, 1);
      lcd.print(" Heat");
      menuSelected = 10;
      chosenElement = 6;
      chosenElement2 = 30;
      break;

    case 2: //Bedroom 1 Heat
      lcd.clear();
      lcd.print(" Light");
      lcd.setCursor(0, 1);
      lcd.print(">Heat<");
      menuSelected = 10;
      chosenElement = 7;
      chosenElement2 = 33;
      break;

    case 3:
      chooseItem4 = 2;
      count = 2;
      break;

  }
}


void bedRoom2() {
  activateScroll = true;
  chooseItem5 = count;
  switch (chooseItem5) {

    case 0:
      chooseItem5 = 1;
      break;

    case 1: //Bedroom 2 Light
      lcd.clear();
      lcd.print(">Light<");
      lcd.setCursor(0, 1);
      lcd.print(" Heat");
      menuSelected = 10;
      chosenElement = 8;
      chosenElement2 = 36;
      break;

    case 2: //Bedroom 2 Heat
      lcd.clear();
      lcd.print(" Light");
      lcd.setCursor(0, 1);
      lcd.print(">Heat<");
      menuSelected = 10;
      chosenElement = 9;
      chosenElement2 = 39;
      break;

    case 3:
      chooseItem5 = 2;
      count = 2;
      break;

  }
}
void bathRoom() {
  activateScroll = true;
  chooseItem6 = count;
  switch (chooseItem6) {
    case 0:
      chooseItem6 = 1;
      break;

    case 1: //Bathroom Light
      lcd.clear();
      lcd.print(">Light<");
      lcd.setCursor(0, 1);
      lcd.print(" Heat");
      menuSelected = 10;
      chosenElement = 10;
      chosenElement2 = 42;

      break;

    case 2: //Bathroom Heat
      lcd.clear();
      lcd.print(" Light");
      lcd.setCursor(0, 1);
      lcd.print(">Heat<");
      menuSelected = 10;
      chosenElement = 11;
      chosenElement2 = 45;
      break;

    case 3:
      chooseItem6 = 2;
      count = 2;
      break;

  }
}

void options() {
  activateScroll = true;
  chooseOption = count;
  switch (chooseOption) {
    case 1:// level
      lcd.clear();
      lcd.print(">Level<");
      lcd.setCursor(0, 1);
      lcd.print(" ON Time");
      menuSelected = 11;
      hourSelect = false;


      break;

    case 2://ON Time
      lcd.clear();
      lcd.print(">ON Time<");
      lcd.setCursor(0, 1);
      lcd.print(" OFF Time");
      menuSelected = 12;
      ON = 1;
      hourSelect = true;
      chosenElement = chosenElement2;

      break;

    case 3:// OFF Time
      lcd.clear();
      lcd.print(" ON Time");
      lcd.setCursor(0, 1);
      lcd.print(">OFF Time<");
      menuSelected = 12;
      ON = 0;
      hourSelect = true;
      chosenElement = chosenElement2;

      break;


  }
}

int level(int chosenElement) {
  activateButton = true;
  lcd.clear();
  lcd.print("- Level +");
  lcd.setCursor(0, 1);
  lcd.print("< ");
  lcd.print(levelControl[chosenElement]);
  lcd.print(" >");

}

int Time(int ON, int chosenElement) {
  mark = 1;
  Hour = count1;
  Minute = count2;

  lcd.clear();
  if (Hour < 10) {
    lcd.print("0");
  }
  lcd.print(Hour);
  lcd.print(":");
  if (Minute < 10) {
    lcd.print("0");
  }
  lcd.print(Minute);
  lcd.setCursor(0, 1);
  lcd.print("UP DOWN Select");

  if ( hourSelect == false && minuteSelect == false) {
    EEPROM.update(chosenElement, ON);
    EEPROM.update((chosenElement + 1), Hour);
    EEPROM.update( (chosenElement + 2), Minute);
    count1 = 0;
    count2 = 0;
    mark = 0;
    menuNumber = 0;
    allMenus();
  }

}

void storedData() {

  Serial.print(F("Ground / Kitchen / Light / Main / Level : " ));
  Serial.println(EEPROM.read(0));

  Serial.print(F("Ground / Kitchen / Light / Main / "));
  if (EEPROM.read(12) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(13) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(13));
  Serial.print(":");
  if (EEPROM.read(14) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(14));

  Serial.print(F("Ground / Kitchen / Heat / Main / Level : "));
  Serial.println(EEPROM.read(1));

  Serial.print(F("Ground / Kitchen / Heat / Main / "));
  if (EEPROM.read(15) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(16) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(16));
  Serial.print(":");
  if (EEPROM.read(17) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(17));

  Serial.print(F("Ground / Hall / Light / Main / Level : "));
  Serial.println(EEPROM.read(2));

  Serial.print(F("Ground / Hall / Light / Main / "));
  if (EEPROM.read(18) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(19) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(19));
  Serial.print(":");
  if (EEPROM.read(20) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(20));

  Serial.print(F("Ground / Hall / Heat / Main / Level : "));
  Serial.println(EEPROM.read(3));

  Serial.print(F("Ground / Hall / Heat / Main / "));
  if (EEPROM.read(21) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(22) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(22));
  Serial.print(":");
  if (EEPROM.read(23) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(23));

  Serial.print(F("Ground / Livingroom / Light / Main / Level : "));
  Serial.println(EEPROM.read(4));

  Serial.print(F("Ground / Livingroom / Light / Main / "));
  if (EEPROM.read(24) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(25) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(25));
  Serial.print(":");
  if (EEPROM.read(26) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(26));

  Serial.print(F("Ground / Livingroom / Heat / Main / Level : "));
  Serial.println(EEPROM.read(5));

  Serial.print(F("Ground / Livingroom / Heat / Main / "));
  if (EEPROM.read(27) == 1) {
    Serial.print("ON  : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(28) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(28));
  Serial.print(":");
  if (EEPROM.read(29) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(29));

  Serial.print(F("First / Bedroom1 / Light / Main / Level : "));
  Serial.println(EEPROM.read(6));

  Serial.print(F("First / Bedroom1 / Light / Main / "));
  if (EEPROM.read(30) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(31) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(31));
  Serial.print(":");
  if (EEPROM.read(32) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(32));

  Serial.print(F("First / Bedroom1 / Heat / Main / Level : "));
  Serial.println(EEPROM.read(7));

  Serial.print(F("First / Bedroom1 / Heat / Main / "));
  if (EEPROM.read(33) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(34) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(34));
  Serial.print(":");
  if (EEPROM.read(35) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(35));

  Serial.print(F("First / Bedroom2 / Light / Main / Level : "));
  Serial.println(EEPROM.read(8));

  Serial.print(F("First / Bedroom2 / Light / Main / "));
  if (EEPROM.read(36) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(37) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(37));
  Serial.print(":");
  if (EEPROM.read(38) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(38));

  Serial.print(F("First / Bedroom2 / Heat / Main / Level : "));
  Serial.println(EEPROM.read(9));

  Serial.print(F("First / Bedroom2 / Heat / Main / "));
  if (EEPROM.read(39) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(40) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(40));
  Serial.print(":");
  if (EEPROM.read(41) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(41));

  Serial.print(F("First / Bathroom / Light / Main / Level : "));
  Serial.println(EEPROM.read(10));

  Serial.print(F("First / Bathroom / Light / Main / "));
  if (EEPROM.read(42) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(43) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(43));
  Serial.print(":");
  if (EEPROM.read(44) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(44));

  Serial.print(F("First / Bathroom / Heat / Main / Level : "));
  Serial.println(EEPROM.read(11));

  Serial.print(F("First / Bathroom / Heat / Main / "));
  if (EEPROM.read(45) == 1) {
    Serial.print("ON : ");
  } else {
    Serial.print("OFF : ");
  }
  if (EEPROM.read(46) < 10) {
    Serial.print("0");
  }
  Serial.print(EEPROM.read(46));
  Serial.print(":");
  if (EEPROM.read(47) < 10) {
    Serial.print("0");
  }
  Serial.println(EEPROM.read(47));


  menuNumber = 0;
  allMenus();
}
