/*
  Dummy for piface
*/

#include <stdint.h>

int     pfio_init(void) { return 1;}
int     pfio_deinit(void) {return 1;}
uint8_t pfio_digital_read(uint8_t pin_number) {return 0;}
void    pfio_digital_write(uint8_t pin_number, uint8_t value) {}
uint8_t pfio_read_input(void) {return 0;}
uint8_t pfio_read_output(void) {return 0;}
void    pfio_write_output(uint8_t value) {}
uint8_t pfio_get_pin_bit_mask(uint8_t pin_number) {return 0;}
uint8_t pfio_get_pin_number(uint8_t bit_mask) {return 0;}



