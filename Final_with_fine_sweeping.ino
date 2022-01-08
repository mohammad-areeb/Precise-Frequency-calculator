
#define pinAttach 12
const byte interruptPin = 34;              // Assign the interrupt pin
volatile unsigned long StartValue;                     // First interrupt value
volatile int PeriodCount;
volatile int PeriodCount_array[4000];
int period[4000];

int start_sweep = 500;
int bandwidth = 500;
int fine_bandwidth = 100;

volatile int volatile_counter;
int coarse_counter = 0;
int fine_counter = 0;
int constant_counter = 0;
int sudden_counter = 0;

int freq_avg_counter = 0;

int channel = 0;
int resolution = 8;
int temp = 0;

float Freq[10];
int freq = 0;
float avg;
int frequ[10000];
int len = 4000;

int i = 0;
int j = 0;
int k = 0;

int max1, index1, max2, index2, max3, index3, max4, index4, max5, index5;


void IRAM_ATTR handleInterrupt()
{
  unsigned long TempVal = micros();
  PeriodCount = TempVal - StartValue;
  PeriodCount_array[volatile_counter++] = TempVal - StartValue;
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

void sort(volatile int a[], int s)
{
  for (int i = 0; i <= (s - 1); i++)
  {
    for (int j = 0; j <= (s - (i + 1)); j++)
    {
      if (a[j] > a[j + 1])
      { int t = a[j];
        a[j] = a[j + 1];
        a[j + 1] = t;
      }
    }
  }
}

void loop()
{
  //  Serial.println();
  //  Serial.println("coarse_counter val: "); Serial.print(coarse_counter);
  //  Serial.println();
  //  Serial.println("fine_counter val: "); Serial.print(fine_counter);

  Serial.println();
  //  temp = 0;
  ledcWrite(channel, 125);
  if (coarse_counter == 0) {
    for (i = start_sweep; i <= start_sweep + bandwidth; i++) {
      if (i < 5000) {
        ledcWriteTone(channel, i);
      }
      else {
        ESP.restart();
      }
    }
    Serial.println("Coarse sweeping");
    Serial.println();
    Serial.println(i);
    Serial.println();
  }

  else if (coarse_counter == 1) {
    for (i = start_sweep; i <= start_sweep + fine_bandwidth; i++) {
      ledcWriteTone(channel, i);
    }
    Serial.println("Fine sweeping");
    fine_counter = 1;
    Serial.println();
    Serial.println(i);
    Serial.println();
  }

  else if (constant_counter == 1) {
    ledcWriteTone(channel, freq);
    Serial.println("Constant");
    Serial.println();
  }

  delay(100);
  ledcWriteTone(channel, 0);
  delay(100);
  volatile_counter = 0;
  interrupts();
  delay(100);
  noInterrupts();


  for (i = 0; i <= volatile_counter; i++) {
    period[i] = PeriodCount_array[i];
  }
  sort(period, len);


  for (i = 0; i <= len; i++) {
    frequ[period[i]]++;
  }
  for (i = 0; i < 10000; i++) {                                                   // for calculating frequency of each element
    if (frequ[i])
      printf("Count of %d is %d\n", i, frequ[i]);
  }

  for (i = 0; i < 10000; i++)
  {
    if (frequ[i] > max1)
    { max4 = max3;
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

  if ( index2 > 0 && index3 > 0 && index4 > 0 && index1 == 0)
  {
    avg = (max2 * index2) + (max3 * index3) + (max4 * index4);
    avg = avg / (max2 + max3 + max4);
  }
  else if ( index3 > 0 && index2 > 0 && index1 == 0 && index4 == 0 )
  {
    avg = (max2 * index2) + (max3 * index3);
    avg = avg / (max2 + max3);
  }
  else if ( index1 > 0 && index4 > 0 && index1 == 0 && index3 == 0)
  {
    avg = (max2 * index2) + (max4 * index4);
    avg = avg / (max1 + max4);
  }
  else if ( index4 > 0 && index2 > 0 && index1 == 0 && index2 == 0)
  {
    avg = (max4 * index4) + (max3 * index3);
    avg = avg / (max4 + max3);
  }
  else if ( index1 == 0 && index2 == 0 && index4 == 0 && index3 == 0)
  {
    Serial.println("Improper Connection");
  }



  Serial.println();
  Serial.print("Time Period: "); Serial.print(avg);
  Serial.println();
  Serial.print("Frequency: ");  Serial.print(1000000 / avg);
  Serial.println();
  if (avg == 0) {
    if (coarse_counter == 0) {
      start_sweep = start_sweep + bandwidth;
    }
    else if (coarse_counter == 1 && fine_counter == 1) {
      start_sweep = start_sweep + fine_bandwidth;
    }
    else if (sudden_counter == 1 ) {
      Serial.println("Restarting.....");
      delay(100);
      ESP.restart();
    }

  }
  else if (avg > 0)  {
    if (fine_counter == 0) {
      coarse_counter = 1;
    }
    else if (fine_counter == 1) {
      sudden_counter = 1;
    }
  }

  if (freq_avg_counter < 10 && sudden_counter == 1 && avg > 0) {
    temp = temp + (1000000 / avg);
    freq_avg_counter++;
  }
  Serial.print("val: "); Serial.println(freq_avg_counter);
  if (freq_avg_counter == 9) {
    Serial.println(freq);
    Serial.println(temp);
    freq = temp / 9;
    constant_counter = 1;
    freq_avg_counter = 0;
    temp = 0;
  }
  Serial.println(freq);


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

  for (i = 0; i <= len; i++) {
    period[i] = 0;
  }


  Serial.println();
  Serial.println("-----------");
  delay(2500);
}
