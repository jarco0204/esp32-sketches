#include <Arduino.h>
#include <WiFiClientSecure.h>

// Network information
const char *WIFI_SSID = "ElPepe";
const char WIFI_PASSWORD[] = "123456789";

const char *AWS_DEVICE_NAME = "P0-08";
const char *AWS_IOT_PUBLISH_MAIN_TOPIC = "kelseys/1";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUOuu1DkgpxMhf2tBbFCj9a/fgxlEwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMTExNjE4MzEw
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANMRhtBj1JvHxIIPwsRw
CZq7jjUlnbz66eAb/Aj5/zd6TBFcEPJ/1J0TiXlc4+K+6rJgZo5jeXTprXPvJ4W2
bqGxpSQ7/62VEYKXSxWrl0UhGmaDCWlCiLmipZYJ6rtoTy51cSnF1QpumRdunmmr
jNG0pXcuJN8ZQ3p9EZ5OClRC48sU8YjvzldYbkgYhDXY6O5f5BLzFwgrtMsBkq9G
spDM9aFKdK+TKuykMHgY+yw0KDo8+LTVzAtsuOfaL0wq9TpdkuC31M9MdAXi5sjC
1lZRxKoEKYp+sJojatUvE41CmwCOV2gViVkfK8cmLg9hi/7SRqYuuAb+IIDcWIF5
gV0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUZdM4EDIi7zblY4ye/nQkuoZp1+swHQYD
VR0OBBYEFCB/JmNOVVu0HVrxZJ67mXYIIxRzMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQA8pi1YNvWRwXrvSk7I6ASnpjo5
d6Fee76tfaOFx+FtRW+3LauIl2W9gNKzx4vZJT/XjOyhSpSZxNYL8baoNrawxXkd
C+e1/bI3a/HWuD1XNoQFs3KQg7RoF+kyPbUZslglvgf0WH5LgFbpmGmmSKCckzDs
DMYtNKMS6ijLSwQNUN7oayRuR7GMvsYnGiArqL4GyGRZD3zYPmCEibUwm0h14kk/
nnv7CO+SWgDji8bbS2Gsw9SinYk8Y17dzWWX4dvwq5g+pMAxsLT3wZklTMOV5VV4
2//aP8+5llLcOFBKpL2nbmdTKjpO+uWhQnFQyf3Ebv7jQ9Bfd8b2manxMQYA
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA0xGG0GPUm8fEgg/CxHAJmruONSWdvPrp4Bv8CPn/N3pMEVwQ
8n/UnROJeVzj4r7qsmBmjmN5dOmtc+8nhbZuobGlJDv/rZURgpdLFauXRSEaZoMJ
aUKIuaKllgnqu2hPLnVxKcXVCm6ZF26eaauM0bSldy4k3xlDen0Rnk4KVELjyxTx
iO/OV1huSBiENdjo7l/kEvMXCCu0ywGSr0aykMz1oUp0r5Mq7KQweBj7LDQoOjz4
tNXMC2y459ovTCr1Ol2S4LfUz0x0BeLmyMLWVlHEqgQpin6wmiNq1S8TjUKbAI5X
aBWJWR8rxyYuD2GL/tJGpi64Bv4ggNxYgXmBXQIDAQABAoIBACna2+UPJuYNY0Z3
YaQao4zvjG+1meYTy7UJV51gFeAmfFck7qdn+f18FlLevMIGIfxlaXDrAPbSL/dB
gZDecGnYFGtvCnCdku9WmSke8cxfmCCj84XQD9TgD/4E0WuAb9NCrsId3PVIoUso
QHmr7yzWqclMeTtUgzETBB9kUmAK13aJB/tsetOlSzOpD85yluVUy7FXTjA2fb+8
cVZWcF6219FPFGwlpwvAmMC6gsbx51mDo3p/9n2Cg/GrBFBYhYGaChwheY2D37nw
0BCfrbcm58vpnAc6JM/iMgUGezu4o3Ws5Jd2VqWfm8oY65U7TqeTWvNt8P4wNZ9v
xA6OBAECgYEA6zTFFAK5W9UI9+txOjnY8hxYeiTLuojh0lsIw3hWTWdMirUOSWU0
0MGXxl+t7zL2p/NCiV6M7JXczxxo3J82Ru/XGfV0d/R23cgUdX9/S7B5d8UJP36w
pJJd33WelyhrE34mX0zYRsdJV+5j751DYgekIgMcIIe2XZQkl/xLDp0CgYEA5bp3
6+88O9Lp65Z7kbIztHpWgJSqApp1f1mhU9kZtVnldbbp3+Z/EUGrVLdb7ZwDnxjT
QUePsE/M3urm2AG4mo0JMkb8ptyeVAToKyG27Y6Cg9Y30qDCszCvXbA9MjSznZrT
PcC6BwCnO5WIsQxCfW4KPghN/mYfG5MGAQGiYcECgYB/8cqq4fh2reIdCqQjKkiL
G0Q93To9lCxTQ1X3DsTTgjeu1GKbJkSeslCfhniVP8HRBpDRLO+32qWsvCWGrczW
tazq4AylHAR1DFuktuBGCg9pSXOIaKAqRbxuzAqi60ej6OFywdh8V/fbr0AJniCD
eOid39czEt/u86EcVA94OQKBgFcLUGUVQL1qLTgWzLtnyi+YKy6NJd3WqK6YR4h9
uj/8z/TKWdN8gqkHOOPglfRyKO0feuxBMTCdDANq/gzamhf1WIa0XwbakSNEuLOf
ErDwToOlp4EnaJ580/Wr0YQ0c9pDz8aWP+ZnWPY6spBjFM5x42+dte+uwhqwmpXH
CMABAoGBAN7BDULqc9vI41lj4aCAmoOw68NEFPDGoO5HkvG4MkXki9Jp6G5biZwG
7FYeKqkQcRR4gSVIODd/R+1J3zb2rvR1f7NfgN9ispQV6XldNmd4ogKqrGxKl7XW
WJIQ+XugbL34nJ8hlRyhGiESA1HFKbKwaHQhkV1TOmFX3p2+8BtX
-----END RSA PRIVATE KEY-----
)KEY";

/*
    Connect to WIFI for first time
*/
void connectToWifi()
{
    char wifiPrompt[32] = {0};
    uint32_t connect_attempt = 0;
    sprintf(wifiPrompt, "Connecting to %s", WIFI_SSID);
    Serial.println("Hello");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        sprintf(wifiPrompt, "Connection to WiFi (%d)", connect_attempt++);
        Serial.println(wifiPrompt);
    }

    sprintf(wifiPrompt, "Connected to WiFi!");
    Serial.println(wifiPrompt);
}

/*
    Setup function
*/
void setup()
{
    // Serial Baud Rate
    Serial.begin(115200);
    Serial.println("Starting Setup...");

    connectToWifi();
}

// Take measurements of the Wi-Fi strength and return the average result.
int getStrength()
{
    long rssi = 0;
    long averageRSSI = 0;
    int points = 7;

    for (int i = 0; i < points; i++)
    {
        rssi += WiFi.RSSI();
        delay(20);
    }

    averageRSSI = rssi / points;
    return averageRSSI;
}

/*
    loop function
*/
void loop()
{
    delay(1000);
    Serial.println("Done");
    int strengthWifi = getStrength();
    char strengthPrompt[32] = {0};
    sprintf(strengthPrompt, "Strength of Signal: (%d)", strengthWifi);
    Serial.println(strengthPrompt);
}