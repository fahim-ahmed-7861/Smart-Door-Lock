#include <Keypad.h>
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
Servo myservo;


#define Password_Lenght 7 

int pos = 0;   

char Data[Password_Lenght]; 
char Master[Password_Lenght] = "123456";

char current[Password_Lenght];
byte data_count = 0, master_count = 0;
bool Pass_is_good;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
int c=0;

bool cpress=false;

int i=0;

const byte ROWS=4;
const byte COLS=4;
 
char keys[ROWS][COLS]={
 
  {'1','2','3','A'},
 
   {'4','5','6','B'},
 
   {'7','8','9','C'},
 
   {'*','0','#','D'},
 
 
};
 
byte rowPins[ROWS]={9,8,7,6};
byte colPins[COLS]={5,4,3,2};
bool door = true;

int e=0;

Keypad customKeypad   = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 


void instruction()
{
  lcd.clear();
  
   lcd.print("Assalamualaikum");

  delay(5000);

  lcd.clear();


lcd.setCursor(2,0);
  lcd.print("Instruction");

  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("To set or Change");

  lcd.setCursor(0,1);

 lcd.print("Password Press C");

  delay(5000);

  lcd.clear();

  lcd.setCursor(1,0);

   lcd.print("Enter Password");
}
void setup()
{
  Serial.begin(9600);
  myservo.attach(11);
  ServoClose();
  cpress=false;
  lcd.begin(16,2);

  instruction();

   
  c=0;
  e=0;

 
 

}

void loop()
{

  

     if (door == 0)
  {
    
   char customKey = customKeypad.getKey();

    if(customKey)Serial.println(customKey);

    if (customKey == '#')
    {
        
     
      ServoClose();

      lcd.clear();

        lcd.print("Door is Closed");

        delay(2000);

        lcd.clear();

  lcd.setCursor(1,0);

   lcd.print("Enter Password");
     
      door = 1;
    }
 
    
  }

  else {


    Open();
  }
   
 

}



void clearData()
{
  while (data_count != 0)
  {
    Data[data_count--] = 0; 
  }
   cpress=false;
   c=0;
  return;
}

void ServoOpen()
{
  for (pos = 180; pos >= 0; pos -= 5) { 
    // in steps of 1 degree
    myservo.write(pos);             
    delay(15);                    
  }
   cpress=false;
   c=0;
}

void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5) { 
    myservo.write(pos);             
    delay(5);                      
  }
  cpress=false;
  c=0;

  
}

int master_key()
{
  char str[7];

  
  
  
    for(int i=0; i<6;)
    {
      char customKey = customKeypad.getKey();

      if(customKey)
      {

         if(customKey=='B')
      {
        if(i>0)
        i--;

        lcd.clear();

           

             lcd.print("Enter Master Key");

        

     
      }

      else{
        
          str[i]= customKey;
          
         
        
           i++;
        
      }

lcd.setCursor(5,1);
       for(int c=0; c<i; c++)
    {
      lcd.print("*");
    }
     
      
    }
    }

    str[6]='\0';

  

    if(strcmp(str, Master)==0)
    return 1;

    else return 0;
}

void Open()
{
  char customKey = customKeypad.getKey();
  if (customKey) 
  {
   Serial.println(customKey);

   if(customKey=='A'){
    instruction();
   }
    else if(customKey == 'C')
    {

       lcd.clear();

        lcd.print("Enter Master Key");



         if(master_key())
         {
            c=0;
      cpress=true;
       Serial.println(0);
       e=0;


           lcd.clear();
           lcd.setCursor(1,0);

           lcd.print("Enter New Pass");
         }
         else
         {
              lcd.clear();

            lcd.print("Wrong Password");

              delay(2000);

              lcd.clear();

             lcd.setCursor(1,0);

             lcd.print("Enter Password");

            
        
         }
      
    
      
    }

    else if(cpress)
    {

      if(customKey=='B')
      {
        if(c>0)
        c--;

         lcd.clear();

             lcd.setCursor(1,0);

            lcd.print("Enter New Pass");

         lcd.setCursor(5,1);

         for(int i=0; i<c; i++)
    {
      lcd.print("*");
    }
        
      }

      else{
       current[c] = customKey; 


        EEPROM.write(c++, customKey);

        lcd.setCursor(5,1);
     
    for(int i=0; i<c; i++)
    {
      lcd.print("*");
    }

       

          if(c==6)
          {
            c=0;
            cpress=false;
            Serial.println(current);

            lcd.clear();

            lcd.setCursor(2,0);
            
            lcd.print("Successfully");

            
            lcd.setCursor(6,1);
            
            lcd.print("Done");

            delay(2000);

             lcd.clear();

             lcd.setCursor(1,0);

             lcd.print("Enter Password");
          }
  

         Serial.println(1);
      }
    }
  
    else {

      if(customKey=='B')
      {
        if(data_count>0)
        data_count--;

        lcd.clear();

             lcd.setCursor(1,0);

             lcd.print("Enter Password");

        

     
      }

      else{
        
      Data[data_count] = customKey; 

      // Serial.println(2);
   
    data_count++;
      }


 
    

     lcd.setCursor(5,1);
     
    for(int i=0; i<data_count; i++)
    {
      lcd.print("*");
    }
    
    
    }
  }

  if (data_count == Password_Lenght - 1) 
  {
    Serial.println(Master);
    Serial.println(Data);
  

   
     for(int i=0; i<6; i++)
     {
        current[i]=EEPROM.read(i);
     }

        Serial.println(current);

      
    
   
    if (!strcmp(Data, Master) || !strcmp(Data, current)) 
    {
       lcd.clear();

    lcd.print("Press # to close");
     
      ServoOpen();
      
      door = 0;
    }
    else
    {
      lcd.clear();

      lcd.print("Wrong Password");

      delay(2000);

      lcd.clear();

      lcd.print("Enter Password");
      
     
     
      door = 1;
    }
    clearData();
  }
}


