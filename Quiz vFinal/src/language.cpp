#include "language.h"

void language::create()
{
    info.loadFont("micross.ttf", 12);
    setup(0, 0, ofGetWidth(), ofGetHeight());
    button aux;
    aux.create(ofGetWidth()/2 - 300 , panelArea.y + 500,200,200, "portuguese", false);
    buttons.push_back(aux);//0
    aux.create(buttons[0].buttonarea.x + 600, panelArea.y + 500,200,200, "english", false);
    buttons.push_back(aux);//1

    setActive(false);
    lang = 2;

}

void language::dealwithtouches(int x, int y, int button)
{
    if(isActive())
    {
        Panel::dealwithtouches(x,y,button);
        for (int i = 0; i < buttons.size(); i++)
        {
            if(buttons[i].isClicked(x,y))
            {
                lang = i;
            }
        }
    }
}

void language::draw()
{
    if(isActive())
    {
        for(int i = 0; i < buttons.size(); i++)
        {
            ofRect(buttons[i].buttonarea);
        }
    }
}
