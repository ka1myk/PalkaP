/**************************************************************
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 * NOTE:
 * Some of the functions may be unavailable for your modem.
 * Just comment them out.
 *
 **************************************************************/

#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


// Select your modem:
// #define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590

// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, T#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFFX


//#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG SerialMon

// Set phone numbers, if you want to test SMS and Calls
// #define SMS_TARGET  ""
#define CALL_TARGET ""

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "internet.mts.ru";
const char user[] = "mts";
const char pass[] = "mts";

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

void setup() {
tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  
  tft.begin(0x8357); // SDFP5408

  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial


//  Serial.println(testText());
  delay(3000);


  
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // Set your reset, enable, power pins here

  delay(3000);

  // Set GSM module baud rate
  TinyGsmAutoBaud(SerialAT);
}

void loop() {

    tft.fillScreen(BLACK);
  tft.setCursor(0, 0);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  DBG("Initializing modem...");
  if (!modem.restart()) {
    delay(10000);
    return;
  }

  String modemInfo = modem.getModemInfo();
  DBG("Modem:", modemInfo);

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  DBG("Waiting for network...");
  if (!modem.waitForNetwork()) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) {
    DBG("Network connected");
     tft.println("Network connected");
  }

  DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    delay(10000);
    return;
  }

  bool res;

  String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);
  tft.println(ccid);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);
  tft.println(imei);

  String cop = modem.getOperator();
  DBG("Operator:", cop);
  tft.println("Operator:");
  tft.println(cop);
  
  IPAddress local = modem.localIP();
  DBG("Local IP:", local);
  tft.println("Local IP:");
  tft.println(local);

  int csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);
  tft.println("Signal quality:");
  tft.println(csq);

  // This is NOT supported on M590
  int battLevel = modem.getBattPercent();
  DBG("Battery lavel:", battLevel);
  tft.println("Battery lavel:");
  tft.println(battLevel);

  // This is only supported on SIMxxx series
  float battVoltage = modem.getBattVoltage() / 1000.0F;
  DBG("Battery voltage:", battVoltage);

  // This is only supported on SIMxxx series
  String gsmLoc = modem.getGsmLocation();
  DBG("GSM location:", gsmLoc);
  tft.println("GSM location:");
  tft.println(gsmLoc);

  String ussd_balance = modem.sendUSSD("*100#");
  DBG("Balance (USSD):", ussd_balance);

  String ussd_phone_num = modem.sendUSSD("*107#");
  DBG("Phone number (USSD):", ussd_phone_num);

#if defined(TINY_GSM_MODEM_SIM808)
  modem.enableGPS();
  String gps_raw = modem.getGPSraw();
  modem.disableGPS();
  DBG("GPS raw data:", gps_raw);
  tft.println("GPS raw data:");
  tft.println(gps_raw);
#endif

#if defined(SMS_TARGET)
  res = modem.sendSMS(SMS_TARGET, String("Hello from ") + imei);
  DBG("SMS:", res ? "OK" : "fail");

  // This is only supported on SIMxxx series
  res = modem.sendSMS_UTF16(SMS_TARGET, u"Привіііт!", 9);
  DBG("UTF16 SMS:", res ? "OK" : "fail");
#endif

#if defined(CALL_TARGET)
  DBG("Calling:", CALL_TARGET);

  // This is NOT supported on M590
  res = modem.callNumber(CALL_TARGET);
  DBG("Call:", res ? "OK" : "fail");

  if (res) {
    delay(1000L);

    // Play DTMF A, duration 1000ms
    modem.dtmfSend('A', 1000);

    // Play DTMF 0..4, default duration (100ms)
    for (char tone='0'; tone<='4'; tone++) {
      modem.dtmfSend(tone);
    }

    delay(5000);

    res = modem.callHangup();
    DBG("Hang up:", res ? "OK" : "fail");
  }
#endif

 
/*
  modem.gprsDisconnect();
  if (!modem.isGprsConnected()) {
    DBG("GPRS disconnected");
  } else {
    DBG("GPRS disconnect: Failed.");
  }

  // Try to power-off (modem may decide to restart automatically)
  // To turn off modem completely, please use Reset/Enable pins
  modem.poweroff();
  DBG("Poweroff.");

  // Do nothing forevermore
  while (true) {
    modem.maintain();
  }
  */
}

