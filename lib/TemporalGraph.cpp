#include "TemporalGraph.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Media.h"

GINGA_NAMESPACE_BEGIN

static map<string, string> mime_table = {
  {"ac3", "audio/ac3"},
  {"avi", "video/x-msvideo"},
  {"bmp", "image/bmp"},
  {"bpg", "image/x-bpg"},
  {"class", "application/x-ginga-NCLet"},
  {"css", "text/css"},
  {"gif", "image/gif"},
  {"htm", "text/html"},
  {"html", "text/html"},
  {"jpeg", "image/jpeg"},
  {"jpg", "image/jpeg"},
  {"lua", "application/x-ginga-NCLua"},
  {"mov", "video/quicktime"},
  {"mp2", "audio/mp2"},
  {"mp3", "audio/mp3"},
  {"mp4", "video/mp4"},
  {"mpa", "audio/mpa"},
  {"mpeg", "video/mpeg"},
  {"mpg", "video/mpeg"},
  {"mpv", "video/mpv"},
  {"ncl", "application/x-ginga-ncl"},
  {"oga", "audio/ogg"},
  {"ogg", "audio/ogg"},
  {"ogv", "video/ogg"},
  {"opus", "audio/ogg"},
  {"png", "image/png"},
  {"smil", "application/smil"},
  {"spx", "audio/ogg"},
  {"srt", "text/srt"},
  {"ssml", "application/ssml+xml"},
  {"svg", "image/svg+xml"},
  {"svgz", "image/svg+xml"},
  {"ts", "video/mpeg"},
  {"txt", "text/plain"},
  {"wav", "audio/basic"},
  {"webp", "image/x-webp"},
  {"wmv", "video/x-ms-wmv"},
  {"xlet", "application/x-ginga-NCLet"},
  {"xlt", "application/x-ginga-NCLet"},
  {"xml", "text/xml"},
};

static bool
mime_table_index (const string &key, string *result)
{
  map<string, string>::iterator it;
  if ((it = mime_table.find (key)) == mime_table.end ())
    return false;
  tryset (result, it->second);
  return true;
}

// Public
/**
 * @brief Constructor.
 * @param _neighbor Destination vertex of the edge.
 */
Edge::Edge (Vertex* _neighbor)
{
    neighbor = _neighbor;
    priority = 0;
}

/**
 * @brief Constructor.
 * @param _neighbor Destination vertex of the edge.
 * @param _priority Priority to verify an edge, considering all edges that have the same source vertex.
 */
Edge::Edge (Vertex* _neighbor, int _priority)
{
    neighbor = _neighbor;
    priority = _priority;
}

/**
 * @brief Inserts a condition to the edge.
 * @param name Variable name.
 * @param value Test value.
 */
void Edge::insertCondition (string name, string value)
{
    conditions[name] = value;
    //conditions.insert ( pair<string,string>(name,value) );
}

/**
 * @brief Removes a condition to the edge.
 * @param name Variable name.
 */
void Edge::removeCondition (string name)
{
    map<string,string>::iterator it;
    it = conditions.find(name);
    conditions.erase (it);  
}

/**
 * @brief Get the list of conditions.
 * @return Conditions list.
 */
map<string, string> Edge::getConditionList ()
{
    return conditions;
}

/**
 * @brief Transforms an edge into string.
 * @return String representing the edge.
 */
string Edge::getEdgeAsString ()
{
    string _edge = "    |----(";
    if(conditions.empty())
    {
        _edge = _edge + "0";
    }
    else
    {
        map<string, string>::iterator it;
	    for(it = conditions.begin(); it != conditions.end(); it++)
        {
            if( it != conditions.begin() )
                _edge = _edge + ",";
            
            _edge += it->first + " = " + it->second;
        }
    }
    _edge += ") ----> " + neighbor->getVertexAsString() ;
    return _edge;
}

int Edge::getDurationEdge()
{
    map<string, string>::iterator it;
    int time = 0;

    it = conditions.find("Duration");
    
    if (it != conditions.end())
    {
        string dur = it->second;
        dur = dur.substr(0, dur.length()-1);
        time = atoi(dur.c_str());
    }

    return time;
}
//------- Vertex class
/**
 * @brief Constructor.
 * @param _id Vertex id.
 */
Vertex::Vertex (string _id)
{
    id = _id;
    visited = false;
}

/**
 * @brief Constructor.
 * @param _id Vertex id.
 * @param _trans Transition.
 * @param _type Event type.
 * @param _object Object.
 */
Vertex::Vertex (string _id, Event::Transition _trans, Event::Type _type, Object* _object, string element)
{
    id = _id;
    transition = _trans;
    type = _type;
    object = _object;
    visited = false;
    element_name = element;
}

Vertex::~Vertex ()
{

}

/**
 * @brief Inserts an edge from the vertex.
 * @param _edge Edge to be inserted.
 */
void Vertex::insertEdge (Edge* _edge)
{
    edges.push_back (_edge);
}

/**
 * @brief Removes an edge from the vertex.
 */
void Vertex::removeEdge ()
{
    edges.pop_back ();
}

/**
 * @brief Get the vertex id.
 * @return Vertex id.
 */
string Vertex::getId ()
{
    return id;
}

/**
 * @brief Get the transition.
 * @return Transition.
 */
Event::Transition Vertex::getTransition ()
{
    return transition;
}

/**
 * @brief Get the event type.
 * @return Event type.
 */
Event::Type Vertex::getEventType ()
{
    return type;
}

/**
 * @brief Get the object.
 * @return Object.
 */
Object* Vertex::getObject ()
{
    return object;
}

/**
 * @brief Get the edge list.
 * @return List of edges.
 */
list<Edge*> Vertex::getEdgeList ()
{
    return edges;
}

/**
 * @brief Return the size of the edges list.
 * @return size of the edges list.
 */
size_t Vertex::sizeEdgeList ()
{
    return edges.size();
}

/**
 * @brief Delete an edge from the vertex.
 */
void Vertex::deleteEdge ()
{
    if (sizeEdgeList () > 0)
    {
        edges.pop_front();
    }
}

/**
 * @brief Remove all edges from the vertex.
 */
void Vertex::edgesCleanup ()
{
    size_t size = sizeEdgeList ();

    for ( size_t i = 0 ; i < size ; i++ ) 
    {
        Edge *obj = getFirstEdge ();
        deleteEdge ();
        delete obj;
    }
}

/**
 * @brief Transforms a vertex into string.
 * @return String representing the vertex.
 */
string Vertex::getVertexAsString ()
{
    string _v, _transition, _event;
    switch (type)
    {
    case Event::PRESENTATION:
      _event = "presentation";
      break;
    case Event::ATTRIBUTION:
      _event = "attribution";
      break;
    case Event::SELECTION:
      _event = "selection";
      break;
    case Event::PREPARATION:
      _event = "preparation";
      break;
    default:
      g_assert_not_reached ();
    }

    switch (transition)
    {
    case Event::START:
        _transition = "start";
    break;
    case Event::PAUSE:
        _transition = "pause";
    break;
    case Event::RESUME:
        _transition = "resume";
    break;
    case Event::STOP:
        _transition = "stop";
    break;
    case Event::ABORT:
        _transition = "abort";
    break;
    default:
        g_assert_not_reached ();
    }

    _v = "(" + _transition + ", " + _event + ", " + id + ")";
    return _v;
}

// Private
/**
 * @brief Get the first edge of the vertex.
 * @return First edge.
 */
Edge* Vertex::getFirstEdge ()
{
   if ( sizeEdgeList () <= 0 )
      return NULL;
   return edges.front ();
}

string Vertex::getElementName ()
{
    return element_name;
}

//----- TemporalGraph class
/**
 * @brief Constructor.
 */
TemporalGraph::TemporalGraph ()
{
    num_vertices = 0;               
    start_vertex = createVertex("v_init", Event::START, Event::PRESENTATION, NULL, "") ;
}

/**
 * @brief Return the number of vertices that compound the graph.
 * @return Number of vertices.
 */
int TemporalGraph::getNumberVertices ()
{
    return num_vertices;
} 

/**
 * @brief Inserts a vertex in the graph.
 * @param Vertex to be inserted.
 */
void TemporalGraph::insertVertex (Vertex* _vertex)
{
    v.push_back(_vertex);
}

/**
 * @brief Deletes a vertex to the graph.
 */
void TemporalGraph::deleteVertex ()
{
    if (sizeVertexList () > 0)
    {
        v.pop_front();
    }
}

/**
 * @brief Return the list of vertices that compound the graph.
 * @return List of vertices.
 */
list<Vertex*> TemporalGraph::getVertexList ()
{
    return v;
}

/**
 * @brief Removes all vertices from the graph.
 */
void TemporalGraph::graphCleanup ()
{
    size_t size = sizeVertexList ();

    for ( size_t i = 0 ; i < size ; i++ ) 
    {
        Vertex *obj = getFirstVertex ();
        deleteVertex ();
        delete obj;
    }
}

/**
 * @brief Return the size of the vertices list.
 * @return Size of the vertices list.
 */
size_t TemporalGraph::sizeVertexList ()
{
    return v.size();
}

/**
 * @brief Return the first vertex from the graph.
 * @return The first vertex.
 */
Vertex* TemporalGraph::getFirstVertex ()
{
    if (sizeVertexList () <= 0)
        return NULL;
    return v.front ();   
}

/**
 * @brief Prints the temporal graph elements.
 */
void TemporalGraph::printGraph ()
{
    list<Vertex*>::iterator it_v;
    list<Edge*>::iterator it_ep, it_e;
    list<Edge*> aux_edges;
    
    for(it_v = v.begin(); it_v != v.end(); it_v++)
    {

       if( (*it_v)->getId() != "v_init"){
             //vertex
            g_print("Vertice: %s\n", (*it_v)->getVertexAsString().c_str());

            if (!(*it_v)->getEdgeList().empty())
            {
                aux_edges = (*it_v)->getEdgeList();
                for(it_e = aux_edges.begin(); it_e != aux_edges.end(); it_e++)
                {
                    g_print("%s\n", (*it_e)->getEdgeAsString().c_str());
                }
            }
            g_print("\n");
        }
    }   
}

/**
 * @brief Search a vertex in the graph.
 * @param _id Vertex id.
 * @return The vertex if the search successful, or null otherwise.
 */
Vertex* TemporalGraph::findVertex (string _id, Event::Transition _trans, Event::Type _type)
{
    list<Vertex*>::iterator it_v;
    for(it_v = v.begin(); it_v != v.end(); it_v++)
    {
        if((*it_v)->getId() == _id && (*it_v)->getEventType() == _type && (*it_v)->getTransition() == _trans )
            return (*it_v);
    }
    return NULL;

}

/**
 * @brief Delete all unreachable vertices from the graph.
 */
void TemporalGraph::removeUVertices ()
{

}

/**
 *  @brief Create a edge betweeen v1 and v2 vertices, without condition
 *  @param v1 Id of the source vertex.
 *  @param v2 Id of the destination vertex. 
 *  @return True if the edge is built, or false otherwise.
 */ 
bool TemporalGraph::createEdge (Vertex* v1, Vertex* v2)
{
    if( v1 != NULL && v2 != NULL)
    {
        Edge* e = new Edge (v2);
        v1->insertEdge (e);
        return true;  
    }
    else
    {
        return false;
    }
}

/**
 *  @brief Create a edge betweeen v1 and v2 vertices, with condition
 *  @param id_v1 Id of the source vertex.
 *  @param id_v2 Id of the destination vertex. 
 *  @param var Variable name.
 *  @param value Test value.
 *  @return True if the edge is built, or false otherwise.
 */ 
bool TemporalGraph::createEdge (Vertex* v1, Vertex* v2, string var, string value)
{
    if( v1 != NULL && v2 != NULL)
    {
        Edge* e = new Edge (v2);
        
        e->insertCondition (var,value);
        v1->insertEdge(e);
        return true;  
    }
    else
    {
        return false;
    }
}

/**
  * @brief Create a new vertex, if don't exists and insert in the graph
  * @param _id vertex id.
  * @param _trans event transition.
  * @param _type event type.
  * @param _obj object.
  * @return The vertex that was created.
  */
Vertex* TemporalGraph::createVertex (string _id, Event::Transition _trans, Event::Type _type, Object* _obj, string element )
{
    Vertex* v;
    v = findVertex (_id,_trans, _type);
    if ( v == NULL)
    {
        v = new Vertex (_id,_trans,_type,_obj, element);
        insertVertex (v);    
    }
    return v;
}

/**
 * @brief Return the object duration by the src attribute
 * @param src Object src attribute
 * @return The duration time of the media object
 */  
int TemporalGraph::getObjectDuration(string src) 
{
    string data;
    string cmd;
    cmd = "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 ";
    cmd.append(src);
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return atoi(data.c_str());
}

/**
  * @brief Create two new vertices: a vertex that representing the start of the media object presentation,
  *        and a vertex that representing the stop of the media object presentation.
  * @param _trans event transition.
  * @param _type event type.
  * @param _obj object.
  * @return The vertex representing the start of media object presentation
  */
Vertex* TemporalGraph::createVerticesByMedia (string _id, Event::Transition _trans, Event::Type _type, Object *_obj )
{
    Vertex* v_start;
    v_start = createVertex (_id, _trans, _type, _obj, "media");
    
    Media *media = cast (Media *, _obj);
    string uri = media->getProperty ("uri");
    string type = media->getProperty ("type");

    if (type == "" && uri != "")
    {
      string::size_type index, len;
      index = uri.find_last_of (".");
      if (index != std::string::npos)
        {
          index++;
          len = uri.length ();
          if (index < len)
            {
              string extension = uri.substr (index, (len - index));
              if (extension != "")
                mime_table_index (extension, &type);
            }
        }    
    }

    if (xstrhasprefix (type, "audio") || xstrhasprefix (type, "video"))
    {
       int duration = 0;
       duration =  getObjectDuration (uri);
       if (duration > 0)
       {
           Vertex* v_stop;
           v_stop = createVertex (_id, Event::STOP, Event::PRESENTATION, _obj, "media");
           Edge* edge = new Edge (v_stop);
           edge->insertCondition ("Duration",to_string(duration)+"s");
           v_start->insertEdge (edge);
       }      
    }
    
    
    return v_start;
}

/**
  * @brief Create new vertices and edges that represent the <link> elements in the NCL document.
  * @param conditions List of conditions role.
  * @param actions List of actions role.
  */
void TemporalGraph::addRelations (list<Action> conditions, list<Action> actions)
{
    list<Action>::iterator itAction;
    list<Action>::iterator itCondition;
    string id_vCondition;
    string id_vAction;
    string delayC="";
    string element;
    for( itCondition = conditions.begin(); itCondition != conditions.end(); itCondition++)
    {
        id_vCondition = (*itCondition).event->getId();
        if(id_vCondition == "@lambda"){
            id_vCondition = (*itCondition).event->getObject()->getId();
            element = "media";
        }
        else
        {
            element = "area";
        }        
        
        Vertex* vCondition = createVertex(id_vCondition, (*itCondition).transition, 
            (*itCondition).event->getType(), (*itCondition).event->getObject(), element);
        
        if(!(*itCondition).delay.empty())
            delayC = (*itCondition).delay;

        for ( itAction = actions.begin(); itAction != actions.end(); itAction++)
        {
            id_vAction = (*itAction).event->getId();
            if(id_vAction == "@lambda")
              id_vAction = (*itAction).event->getObject()->getId();
           
            Vertex* vAction = createVertex(id_vAction, (*itAction).transition, 
            (*itAction).event->getType(), (*itAction).event->getObject(), element);
            Edge* _edge = new Edge (vAction);
            
            if(!(*itAction).delay.empty())
                _edge->insertCondition("Duration",(*itAction).delay);
            if(!delayC.empty())
                _edge->insertCondition("Duration","delayC");
            
            vCondition->insertEdge(_edge);            
        }        
    }
}



GINGA_NAMESPACE_END