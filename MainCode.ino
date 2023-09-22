/*
 * EUREKA - ESCOLA DE TECNOLOGIA E PESQUISA
 * 
 * VACCINE CONTROL
 * 
 * JEAN RAFAEL KOT
 * ARTHUR ROTHENBURG
 * 
 * CASCAVEL - PR
 */

//Inclusão de bibliotecas
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

//Define saidas dos leds de status
#define LedYellow 9
#define LedRed 8
#define LedGreen 10

//Define a saida do relé
#define RelePin 6

//Pino conectado ao pino de dados do sensor
#define DHTPIN 7

//Define o modelo do DHT
#define DHTTYPE DHT22   // Sensor DHT 22  (AM2302)

//Definições do sensor do DHT para: pino e tipo
DHT dht(DHTPIN, DHTTYPE);

//Array simbolo de grau
byte grau[8] ={ B00001100, 
                B00010010, 
                B00010010, 
                B00001100, 
                B00000000, 
                B00000000, 
                B00000000, 
                B00000000,}; 

void setup() 
{
  //Inicializa o display
  lcd.begin(16, 2);
  lcd.clear();
  lcd.init();
  
  //Settup inicial da lcd
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("VACCINE CONTROL");
  lcd.setCursor(0,1);
  lcd.print("EUREKA CASCAVEL");
  lcd.setCursor(0,0);
  delay(3000);
  lcd.clear();
  
  //Cria o caracter customizado com o simbolo do grau
  lcd.createChar(0, grau); 
  
  //Informacoes iniciais no display
  lcd.setCursor(0,0);
  lcd.print("Temp. : ");
  lcd.setCursor(13,0);
  
  //Mostra o simbolo do grau
  lcd.write(byte(0));
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Umid. : ");
  lcd.setCursor(14,1);
  lcd.print("%");
  Serial.begin(9600); 
  Serial.println("Aguardando dados...");
 
  //Iniclaiza o sensor DHT
  dht.begin();
}

void loop() 
{
  //Aguarda 2 segundos entre as medicoes
  delay(2000);
  
  //Leitura da umidade
  float h = dht.readHumidity();
  //Leitura da temperatura (Celsius)
  float t = dht.readTemperature();
  
  //Verifica se o sensor esta respondendo
  if (isnan(h) || isnan(t))
  {
    Serial.println("Falha ao ler dados do sensor DHT !!!");
    return;
  }

  //Mostra a temperatura no serial monitor e no display
  Serial.print("Temperatura: "); 
  Serial.print(t);
  lcd.setCursor(8,0);
  lcd.print(t);
  Serial.print(" *C  ");
  //Mostra a umidade no serial monitor e no display
  Serial.print("Umidade : "); 
  Serial.print(h);
  Serial.println(" %");
  lcd.setCursor(8,1);
  lcd.print(h);
  
  //Ativa os leds e relé de acordo com a temperatura
  //Ativa o led verde quando temperatura estiver entre 0 e 10
  if (t > 0 && t <= 10)
  {
    digitalWrite(LedGreen, HIGH);
  }

  //Ativa o led amarelo e o rele alternadamente em um intervalo de 5 segundos quando temperatura estiver entre 10 e 20
  if (t > 10 && t <= 20)
  {
    digitalWrite(LedYellow, HIGH);
    digitalWrite(RelePin, HIGH);
    delay(5000);
    digitalWrite(RelePin, LOW);
    delay(5000);
  }

  //Ativa o led vermelho e o rele quando temperatura estiver entre 20 e 30
  if (t > 20 && t <= 30)
  {
    digitalWrite(LedRed, HIGH);
    digitalWrite(RelePin, HIGH);
  }

  //caso a temperatura não esteja dentre essas, desligar leds e rele
  else
  {
    digitalWrite(LedGreen, LOW);
    digitalWrite(LedYellow, LOW);
    digitalWrite(LedRed, LOW);
    digitalWrite(RelePin, LOW);
  }
}
