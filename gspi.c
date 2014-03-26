//�˳���ʵ��8��LED��������
//��ʹ�õĿ�����ΪMEGA128��PB��8��LED�ƣ�1Ϊ����0Ϊ��
//��ʹ�ò�ͬ�Ŀ����壬��Ӧ��Ӧ�޸�

#include <avr/io.h> //AVRͷ�ļ�
#include <util/delay.h> //�ڲ�����ͷ�ļ�

#include <inttypes.h>

// CS PB1
// MOSI PB3
// MISO PB4
// SCLK PB5
/* Please define these macros */
#define SCLK_HIGH()  PORTB |= 1 << 5
#define SCLK_LOW()   PORTB &= ~(unsigned char)(1 << 5)
#define SS_HIGH()    PORTB |= 1 << 1
#define SS_LOW()     PORTB &= ~(unsigned char)(1 << 1)
#define MOSI_HIGH()  PORTB |= 1 << 3
#define MOSI_LOW()   PORTB &= ~(unsigned char)(1 << 3)
#define MISO_GET()   ((PINB >> 4) & 1)

void gspi_initialize(void) {
  SCLK_LOW();
  SS_HIGH();
}

void gspi_write(uint16_t address, uint16_t data) {
  SS_LOW();
  MOSI_LOW();  /* write */
  SCLK_HIGH();
  SCLK_LOW();

  /* ?RSV */
  SCLK_HIGH();
  SCLK_LOW();
  SCLK_HIGH();
  SCLK_LOW();

  /* NO auto inc */
  SCLK_HIGH();
  SCLK_LOW();

  for (int i = 0; i < 12; ++i) {
    if (0x0800 & address) {
      MOSI_HIGH();
    } else {
      MOSI_LOW();
    }
    SCLK_HIGH();
    address <<= 1;
    SCLK_LOW();
  }

  for (int i = 0; i < 16; ++i) {
    if (0x8000 & data) {
      MOSI_HIGH();
    } else {
      MOSI_LOW();
    }
    SCLK_HIGH();
    data <<= 1;
    SCLK_LOW();
  }

  SS_HIGH();
}

void gspi_read(uint16_t address, uint16_t *data) {
  SS_LOW();
  MOSI_HIGH();  /* read */
  SCLK_HIGH();
  SCLK_LOW();

  /* ?RSV */
  SCLK_HIGH();
  SCLK_LOW();
  SCLK_HIGH();
  SCLK_LOW();
  /* Auto inc */
  SCLK_HIGH();
  SCLK_LOW();

  for (int i = 0; i < 12; ++i) {
    if (0x0800 & address) {
      MOSI_HIGH();
    } else {
      MOSI_LOW();
    }
    SCLK_HIGH();
    address <<= 1;
    SCLK_LOW();
  }

  *data = 0u;
  for (int i = 0; i < 16; ++i) {
    *data <<= 1;
    SCLK_HIGH();
    *data |= MISO_GET() ? 0x0001 : 0x0000;
    SCLK_LOW();
  }

  SS_HIGH();
}

// CS PB1
// MOSI PB3
// MISO PB4
// SCLK PB5
int main(void)
{
  uint16_t data = 0;

  // Init IO
  PORTB = 0xff;
  DDRB = 0;
  DDRB |= 0x2a;

  // test PIN
  PORTC = 0xff;
  DDRC = 0x07;
 
  for (;;) {    
    gspi_read(0x06, &data);
    data >>= 4;
    PORTC = (unsigned char)data;
  }




#if 0
    PORTB = 0xff;//��ʼ��PB��
    DDRB |= 1 << 5;//����PB��Ϊ���

    while(1) {
        PORTB |= 1 << 5;
        _delay_us(500);//��ʱ500����
        PORTB &= ~(unsigned char)(1 << 5);
        _delay_us(500);//��ʱ500����
    }
#endif

}
