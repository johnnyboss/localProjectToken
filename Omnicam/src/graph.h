//
//  label.h
//  openNiSample007
//
//  Created by Joao Freire on 6/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_graph_h
#define openNiSample007_graph_h

#include "ofMain.h"
#include "label.h"

class Graph
{
public:
    void create(int startx, int starty, string _ID);
    void draw();
    void update(int i, ofPoint point);
    void update(int i, int x, int y);
    void updateMax(float vMax);
    void updateMin(float vMin);
    void setAnchor(int x, int y);
    void changeLenght(float lenght);
    void updatecurrent(float pos, float vel);
    void updateFarLeft(float val);
    void updateNearLeft(float val);
    void updateNearRight(float val);
    void updateFarRight(float val);
    void initialize(string msg);
    
    vector <ofPoint> points;
    vector <ofPoint> drawPoints;
    vector <ofPoint> realTime;
    ofPoint start;
    string ID;
    float lenght;
    
};

#endif
