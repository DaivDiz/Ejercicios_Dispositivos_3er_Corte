# 1 "HDM64GS12.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Users/dsdia/.mchp_packs/Microchip/PIC18Fxxxx_DFP/1.2.26/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "HDM64GS12.c" 2
# 108 "HDM64GS12.c"
void glcd_init(int1 mode);
void glcd_pixel(unsigned int8 x, unsigned int8 y, int1 color);
void glcd_fillScreen(int1 color);
void glcd_writeByte(int1 side, BYTE data);
BYTE glcd_readByte(int1 side);
void glcd_update();
# 129 "HDM64GS12.c"
void glcd_init(int1 mode)
{

   output_high(PIN_C0);
   output_low(PIN_B5);
   output_low(PIN_B0);
   output_low(PIN_B1);

   output_low(PIN_B2);
   glcd_writeByte(0, 0xC0);
   glcd_writeByte(1, 0xC0);
   glcd_writeByte(0, 0x40);
   glcd_writeByte(1, 0x40);
   glcd_writeByte(0, 0xB8);
   glcd_writeByte(1, 0xB8);

   if(mode == 1)
   {
      glcd_writeByte(0, 0x3F);
      glcd_writeByte(1, 0x3F);
   }
   else
   {
      glcd_writeByte(0, 0x3E);
      glcd_writeByte(1, 0x3E);
   }

   glcd_fillScreen(0);




}
# 199 "HDM64GS12.c"
void glcd_pixel(unsigned int8 x, unsigned int8 y, int1 color)
# 227 "HDM64GS12.c"
{
   BYTE data;
   int1 side = 0;

   if(x > 63)
   {
      x -= 64;
      side = 1;
   }

   output_low(PIN_B2);
   bit_clear(x,7);
   bit_set(x,6);
   glcd_writeByte(side, x);
   glcd_writeByte(side, (y/8 & 0xBF) | 0xB8);
   output_high(PIN_B2);
   glcd_readByte(side);
   data = glcd_readByte(side);

   if(color == 1)
      bit_set(data, y%8);
   else
      bit_clear(data, y%8);

   output_low(PIN_B2);
   glcd_writeByte(side, x);
   output_high(PIN_B2);
   glcd_writeByte(side, data);
}






void glcd_fillScreen(int1 color)
# 280 "HDM64GS12.c"
{
   unsigned int8 i, j;


   for(i = 0; i < 8; ++i)
   {
      output_low(PIN_B2);
      glcd_writeByte(0, 0b01000000);
      glcd_writeByte(1, 0b01000000);
      glcd_writeByte(0, i | 0b10111000);
      glcd_writeByte(1, i | 0b10111000);
      output_high(PIN_B2);


      for(j = 0; j < 64; ++j)
      {
         glcd_writeByte(0, 0xFF*color);
         glcd_writeByte(1, 0xFF*color);
      }
   }
}






void glcd_writeByte(int1 side, BYTE data)
{
   set_tris_d(0x00);

   output_low(PIN_B4);

    if(side)
      output_high(PIN_B1);
   else
      output_high(PIN_B0);

    delay_us(1);

   output_d(data);
   delay_us(1);
   output_high(PIN_B5);
   delay_us(1);
   output_low(PIN_B5);

   output_low(PIN_B0);
   output_low(PIN_B1);
}




BYTE glcd_readByte(int1 side)
{
   BYTE data;

   set_tris_d(0xFF);
   output_high(PIN_B4);

   if(side)
      output_high(PIN_B1);
   else
      output_high(PIN_B0);

   delay_us(1);
   output_high(PIN_B5);
   delay_us(1);
   data = input_d();
   output_low(PIN_B5);

   output_low(PIN_B0);
   output_low(PIN_B1);
   return data;
}
