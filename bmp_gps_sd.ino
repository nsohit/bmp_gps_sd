#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h> 
File myFile;
TinyGPS gps;
SoftwareSerial ss(5,6);//rx,tx


/*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 bmp;
  
void setup() {
   Serial.begin(9600);
   ss.begin(9600);
   if (!SD.begin(10)) { //tergantung di pin chipselect yang digunakan
    Serial.println("Gagal baca microSD!");
    return;
  }
  Serial.println("Sukses baca kartu microSD!");

  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}
  
void loop() {
    delay(500000);
   myFile = SD.open("RISET3.txt", FILE_WRITE); //menulis File coba.txt
   
   
   if (myFile) {
    while(1){
   bool newData = false;
   unsigned long chars;
   unsigned short sentences, failed;
   
   for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // hilangkan koment jika mau melihat data yang dikirim dari modul GPS
      if (gps.encode(c)) // Parsing semua data
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    myFile.print("LAT=");
    myFile.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print("LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    myFile.print("LON=");
    myFile.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print("SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    myFile.print("SAT=");
    myFile.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print("PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    myFile.print("PREC=");
    myFile.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
   // delay(50000);
    
  }
  
  gps.stats(&chars, &sentences, &failed);
 /* Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
*/  //Serial.println("** Tidak ada Data Masuk, Periksa Wiring **");


    
  //  Serial.println("mencoba nulis data di file RISET.txt");
    //myFile.println("Temperature = "+bmp.readTemperature());
    // tutup file
    //myFile.close();
   // Serial.println("SELESAI!");
    
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.print("*C ");
   // delay(100); 
    myFile.print("Temperature=");
    myFile.print(bmp.readTemperature());
    myFile.print("*C ");
  //  myFile.close();
   // delay(100);
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println("Pa ");
    delay(100);
    myFile.print("Pressure = ");
    myFile.print(bmp.readPressure());
    myFile.print("Pa ");
    //myFile.close();
   // delay(100);
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.print("meters ");
   // delay(100);
    myFile.print("Altitude = ");
    myFile.print(bmp.readAltitude());
    myFile.print("meters ");
    //myFile.close();
    //delay(100);

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.print("Pa ");
    delay(100);
    myFile.print("Pressure at sealevel (calculated) = ");
    myFile.print(bmp.readSealevelPressure());
    myFile.print("Pa ");
    //myFile.close();
    //delay(100);
//    String data[] =bmp.readTemperature();

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.print("meters ");
    //delay(100);
    myFile.print("Real altitude = ");
    myFile.print(bmp.readAltitude(101500));
    myFile.print("meters ");
    //myFile.close();
       myFile.close();
    Serial.println("SELESAI!");
    return;

 /*   
    if (myFile) {
    Serial.println("mencoba nulis data di file RISET.txt");
    //myFile.println("Temperature = "+bmp.readTemperature());
    // tutup file
    myFile.close();
    Serial.println("SELESAI!");
  } else {
    // jika gagal print error
    Serial.println("GAGAL TULIS RISET.txt");
  }

    Serial.println();
    delay(500);*/
}

}
    else {
    // jika gagal print error
    Serial.println("GAGAL TULIS RISET.txt");
  }

}
