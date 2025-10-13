#include <Arduino.h>
#include <Thread.h>

#define LIGHT 'g'
#define LIGTH_PIN A5

class MyLights
{
private:
    bool statusFarol = false;

public:
    bool obter_status_farol()
    {
        return statusFarol;
    }

    void acende_farol()
    {
        analogWrite(LIGTH_PIN, 230);
        statusFarol = true;
    }

    void apaga_farol()
    {
        analogWrite(LIGTH_PIN, 0);
        statusFarol = false;
    }
};