// Define pins for Motor 1
#define dirPin1 2
#define stepPin1 3
#define enablePin1 4

// Define pins for Motor 2
#define dirPin2 5
#define stepPin2 6
#define enablePin2 7





#define prescalar1 64
#define prescalar2 64 

// Motor control variables
unsigned long ticks1 = 255, ticks2 = 255;
bool isStep1 = false, isStep2 = false;

void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Setup pins for Motor 1
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(enablePin1, OUTPUT);

  // Setup pins for Motor 2
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  setTimers();
}

void loop() {
  if (Serial.available() > 0) {
    String receivedStr = Serial.readString();

    Serial.println(receivedStr);
    // Parsing commands for Motor 1
    if (receivedStr.substring(0,3) == "ok?"){
      Serial.println("ok");
    }
    if (receivedStr.startsWith(">")) {
      digitalWrite(dirPin1, HIGH);
      TIMSK1 |= _BV(OCIE1A);
      Serial.println(">");
    } else if (receivedStr.startsWith("<")) {
      digitalWrite(dirPin1, LOW);
      TIMSK1 |= _BV(OCIE1A);
      Serial.println("<");
    } else if (receivedStr.startsWith("0")) {
      TIMSK1 &= ~_BV(OCIE1A);
      Serial.println("0");
    } else if (receivedStr.startsWith("x")) {
      digitalWrite(enablePin1, HIGH);
      Serial.println("x");
    }

    // Parsing commands for Motor 2
    if (receivedStr.startsWith("l")) {
      digitalWrite(dirPin2, HIGH);
      TIMSK2 |= _BV(OCIE2A);
      Serial.println("l");
    } else if (receivedStr.startsWith("r")) {
      digitalWrite(dirPin2, LOW);
      TIMSK2 |= _BV(OCIE2A);
      Serial.println("r");
    } else if (receivedStr.startsWith("s")) {
      TIMSK2 &= ~_BV(OCIE2A);
      Serial.println("s");
    } else if (receivedStr.startsWith("d")) {
      digitalWrite(enablePin2, HIGH);
      Serial.println("d");
    }

    // Parsing speed/tick commands
    if (receivedStr.startsWith("t")) {
      ticks1 = receivedStr.substring(1).toInt();
      OCR1A = int(ticks1);
      Serial.println("t");
      Serial.println(ticks1);
    }
    if (receivedStr.startsWith("q")) {
      ticks2 = receivedStr.substring(1).toInt();
      OCR2A = int(ticks2);
      TCNT2 = 0;
      Serial.println("q");
      Serial.println(ticks2);
//      Serial.print("OCR2A updated to: ");
//      Serial.println(OCR2A); // Debug statement
    }
  }
}

// ISR for Timer1 (Motor 1)
ISR(TIMER1_COMPA_vect) {
  isStep1 = !isStep1;
  if (isStep1) {
    digitalWrite(stepPin1, HIGH);
  } else {
    digitalWrite(stepPin1, LOW);
  }
}

// ISR for Timer2 (Motor 2)
ISR(TIMER2_COMPA_vect) {
  isStep2 = !isStep2;
  if (isStep2) {
    digitalWrite(stepPin2, HIGH);
  } else {
    digitalWrite(stepPin2, LOW);
  }
}




void setTimers(){

    cli();
      // Setup Timer1 for Motor 1
    TCCR1A = 0; TCCR1B = 0; TIMSK1 = 0;
    OCR1A = int(ticks1);
    TCCR1B |= _BV(WGM12); // CTC mode
    switch (prescalar1){
        case 1:
          TCCR1B |= _BV(CS10);
          break;
        case 8:
          TCCR1B |= _BV(CS11);
          break;
        case 32:
          TCCR1B |= _BV(CS10) | _BV(CS11);
          break;
        case 64:
          TCCR1B |= _BV(CS12); 
          break;
        case 128:
          TCCR1B |= _BV(CS10) | _BV(CS12);
          break;
        case 256:
          TCCR1B |= _BV(CS11) | _BV(CS12);
          break;
        case 1024:
          TCCR1B |= _BV(CS10) | _BV(CS11) | _BV(CS12);
          break;
    }
    TIMSK1 &= ~_BV(OCIE1A); // Enable Timer1 Compare Match A Interrupt
  
    // Setup Timer2 for Motor 2 (8-bit timer)
    TCCR2A = 0; TCCR2B = 0; TIMSK2 = 0;
    OCR2A = int(ticks2);
    TCCR2A |= _BV(WGM21); // CTC mode
    switch (prescalar2) {
      case 1:
        TCCR2B = (TCCR2B & 0b11111000) | _BV(CS20);
        break;
      case 8:
        TCCR2B = (TCCR2B & 0b11111000) | _BV(CS21);
        break;
      case 32:
        TCCR2B = (TCCR2B & 0b11111000) | BV(CS20) | BV(CS21);
        break;
      case 64:
        TCCR2B = (TCCR2B & 0b11111000) | _BV(CS22);
        break;
      case 128:
        TCCR2B = (TCCR2B & 0b11111000) | _BV(CS20) | _BV(CS22);
        break;
      case 256:
        TCCR2B = (TCCR2B & 0b11111000) | _BV(CS21) | _BV(CS22);
        break;
      case 1024:
        TCCR2B = (TCCR2B & 0b11111000) | _BV(CS20) | _BV(CS21) | _BV(CS22);
        break;
    }
    TIMSK2 &= ~_BV(OCIE2A); // Enable Timer2 Compare Match A Interrupt
  
    
 
  sei();

  }