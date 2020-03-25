/* 
    Author: Marina Ivanov
    Description: This class implements a hypermedia temporal graph that models an NCL application.
    The Ginga presentation data structure, called Hypemedia Temporal Graph (HTG), is composed of vertices,
    which represent actions (for state changes) performed on event state machines, and directed edges
    that represent relationships among actions, derived from the document specification in NCL. 
    An edge is labeled by a condition that must be satisfied in order to trigger the action specified 
    in the edge’s output vertice.
*/

#ifndef TEMPORAL_GRAPH_H
#define TEMPORAL_GRAPH_H

#include "aux-ginga.h"
#include "Event.h"
#include "Object.h"
#include <stdlib.h>

GINGA_NAMESPACE_BEGIN
class Vertex;

class Edge
{

public:
    int priority;                    ///< Edge priority.
    Vertex* neighbor;                     ///< Neighbor vertex id.
    
    Edge () {};
    Edge (Vertex* );
    Edge (Vertex* v , int );
    void insertCondition (string , string );
    void removeCondition (string );
    map<string, string> getConditionList ();
    string getEdgeAsString ();
    int getDurationEdge();
    
private:
    map<string, string> conditions; ///< Edge conditions.

};


class Vertex
{

public:
    bool visited;                   ///< Flag to indicate if the vertex has been visited
    Vertex (string );
    Vertex (string, Event::Transition, Event::Type, Object*, string );
    ~Vertex ();
    void insertEdge (Edge* );
    void removeEdge ();
    string getId ();
    Event::Transition getTransition ();
    Event::Type getEventType ();
    Object* getObject ();
    list<Edge*> getEdgeList ();
    void edgesCleanup ();
    void deleteEdge ();
    size_t sizeEdgeList ();
    string getVertexAsString ();
    string getElementName ();
    Edge* getFirstEdge ();

private:
    Event::Transition transition;   ///< Transition on event state machines.
    Event::Type type;               ///< Event type.
    Object* object;                 ///< Target object.
    string id;                      ///< Vertex id.
    list<Edge*> edges;              ///< List of edges.
    string element_name;            ///< Element represented in the vertex (media, area, switch or context)
    
};

class TemporalGraph
{

public:
    TemporalGraph ();
    int getNumberVertices (); 
    Vertex* createVertex (string, Event::Transition, Event::Type, Object*, string );
    Vertex* createVerticesByMedia (string, Event::Transition, Event::Type, Object*);
    void insertVertex (Vertex* );
    void deleteVertex ();
    list<Vertex*> getVertexList ();
    void graphCleanup ();
    size_t sizeVertexList ();
    Vertex* findVertex (string ,Event::Transition , Event::Type );
    void removeUVertices ();  
    void printGraph ();  
    bool createEdge (Vertex* , Vertex* , string , string );   
    bool createEdge (Vertex* , Vertex* );     
    void addRelations (list<Action>, list<Action>);  
    Vertex* start_vertex;           ///< Vertex that represents the start of application.
    
private:
    int num_vertices;               ///< Number of vertices.
    list<Vertex*> v ;               ///< List of vertices of the graph.
    Vertex* getFirstVertex ();   
    string getIdByEvent ();   
    int getObjectDuration(string ); 
};

GINGA_NAMESPACE_END

#endif // TEMPORAL_GRAPH_H