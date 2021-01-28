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

#ifndef USERDOCUMENT_H
#define USERDOCUMENT_H

GINGA_NAMESPACE_BEGIN

typedef struct
{
  string type;           ///< id component.
  string name;                 ///< Value key.
  string value;                ///< Owner Action.
} property;

/**
 * @brief user document - MPEG21.
 *
 * Maintains all objects in document.
 */
class UserDocument
{
public:
  UserDocument ();
  virtual ~UserDocument ();

  const set<Object *> *getObjects ();
  Object *getObjectById (const string &);
  Object *getObjectByIdOrAlias (const string &);
  bool addObject (Object *);

  
  Context *getRoot ();
  MediaSettings *getSettings ();
  const set<Media *> *getMedias ();
  const set<Context *> *getContexts ();
  const set<Switch *> *getSwitches ();

  map<string,user> getUsers();
  map<string,profile> getProfiles ();
  map<string, MediaSettings *> getuserSettings ();

  int evalAction (Event *, Event::Transition, const string &value = "");
  int evalAction (Action);
  bool evalPredicate (Predicate *);
  bool evalPropertyRef (const string &, string *);

  bool getData (const string &, void **);
  bool setData (const string &, void *, UserDataCleanFunc fn = nullptr);

  map<Event::Type,bool> getInteractions();
  map<Event::Type,list<Key>> getKeyList();
  bool addInteractions (Event::Type intEvent, bool on);
  bool setInteractions (Event::Type intEvent, bool on);
  bool checkInteractions (Event::Type intEvent);
  void addKeyList (Event::Type intEvent, Key key);

  bool addUser (user _user);
  bool addProfile (profile _profile);
  MediaSettings * addUserSetting (string idUser);


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
  map<Event::Type,list<Key>> _keyList;
  map<string,user> _userList;  
  map<string,profile> _profileList;
  map<string, MediaSettings *> _userSettingsList;

};

GINGA_NAMESPACE_END

#endif // DOCUMENT_H
