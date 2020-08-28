/* Copyright (C) 2006-2018 PUC-Rio/Laboratorio TeleMidia

This file is part of Ginga (Ginga-NCL).

Ginga is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Ginga is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Ginga.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef EVENT_H
#define EVENT_H

#include "aux-ginga.h"
#include "Predicate.h"

GINGA_NAMESPACE_BEGIN

class Object;

/**
 * @brief Event state machine.
 */
class Event
{
public:
  /// @brief Event type.
  enum Type
  {
    /**
     * @brief Attribution event.
     *
     * Stands for the attribution of some value to a specific property of
     * the object.
     */
    ATTRIBUTION = 0,

    /**
     * @brief Presentation event.
     *
     * Stands for the presentation of a specific time interval of the
     * object.
     */
    PRESENTATION,

    /**
     * @brief Selection event.
     *
     * Stands for the pressing or releasing of a specific key which is
     * directed to the object.
     */
    SELECTION,
    
    /**
     * @brief Preparation event.
     *
     * Stands for the preparation of a specific time interval of the object.
     */
    PREPARATION,

	/**
	 * @brief Voice Recognition event.
	 *
	 *Stands for the recognition of a  specific key (this case word), which is
	 * directed to the object.
	 */
	 VOICE_RECOGNITION,

	 /**
		 * @brief Eye Recognition event.
		 *
		 *Stands for the recognition of a specific area (this case a region), which is
		 * directed to the object.
	*/
	 EYE_GAZE,

	 /**
		 * @brief Face Recognition event.
		 *
		 *Stands for the recognition of a Face expression, which is
		 * directed to the object.
	*/
	 FACE_RECOGNITION,

	 /**
		 * @brief Gesture Recognition event.
		 *
		 *Stands for the recognition of a gesture, which is
		 * directed to the object.
	*/
	 GESTURE_RECOGNITION,
  };

  /// @brief Event state.
  enum State
  {
    OCCURRING = 0, ///< Occurring.
    PAUSED,        ///< Paused.
    SLEEPING,      ///< Sleeping.
  };

  /// @brief Event state transitions.
  enum Transition
  {
    ABORT = 0, ///< Abort.
    PAUSE,     ///< Pause.
    RESUME,    ///< Resume.
    START,     ///< Start.
    STOP,      ///< Stop.
  };

  Event (Event::Type, Object *, const string &id);
  Event (Event::Type, Object *, const string &id, const string &owner);
  ~Event ();

  Event::Type getType ();
  Object *getObject ();
  string getId ();
  string getOwner ();
  string getFullId ();
  Event::State getState ();
  string toString ();

  bool isLambda ();
  void getInterval (Time *, Time *);
  void setInterval (Time, Time);

  bool hasLabel ();
  std::string getLabel ();
  void setLabel (const std::string &);

  bool getParameter (const string &, string *);
  bool setParameter (const string &, const string &);

  bool transition (Event::Transition);
  void reset ();

public:
  static string getEventTypeAsString (Event::Type);
  static string getEventStateAsString (Event::State);
  static string getEventTransitionAsString (Event::Transition);
  static Event::Transition getStringAsTransition (string str);

private:
  Event::Type _type;               ///< Event type.
  Object *_object;                 ///< Target object.
  string _id;                      ///< Event id.
  string _owner;                   ///< Event owner.
  Event::State _state;             ///< Event state.
  Time _begin;                     ///< Begin time.
  Time _end;                       ///< End time.
  std::string _label;              ///< Label.
  map<string, string> _parameters; ///< Parameters.
};

/**
 * @brief Action.
 */
typedef struct
{
  Event *event;                 ///< Target event.
  Event::Transition transition; ///< Desired transition.
  Predicate *predicate;         ///< Predicate conditioning the execution.
  string value;                 ///< Value to set (if attribution).
  string owner;                 ///< Owner Action.
  string duration;              ///< Duration.
  string delay;                 ///< Delay.
} Action;

GINGA_NAMESPACE_END

#endif // EVENT_H
