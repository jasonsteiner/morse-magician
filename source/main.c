#include <pin_mux.h>
#include <clock_config.h>
#include <stdio.h>
#include <board.h>
#include <MKL46Z4.h>
#include <fsl_debug_console.h>
#include <string.h>

void SetupADC(){

	int cal_v;

	//Enable clock gate for ADC0
	SIM->SCGC6 |= (1 << 27);

	// Setup ADC
	ADC0->CFG1 = 0;  // Default everything.
	ADC0->CFG1 |= ADC_CFG1_ADICLK(0b00); // Use bus clock.
	ADC0->CFG1 |= ADC_CFG1_MODE(0b10);   // 00 for 8-bit
	                                     // 01 for 12-bit
	                                     // 10 for 10-bit
	                                     // 11 for 16-bit

	//Calibrate
	ADC0->SC3 = 0;
	ADC0->SC3 |= ADC_SC3_AVGS(0b11);  // SelectMaximum Hardware Averaging (32) see 28.3.7 for details
	ADC0->SC3 |= ADC_SC3_AVGE_MASK;   // Enable Hardware Averaging
	ADC0->SC3 |= ADC_SC3_CAL_MASK;    // Start Calibration

	// Wait for calibration to complete
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

	//Assume calibration worked, or check ADC_SC3_CALF

	// Calibration Complete, write calibration registers.
	cal_v = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0->PG = cal_v;

	cal_v = 0;
	cal_v = ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0->MG = cal_v;


	ADC0->SC3 = 0;  //Turn off hardware averaging for faster conversion
			//or enable as above in calibration.
	return;
}
/*------------------------------*/
/* UART communication functions */
/*------------------------------*/
/* Initialize the UART for TX (115200, 8N1) */

void init_uart(void)
{
	//Use UART port through debug interface
	// Connect to UART with TX (115200, 8N1)

	BOARD_InitBootPins();
        BOARD_InitBootClocks();
 	BOARD_InitDebugConsole();
}

void uart_putc (char ch)
{
	/* Wait until space is available in the FIFO */
	while(!(UART0->S1 & UART_S1_TDRE_MASK));
	/* Send the character */
	UART0->D = (uint8_t)ch;
}

void uart_puts(char *ptr_str)
{
    while(*ptr_str){
			/* Replace newlines with \r\n carriage return */
			if(*ptr_str == '\n') { uart_putc('\r'); }
      uart_putc(*ptr_str++);
		}
}

void short_delay()
{
	for(int i=1000000; i>0; i--){}
}

void delay_ms(int ms) {
    int i;
    for (i = 0; i < ms; i++) {
        int j;
        for (j = 0; j < 3500; j++) {
            __asm__("nop");
        }
    }
}

char* morse_decode(const char* morse) {
    if (strcmp(morse, ".-") == 0) return "A";
    if (strcmp(morse, "-...") == 0) return "B";
    if (strcmp(morse, "-.-.") == 0) return "C";
    if (strcmp(morse, "-..") == 0) return "D";
    if (strcmp(morse, ".") == 0) return "E";
    if (strcmp(morse, "..-.") == 0) return "F";
    if (strcmp(morse, "--.") == 0) return "G";
    if (strcmp(morse, "....") == 0) return "H";
    if (strcmp(morse, "..") == 0) return "I";
    if (strcmp(morse, ".---") == 0) return "J";
    if (strcmp(morse, "-.-") == 0) return "K";
    if (strcmp(morse, ".-..") == 0) return "L";
    if (strcmp(morse, "--") == 0) return "M";
    if (strcmp(morse, "-.") == 0) return "N";
    if (strcmp(morse, "---") == 0) return "O";
    if (strcmp(morse, ".--.") == 0) return "P";
    if (strcmp(morse, "--.-") == 0) return "Q";
    if (strcmp(morse, ".-.") == 0) return "R";
    if (strcmp(morse, "...") == 0) return "S";
    if (strcmp(morse, "-") == 0) return "T";
    if (strcmp(morse, "..-") == 0) return "U";
    if (strcmp(morse, "...-") == 0) return "V";
    if (strcmp(morse, ".--") == 0) return "W";
    if (strcmp(morse, "-..-") == 0) return "X";
    if (strcmp(morse, "-.--") == 0) return "Y";
    if (strcmp(morse, "--..") == 0) return "Z";
    if (strcmp(morse, "-----") == 0) return "0";
    if (strcmp(morse, ".----") == 0) return "1";
    if (strcmp(morse, "..---") == 0) return "2";
    if (strcmp(morse, "...--") == 0) return "3";
    if (strcmp(morse, "....-") == 0) return "4";
    if (strcmp(morse, ".....") == 0) return "5";
    if (strcmp(morse, "-....") == 0) return "6";
    if (strcmp(morse, "--...") == 0) return "7";
    if (strcmp(morse, "---..") == 0) return "8";
    if (strcmp(morse, "----.") == 0) return "9";
    if (strcmp(morse, ".-.-.-") == 0) return ".";
    if (strcmp(morse, "--..--") == 0) return ",";
    if (strcmp(morse, "..--..") == 0) return "?";
    if (strcmp(morse, ".----.") == 0) return "'";
    if (strcmp(morse, "-.-.--") == 0) return "!";
    if (strcmp(morse, "-..-.") == 0) return "/";
    if (strcmp(morse, "-.--.") == 0) return "(";
    if (strcmp(morse, "-.--.-") == 0) return ")";
    if (strcmp(morse, ".-...") == 0) return "&";
    if (strcmp(morse, "---...") == 0) return ":";
    if (strcmp(morse, "-.-.-.") == 0) return ";";
    if (strcmp(morse, "-...-") == 0) return "=";
    if (strcmp(morse, ".-.-.") == 0) return "+";
    if (strcmp(morse, "-....-") == 0) return "-";
    if (strcmp(morse, "..--.-") == 0) return "_";
    if (strcmp(morse, ".-..-.") == 0) return "\"";
    if (strcmp(morse, "...-..-") == 0) return "$";
    if (strcmp(morse, ".--.-.") == 0) return "@";
    return " ";
}

int main(void) {
    int light_val;
    char light_str[10];
    int light_on_time = 0;
    int light_off_time = 0;
    int light_threshold = 250;
    int dot_threshold = 200;
    char morse_buffer[20] = "";
    int off_count = 0;
    int waiting_time = 0;
    int char_threshold = 2500;   // threshold for decoding character (around 5 secs)
    int done_threshold = 10000;  // threshold for printing decoded message (around 10 secs)
    int is_done = 0;  // flag indicating whether program is finished
    char decoded_message[100] = "";  // buffer storing decoded message

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    SetupADC();

    SIM->SCGC5 |= (1<<13);

    init_uart();

    while(!is_done) {
        ADC0->SC1[0] = ADC_SC1_ADCH(3);
        while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
        light_val = ADC0->R[0];

        if (light_val <= light_threshold) {
            light_on_time++;
            light_off_time = 0;
            off_count = 0;
            waiting_time = 0;  // reset waiting time while light on
        } else {
            if (light_on_time > 0) {
                if (light_on_time <= dot_threshold) {
                    uart_putc('.');
                    strcat(morse_buffer, ".");
                } else {
                    uart_putc('-');
                    strcat(morse_buffer, "-");
                }
                light_on_time = 0;
            }
            light_off_time++;
            off_count++;
            waiting_time++;  // increment waiting time while light off
        }

        if (off_count >= char_threshold) {
            if (strlen(morse_buffer) > 0) {  // morse_buffer not empty
                char decoded_char = *morse_decode(morse_buffer);
                uart_puts("\n");
                uart_putc(decoded_char);
                strncat(decoded_message, &decoded_char, 1);  // append decoded character to message buffer
                uart_puts("\n");
                memset(morse_buffer, 0, sizeof(morse_buffer));
            }
            off_count = 0;  // reset off_count after decoding a character
        }

        if (waiting_time >= done_threshold && !is_done) {
        	uart_puts("\n");
            uart_puts("Decoded message: ");
            uart_puts(decoded_message);
            uart_puts("\n");
            is_done = 1;  // set flag to indicate program finishing
        }

        delay_ms(1);
    }
    return 0;
}
