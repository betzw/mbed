#include "mbed.h"
#include <Ticker.h>

namespace {
	const int MS_INTERVALS = 1000;
}

static Ticker timer;
static DigitalOut led(LED1);
static DigitalIn button(USER_BUTTON);

static volatile bool irq_triggered = false;

static void print_char(char c)
{
	printf("%c", c);
	fflush(stdout);
}

/* Called in interrupt context, therefore just set a trigger variable */
static void timer_irq(void) {
	irq_triggered = true;
}

/* Everything is happening here 
   (and above all in "normal" context, i.e. not in IRQ context)
*/
static void main_cycle(void) {
	led = !led; // Blink
	if(button) 
		print_char('*');
	else
		print_char('-');
}

/* Generic main function for enabling WFI in case of 
   time based cyclic execution
*/
int main()
{
	timer.attach_us(timer_irq, MS_INTERVALS * 1000);

	while (true) {
		__disable_irq();
		if(irq_triggered) {
			irq_triggered = false;
			__enable_irq();
			main_cycle();
		} else {
			__enable_irq();
			__WFI();
		}
	}
}
