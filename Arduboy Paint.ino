
#include <Arduboy2.h>
Arduboy2 arduboy;

int x = 2;
int y = 2;
const unsigned int minX = 2;
const unsigned int maxX = 125;
const unsigned int minY = 2;
const unsigned int maxY = 47;
int memSize = 768;
unsigned char pic[768];
int crsr[9] = {1, 1, 1, 1, 0, 1, 1, 1, 1};

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.clear();
  addInstructions();
  load();
}

void loop()
{
  if (!(arduboy.nextFrame())) return;
  //arduboy.fillRect(3 ,3, 120, 44, 0);
  picToScreen();
  arduboy.drawRect(1, 1, 126, 48, 1);
  setCursor();
  arduboy.display();
}

void setCursor() {
  if (arduboy.pressed(LEFT_BUTTON)) x--;
  if (arduboy.pressed(RIGHT_BUTTON)) x++;
  if (arduboy.pressed(UP_BUTTON)) y--;
  if (arduboy.pressed(DOWN_BUTTON)) y++;

  x = min(max(x, minX), maxX);
  y = min(max(y, minY), maxY);
  
  if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(B_BUTTON)) wipe();
  if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)) save();

  if (arduboy.pressed(A_BUTTON)) paint(1);
  if (arduboy.pressed(B_BUTTON)) paint(0);

  drawCursor();
}

void picToScreen() {
  for (int i = 0; i < memSize; i++) {
    arduboy.sBuffer[i] = pic[i];
  }
}

void drawCursor() {
  int i = 0;
  for (int yy = -1; yy <= 1; yy++) {
    for (int xx = -1; xx <= 1; xx++) {
      arduboy.drawPixel(x + xx, y + yy, crsr[i]);
      i++;
    }
  }
}

void paint(int color) {
  int array = x % WIDTH + (WIDTH * floor(y / 8));
  int bitI = y % 8;
  pic[array] ^= (-color ^ pic[array]) & (1 << bitI);
}

void save() {
  for (int i = 0; i < memSize; i++) {
    EEPROM.write(i+16, pic[i]);
  }
}

void wipe() {
  for (int i = 0; i < memSize; i++) {
    pic[i] = 0x00;
  }
}

void load() {
  for (int i = 0; i < memSize; i++) {
    pic[i] = EEPROM.read(i+16);
  }
}

void addInstructions() {
  arduboy.setCursor(24, 49);
  arduboy.print(F("ARDUBOY PAINT"));
  arduboy.setCursor(3, 57);
  arduboy.print(F("A+B Save | UP+B Wipe"));
}

