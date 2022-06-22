/*
 *  2114 (1024 x 4bits) RAM chip tester 
 *  Nino MegaDriver - nino@nino.com.br
 *  https://www.megadriver.com.br
 *  
 *  Default Pinout (Mega):
 *  AO <= [A6         Vcc] => 13
 *  A1 <= [A5    2     A7] => 12
 *  A2 <= [A4    1     A8] => 11
 *  A3 <= [A3    1     A9] => 10
 *  A4 <= [A0    4     O1] => 9
 *  A5 <= [A1          02] => 8
 *  A6 <= [A2          03] => 7
 *  A7 <= [~CS         04] => 6
 * GND <= [GND        ~WE] => 5
 * 
 */

uint8_t A[] = {A4,A5,A6,A3,A2,A1,A0,12,11,10};
uint8_t O[] = {9,8,7,6};
uint8_t CS = A7;
uint8_t WE = 5;

uint16_t bits[] = {
  0b0000000000000001,
  0b0000000000000010,
  0b0000000000000100,
  0b0000000000001000,
  0b0000000000010000,
  0b0000000000100000,
  0b0000000001000000,
  0b0000000010000000,
  0b0000000100000000,
  0b0000001000000000,
};

String setAddress(uint16_t address){
  String ret;
  uint8_t status;
  for(uint8_t i=0; i<sizeof A;i++){
    if ((address & bits[i])==0) {
      digitalWrite(A[i], LOW);
      status = 0;
    }else{
      digitalWrite(A[i], HIGH);
      status = 1;
    }
    ret.concat(status); 
    ret.concat(":");    
  }
  ret.remove(ret.length()-1,1);
  return ret;
}

void bruteforce(bool quit=false){
  int total_errs=0;
  Serial.print("Brute forcing: ");
  for(int16_t address=0; address<1024; address++){
    digitalWrite(CS, HIGH);
    digitalWrite(WE, HIGH);
    String ret = setAddress(address);
    for(int a=0;a<2;a++)
      for(int b=0;b<2;b++)
        for(int c=0;c<2;c++)
          for(int d=0;d<2;d++){
            for(int i=0;i<sizeof O; i++) pinMode(O[i], OUTPUT);
            digitalWrite(O[0], a);
            digitalWrite(O[1], b);
            digitalWrite(O[2], c);
            digitalWrite(O[3], d);
            digitalWrite(WE, LOW);
            digitalWrite(CS, LOW);
            digitalWrite(CS, HIGH);
            digitalWrite(WE, HIGH);
            for(int i=0;i<sizeof O; i++) pinMode(O[i], INPUT_PULLUP);
            digitalWrite(CS, LOW);
            int err=0;
            if(digitalRead(O[0]) != a) err++;
            if(digitalRead(O[1]) != b) err++;
            if(digitalRead(O[2]) != c) err++;
            if(digitalRead(O[3]) != d) err++;
            if(err>0){
              char msg[100];
              sprintf(msg, "[%d:%d:%d:%d]!=[%d:%d:%d:%d] ERROR @ 0x%0.10X", a,b,c,d,digitalRead(O[0]),digitalRead(O[1]),digitalRead(O[2]),digitalRead(O[3]),address);
              Serial.println(msg);
              total_errs++;
              if(quit) return;
            }
            digitalWrite(CS, HIGH);
            digitalWrite(WE, HIGH);
          }
  }
  
  char msg[100];
  if(total_errs>0){
    sprintf(msg, "BRUTE FORCE: BAD %d error(s) found", total_errs);
    Serial.println(msg);
  }else{
    Serial.println("BRUTE FORCE: OK!"); 
  }
  
}

void test(int st, bool quit=false){
  int total_errs = 0;
  Serial.print("Testing with fixed state: ");
  Serial.println(st);
  for(int16_t address=0; address<1024; address++){
    digitalWrite(CS, HIGH);
    digitalWrite(WE, HIGH);
    String ret = setAddress(address);
    for(int i=0;i<sizeof O; i++) {
      pinMode(O[i], OUTPUT);
      digitalWrite(O[i], st);
    }
    digitalWrite(WE, LOW);
    digitalWrite(CS, LOW);
    digitalWrite(CS, HIGH);
    digitalWrite(WE, HIGH);
    for(int i=0;i<sizeof O; i++) pinMode(O[i], INPUT_PULLUP);
    digitalWrite(CS, LOW);
    int err = 0;
    for(int i=0;i<sizeof O; i++){
      if(digitalRead(O[i]) != st) err++;
    }
    if(err>0){
      char msg[100];
      sprintf(msg, "ERROR @ 0x%0.10X", address);
      Serial.println(msg);
      total_errs += err;
      if(quit) return;
    }
    digitalWrite(CS, HIGH);
    digitalWrite(WE, HIGH);
  }
  
  char msg[100];
  if(total_errs>0){
    sprintf(msg, "TEST [%d]: BAD %d error(s) found", st, total_errs);
    Serial.println(msg);
  }else{
    sprintf(msg, "TEST [%d]: OK", st);
    Serial.println(msg);
  }
  
}

void setup() {
  for(int i=0;i<sizeof A; i++) pinMode(i, OUTPUT);
  for(int i=0;i<sizeof O; i++) pinMode(i, INPUT);
  pinMode(WE, OUTPUT);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  digitalWrite(WE, HIGH);
  Serial.begin(115200);
}

int aux = 0;
void loop() {
  Serial.println("********************************************************");
  Serial.println("2114 Tester ready...                                    ");
  Serial.println("                                                        ");
  Serial.println("Input:                                                  ");
  Serial.println("testlow    => test with 0s                              ");
  Serial.println("testhigh   => test with 1s                              ");
  Serial.println("bruteforce => test all combination of bits possible     ");
  Serial.println("default    => test everything three times               ");
  Serial.println("********************************************************");

  while(!Serial.available() ){ }
  String input = Serial.readStringUntil('\n');
  char msg[100];
  if(input.equals("testlow")){
    test(LOW);
  }else if(input.equals("testhigh")){
    test(HIGH);
  }else if(input.equals("brute") || input.equals("bruteforce")){
    bruteforce();
  }else{
    for(int i=0;i<3;i++){
      if(i>0)   Serial.println("********************************************************");
      sprintf(msg, "Tests run #%d", i);
      Serial.println(msg);
      test(LOW,true);
      test(HIGH,true);
      bruteforce(true);
    }
  }
}
