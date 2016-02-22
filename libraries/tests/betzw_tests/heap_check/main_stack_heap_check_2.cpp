#include "mbed.h"
#include <new>

DigitalOut myled(LED1);

Serial *bs;

void SunFunc() {
    uint32_t temp[16];
    bs->printf("stackSunFunc=0x%x\r\n", &temp);
}

uint32_t MaxMemoryBlockAvailable(void)
{
    int i = 0;
    for (i = 1; i < 1024; i++) {
        char *p_buffer = new (std::nothrow) char[i*1024];
        bs->printf("p_buffer(%d kb)=0x%x-0x%x", i, p_buffer, p_buffer+i*1024);
        if (p_buffer) {
             delete[] p_buffer;
             bs->printf(" deleted OK\r\n");
        }
        else {
            i--;
            bs->printf(" not created\r\n");          
            break;
        }
    }
    bs->printf("Max memory available: %d kB\r\n", i);
    return i;
}

int main() {
    uint32_t temp = 0; // STACK
  
    bs = new Serial(USBTX, USBRX); // DATA
    // bs->baud(115200);
    bs->printf("\r\n\r\n*** Stack/Heap check ***\r\n");

    char *p1 = new char[4096]; // HEAP
    memset(p1, 0, 4096);

    bs->printf(" main=0x%x\r\n", &main); // Doesn't compile with IAR
    bs->printf(" data=0x%x\r\n", &bs);
    bs->printf("stack=0x%x\r\n", &temp);
    bs->printf("   p1=0x%x-0x%x\r\n", p1, p1+4096);
    
    SunFunc();

    MaxMemoryBlockAvailable();

    //for (int i = 0; i < 21; i++) {
    int i = 1;
    do {
        void *t = malloc(1024); // HEAP
        if (t == NULL) {
            bs->printf("malloc: Out of memory\r\n");
            break;
        }
        memset(t, 0, 1024);
        bs->printf(" data(%d)=0x%x-0x%x created\r\n", i, t, (char *)t+1024); // Doesn't compile with IAR
        //bs->printf(" data(%d)=0x%x-0x created\r\n", i, t); // For IAR
        i++;
    } while(1);

    while(1) {
        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
