//Dwa ukosniki oznaczaja komentarz.
//to jest komentarz



uint32_t Swiatlosc1;   //deklaracja zmiennych- odczytow z 4 diod-czujnikow. uint32_t oznacza ze sa to liczby 32bitowe dodatnie(u oznacze ze liczba nie ma znaku minus)
uint32_t Swiatlosc2;
uint32_t Swiatlosc3;
uint32_t Swiatlosc4;

uint16_t liczba_probek=100; //liczba probek z pomiaru swiata po ktorych wartosc bedzie usredniona . libcza 16 bitowa dodatnia


byte GranicaChmurPin=A0;    //Zmienna odczytu progu zadziałania z potencjometru(wartoś porównywana z fototranzystorem tj. SwiatloAPin). byte czyli liczba osmiobitowa ze znakiem
byte IN1=A1;                 //definicja numerow wejsc analogowych, w ktorych umieszczone sa czujniki 
byte IN2=A2;
byte IN3=A3;  
byte IN4=A4;
byte SwiatloscPin=A5;

byte Dioda=4;               //numer wyjscia cyfrowego diody pomocniczej." Dodamy tez druga diode pomocnicza ktora bedzie wskazywac to co w zapytaniu nr 2     "  

byte SilA1=5;               //numery wyjsc cyfrowych na silniki
byte SilA2=6;   
byte SilA3=9;
byte SilA4=10;

byte ReczA1=8;				//przyciski do obslugi recznej
byte ReczA2=11;
byte ReczA3=12;
byte ReczA4=13;

byte PinTryb=7;             //numer wejscia cyfrowego(przycisk Tryb)) okreslajacego przejscie w tryb reczny/automatyczny
byte PinSol=1;              //numer wejscia cyfrowego na ktore podaje napięcie z panelu "HIGH"ograniczone dzielnikiem i zabezpieczone dioda zenera 4,7V "szukanie slonca"

float ulamekMroku=0.03; //mnoznik wartosciGranicaChmur powodujaca załączenie silnika A3 w przypadku ciemnosci- powrot do poranka.przy 0.05 troche za wczesnie sie przestawia
uint16_t Odczyt;
uint16_t Tol;
uint16_t SwiatloscOdczyt;		  //do tej zmiennej zostanie przypisana wartosc swiatlosci z fotoogniwa/fototranzystora
uint16_t GranicaChmur=500; //definicja poczatkowej minimalnej swiatlosci do dzialania, czulosc?

byte PWM=255;               //definicja PWMow z jaka maja dzialac silniki - 255 to max

const uint32_t TOLERANCJA = 2;

boolean wiekszeNiz(uint32_t x, uint32_t y) {
  return x > y + TOLERANCJA; 
}

void setup() {				//void setup to funkcja w ktorej deklaruje sie w jaki sposob maja pracowac wejscia/wyjscia. Blok void setup wykonuje sie tylko raz po zalaczeniu kontrolera
           pinMode(IN1,INPUT);      
           pinMode(IN2,INPUT);
           pinMode(IN3,INPUT);
           pinMode(IN4,INPUT);
           pinMode(Dioda,OUTPUT);	//np. pin o numerze Dioda(zdefiniowanej wczesniej) ma pracowac jako OUTPUT czyli wyjscie

           pinMode(PinTryb,INPUT);	//tutaj INPUT czyli cos bedzie odczytywane. W przypadku pinow cyfrowych bedzie to 0 lub 1
           pinMode(PinSol,INPUT);
           pinMode(SilA1,OUTPUT);     
           pinMode(SilA2,OUTPUT);
           pinMode(SilA3,OUTPUT);
           pinMode(SilA4,OUTPUT);
           pinMode(ReczA1,INPUT);
           pinMode(ReczA2,INPUT);
           pinMode(ReczA3,INPUT);
           pinMode(ReczA4,INPUT);           
           
}


void loop() {					//blok void loop wykonuje sie w nieskonczonosc
          Swiatlosc1=0;			//przypisanie do zmiennych Swiatlosc(czyli odczyty z diod) wartosci 0
          Swiatlosc2=0;
          Swiatlosc3=0;
          Swiatlosc4=0;
                  
          SwiatloscOdczyt=analogRead(SwiatloscPin); 			//odczyt napiecia z fototranzystora, (read=odczytaj)
          GranicaChmur=analogRead(GranicaChmurPin);             //odczyt granicy do dzialania z potencjometru
                   
          for (int i=1;i<liczba_probek;i++){				//petla ktora wykona sie liczba_probek razy w ktorej odczytywane beda napiecia z diod-czujnikow
               Swiatlosc1=Swiatlosc1+analogRead(IN1);
               Swiatlosc2=Swiatlosc2+analogRead(IN2);
               Swiatlosc3=Swiatlosc3+analogRead(IN3);
               Swiatlosc4=Swiatlosc4+analogRead(IN4);
               delay(10);									//delay=czekaj, 10 to 10 milisekund
          };
      
          Swiatlosc1=float(Swiatlosc1)/float(liczba_probek); //usrednienie sumy z odczytanych wartosci po liczbie probek. float oznacza zamiane reprezentacji liczb na zmiennoprzecinkowa
          Swiatlosc2=float(Swiatlosc2)/float(liczba_probek);
          Swiatlosc3=float(Swiatlosc3)/float(liczba_probek);
          Swiatlosc4=float(Swiatlosc4)/float(liczba_probek);

          if (float(SwiatloscOdczyt)<ulamekMroku*float(GranicaChmur) && digitalRead(PinTryb) == 0){          
                 digitalWrite(SilA4,LOW);                 
                 analogWrite(SilA3,PWM);                
                 digitalWrite(Dioda,HIGH);              
          }
         
          else if (SwiatloscOdczyt>GranicaChmur && digitalRead(PinTryb) == 0){    //jesli jest odpowiednio jasno i jestesmy w trybie auto to niech sie wykona to co ponizej       
           
             if (wiekszeNiz(Swiatlosc1, Swiatlosc2)){
                 digitalWrite(SilA2,LOW);								//digital=cyfra write=wpisz. Wpisz na pin SilA2 stan LOW=niski 
                 analogWrite(SilA1,PWM);								//wpisz na pin SilA1 wartosc pseudoanalogowa(bo sa to pwmy a nie analogowa wartosc) o wartosci PWM
                 digitalWrite(Dioda,HIGH);
             }
             
             else if (wiekszeNiz(Swiatlosc2, Swiatlosc1)){
                 digitalWrite(SilA1,LOW);
                 analogWrite(SilA2,PWM);
                 digitalWrite(Dioda,HIGH);

             }
             
             else{														//w kazdym innym przypadku naloz stany niskie
                 digitalWrite(SilA1,LOW);
                 digitalWrite(SilA2,LOW);
                 digitalWrite(Dioda,LOW); 
             }    
                
             if (wiekszeNiz(Swiatlosc3, Swiatlosc4)){		//to samo co powyzej ale dla drugiej pary silnikow/czujnikow
                 digitalWrite(SilA4,LOW);              
                 analogWrite(SilA3,PWM);
                 digitalWrite(Dioda,HIGH);
            }
            
             else if (wiekszeNiz(Swiatlosc4, Swiatlosc3)){
                 digitalWrite(SilA3,LOW);
                 analogWrite(SilA4,PWM);
                 digitalWrite(Dioda,HIGH);  
             }
             
             else{                             //w kazdym innym przypadku stany niskie
                 digitalWrite(SilA3,LOW);
                 digitalWrite(SilA4,LOW);
                 digitalWrite(Dioda,LOW);                      
             }                              
                // tutaj dac warunek na szukanie slonca po przerwie na chmure lub start poranny

             if(digitalRead(PinSol)==1)
            { 
              czekajDioda(1000,10);       //wykonuj przez 1 sekunde,( ale co 10 milisekund sprawdzaj chmury i załączaj/wyłączaj diode? uklad)
             stanNiski();               //zalacz stani niskie na silniki
             czekajDioda(30000,10);  //przez 30 sekund, (ale co 10 milisekund sprawdzaj chmury i załączaj/wyłączaj diode?)
             }    
             else
             {
              czekajDioda(5000,10);       //wykonuj prez 5 sekund, ale co 10 milisekund sprawdzaj chmury i załączaj/wyłączaj diode? uklad
             stanNiski();               //zalacz stani niskie na silniki
             czekajDioda(1000,10);  //przez 1 sekunde, ale co 10 milisekund sprawdzaj chmury i załączaj/wyłączaj diode?      
              }
          }
        

        else if(digitalRead(PinTryb) == 1){			//tutaj zdefiniowane jest dzialanie przy trybie manulanym
                stanNiski();						//wywolanie funkcji stanNiski(); ktora znajduje sie na samym dole
                while(digitalRead(PinTryb)==1){     //dopoki zalaczany jest stan wysoki na PinTryb to rob to co ponizej            

                      if(digitalRead(ReczA1) == 1){
                         digitalWrite(Dioda,HIGH);
                         stanNiski();
                         analogWrite(SilA1,PWM);
                         delay(100);
                         digitalWrite(Dioda,LOW);
                      }
                      else if(digitalRead(ReczA2) == 1){
                        digitalWrite(Dioda,HIGH);
                        stanNiski();
                        analogWrite(SilA2,PWM);
                        delay(100);
                        digitalWrite(Dioda,LOW);
                      }
                      else if(digitalRead(ReczA3) == 1){
                              digitalWrite(Dioda,HIGH);
                              stanNiski();  
                              analogWrite(SilA3,PWM);
                              delay(100);
                              digitalWrite(Dioda,LOW);
                      }
                      else if(digitalRead(ReczA4) == 1){
                              digitalWrite(Dioda,HIGH);
                              stanNiski();  
                              analogWrite(SilA4,PWM);
                              delay(100);
                              digitalWrite(Dioda,LOW);
                      }
                      else{
                           stanNiski();
                           digitalWrite(Dioda,LOW);
                      }
              
               }

                stanNiski();
                digitalWrite(Dioda,HIGH);		//tutaj i kilka liniej nizej zrealizowane jest mruganie diody sygnalizacyjnej
                delay(200);
                digitalWrite(Dioda,LOW);
                delay(200);
                digitalWrite(Dioda,HIGH);
                delay(200);
                digitalWrite(Dioda,LOW);
        }
        
        else {
            digitalWrite(SilA1,LOW);
            digitalWrite(SilA2,LOW);  
            digitalWrite(SilA3,LOW);
            digitalWrite(SilA4,LOW);        
        }
        delay(100);
}





void stanNiski(){
               digitalWrite(SilA1,LOW);
               digitalWrite(SilA2,LOW);  
               digitalWrite(SilA3,LOW);
               digitalWrite(SilA4,LOW);
               digitalWrite(Dioda,LOW);                       
}



void miga(){
          digitalWrite(Dioda,LOW);
          digitalWrite(Dioda,HIGH);  
          digitalWrite(Dioda,LOW);
          digitalWrite(Dioda,HIGH);
          digitalWrite(Dioda,LOW);                
}


void czekajDioda(float Czas,float Czek){
                 for (int i=1;i<Czas/Czek;i++){ 
                      delay(Czek);                     
                      SwiatloscOdczyt=analogRead(SwiatloscPin);       //odczyt napiecia z fototranzystora, (read=odczytaj)
                      GranicaChmur=analogRead(GranicaChmurPin);             //odczyt granicy dzialania z potencjometru                             
                      if (SwiatloscOdczyt>GranicaChmur){
                          digitalWrite(Dioda,HIGH);         
                      }
                      else{
                          digitalWrite(Dioda,LOW);         
                      }
                 }                     
}


