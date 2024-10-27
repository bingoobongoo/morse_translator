#include <LiquidCrystal.h>

const unsigned long UNIT_TIME = 75;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const int RS_PIN = 2;
const int ENABLE_PIN = 3;
const int D4_PIN = 4;
const int D5_PIN = 5;
const int D6_PIN = 6;
const int D7_PIN = 7;
const int BUZZER_PIN = 13;
const int LED_PIN = 12;

LiquidCrystal lcd(RS_PIN, ENABLE_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RS_PIN, LOW);
  digitalWrite(ENABLE_PIN, LOW);
  digitalWrite(D4_PIN, LOW);
  digitalWrite(D5_PIN, LOW);
  digitalWrite(D6_PIN, LOW);
  digitalWrite(D7_PIN, LOW);

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.setCursor(0, 0);
}

void lcd_blink_disp() {
  for (int i=0; i<1; i++) {
    lcd.noDisplay();
    delay(2 * UNIT_TIME);
    lcd.display();
    delay(2 * UNIT_TIME);
  }
}

void lcd_clear_row(int row) {
  lcd.setCursor(0, row);
  for (int i=0; i<LCD_COLS; i++)
    lcd.print(' ');
} 

void send_morse(String morse_code) {
  for (int i=0; i<morse_code.length(); i++) {
    char c = morse_code[i];

    lcd.setCursor(i, 1);
    lcd.print(c);

    switch (c) {
      case '.': 
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);
        delay(UNIT_TIME);
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        break;
      case '-':
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);
        delay(3 * UNIT_TIME);
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        break;
      case ' ':
        delay(4 * UNIT_TIME);       
    }

    if (i == morse_code.length() - 1) {
      delay(3 * UNIT_TIME);
      lcd_clear_row(1);
    }
    else
      delay(UNIT_TIME);
  }
}

String char_to_morse(char c) {
  switch (c) {
    case 'A': return ".-";
    case 'B': return "-...";
    case 'C': return "-.-.";
    case 'D': return "-..";
    case 'E': return ".";
    case 'F': return "..-.";
    case 'G': return "--.";
    case 'H': return "....";
    case 'I': return "..";
    case 'J': return ".---";
    case 'K': return "-.-";
    case 'L': return ".-..";
    case 'M': return "--";
    case 'N': return "-.";
    case 'O': return "---";
    case 'P': return ".--.";
    case 'Q': return "--.-";
    case 'R': return ".-.";
    case 'S': return "...";
    case 'T': return "-";
    case 'U': return "..-";
    case 'V': return "...-";
    case 'W': return ".--";
    case 'X': return "-..-";
    case 'Y': return "-.--";
    case 'Z': return "--..";
    case ' ': return " ";
    default: return "";
  }
}

void read_msg(String text) {
  int top_cursor_col = 0;
  for (int i=0; i<text.length(); i++) {
    String morse_code = char_to_morse(text[i]);
    send_morse(morse_code);

    if (text[i] == ' ') {
      lcd_blink_disp();
      lcd_clear_row(0);
      lcd.setCursor(0, 0);
      top_cursor_col = 0;
    }
    else {
      lcd.setCursor(top_cursor_col, 0);
      lcd.print(text[i]);
      top_cursor_col++;
    }
  }
  lcd_blink_disp();
  lcd_clear_row(0);
  lcd.setCursor(0, 0);
}

void loop() {
  if (Serial.available() == 0) {}
  String msg = Serial.readString();
  msg.trim();
  msg.toUpperCase();
  read_msg(msg);

  for (int i=0; i<msg.length(); i++) {
    lcd.print(msg[i]);
    delay(50);
  }
}