//  controller.hpp
//  interactiveMirror
//
//  Created by Zac Ioannidis on 3/19/16.
//
//  Used for grabbing data from the API


#ifndef controller_hpp
#define controller_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxJSON.h"

using namespace std;

class GSTController : public ofThread {
public:
    void setAccessTokens();
    void setDate(string _date);
    void setAggregateData();
    void getMetricsForDay();
    void start();
    void stop();
    void drawMetrics(int WIDTH, int HEIGHT);
    
    // the thread function
    void threadedFunction();
    
    const string USER_ID = "56ee6e2bed8de9a62ef25452";
    const string APP_URL = "http://localhost:3000/";
    string movesAccessToken;
    string fitbitAccessToken;
    string userID;
    string currentDate;
    
    // Metrics
    float walkingDuration;
    float walkingDistance;
    float transportDuration;
    float transportDistance;
    float overallDistance;
    float totalSteps;
    
    ofTrueTypeFont font;
    
    
    GSTController();
    
private:
    //    ofxJSONElement response;
    //    void setMovesAccessToken();
    //    void setFitbitAccessToken();
    
protected:
    // A flag to check and see if new data should be retrieved
    Poco::AtomicCounter shouldRequestNewData;
    
};

#endif /* controller_hpp */
