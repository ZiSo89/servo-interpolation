#include <Servo.h>
Servo myservo;
#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x))) //SMOOTHSTEP expression.
int buffer[255];
int last_val;
void setup() {
  Serial.begin(9600);  //establish serial connection for debugging
  int steps_len = interpolate(0, 180, 0.80, buffer);

  for (int i = 0; i < steps_len; i++) {
    Serial.print(i); Serial.print(" ");
    Serial.println(buffer[i]);
  }
  myservo.attach(9);
}

void loop()
{
  int  val = analogRead(A0);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);
  if (val != last_val) {
    int steps_len = interpolate(last_val, val, 0.20, buffer);
    for (int i = 0; i < steps_len; i++) {
      Serial.print(last_val); Serial.print(" ");
      Serial.println(buffer[i]);
      myservo.write(buffer[i]);
    }

    last_val = val;
  }
}

int interpolate(int min, int max, float steps_pct, int to_smooth[]) {
  int diff;
  if (min > max) {
    diff = min - max;
  } else {
    diff = max - min;
  }
  float steps = float(diff) * steps_pct;
  Serial.print("pct "); Serial.print(steps_pct * 100, 1); Serial.print("% ");
  Serial.print("steps "); Serial.println(steps);

  int j = 0;              //Just an Iterator.
  int i = 0;              //Just another Iterator.
  float A = float(min);   //Input Min Value
  float B = float(max);   //Input Max Value
  float N = steps;        //Input number of steps for transition
  float X;                //final smoothstepped value
  float v;                //smoothstep expression variable

  while (j < N) {
    v = j / N;                    // Iteration divided by the number of steps.
    v = SMOOTHSTEP(v);            // Run the smoothstep expression on v.
    X = (B * v) + (A * (1 - v));  // Run the linear interpolation expression using the current
    to_smooth[j] = int(X);
    j++;                          // Increments j by 1.
  }
  return int(steps);
}




