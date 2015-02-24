/*
 *  ofvUtils.h
 *  fernmatch
 *
 *  Created by Freire on 1/3/11.
 *  Copyright 2011 CoVii Lda. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"


static float ofLerpDegrees(float currentAngle, float targetAngle, float pct) {
	float diff = targetAngle - currentAngle;
	while(diff > 180.0f)  diff -= 360.0f;
	while(diff < -180.0f) diff += 360.0f;
	
	return currentAngle + diff * pct;
}

//------------------------------
//------------------------------
static float ofAngleDifferenceDegrees(float currentAngle, float targetAngle) {
	float diff = targetAngle - currentAngle;
	while(diff > 180.0f)  diff -= 360.0f;
	while(diff < -180.0f) diff += 360.0f;
	return diff;
}

//------------------------------
//------------------------------
static float ofAngleDifferenceRadians(float currentAngle, float targetAngle) {
	float diff = targetAngle - currentAngle;
	while(diff > PI)  diff -= TWO_PI;
	while(diff < -PI) diff += TWO_PI;
	return diff;
}

//------------------------------
//------------------------------
static float ofLerpRadians(float currentAngle, float targetAngle, float pct) {
	float diff = targetAngle - currentAngle;	
	while(diff > PI)  diff -= TWO_PI;
	while(diff < -PI) diff += TWO_PI;
	
	return currentAngle + diff * pct;
}
