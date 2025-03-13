#include <Arduino.h>

// Definición de pines
const int switchPins[4] = {A0, A1, A2, A3}; // Pines de los switches
const int ledPins[5] = {13, 12, 11, 10, 9}; // Pines de los LEDs

// Estado inicial de los LEDs
bool EstadoLeds[5] = {false, false, false, false, false};

// Variables de tiempo
unsigned long previousMillis = 0;   //Almacena el tiempo que lleve millis()
int blinkContador = 0;   //Lleva la cuenta de cuantas veces a parpadeado el LED
int blinkLimite = 3;   //Define el límite de parpadeo por ciclos
int blinkIntervalo = 1000; // Intervalo de parpadeo (modificable según el switch activo)

void setup() {
    Serial.begin(9600);
    
    // Configurar switches como entradas con pull-up
    for (int i = 0; i < 4; i++) {
        pinMode(switchPins[i], INPUT);
    }
    
    // Configurar LEDs como salida
    for (int i = 0; i < 5; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

//DECLARACIÓN DE FUNCIONES (antes de usarlas)
int controlSwitches();
void controlLeds(int activeSwitch);
void apagarLeds();
void conmutadorLeds(int activeSwitch);

void loop() {

    int activeSwitch = controlSwitches(); // Detectar qué switch está activado
    controlLeds(activeSwitch); // Controlar los LEDs según el switch activo
    Serial.print("Switch activado: ");
    Serial.println(activeSwitch); // Imprimir estado en consola
    delay(2000);
}

// Función para leer switches
int controlSwitches() {
    for (int i = 0; i < 4; i++) {
        if (digitalRead(switchPins[i]) == LOW) {
            return i + 1; // Retorna el número de switch activado (1-4)
        }
    }
    return 0; // Ningún switch activado
}

// Función para controlar LEDs
void controlLeds(int activeSwitch) {
    static unsigned long lastMillis = 0;  //Almacena el tiempo de la última actualización de los LEDs
    static int blinkCounter = 0;   //Cuenta cuántas veces ha parpadeado el LED
    unsigned long currentMillis = millis();   //uarda el tiempo actual en milisegundos desde que se encendió el Arduino
    
    switch (activeSwitch) {
        case 1:
            blinkIntervalo = 333; // 3 veces por segundo
            break;
        case 2:
            blinkIntervalo = 167; // 3 veces por medio segundo
            break;
        case 3:
            blinkIntervalo = 83;  // 3 veces por cuarto de segundo
            break;
        case 4:
            blinkIntervalo = 500; // Todos los LEDs parpadean cada 1 segundo
            break;
        default:
            blinkCounter = 0;
            apagarLeds(); // Apaga todos los LEDs si no hay switches activados
            return;
    }
    
    if (currentMillis - lastMillis >= blinkIntervalo && blinkCounter < blinkLimite * 2) {   //verifica si ha pasado el tiempo suficiente desde el último cambio de estado del LED y comprueba el número de parpadeos
        lastMillis = currentMillis;   //Actualiza el tiempo antes de otro intervalo de parpadeo
        conmutadorLeds(activeSwitch);   //Cambia el estado del LED correspondiente
        blinkCounter++;   //Se incrementa el contador
    }
    
    if (blinkCounter >= blinkLimite * 2) {   //Verifica que no se exceda el límite de parpadeos
        blinkCounter = 0;   //Reinicia el contador
    }
}

// Función para apagar todos los LEDs
void apagarLeds() {
    for (int i = 0; i < 5; i++) {
        digitalWrite(ledPins[i], LOW);
        EstadoLeds[i] = false;
    }
}

// Función para alternar los LEDs según el switch activado
void conmutadorLeds(int activeSwitch) {
    apagarLeds();
    
    if (activeSwitch == 4) {
        // Si el switch 4 está activo, parpadean todos los LEDs
        for (int i = 0; i < 5; i++) {
            EstadoLeds[i] = !EstadoLeds[i];
            digitalWrite(ledPins[i], EstadoLeds[i]);
        }
    } else {
        // Solo parpadea un LED a la vez
        EstadoLeds[activeSwitch - 1] = !EstadoLeds[activeSwitch - 1];
        digitalWrite(ledPins[activeSwitch - 1], EstadoLeds[activeSwitch - 1]);
    }
}

// Función para imprimir el estado de los LEDs
void printStatus(int activeSwitch) {
    Serial.print("Switch activo: ");
    Serial.println(activeSwitch);
    
    Serial.print("LEDs: ");
    for (int i = 0; i < 5; i++) {
        Serial.print(EstadoLeds[i] ? "ON " : "OFF ");
    }
    Serial.println();
}