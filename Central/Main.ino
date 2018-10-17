// Public Libraries...
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Private Libraries...
#include <Data.h>
#include <Console.h>

#define TIMEOUT 5000

RF24 wireless(7, 8); // CE, CSN

Console console;
Data data;
typedef struct Data dataType;

// Connection information...
char* ssid = "Casa 03";
char* password = "8532785419";
char* host = "www.vize.com.br";

// URLs...
String base_url = "/php/";
String request_url = base_url + "request.php?value=";
String save_url = base_url + "save.php?";

const byte address[6] = "00001";

String fct1 = "WiFi";
String fct2 = "Wireless";

void configConsole()
{
    console.setName("Automatic House");
    console.setShortName("A.H.");
    console.setBound(115200);
    console.changeDebug();
    console.setVersion(1);
    console.setBuild(0);
    console.addFunction(fct1);
    console.addFunction(fct2);
    console.start();
}

void initialization()
{
    // Console configuration...
    configConsole();

    // Modules initialization...
    WiFi.begin(ssid, password);
    if (wireless.begin())
    {
        console.changeFunctionState(fct2);

        // Wireless configuration...
        wireless.openWritingPipe(address);
        wireless.setPALevel(RF24_PA_MIN);
        wireless.stopListening();
    }

    // WiFi configuration...
    int max_time = millis() + TIMEOUT;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (max_time <= millis()) ESP.restart();
        console.sendMessage(".");
        delay(500);
    }

    console.changeFunctionState(fct1);
    console.sendMain();
}

bool sendValues()
{
    // Check miss values...
    if (data.miss()) return false;

    // Send data values...
    int max_time = millis() + TIMEOUT;
    while (!wireless.write(&data, sizeof(dataType)))
    {
        // Timeout...
        if (max_time <= millis()) return false;
    }

    return false;
}

String sendData(String url)
{
    // Variables...
    WiFiClient client;
    String message;
    String error_prefix = "Falha em conexao: ";

    // Check host connection...
    if (!client.connect(host, 80))
    {
        console.sendMessage(error_prefix + "Impossivel conectar com HOST.");
        return "Erro 01";
    }

    // Client content...
    client.print(
        String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "Connection: close\r\n\r\n"
    );
    
    delay(10);

    // Waiting for client response...
    unsigned long time = millis();
    while (client.available() == 0)
    {
        // Timeout...
        if (millis() - time > TIMEOUT)
        {
            console.sendMessage(error_prefix + "Timeout");
            client.stop();
            return "Erro 02";
        }
    }

    // Check IP...
    if (console.inDebug()) console.sendMessage(String("IP: " + WiFi.localIP()));

    // Saving response from client...
    while (client.available())
    {
        message = client.readStringUntil('\r');
    }

    return message;
}

void updateData()
{
    data.temperature = sendData(request_url + "0").toInt();
    data.input = sendData(request_url + "1").toInt();
}

String formatData(String name[], String value[])
{
    String format;
    String add = "&";
    for (int i = 0; i < sizeof(name) / sizeof(name[0]); i++)
    {
        if (i + 1 > sizeof(name))
        {
            add = "";
        }
        format += name[i] + "=" + value[i] + add;
    }
    return format;
}

void setup()
{
    // Initialization system...
    initialization();

    // Update data values...
    updateData();
}

void loop()
{
    // Check if another client change any value of room...
    if (sendData(request_url + "-1") == "true")
    {
        updateData();
        return;
    }

    // Send command to module and update database...
    if (console.inDebug())
    {
        sendValues();
        String names[3] = { "send", "temp", "input" };
        String values[3] = { "1", String(data.temperature), String(data.input) };
        sendData(save_url + formatData(names, values));
    }
}
