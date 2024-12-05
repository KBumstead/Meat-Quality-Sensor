#ifndef lcd2_h
#define lcd2_h

void init_i2c();
void startI2C_Trans();
void stopI2C_Trans();
void write(uint8_t data);
void LCD_4BitCommand(uint8_t nibble, uint8_t control);
void LCD_8BitCommand(uint8_t byte, uint8_t control);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_Init();
void LCD_Clear();
void LCD_moveCursor(uint8_t row, uint8_t col);
void LCD_WriteString(const char *str);

#endif