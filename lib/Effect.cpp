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
}

void
Effect::sendTick (Time total, Time diff, Time frame)
{
  Time dur;
  
  //TO DO
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

  //TO DO
  //if (_player == nullptr)
  //  return; // nothing to do.

  // Update player time.
  //g_assert_nonnull (_player);
  //_player->incTime (diff);
  
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

        //TO DO: implements pause and resume 
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

        case Event::PAUSE:
          break; // nothing to do

        case Event::RESUME:
          break; // nothing to do

        case Event::STOP:
        case Event::ABORT:
          if (evt->isLambda ())
            {
              // Stop object.
              g_assert_nonnull (_player);
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
