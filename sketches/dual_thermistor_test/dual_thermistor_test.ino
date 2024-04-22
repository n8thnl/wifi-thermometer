const int ADC_PIN_GLASS = 34;
const int ADC_PIN_BEAD = 12;

float KNOWN_RESISTANCE = 7500;
float TEMP_REFERENCE = 25 + 273.15;

int B_GLASS = 3892;
int B_BEAD = 3380;

int global_count = 0;

float v_at_analog_pin(int pin_num) {
  int raw = analogRead(pin_num);
  return raw * (3.3 / 4095.0);
}

float find_unknown_R_given_drop(float voltage_drop, int known_resistance) {
  return known_resistance * (3.3 / voltage_drop - 1);
}

float temp_K_from_R_10k(int B, float resistance) {
  return B / (log(resistance / (10000.0 * exp(-1 * B / TEMP_REFERENCE))));
}

float temp_K_to_C(float temp_K) {
  return temp_K - 273.15;
}

float temp_C_to_F(float temp_C) {
  return (temp_C * 9 / 5) + 32;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  float v_glass = v_at_analog_pin(ADC_PIN_GLASS);
  float v_bead  = v_at_analog_pin(ADC_PIN_BEAD);

  // Serial.println("v_glass: " + String(v_glass));
  // Serial.println("v_bead: " + String(v_bead));

  float r_glass = find_unknown_R_given_drop(v_glass, KNOWN_RESISTANCE);
  float r_bead  = find_unknown_R_given_drop(v_bead,  KNOWN_RESISTANCE);

  // Serial.println("r_glass: " + String(r_glass));
  // Serial.println("r_bead: " + String(r_bead));

  float temp_C_glass = temp_K_to_C(temp_K_from_R_10k(B_GLASS, r_glass));
  float temp_C_bead  = temp_K_to_C(temp_K_from_R_10k(B_BEAD,  r_bead));

  float temp_F_glass = temp_C_to_F(temp_C_glass);
  float temp_F_bead  = temp_C_to_F(temp_C_bead);

  Serial.println("glass temp F: " + String(temp_F_glass));
  Serial.println("bead temp F: " + String(temp_F_bead));
  Serial.println("");

  delay(500);
}
