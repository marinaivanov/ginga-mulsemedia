/* 
    Author: Marina Ivanov
    Description: This class implements a presentation orchestrator for an NCL application.
    The orchestrator creates a presentation plan containing the instants which each transition should be occur during the
    execution of multimedia application.
*/

#ifndef PRESENTATION_ORCHESTRATOR_H
#define PRESENTATION_ORCHESTRATOR_H

#include "aux-ginga.h"
#include "TemporalGraph.h"
#include "Media.h"
#include "Event.h"
#include "Context.h"
#include "Effect.h"

#define PREPARATION_TIME 2 //time default to prepare a media object 
#define EFFECT_PREPARATION_TIME 1 //time default to prepare a media object
GINGA_NAMESPACE_BEGIN

/**
 * @brief Represents a item of the presentation plan or preparation plan
 */ 
typedef struct 
{
    float instant;        ///< Instant of transition occurrence
    Vertex* vertex;     ///< Vertex representing the transition in the event states machine
    string action;      //< Action to be prepared (starts or stop)
} PlanItem;

class PreparationOrchestrator
{
public:
    PreparationOrchestrator (list<PlanItem> , TemporalGraph*);
    float getPreparationDuration (Media* media);
    void createPreparationPlan (map<string, Device*>);
    void insertPreparationActions ();
private:
    TemporalGraph* htg;
    int getMediaSize (string );
    void printPreparationPlan();
    list<PlanItem> preparation_plan;
    list<PlanItem> presentation_plan;
    bool verifyMediaType (string , string );
    int getPreparationTime (string, string);
    map<string, Device*> _deviceList;
};

class PresentationOrchestrator
{
public:
    PresentationOrchestrator ();
    void depthFirstSearch (Vertex* );
    void createPresentationPlan (TemporalGraph*, map<string, Device*>);
    list<PlanItem> presentation_plan;       ///< List containing the presentation instant of each vertex.
    bool preparationEnabled;
private:
    float time;
    TemporalGraph* htg;                     ///< Temporal Graph that represents the multimedia application
    PreparationOrchestrator* pO;    
    map<string, Device*> _deviceList;      
};

GINGA_NAMESPACE_END
#endif // PRESENTATION_ORCHESTRATOR_H