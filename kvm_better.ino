#include <Keyboard.h>

#define SEQ_MAX 5

void setup() {
    Serial.begin(115200);
    Keyboard.begin();
}

unsigned long last = 0;
uint8_t pos = 0;
char seq[SEQ_MAX];

void reset_seq() {
    for(int i = 0; i < SEQ_MAX; i++) {
        seq[i] = 0;
    }
    pos = 0;
}

void print_char(char c) {
  if(c >= ' ' && c <= '~') Serial.print(c);
  else {
    Serial.print("\\x");
    Serial.print(c, HEX);
  }
}

void send_simple_char(char c) {
  switch(c) {
    case '\r':
    case '\n':
      Keyboard.press(KEY_RETURN);
      Keyboard.release(KEY_RETURN);
      break;
    case '\x7f':
      Keyboard.press(KEY_BACKSPACE);
      Keyboard.release(KEY_BACKSPACE);
      break;
    case '\x3':
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('c');
      Keyboard.releaseAll();
      break;
    case '\x4':
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('d');
      Keyboard.releaseAll();
      break;
    case '\x1':
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('a');
      Keyboard.releaseAll();
      break;
    case '\x1b':
      Keyboard.press(KEY_ESC);
      Keyboard.release(KEY_ESC);
      break;
    case '\x5':
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('e');
      Keyboard.releaseAll();
      break;
    default:
      Keyboard.write(c);
      break;
  }
}

void loop() {
    if (pos > 0 && millis() - last > 5) {
      // detect, dump and reset sequence
      if(pos == 1) {
        Serial.println("sending simple char");
        print_char(seq[0]);
        Serial.println();
        send_simple_char(seq[0]);
      } else {
        for(int i = 0; i < pos; i++) {
          print_char(seq[i]);
          Serial.print(" ");
        }
        Serial.println();
        switch (seq[0]) {
          case '\x1b':
            switch(seq[1]) {
              case '[':
                switch(seq[2]) {
                  case 'A':
                    Keyboard.press(KEY_UP_ARROW);
                    Keyboard.release(KEY_UP_ARROW);
                    break;
                  case 'B':
                    Keyboard.press(KEY_DOWN_ARROW);
                    Keyboard.release(KEY_DOWN_ARROW);
                    break;
                  case 'C':
                    Keyboard.press(KEY_RIGHT_ARROW);
                    Keyboard.release(KEY_RIGHT_ARROW);
                    break;
                  case 'D':
                    Keyboard.press(KEY_LEFT_ARROW);
                    Keyboard.release(KEY_LEFT_ARROW);
                    break;
                  case 'H':
                    Keyboard.press(KEY_HOME);
                    Keyboard.release(KEY_HOME);
                    break;
                  case 'F':
                    Keyboard.press(KEY_END);
                    Keyboard.release(KEY_END);
                    break;
                  case '3':
                    if(seq[3] == '~') {
                      Keyboard.press(KEY_DELETE);
                      Keyboard.release(KEY_DELETE);
                    }
                    break;
                  case '1':
                    if(seq[3] != '~') break;
                    switch(seq[3]) {
                      case '5':
                        Keyboard.press(KEY_F5);
                        Keyboard.release(KEY_F5);
                        break;
                      case '7':
                        Keyboard.press(KEY_F6);
                        Keyboard.release(KEY_F6);
                        break;
                      case '8':
                        Keyboard.press(KEY_F7);
                        Keyboard.release(KEY_F7);
                        break;
                      case '9':
                        Keyboard.press(KEY_F8);
                        Keyboard.release(KEY_F8);
                        break;
                    }
                    break;
                  case '2':
                    if(seq[3] != '~') break;
                    switch(seq[3]) {
                      case '0':
                        Keyboard.press(KEY_F9);
                        Keyboard.release(KEY_F9);
                        break;
                      case '1':
                        Keyboard.press(KEY_F10);
                        Keyboard.release(KEY_F10);
                        break;
                      case '3':
                        Keyboard.press(KEY_F11);
                        Keyboard.release(KEY_F11);
                        break;
                      case '4':
                        Keyboard.press(KEY_F12);
                        Keyboard.release(KEY_F12);
                        break;
                    }
                    break;
                }
                break;
              case 'O':
                switch(seq[2]) {
                  case 'P':
                    Keyboard.press(KEY_F1);
                    Keyboard.release(KEY_F1);
                    break;
                  case 'Q':
                    Keyboard.press(KEY_F2);
                    Keyboard.release(KEY_F2);
                    break;
                  case 'R':
                    Keyboard.press(KEY_F3);
                    Keyboard.release(KEY_F3);
                    break;
                  case 'S':
                    Keyboard.press(KEY_F4);
                    Keyboard.release(KEY_F4);
                    break;
                }
                break;
              default:
                if(pos == 2) {
                  Keyboard.press(KEY_LEFT_ALT);
                  send_simple_char(seq[1]);
                  Keyboard.release(KEY_LEFT_ALT);
                }
                break;
            }
            break;
          case '\x18':
            switch(seq[1]) {
              case '@':
                switch(seq[2]) {
                  case 's':
                    Keyboard.press(KEY_LEFT_GUI);
                    Keyboard.press(seq[3]);
                    Keyboard.releaseAll();
                    break;
                }
                break;
            }
            break;
        }
      }

      reset_seq();
    }

    if (Serial.available() > 0) {
        char c = Serial.read();
        if (pos == 0 || millis() - last < 5) {
            if(pos == SEQ_MAX) {
	    	Serial.println("==> Warning more than SEQ_LIMIT characters sent!");
                reset_seq();
                return;
            }
            seq[pos++] = c;
            last = millis();
            return;
        }
    }
}

