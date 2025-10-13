#ifdef FLAG_CAR

#include <Arduino.h>
#include <MyHorn.h>
#include <MyLights.h>
#include <MyMotors.h>
#include <MyReceiver.h>

MyMotors motors;
MyLights lights;
MyHorn horn;
void do_action(char c);

void setup()
{
  Serial.begin(9600);
  Serial.println("Iniciando...");
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A5, OUTPUT);
}

void loop()
{
  while (Serial.available())
  {
    char c = (char)Serial.read();
    do_action(c);
  }
  horn.loop();
}

void do_action(char c)
{
  Serial.print("Recebido: ");
  Serial.println(c);
  switch (c)
  {
  case START:
    lights.acende_farol();
    motors.libera_motores();
    break;
  case STOP:
    lights.apaga_farol();
    motors.desliga_motores();
    break;
  case GO_FORWARD:
    motors.anda_para_frente();
    break;
  case GO_BACKWARD:
    motors.anda_para_tras();
    break;
  case TURN_LEFT:
    if (motors.esta_indo_para_frente())
      motors.virar_para_esquerda();
    else
      motors.virar_para_direita();
    break;
  case TURN_RIGHT:
    if (motors.esta_indo_para_frente())
      motors.virar_para_direita();
    else
      motors.virar_para_esquerda();
    break;
  case LIGHT:
    if (lights.obter_status_farol())
      lights.apaga_farol();
    else
      lights.acende_farol();
    break;
  case TURBO:
    motors.turbina_motores();
    break;
  case HORN:
    horn.buzinar();
    break;
  default:
    motors.desliga_motores();
    break;
  }
}

void change_bt_settings()
{
  Serial.println("Setting...");
  Serial.write("AT+");
  delay(1000);
  Serial.write("AT+NAMEGIGICAR");
  delay(1000);
  Serial.write("AT+PIN1234");
  delay(1000);
}

#endif