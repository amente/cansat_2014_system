#define XBEE_RESET_PIN  4
#define MAX_CMD_LEN     20
usb_serial_class &Term = Serial;
HardwareSerial &Xbee = Serial1;

void setup() 
{
  Term.begin(9600);
  Xbee.begin(9600);

  pinMode(XBEE_RESET_PIN, INPUT);
}

void loop() 
{ 
  serialEvent();
}

String cmd = "";
unsigned long last;
void serialEvent()
{
  if (Term.available())
  {
    char temp = Term.read();
    if ( (temp == ':' || cmd.length() ) && (millis() - last > 5) )  // also checks for xmodem; threshold = 5ms
    {
      Term.write(temp);
      if (temp != '\r')
      {
        if (cmd.length() <= MAX_CMD_LEN)  // check for buffer overflow
        {
          cmd.append(temp);
        }
        else
        {
          Term.print("\n: Max cmd len of ");
          Term.print(MAX_CMD_LEN);
          Term.println(" reached!");
          cmd = "";
        }
      }
      else
      {
        parseCmd(cmd.substring(1)); // trim the leading ':'
        cmd = "";
      }       
    }    
    else
    {  
      Xbee.write(temp);
    }
    last = millis();    
  }

  if (Xbee.available())
    Term.write(Xbee.read());  
}

void parseCmd(String cmd)
{
  cmd.toLowerCase();
  
  if (cmd == "q!")
    rebootXbee(false);
  else if (cmd == "wq")
    rebootXbee(true);
  
  // unmatched command
  else
  {
    Term.print(":");
    Term.print(cmd);
    Term.println(" - Invalid Command"); 
  }
}

void rebootXbee(char bootloader)
{
  // To put the Xbee into bootloader mode, /DTR(9), DIN(2) = LOW and /RTS(16) = HIGH upon boot up.  
  if (bootloader) UART0_C2 |= UART_C2_SBK; // assert seral break
  pinMode(XBEE_RESET_PIN, OUTPUT);  
  digitalWriteFast(XBEE_RESET_PIN, LOW);  
  pinMode(XBEE_RESET_PIN, INPUT);
  delay(5); // wait a bit to let the Xbee boot up
  UART0_C2 &= ~UART_C2_SBK; // de-assert serial break    
  Xbee.write(10); // the bootloader has no prompt so send a \n to bring up the menu
}


