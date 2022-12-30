#define set_register_bit_high(port, pin) (port |= (1 << pin))
#define set_register_bit_low(port, pin) (port &= ~(1 << pin))

#define get_register_bit(port, pin) ((port & (1 << pin)) > 0)