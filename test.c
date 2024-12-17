sbit LCD_RS at RC4_bit;
sbit LCD_EN at RC5_bit;
sbit LCD_D4 at RC0_bit;
sbit LCD_D5 at RC1_bit;
sbit LCD_D6 at RC2_bit;
sbit LCD_D7 at RC3_bit;
sbit LCD_RS_Direction at TRISC4_bit;
sbit LCD_EN_Direction at TRISC5_bit;
sbit LCD_D4_Direction at TRISC0_bit;
sbit LCD_D5_Direction at TRISC1_bit;
sbit LCD_D6_Direction at TRISC2_bit;
sbit LCD_D7_Direction at TRISC3_bit;
char txt1[] = "Temperature :";
char txt2[] ="Bienvenue";
char txt3[16];
char i;
int NB;
int x;
int bpm;
int y;
float fre,bpmf,disv,disf;
float dis;
int distance=0;
int zz;
int nbtime=77;
unsigned short test;
void interrupt() {
    if (intcon.INTF == 1) {
        NB++;
        PORTD = 0b00000100;
        Delay_ms(1000);
        x = 1;
        intcon.INTF = 0;
    }
    if (intcon.RBIF == 1) {
        if (PORTB == 0b00100000) {
            PORTD = 0b00000010;
            Delay_ms(2000);
            y = 1;
        }
        if (PORTB == 0b01000000) {
            zz = 1;
        }
        if (PORTB == 0b00010000) {
            PORTD=0x01;
            intcon.T0IE=1;
        }
        intcon.RBIF = 0;


    }
    if(intcon.T0IF==1){
         nbtime--;
         if(nbtime==0){
             PORTD = 0b00000000;
             intcon.T0IE=0;
             nbtime=77;
         }
         intcon.T0IF=0;
    }



}
void main() {
  float temperature;
  char temperature_str[16];
  intcon.GIE=1;
  intcon.INTE=1;
  intcon.RBIE=1;
  option_reg.T0CS=0;
  option_reg.PSA=0;
  option_reg.PS0=1;
  option_reg.PS1=1;
  option_reg.PS2=1;
  option_reg.INTEDG=1;
  ADC_Init();
  Lcd_Init();
  TRISB=0xFF;
  TRISD = 0b01111000;
  TRISA = 0b111111;
  while(1) {
    int adc_value = ADC_Get_Sample(0);
    float voltage = (adc_value * 5.0) / 1023.0 * 1000;
    temperature = voltage / 10.0;
    FloatToStr(temperature, temperature_str);
    Lcd_Out(1, 1, txt1);
    Lcd_Out(2, 1, temperature_str);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    if (temperature > 28) {
      portD = 0b10000100;
    } else {
      portD = 0b00000001;
    }
    Delay_ms(500);
    if(x==1){
      EEPROM_Write(0x00+NB,NB);
      test=EEPROM_Read(NB);
      ShortToStr(test,txt3);
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1,1,txt2);
      Lcd_Out(2,1,"MEMBER:");
      Lcd_Out(2,9,txt3);
      Delay_ms(3000);
      Lcd_Cmd(_LCD_CLEAR);
      x=0;
  }
    if(y==1){
       bpm = ADC_Get_Sample(1);
       fre = (bpm * 5.0) / 1023.0 * 1000 ;
       bpmf = fre / 10.0;
       if(bpmf>100){
               PORTD=0b0000011;
               Delay_ms(1000);
               PORTD=0b0000010;
               Delay_ms(1000);
               PORTD=0b0000011;
               Delay_ms(1000);
               PORTD=0b0000010;
               Delay_ms(1000);
               PORTD=0b0000011;
               y=0;
         }
    }

    if(zz==1){
              distance = ADC_Get_Sample(7) ;
              dis = (distance * 5.0) / 1023.0 * 1000 ;
              disf = dis / 10.0 ;
              if(disf > 0){
              for(i=0 ; i<5 ; i++){
                PORTD=0b00000100;
                Delay_ms(1000);
                PORTD=0b00000000;
                Delay_ms(1000);
              }
              }
              Lcd_Cmd(_LCD_CLEAR);
              Lcd_Out(1,1,"BRAVO");
              Delay_ms(3000);
              zz=0;
    }
  }
}