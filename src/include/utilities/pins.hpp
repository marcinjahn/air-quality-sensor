#define set_pin_high(port, pin) (port |= (1 << pin))
#define set_pin_low(port, pin) (port &= ~(1 << pin))

#define get_pin_state(port, pin) ((port & (1 << pin)) > 0)