// -*- C++ -*-
#ifndef _MultiplayerLobbyWnd_h_
#define _MultiplayerLobbyWnd_h_

#ifndef _CUI_Wnd_h_
#include "CUI_Wnd.h"
#endif

#ifndef _GalaxySetupWnd_h_
#include "GalaxySetupWnd.h"
#endif

#include <vector>

class CUIButton;
class CUIDropDownList;
class CUIEdit;
class CUIListBox;
class CUIMultiEdit;
class Message;
namespace GG {
class StaticGraphic;
class TextControl;
}

/** multiplayer lobby window */
class MultiplayerLobbyWnd : public CUI_Wnd
{
public:
    /** \name Structors */ //@{
    MultiplayerLobbyWnd(bool host);
    MultiplayerLobbyWnd(const GG::XMLElement& elem);
    virtual ~MultiplayerLobbyWnd();
    //@}
    
    /** \name Accessors */ //@{
    bool Result() const {return m_result;}  ///< returns true iff a new game was successfully launched from the lobby
    //@}

    /** \name Mutators */ //@{
    virtual bool Render();
    virtual void Keypress(GG::Key key, Uint32 key_mods);

    void         HandleMessage(const Message& msg);
    void         Cancel() {CancelClicked();}
    //@}

private:
    void Init();
    void AttachSignalChildren();
    void DetachSignalChildren();
    void NewLoadClicked(int idx);
    void GalaxySetupPanelChanged();
    void SaveGameChanged(int idx);
    void PreviewImageChanged(boost::shared_ptr<GG::Texture> new_image);
    void PlayerSelected(const std::set<int>& selections);
    void PlayerDataChanged();
    void StartGameClicked();
    void CancelClicked();
    bool PlayerDataUnique();
    void SendUpdate();
    GG::XMLDoc LobbyUpdateDoc() const;

    bool m_result;

    bool m_host;
    std::map<std::string, int> m_player_IDs;
    std::map<int, std::string> m_player_names;

    CUIMultiEdit*         m_chat_box;
    CUIEdit*              m_chat_input_edit;
    GG::RadioButtonGroup* m_new_load_game_buttons;
    GalaxySetupPanel*     m_galaxy_setup_panel;
    CUIDropDownList*      m_saved_games_list;
    GG::StaticGraphic*    m_preview_image;
    CUIListBox*           m_players_lb;
    CUIButton*            m_start_game_bn;
    CUIButton*            m_cancel_bn;
};

#endif // _MultiplayerLobbyWnd_h_
