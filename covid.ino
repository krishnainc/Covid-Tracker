#include <ESP8266WiFi.h>        //Use ESP8266 functions                                              
 #include <ESP8266HTTPClient.h>
 #include <LiquidCrystal_I2C.h>
 
 #include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier

  LiquidCrystal_I2C lcd(0x27, 20, 4);
  
  const char* ssid = "Anonymous@unifi";              //WIFI SSID Name                             
  const char* password = "NBV1190WAJA";        //WIFI Password
  const char* host = "api.thingspeak.com";  //We read the data from this host                                   
  const int httpPortRead = 80;                                           
                                                  
  const char* url1 = "/apps/thinghttp/send_request?api_key=GGSXGM6FH0QLVKHU";     //Change this URL Cases
  const char* url2 = "/apps/thinghttp/send_request?api_key=G8OO9V4Q83N3YMZZ";   //Deaths
  const char* url3 = "/apps/thinghttp/send_request?api_key=GSKGR2JVCRZFFBHT";   //Recovered
  int To_remove;      //There are some irrelevant data on the string and here's how I keep the index
                      //of those characters 
  String Cases,Deaths,Recovered,Data_Raw,Data_Raw_1,Data_Raw_2,Data_Raw_3;  //Here I keep the numbers that I got

  WiFiClient client;      //Create a WiFi client and http client                                                     
  HTTPClient http; 

  
  void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Covid-19 Update Malaysia");
  delay(1000);
  lcd.clear();
    
    Serial.begin(115200);
    WiFi.disconnect();             //Disconnect and reconnect to the Wifi you set                                                 
    delay(1000);                                                                  
    WiFi.begin(ssid, password);                                                   
    Serial.println("Connected to the WiFi network");   //Display feedback on the serial monitor                                        
    Serial.println(WiFi.localIP());
  }

void loop() {
  

   //Reading 1: Reading of cases
    if( http.begin(host,httpPortRead,url1))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw = http.getString();   //Here we store the raw data string
            
              
              Data_Raw_1 = Data_Raw;                                    
              To_remove = Data_Raw_1.indexOf(">");                      //I look for the position of this symbol ">"                                 
              Data_Raw_1.remove(0,To_remove+1);                        //I remove it and everything that's before
              To_remove = Data_Raw_1.indexOf("<");                     //I look for the position of this symbol ">"                                
              Data_Raw_1.remove(To_remove,Data_Raw_1.length());          //I remove it and everything that's after
                                                                     //Example: This is the raw data received <td style="font-weight: bold; text-align:right">63,927</td>
                                                                     //First we look for ">" and we remove everything before it including it
                                                                     //We stay with this 63,927</td>
                                                                     //We look for "<" symbol and we remove it + everything after
                                                                     //We keep only this 63,927 as string
              Cases=Data_Raw_1;
              Serial.print("Cases: ");  //I choosed to display it on the serial monitor to help you debug
              Serial.println(Cases); 
              
              Data_Raw_2=Data_Raw;
              To_remove = Data_Raw_2.indexOf("<span>");
              Data_Raw_2.remove(0,To_remove+6);
              Data_Raw_3=Data_Raw_2;
              To_remove = Data_Raw_2.indexOf("</span>");
              Data_Raw_2.remove(To_remove,Data_Raw_2.length());

              Deaths=Data_Raw_2;
              Serial.print("Deaths: ");
              Serial.println(Deaths);
            
              To_remove = Data_Raw_3.indexOf("<span>");
              Data_Raw_3.remove(0,To_remove+6);

              To_remove = Data_Raw_3.indexOf("<");
              Data_Raw_3.remove(To_remove,Data_Raw_3.length());

              Recovered=Data_Raw_3;
              
              Serial.print("Recovered: ");
              Serial.println(Recovered);    
                                                                                       
            }
        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        
        http.end();                                                                 
      } 
    else //If we can't connect to the HTTP
      {
        Serial.printf("[HTTP} Unable to connect\n");
      }

    while (WiFi.status() != WL_CONNECTED)     //In case the Wifi connexion is lost                                    
      { 
        
        WiFi.disconnect();                                                        
        delay(1000);                                                             
        
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       
        delay(10000);                                                             
      }
        lcd.clear();
        
      
        lcd.setCursor(0, 0);
        lcd.print("Cases : ");
        lcd.print(Cases);

        lcd.setCursor(0, 1);
        lcd.print("D : ");
        lcd.print(Deaths);
        
        lcd.setCursor(20, 0);
        lcd.print("R : ");
        lcd.print(Recovered);

        
  for (int positionCounter = 0; positionCounter < 80; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft(); 
    // wait a bit:
    delay(500);


    }
}
