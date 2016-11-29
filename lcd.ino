#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TEA5767.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
TEA5767 *tea = new TEA5767();

const int 		inPin = 44,
				pin_A = 46,
				pin_B = 48;
const float		minFreq = 87.5,
				maxFreq = 109;

byte 			current,
				previous = LOW;
int				click_count = 0;
float			currentFreq  = minFreq,
				lastFreq = currentFreq;
bool			started = false;
unsigned char   encoder_A_prev=0;
unsigned long	firstTime = 0,
				currentTime = 0,
				loopTime = 0,
				lastClick = 0;

void setFreq(double freq) {
	lcd.setCursor(0, 0);
	String freqLine = "FM " + String(freq, 1);

	tea->set_frequency(freq);
	lcd.print(freqLine);
}

void setup() {
  	lcd.init();
  	lcd.backlight();
	setFreq(currentFreq);
  	Serial.begin(9600);
  	pinMode(inPin, INPUT);
	pinMode(pin_A, INPUT);
	pinMode(pin_B, INPUT);
	currentTime = loopTime = millis();
}

void loop() {
	currentTime = millis();
  	current = digitalRead(inPin);

	if (currentTime >= (loopTime + 5)) {
    	char encoder_A = digitalRead(pin_A);
    	char encoder_B = digitalRead(pin_B);

    	if ((!encoder_A) && (encoder_A_prev)) {
      		if (encoder_B) {
				currentFreq += 0.1;
				if (currentFreq > maxFreq)
					currentFreq = maxFreq;
			} else {
				currentFreq -= 0.1;
				if (currentFreq < minFreq)
					currentFreq = minFreq;
			}
			setFreq(currentFreq);
		}
		encoder_A_prev = encoder_A;
	}

  	// if the button state changes to pressed, remember the start time
 	if (current == HIGH && previous == LOW) {
    	firstTime = millis();
		started = true;
  	}

  	long millis_held = (millis() - firstTime);

  	// This if statement is a basic debouncing tool, the button must be pushed for at least
  	// 50 milliseconds in a row for it to be considered as a push.
  	if (started && millis_held > 50) {
		// check if the button was released since we last checked
		if (current == LOW && previous == HIGH) {
		  	// button pressed for less than 500 milliseconds, then it's a click
		  	if (millis_held <= 500) {
				lastClick = millis();
				click_count += 1;
		 	}

		  	// button held for 600 milliseconds, then it's a hold
		  	if (millis_held >= 600) {
				lcd.setCursor(0, 1);
				lcd.print("hold    ");
		  	}

			started = false;
		}
	}

	// click actions, we give 300 milliseconds between clicks
	if (click_count && ((millis() - lastClick) > 300)) {
		if (click_count == 1) {
			lcd.setCursor(0, 1);
			lcd.print("1 click   ");
		}

		if (click_count == 2) {
			lcd.setCursor(0, 1);
			lcd.print("2 click   ");
		}

		click_count = 0;
	}

  	previous = current;
}
