#include "aux-ginga.h"

#include "Effect.h"
#include "Event.h"
#include "EffectPlayer.h"

GINGA_NAMESPACE_BEGIN

// Public.

Effect::Effect (const string &id) : Object (id)
{
  _player = nullptr;
  _currentPreparationEvent = nullptr;
}

Effect::~Effect ()
{
  this->doStop ();
}

// Public: Object.

string
Effect::getObjectTypeAsString ()
{
  return "Effect";
}

string
Effect::toString ()
{
  string str;
  string type;

  str = Object::toString ();
  type = _properties["type"];
  if (type != "")
    str += "  type: " + type + "\n";
  
  //TO DO: imprimir o player de efeito
  //str += xstrbuild ("  player: %p\n", _player);

  return str;
}

void
Effect::setProperty (const string &name, const string &value, Time dur)
{
  string from = this->getProperty (name);
  Object::setProperty (name, value, dur);

  if (_player == nullptr)
    return;

  _player->setProperty (name, value);
  //g_assert (GINGA_TIME_IS_VALID (dur));
  //TO DO: adicionar isso pro player de efeito
  /*if (dur > 0)
    _player->schedulePropertyAnimation (name, from, value, dur);
  else
    _player->setProperty (name, value);*/
}

void
Effect::sendTick (Time total, Time diff, Time frame)
{
  Time dur;
  
  //TO DO: inserir isso apos criar o player de efeito
  /*if (this->isPreparing ())
    {
      g_assert_nonnull (_player);
      if (_player->getPrepared () )
      {
        Event *currentPreparation = this->getCurrentPreparationEvent ();
        g_assert_nonnull (currentPreparation);
        _doc->evalAction (currentPreparation, Event::STOP);
        return;
      }
      
    }*/

  // Update object time.
  Object::sendTick (total, diff, frame);

  //TO DO: descomentar isso apos ter o player de efeito
  //if (_player == nullptr)
  //  return; // nothing to do.

  // Update player time.
  //g_assert_nonnull (_player);
  //_player->incTime (diff);

  // Check EOS.
  //TO DO: o efeito vai ter duração implicita?
    /*if (_player->getEOS ()
        || (GINGA_TIME_IS_VALID (dur = _player->getDuration ())
            && _time > dur ))
      {
        Event *lambda = this->getLambda ();
        g_assert_nonnull (lambda);
        //TRACE ("eos %s at %" GINGA_TIME_FORMAT, lambda->getFullId ().c_str (),
        //      GINGA_TIME_ARGS (_time));
        _doc->evalAction (lambda, Event::STOP);
        return;
      }*/
  
}

bool
Effect::beforeTransition (Event *evt, Event::Transition transition)
{
  switch (evt->getType ())
    {
    case Event::PRESENTATION:
      switch (transition)
        {
        case Event::START:
          {
            // Create underlying player.
            if (evt->getState () == Event::SLEEPING)
              {
                if (evt->isLambda ())
                  { // Lambda
                    createEffectPlayer ();
                    if (unlikely (_player == nullptr))
                      return false; // fail

                    // Start underlying player.
                    // TO DO: Check player failure. 
                    _player->start (); // Just lambda events reaches this!
                  }
              }
            break;
          }

        //TO DO: implementar o pause e resume de efeitos
       // case Event::PAUSE:
       /* case Event::RESUME:
          {
            if (!evt->isLambda ())
              break; // nothing to do

            // Pause/resume all the media anchors.
            for (auto e : _events)
              if (!e->isLambda () && e->getType () == Event::PRESENTATION)
                _doc->evalAction (e, transition);

            // Pause/resume the underlying player.
            g_assert_nonnull (_player);
            if (transition == Event::PAUSE)
              _player->pause ();
            else
              _player->resume ();
            break;
          }
        */
        case Event::STOP:
          if(evt->isLambda())
            g_print("stop presentation %s\n", this->getId().c_str());
          break; // nothing to do

        //TO DO: como implementar o abort de efeitos
        case Event::ABORT:
          break; // nothing to do

        case Event::PREPARE_STOP:
          break; // nothing to do

        default:
          g_assert_not_reached ();
        }
      break;
    
    case Event::ATTRIBUTION:
      break; // nothing to do
    //TO DO: vai ter selection em efeito?
    case Event::SELECTION:
      break; // nothing to do

    case Event::PREPARATION:
      break;

    default:
      g_assert_not_reached ();
    }
  return true;
}

bool
Effect::afterTransition (Event *evt, Event::Transition transition)
{
  switch (evt->getType ())
    {
    case Event::PRESENTATION:
      switch (transition)
        {
        case Event::START:
          if (evt->isLambda ())
            {
              g_assert_nonnull (_player);
              Object::doStart ();
             
              TRACE ("start %s", evt->getFullId ().c_str ());
              g_print ("start presentation %s\n", this->getId ().c_str ());
            }
          break;

        //TO DO: Implementar se tiver pause e resume para efeito
        /*case Event::PAUSE:
          TRACE ("pause %s", evt->getFullId ().c_str ());
          break; // nothing to do

        case Event::RESUME:
          TRACE ("resume %s", evt->getFullId ().c_str ());
          break; // nothing to do*/

        case Event::STOP:
        case Event::ABORT:
          if (evt->isLambda ())
            {
              // Stop object.
              //TO DO: inserir apos criar o player de efeitos
              //g_assert_nonnull (_player);
              this->doStop ();
              if (transition == Event::ABORT)
                TRACE ("abort %s", evt->getFullId ().c_str ());
              else
                TRACE ("stop %s", evt->getFullId ().c_str ());
            }
          
          break;
        case Event::PREPARE_STOP:
          g_assert_nonnull(_player);
          _player->prepareStop();
          break;
        default:
          g_assert_not_reached ();
        }
      break;

    case Event::ATTRIBUTION:
      switch (transition)
        {
        case Event::START:
          {
            string name;
            string value;
            string s;
            Time dur;

            name = evt->getId ();
            evt->getParameter ("value", &value);
            _doc->evalPropertyRef (value, &value);

            dur = 0;
            if (evt->getParameter ("duration", &s))
              {
                _doc->evalPropertyRef (s, &s);
                dur = ginga::parse_time (s);
              }
            this->setProperty (name, value, dur);
            this->addDelayedAction (evt, Event::STOP, value, dur);
            TRACE ("start %s:='%s' (dur=%s)", evt->getFullId ().c_str (),
                   value.c_str (), (s != "") ? s.c_str () : "0s");
            break;
          }

        case Event::STOP:
          TRACE ("stop %s:=...", evt->getFullId ().c_str ());
          break;

        default:
          g_assert_not_reached ();
        }
      break;

    //TO DO: Inserir se for permitido seleção de efeito sensorial
    /*case Event::SELECTION:
      {
        string key;
        evt->getParameter ("key", &key);
        switch (transition)
          {
          case Event::START:
            TRACE ("start %s", evt->getFullId ().c_str ());
            break;
          case Event::STOP:
            TRACE ("stop %s", evt->getFullId ().c_str ());
            break;
          default:
            g_assert_not_reached ();
          }
        break;
      }*/

    case Event::PREPARATION:
      {
        switch (transition)
          {
          case Event::START:
            createEffectPlayer ();
            g_print ("start preparation effect %s\n", getId().c_str ());
            _player->startPreparation();
            _isPreparing = true;
            _currentPreparationEvent = evt;
            break;
          case Event::STOP:
              g_print ("stop preparation %s\n", getId().c_str ());
            _isPreparing = false;
            break;
          default:
            g_assert_not_reached ();
          }
        break;
      }

    default:
      g_assert_not_reached ();
    }

  return true;
}

Event* 
Effect::getCurrentPreparationEvent ()
{
  return _currentPreparationEvent;
}

bool
Effect::isFocused ()
{
  if (!this->isOccurring ())
    return false;
  g_assert_nonnull (_player);
  return _player->isFocused ();
}

void
Effect::createEffectPlayer ()
{
  if (_player)
    return;
  Formatter *fmt;
  g_assert (_doc->getData ("formatter", (void **) &fmt));
  g_assert_null (_player);
  _player = EffectPlayer::createEffectPlayer (fmt, this, _properties["type"]);
  g_assert_nonnull (_player);
  for (auto it : _properties)
  {
    _player->setProperty (it.first, it.second);
  }
}

// Protected.

void
Effect::doStop ()
{
  if (_player == nullptr)
    {
      g_assert (this->isSleeping ());
      return; // nothing to do
    }

  if (_player->getState () != EffectPlayer::SLEEPING)
    _player->stop ();
  delete _player;
  _player = nullptr;
  Object::doStop ();
}

GINGA_NAMESPACE_END
