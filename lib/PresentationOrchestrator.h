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

#define PREPARATION_TIME 3 //time default to prepare a media object 
GINGA_NAMESPACE_BEGIN

/**
 * @brief Represents a item of the presentation plan or preparation plan
 */ 
typedef struct 
{
    float instant;        ///< Instant of transition occurrence
    Vertex* vertex;     ///< Vertex representing the transition in the event states machine
} PlanItem;

class PreparationOrchestrator
{
public:
    PreparationOrchestrator (list<PlanItem> , TemporalGraph*);
    float getPreparationDuration (Media* media);
    void createPreparationPlan ();
    void insertPreparationActions ();
private:
    TemporalGraph* htg;
    int getMediaSize (string );
    void printPreparationPlan();
    list<PlanItem> preparation_plan;
    list<PlanItem> presentation_plan;
    bool verifyMediaType (string , string );
};

class PresentationOrchestrator
{
public:
    PresentationOrchestrator ();
    void depthFirstSearch (Vertex* );
    void createPresentationPlan (TemporalGraph*);
    list<PlanItem> presentation_plan;       ///< List containing the presentation instant of each vertex.
private:
    float time;
    TemporalGraph* htg;                     ///< Temporal Graph that represents the multimedia application
    PreparationOrchestrator* pO;          
};

GINGA_NAMESPACE_END
#endif // PRESENTATION_ORCHESTRATOR_H