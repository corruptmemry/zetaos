#include <zetaos/io/IO.h>
#include <zetaos/userinput/kbScancodeTranslation.h>
#include <zetaos/video/tty.h>
#include <zetaos/userinput/keyboard.h>
#include <zetaos/mm/heap.h>
#include <zetaos/zetalib.h>
bool isLeftShiftPressed;
bool isRightShiftPressed;
char text;

void prompt(){
    Print(0x55FF55, "\n# ");
}

char *strappend(char *str, char chr)
{
    size_t len = strlen(str);
    
    str[len] = chr;
    str[len + 1] = '\0';
    
    return str;
}

void HandleKeyboard(uint8_t scancode){
    switch (scancode){
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
        case Enter:
            if(!strcmp(text, "hello")){ Print(0xffffff, "\nHello"); }
            memset(text, 0, strlen(text));
            prompt();
            return;
        case BackSpace:
            if(strlen(text) != 0){ ClearChar(); }
            return;
        case Spacebar:
            PutChar(' ');
            strappend(text, " ");
            return;
    }
    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);
    if(ascii != 0){
        PutChar(ascii);
        strappend(text, ascii);
    }
}