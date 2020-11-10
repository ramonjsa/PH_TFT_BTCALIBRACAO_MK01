#include <FirmataConstants.h>
#include <Firmata.h>
#include <FirmataMarshaller.h>
#include <FirmataDefines.h>
#include <Boards.h>
#include <FirmataParser.h>

#include <AnalogPHMeter.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/FreeSans12pt7b.h>
//#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF000
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410
#define BLUE    0x02F0
SoftwareSerial mySerial(12, 13); // RX, TX



int botaomenor = 10;
char buffer[33];
int contador = 0;

//int botaomaior = 3;
const int buttonPin = 11;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

int flag;


String texto;


//volatile int contador = 0;

AnalogPHMeter pHSensor(A7);
unsigned int pHCalibrationValueAddress = 0;

float falaph = pHSensor.singleReading().getpH();
String PH = " ph ";
String fala;
float valor_atual;
float novo_valor;
unsigned long t;

void setup(void) {

  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("LPEQI - PH");
  mySerial.print("LPEQI - PH");


  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
  tft.begin(ID);
  tft.setRotation(2);


  pinMode (10, INPUT);
  pinMode (11, INPUT);

  delay(1000);

  struct PHCalibrationValue pHCalibrationValue;
  EEPROM.get(pHCalibrationValueAddress, pHCalibrationValue);
  // pHSensor.initialize(pHCalibrationValue);
  pHSensor.initialize();
  valor_atual = pHSensor.singleReading().getpH();
  valor_atual = novo_valor;
  texto = String (novo_valor);
  texto.toCharArray(buffer2, 49);

  t = millis();
  /*
    Serial.println("Analog PH Meter Calibration");
    Serial.println("The value will be saved on EEPROM");
    Serial.println("Procedure:");
    Serial.println(" - Put pH on pH 7 solution");
    Serial.println(" - Wait until stable");
    Serial.println(" - Press 'm' to calibrate");
    Serial.println(" - Same process for pH 4 and/or pH 10,");
    Serial.println("   except press 'l' for pH 4, and 'h' for pH 10");
    Serial.println();
    Serial.println("All calibration value will be reset after restart.");
    Serial.println("In this example, press 's' to save the value to EEPROM");
    Serial.println("You can freely change the storage.");
    Serial.println("It is not limited to EEPROM only");
    Serial.println();
    Serial.println("*to clear calibration value press 'c'");*/
}



void ponto_para_virgula(String texto) {
  texto.replace(".", ",");

}

void loop(void) {
  char buffer2[49];

  novo_valor =  pHSensor.singleReading().getpH();
  if (abs(valor_atual - novo_valor) > 0.2 ) {
    valor_atual = novo_valor;
    texto = String (novo_valor);
    texto.toCharArray(buffer2, 49);
  }


  if (millis() - t > 1000) {


    // lcd.print(valor_atual);
    //Serial.println(pHSensor.singleReading().getpH());
    tft.fillScreen(BLACK);
    showmsgXY(20, 70, 3, &FreeSans9pt7b, "LEPEQI");
    showmsgXY(20, 180, 4, &FreeSans9pt7b, "PH");

    showmsgXY(10, 310, 5, &FreeSans9pt7b, buffer2);
    //                      tft.setTextColor(RED, GREY);
    //                      tft.setTextSize(1);
    //                      tft.setCursor(0, 470);
    //                      tft.print("linha final");
    delay(1000);
    Serial.println ("variavel" +String(novo_valor));
    Serial.println ("buffer2" +String(buffer2));
    Serial.println ("texto" +texto);
    t = millis();

  }

  //-------------------------------------------------------------------

  botaomenor = digitalRead(10);
  if (canInterrupt())
  {
    if (botaomenor == HIGH)
    {
      sentebotao("1");
    }
  }


  if (flag == 1) {
    switch (contador) {
      case 0:
        Serial.println("case0");
        //lcd.noDisplay();
        //lcd.begin(16, 2);
        //lcd.print("LPEQI - PH");
        //lcd.setCursor(0, 0);
        //lcd.print("CALIBRACAO");
        tft.setTextColor(WHITE, GREY);
        tft.setTextSize(2);
        tft.setCursor(0, 380);
        tft.print("Calibracao");
        mySerial.print("Calibração");
        delay(2000);
        break;

      case 1:
        Serial.println("case1");
        //lcd.noDisplay();
        //lcd.begin(16, 2);
        //lcd.print("LPEQI - PH");
        //lcd.setCursor(0, 0);
        //lcd.print("solucao PH  4.00");// 56 caracteres
        tft.setTextColor(WHITE, GREY);
        tft.setTextSize(2);
        tft.setCursor(0, 390);
        tft.print("Solucao PH");
        tft.setCursor(0, 450);
        tft.print(" 4,00");
        pHSensor.calibrationClear();
        mySerial.print("Coloque o eletrodo na solução PH 4.00 e aguarde 1 minuto");
        delay(2000);
        break;
      case 2:
        Serial.println("case2");
        //lcd.begin(16, 2);
        //lcd.setCursor(6, 0);
        //lcd.print("LPEQI - PH");
        //lcd.setCursor(0, 0);
        //lcd.print("solucao PH  7.00");//56 caracteres
        tft.setTextColor(WHITE, GREY);
        tft.setTextSize(2);
        tft.setCursor(0, 390);
        tft.print("Solucao PH");
        tft.setCursor(0, 450);
        tft.print(" 7,00");
        pHSensor.calibrationMid(4.000f);
        mySerial.print("Coloque o eletrodo na solução PH 7.00 e aguarde 1 minuto");
        delay(2000);
        break;
      case 3:
        Serial.println("case3");
        //lcd.begin(16, 2);
        //lcd.setCursor(6, 0);
        //lcd.print("LPEQI - PH");
        //lcd.setCursor(0, 0);
        //lcd.print("solucao PH 10.00");//57 caracteres
        tft.setTextColor(WHITE, GREY);
        tft.setTextSize(2);
        tft.setCursor(0, 390);
        tft.print("Solucao PH");
        tft.setCursor(0, 450);
        tft.print("10,00");
        pHSensor.calibrationLow(7.000f);
        mySerial.print("Coloque o eletrodo na solução PH 10.00 e aguarde");
        delay(500);
        break;
      case 4:
        Serial.println("case4");
        //lcd.begin(16, 2);
        //lcd.setCursor(6, 0);
        //lcd.print("LPEQI - PH");
        //lcd.setCursor(0, 0);
        //lcd.print("   pressione fim");//51 caracteres
        tft.setTextColor(WHITE, GREY);
        tft.setTextSize(2);
        tft.setCursor(0, 390);
        tft.print("pressione para");
        tft.setCursor(0, 450);
        tft.print("finalizar");
        mySerial.print ("Pressione para finalizar");
        pHSensor.calibrationHigh(10.000f);
        delay(500);
        break;
      case 5:
        Serial.print("case5");
        //lcd.begin(16, 2);
        //lcd.setCursor(6, 0);
        //lcd.print("LPEQI - PH");
        //lcd.setCursor(0, 2);
        //lcd.print("PH");
        EEPROM.put(pHCalibrationValueAddress, pHSensor.getCalibrationValue());
        Serial.println("case5");
        delay(500);
        break;
    }
    flag = 0;
  }


  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    //mySerial.println(pHSensor.singleReading().getpH());
    //falaph = pHSensor.singleReading().getpH();
    fala = String(PH + valor_atual);
    fala.replace(".", ",");
    Serial.println(fala);
    mySerial.println(fala);
    delay(500);
  }



  /*
    if (millis() - t > 1000) {
     lcd.begin(16, 2);
     lcd.setCursor(6, 0);
     lcd.print("LPEQI - PH");
     lcd.setCursor(0, 2);
     lcd.print(pHSensor.singleReading().getpH());
     //Serial.println(pHSensor.singleReading().getpH());
     t = millis();
    }


  */

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'm') {
      pHSensor.calibrationMid(7.000f);
    } else if (c == 'l') {
      pHSensor.calibrationLow(4.000f);
    } else if (c == 'h') {
      pHSensor.calibrationHigh(10.000f);
    } else if (c == 'c') {
      pHSensor.calibrationClear();
    } else if (c == 's') {
      //EEPROM.put(pHCalibrationValueAddress, pHSensor.getCalibrationValue());
    }
  }
}

void sentebotao(char *botao) {
  contador++;
  flag = 1;

  sprintf(buffer, "Botao %s: %d", botao, contador); // Monta a string

  if (contador == 6) {
    contador = 0;
  }
}
bool canInterrupt()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 9000) // Verifica se a ultima interrupção foi à 9 segundos
  {
    last_interrupt_time = interrupt_time;
    return true;
  }
  return false;
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.drawFastHLine(0, y, tft.width(), BLACK);
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(WHITE);
  tft.setTextSize(sz);
  tft.print(msg);
  delay(1000);
}




