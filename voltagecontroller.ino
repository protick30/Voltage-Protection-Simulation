
#include <LiquidCrystal.h>

// Initialize the LCD with the same interface pins
LiquidCrystal lcd(14, 13, 12, 11, 10, 9, 8);

const int Relay_PIN = 7;
const int LED_PIN = 6;
const int Voltage_PIN = A0;

// Original calibration and phase shift values
const float Calibration = 187.0;
const float Phase_Shift = 1.7;

void setup()
{
  pinMode(Relay_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(Voltage_PIN, INPUT);

  lcd.begin(20, 4);

  lcd.setCursor(0, 0);
  lcd.print("  THE BRIGHT LIGHT ");

  lcd.setCursor(0, 1);
  lcd.print(" VOLTAGE PROTECTION ");
}

void loop()
{
  const int samples = 200;
  float sumSquares = 0;
  float voltageOffset = 0;

  // Find the DC offset of the AC waveform
  for (int i = 0; i < samples; i++)
  {
    voltageOffset += analogRead(Voltage_PIN);
    delayMicroseconds(200);
  }

  voltageOffset /= samples;

  // Calculate the RMS value of the AC waveform
  for (int i = 0; i < samples; i++)
  {
    float reading = analogRead(Voltage_PIN);
    float centeredReading = reading - voltageOffset;

    sumSquares += centeredReading * centeredReading;

    delayMicroseconds(200);
  }

  float adcRMS = sqrt(sumSquares / samples);

  // Convert ADC RMS reading to voltage using calibration
  float Voltage = (adcRMS * Calibration) / 1023.0;

  // Display voltage
  lcd.setCursor(0, 2);
  lcd.print("   VOLTAGE = ");
  lcd.print(Voltage, 0);
  lcd.print("V         ");

  // Normal voltage condition
  if (Voltage >= 180 && Voltage <= 250)
  {
    digitalWrite(Relay_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);

    lcd.setCursor(0, 3);
    lcd.print("   NORMAL VOLTAGE   ");
  }

  // Overvoltage condition
  if (Voltage > 250)
  {
    digitalWrite(Relay_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);

    lcd.setCursor(0, 3);
    lcd.print("    OVER VOLTAGE    ");
  }

  // Undervoltage condition
  if (Voltage < 180)
  {
    digitalWrite(Relay_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);

    lcd.setCursor(0, 3);
    lcd.print("    UNDER VOLTAGE   ");
  }

  delay(200);
}
