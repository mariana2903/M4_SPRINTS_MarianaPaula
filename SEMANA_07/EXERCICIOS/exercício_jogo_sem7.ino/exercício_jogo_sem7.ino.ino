// Genius esp32 - exercício semana 7
// Mariana Silva de Paula

#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// configura o nome da rede que será criada
const char *ssid = "Marii";

// configura a senha da rede sem fio criada
const char *password = "12345678";

#define LED_BUILTIN 2 // configura o pino do LED embutido
#define ledG 35
#define ledR 36
#define ledY 37
#define ledB 38
#define buzzer 47
#define botaoG 18
#define botaoR 17
#define botaoY 16
#define botaoB 15

AsyncWebServer server(80);

// Variáveis globais
int sequencia[32] = {};
int botoes[4] = {botaoG, botaoR, botaoY, botaoB};
int leds[4] = {ledG, ledR, ledY, ledB};
int tons[4] = {262, 294, 330, 349};
int rodada = 0;
int passo = 0;
int botao_pressionado = 0;
bool game_over = false;

void nextRound();
void reproduceSequency();
void awaitPlayer();
String html();

void setup()
{
    Serial.begin(9600);

    pinMode(ledG, OUTPUT);   // led1
    pinMode(ledR, OUTPUT);   // led2
    pinMode(ledY, OUTPUT);   // led3
    pinMode(ledB, OUTPUT);   // led4
    pinMode(buzzer, OUTPUT); // buzzer
    pinMode(botaoG, INPUT);  // botão1
    pinMode(botaoR, INPUT);  // botão2
    pinMode(botaoY, INPUT);  // botão3
    pinMode(botaoB, INPUT);  // botão4

    Serial.println("Conectando ao wifi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);
        delay(800);
        Serial.print(".");
    }

    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *req)
              { req->send(200, "text/html; charset=UTF-8", html()); });

    server.begin();
}

String html()
{
    String html = "";
    html += "<meta name=\"viewport\" content=\"width=device-width, "
            "initial-scale=1\">";
    html += "<title>Relatorio</title>";
    html += "<style>html { font-family: Helvetica; display: inline-block; "
            "margin: 0px auto; text-align: center;}";
    html += "@media screen and (max-width: 480px){";
    html += ".content{width: 94%;}";
    html += ".rTable thead{display: none;}";
    html += ".rTable tbody td{";
    html += "    display: flex;";
    html += "    flex-direction: column;";
    html += "}";
    html += " .rTable tbody td{";
    html += "    display: flex;";
    html += "    flex-direction: column;";
    html += "}";
    html += "}";
    html += "@media screen and (min-width: 1200px){";
    html += "    .content{width: 100%;}";
    html += "    .rTable th , rTable td{padding: 7px 0;}";
    html += "    .rTable tbody tr td:nth-child(1){width:10%}";
    html += "    .rTable tbody tr td:nth-child(2){width:10%}";
    html += "    .rTable tbody tr td:nth-child(3){width:10%}";
    html += "    .rTable tbody tr td:nth-child(4){width:10%}";
    html += "}";
    html += "*{";
    html += "margin: 0;";
    html += "padding: 0;";
    html += "box-sizing: border-box;";
    html += "align-items: center;";
    html += "}";
    html += ".content{";
    html += "    display: flex;";
    html += "    margin: auto;";
    html += "}";
    html += ".rTable{";
    html += "    width: 100%;";
    html += "    text-align: center;";
    html += "    font-size: 20px;";
    html += "}";
    html += "h1{";
    html += "    font-size: 30px;";
    html += "    padding-bottom: 20px;";
    html += "    text-align: center;";
    html += "    font-weight: bold;";
    html += "}";
    html += "#oi{";
    html += "    font-weight: bold;";
    html += "}";
    html += "</style>";
    html += "</head>";

    html += "<body>";
    html += "<div class=\"container\">";
    html += "  <table class=\"table\">";
    html += " <thead>";
    html += "   <tr>";
    html += "     <th id=\"main\">ID</th>";
    html += "      </tr>";
    html += "    </tbody>";
    html += "  </table>";
    html += "</div>";
    html += "</body>";

    html += "<h1>JOGO GENIUS</h1>";
    html += "</br>";
    html += "<h3>Pontuação</h3>";
    html += "</br>";
    html += "<h3>Rodadas: " + String(rodada) + " </h3> ";
    html += "</div>";
    return html;
}

void loop()
{
    nextRound();
    reproduceSequency();
    awaitPlayer();
    // estrutura para reiniciar todas as variáveis
    if (game_over == true)
    {
        sequencia[32] = {};
        rodada = 0;
        passo = 0;
        game_over = false;
    }
    delay(1000);
}

// Funções
void nextRound()
{
    int sorteio = random(4);
    sequencia[rodada] = sorteio;
    rodada = rodada + 1;
    // Serial.print(sorteio);
}

void reproduceSequency()
{
    for (int i = 0; i < rodada; i++)
    {
        tone(7, tons[sequencia[i]]);
        digitalWrite(leds[sequencia[i]], HIGH);
        delay(500);
        noTone(7);
        digitalWrite(leds[sequencia[i]], LOW);
        delay(100);
    }
}

void awaitPlayer()
{
    for (int i = 0; i < rodada; i++)
    {
        bool jogada_efetuada = false;
        while (!jogada_efetuada)
        {
            for (int i = 0; i <= 3; i++)
            {
                if (digitalRead(botoes[i]) == HIGH)
                {
                    botao_pressionado = i;
                    tone(7, tons[i]);
                    digitalWrite(leds[i], HIGH);
                    delay(300);
                    digitalWrite(leds[i], LOW);
                    noTone(7);
                    jogada_efetuada = true;
                }
            }
        }
        // verificar a jogada
        if (sequencia[passo] != botao_pressionado)
        {
            // efeito luminoso/sonoro indicando que o jogador perdeu
            for (int i = 0; i <= 3; i++)
            {
                tone(7, 70);
                digitalWrite(leds[i], HIGH);
                delay(100);
                digitalWrite(leds[i], LOW);
                noTone(7);
            }
            game_over = true;
            break;
        }
        passo = passo + 1;
    }
    passo = 0;
}

