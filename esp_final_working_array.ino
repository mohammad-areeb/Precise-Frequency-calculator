
#define pinAttach 12
const byte interruptPin = 34;              // Assign the interrupt pin
volatile unsigned long StartValue;                     // First interrupt value
volatile int PeriodCount;                    // period in counts of 0.000001 of a second
volatile int PeriodCount_array[2000];
int period[2000];
volatile int counter2;
float Freq;                           // frequency
// for printing uint64_t values
int freq = 2843;
int channel = 0;
int resolution = 8;
int temp;
float avg;
int i = 0;
int j = 0;
int k = 0;
int len = 2000;
int frequ[10000];
int max1, index1, max2, index2, max3, index3, max4, index4, max5, index5;

hw_timer_t * timer = NULL;                        // pointer to a variable of type hw_timer_t
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;  // synchs between maon cose and interrupt?


void IRAM_ATTR handleInterrupt()
{

  unsigned long TempVal = micros();
  PeriodCount = TempVal - StartValue;
  PeriodCount_array[counter2++] = TempVal - StartValue;

  StartValue = TempVal;
}


void setup()
{
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(pinAttach, channel);
  ledcWrite(channel, 125);
  pinMode(interruptPin, INPUT_PULLUP);                                            // sets pin high
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING); // attaches pin to interrupt on Falling Edge

  noInterrupts();
}

void sort(volatile int a[], int s) {
  for (int i = 0; i <= (s - 1); i++) {
    for (int j = 0; j <= (s - (i + 1)); j++) {
      if (a[j] > a[j + 1]) {
        int t = a[j];
        a[j] = a[j + 1];
        a[j + 1] = t;
      }
    }
  }
}

void loop()
{
  Serial.println();

  ledcWrite(channel, 125);
  //    for (i = 2450; i <= 2500; i++) {
  //      ledcWriteTone(channel, i);
  //    }

  ledcWriteTone(channel, freq);
  delay(100);
  ledcWriteTone(channel, 0);
  delay(100);
  counter2 = 0;
  interrupts();

  delay(100);
  noInterrupts();


  for (i = 0; i <= counter2; i++) {
    period[i] = PeriodCount_array[i];
  }
  sort(period, len);

  //  Serial.println();
  //  for (i = 0; i <= len; i++) {
  //    Serial.println(period[i]);
  //    //delay(1);
  //  }

  //  for (i = 0; i <= len; i++) {
  //    if (period[i] > 0) {
  //      periodFin[k] = period[i];
  //      k++;
  //    }
  //    //delay(1);
  //
  //  }



  for (i = 0; i <= len; i++) {
    frequ[period[i]]++;
  }
  for (i = 0; i < 10000; i++) {                                                   // for calculating frequency of each element
    if (frequ[i])
      printf("Count of %d is %d\n", i, frequ[i]);
  }


  Serial.println(frequ[352]);
  for (i = 0; i < 10000; i++)
  {
    if (frequ[i] > max1)
    { max5 = max4;
      max4 = max3;
      max3 = max2;
      max2 = max1;
      max1 = frequ[i];
      index1 = i;
    }
    else if (frequ[i] > max2 && frequ[i] < max1)
    { max2 = frequ[i];
      index2 = i;                                      //for slecting the highest freuency elements

    }
    else if (frequ[i] > max3 && frequ[i] < max1 && frequ[i] < max2)
    { max3 = frequ[i];
      index3 = i;
    }
    else if (frequ[i] > max4 && frequ[i] < max1 && frequ[i] < max2 && frequ[i] < max3)
    { max4 = frequ[i];
      index4 = i;
    }
    else if (frequ[i] > max5 && frequ[i] < max1 && frequ[i] < max2 && frequ[i] < max3 && frequ[i] < max4)
    { max5 = frequ[i];
      index5 = i;
    }
  }

  Serial.println();
  Serial.println(max1);
  Serial.println(index1);
  Serial.println(max2);
  Serial.println(index2);
  Serial.println(max3);
  Serial.println(index3);
  Serial.println(max4);
  Serial.println(index4);
  Serial.println(max5);
  Serial.println(index5);


  if ( index1 > 0 && index2 > 0 && index3 > 0 && index4 > 0)
  {
    avg = (max1 * index1) + (max2 * index2) + (max3 * index3) + (max4 * index4);
    avg = avg / (max1 + max2 + max3 + max4);
  }
  else if ( index2 > 0 && index3 > 0 && index4 > 0 && index5 > 0 && index1 == 0)
  {
    avg = (max2 * index2) + (max3 * index3) + (max4 * index4) + (max5 * index5);
    avg = avg / (max2 + max3 + max4 + max5);
  }
  else if ( index1 > 0 && index3 > 0 && index2 == 0  && index4 == 0)
  {
    avg = (max2 * index2) + (max3 * index3) + (max4 * index4);
    avg = avg / (max2 + max3 + max4);
  }
  else if ( index1 > 0 && index3 > 0 && index4 > 0 && index2 == 0 )
  {
    avg = (max1 * index1) + (max3 * index3) + (max4 * index4) + (352 * max2) ;
    avg = avg / (max1 + max2 + max3 + max4);                                                                  // Conditions for neglecting the bug or skipping one condition of the code
  }
  else if ( index1 > 0 && index3 > 0 && index2 > 0 && index4 == 0 )
  {
    avg = (max1 * index1) + (max3 * index3) + (max2 * index2);
    avg = avg / (max1 + max3 + max2);
  }
  else if ( index1 > 0 && index2 > 0 && index4 > 0 && index3 == 0 )
  {
    avg = (max1 * index1) + (max2 * index2) + (max4 * index4);
    avg = avg / (max1 + max2 + max4);
  }
  else if ( index1 > 0 && index4 > 0 && index2 == 0 && index3 == 0)
  {
    avg = (max1 * index1) + (max4 * index4);
    avg = avg / (max1 + max4);
  }
  else if ( index1 > 0 && index2 > 0 && index4 == 0 && index3 == 0)
  {
    avg = (max1 * index1) + (max2 * index2);
    avg = avg / (max1 + max2);
  }
  else if ( index1 == 0 && index2 == 0 && index4 == 0 && index3 == 0)
  {
    Serial.println("Improper Connection");
  }



  Serial.println();
  Serial.println(avg);
  Serial.println();
  Serial.println(1000000 / avg);
  Serial.println("-----------");

  // reinitialiasing all the variables

  for (i = 0; i <= 10000; i++) {
    frequ[i] = 0;
  }

  max1 = 0;
  index1 = 0;
  max2 = 0;
  index2 = 0;
  max3 = 0;
  index3 = 0;
  max4 = 0;
  index4 = 0;
  max5 = 0;
  index5 = 0;



  period[2000] = {0};
  for (i = 0; i <= len; i++) {
    period[i] = 0;
  }


  delay(3000);
}
