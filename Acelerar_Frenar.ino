#include <SPI.h>    // incluye libreria bus SPI
#include <Adafruit_GFX.h> // incluye libreria para manejo de graficos
#include <Adafruit_ILI9341.h> // incluye libreria para controlador ILI9341
#include <MCUFRIEND_kbv.h>
#include <SD.h>
#define TFT_DC 9    // define constante TFT_DC con numero 9
#define TFT_CS 10   // define constante TFT_CS con numero 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);  // crea objeto
const int rojo = 3;
const int verde = 4;
const int rosa = 5;
//const int partir =6;
//const int acelerar =7;
//const int frenar =8;
unsigned long duracion;
bool BANDERA_LED_VERDE = false;
int modo = 0;
#define BTN_PARTIR  0
#define BTN_ACELERAR  1
#define BTN_FRENAR    2
uint8_t button[3] = {
  6,
  7,
  8
};
uint8_t button_estate[5];
#define S_APAGADO 0
#define S_PARTIR 1
#define S_INSTRUCCIONES 2
#define S_PROGRAMA 3
#define S_INSTRUCCIONES_2 4
uint8_t estado = S_APAGADO;
void setup() {
  Serial.begin(115200);
  tft.begin();          // inicializa pantalla
  tft.setRotation(3);       // establece posicion vertical con pines hacia abajo
  tft.fillScreen(ILI9341_BLACK);    // fondo de pantalla de color negro
  tft.fillRect(0, 0, 320, 30, ILI9341_NAVY);  // rectangulo azul naval a modo de fondo de titulo
  tft.setTextSize(2);
  tft.setCursor(18, 8);
  tft.setTextColor(ILI9341_RED);
  tft.print("Practica Acelerar-Frenar");
  delay(500);
  // initialize the LED pin as an output:
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(rosa, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button[BTN_PARTIR], INPUT_PULLUP);
  pinMode(button[BTN_ACELERAR], INPUT_PULLUP);
  pinMode(button[BTN_FRENAR], INPUT_PULLUP);
  button_estate[0] = HIGH;
  button_estate[1] = HIGH;
  button_estate[2] = HIGH;
  button_estate[3] = HIGH;
}
uint8_t flancoSubida(int btn) {
  uint8_t valor_nuevo = digitalRead(button[btn]);
  uint8_t result = button_estate[btn] != valor_nuevo && valor_nuevo == 1;
  button_estate[btn] = valor_nuevo;
  return result;
}
void loop() {
  switch (estado) {
    case S_APAGADO:
      if (flancoSubida(BTN_PARTIR))  { // Transición BTN_MENU
        estado = S_PARTIR;
        INSTRUCCIONES_NEGRO();
        BANDERA_LED_VERDE = false;
        break;
      }
      BANDERA_LED_VERDE = false;
      //   tft.fillCircle(80, 150, 40, ILI9341_BLACK); //ROJO
      //   tft.fillCircle(245, 150, 40, ILI9341_BLACK);  //VERDE
      digitalWrite(verde, LOW);
      digitalWrite(rosa, LOW);
      digitalWrite(rojo, LOW);
      SISTEMA_COLOR();
      yield();
      break;
    case S_PARTIR:
      if (flancoSubida(BTN_PARTIR))  { // Transición BTN_MENU
        estado = S_APAGADO;
        BANDERA_LED_VERDE = false;
        INSTRUCCIONES_NEGRO();
        break;
      }
      SISTEMA_NEGRO();
      INSTRUCCIONES_COLOR();
      // tft.fillCircle(80, 150, 40, ILI9341_BLACK); //ROJO
      digitalWrite(rosa, HIGH);
      digitalWrite(rojo, HIGH);
      BANDERA_LED_VERDE = true;
      do
      {
        digitalWrite(verde, HIGH);
        delay(200);
        digitalWrite(verde, LOW);
        delay(200);
        if (flancoSubida(BTN_ACELERAR))  { // Transición BTN_MENU
          estado = S_INSTRUCCIONES;
          BANDERA_LED_VERDE = false;
          INSTRUCCIONES_NEGRO();
          digitalWrite(rosa, LOW);
          digitalWrite(rojo, LOW);
          digitalWrite(verde, LOW);
          break;
        }
        if (flancoSubida(BTN_PARTIR))  { // Transición BTN_MENU
          estado = S_APAGADO;
          BANDERA_LED_VERDE = false;
          INSTRUCCIONES_NEGRO();
          digitalWrite(rosa, LOW);
          digitalWrite(rojo, LOW);
          digitalWrite(verde, LOW);
          break;
        }
      }
      while (BANDERA_LED_VERDE = true);
      break;
    case S_INSTRUCCIONES:
      INSTRUCCIONES_COLOR();
      BANDERA_LED_VERDE = true;
      do
      {
        digitalWrite(verde, HIGH);
        delay(200);
        digitalWrite(verde, LOW);
        delay(200);
        if (flancoSubida(BTN_PARTIR))  { // Transición BTN_MENU
          estado = S_APAGADO;
          BANDERA_LED_VERDE = false;
          INSTRUCCIONES_NEGRO();
          INSTRUCCIONES_NEGRO_2();
          break;
        }
        if (flancoSubida(BTN_ACELERAR))  { // Transición BTN_MENU
          BANDERA_LED_VERDE = false;
          INSTRUCCIONES_NEGRO();
          INSTRUCCIONES_NEGRO_2();
          estado = S_INSTRUCCIONES_2;
          break;
        }
      }
      while (BANDERA_LED_VERDE = true);
      break;
    case S_INSTRUCCIONES_2:
      BANDERA_LED_VERDE = true;
      INSTRUCCIONES_COLOR_2();
      do
      {
        if (flancoSubida(BTN_PARTIR))  { // Transición BTN_MENU
          estado = S_APAGADO;
          BANDERA_LED_VERDE = false;
          INSTRUCCIONES_NEGRO();
          INSTRUCCIONES_NEGRO_2();
          break;
        }
        if (flancoSubida(BTN_ACELERAR))  { // Transición BTN_MENU
          estado = S_PROGRAMA;
          BANDERA_LED_VERDE = false;
          INSTRUCCIONES_NEGRO();
          INSTRUCCIONES_NEGRO_2();
          break;
        }
        digitalWrite(verde, HIGH);
        delay(200);
        digitalWrite(verde, LOW);
        delay(200);
      }
      while (BANDERA_LED_VERDE = true);
      break;
    case S_PROGRAMA:
      if (flancoSubida(BTN_PARTIR))  { // Transición BTN_MENU
        estado = S_APAGADO;
        INSTRUCCIONES_NEGRO();
        INSTRUCCIONES_NEGRO_2();
        tft.fillCircle(80, 150, 40, ILI9341_BLACK); //ROJO
        tft.fillCircle(245, 150, 40, ILI9341_BLACK);  //VERDE
        digitalWrite(verde, LOW);
        break;
      }
      break;
  }
}

void INSTRUCCIONES_NEGRO()
{
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setCursor(10, 40);
  tft.print("PRIMERO LEA ATENTAMENTE");
  tft.setCursor(10, 65);
  tft.print("TODAS LAS INSTRUCCIONES:");
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(10, 90);
  tft.print("Este programa mide la");
  tft.setCursor(10, 105);
  tft.print("la velocidad de reaccion");
  tft.setCursor(10, 120);
  tft.print("de frenado.Para continuar");
  tft.setCursor(10, 135);
  tft.print("presione el boton verde.");
}
void INSTRUCCIONES_COLOR()
{
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setCursor(10, 40);
  tft.print("PRIMERO LEA ATENTAMENTE");
  tft.setCursor(10, 65);
  tft.print("TODAS LAS INSTRUCCIONES:");
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(10, 90);
  tft.print("Este programa mide la");
  tft.setCursor(10, 105);
  tft.print("la velocidad de reaccion");
  tft.setCursor(10, 120);
  tft.print("de frenado.Para continuar");
  tft.setCursor(10, 135);
  tft.print("presione el boton verde.");
}
void INSTRUCCIONES_NEGRO_2()
{
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(10, 40);
  tft.print("Debera acelerar cuando el");
  tft.setCursor(10, 55);
  tft.print("programa lo indique.");
  tft.setCursor(10, 70);
  tft.print("Una vez iniciado son 10");
  tft.setCursor(10, 85);
  tft.print("veces las que tiene que ");
  tft.setCursor(10, 100);
  tft.print("frenar y acelerar. Debe");
  tft.setCursor(10, 115);
  tft.print("hacerlo consecutivamente.");
  tft.setCursor(10, 130);
  tft.print("Una vez finalizado, el ");
  tft.setCursor(10, 145);
  tft.print("programa mostrara todas");
  tft.setCursor(10, 160);
  tft.print("las entradas y el promedio");
  tft.setCursor(10, 175);
  tft.print("hacerlo consecutivamente.");
  tft.setCursor(10, 190);
  tft.print("presione el boton verde.");

}
void INSTRUCCIONES_COLOR_2()
{
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(10, 40);
  tft.print("Debera acelerar cuando el");
  tft.setCursor(10, 55);
  tft.print("programa lo indique.");
  tft.setCursor(10, 70);
  tft.print("Una vez iniciado son 10");
  tft.setCursor(10, 85);
  tft.print("veces las que tiene que ");
  tft.setCursor(10, 100);
  tft.print("frenar y acelerar. Debe");
  tft.setCursor(10, 115);
  tft.print("hacerlo consecutivamente.");
  tft.setCursor(10, 130);
  tft.print("Una vez finalizado, el ");
  tft.setCursor(10, 145);
  tft.print("programa mostrara todas");
  tft.setCursor(10, 160);
  tft.print("los datos y el promedio");
  tft.setCursor(10, 175);
  tft.print("hacerlo consecutivamente.");
  tft.setCursor(10, 190);
  tft.print("presione el boton verde.");
}
void SISTEMA_NEGRO()
{
  tft.setTextSize(3);
  tft.setCursor(25, 80);
  tft.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
  tft.print("SISTEMA APAGADO");
  tft.setTextSize(2);
  tft.setCursor(30, 150);
  tft.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
  tft.print("Presione Boton Partir");
}
void SISTEMA_COLOR()
{
  tft.setTextSize(3);
  tft.setCursor(25, 80);
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.print("SISTEMA APAGADO");
  tft.setTextSize(2);
  tft.setCursor(30, 150);
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.print("Presione Boton Partir");
}
//sprintf( _buffer, "%15u", (int)abs(iPPM_LPG));
//  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);  // texto en color amarillo
//  tft.setTextSize(2);   // escala de texto en 6
//  tft.setCursor(75, 70); // ubica cursor
// tft.print(_buffer);   // valor que representa la temperatura del sensor en zona 1
