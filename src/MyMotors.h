#include <Arduino.h>
#include <AFMotor.h>

#define NORMAL_VELOCITY 150
#define TURBO_VELOCITY 255

#define TURN_LEFT 'l'
#define TURN_RIGHT 'r'
#define GO_FORWARD 'f'
#define GO_BACKWARD 'b'
#define START 'o'
#define STOP 's'
#define TURBO 't'

class MyMotors
{
private:
    AF_DCMotor *motor1;
    AF_DCMotor *motor2;
    AF_DCMotor *motor3;
    AF_DCMotor *motor4;
    int current_velocity = NORMAL_VELOCITY;
    bool turbo_mode = true;
    bool forward_direction = true;

    void vai()
    {
        if (forward_direction)
            anda_para_frente();
        else
            anda_para_tras();
    }

    int turn_velocity()
    {
        return current_velocity / 2.5;
    }

public:
    MyMotors()
    {
        Serial.println("inicia motores 1 e 2");
        motor1 = new AF_DCMotor(1);
        motor2 = new AF_DCMotor(2);
        motor3 = new AF_DCMotor(3);
        motor4 = new AF_DCMotor(4);
        libera_motores();
    }

    bool esta_indo_para_frente()
    {
        return forward_direction;
    }

    void libera_motores()
    {
        Serial.println("libera motores");
        motor1->run(RELEASE);
        motor2->run(RELEASE);
        motor3->run(RELEASE);
        motor4->run(RELEASE);
    }

    void desliga_motores()
    {
        libera_motores();
        delay(500);
    }

    void anda_para_frente()
    {
        String msg = "anda pra frente : ";
        Serial.println(turbo_mode ? msg + "turbo" : msg + "normal");
        forward_direction = true;
        motor1->setSpeed(current_velocity);
        motor2->setSpeed(current_velocity);
        motor3->setSpeed(current_velocity);
        motor4->setSpeed(current_velocity);
        motor1->run(FORWARD);
        motor2->run(FORWARD);
        motor3->run(FORWARD);
        motor4->run(FORWARD);
    }

    void anda_para_tras()
    {
        String msg = "anda pra tras : ";
        Serial.println(turbo_mode ? msg + "turbo" : msg + "normal");
        forward_direction = false;
        motor1->setSpeed(current_velocity);
        motor2->setSpeed(current_velocity);
        motor3->setSpeed(current_velocity);
        motor4->setSpeed(current_velocity);
        motor1->run(BACKWARD);
        motor2->run(BACKWARD);
        motor3->run(BACKWARD);
        motor4->run(BACKWARD);
    }

    void virar_para_esquerda()
    {
        Serial.println("vira pra esquerda: " + String(current_velocity) + " | " + String(turn_velocity()));
        motor1->setSpeed(turn_velocity());
        motor2->setSpeed(current_velocity);
        motor3->setSpeed(current_velocity);
        motor4->setSpeed(turn_velocity());
        motor1->run(forward_direction ? FORWARD : BACKWARD);
        motor2->run(forward_direction ? FORWARD : BACKWARD);
        motor3->run(forward_direction ? FORWARD : BACKWARD);
        motor4->run(forward_direction ? FORWARD : BACKWARD);
    }

    void virar_para_direita()
    {
        Serial.println("vira pra direita: " + String(turn_velocity()) + " | " + String(current_velocity));
        motor1->setSpeed(current_velocity);
        motor2->setSpeed(turn_velocity());
        motor3->setSpeed(turn_velocity());
        motor4->setSpeed(current_velocity);
        motor1->run(forward_direction ? FORWARD : BACKWARD);
        motor2->run(forward_direction ? FORWARD : BACKWARD);
        motor3->run(forward_direction ? FORWARD : BACKWARD);
        motor4->run(forward_direction ? FORWARD : BACKWARD);
    }

    void turbina_motores()
    {
        turbo_mode = !turbo_mode;
        Serial.println("alterna turbo: " + String(turbo_mode ? "turbo" : "normal"));

        if (!turbo_mode)
        {
            current_velocity = NORMAL_VELOCITY;
            vai();
            return;
        }

        current_velocity = TURBO_VELOCITY;
        vai();
    }
};