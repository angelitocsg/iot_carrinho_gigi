/*
  Buzina Engraçada (sem delay)
  Autor: ChatGPT (GPT-5)
  Descrição: toca uma sequência de notas cômicas sem travar o loop principal.
*/

#include <Arduino.h>
#include <Thread.h>

// Estrutura de notas (frequência, duração)
struct Note
{
    int freq;
    unsigned long duration;
};

// Sequência da buzina (tipo "pi-pó-pé-po-pi!")
Note funnyHorn0[] = {
    {523, 200},  // C5
    {659, 200},  // E5
    {784, 200},  // G5
    {1047, 400}, // C6
    {784, 150},  // G5
    {659, 150},  // E5
    {523, 400}   // C5
};

// Sequência que simula o som "TURUUURU" (grave → sobe → desce)
Note funnyHorn1[] = {
  {300, 120}, // Tu
  {380, 120}, // ru
  {450, 150}, // úu
  {400, 120}, // ru
  {330, 200}, // uuu (descendo)
  {0,   100}  // pausa final
};

// Sequência da sirene (subida e descida)
Note funnyHorn[] = {
  {400, 100},  // tu
  {450, 100},  // ru
  {500, 100},  // uu
  {550, 100},  // ru
  {500, 100},  // uu
  {450, 100},  // ru
  {400, 200},  // final descida
  {0,   100}   // pausa
};

const int totalNotes = sizeof(funnyHorn) / sizeof(Note);

#define HORN 'h'
#define BUZZER_PIN A3

class MyHorn
{
private:
    bool isPlaying = false;
    int currentNote = 0;
    unsigned long noteStartTime = 0;

public:
    void loop()
    {
        if (!isPlaying || currentNote >= totalNotes)
            return;

        Serial.println("buzinando... isPlaying");
        Serial.println(isPlaying);
        Serial.println(currentNote);
        Serial.println(totalNotes);

        unsigned long now = millis();

        // Verifica se a nota atual terminou
        if (now - noteStartTime >= funnyHorn[currentNote].duration)
        {
            noTone(BUZZER_PIN);
            currentNote++;

            if (currentNote < totalNotes)
            {
                // Próxima nota
                tone(BUZZER_PIN, funnyHorn[currentNote].freq);
                noteStartTime = now;
            }
            else
            {
                // Sequência finalizada
                isPlaying = false;
                noTone(BUZZER_PIN);
            }
        }
    }
    void buzinar()
    {
        isPlaying = true;
        currentNote = 0;
        noteStartTime = millis();
        tone(BUZZER_PIN, funnyHorn[currentNote].freq);
    }
};
