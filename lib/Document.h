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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Object.h"

GINGA_NAMESPACE_BEGIN

typedef struct
{
  string key;                 ///< Value key.
  string user;                ///< Owner Action.
  string delay;               ///< Delay.
} Key;



class Context;
class Media;
class Switch;

/**
 * @brief NCL document.
 *
 * Maintains all objects in document.
 */
class Document
{
public:
  Document ();
  virtual ~Document ();

  const set<Object *> *getObjects ();
  Object *getObjectById (const string &);
  Object *getObjectByIdOrAlias (const string &);
  bool addObject (Object *);

  
  Context *getRoot ();
  MediaSettings *getSettings ();
  const set<Media *> *getMedias ();
  const set<Context *> *getContexts ();
  const set<Switch *> *getSwitches ();

  int evalAction (Event *, Event::Transition, const string &value = "");
  int evalAction (Action);
  bool evalPredicate (Predicate *);
  bool evalPropertyRef (const string &, string *);

  bool getData (const string &, void **);
  bool setData (const string &, void *, UserDataCleanFunc fn = nullptr);

  map<Event::Type,bool> getInteractions();
  map<Event::Type,Key> getKeyList();
  bool addInteractions (Event::Type intEvent, bool on);
  bool setInteractions (Event::Type intEvent, bool on);
  bool checkInteractions (Event::Type intEvent);
  bool addKeyList (Event::Type intEvent, Key key);


private:
  set<Object *> _objects;             	///< Objects.
  map<string, Object *> _objectsById; 	///< Objects indexed by id.
  Context *_root;                     	///< Root context (body).
  MediaSettings *_settings;           	///< Settings object.
  set<Media *> _medias;               	///< Media objects.
  set<Context *> _contexts;           	///< Context objects.
  set<Switch *> _switches;            	///< Switch objects.
  UserData _udata;                    	///< Attached user data.
  map<Event::Type,bool> _interactions;  ///< Used to signal which types of interaction events that happen in this document.
  map<Event::Type, Key> _keyList; ///< Used to store keys that to be recognized by modules.
};



GINGA_NAMESPACE_END

#endif // DOCUMENT_H
