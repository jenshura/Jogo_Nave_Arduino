#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal.h> //Biblioteca display sem ser I2C

#define btcima 8
#define btbaixo 9
#define bttiro 10

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do display 16x2
//LiquidCrystal lcd(7,6,5,4,3,2); //Declaração do display sem ser I2C

int vel=100;
int pxnave, pynave, pxaste,pyaste, pxenergia, pyenergia, pxtiro,pytiro;
bool game, vtiro, vpilha;
int pontos;
double venergia;
int vtela;
int maxpontos = 10;

byte nave[8] = {B11000, B01100, B01110, B01111, B01111, B01110, B01100, B11000};
byte asteroid[8] = {B00000,	B00100,	B01110,	B10111,	B11101,	B01110,	B00100,	B00000};
byte explosao[8] = {B10001,	B10101,	B01010,	B00100,	B00100,	B01010,	B10101,	B10001};
byte energia[8] = {B01110,	B11011,	B10001,	B10101,	B10101,	B10101,	B10001,	B11111};
byte tiro[8] = {B00000, B00000, B00000,	B00111,	B00111,	B00000,	B00000,	B00000};

void setup() 
{    // Inicialização das variáveis
  pxnave = pynave = pxaste = pyaste = pxenergia = pyenergia = pxtiro = pytiro = 0;
  pxtiro=-1;
  pxaste=12;
  pxenergia=100;
  vtela=0;
pinMode(bttiro, INPUT);
// Inicialização do display LCD quando é I2C
  lcd.init();
  lcd.backlight();

  // Criação dos caracteres customizados para o display
  lcd.createChar(1, nave);
  lcd.createChar(2, asteroid);
  lcd.createChar(3, explosao);
  lcd.createChar(4, energia);
  lcd.createChar(5, tiro);
  //lcd.begin(16, 2); //Função com display sem ser I2C
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   ASTEROID   ");
  delay(2000);
  lcd.clear();

  game=true;
  vtiro=vpilha=false;
}

void loop() 
{
   if (game) 
  {// Lógica do jogo enquanto estiver em execução
    venergia-=0.25;
    lcd.clear();

    if (venergia<=0) {
      game=0;
      desenhaExplosaoNave(pxnave, pynave);
      vtela=2;
    }
    painel(13);
    if (digitalRead(btcima)==1) {
      pynave=0;
    }
    if (digitalRead(btbaixo)==1) {
      pynave=1;
    }
    if (digitalRead(bttiro)==1) {
    pxtiro=1;
    vtiro=true;
    pytiro=pynave;
    }
    pxaste-=1;
    desenhaNave(pxnave, pynave);
    desenhaAsteroid(pxaste, pyaste);
    if (vtiro)    {
       desenhaTiro(pxtiro, pytiro); 
       pxtiro+=1;
    }

    if (pxaste<0) {
      pxaste=12;
      pyaste=random(0,2);
    }

    if (pxtiro>16) {
      vtiro=false;
      pxtiro=-1;
    }

    if (((pxtiro==pxaste+1)&&(pytiro==pyaste)) || ((pxtiro==pxaste) && (pytiro==pyaste))) 
    {
      vtiro=false;
      pxtiro=-1;
      desenhaExplosaoAsteroid(pxaste, pyaste);
      pyaste=random(0,2);
      pxaste=12;
      pontos+=2;
      if (pontos>=maxpontos) {
      game=0;
      vtela=1;
      }
    }

    if (((pxnave==pxaste)&&(pynave==pyaste))) {
      game=0;
      desenhaExplosaoNave(pxnave, pyaste);
      vtela=2;
    }

    if (!vpilha) 
    {
      if (random(0,60)>58) 
        {
          pxenergia=12;
          vpilha=true;
          pyenergia=random(0,2);
        }else 
          {
            pxenergia-=1;
            desenhaenErgia(pxenergia, pyenergia);
            if (((pxnave==pxenergia+1)&&(pynave==pyenergia)) || ((pxnave==pxenergia) && (pynave==pyenergia))) 
            {
              vpilha=false;
              pxenergia=-1;
              venergia=100;
            }
          }
      
    }

    delay(vel);
  }else {// Condição quando o jogo termina (ganhou ou perdeu)
          tela(vtela);
    if (digitalRead(bttiro)==1)
      {
       reset();
      }
  }
}

void desenhaNave(int px, int py)
{
  lcd.setCursor(px, py);
  lcd.write(1);
}

void desenhaAsteroid(int px, int py)
{
  lcd.setCursor(px, py);
  lcd.write(2);
}

void desenhaenErgia(int px, int py)
{
  lcd.setCursor(px, py);
  lcd.write(4);
}

void desenhaTiro(int px, int py)
{
  lcd.setCursor(px, py);
  lcd.write(5);
}

void desenhaExplosaoNave(int px, int py)
{
  lcd.clear();
  lcd.setCursor(px, py);
  lcd.write(3);
  delay(1000);
  lcd.clear();
}

void desenhaExplosaoAsteroid(int px, int py)
{
  lcd.setCursor(px, py);
  lcd.write(3);
  delay(1000);
  lcd.clear();
}

void reset()
{
  pontos=0;
  venergia=100;
  game=true;
  vtela=0;
}

void painel(int px)
{
  lcd.setCursor(px, 0);
  lcd.print(pontos);
  lcd.setCursor(px, 1);
  lcd.print(venergia);
}
// Função para mostrar a tela de vitória ou derrota
/*void tela(int cond)
{
  if(cond<=0)
    {
      lcd.setCursor(3, 0);
      lcd.print("CFB NAVE");
      lcd.setCursor(0, 1);
      lcd.print("Pressione Tiro");
    }
   else
    {
      String txt = {(cond>1 ? "GANHOU" : "PERDEU")}; // 0 = tela inicial | 1 = ganhou | 2 = perdeu
      lcd.setCursor(9, 0);
      lcd.print("pts:");
      lcd.setCursor(13, 0);
      lcd.print(pontos);
      lcd.setCursor(1, 0);
      lcd.print(txt);
      lcd.setCursor(0, 1);
      lcd.print("Pressione Tiro");
    
  }*/
  void tela(int cond)
{
  switch (cond) {
        /*case 0:
              lcd.setCursor(3, 0);
              lcd.print("CFB NAVE");
              lcd.setCursor(0, 1);
              lcd.print("Pressione Tiro");
              break;*/
        case 1:
              lcd.setCursor(9, 0);
              lcd.print("pts:");
              lcd.setCursor(13, 0);
              lcd.print(pontos);
              lcd.setCursor(1, 0);
              lcd.print("GANHOU" );
              lcd.setCursor(0, 1);
              lcd.print("Pressione Tiro");
              break;
        case 2:
              lcd.setCursor(9, 0);
              lcd.print("pts:");
              lcd.setCursor(13, 0);
              lcd.print(pontos);
              lcd.setCursor(1, 0);
              lcd.print("PERDEU");
              lcd.setCursor(0, 1);
              lcd.print("Pressione Tiro");
              break;
    
  }
}