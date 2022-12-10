#include <SPI.h>
#include <SD.h>
#include <AudioFileSourceSD.h>
#include <AudioFileSourceID3.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <Update.h>
#include <FS.h>
//////////////////////////////////////////
AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#define sclk 14
#define mosi 13
#define cs   15
#define rst  22
#define dc   12
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);

//// Color definitions
#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F
#define DARKGREEN 0x03E0

int color_Arrays_1[] = {DARKGREEN, PINK, GREENYELLOW, ORANGE, WHITE, BLUE, GREEN, RED, MAROON, CYAN, YELLOW, PURPLE, DARKGREY, LIGHTGREY, OLIVE, DARKCYAN, NAVY};
int color_count_1 = 0;
int color_count_2 = 0;

String mp3list_0[200];
String mp3list_1[200];
String mp3list_2[200];
String mp3list_3[200];
String mp3list_4[200];
String mp3list_5[200];
String mp3list_6[200];
String mp3list_7[200];
String mp3list_8[200];
String mp3list_9[200];
String mp3list_A[200];
String mp3list_B[200];
String mp3list_C[200];
String mp3list_D[200];
String mp3list_E[200];
String mp3list_F[200];
String mp3list_G[200];
String mp3list_H[200];
String mp3list_I[200];
String mp3list_J[200];
String mp3list_K[200];
String mp3list_L[200];
String mp3list_M[200];
String mp3list_N[200];
String mp3list_O[200];
String mp3list_P[200];
String mp3list_Q[200];
String mp3list_R[200];
String mp3list_S[200];
String mp3list_T[200];
String mp3list_U[200];
String mp3list_V[200];
String mp3list_W[200];
String mp3list_X[200];
String mp3list_Y[200];
String mp3list_Z[200];


String file_name_list[1200];
int mp3_file_number = 0;
int playing_number = 0;
////////اگر آهنک کاور و یا همون عکس داشته باشه طول میکشه تا بخونش////////////////
//////////////////////////////////////////////////////////////////////////////////

int threshold = 40;
bool touch8detected = false;
bool touch9detected = false;

void gotTouch8() {
  touch8detected = true;
}
///////////////////////////////////////
void gotTouch9() {
  touch9detected = true;
}
//////////////////////////////////////
// perform the actual update from a given stream
void performUpdate(Stream &updateSource, size_t updateSize) {
  if (Update.begin(updateSize)) {
    size_t written = Update.writeStream(updateSource);
    if (written == updateSize) {
      Serial.println("Written : " + String(written) + " successfully");
    }
    else {
      Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
    }
    if (Update.end()) {
      Serial.println("OTA done!");
      if (Update.isFinished()) {
        Serial.println("Update successfully completed. Rebooting.");
      }
      else {
        Serial.println("Update not finished? Something went wrong!");
      }
    }
    else {
      Serial.println("Error Occurred. Error #: " + String(Update.getError()));
    }

  }
  else
  {
    Serial.println("Not enough space to begin OTA");
  }
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// check given FS for valid update.bin and perform update if available
void updateFromFS(fs::FS &fs) {
  File updateBin = fs.open("/update.bin");
  if (updateBin) {
    if (updateBin.isDirectory()) {
      Serial.println("Error, update.bin is not a file");
      updateBin.close();
      return;
    }

    size_t updateSize = updateBin.size();

    if (updateSize > 0) {
      Serial.println("Try to start update");
      performUpdate(updateBin, updateSize);
    }
    else {
      Serial.println("Error, file is empty");
    }

    updateBin.close();

    // whe finished remove the binary from sd card to indicate end of the process
    fs.remove("/update.bin");
  }
  else {
    Serial.println("Could not load update.bin from sd root");
    display.setTextColor(RED, BLACK);
    display.println(F("update.bin Failed"));
  }
}
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void rebootEspWithReason(String reason) {
  Serial.println(reason);
  delay(1000);
  ESP.restart();
}
/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void setup()
{
  Serial.begin(115200);
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T8, gotTouch8, threshold);
  touchAttachInterrupt(T9, gotTouch9, threshold);

  // Init Video
  display.begin(40000000);//80Mhz//try 80Mhz may be less stable)
  display.fillScreen(BLACK);
  ////// Init SD////////////////////////
  while (!SD.begin()) {
    display.setCursor(0, 0);
    Serial.println("Card Mount Failed");
    display.setTextColor(RED, BLACK);
    display.println(F("Card Mount Failed"));
  }
  updateFromFS(SD);
  ////////////////////////////////////////
  display.fillScreen(BLACK);
  uint8_t cardType = SD.cardType();

  while (cardType == CARD_NONE) {
    display.setCursor(0, 0);
    display.setTextColor(RED, BLACK);
    display.println(F("No SD card attached"));
    Serial.println("No SD card attached");
  }
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  Serial.print("SD Card Type: ");
  display.setTextColor(WHITE, BLACK);
  display.println(F("SD Card Type: "));
  delay(100);
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
    display.setTextColor(GREEN, BLACK);
    display.println(F("MMC"));
    delay(100);
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
    display.setTextColor(GREEN, BLACK);
    display.println("SDSC");
    delay(100);
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
    display.setTextColor(GREEN, BLACK);
    display.println(F("SDHC"));
    delay(100);
  } else {
    Serial.println("UNKNOWN");
    display.setTextColor(GREEN, BLACK);
    display.println(F("UNKNOWN"));
    delay(100);
  }
  display.setCursor(0, 0);
  display.setTextColor(WHITE, BLACK);
  display.fillScreen(BLACK);
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  display.print(F("SD Card Size: %lluMB\n")); display.println(cardSize);
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  display.print(F("Total space: %lluMB\n")); display.println(SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  display.print(F("Used space: %lluMB\n")); display.println(SD.usedBytes() / (1024 * 1024));
  delay(100);
  display.fillScreen(BLACK);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  //Read SD

  deleteFile(SD, "/hello.txt");


  mp3_file_number = get_music_list(SD, "/MP3", 0, file_name_list);

  Serial.print("Music file count:");   Serial.println(mp3_file_number);
  display.print("Music file count:");   display.println(mp3_file_number);

  delay(100);
  display.fillScreen(BLACK);

  Serial.println("__readFile__");
  readFile(SD, "/hello.txt");

}
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void loop()
{
  display.setCursor(0, 0);
  display.setTextSize(0);
  open_new_song(file_name_list[playing_number]);
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  out = new AudioOutputI2S(0, 1, 128); // Output to builtInDAC
  out->SetGain(0.9);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
  //////////////////////////////////////////////
  while (mp3->isRunning())
  {
    (AudioGeneratorMP3 *)mp3;
    if (mp3->isRunning())
    {

      if (!mp3->loop())
      {
        mp3->stop();
        Serial.println(F("mp3: loop!  stop"));
        if (playing_number < mp3_file_number - 1 || playing_number <= mp3_file_number) {
          Serial.println("next_Song");
          display.fillScreen(BLACK);
          playing_number++;
        }
      } else if (touch8detected) {
        delay(100);
        touch8detected = false;
        Serial.println("next_Song_Touch 8 pin_33");
        mp3->stop();
        if ( playing_number < mp3_file_number - 1) {
          playing_number++;
          display.setCursor(0, 0);
          display.setTextColor(DARKGREEN, BLACK);
          display.setTextSize(2);
          display.print(mp3_file_number);
          display.setTextColor(RED, BLACK);
          display.print("_");
          display.setTextColor(WHITE, BLACK);
          display.print(playing_number + 1);
        }
      } else if (touch9detected) {
        delay(100);
        touch9detected = false;
        Serial.println("previous_Song_Touch 9 pin_32");
        mp3->stop();
        if (playing_number > 0) {
          playing_number--;
          display.setCursor(0, 0);
          display.setTextColor(DARKGREEN, BLACK);
          display.setTextSize(2);
          display.print(mp3_file_number);
          display.setTextColor(RED, BLACK);
          display.print("_");
          display.setTextColor(WHITE, BLACK);
          display.print(playing_number + 1);
        }
      }
      else {
        delay(100);
        display.fillScreen(BLACK);
      }
    }//// if (mp3->isRunning())
  }//// while (mp3->isRunning())
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  touch8detected = false;
  touch9detected = false;
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);
  //  display.printf("ID3 callback for: %s = '", type);
  display.printf("%s ='", type);
  color_count_1++;
  if (color_count_1 >= 16) {
    color_count_1 = 0;
  }
  if (isUnicode) {
    string += 2;
  }

  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    color_count_2++;
    if (color_count_2 >= 16) {
      color_count_2 = 0;
    }
    Serial.printf("%c", a);
    display.setTextColor(color_Arrays_1[color_count_2 + 1]);
    display.printf("%c", a);
  }
  display.printf("'\n");
  display.setTextColor(color_Arrays_1[color_count_1]);
  Serial.printf("'\n");
  Serial.flush();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int get_music_list(fs::FS &fs, const char *dirname, uint8_t levels, String  mp3list[1200])
{
  Serial.printf("Listing directory: %s\n", dirname);
  int counter_1 = 0;
  int counter_2 = 0;
  int A_ = 0;
  int B_ = 0;
  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
  }

  File file = root.openNextFile();
  Serial.println("All music:");
  while (file)
  { String mp3_name;
    int thisChar;

    mp3_name = file.name();
    //    while (mp3_name.startsWith("/MP3/A") && mp3_name.endsWith(".mp3")) {
    //      mp3list_A[A_] = mp3_name;
    //      mp3list[A_] = mp3list_A[A_];
    //      Serial.println(mp3list[A_]);
    //      //const char* ccx = temp.c_str();
    //      //appendFile(SD, "/hello.txt", ccx);
    //      //appendFile(SD, "/hello.txt", "\n");
    //      //writeFile(SD, "/hello.txt",ccx);
    //      counter_1++;
    //      A_++;
    //      file = root.openNextFile();
    //      mp3_name = file.name();
    //    }
    //    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //    while (mp3_name.startsWith("/MP3/B") && mp3_name.endsWith(".mp3")) {
    //      mp3list_B[B_] = mp3_name;
    //      mp3list[B_] = mp3list_B[B_];
    //      Serial.println(mp3list[B_]);
    //      //const char* ccx = temp.c_str();
    //      //appendFile(SD, "/hello.txt", ccx);
    //      //appendFile(SD, "/hello.txt", "\n");
    //      //writeFile(SD, "/hello.txt",ccx);
    //      counter_1++;
    //      B_++;
    //      file = root.openNextFile();
    //      mp3_name = file.name();
    //    }
    //    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //    file = root.openNextFile();
    //  }
    //  return A_ + B_;
    //}
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //if (temp.startsWith("/MP3/" + thisChar) && temp.endsWith(".mp3")) {mp3list_0
    if (mp3_name.startsWith("/MP3/0")) {
      mp3list_0[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/1")) {
      mp3list_1[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/2")) {
      mp3list_2[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/3")) {
      mp3list_3[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/4")) {
      mp3list_4[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/5")) {
      mp3list_5[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/6")) {
      mp3list_6[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/7")) {
      mp3list_7[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/8")) {
      mp3list_8[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/9")) {
      mp3list_9[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/A") && mp3_name.endsWith(".mp3")) {
      mp3list_A[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/B") && mp3_name.endsWith(".mp3")) {
      mp3list_B[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/C") && mp3_name.endsWith(".mp3")) {
      mp3list_C[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/D") && mp3_name.endsWith(".mp3")) {
      mp3list_D[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/E") && mp3_name.endsWith(".mp3")) {
      mp3list_E[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/F") && mp3_name.endsWith(".mp3")) {
      mp3list_F[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/G") && mp3_name.endsWith(".mp3")) {
      mp3list_G[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/H") && mp3_name.endsWith(".mp3")) {
      mp3list_H[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/H") && mp3_name.endsWith(".mp3")) {
      mp3list_H[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/I") && mp3_name.endsWith(".mp3")) {
      mp3list_I[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/J") && mp3_name.endsWith(".mp3")) {
      mp3list_J[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/K") && mp3_name.endsWith(".mp3")) {
      mp3list_K[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/L") && mp3_name.endsWith(".mp3")) {
      mp3list_L[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/M") && mp3_name.endsWith(".mp3")) {
      mp3list_M[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/N") && mp3_name.endsWith(".mp3")) {
      mp3list_N[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/O") && mp3_name.endsWith(".mp3")) {
      mp3list_O[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/P") && mp3_name.endsWith(".mp3")) {
      mp3list_P[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/Q") && mp3_name.endsWith(".mp3")) {
      mp3list_Q[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/R") && mp3_name.endsWith(".mp3")) {
      mp3list_R[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/S") && mp3_name.endsWith(".mp3")) {
      mp3list_S[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/T") && mp3_name.endsWith(".mp3")) {
      mp3list_T[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/U") && mp3_name.endsWith(".mp3")) {
      mp3list_U[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/V") && mp3_name.endsWith(".mp3")) {
      mp3list_V[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/W") && mp3_name.endsWith(".mp3")) {
      mp3list_W[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/X") && mp3_name.endsWith(".mp3")) {
      mp3list_X[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/Y") && mp3_name.endsWith(".mp3")) {
      mp3list_Y[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    else if (mp3_name.startsWith("/MP3/Z") && mp3_name.endsWith(".mp3")) {
      mp3list_Z[counter_1] = mp3_name;
      //const char* ccx = temp.c_str();
      //appendFile(SD, "/hello.txt", ccx);
      //appendFile(SD, "/hello.txt", "\n");
      //writeFile(SD, "/hello.txt",ccx);
      counter_1++;
    }
    file = root.openNextFile();
  }

  for (int i = 0; i < 200; i++) {
    if (mp3list_0[i].startsWith("/MP3/0")) {
      mp3list[counter_2] = mp3list_0[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_1[i].startsWith("/MP3/1")) {
      mp3list[counter_2] = mp3list_1[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_2[i].startsWith("/MP3/2")) {
      mp3list[counter_2] = mp3list_2[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_3[i].startsWith("/MP3/3")) {
      mp3list[counter_2] = mp3list_3[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_4[i].startsWith("/MP3/4")) {
      mp3list[counter_2] = mp3list_4[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_5[i].startsWith("/MP3/5")) {
      mp3list[counter_2] = mp3list_5[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_6[i].startsWith("/MP3/6")) {
      mp3list[counter_2] = mp3list_6[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_7[i].startsWith("/MP3/7")) {
      mp3list[counter_2] = mp3list_7[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_8[i].startsWith("/MP3/8")) {
      mp3list[counter_2] = mp3list_8[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_9[i].startsWith("/MP3/9")) {
      mp3list[counter_2] = mp3list_9[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_A[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_A[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_B[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_B[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_C[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_C[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_D[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_D[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_E[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_E[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_F[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_F[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_G[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_G[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_H[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_H[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_I[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_I[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_J[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_J[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_K[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_K[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_L[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_L[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_M[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_M[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_N[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_N[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_O[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_O[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_P[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_P[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_Q[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_Q[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_R[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_R[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_S[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_S[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_T[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_T[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_U[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_U[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_V[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_V[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_W[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_W[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_X[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_X[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_Y[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_Y[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < 200; i++) {
    if (mp3list_Z[i].endsWith(".mp3")) {
      mp3list[counter_2] = mp3list_Z[i];
      counter_2++;           Serial.println(counter_2);
    }
  }
  for (int i = 0; i < counter_2; i++) {
    Serial.println(mp3list[i]);
  }
  return counter_2;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void appendFile(fs::FS & fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void readFile(fs::FS & fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);
  int d;
  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("Read from file: ");
  while (file.available()) {
    char  b = file.read();
    String myString = String(b);
    Serial.print(myString);
  }
  file.close();
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void open_new_song(String filename)
{
  String Music_info = filename.substring(1, filename.indexOf("."));
  String song_name = "/" + Music_info + ".mp3";
  file = new AudioFileSourceSD(song_name.c_str());
  Music_info = filename.substring(5, filename.indexOf("."));
  Serial.println("playing : " + Music_info);
}
