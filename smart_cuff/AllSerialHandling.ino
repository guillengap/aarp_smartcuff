void serialOutput(){   // Decide How To Output Serial.
  switch(outputType){
    case PROCESSING_VISUALIZER:
      sendDataToSerial('S', Signal);     // goes to sendDataToSerial function
      break;
    case SERIAL_PLOTTER:  // open the Arduino Serial Plotter to visualize these data
      Serial.print(BPM);
      Serial.print(",");
      Serial.print(IBI);
      Serial.print(",");
      Serial.println(Signal);  
///////////////////////////////////////////////////// TFT output
      sensors.requestTemperatures();   //The command is sent to read the temperature
      float temp= sensors.getTempCByIndex(0); //The temperature is obtained in ºC
      tft.setCursor(0, 5);
      tft.setTextSize(3);   
      tft.setTextColor(MAGENTA, BLACK);   
      tft.println(" SMART CUFF");
      tft.println(" ");   
      tft.setTextColor(YELLOW, BLACK);
      tft.println("BPM " + String(BPM) + "  "); //Print the pulses per second of the pulse sensor
      tft.println(" ");
      tft.println("TEMP " + String(temp,1) + " C  "); //Print DS18B20 sensor temperature
      tft.println(" ");
      delay(5);
      break;
    default:
      break;
  }

}

//  Decides How To OutPut BPM and IBI Data
void serialOutputWhenBeatHappens(){
  switch(outputType){
    case PROCESSING_VISUALIZER:    // find it here https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
      sendDataToSerial('B',BPM);   // send heart rate with a 'B' prefix
      sendDataToSerial('Q',IBI);   // send time between beats with a 'Q' prefix
   
      break;
    default:
      break;
  }
}

//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
void sendDataToSerial(char symbol, int data ){
    Serial.print(symbol);
    Serial.println(data);
  }
