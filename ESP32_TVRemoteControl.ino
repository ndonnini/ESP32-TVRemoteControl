//Bibliotecas para acesso Wifi
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

//Bibliotecas para comunicação por infravermelho
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>


#define Rele1 33 //usando o rele 01 no IN01

#define LED_externo 27
#define LED_interno 2 // Led Interno do Esp3

bool Estado_Do_Rele1 = LOW;


// substitua os dados entre parênteses com o nome da Rede e a senha desejados
const char *ssid = "CASADOTOM";
const char *password = "123conectarPC";

WiFiServer server(80);

//Pino de saída para infravermelho
const uint16_t kIrLed = 4;  // Recommended: 4 (D2).


IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.



uint16_t bonoff[203] = {4512, 4492,  560, 1678,  560, 1676,  562, 1674,  560, 556,  562, 556,  
    560, 558,  560, 558,  558, 558,  562, 1676,  560, 1676,  562, 1676,  564, 554,  558, 558,  560, 
    558,  586, 532,  562, 558,  560, 558,  586, 1650,  560, 1674,  560, 560,  560, 558,  560, 1678,  
    588, 1648,  558, 1678,  560, 1676,  560, 560,  560, 556,  560, 1676,  560, 1676,  560, 558,  586, 
    532,  560, 556,  560, 46746,  4512, 4492,  560, 1676,  560, 1676,  586, 1650,  560, 558,  560, 558, 
    560, 558,  558, 560,  560, 558,  560, 1678,  560, 1676,  560, 1678,  560, 558,  560, 558,  558, 560,  
    562, 556,  558, 560,  560, 558,  560, 1678,  532, 1704,  558, 560,  560, 560,  560, 1676,  560, 1676,  
    560, 1678,  558, 1676,  560, 558,  560, 558,  560, 1676,  560, 1676,  560, 558,  586, 532,  560, 556, 
    588, 46724,  4514, 4492,  560, 1678,  586, 1650,  560, 1676,  558, 560,  548, 570,  534, 582,  562, 
    558,  586, 532,  560, 1676,  560, 1678,  558, 1678,  562, 556,  560, 560,  560, 558,  560, 556,  534, 
    584,  560, 558,  560, 1676,  560, 558,  534, 584,  560, 558,  560, 558,  560, 558,  560, 558,  560, 
    1676,  562, 556,  588, 1650,  558, 1678,  560, 1676,  560, 1678,  558, 1678,  558, 1676,  534};
uint16_t b123[75] = {4530, 4494,  560, 1676,  558, 1678,  560, 1676,  558, 560,  558, 560,  560, 558,  560, 556,  534, 586,  558, 1680,  532, 1704,  560, 1678,  558, 560,  560, 558,  558, 560,  558, 560,  558, 560,  558, 560,  558, 1678,  532, 586,  558, 560,  560, 1678,  558, 560,  532, 1704,  562, 1676,  560, 1676,  534, 584,  532, 1704,  564, 1672,  558, 560,  532, 1704,  558, 560,  558, 558,  532, 48564,  286, 980,  266, 432,  268, 706,  294}; 
uint16_t bcolors[67] = {4512, 4494,  560, 1676,  558, 1678,  558, 1678,  530, 588,  532, 586,  530, 588,  558, 560,  556, 562,  556, 1680,  530, 1708,  532, 1704,  558, 560,  558, 560,  558, 560,  530, 586,  558, 562,  530, 586,  530, 1706,  530, 1706,  530, 1706,  530, 588,  556, 562,  532, 1706,  530, 1706,  556, 1680,  558, 560,  558, 560,  532, 588,  556, 1680,  530, 1706,  556, 562,  530, 588,  556};
uint16_t bcenter[67] = {4570, 4438,  562, 1674,  640, 1596,  614, 1622,  588, 530,  560, 560,  588, 530,  562, 556,  616, 502,  590, 1650,  640, 1598,  612, 1624,  560, 560,  590, 528,  562, 556,  560, 558,  586, 532,  644, 476,  614, 504,  586, 532,  588, 1650,  614, 504,  588, 1648,  558, 1678,  586, 530,  560, 1678,  558, 1678,  590, 1648,  616, 502,  560, 1676,  640, 478,  588, 530,  562, 1676,  586};
uint16_t bup[67] = {4486, 4520,  558, 1680,  556, 1678,  560, 1676,  562, 556,  560, 558,  560, 558,  560, 558,  560, 558,  560, 1676,  560, 1678,  558, 1678,  560, 560,  562, 556,  560, 558,  532, 586,  560, 558,  560, 558,  534, 584,  560, 558,  534, 586,  560, 558,  558, 1680,  560, 1676,  558, 560,  560, 1678,  560, 1678,  558, 1678,  560, 1678,  532, 1704,  560, 558,  532, 586,  560, 1678,  560};
uint16_t bdown[67] = {4512, 4492,  584, 1652,  560, 1676,  560, 1678,  560, 558,  560, 558,  562, 558,  558, 560,  560, 560,  532, 1704,  558, 1678,  560, 1676,  532, 586,  560, 558,  560, 558,  558, 560,  560, 558,  560, 1676,  560, 558,  532, 586,  558, 560,  558, 560,  560, 1678,  534, 1702,  560, 558,  586, 532,  534, 1704,  560, 1676,  534, 1702,  530, 1704,  558, 562,  560, 558,  558, 1678,  532};
uint16_t bleft[67] = {4502, 4520,  560, 1678,  560, 1678,  532, 1704,  558, 560,  534, 584,  532, 586,  558, 560,  532, 586,  560, 1676,  532, 1704,  532, 1704,  532, 584,  560, 558,  558, 560,  558, 560,  560, 558,  558, 1678,  532, 586,  560, 1676,  558, 562,  532, 586,  556, 1680,  558, 1678,  560, 558,  558, 560,  558, 1678,  558, 560,  558, 1678,  558, 1678,  530, 586,  560, 558,  560, 1676,  560};
uint16_t bright[75] = {4484, 4522,  556, 1680,  532, 1706,  556, 1680,  530, 588,  556, 562,  532, 586,  558, 562,  556, 562,  556, 1680,  530, 1706,  556, 1680,  530, 588,  558, 560,  530, 588,  532, 588,  554, 562,  530, 588,  530, 1706,  530, 588,  530, 586,  532, 586,  530, 1708,  530, 1706,  530, 588,  532, 1704,  556, 562,  530, 1706,  530, 1706,  530, 1706,  532, 588,  530, 588,  530, 1706,  530, 48564,  258, 1010,  240, 458,  262, 710,  266};
uint16_t bback[67] = {4534, 4490,  562, 1676,  536, 1700,  560, 1676,  560, 556,  562, 556,  566, 552,  534, 584,  560, 558,  560, 1676,  562, 1676,  560, 1678,  560, 556,  560, 558,  560, 558,  558, 558,  588, 528,  562, 558,  560, 556,  560, 558,  560, 1676,  586, 1652,  560, 558,  560, 1676,  558, 560,  562, 1674,  560, 1676,  560, 1676,  558, 558,  562, 558,  560, 1676,  560, 558,  562, 1674,  562}; 
uint16_t bhome[75] = {4560, 4464,  564, 1674,  564, 1672,  562, 1674,  618, 500,  564, 556,  562, 554,  562, 558,  560, 556,  590, 1648,  562, 1674,  562, 1674,  562, 556,  590, 528,  562, 556,  562, 556,  562, 556,  564, 1674,  564, 554,  562, 556,  560, 1676,  564, 1672,  590, 1646,  562, 1676,  562, 556,  564, 554,  562, 1674,  560, 1676,  562, 556,  562, 554,  590, 528,  564, 554,  564, 1674,  588, 48500,  316, 954,  296, 400,  294, 678,  270};
uint16_t bpause[75] = {4532, 4490,  562, 1676,  562, 1676,  558, 1676,  562, 556,  560, 558,  562, 558,  560, 556,  560, 558,  534, 1702,  562, 1676,  560, 1676,  562, 558,  558, 558,  562, 558,  560, 558,  560, 558,  560, 1676,  562, 556,  560, 556,  560, 1676,  560, 1674,  562, 1674,  586, 532,  560, 1678,  558, 558,  560, 1674,  564, 1674,  560, 558,  560, 558,  558, 560,  558, 1676,  562, 558,  560, 48548,  270, 982,  294, 402,  266, 706,  294}; 
uint16_t bvolup[67] = {4516, 4490,  562, 1674,  562, 1674,  588, 1654,  556, 556,  588, 530,  560, 560,  562, 556,  586, 532,  560, 1678,  560, 1676,  560, 1674,  586, 532,  560, 558,  562, 556,  562, 554,  562, 556,  562, 1674,  560, 1678,  558, 1678,  560, 556,  588, 530,  534, 584,  588, 530,  560, 556,  560, 558,  588, 530,  560, 558,  586, 1648,  562, 1674,  560, 1676,  586, 1650,  560, 1676,  562};
uint16_t bvoldown[67] = {4514, 4490,  562, 1672,  562, 1674,  588, 1650,  562, 556,  562, 556,  564, 554,  564, 556,  590, 528,  560, 1678,  562, 1672,  562, 1676,  562, 556,  590, 528,  588, 530,  562, 556,  562, 556,  562, 1674,  562, 1674,  562, 556,  588, 1650,  562, 556,  562, 556,  562, 556,  562, 556,  562, 556,  590, 528,  562, 1674,  562, 556,  562, 1674,  562, 1674,  562, 1674,  562, 1674,  590};
uint16_t bchup[67] = {4508, 4498,  530, 1706,  530, 1706,  556, 1680,  554, 564,  530, 588,  530, 588,  556, 562,  530, 588,  528, 1706,  556, 1680,  528, 1706,  556, 562,  528, 590,  556, 562,  532, 586,  530, 588,  530, 588,  530, 1706,  530, 588,  530, 588,  530, 1706,  530, 588,  556, 564,  530, 586,  530, 1706,  530, 588,  530, 1706,  556, 1682,  530, 588,  556, 1680,  530, 1706,  556, 1680,  556};
uint16_t bchdown[67] = {4526, 4492,  564, 1672,  562, 1676,  558, 1678,  560, 556,  560, 558,  560, 558,  560, 558,  560, 558,  532, 1704,  560, 1678,  530, 1704,  560, 558,  560, 558,  558, 560,  560, 558,  562, 556,  562, 556,  534, 584,  560, 558,  562, 556,  558, 1678,  534, 584,  534, 584,  558, 560,  562, 1674,  562, 1674,  560, 1676,  558, 1678,  558, 560,  558, 1678,  560, 1676,  558, 1678,  558}; 
uint16_t bnetf[75] = {4530, 4492,  560, 1678,  558, 1678,  558, 1676,  560, 558,  560, 558,  558, 558,  560, 558,  558, 560,  560, 1676,  558, 1678,  558, 1678,  560, 558,  558, 560,  558, 558,  560, 558,  558, 560,  560, 1676,  558, 1678,  564, 554,  556, 562,  558, 1678,  558, 1678,  560, 1676,  556, 1680,  560, 556,  560, 558,  560, 1676,  558, 1678,  558, 560,  556, 562,  558, 558,  558, 560,  558, 48520,  314, 956,  266, 430,  268, 704,  268};
uint16_t bamaz[75] = {4512, 4490,  560, 1674,  592, 1644,  562, 1672,  562, 556,  562, 556,  590, 526,  564, 556,  562, 556,  562, 1674,  564, 1670,  590, 1648,  562, 556,  590, 528,  562, 554,  562, 556,  618, 498,  562, 554,  592, 526,  590, 1648,  562, 552,  562, 1674,  590, 1644,  564, 1672,  562, 1674,  588, 1646,  562, 1674,  564, 552,  618, 1620,  590, 530,  564, 554,  592, 526,  562, 556,  562, 48516,  316, 952,  326, 370,  296, 676,  296};
uint16_t bwww[67] = {4530, 4490,  588, 1648,  588, 1650,  588, 1646,  592, 528,  562, 554,  588, 530,  562, 556,  562, 554,  562, 1676,  562, 1672,  590, 1646,  562, 556,  562, 556,  562, 556,  588, 528,  562, 556,  566, 1670,  562, 1674,  560, 1674,  562, 556,  562, 1674,  590, 1646,  562, 556,  562, 554,  562, 556,  562, 556,  590, 528,  562, 1676,  590, 526,  588, 530,  588, 1648,  590, 1646,  590};


void setup() {

  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266

pinMode(LED_externo, OUTPUT);
pinMode(LED_interno, OUTPUT);
digitalWrite(LED_externo,LOW);// inicializando como desligado o led externo
digitalWrite(LED_interno,HIGH);// inicializando como ligado o led interno
  
pinMode(Rele1, OUTPUT);

//Serial.begin(115200);
Serial.println();
Serial.println("Conectando ao roteador ...");
//conectar ao roteador
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(2000);
Serial.print(".");
}
Serial.println(" ");
Serial.println("Rede Wifi Conectada");
Serial.println("IP de acesso: ");
Serial.println(WiFi.localIP());
delay(2000);

//inicia o webserver
server.begin();

Serial.println("Servidor Iniciado!!");
}

void loop() 

{

if(WiFi.status() == WL_CONNECTED){
  digitalWrite(LED_externo, HIGH);
  digitalWrite(LED_interno,LOW);
  }
else
  {
  digitalWrite(LED_externo, LOW);
  digitalWrite(LED_interno,HIGH);
  }

WiFiClient client = server.available(); // verifica e armazena se tem clientes conectados na rede

if (client) { // se você conseguir um cliente,
  Serial.println("Novo Cliente."); // imprime uma mensagem pela porta serial
  String currentLine = ""; // faz uma String para manter os dados recebidos do cliente

  while (client.connected()) { // loop enquanto o cliente está conectado

    if (client.available()) { // se houver bytes para ler do cliente,
      char c = client.read(); // leia um byte e depois
      Serial.write(c); // imprima o monitor serial
      if (c == '\n') { // se o byte for um caractere de nova linha
          /* se a linha atual estiver em branco, você terá dois caracteres de nova linha em uma linha.
          esse é o fim da solicitação HTTP do cliente, então envie uma resposta: */
          if (currentLine.length() == 0) {
              /* Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
              e um tipo de conteúdo para que o cliente saiba o que está por vir e, em seguida, uma linha em branco: */
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();


              // configuração de estilo do site
              client.println("<!DOCTYPE html><html>");
              //client.println("<head><title>Roadmap ESP32</title><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\"><meta charset=\"utf-8\" />");
              client.println("<head><title>Controle Remoto</title><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\"><meta charset=\"utf-8\" />");
              client.print("<style type=\"text/css\"> a{margin: 5px 5px; background: #000000; font-family: Arial; color: #ffffff; text-decoration: none; padding: 2% 20% 2%; border-radius: 10px; font-size: 1.4em;}</style>");
              client.print("<style type=\"text/css\"> h1{font-family: Arial; color: #FFFFFF;text-align:center}</style>");
              client.print("<style>th,td{text-align:center; height: 50px;}</style>");
              client.print("<meta name=""viewport"" content=""width=device-width, initial-scale=0.3"">");
              client.print("</head>");
              client.println("<body style='width:600px; background-color:#444444;'><h1>Controle TV Samsung</h1><br>");
              
              //client.print("<a style=\"font-family: Arial; font-size: 24px;\" href=\"/onoff\">Liga/Desliga</a> </br></br></br>");
              /*font-size: 24px*/

              client.println("<table style=""width:100%""><tr>");
              client.println("<th></th>");
              client.println("<th><a href=\"/onoff\">Liga/Desliga</a></th>");
              client.println("<th></th>");
              client.println("</tr>");
              
                client.println("<tr><td><a href=\"/123\">123</a></td>");
                client.println("<td></td>");
                client.println("<td><a href=\"/colors\">Cores</a></td></tr>");

                client.println("<tr><td></td>");
                client.println("<td><a href=\"/up\">Cima</a></td>");
                client.println("<td></td></tr>	");

                client.println("<tr><td><a href=\"/left\">Esq.</a></td>");
                client.println("<td><a href=\"/center\">Centro</a></td>");
                client.println("<td><a href=\"/right\">Dir</a></td></tr>");

                client.println("<tr><td></td>");
                client.println("<td><a href=\"/down\">Baixo</a></td>");
                client.println("<td></td></tr>	");

                client.println("<tr><td></td></tr>");

                client.println("<tr><td><a href=\"/back\">Voltar</a></td>");
                client.println("<td><a href=\"/home\">Home</a></td>");
                client.println("<td><a href=\"/pause\">Pause</a></td></tr>");

                client.println("<tr><td><a href=\"/volup\">Volume +</a></td>");
                client.println("<td></td>");
                client.println("<td><a href=\"/chup\">Canal +</a></td></tr>");

                client.println("<tr><td><a href=\"/voldown\">Volume -</a></td>");
                client.println("<td></td>");
                client.println("<td><a href=\"/chdown\">Canal -</a></td></tr>");

                client.println("<tr><td><a href=\"/netf\">Netflix</a></td>");
                client.println("<td><a href=\"/amaz\">Amazon</a></td>");
                client.println("<td><a href=\"/www\">WWW</a></td></tr>");

                client.println("</table></body>");
              // A resposta HTTP termina com outra linha em branco:
              client.println();
              // sair do loop while:
              break;
            } 
            else 
            { // se você tiver uma nova linha, então limpe a linha atual:
            currentLine = "";
            }
        
      } else if (c != '\r') 
        { // se você tiver qualquer outra coisa além de um caractere de retorno de linha,
        currentLine += c; // adicioná-lo ao final da linha atual
        }

    // Verifica se o pedido do cliente foi para atualizar algum dos reles (GET /T)

      if (currentLine.endsWith("GET /onoff")) {
          irsend.sendRaw(bonoff, 203, 38);  // Send a raw data capture at 38kHz.
          Serial.println();
          Serial.println("Comando enviado: Liga/Desliga");
        }
      if (currentLine.endsWith("GET /123")) {
        irsend.sendRaw(b123, 75, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: 123");
      }
      if (currentLine.endsWith("GET /colors")) {
        irsend.sendRaw(bcolors, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Colors");        
      }
      if (currentLine.endsWith("GET /up")) {
        irsend.sendRaw(bup, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Cima");        
      }
      if (currentLine.endsWith("GET /left")) {
        irsend.sendRaw(bleft, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Esquerda");
      } 
      if (currentLine.endsWith("GET /center")) {
        irsend.sendRaw(bcenter, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Center");
      } 
      if (currentLine.endsWith("GET /right")) {
        irsend.sendRaw(bright, 75, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Direita");        
      }
     
      if (currentLine.endsWith("GET /down")) {
        irsend.sendRaw(bdown, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Baixo");        
      }
      if (currentLine.endsWith("GET /back")) {
        irsend.sendRaw(bback, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Voltar");        
      }
      if (currentLine.endsWith("GET /home")) {
        irsend.sendRaw(bhome, 75, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Home");        
      }      
      if (currentLine.endsWith("GET /pause")) {
        irsend.sendRaw(bpause, 75, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Pause");        
      }
      if (currentLine.endsWith("GET /volup")) {
        irsend.sendRaw(bvolup, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Vol up");        
      }
      if (currentLine.endsWith("GET /voldown")) {
        irsend.sendRaw(bvoldown, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Vol down");        
      }
      if (currentLine.endsWith("GET /chup")) {
        irsend.sendRaw(bchup, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Canal up ");        
      }
      if (currentLine.endsWith("GET /chdown")) {
        irsend.sendRaw(bchdown, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Canal Down ");        
      }
      if (currentLine.endsWith("GET /netf")) {
        irsend.sendRaw(bnetf, 75, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Netflix ");        
      }
      if (currentLine.endsWith("GET /amaz")) {
        irsend.sendRaw(bamaz, 75, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: Amazon ");        
      }
      if (currentLine.endsWith("GET /www")) {
        irsend.sendRaw(bwww, 67, 38);  // Send a raw data capture at 38kHz.
        Serial.println();
        Serial.println("Comando enviado: WWW ");        
      }

      
      }
    }
  }

 

// termina a conexão com o cliente
client.stop();
Serial.println("Cliente desconectado.");
Serial.println("IP de acesso: ");
Serial.println(WiFi.localIP());
delay(1000);

}
