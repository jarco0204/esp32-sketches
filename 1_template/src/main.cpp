#include <Arduino.h>

/*
    Setup function
*/
void setup()
{
    // Serial Baud Rate
    Serial.begin(115200);
    Serial.println("Starting Setup...");
}

/*
    loop function
*/
void loop()
{
    delay(1000);
    Serial.println("Done");
}