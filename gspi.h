#ifndef GSPI_GSPI_H_
#define GSPI_GSPI_H_

#include <inttypes.h>

void gspi_initialize(void);
void gspi_write(uint16_t address, uint16_t data);
void gspi_read(uint16_t address, uint16_t *data);

#endif


