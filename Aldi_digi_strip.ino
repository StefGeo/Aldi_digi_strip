// Drive the Strips with TS1829 available at aldi sued with custom Modes
// S. Groetsch  v0.10 24. 11. 2013

#include "pfodParser.h"  //  see http://www.forward.com.au/pfod/pfodParserLibraries/index.html
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h> // NB!! Modivied version from 
                              // 1 . https://github.com/chriszero/neobob
pfodParser parser;


// Wohnzimmer Einstellungen
int Pixelanz = 80;
int PWM1PIN=11;
int PWM2PIN=9;
uint8_t Aldipin= 6;

/*
// 1x1 m Mood light
int Pixelanz = 40;
int PWM1PIN=6;
int PWM2PIN=9;
uint8_t Aldipin= 8;
*/

const float pi=3.1459;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixelanz, Aldipin, NEO_GRB + NEO_KHZ800);

int32_t aldistrom(uint8_t strom_r,uint8_t strom_g,uint8_t strom_b) {
  uint8_t low;
  uint8_t med;
  uint16_t stromwort;
  if (strom_r <10) {strom_r=10;} else if (strom_r>41) {strom_r=41;} else {};
  if (strom_g <10) {strom_g=10;} else if (strom_g>41) {strom_g=41;} else {};
  if (strom_b <10) {strom_b=10;} else if (strom_b>41) {strom_b=41;} else {};
  strom_r-=10;
  strom_g-=10;
  strom_b-=10;
  stromwort= ((uint16_t) strom_r << 11) | ((uint16_t) strom_g << 6) | ((uint16_t) strom_b << 1);
  low = (uint8_t)(stromwort & 255);
  med = (uint8_t)(stromwort >> 8);
  return ((uint32_t)med << 16) | ((uint32_t)255 <<  8) | low;  // Stromübertragung  r =med, g=low, b=high
}




byte welle[] = {0,3,6,9	,12,16,19,22,25,28,31,34,37,40,43,46,49,51,54,57,60,63,65,68,71,73,76,78,81,83,85,88,
                90,92,94,96,98,100,102,104,106,107,109,111,112,113,115,116,117,118,120,121,122,122,123,124,
                125,125,126,126,126,127,127,127,127,127,127,127,126,126,126,125,125,124,123,122,122,121,120,118,
                      117	,	116	,	115	,	113	,	112	,	111	,	109	,	107	,
                      106	,	104	,	102	,	100	,	98	,	96	,	94	,	92	,
                      90	,	88	,	85	,	83	,	81	,	78	,	76	,	73	,
                      71	,	68	,	65	,	63	,	60	,	57	,	54	,	51	,
                      49	,	46	,	43	,	40	,	37	,	34	,	31	,	28	,
                      25	,	22	,	19	,	16	,	12	,	9	,	6	,	3	,
                      0	,	-3	,	-6	,	-9	,	-12	,	-16	,	-19	,	-22	,
                      -25	,	-28	,	-31	,	-34	,	-37	,	-40	,	-43	,	-46	,
                      -49	,	-51	,	-54	,	-57	,	-60	,	-63	,	-65	,	-68	,
                      -71	,	-73	,	-76	,	-78	,	-81	,	-83	,	-85	,	-88	,
                      -90	,	-92	,	-94	,	-96	,	-98	,	-100	,	-102	,	-104	,
                      -106	,	-107	,	-109	,	-111	,	-112	,	-113	,	-115	,	-116	,
                      -117	,	-118	,	-120	,	-121	,	-122	,	-122	,	-123	,	-124	,
                      -125	,	-125	,	-126	,	-126	,	-126	,	-127	,	-127	,	-127	,
                      -127	,	-127	,	-127	,	-127	,	-126	,	-126	,	-126	,	-125	,
                      -125	,	-124	,	-123	,	-122	,	-122	,	-121	,	-120	,	-118	,
                      -117	,	-116	,	-115	,	-113	,	-112	,	-111	,	-109	,	-107	,
                      -106	,	-104	,	-102	,	-100	,	-98	,	-96	,	-94	,	-92	,
                      -90	,	-88	,	-85	,	-83	,	-81	,	-78	,	-76	,	-73	,
                      -71	,	-68	,	-65	,	-63	,	-60	,	-57	,	-54	,	-51	,
                      -49	,	-46	,	-43	,	-40	,	-37	,	-34	,	-31	,	-28	,
                      -25	,	-22	,	-19	,	-16	,	-12	,	-9	,	-6	,	-3	,
                      0	,	3	,	6	,	9	,	12	,	16	,	19	,	22	,
                      25	,	28	,	31	,	34	,	37	,	40	,	43	,	46	,
                      49	,	51	,	54	,	57	,	60	,	63	,	65	,	68	,
                      71	,	73	,	76	,	78	,	81	,	83	,	85	,	88	,
                      90	,	92	,	94	,	96	,	98	,	100	,	102	,	104	,
                      106	,	107	,	109	,	111	,	112	,	113	,	115	,	116	,
                      117	,	118	,	120	,	121	,	122	,	122	,	123	,	124	,
                      125	,	125	,	126	,	126	,	126	,	127	,	127	,	127	,
                      127	,	127	,	127	,	127	,	126	,	126	,	126	,	125	,
                      125	,	124	,	123	,	122	,	122	,	121	,	120	,	118	,
                      117	,	116	,	115	,	113	,	112	,	111	,	109	,	107	,
                      106	,	104	,	102	,	100	,	98	,	96	,	94	,	92	,
                      90	,	88	,	85	,	83	,	81	,	78	,	76	,	73	,
                      71	,	68	,	65	,	63	,	60	,	57	,	54	,	51	,
                      49	,	46	,	43	,	40	,	37	,	34	,	31	,	28	,
                      25	,	22	,	19	,	16	,	12	,	9	,	6	,	3	,
 };


uint8_t fsr[]={200,};
uint8_t fsg[]={ 60,};
uint8_t fsb[]={  0,};
uint16_t fsd[]={ 0,  };

uint8_t fireloop=0;
int modus = 5;
char Settings = 'r';
byte rein;
uint8_t red,red2;
uint8_t green,green2;
uint8_t blue,blue2;
uint8_t pwm1,pwm2,pwm3;
uint16_t geschw=160;
uint8_t wavelength=Pixelanz/4;
unsigned long time,time2;
uint16_t phase=0;
uint16_t i;
uint8_t temp=0;
uint8_t hell=255;
uint8_t hell2;
byte* idxPtr;
long longResult;
byte krDirection;
byte krSize;
byte howMany;
byte krMode;

void einstellmenue() {
    // Colors and Speed Menue
        Serial.print("{.Colors and Speed");
        Serial.print("|r~red `")  ;Serial.print(red);    Serial.print("`254`0");
        Serial.print("|g~green `");Serial.print(green);  Serial.print("`254`0");
        Serial.print("|b~blue `") ;Serial.print(blue);   Serial.print("`254`0");
        Serial.print("|w~PWM1 `") ;Serial.print(pwm1);   Serial.print("`254`0");
        Serial.print("|c~PWM2 `") ;Serial.print(pwm2);   Serial.print("`254`0");
       // Serial.print("|i~PWM3 `") ;Serial.print(pwm3);   Serial.print("`254`0");
        Serial.print("|s~speed `") ;Serial.print(geschw); Serial.print("`254`0");
        Serial.print("|d~red2 `")  ;Serial.print(red2);   Serial.print("`254`0");
        Serial.print("|e~green2 `");Serial.print(green2); Serial.print("`254`0");
        Serial.print("|f~blue2 `") ;Serial.print(blue2);  Serial.print("`254`0");
        Serial.print("|a~wavelength `") ;Serial.print(wavelength);  Serial.print("`"); ;Serial.print(Pixelanz); Serial.print("`1");
        Serial.print("|.~Mainmenue}");
}

void mainmenue() {  Serial.print("{.Mainmenue|M~Modes|F~colors and speed|S~Save Presets |L~Load Presets|A~ on/off}");}
void modemenue() {  Serial.print("{.Modes|1~Static color|2~Color Change|3~Colorwheel pixels|4~colorbreathe|5~Fire|6~Single Color Wave|7~Dual Color Wave|8~Knight Rider 1|9~Knight Rider 2|0~Knight Rider 3|.~Mainmenue}");}
void savemenue() {  Serial.print("{.Save Presets|o~Save Preset 1 (default)|q~Save Preset 2|v~Save Preset 3|y~Save Preset 4|.~Mainmenue}"); }
void loadmenue() {  Serial.print("{.Load Presets|p~Load Preset 1(default)|t~Load Preset 2|x~Load Preset 3|z~Load Preset 4|.~Mainmenue}"); }


void setup() {
 i= EEPROM.read(200);
  if ( i!=123) { modus = 3;
         red =254;  green=180; blue=20;
         red2 =20;  green2=60; blue2=254;
         pwm1=20;    pwm2=20;  pwm3=125;
        
                } 
  else { load_mode(0);  } ; 
  pinMode(PWM1PIN,OUTPUT);
  pinMode(PWM2PIN,OUTPUT);
  parserSetup(); // initialize pfod Cmd parser
  strip.begin();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, aldistrom(20,20,20));}
  strip.show();
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  time= millis();
}

void loop() {
  byte cmd = 0;
  byte in = 0;
 
    if (Serial.available()) {
    in = Serial.read();
    cmd = parser.parse(in);
    }
    if (cmd != 0) {
      Serial.print("Cmd:");
      Serial.write(cmd);
      Serial.println();
      if        (cmd == ((byte)'M')) { modemenue();
      } else if (cmd == ((byte)'F')) { einstellmenue();
      } else if (cmd == ((byte)'S')) { savemenue();
      } else if (cmd == ((byte)'L')) { loadmenue();
      } else if (cmd == ((byte)'A')) {  // Toggle on/off
         if ((red + green + blue + pwm1+pwm2+pwm3) == 0) {
             load_mode(0);}
         else {
             red = 0;
             green=0;
             blue=0;
             pwm1=0;
             pwm2=0;
             pwm3=0;};
         mainmenue();  
         
      //switch Modes   
      } else if (cmd == (byte)'1') {modus = 1;    modemenue(); // static mode
      } else if (cmd == (byte)'2') {modus = 2;    modemenue(); red =254;green=254;blue=254;pwm1=20;pwm2=20;geschw=70;// Colorchange all
      } else if (cmd == (byte)'3') {modus = 3;    modemenue(); red =254;green=254;blue=254;pwm1=20;pwm2=20;geschw=40;// colorweel pixels
      } else if (cmd == (byte)'4') {modus = 4;    modemenue(); red =254;green=254;blue=254;pwm1=20;pwm2=20;geschw=40;// colorbreathe
      } else if (cmd == (byte)'5') {modus = 5;    modemenue(); // Fire
      } else if (cmd == (byte)'6') {modus = 6;    modemenue(); red =254;green=180;blue=30;geschw=30;// Single Color Wave
      } else if (cmd == (byte)'7') {modus = 7;    modemenue(); red =254;green=180;blue=30;red2=0;green2=0;blue2=254;geschw=4;// dual Color Wave     
      } else if (cmd == (byte)'8') {modus = 8;    modemenue(); red =254;green=180;blue=50;red2=0;green2=0;blue2=254;pwm1=20;pwm2=20;setupKnightRider(1, 4, 0);geschw=70;// Knight Rider
      } else if (cmd == (byte)'9') {modus = 8;    modemenue(); red =254;green=180;blue=50;red2=0;green2=0;blue2=254;pwm1=20;pwm2=20;setupKnightRider(1, 1, 0);geschw=40;// Knight Rider
      } else if (cmd == (byte)'0') {modus = 8;    modemenue(); red =254;green=254;blue=254;pwm1=20;pwm2=20;setupKnightRider(1, 1, 1);geschw=7;// Knight Rider
      // Slider Values
      } else if (cmd == (byte)'r') {red= read_slider();       einstellmenue(); // set red intensity
      } else if (cmd == (byte)'g') {green= read_slider();     einstellmenue(); // set green intensity
      } else if (cmd == (byte)'b') {blue= read_slider();      einstellmenue(); // set blue intensity
      } else if (cmd == (byte)'w') {pwm1= read_slider();      einstellmenue(); // set PWM1
      } else if (cmd == (byte)'c') {pwm2= read_slider();      einstellmenue(); // set PWM2
      } else if (cmd == (byte)'i') {pwm3= read_slider();      einstellmenue(); // set PWM3
      } else if (cmd == (byte)'s') {geschw= read_slider();    einstellmenue(); // set speed
      } else if (cmd == (byte)'a') {wavelength= read_slider();    einstellmenue(); // 
      } else if (cmd == (byte)'d') {red2= read_slider();    einstellmenue(); // set 
      } else if (cmd == (byte)'e') {green2= read_slider();    einstellmenue(); // set 
      } else if (cmd == (byte)'f') {blue2= read_slider();    einstellmenue(); // set 
      
      // Load and Store 
      } else if (cmd == (byte)'o') {save_mode(0);      savemenue();
      } else if (cmd == (byte)'q') {save_mode(1);      savemenue();
      } else if (cmd == (byte)'v') {save_mode(2);      savemenue();
      } else if (cmd == (byte)'y') {save_mode(3);      savemenue();
      } else if (cmd == (byte)'p') {load_mode(0);      loadmenue();
      } else if (cmd == (byte)'t') {load_mode(1);      loadmenue(); 
      } else if (cmd == (byte)'x') {load_mode(2);      loadmenue();
      } else if (cmd == (byte)'z') {load_mode(3);      loadmenue();
      
      } else if (cmd == (byte)'.') {  mainmenue();     // Sende Hauptmenue
       
      } else { Serial.println("Unknown cmd");    Serial.println("{}");     // this should never happen
      }  
      
      cmd = 0; // have processed this cmd now and clear it
  }  // end of if serial available
  time2=millis();    
  if ( time2 > (time + geschw)) {
    time=millis();
    if (phase < 65535) {phase++;} else {phase=0;}; 
    switch (modus) {
      case 1: {static_color();      break;}
      case 2: {colorwheel_all();    break;}
      case 3: {colorwheel_pixels(); break;}
      case 4: {color_breathe();     break;}
      case 5: {fire_mode();         break;}
      case 6: {single_color_wave(); break;}
      case 7: {dual_color_wave();   break;}
      case 8: { loopKnightRider();   break;}
    default: {};  
    }  
  } 
} // END OF LOOP

const uint8_t stellen=15;

void save_mode(uint8_t rein) {
  //if (rein =0) {
  EEPROM.write(200,123);
  //};
  EEPROM.write((1+rein*stellen),modus);
  EEPROM.write((2+rein*stellen),red);
  EEPROM.write((3+rein*stellen),green);
  EEPROM.write((4+rein*stellen),blue);
  EEPROM.write((5+rein*stellen),pwm1);
  EEPROM.write((6+rein*stellen),pwm2);
  EEPROM.write((7+rein*stellen),pwm3);
  EEPROM.write((8+rein*stellen),geschw);
  EEPROM.write((9+rein*stellen),red2);
  EEPROM.write((10+rein*stellen),green2);
  EEPROM.write((11+rein*stellen),blue2);
  EEPROM.write((12+rein*stellen),wavelength);
  EEPROM.write((13+rein*stellen),krSize);
  EEPROM.write((14+rein*stellen),howMany);
  EEPROM.write((15+rein*stellen),krMode);
}
void load_mode(uint8_t raus) {
 modus       =EEPROM.read(1+raus*stellen);
 red         =EEPROM.read(2+raus*stellen); if(red=255) {red=254; };
 green       =EEPROM.read(3+raus*stellen);
 blue        =EEPROM.read(4+raus*stellen);
 pwm1        =EEPROM.read(5+raus*stellen);
 pwm2        =EEPROM.read(6+raus*stellen);
 pwm3        =EEPROM.read(7+raus*stellen);
 geschw      =EEPROM.read(8+raus*stellen);
 red2        =EEPROM.read(9+raus*stellen);
 green2      =EEPROM.read(10+raus*stellen);
 blue2       =EEPROM.read(11+raus*stellen);
 wavelength  =EEPROM.read(12+raus*stellen);
 krSize      =EEPROM.read(13+raus*stellen);
 howMany     =EEPROM.read(14+raus*stellen);
 krMode      =EEPROM.read(15+raus*stellen);
}

// Definition of the individual Modes

void static_color(){
   analogWrite(PWM1PIN,pwm1);
   analogWrite(PWM2PIN,pwm2);
   for( i=0; i<strip.numPixels(); i++) {
   strip.setPixelColor(i, strip.Color(red, green, blue));}
   strip.show();
}        
void colorwheel_all() {
   hell=255;
   analogWrite(PWM1PIN,pwm1);
   analogWrite(PWM2PIN,pwm2);
   for( i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, Wheel((phase) & 255));    
   }
   strip.show();
}        
  
void colorwheel_pixels(){
  hell=255;
  analogWrite(PWM1PIN,pwm1);
  analogWrite(PWM2PIN,pwm2);
  for( i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + phase) & 255));    
  }
  strip.show();
}  
void color_breathe() {
  hell= 128+welle[(phase & 255)];
  analogWrite(PWM1PIN,pwm1*hell/255);
  analogWrite(PWM2PIN,pwm2*hell/255);
  red= hell*254/255;
  green=hell;
  blue=hell;
  for( i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((phase) & 255));    
  }
  strip.show();
}        

void single_color_wave() {
    analogWrite(PWM1PIN,pwm1);
    analogWrite(PWM2PIN,pwm2);
    for( i=0; i<strip.numPixels(); i++) {
      hell= (uint8_t) ((cos((float)2*pi*(i+0.5)/wavelength)*sin((float)phase/255*2*pi)+1)/2*255);
      strip.setPixelColor(i, strip.Color(red*hell/258,  green* hell/255, blue*hell/255));     }
    strip.show();
}

void dual_color_wave() {
    analogWrite(PWM1PIN,pwm1);
    analogWrite(PWM2PIN,pwm2);
    for( i=0; i<strip.numPixels(); i++) {
      hell= (uint8_t) ((cos((float)2*pi*(i+0.5)/wavelength)*sin((float)phase/255*2*pi)+1)/2*255);
      hell2= (uint8_t) ((cos((float)2*pi*(i+0.5+wavelength/2)/wavelength)*sin((float)phase/255*2*pi)+1)/2*255);
      strip.setPixelColor(i, strip.Color(red*hell/258+red2*hell2/258,  green* hell/255+green2* hell2/255, blue*hell/255+blue2*hell2/255));     }
    strip.show();
}

void fire_mode() {
  analogWrite(PWM1PIN,pwm1);
  analogWrite(PWM2PIN,pwm2);
  if (fireloop <11) {fireloop++;} else {fireloop=0;};
  geschw=1;
  switch (fireloop) {
    case 1: {
        red= 200; green=50; blue=0;
        for( i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(red, green, blue));}
        strip.show();
        break;     }
    case 2: {
        geschw= random(10,41);
        break; }
    case 3: {
        red = random( 150, 254);
        for( i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(random( 150, 254), green, blue));}
        strip.show();
        break; } 
    case 4: {
        if (random(0,3)<1) {fireloop =1;};
        break;     }
    case 5: {
        red =254;
        for( i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(red, green, blue));}
        strip.show();
        break; }
    case 6: {
        if (random(0,3)<1) {fireloop =0;};
        break; }
    case 7: {
        green = random( 43, 110);
        for( i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(red, random( 43, 110), blue));}
        strip.show();;
        break;}
    case 8: {
        geschw= random(3,11);
        break;}
    case 9: {
        fireloop=5;
        break; }
    default : {
        fireloop=0;
        };
  };
}


uint8_t read_slider() {
  idxPtr = parser.getFirstArg();
  idxPtr = parser.parseLong(idxPtr,&longResult);
  return (uint8_t)(longResult & 255);
}  

byte cmdByte;
byte parserByteCounter;
byte parserState;
static const byte pfodMaxMsgLen = 0xff; // == 255, if no closing } by now ignore msg 
static const byte pfodStartMsg = (byte)'{';
static const byte pfodEndMsg = (byte)'}';
static const byte pfodWaitingForStart = 0xff;
static const byte pfodInMsg = 0;

void parserSetup() {
  parserByteCounter = 0;
  cmdByte = 0; // not started yet
  parserState = pfodWaitingForStart;
  // switch to pfodInMsg when see {
}

byte parse(byte in) {
  parserByteCounter++;
  if (parserState == pfodWaitingForStart) {
    if (in == pfodStartMsg) { // found {
       parserSetup(); // clean out last cmd
       parserState = pfodInMsg;
    } // else ignore this char as waiting for start {
    // always reset counter if waiting for {
    parserByteCounter = 1;
    return 0;
  }    
  
  // else have seen { 
  if ((parserByteCounter == pfodMaxMsgLen) &&
        (in != pfodEndMsg)) {
    // ignore this msg and reset
    // should not happen as pfodApp should limit
    // msgs sent to pfodDevice to <=255 bytes
    parserSetup();
    return 0;
  }
  
  // else is this the end of the msg
  if (in == pfodEndMsg) {
    byte pfodMsgCmd = cmdByte;
    // this will return 0 when parsing {} msg
    // set up to wait for next {
    parserSetup();
    // return command byte found
    return pfodMsgCmd;
  // else normal byte
  } else if (cmdByte == 0) {
    // save first cmd byte (only the first)
    cmdByte = in;
  } 
  return 0;
 }

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color((WheelPos * 3)*red/255, (255 - WheelPos * 3)*green/255, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color((255 - WheelPos * 3)*red/255, 0, (WheelPos * 3)*blue/255);
  } else {
   WheelPos -= 170;
   return strip.Color(0, (WheelPos * 3)*green/255, (255 - WheelPos * 3)*blue/255);
  }
}
