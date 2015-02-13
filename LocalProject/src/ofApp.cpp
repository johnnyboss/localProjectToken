#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
    message = "loading mySettings.xml";
    win_str = "Congratulations!\n Your constituents general happiness\n and safety leves are much above country average";
    loose_str = "Sorry!\n Your public health, safety and \n happiness are inadmissible.\n You should take better care of your constituents!";
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
    
    yes_button.create(ofGetWidth()/2 - 150, 200);
    no_button.create(ofGetWidth()/2 + 30, 200);
	if( XMLToken.loadFile("tokens.xml") )
    {
		message = "tokens.xml loaded!";
	}
    else
    {
		message = "unable to load tokens.xml check data/ folder";
	}
    
    if( XMLQuestion.loadFile("questions.xml") )
    {
        message = "questions.xml loaded!";
    }
    else
    {
        message = "unable to load questions.xml check data/ folder";
    }
    
    int nTokens = XMLToken.getNumTags("TOKEN");
    int nQuestions = XMLQuestion.getNumTags("QUESTION");
    
    for(int i = 0; i < nTokens; i++)
    {
        token aux;
        aux.name = XMLToken.getValue("TOKEN:NAME","",i);
        aux.ref = XMLToken.getValue("TOKEN:REF", "",i);
        aux.value = XMLToken.getValue("TOKEN:VALUE", 0,i);
        tokens.push_back(aux);
    }
    
    for(int i = 0; i < nQuestions; i++)
    {
        XMLQuestion.pushTag("QUESTION",i);
        question aux;
        aux.short_name = XMLQuestion.getValue("SHORT_NAME", "");
        aux.text = XMLQuestion.getValue("TEXT", "");
        int _nrel = XMLQuestion.getNumTags("RELATIONS");
        
        for(int j = 0; j < _nrel; j++)
        {
            struct relation aux2;
            aux2.short_name= XMLQuestion.getValue("RELATIONS:T_REF", "",j);
            aux2.yes_val = XMLQuestion.getValue("RELATIONS:YES", 0,j);
            aux2.no_val = XMLQuestion.getValue("RELATIONS:NO", 0,j);
            aux.relations.push_back(aux2);
        }
        questions.push_back(aux);
        XMLQuestion.popTag();
    }

    for(int i = 0; i < tokens.size(); i++)
    {
        ofRectangle aux;
        aux.x = ofGetWidth()/2 - 100;
        aux.y = ofGetHeight() - 40 * (tokens.size() - i)-30;
        aux.width = 200;
        aux.height = 30;
        tokenArea.push_back(aux);
    }
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 20);
    token_descriptor.loadFont("mono.ttf", 10);
}

int ofApp::getTokenIndexByName(string ref)
{
    for(int i = 0; i < tokens.size(); i++)
    {
        if(tokens[i].ref == ref)
            return i;
    }
    return -1;
}

//--------------------------------------------------------------
void ofApp::update()
{
	//we change the background color here based on the values
	//affected by the mouse position
    int health_token = tokens[getTokenIndexByName("health")].value;
    int happy_token = tokens[getTokenIndexByName("happy")].value;
    int safety_token = tokens[getTokenIndexByName("safety")].value;
    int income_token = tokens[getTokenIndexByName("tax")].value;
    int education_token = tokens[getTokenIndexByName("education")].value;
    if(happy_token > 10 || income_token > 8)
        game_status = 1;
    
    if(happy_token < -4 || safety_token < -8 || health_token < -5 || education_token < -8)
        game_status = 2;
}

void ofApp::drawTokenArea(int _ID)
{
    int x = ofGetWidth()/2 + tokens[_ID].value * 5 ;
    int y = tokenArea[_ID].y;
    ofSetColor(120,80,0);
    ofSetLineWidth(1);
    ofRectRounded(tokenArea[_ID],5);
    ofSetColor(200,200,200);
    ofSetLineWidth(5);
    ofLine(ofGetWidth()/2, y,ofGetWidth()/2,y+tokenArea[_ID].height);
    ofSetColor(0,0,0);
    ofCircle(x,y +15 ,10);
    token_descriptor.drawString(tokens[_ID].name, ofGetWidth()/2 + 105, y+10 );
    token_descriptor.drawString(ofToString(tokens[_ID].value), ofGetWidth()/2 + 105, y + 25);
    
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(200,200,200);
    for(int i = 0; i < tokens.size(); i++)
    {
        drawTokenArea(i);
    }
    if(currentQuestion < questions.size() && game_status == 0)
    {
        ofSetColor(0,255,0,122);
        yes_button.draw();
        ofSetColor(255,0,0,122);
        no_button.draw();
        ofSetColor(0,20,120,200);
        int question_start = ofGetWidth()/2 - TTF.getSize()* questions[currentQuestion].text.size()/2 * 0.8;
        TTF.drawString(questions[currentQuestion].text, question_start, 150);
    }
    else if(game_status == 1)
        TTF.drawString(win_str, 30,  150);
    else if(game_status == 2)
        TTF.drawString(loose_str, 30,  150);
 
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    if(currentQuestion < questions.size())
    {
        if(yes_button.bound.inside(x, y))
        {
            for(int i = 0; i < questions[currentQuestion].relations.size();i++)
            {
                int curr_token = getTokenIndexByName(questions[currentQuestion].relations[i].short_name);
                tokens[curr_token].change_val(questions[currentQuestion].relations[i].yes_val);
            }
            currentQuestion++;
            
        }
        else if(no_button.bound.inside(x, y))
        {
            for(int i = 0; i < questions[currentQuestion].relations.size();i++)
            {
                int curr_token = getTokenIndexByName(questions[currentQuestion].relations[i].short_name);
                tokens[curr_token].change_val(questions[currentQuestion].relations[i].no_val);
            }
            currentQuestion++;
        }
    }
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

