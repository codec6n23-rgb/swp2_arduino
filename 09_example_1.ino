#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100
#define _DIST_MAX 300

#define _MEDIAN_N 30

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;
float samples[_MEDIAN_N];
int sample_count = 0;
int sample_index = 0;

float median_filter(float new_sample);

void setup() {

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(57600);
}

void loop() {
  float dist_raw, dist_median;

  if (millis() < last_sampling_time + INTERVAL)
    return;

  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  dist_median = median_filter(dist_raw);

  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(",raw:"); Serial.print(min(dist_raw, _DIST_MAX + 100));
  Serial.print(",median:"); Serial.print(min(dist_median, _DIST_MAX + 100));
  Serial.print(",Max:"); Serial.print(_DIST_MAX);
  Serial.println("");

  if ((dist_raw < _DIST_MIN) || (dist_raw > _DIST_MAX))
    digitalWrite(PIN_LED, 1);
  else
    digitalWrite(PIN_LED, 0);

  last_sampling_time += INTERVAL;
}

float median_filter(float new_sample)
{
  samples[sample_index] = new_sample;
  sample_index = (sample_index + 1) % _MEDIAN_N;

  if (sample_count < _MEDIAN_N)
    sample_count++;

  float sorted[_MEDIAN_N];

  for (int i = 0; i < sample_count; i++)
    sorted[i] = samples[i];

  for (int i = 1; i < sample_count; i++) {
    float key = sorted[i];
    int j = i - 1;

    while (j >= 0 && sorted[j] > key) {
      sorted[j + 1] = sorted[j];
      j--;
    }
    sorted[j + 1] = key;
  }

  if (sample_count % 2 == 1)
    return sorted[sample_count / 2];

  return (sorted[sample_count / 2 - 1] + sorted[sample_count / 2]) / 2.0;
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
