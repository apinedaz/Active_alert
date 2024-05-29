#include <Wire.h>

#include <PN532_I2C.h>

#include <PN532.h>

#include <NfcAdapter.h>


# define LED  D3

PN532_I2C pn532_i2c (Wire) ;

NfcAdapter nfc = NfcAdapter (pn532_i2c);


String tag_on = "03 96 DE B6";

String tag_off = "53 B7 DA 0E";

void setup (void) {

  pinMode(LED, OUTPUT); 
  digitalWrite(LED, LOW);
  Serial.begin (9600);
  nfc.begin ();
}

void loop () {
  
  if (nfc.tagPresent ())
  {
    NfcTag tag = nfc.read ();
    if(tag.getUidString() == tag_on){
      digitalWrite(LED, HIGH);
    }
    if(tag.getUidString() == tag_off){
      digitalWrite(LED, LOW);
    }
  }
}

