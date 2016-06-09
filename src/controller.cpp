//  controller.cpp
//  interactiveMirror
//
//  Created by Zac Ioannidis on 3/19/16.
//
//  Used for grabbing data from the API

#include "controller.hpp"

GSTController::GSTController() {
    shouldRequestNewData = 0;
}

void GSTController::threadedFunction() {
    while(isThreadRunning()) {
        if (lock()) {
            if (shouldRequestNewData > 0) {
                shouldRequestNewData = 0;
                setAggregateData();
            }
            
            // Unlock the mutex
            unlock();
            // Sleep for one second
            // sleep(1000);
        }
        else {
            ofLogWarning("GSTController::threadedFunction()") << "Unable to lock mutex.";
        }
    }
}

void GSTController::start() {
    font.load("futura.ttf", 24);
    setDate("20160317");
    setAccessTokens();
    setAggregateData();
    shouldRequestNewData = 0;
    startThread(true);
}

void GSTController::stop() {
    stopThread();
}

void GSTController::setAccessTokens() {
    ofxJSONElement response;
    string url = APP_URL + "api/authenticate?user_id=";
    url = url + USER_ID;
    
    bool parsingSuccessful = response.open(url);
    
    if (parsingSuccessful) {
        for (int i=0; i<response["tokens"].size(); i++) {
            if (std::strcmp(response["tokens"][i]["kind"].asCString(), "moves") == 0) {
                movesAccessToken = response["tokens"][i]["accessToken"].asString();
            }
            
            if (std::strcmp(response["tokens"][i]["kind"].asCString(), "fitbit") == 0) {
                fitbitAccessToken = response["tokens"][i]["accessToken"].asString();
            }
        }
        
    }
    else {
        ofLogNotice("GSTController::setAccessToken") << "Failed to parse JSON";
    }
    
    return;
}

void GSTController::setDate(string _date) {
    if (lock()) {
        currentDate = _date;
        cout << endl << "Setting date";
        cout << endl << currentDate << endl;
        unlock();
    }
    
    return;
}

void GSTController::getMetricsForDay() {
    shouldRequestNewData = 1;
}

void GSTController::drawMetrics(int WIDTH, int HEIGHT) {
    ofColor(255, 255, 255);
    
    font.drawString(currentDate.substr(0,4), 20, HEIGHT+370);
    font.drawString("/" + currentDate.substr(4,2), 130, HEIGHT+370);
    font.drawString("/" + currentDate.substr(6,2), 205, HEIGHT+370);
    font.drawString(ofToString(overallDistance) + " KM", 20, HEIGHT+410);
    font.drawString(ofToString(totalSteps) + " STEPS", 20, HEIGHT+450);
}

void GSTController::setAggregateData() {
    ofxJSONElement response;
    cout << "Current date" << endl << currentDate << endl;
    if (currentDate.empty()) {
        cout << endl << "Date was empty" << endl;
        currentDate = "20160316";
    }
    
    string url;
    url += APP_URL + "api/moves/distance?date=" + currentDate + "&access_token=" + movesAccessToken;
    
    cout << "url is " << url << endl;
    
    bool parsingSuccessful = response.open(url);
    //    if (lock()) {
    if (parsingSuccessful) {
        walkingDuration = response["walking"]["duration"].asFloat();
        walkingDistance = response["walking"]["distance"].asFloat();
        transportDuration = response["transport"]["duration"].asFloat();
        transportDistance = response["transport"]["distance"].asFloat();
        overallDistance = response["distance"].asFloat();
        totalSteps = response["totalSteps"].asFloat();
        cout << "Parsed successfully" << endl;
        cout << "Total Steps: " << totalSteps << endl;
        cout << "Overal Distance: " << overallDistance << endl;
    }
    else {
        ofLogError("GSTController::getAggregateData") << "Can't parse response";
    }
    //        unlock();
    
    //    }
    //    else {
    //        ofLogNotice("GSTController::setAggregateData") << "Failed to lock";
    //    }
    
    return;
}









