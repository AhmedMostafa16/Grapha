/*
 *  Keyboard Shortcuts Configurations
 */

#include<Grapha.h>
#include<Command.h>
#include<Declaration.h>

extern bool MFLAG;    //defined in Mouse.cpp
extern Grapha  Gra;
extern Command Cmd;

void KeyboardShortcut(unsigned char key, int x, int y){
    switch(key){
        case 'q': /* Quit */
            Exit();
            break;
        case 27:  /* ESC */
            Gra.SetDrawMode();
            break;
        case 'u': /* Undo */
            Gra.Undo();
            break;
        case 'c': /* Color */
            Gra.SetDrawMode(MCOLORBARLINE);
            break;
        case 'r': /* Redo */
            Gra.Redo();
            break;
        case 'e': /* Erase */
            Gra.EraseLine();
            break;
        case 'p': /* Pixel Mode */
            Gra.PixelMode();
            break;
        case 's': /* Show Status */
            Gra.ShowStatus();
            break;
        case ':': /* Prompt Mode */
            Cmd.CommandMode();
            KeyboardMode();
            break;
        default:
            break;
    }
}


void KeyboardMode(void){
    if(Cmd.GetCommandFlag()) glutKeyboardFunc(InputKey);
    else glutKeyboardFunc(KeyboardShortcut);
}

