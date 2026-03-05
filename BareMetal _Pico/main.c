#include <stdint.h>

// --- MAPA DE MEMORIA (RP2040 Datasheet) ---

// 1. SUBSYSTEM RESETS (Base: 0x4000C000)
// Controla que perifericos estan despiertos o dormidos.
#define RESETS_BASE    0x4000C000
#define RESETS_RESET    (*(volatile uint32_t *) (RESETS_BASE + 0x0))
#define RESETS_DONE    (*(volatile uint32_t *) (RESETS_BASE + 0x8))

// 2. IO BANK 0 (Base: 0x40014000)
// Controla la funcion logica de los pines (GPIO, UART, etc).
#define IO_BANK0_BASE    0x40014000
// El pin 25 tiene un offset de 0x0CC
#define GPIO25_CTRL    (*(volatile uint32_t *) (IO_BANK0_BASE + 0x0CC))

// 3. SINGLE-CYCLE IO (SIO) (Base: 0xD0000000)
// Controla el estado alto/bajo de los pines de forma rapida.
#define SIO_BASE    0xD0000000
#define SIO_GPIO_OE    (*(volatile uint32_t *) (SIO_BASE + 0x020)) // Output Enable
#define SIO_GPIO_OUT   (*(volatile uint32_t *) (SIO_BASE + 0x010)) // Output Value
// NOTA: Si usas Pico W, cambia 25 por un pin externo (ej. 16)
#define LED_PIN    25

// Encender led 
void encender_led() {
    SIO_GPIO_OUT |= (1 << LED_PIN);  // Bit en 1 = encendido
}

// Apagar  led 
void apagar_led() {
    SIO_GPIO_OUT &= ~(1 << LED_PIN);  // Bit en 0 = apagado
}

// Tiempo de espera
void esperar(int segundos){
    for (volatile int s = 0; s < segundos; s++) {
        // 1 segundo de retardo
        for (volatile int i = 0; i < 3000000; i++);
    }
}

/**
 * funcion para que la placa trasnmita una serie de bits
 *   ejemplo : si la palabra es 1010
 *   la placa encendera el foco 1 segundo luego lo apagara 1segundos 
 *   y volvera a prender y apagar el foco otrra vez en intervalos de un segundos
 * @param palabra  Palabra compuesta de 0's y 1's
 */
void pasar_palabra(char palabra[]){
    int bandera = 0;
    for (volatile int i = 0; i < 4;i++){
        if(palabra[i]=='1'){
            encender_led();
            bandera = 1;
        } else {
            apagar_led();
            bandera = 0;
        }
        // dura en el estado (prendido/apagado) un segundo
        esperar(5);
        apagar_led();  
        esperar(1) ;



    }
}

int main() {
    // --- PASO 1: BOOTSTRAPPING (Despertar Hardware) ---
    // Debemos quitar el Reset a dos componentes:
    // Bit 5: IO_BANK0 (Logica de pines)
    // Bit 8: PADS_BANK0 (Electricidad de pines)
    // Escribimos 0 en esos bits para sacarlos del reset.
    RESETS_RESET &= ~((1 << 5) | (1 << 8));

    // Esperamos hasta que el hardware confirme que desierto
    while ((RESETS_DONE & ((1 << 5) | (1 << 8))) != ((1 << 5) | (1 << 8)));

    // --- PASO 2: CONFIGURACION ---
    // Seleccionamos la Funcion 5 (SIO - Software Control) para el Pin 25
    GPIO25_CTRL = 5;

    // Habilitamos el Pin 25 como SALIDA en el SIO
    SIO_GPIO_OE |= (1 << LED_PIN);

    // --- PASO 3: BUCLE INFINITO (Kernel Loop) ---
    /**while (1) {
    // ENCENDER: Usamos XOR (~) para invertir el estado actual
    SIO_GPIO_OUT ^= (1 << LED_PIN);

    // RETARDO: Ciclo vacio. Usamos 'volatile' para que el compilador
    // no elimine este bucle.
    for (volatile int x = 0; x < 10; x++)
    {
    for (volatile int i = 0; i < 500000; i++);
    }
    }*/

    // Mi numiero de cuenta : 317095346
    char tres[] = "0011";
    char uno[] = "0001";
    char siete[] = "0111";
    char cero[] = "0000";
    char nueve[] = "1001";
    char cinco[] = "0101";
    //             0012
    char cuatro[] = "0100";
    char seis[] = "0110";

    while (1)
    {
        
        esperar(10);
        pasar_palabra(tres);
        esperar(5);

        pasar_palabra(uno);
        esperar(5);

        pasar_palabra(siete);
        esperar(5);

        pasar_palabra(cero);
        esperar(5);

        pasar_palabra(nueve);
        esperar(5);

        pasar_palabra(cinco);
        esperar(5);

        pasar_palabra(tres);
        esperar(5);

        pasar_palabra(cuatro);
        esperar(5);

        pasar_palabra(seis);
        esperar(15);
    
        

    }
    
    

    return 0;
}
