void initUART();
void uart_transmit_char(char c);
void uart_transmit_string(const char* str);
char uart_receive_char();
void uart_receive_string(char* buffer, uint8_t max_length);
void send_command(const char* command, char* response, uint8_t response_size);
void check_connection_state();