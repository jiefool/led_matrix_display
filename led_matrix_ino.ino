#include "CMAP.h"
#include "wild_dust_cmap.h"
#define INTEN 3000

char Display_Ram[70];
char LED_DATA[]  = "Rugged and Sports Tailoring  ~";
char SERVICES[]  = "  Accepts:   ";
char SCHOOLS[]   = "  Schools    ";
char SPORTS[]    = "  Sports     ";
char OFFICES[]   = "  Office     ";
char UNIFORMS[]  = " Uniforms    ";
char PRINTJOBS[] = "  Prints     ";
char REPAIRS[]   = "  Repairs    ";
char MORE[]      = "  & more!!   ";



int d;
int mask;
int char_pointer;
int shift_counter;
int count;
int CLK  = 5;
int DAT = 6;
int STR = 7;
int DATA_PINS[] = {A0, A1, A2, A3, 8, 9, 10, 11};

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(DAT, OUTPUT);
  pinMode(STR, OUTPUT);
  for(int i=0; i<8; i++){
    pinMode(DATA_PINS[i], OUTPUT);
  }
}

void loop(){

  //WILD DUST static display
  for(int i=0; i<150; i++){
    wild_dust_display();
  }

  //WILD DUST  center out display
  for(int i=0; i<50; i++){
      center_out_shift();
      for(int y=0; y<2; y++)
        Display();
  }

  //clears the display
  /*
  for(int i=0; i<20; i++){
    Clear_Display_Ram();
  }*/

  //running text from right to left
  for(int i=0; i<168; i++){
     sports_and_rugged_running_display();
  }

  //clears the display
  for(int i=0; i<10; i++){
    Clear_Display_Ram();
  }

  //reset the char pointer of the running text

  //display services data
  load_char_data(SERVICES);
  display_loop();
  load_char_data(SCHOOLS);
  display_loop();
  load_char_data(SPORTS);
  display_loop();
  load_char_data(OFFICES);
  display_loop();
  load_char_data(UNIFORMS);
  display_loop();
  load_char_data(PRINTJOBS);
  display_loop();
  load_char_data(REPAIRS);
  display_loop();
  load_char_data(MORE);
  display_loop();

  //inquire inside display with blink
  for(int j=0; j<5; j++){
    for(int i=0; i<20; i++){
      inquire_inside_display();
    }

    for(int i=0; i<20; i++){
    Clear_Display_Ram();
    }
  }

  char_pointer = 0;
}

void display_loop(){
  int x = 0;
  for(int j=0; j<25; j++){
    for(int y = 0; y<5; y++){
      Display();
    }
    if (x > 8)
      shift_bit_up();
    x++;
  }
}


void load_char_data(char data_char[]){
    int x=0;
    for(int i=0; i<64; i++){
      for( int j=0; j<6; j++){
        Display_Ram[x] = character_data[(data_char[i]- 0x20)][j];
        x++;
      }
    }
}

void shift_bit_up(){
  int x=0;
    for(int i=0; i<64; i++){
      for( int j=0; j<6; j++){
        int a = Display_Ram[x];
        a = Display_Ram[x] << 1;
        Display_Ram[x] = a;
        x++;
      }
    }
}


void wild_dust_display(){
  Display();
  for(int i=0; i<64; i++){
    Display_Ram[i] = wild_dust_char_data[i];
  }
}

void inquire_inside_display(){
  Display();
  for(int i=0; i<64; i++){
    Display_Ram[i] = inquire_inside_char_data[i];
  }
}

void center_out_shift(){
  for(int i=0; i<30; i++){
    Display_Ram[i] = Display_Ram[i+1];
  }
  for(int i=0; i<34; i++){
    Display_Ram[64-i] = Display_Ram[64-(i+1)];
  }
}

void sports_and_rugged_running_display(){
  for(int i=0; i<2; i++){
    Display();
  }
  shift();
}

void shift(void)
{
  int y = 0;
  for(int i=0; i<70; i++){
       y=i+1;
    Display_Ram[i] = Display_Ram[y];
  }

  shift_counter++;
  if(shift_counter == 6){
    shift_counter = 0;
    load();
  }
}

void Display(void)
{
  mask = 0x01;
  set_up_pin(7);

  mask = 0x02;
  set_up_pin(6);

  mask = 0x04;
  set_up_pin(5);

  mask = 0x08;
  set_up_pin(4);

  mask = 0x10;
  set_up_pin(3);

  mask = 0x20;
  set_up_pin(2);

  mask = 0x40;
  set_up_pin(1);

  mask = 0x80;
  set_up_pin(0);
}

void set_up_pin(int pin){
  scan();
  digitalWrite(DATA_PINS[pin],LOW);
  for(d=0;d<INTEN;d++){}
  digitalWrite(DATA_PINS[pin],HIGH);
}

void load(void){
  if(LED_DATA[char_pointer+1] == '~')
    char_pointer = 0;
  for(int i=0; i<5; i++){
    Display_Ram[64 + i] = character_data[(LED_DATA[char_pointer] - 0x20)][i];
  }
  Display_Ram[69] = 00;
  char_pointer++;
}

void Clear_Display_Ram(void){
  Display();
  for(int i = 0; i<70; i++) { Display_Ram[i] = 0x00; }
}

void scan(void){
  for (count=64;count>(-1);count--){
    (Display_Ram[count] & mask) == mask ? digitalWrite(DAT,LOW) : digitalWrite(DAT,HIGH);
    clock();
  }
  strobe();
}

void clock(void){
  on_off_pin(CLK);
}

void strobe(void){
  on_off_pin(STR);
}

void on_off_pin(int pin){
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}
