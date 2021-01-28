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

#include "aux-ginga.h"
#include "ParserUserDescription.h"

#include "UserDocument.h"
#include "MediaSettings.h"

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/uri.h>

//GINGA_NAMESPACE_BEGIN

// XML helper macros and functions.
#define toCString(s) deconst (char *, (s))
#define toXmlChar(s) (xmlChar *) (deconst (char *, (s).c_str ()))
#define toCPPString(s) string (deconst (char *, (s)))

/// Flags to LibXML parser.
#define PARSER_LIBXML_FLAGS (XML_PARSE_NOERROR | XML_PARSE_NOWARNING)

/// Gets last XML error as C++ string.
static inline string
xmlGetLastErrorAsString ()
{
  xmlError *err;
  string errmsg;

  err = xmlGetLastError ();
  g_assert_nonnull (err);

  errmsg = (err->file != nullptr) ? toCPPString (err->file) + ": " : "";
  errmsg += "XML error";
  if (err->line > 0)
    errmsg += xstrbuild (" at line %d", err->line);
  errmsg += ": " + xstrstrip (string (err->message));

  return errmsg;
}

/// Gets node property as C++ string.
static inline bool
xmlGetPropAsString (xmlNode *node, const string &name, string *result)
{
  xmlChar *str = xmlGetProp (node, toXmlChar (name));
  if (str == nullptr)
    return false;
  tryset (result, toCPPString (str));
  g_free (str);
  return true;
}

/// Finds node children by tag.
static list<xmlNode *>
xmlFindAllChildren (xmlNode *node, const string &tag)
{
  list<xmlNode *> children;
  for (xmlNode *child = node->children; child; child = child->next)
    {
      if (child->type == XML_ELEMENT_NODE
          && toCPPString (child->name) == tag)
        {
          children.push_back (child);
        }
    }
  return children;
}

/// Tests whether value is a valid XML name.
static bool
xmlIsValidName (const string &value, char *offending)
{
  const char *str;
  char c;

  str = value.c_str ();
  while ((c = *str++) != '\0')
    {
      if (!(g_ascii_isalnum (c) || c == '-' || c == '_' || c == ':'
            || c == '.' || c == '#'))
        {
          tryset (offending, c);
          return false;
        }
    }
  return true;
}

// Parser internal types.

// Forward declarations.
typedef struct ParserSyntaxAttr ParserSyntaxAttr;
typedef struct ParserSyntaxElt ParserSyntaxElt;

/**
 * @brief Parser element wrapper.
 *
 * Data associated with a specific \c xmlNode.
 */
class ParserElt
{
public:
  explicit ParserElt (xmlNode *);
  ~ParserElt ();

  string getTag ();
  xmlNode *getNode ();
  xmlNode *getParentNode ();

  const map<string, string> *getAttributes ();
  bool getAttribute (const string &, string *);
  bool setAttribute (const string &, const string &);

  bool getData (const string &, void **);
  bool setData (const string &, void *, UserDataCleanFunc fn = nullptr);

private:
  string _tag;                ///< Element tag.
  xmlNode *_node;             ///< Corresponding node in document tree.
  map<string, string> _attrs; ///< Element attributes.
  UserData _udata;            ///< Attached user data.
};

/**
 * @brief Parser state.
 *
 * Parser state while it's parsing the document.
 */
class ParserState
{
public:
  /// Parser error codes.
  enum Error
  {
    ERROR_NONE = 0,                     ///< No error.
    ERROR_ELT_UNKNOWN,                  ///< Unknown element.
    ERROR_ELT_MISSING_PARENT,           ///< Missing parent element.
    ERROR_ELT_BAD_PARENT,               ///< Bad parent element.
    ERROR_ELT_UNKNOWN_ATTRIBUTE,        ///< Unknown attribute.
    ERROR_ELT_MISSING_ATTRIBUTE,        ///< Missing attribute.
    ERROR_ELT_BAD_ATTRIBUTE,            ///< Bad attribute.
    ERROR_ELT_MUTUALLY_EXCL_ATTRIBUTES, ///< Mutually exclusive attributes.
    ERROR_ELT_UNKNOWN_CHILD,            ///< Unknown child element.
    ERROR_ELT_MISSING_CHILD,            ///< Missing child element.
    ERROR_ELT_BAD_CHILD,                ///< Bad child element.
    ERROR_ELT_IMPORT,                   ///< Error in imported document.
  };

  ParserState (int, int);
  ~ParserState ();
  ParserState::Error getError (string *);
  Document *process (xmlDoc *);

  // push & pop
  static bool pushNcl (ParserState *, ParserElt *);
  static bool popNcl (ParserState *, ParserElt *);

private:
  UserDocument *_doc;      ///< The resulting #Document.
  xmlDoc *_xml;        ///< The DOM tree being processed.
  int _genid;          ///< Last generated id.
  UserData _udata;     ///< Attached user data.
  set<string> _unique; ///< Unique attributes seen so far.

  ParserState::Error _error; ///< Last error code.
  string _errorMsg;          ///< Last error message.

  map<xmlNode *, ParserElt *> _eltCache;          ///< Element cache.
  map<string, list<ParserElt *> > _eltCacheByTag; ///< Element cache by tag.

  /// Alias stack for solving imports.
  list<pair<string, string> > _aliasStack;

  ///< #Object stack for solving object hierarchy.
  list<Object *> _objStack;

  ///< Rectangle stack for solving region hierarchy.
  list<Rect> _rectStack;

  ///< Reference map for solving the refer attribute in \<media\>.
  map<string, Media *> _referMap;

  string genId ();
  string getURI ();
  bool isInUniqueSet (const string &);
  void addToUniqueSet (const string &);
  bool getData (const string &, void **);
  bool setData (const string &, void *, UserDataCleanFunc fn = nullptr);

  // Errors.
  bool errElt (xmlNode *, ParserState::Error, const string &);
  bool errEltUnknown (xmlNode *);
  bool errEltMissingParent (xmlNode *);
  bool errEltBadParent (xmlNode *);
  bool errEltUnknownAttribute (xmlNode *, const string &);
  bool errEltMissingAttribute (xmlNode *, const string &);
  bool errEltBadAttribute (xmlNode *, const string &, const string &,
                           const string &explain = "");
  bool errEltMutuallyExclAttributes (xmlNode *, const string &,
                                     const string &);
  bool errEltUnknownChild (xmlNode *, const string &);
  bool errEltMissingChild (xmlNode *, const list<string> &);
  bool errEltBadChild (xmlNode *, const string &,
                       const string &explain = "");
  bool errEltImport (xmlNode *, const string &explain = "");

  // Element cache.
  bool eltCacheIndex (xmlNode *, ParserElt **);
  bool eltCacheIndexParent (xmlNode *, ParserElt **);
  bool eltCacheIndexById (const string &, ParserElt **,
                          const list<string> &);
  size_t eltCacheIndexByTag (const list<string> &, list<ParserElt *> *);
  bool eltCacheAdd (ParserElt *);

  // Alias stack.
  string aliasStackCombine ();
  bool aliasStackPeek (string *, string *);
  bool aliasStackPop (string *, string *);
  bool aliasStackPush (const string &, const string &);

  // Object stack.
  Object *objStackPeek ();
  Object *objStackPop ();
  void objStackPush (Object *);

  // Rectangle stack.
  Rect rectStackPeek ();
  Rect rectStackPop ();
  void rectStackPush (Rect);

  // Reference map.
  bool referMapIndex (const string &, Media **);
  bool referMapAdd (const string &, Media *);

  // Reference solving.
  bool resolveComponent (Composition *, ParserElt *, Object **);
  bool resolveInterface (Composition *, ParserElt *, Event **);
  string resolveParameter (const string &, const map<string, string> *,
                           const map<string, string> *,
                           const map<string, string> *);
  // Predicate solving.
  Predicate *obtainPredicate (xmlNode *);
  Predicate *solvePredicate (Predicate *, const map<string, string> *);

  // Node processing.
  ParserSyntaxElt *checkNode (xmlNode *, map<string, string> *,
                              list<xmlNode *> *);
  bool processNode (xmlNode *);

  Document* getDoc();
};

/// Asserted version of UserData::getData().
#define UDATA_GET(obj, key, ptr)                                           \
  G_STMT_START                                                             \
  {                                                                        \
    *(ptr) = nullptr;                                                      \
    g_assert ((obj)->getData ((key), (void **) (ptr)));                    \
    g_assert_nonnull (*(ptr));                                             \
  }                                                                        \
  G_STMT_END

/// Asserted version of UserData::setData().
#define UDATA_SET(obj, key, ptr, fn)                                       \
  G_STMT_START                                                             \
  {                                                                        \
    (obj)->setData ((key), (void *) (ptr), (fn));                          \
  }                                                                        \
  G_STMT_END

// NCL syntax.

/// Type for element push function.
typedef bool(ParserSyntaxEltPush) (ParserState *, ParserElt *);

/// Type for element pop function.
typedef bool(ParserSyntaxEltPop) (ParserState *, ParserElt *);

/**
 * @brief NCL attribute syntax.
 */
typedef struct ParserSyntaxAttr
{
  string name; ///< Attribute name.
  int flags;   ///< Processing flags.
} ParserSyntaxAttr;

/**
 * @brief NCL element syntax.
 */
typedef struct ParserSyntaxElt
{
  ParserSyntaxEltPush *push;         ///< Push function.
  ParserSyntaxEltPop *pop;           ///< Pop function.
  int flags;                         ///< Processing flags.
  list<string> parents;              ///< Possible parents.
  list<ParserSyntaxAttr> attributes; ///< Attributes.
} ParserSyntaxElt;

/**
 * @brief NCL attribute processing flags.
 */
typedef enum {
  PARSER_SYNTAX_ATTR_NONEMPTY = 1 << 1,   ///< Cannot be not empty.
  PARSER_SYNTAX_ATTR_REQUIRED = 1 << 2,   ///< Is required.
  PARSER_SYNTAX_ATTR_TYPE_ID = 1 << 3,    ///< Is an id.
  PARSER_SYNTAX_ATTR_TYPE_IDREF = 1 << 4, ///< Is an id-ref.
  PARSER_SYNTAX_ATTR_TYPE_NAME = 1 << 5,  ///< Is a name.
  PARSER_SYNTAX_ATTR_UNIQUE = 1 << 6,     ///< Must be unique in document.
} ParserSyntaxAttrFlag;

#define ATTR_NONEMPTY (PARSER_SYNTAX_ATTR_NONEMPTY)
#define ATTR_REQUIRED (PARSER_SYNTAX_ATTR_REQUIRED)
#define ATTR_TYPE_ID (PARSER_SYNTAX_ATTR_TYPE_ID)
#define ATTR_TYPE_IDREF (PARSER_SYNTAX_ATTR_TYPE_IDREF)
#define ATTR_TYPE_NAME (PARSER_SYNTAX_ATTR_TYPE_NAME)
#define ATTR_UNIQUE (PARSER_SYNTAX_ATTR_UNIQUE)

#define ATTR_NONEMPTY_NAME (ATTR_NONEMPTY | ATTR_TYPE_NAME)
#define ATTR_REQUIRED_NONEMPTY_NAME (ATTR_REQUIRED | ATTR_NONEMPTY_NAME)

#define ATTR_ID (ATTR_UNIQUE | ATTR_REQUIRED_NONEMPTY_NAME | ATTR_TYPE_ID)
#define ATTR_OPT_ID (ATTR_UNIQUE | ATTR_NONEMPTY_NAME | ATTR_TYPE_ID)

#define ATTR_IDREF (ATTR_REQUIRED_NONEMPTY_NAME | ATTR_TYPE_IDREF)
#define ATTR_OPT_IDREF (ATTR_NONEMPTY_NAME | ATTR_TYPE_IDREF)

/**
 * @brief NCL element processing flags.
 */
typedef enum {
  PARSER_SYNTAX_ELT_CACHE = 1 << 1,  ///< Save element in #Parser cache.
  PARSER_SYNTAX_ELT_GEN_ID = 1 << 2, ///< Generate id if not present.
} ParserSyntaxEltFlag;

#define ELT_CACHE (PARSER_SYNTAX_ELT_CACHE)
#define ELT_GEN_ID (PARSER_SYNTAX_ELT_GEN_ID)

// Defines table index function.
#define PARSER_SYNTAX_TABLE_INDEX_DEFN(tabname, Type)                      \
  static bool parser_syntax_##tabname##_table_index (const string &str,    \
                                                     Type *result)         \
  {                                                                        \
    auto it = parser_syntax_##tabname##_table.find (str);                  \
    if (it == parser_syntax_##tabname##_table.end ())                      \
      return false;                                                        \
    tryset (result, it->second);                                           \
    return true;                                                           \
  }

/// NCL syntax table (grammar).
static map<string, ParserSyntaxElt> 
parser_syntax_table ={
{
  "ud:UD",                  // element name
  {nullptr,                 // push function
   nullptr,                // pop function
   0,                      // flags
   {},                     // possible parents
   { {"title", 0},         // attributes
     {"schemaLocation", 0},
     {"xmlns", 0}
   }
  },
},
{
      "UserID",                 // element name
      {nullptr,    // push function
        nullptr,    // pop function
        0,                      // flags
        {"ud:UD"},              // possible parents
        {}                      // attributes
      },
  },
  {
      "ud:UserProfile",         // element name
      {nullptr,                 // push function
        nullptr,                // pop function
        0,                      // flags
        {"ud:UD"},              // possible parents
        {"xsi:type", 0}         // attributes
      },
  },
  {
      "ud:User",                // element name
      {nullptr,                 // push function
        nullptr,                // pop function
        0,                      // flags
        {"ud:UserProfile"},     // possible parents
        {"xsi:type", 0}         // attributes
      },
  },
  {
      "ud:PersonInformation",   // element name
      {nullptr,                 // push function
        nullptr,                // pop function
        0,                      // flags
        {"ud:UserProfile"},     // possible parents
        {}         // attributes
      },
  },
  {
      "ud:Preference",   // element name
      {nullptr,                 // push function
        nullptr,                // pop function
        0,                      // flags
        {"ud:UD"},     // possible parents
        {}         // attributes
      },
  },
  {
      "ud:color",   // element name
      {nullptr,                 // push function
        nullptr,                // pop function
        0,                      // flags
        {"ud:Preference"},     // possible parents
        {}         // attributes
      },
  },
 {
      "ud:blind",   // element name
      {nullptr,                 // push function
        nullptr,                // pop function
        0,                      // flags
        {"ud:PersonInformation"},     // possible parents
        {}         // attributes
      },
  },
};

/// Indexes syntax table.
static bool
parser_syntax_table_index (const string &tag, ParserSyntaxElt **result)
{
  auto it = parser_syntax_table.find (tag);
  if (it == parser_syntax_table.end ())
    return false;
  tryset (result, &it->second);
  return true;
}

/// Gets possible children of a given element.
static map<string, bool>
parser_syntax_table_get_possible_children (const string &tag)
{
  map<string, bool> result;
  for (auto it : parser_syntax_table)
    for (auto parent : it.second.parents)
      if (parent == tag)
        result[it.first] = true;
  return result;
}


/// Parses boolean.
static bool
parser_syntax_parse_bool (const string &str, bool *result)
{
  return ginga::try_parse_bool (str, result);
}

// ParserElt: public.

/**
 * @brief Creates a new element.
 * @param node The node to wrap.
 * @return New #ParserElt.
 */
ParserElt::ParserElt (xmlNode *node)
{
  g_assert_nonnull (node);
  _node = node;
  _tag = toCPPString (node->name);
}

/**
 * @brief Destroys element.
 */
ParserElt::~ParserElt ()
{
}

/**
 * @brief Gets element tag.
 * @return Tag.
 */
string
ParserElt::getTag ()
{
  return _tag;
}

/**
 * @brief Gets element node.
 * @return Associated node.
 */
xmlNode *
ParserElt::getNode ()
{
  return _node;
}

/**
 * @brief Gets element parent node.
 * @return Parent of associated node.
 */
xmlNode *
ParserElt::getParentNode ()
{
  return _node->parent;
}

/**
 * @brief Gets element attribute map.
 * @return Attribute map.
 */
const map<string, string> *
ParserElt::getAttributes ()
{
  return &_attrs;
}

/**
 * @brief Gets element attribute value
 * @param name Attribute name.
 * @param[out] value Variable to store the attribute value (if any).
 * @return \c true if successful, or false otherwise.
 */
bool
ParserElt::getAttribute (const string &name, string *value)
{
  auto it = _attrs.find (name);
  if (it == _attrs.end ())
    return false;
  tryset (value, it->second);
  return true;
}

/**
 * @brief Sets element attribute value.
 * @param name Attribute name.
 * @param value Attribute value.
 * @return \c true if the attribute was previously unset, or \c false
 * otherwise.
 */
bool
ParserElt::setAttribute (const string &name, const string &value)
{
  bool result = !this->getAttribute (name, nullptr);
  _attrs[name] = value;
  return result;
}

/**
 * @brief Gets user data attached to element.
 * @param key User data key.
 * @param[out] value Variable to store the user data value (if any).
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserElt::getData (const string &key, void **value)
{
  return _udata.getData (key, value);
}

/**
 * @brief Attaches user data to element.
 * @param key User data key.
 * @param value User data value.
 * @param fn Cleanup function (used to destroy data when #ParserElt is
 * destroyed or key is overwritten).
 * @return \c true if key was previously unset, or \c false otherwise.
 */
bool
ParserElt::setData (const string &key, void *value, UserDataCleanFunc fn)
{
  return _udata.setData (key, value, fn);
}

// ParserState: private.

/**
 * @brief Generates an unique id.
 * @return Unique id.
 */
string
ParserState::genId ()
{
  string id = xstrbuild ("__unamed-%d__", _genid++);
  g_assert_false (isInUniqueSet (id));
  return id;
}

/**
 * @brief Gets the URI of current loaded XML file.
 * @return Directory part if there is a loaded XML file, or the empty string
 * otherwise.
 */
string
ParserState::getURI ()
{
  if (_xml == nullptr || _xml->URL == nullptr)
    return "";

  return toCPPString (_xml->URL);
}

/**
 * @brief Tests whether id is unique (hasn't been seen yet).
 * @param id The id to test.
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::isInUniqueSet (const string &id)
{
  return _unique.find (id) != _unique.end ();
}

/**
 * @brief Adds id to the set of known ids.
 * @param id The id to add.
 */
void
ParserState::addToUniqueSet (const string &id)
{
  _unique.insert (id);
}

/**
 * @brief Gets user data attached to parser state.
 * @param key User data key.
 * @param[out] value Variable to store user data value (if any).
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::getData (const string &key, void **value)
{
  return _udata.getData (key, value);
}

/**
 * @brief Attaches user data to parser state.
 * @param key User data key.
 * @param value User data value.
 * @param fn Cleanup function (used to destroy data when #ParserElt is
 * destroyed or key is overwritten).
 * @return \c true if key was previously unset, or \c false otherwise.
 */
bool
ParserState::setData (const string &key, void *value, UserDataCleanFunc fn)
{
  return _udata.setData (key, value, fn);
}

// ParserState: private (error handling).

/**
 * @brief Sets parser error code and error message.
 * @param node The node that caused the error.
 * @param error The error code to be set.
 * @param message The error message to be set.
 * @return \c false.
 */
bool
ParserState::errElt (xmlNode *node, ParserState::Error error,
                     const string &message)
{
  g_assert (error != ParserState::ERROR_NONE);
  g_assert_nonnull (node);

  _error = error;
  _errorMsg = "";
  if (node->doc->URL != nullptr)
    {
      string path = toCPPString (node->doc->URL);
      xmlChar *s = xmlBuildURI (toXmlChar (path), node->doc->URL);
      string uri = toCPPString (s);
      xmlFree (s);
      _errorMsg = uri + ": ";
    }
  _errorMsg
      += xstrbuild ("Element <%s> at line %d: ", toCString (node->name),
                    node->line)
         + message;
  return false;
}

/**
 * @brief Sets parser error to "Unknown element".
 * @param node The node that caused the error.
 * @return \c false.
 */
bool
ParserState::errEltUnknown (xmlNode *node)
{
  return this->errElt (node, ParserState::ERROR_ELT_UNKNOWN,
                       "Unknown element");
}

/**
 * @brief Sets parser error to "Missing parent".
 * @param node The node that caused the error.
 * @return \c false.
 */
bool
ParserState::errEltMissingParent (xmlNode *node)
{
  return this->errElt (node, ParserState::ERROR_ELT_MISSING_PARENT,
                       "Missing parent");
}

/**
 * @brief Sets parser error to "Bad parent".
 * @param node The node that caused the error.
 * @return \c false.
 */
bool
ParserState::errEltBadParent (xmlNode *node)
{
  string parent;
  g_assert_nonnull (node->parent);
  parent = toCPPString (node->parent->name);
  return this->errElt (node, ParserState::ERROR_ELT_BAD_PARENT,
                       "Bad parent <" + parent + ">");
}

/**
 * @brief Sets parser error to "Unknown attribute".
 * @param node The node that caused the error.
 * @param name The name of the attribute that caused the error.
 * @return \c false.
 */
bool
ParserState::errEltUnknownAttribute (xmlNode *node, const string &name)
{
  return this->errElt (node, ParserState::ERROR_ELT_UNKNOWN_ATTRIBUTE,
                       "Unknown attribute '" + name + "'");
}

/**
 * @brief Sets parser error to "Missing attribute".
 * @param node The node that caused the error.
 * @param name The name of the attribute that caused the error.
 * @return \c false.
 */
bool
ParserState::errEltMissingAttribute (xmlNode *node, const string &name)
{
  return this->errElt (node, ParserState::ERROR_ELT_MISSING_ATTRIBUTE,
                       "Missing attribute '" + name + "'");
}

/**
 * @brief Sets parser error to "Bad value for attribute".
 * @param node The node that caused the error.
 * @param name The name of the attribute that caused the error.
 * @param value The value of the attribute that caused the error.
 * @param explain Further explanation.
 * @return \c false.
 */
bool
ParserState::errEltBadAttribute (xmlNode *node, const string &name,
                                 const string &value, const string &explain)
{
  string msg = "Bad value '" + value + "' for attribute '" + name + "'";
  if (explain != "")
    msg += " (" + explain + ")";
  return this->errElt (node, ParserState::ERROR_ELT_BAD_ATTRIBUTE, msg);
}

/**
 * @brief Sets parser error to "Attributes are mutually exclusive".
 * @param node The node that caused the error.
 * @param attr1 The name of the first attribute.
 * @param attr2 The name of the second attribute.
 * @return \c false.
 */
bool
ParserState::errEltMutuallyExclAttributes (xmlNode *node,
                                           const string &attr1,
                                           const string &attr2)
{
  return this->errElt (node,
                       ParserState::ERROR_ELT_MUTUALLY_EXCL_ATTRIBUTES,
                       "Attributes '" + attr1 + "' and '" + attr2
                           + "' are mutually exclusive");
}

/**
 * @brief Sets parser error to "Unknown child".
 * @param node The node that caused the error.
 * @param name The name of the child element that caused the error.
 * @return \c false.
 */
bool
ParserState::errEltUnknownChild (xmlNode *node, const string &name)
{
  return this->errElt (node, ParserState::ERROR_ELT_UNKNOWN_CHILD,
                       "Unknown child <" + name + ">");
}

/**
 * @brief Sets parser error to "Missing child".
 * @param node The node that caused the error.
 * @param children The list of names of children that caused the error.
 * @return \c false.
 */
bool
ParserState::errEltMissingChild (xmlNode *node,
                                 const list<string> &children)
{
  string msg;
  auto it = children.begin ();

  g_assert (it != children.end ());
  msg = "Missing child <" + *it + ">";
  while (++it != children.end ())
    msg += " or <" + *it + ">";

  return this->errElt (node, ParserState::ERROR_ELT_MISSING_CHILD, msg);
}

/**
 * @brief Sets parser error to "Bad child".
 * @param node The node that caused the error.
 * @param name The name of the child that caused the error.
 * @param explain Further explanation.
 * @return \c false.
 */
bool
ParserState::errEltBadChild (xmlNode *node, const string &name,
                             const string &explain)
{
  string msg = "Bad child <" + name + ">";
  if (explain != "")
    msg += " (" + explain + ")";
  return this->errElt (node, ParserState::ERROR_ELT_BAD_CHILD, msg);
}

/**
 * @brief Sets parser error to "Syntax error in imported document".
 * @param node The \<importBase\> node that caused the error.
 * @param explain Further explanation.
 * return \c false
 */
bool
ParserState::errEltImport (xmlNode *node, const string &explain)
{
  string msg = "Syntax error in imported document";
  if (explain != "")
    msg += " (" + explain + ")";
  return this->errElt (node, ParserState::ERROR_ELT_IMPORT, msg);
}

// ParserState: private (element cache).

/**
 * @brief Indexes element cache by node.
 * @param node The node to be used as key.
 * @param[out] elt Variable to store the element associated with \p node (if
 * any).
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::eltCacheIndex (xmlNode *node, ParserElt **elt)
{
  auto it = _eltCache.find (node);
  if (it == _eltCache.end ())
    return false;
  g_assert_nonnull (it->second);
  tryset (elt, it->second);
  return true;
}

/**
 * @brief Indexes element cache by node parent.
 * @param node The node whose parent will be used as key.
 * @param[out] elt Variable to store the element associated with \p node (if
 * any).
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::eltCacheIndexParent (xmlNode *node, ParserElt **elt)
{
  return (node->parent != nullptr) ? this->eltCacheIndex (node->parent, elt)
                                   : false;
}

/**
 * @brief Indexes element cache by id.
 * @param id The id to be used as key.
 * @param[out] elt Variable to store the element associated with \p id (if
 * any).
 * @param tags Accepted tags (match only elements with one of these).
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::eltCacheIndexById (const string &id, ParserElt **elt,
                                const list<string> &tags)
{
  for (auto tag : tags)
    {
      auto it = _eltCacheByTag.find (tag);
      if (it == _eltCacheByTag.end ())
        continue;
      for (auto other : it->second)
        {
          string other_id;
          if (other->getAttribute ("id", &other_id) && other_id == id)
            {
              tryset (elt, other);
              return true;
            }
        }
    }
  return false;
}

/**
 * @brief Indexes element cache by tag.
 * @param tags The tags to be used as keys.
 * @param[out] result Variable to store the list of matched elements.
 * @return The number of matched elements.
 */
size_t
ParserState::eltCacheIndexByTag (const list<string> &tags,
                                 list<ParserElt *> *result)
{
  size_t n = 0;
  g_assert_nonnull (result);
  for (auto tag : tags)
    {
      auto it = _eltCacheByTag.find (tag);
      if (it == _eltCacheByTag.end ())
        continue;
      for (auto elt : it->second)
        {
          result->push_back (elt);
          n++;
        }
    }
  return n;
}

/**
 * @brief Adds element to cache.
 * @param elt The element to add.
 * @return \c true if successful, or \c false otherwise (already in cache).
 */
bool
ParserState::eltCacheAdd (ParserElt *elt)
{
  xmlNode *node;

  node = elt->getNode ();
  if (_eltCache.find (node) != _eltCache.end ())
    return false;
  _eltCache[node] = elt;
  _eltCacheByTag[elt->getTag ()].push_back (elt);
  return true;
}

// ParserState: private (alias stack).

/**
 * @brief Combines all aliases in alias stack.
 * @return The combined alias.
 */
string
ParserState::aliasStackCombine ()
{
  string result = "";
  for (auto &it : _aliasStack)
    result += it.first + "#";
  return result;
}

/**
 * @brief Peeks at alias stack.
 * @param alias Variable to store the alias at top of stack.
 * @param path Variable to store the path at top of stack.
 * @return \c true if successful, or \c false otherwise
 * (alias stack is empty).
 */
bool
ParserState::aliasStackPeek (string *alias, string *path)
{
  pair<string, string> top;

  if (_aliasStack.empty ())
    return false;

  top = _aliasStack.back ();
  tryset (alias, top.first);
  tryset (path, top.second);
  return true;
}

/**
 * @brief Pops alias stack.
 * @param alias Variable to store the popped alias.
 * @param path Variable to store the popped path.
 * @return \c true if successful, or \c false otherwise
 * (alias stack is empty).
 */
bool
ParserState::aliasStackPop (string *alias, string *path)
{
  if (!this->aliasStackPeek (alias, path))
    return false;
  _aliasStack.pop_back ();
  return true;
}

/**
 * @brief Pushes alias and path onto alias stack.
 * @param alias The alias to push.
 * @param path The path to push.
 * @return \c true if successful, or \c false otherwise
 * (path already in stack).
 */
bool
ParserState::aliasStackPush (const string &alias, const string &path)
{
  for (auto &it : _aliasStack)
    if (it.second == path)
      return false;
  _aliasStack.push_back (std::make_pair (alias, path));
  return true;
}

// ParserState: private (object stack).

/**
 * @brief Peeks at object stack.
 * @return The top of object stack or null if stack is empty.
 */
Object *
ParserState::objStackPeek ()
{
  return (_objStack.empty ()) ? nullptr : _objStack.back ();
}

/**
 * @brief Pops object from stack.
 * @return The popped object or null if the stack is empty.
 */
Object *
ParserState::objStackPop ()
{
  Object *obj = this->objStackPeek ();
  if (obj == nullptr)
    return nullptr;
  _objStack.pop_back ();
  return obj;
}

/**
 * @brief Pushes object onto stack.
 * @param obj The object to push.
 */
void
ParserState::objStackPush (Object *obj)
{
  g_assert_nonnull (obj);
  _objStack.push_back (obj);
}

// ParserState: private (rectangle stack).

/**
 * @brief Peeks at rectangle stack.
 * @return The top of rectangle stack.
 *
 * This function aborts when called with the empty stack.
 */
Rect
ParserState::rectStackPeek ()
{
  g_assert_false (_rectStack.empty ());
  return _rectStack.back ();
}

/**
 * @brief Pops rectangle from stack.
 * @return The popped rectangle.
 *
 * This function aborts when called with the empty stack.
 */
Rect
ParserState::rectStackPop ()
{
  Rect rect = this->rectStackPeek ();
  _rectStack.pop_back ();
  return rect;
}

/**
 * @brief Pushes rectangle onto stack.
 * @param rect The rectangle to push.
 */
void
ParserState::rectStackPush (Rect rect)
{
  _rectStack.push_back (rect);
}

// ParserState: private (refer map).

/**
 * @brief Indexes refer map.
 * @param id The id of the reference.
 * @param[out] media Variable to store referenced media.
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::referMapIndex (const string &id, Media **media)
{
  auto it = _referMap.find (id);
  if (it == _referMap.end ())
    return false;
  tryset (media, it->second);
  return true;
}

/**
 * @brief Adds entry to refer map.
 * @param id The id of the reference.
 * @param media The referenced media.
 * @return \c true if successful, or \c false otherwise (already in map).
 */
bool
ParserState::referMapAdd (const string &id, Media *media)
{
  auto it = _referMap.find (id);
  if (it != _referMap.end ())
    return false;
  _referMap[id] = media;
  return true;
}

// ParserState: private (reference solving).

/**
 * @brief Resolves reference to component within a context.
 *
 * This function assumes that \p elt has an attribute "component" which
 * contains the id of the component to be resolved.  In case resolution
 * fails, the function sets #Parser error accordingly.
 *
 * @param scope The composition that determines the resolution scope.
 * @param elt The element to be resolved.
 * @param[out] obj Variable to store the resulting object (if any).
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::resolveComponent (Composition *scope, ParserElt *elt,
                               Object **obj)
{
  string label;
  string comp;
  string refer;

  label = (elt->getTag () == "bindRule") ? "constituent" : "component";
  g_assert (elt->getAttribute (label, &comp));

  // Check if component refers to scope itself.
  if (comp == scope->getId () || scope->hasAlias (comp))
    {
      tryset (obj, scope);
      return true;
    }

  // Check if component refers to a child of scope.
  Object *child = scope->getChildByIdOrAlias (comp);
  if (child != nullptr)
    {
      tryset (obj, child);
      return true;
    }

  // Check if component refers to a reference (refer) object.
  Media *media;
  if (this->referMapIndex (comp, &media))
    {
      tryset (obj, media);
      return true;
    }

  // Not found.
  return this->errEltBadAttribute (elt->getNode (), label, comp,
                                   "no such object in scope");
}

/**
 * @brief Resolves reference to an interface within a context.
 *
 * This function assumes that \p elt has attributes "component" and
 * "interface" which identify the interface to be resolved.  In case
 * resolution fails, the function sets #Parser error accordingly.
 *
 * @param ctx The contexts that determines the resolution scope.
 * @param elt The element to be resolved.
 * @param[out] evt Variable to store the resulting event (if any).
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::resolveInterface (Composition *ctx, ParserElt *elt, Event **evt)
{
  string comp;
  string iface;
  Object *obj;
  Event *result;

  if (unlikely (!this->resolveComponent (ctx, elt, &obj)))
    return false;

  if (!elt->getAttribute ("interface", &iface))
    {
      result = obj->getLambda ();
      goto success;
    }

  result = nullptr;
  if (instanceof (Media *, obj))
    {
      result = obj->getPresentationEvent (iface);
      if (result == nullptr)
        {
          result = obj->getAttributionEvent (iface);
          if (unlikely (result == nullptr))
                goto fail;

        }
    }
  else if (instanceof (Context *, obj))
    {
      if (obj == ctx)
        {
          result = obj->getAttributionEvent (iface);
          if (unlikely (result == nullptr))
            goto fail;
        }
      else
        {
          ParserElt *iface_elt;
          ParserElt *parent_elt;

          if (unlikely (
                  !this->eltCacheIndexById (iface, &iface_elt, {"port"})))
            {
              result = obj->getAttributionEvent (iface);
              if (likely (result != nullptr))
                goto success; // interface point to context property
              else
                goto fail;
            }

          // Interface points to context port: resolve it recursively.
          g_assert (this->eltCacheIndexParent (iface_elt->getNode (),
                                               &parent_elt));
          if (parent_elt->getTag () == "body")
            {
              ctx = _doc->getRoot ();
            }
          else
            {
              string id;
              g_assert (parent_elt->getAttribute ("id", &id));
              ctx = cast (Context *, _doc->getObjectById (id));
              g_assert_nonnull (ctx);
            }

          if (ctx->getId () != obj->getId ())
            goto fail;

          return this->resolveInterface (ctx, iface_elt, evt);
        }
    }
  else if (instanceof (Switch *, obj))
    {
       result	
	          = obj->getPresentationEvent (iface); // A switchPort is resolved	
	                                               // as a PresentationEvent.	
	      if (unlikely (result == nullptr))	
	        goto fail;
    }
  else
    {
      g_assert_not_reached ();
    }

success:
  g_assert_nonnull (result);
  tryset (evt, result);
  return true;

fail:
  return this->errEltBadAttribute (elt->getNode (), "interface", iface,
                                   "no such interface");
}

/**
 * @brief Resolve reference to (bind, link, or ghost) parameter.
 *
 * The function uses the translation tables in the order they were given,
 * and returns as soon as any of them resolves the reference.
 *
 * @param ref The reference to resolve.
 * @param bindParams The bind parameter translation table.
 * @param linkParams The link parameter translation table.
 * @param ghosts The ghost binds translation table.
 * @return The resolved reference or \p ref itself if it is not a reference.
 */
string
ParserState::resolveParameter (const string &ref,
                               const map<string, string> *bindParams,
                               const map<string, string> *linkParams,
                               const map<string, string> *ghosts)
{
  string name;
  string result;

  if (ref[0] != '$')
    return ref; // nothing to do

  name = ref.substr (1, ref.length () - 1);
  auto it_bind = bindParams->find (name);
  if (it_bind != bindParams->end ())
    {
      result = it_bind->second;
    }
  else
    {
      auto it_link = linkParams->find (name);
      if (it_link != linkParams->end ())
        result = it_link->second;
      else
        return ""; // unknown reference
    }

  if (result[0] != '$')
    return result;

  name = result.substr (1, result.length () - 1);
  auto it_ghost = ghosts->find (name);
  if (it_ghost == ghosts->end ())
    return result; // unknown reference

  return it_ghost->second;
}

// ParserState: private (predicate solving).

/**
 * @brief Obtains predicate associated with a \<simpleCondition\> node.
 *
 * This function collects all predicates inherited by \c node and, if there
 * are such predicates, combines them into a new predicate and returns it.
 *
 * @param node The simple condition node.
 * @return A new predicate for \p node, or \c nullptr if \p node has no
 * associated predicate.
 */
Predicate *
ParserState::obtainPredicate (xmlNode *node)
{
  list<Predicate *> pred_list;
  ParserElt *elt;
  Predicate *pred;

  g_assert (toCPPString (node->name) == "simpleCondition");
  g_assert_nonnull (node->parent);
  node = node->parent;

  g_assert (this->eltCacheIndex (node, &elt));
  while (elt->getTag () != "causalConnector")
    {
      g_assert (elt->getTag () == "compoundCondition");
      if (elt->getData ("pred", (void **) &pred))
        pred_list.push_front (pred);
      g_assert (this->eltCacheIndexParent (elt->getNode (), &elt));
    }

  switch (pred_list.size ())
    {
    case 0: // no predicate
      {
        return nullptr;
      }
    case 1:
      { // single predicate
        pred = pred_list.back ();
        g_assert (pred->getType () == Predicate::CONJUNCTION);
        auto children = pred->getChildren ();
        switch (children->size ())
          {
          case 0:
            return nullptr;
          case 1:
            return children->front ()->clone ();
          default:
            return pred->clone ();
          }
        break;
      }
    default: // multiple predicates
      {
        pred = new Predicate (Predicate::CONJUNCTION);
        for (auto p : pred_list)
          {
            g_assert (p->getType () == Predicate::CONJUNCTION);
            auto children = p->getChildren ();
            switch (children->size ())
              {
              case 0:
                continue;
              case 1:
                pred->addChild (children->front ()->clone ());
                break;
              default:
                pred->addChild (p->clone ());
                break;
              }
          }
        return pred;
      }
    }
  g_assert_not_reached ();
}

/**
 * @brief Obtains predicate by solving role references in another predicate.
 *
 * This function does not modify predicate \p pred.
 *
 * @param pred The predicate to solve.
 * @param tr The role translation table mapping role references to property
 * references.
 * @return A new predicate obtained by solving \p pred via table \p tr.
 */
Predicate *
ParserState::solvePredicate (Predicate *pred, const map<string, string> *tr)
{
  list<Predicate *> buf;
  Predicate *result;

  result = pred->clone ();
  g_assert_nonnull (result);

  buf.push_back (result);
  while (!buf.empty ())
    {
      Predicate *p = buf.back ();
      buf.pop_back ();
      switch (p->getType ())
        {
        case Predicate::FALSUM:
        case Predicate::VERUM:
          break; // nothing to do
        case Predicate::ATOM:
          {
            Predicate::Test test;
            string left, right, ghost;
            p->getTest (&left, &test, &right);
            if (left[0] == '$')
              {
                auto it = tr->find (left.substr (1, left.length () - 1));
                if (it != tr->end ())
                  left = it->second;
              }
            if (right[0] == '$')
              {
                auto it = tr->find (right.substr (1, right.length () - 1));
                if (it != tr->end ())
                  right = it->second;
              }
            p->setTest (left, test, right);
            break;
          }
        case Predicate::NEGATION:
        case Predicate::CONJUNCTION:
        case Predicate::DISJUNCTION:
          for (auto child : *p->getChildren ())
            buf.push_back (child);
          break;
        default:
          g_assert_not_reached ();
        }
    }
  return result;
}

// ParserState: private (node processing).

/**
 * @brief Checks node syntax according to syntax table.
 * @param node The node to check.
 * @param[out] attrs Variable to store node's attributes.
 * @param[out] children Variable to store node's children.
 * @return Pointer to entry in syntax table if successful, otherwise returns
 * \c nullptr and sets #Parser error accordingly.
 */
ParserSyntaxElt *
ParserState::checkNode (xmlNode *node, map<string, string> *attrs,
                        list<xmlNode *> *children)
{
  string tag;
  ParserSyntaxElt *eltsyn;
  map<string, bool> possible;

  g_assert_nonnull (node);
  tag = toCPPString (node->name);

  // Check if element is known.
  if (unlikely (!parser_syntax_table_index (tag, &eltsyn)))
    return (this->errEltUnknown (node), nullptr);

  // Check parent.
  g_assert_nonnull (node->parent);
  if (eltsyn->parents.size () > 0)
    {
      string parent;
      bool found;

      if (unlikely (node->parent->type != XML_ELEMENT_NODE))
        return (this->errEltMissingParent (node), nullptr);

      parent = toCPPString (node->parent->name);
      found = false;
      for (auto par : eltsyn->parents)
        {
          if (parent == par)
            {
              found = true;
              break;
            }
        }
      if (unlikely (!found))
        return (this->errEltBadParent (node), nullptr);
    }

  // Collect attributes.
  for (auto attrsyn : eltsyn->attributes)
    {
      string value;
      string explain;

      if (!xmlGetPropAsString (node, attrsyn.name, &value)) // not found
        {
          if (attrsyn.name == "id" && eltsyn->flags & ELT_GEN_ID)
            {
              if (attrs != nullptr)
                (*attrs)["id"] = this->genId ();
              continue;
            }
          if (unlikely (attrsyn.flags & ATTR_REQUIRED))
            {
              return (this->errEltMissingAttribute (node, attrsyn.name),
                      nullptr);
            }
          else
            {
              continue;
            }
        }

      if (unlikely ((attrsyn.flags & ATTR_NONEMPTY) && value == ""))
        {
          return (this->errEltBadAttribute (node, attrsyn.name, value,
                                            "must not be empty"),
                  nullptr);
        }

      if (attrsyn.flags & ATTR_TYPE_NAME)
        {
          char offending;
          if (unlikely (!xmlIsValidName (value, &offending)))
            {
              return (this->errEltBadAttribute (
                          node, attrsyn.name, value,
                          xstrbuild ("must not contain '%c'", offending)),
                      nullptr);
            }
        }

      if ((attrsyn.flags & ATTR_TYPE_ID)
          || (attrsyn.flags & ATTR_TYPE_IDREF))
        {
          value = this->aliasStackCombine () + value;
        }

      if (attrsyn.flags & ATTR_UNIQUE)
        {
          if (unlikely (this->isInUniqueSet (value)))
            {
              return (this->errEltBadAttribute (node, attrsyn.name, value,
                                                "must be unique"),
                      nullptr);
            }
          else
            {
              this->addToUniqueSet (value);
            }
        }

      if (attrs != nullptr)
        (*attrs)[attrsyn.name] = value;
    }

  // Check for unknown attributes.
  if (attrs != nullptr)
    {
      for (xmlAttr *prop = node->properties; prop != nullptr;
           prop = prop->next)
        {
          string name = toCPPString (prop->name);
          if (unlikely (attrs->find (name) == attrs->end ()))
            return (this->errEltUnknownAttribute (node, name), nullptr);
        }
    }

  // Collect children.
  possible = parser_syntax_table_get_possible_children (tag);
  for (xmlNode *child = node->children; child; child = child->next)
    {
      if (child->type != XML_ELEMENT_NODE)
        continue;

      string child_tag = toCPPString (child->name);
      if (unlikely (possible.find (child_tag) == possible.end ()))
        return (this->errEltUnknownChild (node, child_tag), nullptr);

      if (children != nullptr)
        children->push_back (child);
    }

  return eltsyn;
}

/**
 * @brief Processes node.
 *
 * After being called by ParserState::process(), starting from the root
 * node, this function proceeds recursively, processing each node in the
 * input document tree.  For each node, it checks its syntax (according to
 * #parser_syntax_table), calls the corresponding push function (if any),
 * processes the node's children, and calls the corresponding pop function
 * (if any).  At any moment, if something goes wrong the function sets
 * #Parser error and returns false.
 *
 * @param node The node to process.
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::processNode (xmlNode *node)
{
  map<string, string> attrs;
  list<xmlNode *> children;
  ParserSyntaxElt *eltsyn;
  ParserElt *elt;
  bool cached;
  bool status;

  // Check node.
  eltsyn = this->checkNode (node, &attrs, &children);
  if (unlikely (eltsyn == nullptr))
    return false;

  // Allocate and initialize element wrapper.
  elt = new ParserElt (node);
  for (auto it : attrs)
      g_assert (elt->setAttribute (it.first, it.second));
  
  // Initialize flags.
  cached = false;
  status = true;

  // Push element.
  if (unlikely (eltsyn->push != nullptr && !eltsyn->push (this, elt)))
    {
      status = false;
      goto done;
    }

  // Save element into cache.
  if (eltsyn->flags & ELT_CACHE)
    {
      cached = true;
      g_assert (this->eltCacheAdd (elt));
    }

  // Process each child.
  for (auto child : children)
    {
      if (unlikely (!this->processNode (child)))
        {
          status = false;
          goto done;
        }
    }

  // Pop element.
  if (unlikely (eltsyn->pop != nullptr && !eltsyn->pop (this, elt)))
    {
      status = false;
      goto done;
    }

done:
  if (!cached)
    delete elt;
  return status;
}

// ParserState: public.

/**
 * @brief Creates a new state.
 * @param width Initial screen width (in pixels).
 * @param height Initial screen height (in pixels).
 * @return New #ParserState.
 */
ParserState::ParserState (int width, int height)
{
  _doc = nullptr;
  _xml = nullptr;
  g_assert_cmpint (width, >, 0);
  g_assert_cmpint (height, >, 0);
  _genid = 0;
  _error = ParserState::ERROR_NONE;
  _errorMsg = "no error";
  this->rectStackPush ({0, 0, width, height});
}

/**
 * @brief Destroys state.
 */
ParserState::~ParserState ()
{
  for (auto it : _eltCache)
    delete it.second;
}

/**
 * @brief Gets last parser error.
 * @param[out] message Variable to store the last error message (if any).
 * @return Last error code.
 */
ParserState::Error
ParserState::getError (string *message)
{
  tryset (message, _errorMsg);
  return _error;
}

/**
 * @brief Processes XML document.
 *
 * This function is a wrapper over ParserState::processNode().  It calls the
 * later with the root node of the given XML document and, if nothing goes
 * wrong, returns the resulting document.
 *
 * @param xml The XML document to process.
 * @return The resulting #Document if successful, or null otherwise.
 */
Document *
ParserState::process (xmlDoc *xml)
{
  xmlNode *root;

  g_assert_nonnull (xml);
  _xml = xml;
  _doc = new Document ();

  root = xmlDocGetRootElement (xml);
  g_assert_nonnull (root);

  if (unlikely (!this->processNode (root)))
    {
      delete _doc;
      _doc = nullptr;
      return nullptr;
    }

  g_assert_nonnull (_doc);
  return _doc;
}

// ParserState: push & pop.

/**
 * @brief Starts the processing of \<ncl\> element.
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::pushNcl (ParserState *st, ParserElt *elt)
{
  Context *root;
  string id;

  root = st->_doc->getRoot ();
  g_assert_nonnull (root);

  if (elt->getAttribute ("id", &id))
    root->addAlias (id);
  
  st->objStackPush (root);
  return true;
}

/**
 * @brief Ends the processing of \<ncl\> element.
 *
 * This function resolves all non-local references.
 *
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */
bool
ParserState::popNcl (ParserState *st, unused (ParserElt *elt))
{
  list<ParserElt *> userAgent_list;
  list<ParserElt *> userProfile_list;

 
 
    if (st->eltCacheIndexByTag ({"userAgent"}, &userAgent_list) > 0)
  {
//printf("\n Tem userAgents!!!!!!!!!!!!!!!!**********\n");
   for (auto userAgent_elt : userAgent_list)
        {
          user _userAgent;
          g_assert (userAgent_elt->getAttribute ("id", &_userAgent.id));
          g_assert (userAgent_elt->getAttribute ("profile", &_userAgent.profile));
          userAgent_elt->getAttribute ("src", &_userAgent.src);
          userAgent_elt->getAttribute ("type", &_userAgent.type);

          st->_doc->addUser(_userAgent);
        
        }
  }
  else
  {
//printf("\n não tem userAgents!!!!!!!!!!!!!!!!**********\n");
  }
  if (st->eltCacheIndexByTag ({"userProfile"}, &userProfile_list) > 0)
  {
//    printf("\n Tem Profiles!!!!!!!!!!!!!!!!**********\n");
    for (auto userProfile_elt : userProfile_list)
        {
          profile _userProfile;
          g_assert (userProfile_elt->getAttribute ("id", &_userProfile.id));
          userProfile_elt->getAttribute ("src", &_userProfile.src);
          userProfile_elt->getAttribute ("type", &_userProfile.type);
          userProfile_elt->getAttribute ("min", &_userProfile.min);
          userProfile_elt->getAttribute ("max", &_userProfile.max);

          st->_doc->addProfile(_userProfile);

        }
  }
  else
  {
//printf("\n não tem Profiles!!!!!!!!!!!!!!!!**********\n");
  }
  

//  st->_presOrch->createPresentationPlan(st->_htg);
  g_assert_nonnull (st->objStackPop ());
  
  return true;
}



/**
 * @brief Starts the processing of \<causalConnector\> element.
 * @fn ParserState::pushCausalConnector
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */

/// Cleans up the set of test roles attached to connector #ParserElt.
static void
testsCleanup (void *ptr)
{
  delete (set<string> *) ptr;
}



/**
 * @brief Starts the processing of \<compoundCondition\> element.
 * @fn ParserState::pushCompoundCondition
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */

/// Cleans up the #Predicate attached to compound condition #ParserElt.
static void
predCleanup (void *ptr)
{
  delete (Predicate *) ptr;
}

/**
 * @brief Starts the processing of \<attributeAssessment\> or
 * \<valueAssessment\> element.
 * @fn ParserState::pushAttributeAssessment
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */

/// Cleans up the strings attached to attribute assessment #ParserElt.
static void
leftOrRightCleanup (void *ptr)
{
  delete (string *) ptr;
}

/**
 * @brief Starts the processing of \<rule\> or \<compositeRule\> element.
 * @fn ParserState::pushRule
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */

/// Cleans up the #Predicate attached to rule or composite rule #ParserElt.
static void
rulePredCleanup (void *ptr)
{
  delete (Predicate *) ptr;
}


/**
 * @brief Starts the processing of \<importBase\> element.
 *
 * This function uses the #ParserState alias stack to collect and process
 * nested imports.
 *
 * @fn ParserState::pushImportBase
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 *
 * @todo Check for circular imports.
 */

/// Cleans up the document associated with \<importBase\> element.
static void
xmlDocCleanup (void *ptr)
{
  xmlFreeDoc ((xmlDoc *) ptr);
}

/**
 * @brief Starts the processing of \<body\> or \<context\> element.
 *
 * This function parses \p elt and pushes it as a #Context onto the object
 * stack.
 *
 * @fn ParserState::pushContext
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */

/// Cleans up the list of port ids attached to context #ParserElt.
static void
portsCleanup (void *ptr)
{
  delete (list<string> *) ptr;
}


/**
 * @brief Starts the processing of \<switch\>.
 *
 * This function parsers \p elt and pushes it as a #Switch onto the object
 * stack.
 *
 * @fn ParserState::pushSwitch
 * @param st #ParserState.
 * @param elt Element wrapper.
 * @return \c true if successful, or \c false otherwise.
 */

/// Cleans up the rules attached to switch #ParserElt.
static void
rulesCleanup (void *ptr)
{
  delete (list<pair<ParserElt *, Object *> > *) ptr;
}


/// Cleans up the mapping list cache attached to a switchPort #ParserElt.	
	static void	
	mappingsCleanup (void *ptr)	
	{	
	  delete (list<const ParserElt *> *) ptr;	
	}	
		


/// Cleans up parameter map attached to link #ParserElt.
static void
paramsCleanup (void *ptr)
{
  delete (map<string, string> *) ptr;
}

Document*
ParserState::getDoc()
{
	return _doc;
}


// External API.

/// Helper function used by Parser::parseBuffer() and Parser::parseFile().
static UserDocument *
process (xmlDoc *xml, int width, int height, string *errmsg)
{
  ParserState st (width, height);
  Document *doc;

  doc = st.process (xml);
  if (unlikely (doc == nullptr))
    {
      g_assert (st.getError (errmsg) != ParserState::ERROR_NONE);
      return nullptr;
    }

  return doc;
}

/**
 * @brief Parses NCL document from memory buffer.
 * @fn Parser::parseBuffer
 * @param buf Buffer.
 * @param size Buffer size in bytes.
 * @param width Initial screen width (in pixels).
 * @param height Initial screen height (in pixels).
 * @param[out] errmsg Variable to store the error message (if any).
 * @return The resulting #Document if successful, or null otherwise.
 */
Document *
ParserUserDescription::parseBuffer (const void *buf, size_t size, int width, int height,
                     string *errmsg)
{
  xmlDoc *xml;
  Document *doc;

  xml = xmlReadMemory ((const char *) buf, (int) size, nullptr, nullptr,
                       PARSER_LIBXML_FLAGS);
  if (unlikely (xml == nullptr))
    {
      tryset (errmsg, xmlGetLastErrorAsString ());
      return nullptr;
    }

  doc = process (xml, width, height, errmsg);
  xmlFreeDoc (xml);
  return doc;
}

/**
 * @brief Parses NCL document from file.
 * @param path File path.
 * @param width Initial screen width (in pixels).
 * @param height Initial screen height (in pixels).
 * @param[out] errmsg Variable to store the error message (if any).
 * @return The resulting #Document if successful, or null otherwise.
 */
Document *
ParserUserDescription::parseFile (const string &path, int width, int height,
                   string *errmsg)
{
  xmlDoc *xml;
  UserDocument *doc;
  string uri = path;

  // Makes the path absolute based in the current dir
  if (!xpathisabs (path))
    uri = xpathmakeabs (path);

  uri = xurifromsrc (uri, "");

  xml = xmlReadFile (uri.c_str (), nullptr, PARSER_LIBXML_FLAGS);
  if (unlikely (xml == nullptr))
    {
      tryset (errmsg, xmlGetLastErrorAsString ());

      return nullptr;
    }
printf("\n Xml valido!");
  doc = process (xml, width, height, errmsg);
  xmlFreeDoc (xml);

  return doc;
}

//GINGA_NAMESPACE_END
