Function:

- Node, capable of communicating up and downlink streams of data to the TTN. 


Hardware:
- Atmega 328 (3.3v)
- RFM95 868 Mhz
- Nokia 5110 / PCD 8544 LCD screen
- 2 LED's


Pin Connections;

Arduino --> RFM 95
----------------------------------
- pin 2 (interrupt 0) -- > pin 14 (Dio0) 
- pin 3               -- > pin 15 (Dio1)
- pin 4               -- > pin 6 (Reset)

- pin 10 (SS)         -- > pin 5 (NSS)
- pin 11 (MOSI)       -- > pin 3 (MOSI)
- pin 12 (MISO)       -- > pin 2 (MISO)
- pin 13 (SCK)        -- > pin 4  (SCK)


Arduino --> Nokia 5110/PCD8544
------------------------------
- pin 5 --> pin 1 (Reset)
- pin 6 --> pin 2 (CS)
- Pin 7 --> pin 3 (DC)
- pin 8 --> pin 4 (Din / MOSI)
- pin 9 --> pin 5 (SCLK)

