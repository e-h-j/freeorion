// -*- C++ -*-
#ifndef _MultiplayerCommon_h_
#define _MultiplayerCommon_h_

#include "GGClr.h"
#include "XMLDoc.h"

#include <vector>

/** The colors that are available for use for empires in the game. */
const std::vector<GG::Clr>& EmpireColors();

/** Returns the integer priority level that should be passed to log4cpp for a given priority name string. */
int PriorityValue(const std::string& name);

/** Returns a vector of the names of all settings files that must be the same between the server and clients. */
const std::vector<std::string>& VersionSensitiveSettingsFiles();

/** Returns a map of all source files to their CVS revision numbers.  This is used to verify that the
    server and clients are all using the same versions of code. */
const std::map<std::string, std::string>& SourceFiles();

/** Saves the filename \a filename and its revision number string for later retrieval via SourceFiles().
    This function returns a dummy boolean value, allowing it to be executed at static initialization time,
    via a hack -- simply declare a file-scope bool variable, and initialize it: bool temp_bool =
    RecordSourceFile(...).  If the same filename is registered more than once, an exception will be thrown.
    \see RecordHeaderFile () has more info on how it and this function should be used. */
bool RecordSourceFile(const std::string& filename, const std::string& revision);

/** Identical to RecordSourceFile(), except that it takes a std::pair of std::strings, instead of two
    individual std::strings.  Header files should declare an inline function that returns a pair specifying
    its CVS-expanded filename and revision string, and the header file's matching source file should record
    that info using this function.  Example:<br>In MultiplayerCommon.h:
    \verbatim
    inline std::pair<std::string, std::string> MultiplayerCommonRevision()
    {return std::pair<std::string, std::string>("$RCSfile$", "$Revision$");}
    \endverbatim<br>In MultiplayerCommon.cpp:
    \verbatim
    namespace {
    ...
    bool temp_header_bool = RecordHeaderFile(MultiplayerCommonRevision());
    bool temp_source_bool = RecordSourceFile("$RCSfile$", "$Revision$");
    }
    \endverbatim
*/
bool RecordHeaderFile(const std::pair<std::string, std::string>& filename_revision_pair);

/** Returns an MD5 "sum" of the given string as a 32-digithexidecimal string. */
std::string MD5StringSum(const std::string& str);

/** Returns an MD5 "sum" of the given text file as a 32-digit hexidecimal string. */
std::string MD5FileSum(const std::string& filename);

/** The data for one empire necessary for game-setup during multiplayer loading. */
struct SaveGameEmpireData
{
    /** \name Structors */ //@{
    SaveGameEmpireData(); ///< default ctor.
    SaveGameEmpireData(const GG::XMLElement& elem); ///< GG::XMLElement ctor.
    //@}

    /** \name Accessors */ //@{
    GG::XMLElement XMLEncode();
    //@}

    int         id;
    std::string name;
    std::string player_name;
    GG::Clr     color;
};

/** The data structure used to represent a single player's setup options for a multiplayer game (in the multiplayer lobby screen). */
struct PlayerSetupData
{
    /** \name Structors */ //@{
    PlayerSetupData(); ///< default ctor.
    PlayerSetupData(const GG::XMLElement& elem); ///< GG::XMLElement ctor.
    //@}

    /** \name Accessors */ //@{
    GG::XMLElement XMLEncode() const;
    //@}

    std::string empire_name;  ///< the name of the player's empire
    GG::Clr empire_color;     ///< the color used to represent this player's empire.
    int save_game_empire_id;  ///< when an MP save game is being loaded, this is the id of the empire that this player will play
};

inline std::pair<std::string, std::string> MultiplayerCommonRevision()
{return std::pair<std::string, std::string>("$RCSfile$", "$Revision$");}

#endif // _MultiplayerCommon_h_
