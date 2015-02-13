//
//  label.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "graph.h"

void Graph::create(int startx, int starty, string _ID)
{
    lenght = 300.0f;
    start.x = startx;
    start.y = starty;
    for (int i = 0; i < 7; i++)
    {
        points.push_back(ofPoint(0,0));
        drawPoints.push_back(ofPoint(0,0));
    }
    
    update(0,-lenght/2.0,30);
    update(1,-lenght/2.0+50, 30);
    update(2, -lenght/2.0 + 60, 180);
    update(3, lenght/2.0 - 60, 180);
    update(4, lenght/2.0 - 50, 30);
    update(5, lenght/2.0, 30);
    cout<<"Create"<<endl;

}

void Graph::update(int i, ofPoint point)
{
    points[i] = point;
}

void Graph::updateFarLeft(float val)
{
    update(1, val, points[1].y);
}

void Graph::updateNearLeft(float val)
{
    update(2, val, points[2].y);
}

void Graph::updateNearRight(float val)
{
    update(3, val, points[3].y);
}
void Graph::updateFarRight(float val)
{
    update(4, val, points[4].y);
}

void Graph::updateMax(float vMax)
{
    update(2, points[2].x, vMax);
    update(3, points[3].x, vMax);
}

void Graph::updateMin(float vMin)
{
    update(0, points[0].x, vMin);
    update(1, points[1].x, vMin);
    update(4, points[4].x, vMin);
    update(5, points[5].x, vMin);
}

void Graph::update(int i, int x, int y)
{
    points[i].x = x;
    points[i].y = y;
    drawPoints[i].x = (float)((points[i].x+ lenght/2.0f)/(lenght))*1000.0f ;
    drawPoints[i].y = 50 - (points[i].y/200.0f)*50.0f;
}

void Graph::initialize(string msg)
{
    vector<string> msgs = ofSplitString(msg, "_");
    
    if(msgs[0] == "vmax")
        updateMax(ofToFloat(msgs[1]));
    else if (msgs[0] == "vmin")
        updateMin(ofToFloat(msgs[1]));
}

void Graph::changeLenght(float _lenght)
{
    lenght = _lenght;
}

void Graph::draw()
{
    ofFill();
    ofSetColor(255,0,0);
    for (int i = 0; i < 5; i++)
    {
        ofCircle(start + drawPoints[i], 5);
    }
    ofSetColor(0,255,0);
    for (int i = 1; i < 6; i++)
    {
        ofLine(start + drawPoints[i], start + drawPoints[i-1]);
    }
    
    for (int i = 0; i < realTime.size(); i++)
    {
        ofSetColor(0,255,255);
        ofCircle(start + 50 - realTime[i], 2);
    }
    ofSetColor(255,255,255);
    ofNoFill();
    ofRect(start.x,start.y, 1000, 50);
    
}

void Graph::updatecurrent(float pos, float vel)
{
    float _vel = (vel/200.0f)*50.0f;
    if(_vel < 0)
        _vel *= -1;
    
    float _pos = pos * 1000;
    
    cout<<"speed "<<_vel<<endl;
    
    if(realTime.size() < 200)
        realTime.push_back(ofPoint(_pos, _vel));
    else
    {
        realTime.erase(realTime.begin()+1);
        realTime.push_back(ofPoint(_pos, _vel));
    }
}

