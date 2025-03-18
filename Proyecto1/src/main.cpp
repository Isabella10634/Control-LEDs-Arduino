#include <Arduino.h>

/* Declaración de funciones */
int readSwitches();   //Lee el estado del switch
void updateLeds(int mode);   //Actualiza el estado de un led
void updateAllLEDs();   //Actualiza el estado de todos los  led
void printStatus();   

// Definición de pines
const int switchPins[4] = {A0, A1, A2, A3}; // Pines de los switches
const int ledPins[5] = {13, 12, 11, 10, 9}; // Pines de los LEDs

// Array para almacenar los estados de los LEDs
bool ledStates[5] = {false, false, false, false, false};

// Variables para el manejo de tiempos y modo actual
unsigned long lastToggleTime = 0;   //Guarda el tiempo en que se realizó el último cambio de estado de LED
int currentMode = 0;   // 0: ninguno; 1: switch 1; 2: switch 2; 3: switch 3; 4: switch 4. Indica el modo actual dependiendo del switch activado
int lastMode = 0;   //Guarda el último modo activo para detectar cambios

void setup() {
  Serial.begin(9600);
  
  // Configuración de los pines de los switches como INPUT
  for (int i = 0; i < 4; i++) {
    pinMode(switchPins[i], INPUT);
  }
  
  // Configuración de los pines de los LEDs como salida
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  Serial.println("Sistema iniciado.");   //Mensaje de inicio en el Monitor Serial
}

void loop() {
  // Lee los switches y determina el modo (1 a 4), o 0 si ninguno está presionado
  currentMode = readSwitches();
  
  // Si se produce un cambio de modo, reinicia el temporizador y muestra el cambio
  if (currentMode != lastMode) {
    lastToggleTime = millis();   //Si cambia el modo, actualiza lastToggleTime
    lastMode = currentMode;   //Guarda el nuevo modo
    Serial.print("Cambio de modo a: ");
    Serial.println(currentMode);
  }
  
  updateLeds(currentMode);   // Actualiza el parpadeo según el modo seleccionado
  updateAllLEDs();   // Llama la función para actualizar el estado de los pines de salida con digitalWrite()
  printStatus();   // Imprime el estado de los LEDs
  delay(10);   //Retardo para observar mejor los mensajes en el Monitor Serial
}

/*
   readSwitches():
   Recorre los 4 pines de switch y usa una variable bool switchState para almacenar su lectura.
   El switch se activa (presionado) cuando digitalRead() es LOW.
   Se retorna el número del switch presionado (1 a 4) o 0 si ninguno está activado.
*/
int readSwitches() {
  bool switchState = false;   //Detecta si el switch está presionado
  for (int i = 0; i < 4; i++) {
    switchState = (digitalRead(switchPins[i]) == LOW);
    if (switchState) {
      return i + 1;  // Retorna 1 para switchPins[0], 2 para switchPins[1], etc.
    }
  }
  return 0; // Ningún switch presionado
}

/*
   updateLeds():
   Utiliza un switch-case para definir el comportamiento según el modo:
   - Modo 1: LED 1 parpadea 3 veces por cada segundo (toggle/conmutación cada ~166 ms) y los demás apagados.
   - Modo 2: LED 2 parpadea 3 veces por cada medio segundo (toggle/conmutación cada ~83 ms) y los demás apagados.
   - Modo 3: LED 3 parpadea 3 veces por cada cuarto de segundo (toggle/conmutación cada ~41 ms) y los demás apagados.
   - Modo 4: Todos los LEDs parpadean (toggle/conmutación cada 500 ms).
   - Si no hay modo (0), se apagan todos los LEDs.
*/
void updateLeds(int mode) {
  unsigned long currentMillis = millis();   //Guarda el tiempo actual desde que se ejecutó el programa usando millis()
  
  switch (mode) {
    case 1: {
      if (currentMillis - lastToggleTime >= 333) { //Indica cuánto tiempo ha pasado desde el últimmo cambio de estado de LED y lo compara con el tiempo requerido. En este caso ~333 ms para 3 ciclos en 1 s
        Serial.println(currentMillis - lastToggleTime);
        lastToggleTime = currentMillis;   //Se actualiza el tiempo desde el cambio de estado de LED
        ledStates[0] = !ledStates[0]; //Alterna el estado del LED
        // Aseguramos que los demás LEDs permanezcan apagados
        ledStates[1] = ledStates[2] = ledStates[3] = ledStates[4] = false;
        Serial.println("Parpadeo LED 1");
      }
      break;
    }
    case 2: {
      if (currentMillis - lastToggleTime >= 166) {  // ~166 ms para 3 ciclos en 0.5 s
        Serial.println(currentMillis - lastToggleTime);
        lastToggleTime = currentMillis;
        ledStates[1] = !ledStates[1];
        ledStates[0] = ledStates[2] = ledStates[3] = ledStates[4] = false;
        
        Serial.println("Parpadeo LED 2");
      }
      break;
    }
    case 3: {
      if (currentMillis - lastToggleTime >= 83) {  // ~83 ms para 3 ciclos en 0.25 s
        Serial.println(currentMillis - lastToggleTime);
        lastToggleTime = currentMillis;
        ledStates[2] = !ledStates[2];
        ledStates[0] = ledStates[1] = ledStates[3] = ledStates[4] = false;
        Serial.println("Parpadeo LED 3");
      }
      break;
    }
    case 4: {
      if (currentMillis - lastToggleTime >= 500) {  // 500 ms para un parpadeo de 1 Hz (toggle cada 500 ms)
        Serial.println(currentMillis - lastToggleTime);
        lastToggleTime = currentMillis;
        // Alterna el estado de todos los LEDs
        for (int i = 0; i < 5; i++) {
          ledStates[i] = !ledStates[i];
        }
        Serial.println("Parpadeo TODOS los LEDs");
      }
      break;
    }
    default: {
      // Si ningún switch está activado, se apagan todos los LEDs
      for (int i = 0; i < 5; i++) {
        ledStates[i] = false;
      }
      break;
    }
  }
}

/*
   updateAllLEDs():
   Recorre el array de estados ledStates usando un while y actualiza los pines de salida.
*/
void updateAllLEDs() {
  int i = 0;
  while (i < 5) {
    digitalWrite(ledPins[i], ledStates[i] ? HIGH : LOW);
    i++;
  }
}

/*
   printStatus():
   Imprime por Serial el estado (ON/OFF) de cada LED para fines de depuración.
*/
void printStatus() {
  Serial.print("Estado LEDs: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(ledStates[i] ? "ON " : "OFF ");
  }
  Serial.println();
}