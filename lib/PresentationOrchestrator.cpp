#include "PresentationOrchestrator.h"

GINGA_NAMESPACE_BEGIN

//Used only to verify the media is a video, audio, text or image.
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

PresentationOrchestrator::PresentationOrchestrator ()
{
    time = 0;
    preparationEnabled = false;
}
/**
 * @brief Traverses the graph in depth
 * @param v Current vertex in the search
 */
void PresentationOrchestrator::depthFirstSearch (Vertex* v)
{
    list<Edge*> listNeighbor = v->getEdgeList();
    list<Edge*>::iterator itNeighbor;
        
    if(!v->visited){
        v->visited = true;
        PlanItem item;
        item.instant = time;
        item.vertex = v;
        presentation_plan.push_back(item);
    }
    
    for(itNeighbor = listNeighbor.begin(); itNeighbor != listNeighbor.end(); itNeighbor++){
        if(!((*itNeighbor)->neighbor->visited)){
            time += (*itNeighbor)->getDurationEdge();
            depthFirstSearch((*itNeighbor)->neighbor);
            time = time - (*itNeighbor)->getDurationEdge();
        }
    }
}

/**
 * @brief Method to compare two PlanItem elements
 * @param a First element to compare
 * @param b Second element to compare
 * @return false if the instant attribute of a is less than the instant attribute of b; 
 *          and true otherwise. 
 */ 
bool compare (const PlanItem& a,const PlanItem& b)
{
    return a.instant < b.instant;
}

/** 
 * @brief Creates the presentation plan from a temporal graph
 * @param _graph The temporal graph representing the multimedia application
 */
void PresentationOrchestrator::createPresentationPlan (TemporalGraph* graph, map<string, Device*> devices)
{
    htg = graph;
    _deviceList = devices;
    //htg->printGraph();
    if(htg->start_vertex->getEdgeList().size() == 1)
    {
        Edge* e = htg->start_vertex->getEdgeList().front();
        depthFirstSearch(e->neighbor);
    }
    
    pO = new PreparationOrchestrator(this->presentation_plan, htg);
    this->presentation_plan.sort(compare);
    list<PlanItem>::iterator it;
    /*g_print("---------------------------------------------------------------\n");
    g_print ("Presentation Plan\n");
    for( it = presentation_plan.begin(); it != presentation_plan.end(); it++ )
    {
        g_print("%.2fs - %s\n", (*it).instant, (*it).vertex->getVertexAsString().c_str());
    }
    g_print("---------------------------------------------------------------\n");  */

    if (preparationEnabled)
    {
        pO->createPreparationPlan(_deviceList);
    }
       
}

// ----------- Preparation Orchestrator
PreparationOrchestrator::PreparationOrchestrator(list<PlanItem> pres_plan, TemporalGraph* _htg)
{
    presentation_plan = pres_plan;
    htg = _htg;
}

/**
 * @brief Return the media size.
 * @param src Source of the media.
 * @retunr The media size in bits.
 */ 
int PreparationOrchestrator::getMediaSize (string src)
{
    string duration, bitrate;
    int _dur, _bitrate, _size;
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
    if (fgets(buffer, max_buffer, stream) != NULL) duration.append(buffer);
        pclose(stream);
    }
    _dur = atoi(duration.c_str());

    cmd.clear();
    cmd = "ffprobe -v error -show_entries format=bit_rate -of default=noprint_wrappers=1:nokey=1 ";
    cmd.append(src);
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) bitrate.append(buffer);
        pclose(stream);
    }
    _bitrate = atoi(bitrate.c_str());
    _size = _dur * _bitrate;
    return _size;
}
/**
 * @brief Gets the required time to prepare a media object.
 * @param media media to be prepared.
 * return Time required to download a media object.
  
float PreparationOrchestrator::getPreparationDuration (Media* media)
{
    float dur;
    int size;

    size = getMediaSize(media->getProperty ("uri"));
    dur = ((float)size)/(1048576*TRHOUGHPUT);

    return dur;
}*/

/**
 * @brief Temporary function to verify the media type.
 * @param type The media type.
 * @param uri The media locator.
 * @return True if the type of media need to be prepared, false for otherwise
 */ 
bool PreparationOrchestrator::verifyMediaType (string type, string uri)
{
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

    if (xstrhasprefix (type, "video"))
    {
       return true;     
    } 
    return false;
}

/**
 * @brief Creates the preparation plan from the presentation plan.
 */ 
void PreparationOrchestrator::createPreparationPlan (map<string, Device*> devices)
{
    list<PlanItem>::iterator it;
    float _time;
    _deviceList = devices;

    for( it = presentation_plan.begin(); it != presentation_plan.end(); it++ )
    {
        if((*it).vertex->getTransition() == Event::START && (*it).vertex->getEventType()==Event::PRESENTATION)
        {
            if((*it).vertex->getElementName()=="Media")
            {
                
                Media* m = (Media*) (*it).vertex->getObject();
                g_assert_nonnull(m);
                string uri = m->getProperty ("uri");
                string type = m->getProperty ("type");
                //Insert to preparation plan only video objects
                if(verifyMediaType (type, uri))
                {
                     _time = (*it).instant - PREPARATION_TIME; 
                    if(_time < 0)
                        _time = 0;
                    PlanItem item;
                    item.instant = _time;
                    item.vertex = (*it).vertex;
                    item.action = "start";
                    preparation_plan.push_back(item);
                } 
            }
            else if((*it).vertex->getElementName()=="Effect")
            {
                
                Effect* e = (Effect*) (*it).vertex->getObject();
                
                g_assert_nonnull(e);
                string type = e->getProperty("type");
                int prep_time = getPreparationTime(type, "start");
                _time = (*it).instant - prep_time; 
                if(_time < 0)
                    _time = 0;
                PlanItem item;
                item.instant = _time;
                item.vertex = (*it).vertex;
                item.action = "start";
                preparation_plan.push_back(item);                 
            }
        }        

        //Preparation to stops a sensory effect in advance
        if((*it).vertex->getTransition() == Event::STOP && (*it).vertex->getEventType()==Event::PRESENTATION
            && strcmp((*it).vertex->getElementName().c_str(),"Effect")==0)
        {

            Effect* e = (Effect*) (*it).vertex->getObject();
            g_assert_nonnull(e);
            string type = e->getProperty("type");
            int prep_time = getPreparationTime(type, "stop");
            _time = (*it).instant - prep_time; 
            if(_time < 0)
                _time = 0;
            PlanItem item;
            item.instant = _time;
            item.vertex = (*it).vertex;
            item.action = "stop";
            preparation_plan.push_back(item);
        }
    }
    insertPreparationActions();
    preparation_plan.sort(compare);
    //printPreparationPlan();    
}

/**
 * @brief Print the preparation plan.
 */ 
void PreparationOrchestrator::printPreparationPlan ()
{
    list<PlanItem>::iterator it;
    g_print("Preparation Plan\n");
    for( it = preparation_plan.begin(); it != preparation_plan.end(); it++ )
    {
       g_print("%.2fs - %s\n", (*it).instant, (*it).vertex->getId().c_str());
    } 
    g_print("---------------------------------------------------------------\n");
}

/**
 * @brief Insert actions in the document to start the 
 *        media object preparation 3 seconds before the your presentation
 */ 
void PreparationOrchestrator::insertPreparationActions ()
{
    //Verificar o contexto que a midia a ser preparada pertence,
    //e chamar o metodo addLinks() igual e' feito na adição de links do parser.
    //Context::addLink (list<Action> conds, list<Action> acts)
    //Olhar o metodo Document::evalAction() como base
    //Para cada preparação, vou criar um link tendo como condição, o início do no' inicial, e 
    // a ação, a preparação definida com delay
    list<PlanItem>::iterator it;
    
    Vertex* v_start = htg->start_vertex;
    if (htg->start_vertex->sizeEdgeList() > 0)
    {
        Vertex* vPort = htg->start_vertex->getFirstEdge()->neighbor;
        Context* _ctx;
        Composition* comp;
        Object* _obj = vPort->getObject(); 
        comp = _obj->getParent ();
        if (comp != nullptr && instanceof (Context *, comp))
            _ctx = cast (Context *, comp);
        
        Action _condition;
        _condition.event = _obj->getPresentationEvent("@lambda");
        _condition.transition = Event::START;
        _condition.predicate = nullptr;
        _condition.value = "";

        for( it = preparation_plan.begin(); it != preparation_plan.end(); it++ )
        {
            list<Action> conditions;
            list<Action> actions;
            Action act;

            if ((*it).instant > 0)
            {
                Media* _media = (Media*) (*it).vertex->getObject(); //Media to be prepared
                _media->addPreparationEvent("@lambda");

                act.event = _media->getPreparationEvent("@lambda");
                act.transition = Event::START;
                act.delay = to_string((*it).instant); 
                act.predicate = nullptr;
                act.value = "";
                actions.push_back(act);
                conditions.push_back(_condition);
                if (_ctx != nullptr){
                    _ctx->addLink(conditions, actions);
                }
            }       
           
           
        }        
    } 
}

/**
 * @brief Return the time to prepare the start or stop of a sensory device
 * @param devices List of sensory devices availables
 * @param deviceType Type of the sensory device to be prepared
 * @param action "start" to get the time to prepare the start of device
 *                 or "stop" to get the time to prepare the stop of device
 */
int
PreparationOrchestrator::getPreparationTime(string deviceType, string action)
{
  auto it = _deviceList.find(deviceType);
  if (it == _deviceList.end ())
    return 0;
  
  Device* dev = it->second;
  if (strcmp(action.c_str(),"start")==0)
  {
    //return the time necessary to prepare the sensory device start
    return dev->getStartPreparationTime();
  }

  if (strcmp(action.c_str(),"stop")==0)
  {
    //return the time necessary to prepare the sensory device start
    return dev->getStopPreparationTime();
  }

  return 0;
}

GINGA_NAMESPACE_END