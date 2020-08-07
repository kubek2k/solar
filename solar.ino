//Dwa ukosniki oznaczaja komentarz.
//to jest komentarz

const uint16_t LICZBA_PROBEK_NATEZENIA_SWIATLA = 10; //liczba probek z pomiaru swiata po ktorych wartosc bedzie usredniona . liczba 16 bitowa dodatnia

const byte NastawProguNatezeniaSwiatla = A0; //Zmienna odczytu progu zadziaĹ‚ania z potencjometru(wartoĹ› porĂłwnywana z fototranzystorem tj. SwiatloAPin). byte czyli liczba osmiobitowa ze znakiem
const byte CzujnikGora = A1; //definicja numerow wejsc analogowych, w ktorych umieszczone sa czujniki 
const byte CzujnikDol = A2;
const byte CzujnikWschod = A3;
const byte CzujnikZachod = A4;
const byte CzujnikNatezeniaSwiatla = A5;

const byte Dioda = 4; //numer wyjscia cyfrowego diody pomocniczej." Dodamy tez druga diode pomocnicza ktora bedzie wskazywac to co w zapytaniu nr 2     "  

const byte SilnikGora = 5; //numery wyjsc cyfrowych na silniki
const byte SilnikDol = 6;
const byte SilnikWschod = 9;
const byte SilnikZachod = 10;

const byte ReczA1 = 8; //przyciski do obslugi recznej
const byte ReczA2 = 11;
const byte ReczA3 = 12;
const byte ReczA4 = 13;

const byte PinTryb = 7; //numer wejscia cyfrowego(przycisk Tryb)) okreslajacego przejscie w tryb reczny/automatyczny
const byte CzujnikLadowania = 1; //numer wejscia cyfrowego na ktore podaje napiÄ™cie z panelu "HIGH"ograniczone dzielnikiem i zabezpieczone dioda zenera 4,7V "szukanie slonca"

const float ulamekMroku = 0.03; //mnoznik wartosciGranicaChmur powodujaca zaĹ‚Ä…czenie silnika A3 w przypadku ciemnosci- powrot do poranka.przy 0.05 troche za wczesnie sie przestawia

byte PWM = 255; //definicja PWMow z jaka maja dzialac silniki - 255 to max

const uint32_t TOLERANCJA = 1;

void setup() { //void setup to funkcja w ktorej deklaruje sie w jaki sposob maja pracowac wejscia/wyjscia. Blok void setup wykonuje sie tylko raz po zalaczeniu kontrolera
    pinMode(CzujnikGora, INPUT);
    pinMode(CzujnikDol, INPUT);
    pinMode(CzujnikWschod, INPUT);
    pinMode(CzujnikZachod, INPUT);
    pinMode(Dioda, OUTPUT); //np. pin o numerze Dioda(zdefiniowanej wczesniej) ma pracowac jako OUTPUT czyli wyjscie

    pinMode(PinTryb, INPUT); //tutaj INPUT czyli cos bedzie odczytywane. W przypadku pinow cyfrowych bedzie to 0 lub 1
    pinMode(CzujnikLadowania, INPUT);
    pinMode(SilnikGora, OUTPUT);
    pinMode(SilnikDol, OUTPUT);
    pinMode(SilnikWschod, OUTPUT);
    pinMode(SilnikZachod, OUTPUT);
    pinMode(ReczA1, INPUT);
    pinMode(ReczA2, INPUT);
    pinMode(ReczA3, INPUT);
    pinMode(ReczA4, INPUT);
}

boolean trybAutomatyczny() {
    return digitalRead(PinTryb) == 0;
}

void jedzNaWschod() {
    digitalWrite(SilnikZachod, LOW);
    analogWrite(SilnikWschod, PWM);
}

void jedzNaZachod() {
    digitalWrite(SilnikWschod, LOW);
    analogWrite(SilnikZachod, PWM);
}

void jedzWGore() {
    digitalWrite(SilnikDol, LOW); //digital=cyfra write=wpisz. Wpisz na pin SilnikDol stan LOW=niski 
    analogWrite(SilnikGora, PWM); //wpisz na pin SilnikGora wartosc pseudoanalogowa(bo sa to pwmy a nie analogowa wartosc) o wartosci PWM     
}

void jedzWDol() {
    digitalWrite(SilnikGora, LOW);
    analogWrite(SilnikDol, PWM);
}

void nieJedzPion() {
    digitalWrite(SilnikGora, LOW);
    digitalWrite(SilnikDol, LOW);
}

void nieJedzPoziom() {
    digitalWrite(SilnikWschod, LOW);
    digitalWrite(SilnikZachod, LOW);
}

void nieJedz() {
    nieJedzPoziom();
    nieJedzPion();
}

void wlaczDiode() {
    digitalWrite(Dioda, HIGH);
}

void wylaczDiode() {
    digitalWrite(Dioda, LOW);
}

boolean wiekszyNiz(uint32_t lewy, uint32_t prawy) {
    return lewy > prawy + TOLERANCJA;
}

boolean sieLaduje() {
    return digitalRead(CzujnikLadowania) == 1;
}

void loop() { //blok void loop wykonuje sie w nieskonczonosc
    if (trybAutomatyczny()) {
        uint32_t SwiatloGora = 0; //deklaracja zmiennych- odczytow z 4 diod-czujnikow. uint32_t oznacza ze sa to liczby 32bitowe dodatnie(u oznacze ze liczba nie ma znaku minus)
        uint32_t SwiatloDol = 0;
        uint32_t SwiatloWschod = 0;
        uint32_t SwiatloZachod = 0;

        uint16_t NatezenieSwiatla = analogRead(CzujnikNatezeniaSwiatla); //odczyt napiecia z fototranzystora, (read=odczytaj)
        uint16_t GranicaChmur = analogRead(NastawProguNatezeniaSwiatla); //odczyt granicy do dzialania z potencjometru

        for (int i = 1; i < LICZBA_PROBEK_NATEZENIA_SWIATLA; i++) { //petla ktora wykona sie LICZBA_PROBEK_NATEZENIA_SWIATLA razy w ktorej odczytywane beda napiecia z diod-czujnikow
            SwiatloGora = SwiatloGora + analogRead(CzujnikGora);
            SwiatloDol = SwiatloDol + analogRead(CzujnikDol);
            SwiatloWschod = SwiatloWschod + analogRead(CzujnikWschod);
            SwiatloZachod = SwiatloZachod + analogRead(CzujnikZachod);
            delay(10); //delay=czekaj, 10 to 10 milisekund
        };

        SwiatloGora = float(SwiatloGora) / float(LICZBA_PROBEK_NATEZENIA_SWIATLA); //usrednienie sumy z odczytanych wartosci po liczbie probek. float oznacza zamiane reprezentacji liczb na zmiennoprzecinkowa
        SwiatloDol = float(SwiatloDol) / float(LICZBA_PROBEK_NATEZENIA_SWIATLA);
        SwiatloWschod = float(SwiatloWschod) / float(LICZBA_PROBEK_NATEZENIA_SWIATLA);
        SwiatloZachod = float(SwiatloZachod) / float(LICZBA_PROBEK_NATEZENIA_SWIATLA);

        if (float(NatezenieSwiatla) < ulamekMroku * float(GranicaChmur)) { // jest na tyle ciemno ze zapewne skonczyl sie dzien
            wylaczDiode();
            jedzNaWschod();
        } else if (NatezenieSwiatla > GranicaChmur) { //jesli jest odpowiednio jasno i jestesmy w trybie auto to niech sie wykona to co ponizej       
            wlaczDiode();

            if (wiekszyNiz(SwiatloGora, SwiatloDol)) {
                jedzWGore(); 
            } else if (wiekszyNiz(SwiatloDol, SwiatloGora)) {
                jedzWDol();
            } else { //w kazdym innym przypadku naloz stany niskie
                nieJedzPion();
            }

            if (wiekszyNiz(SwiatloWschod, SwiatloZachod)) { //to samo co powyzej ale dla drugiej pary silnikow/czujnikow
                jedzNaWschod();
            } else if (wiekszyNiz(SwiatloZachod, SwiatloWschod)) {
                jedzNaZachod();
            } else { //w kazdym innym przypadku stany niskie
                nieJedzPoziom();
            }

            // tutaj dac warunek na szukanie slonca po przerwie na chmure lub start poranny

            if (sieLaduje()) {
                delay(500);
                nieJedz();
                delay(30000);
            } else {
                delay(2000);
                nieJedz();
                delay(500);
            }
        } else {
            wylaczDiode();
            nieJedzPion();
            nieJedzPoziom();
        }
    } else { //tutaj zdefiniowane jest dzialanie przy trybie manulanym
        nieJedz();
        while (digitalRead(PinTryb) == 1) { //dopoki zalaczany jest stan wysoki na PinTryb to rob to co ponizej            
            nieJedz();
            if (digitalRead(ReczA1) == 1) {
                wlaczDiode();
                jedzWGore();
                delay(100);
                wylaczDiode()
            } else if (digitalRead(ReczA2) == 1) {
                wlaczDiode();
                jedzWDol();
                delay(100);
                wylaczDiode();
            } else if (digitalRead(ReczA3) == 1) {
                wlaczDiode();
                jedzNaWschod();
                delay(100);
                wylaczDiode();
            } else if (digitalRead(ReczA4) == 1) {
                wlaczDiode();
                jedzNaZachod();
                delay(100);
                wylaczDiode();
            } else {
                nieJedz();
                wylaczDiode();
            }
        }

        nieJedz();
        wlaczDiode(); //tutaj i kilka liniej nizej zrealizowane jest mruganie diody sygnalizacyjnej
        delay(200);
        wylaczDiode();
        delay(200);
        wlaczDiode();
        delay(200);
        wylaczDiode();
    }

    delay(100);
}
