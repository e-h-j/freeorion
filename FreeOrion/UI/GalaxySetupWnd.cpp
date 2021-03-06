#include "GalaxySetupWnd.h"

#include "CUIControls.h"
#include "CUISpin.h"
#include "Sound.h"
#include "../universe/Universe.h"
#include "../client/human/HumanClientApp.h"
#include "../util/i18n.h"
#include "../util/Logger.h"
#include "../util/MultiplayerCommon.h"
#include "../util/OptionsDB.h"
#include "../util/Directories.h"

#include <boost/filesystem/fstream.hpp>

#include <GG/DrawUtil.h>
#include <GG/StaticGraphic.h>


namespace {
    const GG::X CONTROL_MARGIN(5);
    const GG::Y CONTROL_HEIGHT(30);
    const GG::Y PANEL_CONTROL_SPACING(33);
    const GG::Y GAL_SETUP_PANEL_HT(PANEL_CONTROL_SPACING * 10);
    const GG::X GAL_SETUP_WND_WD(645);
    const GG::Y GAL_SETUP_WND_HT(29 + (PANEL_CONTROL_SPACING * 6) + GAL_SETUP_PANEL_HT);
    const GG::Pt PREVIEW_SZ(GG::X(248), GG::Y(186));
    const bool ALLOW_NO_STARLANES = false;
    const int MAX_AI_PLAYERS = 20;

    // persistant between-executions galaxy setup settings, mainly so I don't have to redo these settings to what I want every time I run FO to test something
    void AddOptions(OptionsDB& db) {
        db.Add("GameSetup.seed",                UserStringNop("OPTIONS_DB_GAMESETUP_SEED"),                    std::string("0"),   Validator<std::string>());
        db.Add("GameSetup.stars",               UserStringNop("OPTIONS_DB_GAMESETUP_STARS"),                   150,                 RangedValidator<int>(10, 5000));
        db.Add("GameSetup.galaxy-shape",        UserStringNop("OPTIONS_DB_GAMESETUP_GALAXY_SHAPE"),            ELLIPTICAL,         RangedValidator<Shape>(SPIRAL_2, RANDOM));
        db.Add("GameSetup.galaxy-age",          UserStringNop("OPTIONS_DB_GAMESETUP_GALAXY_AGE"),              GALAXY_SETUP_MEDIUM,RangedValidator<GalaxySetupOption>(GALAXY_SETUP_LOW, GALAXY_SETUP_HIGH));
        db.Add("GameSetup.planet-density",      UserStringNop("OPTIONS_DB_GAMESETUP_PLANET_DENSITY"),          GALAXY_SETUP_MEDIUM,RangedValidator<GalaxySetupOption>(GALAXY_SETUP_LOW, GALAXY_SETUP_HIGH));
        db.Add("GameSetup.starlane-frequency",  UserStringNop("OPTIONS_DB_GAMESETUP_STARLANE_FREQUENCY"),      GALAXY_SETUP_MEDIUM,RangedValidator<GalaxySetupOption>(ALLOW_NO_STARLANES ? GALAXY_SETUP_NONE : GALAXY_SETUP_LOW, GALAXY_SETUP_HIGH));
        db.Add("GameSetup.specials-frequency",  UserStringNop("OPTIONS_DB_GAMESETUP_SPECIALS_FREQUENCY"),      GALAXY_SETUP_MEDIUM,RangedValidator<GalaxySetupOption>(GALAXY_SETUP_NONE, GALAXY_SETUP_HIGH));
        db.Add("GameSetup.monster-frequency",   UserStringNop("OPTIONS_DB_GAMESETUP_MONSTER_FREQUENCY"),       GALAXY_SETUP_MEDIUM,RangedValidator<GalaxySetupOption>(GALAXY_SETUP_NONE, GALAXY_SETUP_HIGH));
        db.Add("GameSetup.native-frequency",    UserStringNop("OPTIONS_DB_GAMESETUP_NATIVE_FREQUENCY"),        GALAXY_SETUP_MEDIUM,RangedValidator<GalaxySetupOption>(GALAXY_SETUP_NONE, GALAXY_SETUP_HIGH));
        db.Add("GameSetup.empire-name",         UserStringNop("OPTIONS_DB_GAMESETUP_EMPIRE_NAME"),             std::string(""),    Validator<std::string>());
        db.Add("GameSetup.player-name",         UserStringNop("OPTIONS_DB_GAMESETUP_PLAYER_NAME"),             std::string(""),    Validator<std::string>());
        db.Add("GameSetup.empire-color",        UserStringNop("OPTIONS_DB_GAMESETUP_EMPIRE_COLOR"),            0,                  RangedValidator<int>(0, 100));
        db.Add("GameSetup.starting-species",    UserStringNop("OPTIONS_DB_GAMESETUP_STARTING_SPECIES_NAME"),   std::string("SP_HUMAN"),    Validator<std::string>());
        db.Add("GameSetup.ai-players",          UserStringNop("OPTIONS_DB_GAMESETUP_NUM_AI_PLAYERS"),          6,                  RangedValidator<int>(0, MAX_AI_PLAYERS));
        db.Add("GameSetup.ai-aggression",       UserStringNop("OPTIONS_DB_GAMESETUP_AI_MAX_AGGRESSION"),       MANIACAL,           RangedValidator<Aggression>(BEGINNER, MANIACAL));
    }
    bool temp_bool = RegisterOptions(&AddOptions);
}

////////////////////////////////////////////////
// GalaxySetupPanel
////////////////////////////////////////////////
const GG::X GalaxySetupPanel::DEFAULT_WIDTH(305);

GalaxySetupPanel::GalaxySetupPanel(GG::X x, GG::Y y, GG::X w/* = DEFAULT_WIDTH*/) :
    GG::Control(x, y, w, GAL_SETUP_PANEL_HT, GG::Flags<GG::WndFlag>()),
    m_seed_edit(0),
    m_random(0),
    m_stars_spin(0),
    m_galaxy_shapes_list(0),
    m_galaxy_ages_list(0),
    m_starlane_freq_list(0),
    m_planet_density_list(0),
    m_specials_freq_list(0),
    m_monster_freq_list(0),
    m_native_freq_list(0),
    m_ai_aggression_list(0)
{
    Sound::TempUISoundDisabler sound_disabler;

    const GG::X LABELS_WIDTH = (w - CONTROL_MARGIN) / 2;
    const GG::X DROPLIST_WIDTH = LABELS_WIDTH;
    const GG::Y DROPLIST_HEIGHT(ClientUI::Pts() + 4);
    const GG::Y TEXT_ROW_HEIGHT = CUISimpleDropDownListRow::DEFAULT_ROW_HEIGHT;
    const GG::Y MAX_DROPLIST_DROP_HEIGHT = TEXT_ROW_HEIGHT * 5;
    const int TOTAL_LISTBOX_MARGIN = 4;
    int row = -1;

    boost::shared_ptr<GG::Font> font = ClientUI::GetFont();

    // seed
    GG::TextControl* label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_SEED"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.seed")));
    AttachChild(label);
    m_seed_edit = new CUIEdit(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING, LABELS_WIDTH -30, GetOptionsDB().Get<std::string>("GameSetup.seed"));
    m_seed_edit->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_seed_edit->Height()) / 2));

    // random seed button
    m_random = new CUIButton(2 * LABELS_WIDTH + 3 * CONTROL_MARGIN - 30, row * PANEL_CONTROL_SPACING + 5, GG::X(20), "", font, GG::CLR_WHITE);
    m_random->SetUnpressedGraphic(GG::SubTexture(ClientUI::GetTexture(ClientUI::ArtDir() / "icons" / "buttons" / "randomize.png")));
    m_random->SetPressedGraphic  (GG::SubTexture(ClientUI::GetTexture(ClientUI::ArtDir() / "icons" / "buttons" / "randomize_clicked.png"  )));
    m_random->SetRolloverGraphic (GG::SubTexture(ClientUI::GetTexture(ClientUI::ArtDir() / "icons" / "buttons" / "randomize_mouseover.png")));
    m_random->SetBrowseText(UserString("GSETUP_RANDOM_SEED"));
    m_random->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));

    // number of stars
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_STARS"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.stars")));
    AttachChild(label);
    m_stars_spin = new CUISpin<int>(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING,
                                    GG::X(75), 100, 1, 10, 5000, true);
    m_stars_spin->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_stars_spin->Height()) / 2));

    // galaxy shape
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT,
                                UserString("GSETUP_SHAPE"), font, ClientUI::TextColor(),
                                GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.galaxy-shape")));
    AttachChild(label);
    GG::Y drop_height = std::min(TEXT_ROW_HEIGHT * GALAXY_SHAPES, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_galaxy_shapes_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING,
                                               DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_galaxy_shapes_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_galaxy_shapes_list->Height()) / 2));
    m_galaxy_shapes_list->SetStyle(GG::LIST_NOSORT);

    // galaxy age
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT,
                                UserString("GSETUP_AGE"), font, ClientUI::TextColor(),
                                GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.galaxy-age")));
    AttachChild(label);
    drop_height = std::min(TEXT_ROW_HEIGHT * NUM_GALAXY_SETUP_OPTIONS, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_galaxy_ages_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING,
                                             DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_galaxy_ages_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_galaxy_ages_list->Height()) / 2));
    m_galaxy_ages_list->SetStyle(GG::LIST_NOSORT);

    // starlane frequency
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_STARLANE_FREQ"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.starlane-frequency")));
    AttachChild(label);
    drop_height = std::min(TEXT_ROW_HEIGHT * NUM_GALAXY_SETUP_OPTIONS, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_starlane_freq_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING,
                                               DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_starlane_freq_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_starlane_freq_list->Height()) / 2));
    m_starlane_freq_list->SetStyle(GG::LIST_NOSORT);

    // planet density
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT,
                                UserString("GSETUP_PLANET_DENSITY"), font, ClientUI::TextColor(),
                                GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.planet-density")));
    AttachChild(label);
    drop_height = std::min(TEXT_ROW_HEIGHT * NUM_GALAXY_SETUP_OPTIONS, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_planet_density_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING,
                                                DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_planet_density_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_planet_density_list->Height()) / 2));
    m_planet_density_list->SetStyle(GG::LIST_NOSORT);

    // specials frequency
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_SPECIALS_FREQ"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.specials-frequency")));
    AttachChild(label);
    drop_height = std::min(TEXT_ROW_HEIGHT * NUM_GALAXY_SETUP_OPTIONS, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_specials_freq_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING, DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_specials_freq_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_specials_freq_list->Height()) / 2));
    m_specials_freq_list->SetStyle(GG::LIST_NOSORT);

    // monster frequency
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_MONSTER_FREQ"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.monster-frequency")));
    AttachChild(label);
    drop_height = std::min(TEXT_ROW_HEIGHT * NUM_GALAXY_SETUP_OPTIONS, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_monster_freq_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING, DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_monster_freq_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_monster_freq_list->Height()) / 2));
    m_monster_freq_list->SetStyle(GG::LIST_NOSORT);

    // native frequency
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_NATIVE_FREQ"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.native-frequency")));
    AttachChild(label);
    drop_height = std::min(TEXT_ROW_HEIGHT * NUM_GALAXY_SETUP_OPTIONS, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_native_freq_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING, DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_native_freq_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_native_freq_list->Height()) / 2));
    m_native_freq_list->SetStyle(GG::LIST_NOSORT);

    // ai aggression
    label = new GG::TextControl(CONTROL_MARGIN, ++row * PANEL_CONTROL_SPACING, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_AI_AGGR"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.ai-aggression")));
    AttachChild(label);
    drop_height = std::min(TEXT_ROW_HEIGHT * NUM_AI_AGGRESSION_LEVELS, MAX_DROPLIST_DROP_HEIGHT) + TOTAL_LISTBOX_MARGIN;
    m_ai_aggression_list = new CUIDropDownList(LABELS_WIDTH + 2 * CONTROL_MARGIN, row * PANEL_CONTROL_SPACING, DROPLIST_WIDTH, DROPLIST_HEIGHT, drop_height);
    m_ai_aggression_list->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_ai_aggression_list->Height()) / 2));
    m_ai_aggression_list->SetStyle(GG::LIST_NOSORT);

    Init();
}

namespace {
    // set of characters from which to generate random seed that excludes some ambiguous letter/number pairs
    static char alphanum[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
}

void GalaxySetupPanel::RandomClicked() {
    std::string s;
    ClockSeed(); // to ensure we don't always get the same sequence of seeds
    for (int i = 0; i < 8; ++i)
        s += alphanum[ RandSmallInt(0, (sizeof(alphanum) - 2))];
    m_seed_edit->SetText(s);
    //std::cout << "GalaxySetupPanel::RandomClicked() new seed: " << s << std::endl;
}

const std::string& GalaxySetupPanel::GetSeed() const
{ return m_seed_edit->Text(); }

int GalaxySetupPanel::Systems() const
{ return m_stars_spin->Value(); }

Shape GalaxySetupPanel::GetShape() const
{ return Shape(m_galaxy_shapes_list->CurrentItemIndex()); }

GalaxySetupOption GalaxySetupPanel::GetAge() const
{ return GalaxySetupOption(m_galaxy_ages_list->CurrentItemIndex() + 1); }

GalaxySetupOption GalaxySetupPanel::GetStarlaneFrequency() const
{ return GalaxySetupOption(m_starlane_freq_list->CurrentItemIndex() + (ALLOW_NO_STARLANES ? 0 : 1)); }

GalaxySetupOption GalaxySetupPanel::GetPlanetDensity() const
{ return GalaxySetupOption(m_planet_density_list->CurrentItemIndex() + 1); }

GalaxySetupOption GalaxySetupPanel::GetSpecialsFrequency() const
{ return GalaxySetupOption(m_specials_freq_list->CurrentItemIndex()); }

GalaxySetupOption GalaxySetupPanel::GetMonsterFrequency() const
{ return GalaxySetupOption(m_monster_freq_list->CurrentItemIndex()); }

GalaxySetupOption GalaxySetupPanel::GetNativeFrequency() const
{ return GalaxySetupOption(m_native_freq_list->CurrentItemIndex()); }

Aggression GalaxySetupPanel::GetAIAggression() const
{ return Aggression(m_ai_aggression_list->CurrentItemIndex()); }

boost::shared_ptr<GG::Texture> GalaxySetupPanel::PreviewImage() const
{ return m_textures[GetShape()]; }

void GalaxySetupPanel::Disable(bool b/* = true*/) {
    for (std::list<GG::Wnd*>::const_iterator it = Children().begin(); it != Children().end(); ++it)
        static_cast<GG::Control*>(*it)->Disable(b);
}

void GalaxySetupPanel::SetFromSetupData(const GalaxySetupData& setup_data) {
    m_seed_edit->SetText(setup_data.m_seed);
    m_stars_spin->SetValue(setup_data.m_size);
    m_galaxy_shapes_list->Select(setup_data.m_shape);
    ShapeChanged(m_galaxy_shapes_list->CurrentItem());
    m_galaxy_ages_list->Select(setup_data.m_age - 1);
    m_starlane_freq_list->Select(setup_data.m_starlane_freq - (ALLOW_NO_STARLANES ? 0 : 1));
    m_planet_density_list->Select(setup_data.m_planet_density - 1);
    m_specials_freq_list->Select(setup_data.m_specials_freq);
    m_monster_freq_list->Select(setup_data.m_monster_freq);
    m_native_freq_list->Select(setup_data.m_native_freq);
    m_ai_aggression_list->Select(setup_data.m_ai_aggr);
}

void GalaxySetupPanel::GetSetupData(GalaxySetupData& setup_data) const {
    setup_data.m_seed =             GetSeed();
    setup_data.m_size =             Systems();
    setup_data.m_shape =            GetShape();
    setup_data.m_age =              GetAge();
    setup_data.m_starlane_freq =    GetStarlaneFrequency();
    setup_data.m_planet_density =   GetPlanetDensity();
    setup_data.m_specials_freq =    GetSpecialsFrequency();
    setup_data.m_monster_freq =     GetMonsterFrequency();
    setup_data.m_native_freq =      GetNativeFrequency();
    setup_data.m_ai_aggr =          GetAIAggression();
}

void GalaxySetupPanel::Init() {
    AttachChild(m_seed_edit);
    AttachChild(m_random);
    AttachChild(m_stars_spin);
    AttachChild(m_galaxy_shapes_list);
    AttachChild(m_galaxy_ages_list);
    AttachChild(m_starlane_freq_list);
    AttachChild(m_planet_density_list);
    AttachChild(m_specials_freq_list);
    AttachChild(m_monster_freq_list);
    AttachChild(m_native_freq_list);
    AttachChild(m_ai_aggression_list);

    GG::Connect(m_random->LeftClickedSignal,                &GalaxySetupPanel::RandomClicked,   this);
    GG::Connect(m_stars_spin->ValueChangedSignal,           &GalaxySetupPanel::SettingChanged_, this);
    GG::Connect(m_galaxy_shapes_list->SelChangedSignal,     &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_galaxy_ages_list->SelChangedSignal,       &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_starlane_freq_list->SelChangedSignal,     &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_planet_density_list->SelChangedSignal,    &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_specials_freq_list->SelChangedSignal,     &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_monster_freq_list->SelChangedSignal,      &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_native_freq_list->SelChangedSignal,       &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_ai_aggression_list->SelChangedSignal,     &GalaxySetupPanel::SettingChanged,  this);
    GG::Connect(m_galaxy_shapes_list->SelChangedSignal,     &GalaxySetupPanel::ShapeChanged,    this);

    // create and load textures
    m_textures.clear();
    m_textures.resize(GALAXY_SHAPES);
    m_textures[SPIRAL_2] =    ClientUI::GetTexture(ClientUI::ArtDir() / "gp_spiral2.png");
    m_textures[SPIRAL_3] =    ClientUI::GetTexture(ClientUI::ArtDir() / "gp_spiral3.png");
    m_textures[SPIRAL_4] =    ClientUI::GetTexture(ClientUI::ArtDir() / "gp_spiral4.png");
    m_textures[CLUSTER] =     ClientUI::GetTexture(ClientUI::ArtDir() / "gp_cluster.png");
    m_textures[ELLIPTICAL] =  ClientUI::GetTexture(ClientUI::ArtDir() / "gp_elliptical.png");
    m_textures[IRREGULAR] =   ClientUI::GetTexture(ClientUI::ArtDir() / "gp_irregular.png");
    m_textures[RING] =        ClientUI::GetTexture(ClientUI::ArtDir() / "gp_ring.png");
    m_textures[PYTHON_TEST] = ClientUI::GetTexture(ClientUI::ArtDir() / "gp_irregular.png");
    m_textures[RANDOM] =      ClientUI::GetTexture(ClientUI::ArtDir() / "gp_random.png");

    // fill droplists
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_2ARM")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_3ARM")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_4ARM")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_CLUSTER")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_ELLIPTICAL")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_IRREGULAR")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_RING")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_PYTHON_TEST")));
    m_galaxy_shapes_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_RANDOM")));

    m_galaxy_ages_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_YOUNG")));
    m_galaxy_ages_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MATURE")));
    m_galaxy_ages_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_ANCIENT")));

    if (ALLOW_NO_STARLANES)
        m_starlane_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_NONE")));
    m_starlane_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_LOW")));
    m_starlane_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MEDIUM")));
    m_starlane_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_HIGH")));

    m_planet_density_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_LOW")));
    m_planet_density_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MEDIUM")));
    m_planet_density_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_HIGH")));

    m_specials_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_NONE")));
    m_specials_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_LOW")));
    m_specials_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MEDIUM")));
    m_specials_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_HIGH")));

    m_monster_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_NONE")));
    m_monster_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_LOW")));
    m_monster_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MEDIUM")));
    m_monster_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_HIGH")));

    m_native_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_NONE")));
    m_native_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_LOW")));
    m_native_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MEDIUM")));
    m_native_freq_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_HIGH")));

    m_ai_aggression_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_BEGINNER")));
    m_ai_aggression_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_TURTLE")));
    m_ai_aggression_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_DEFENSIVE")));
    m_ai_aggression_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MODERATE")));
    m_ai_aggression_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_AGGRESSIVE")));
    m_ai_aggression_list->Insert(new CUISimpleDropDownListRow(UserString("GSETUP_MANIACAL")));

    // initial settings from stored results or defaults
    m_seed_edit->SetText(GetOptionsDB().Get<std::string>("GameSetup.seed"));
    m_stars_spin->SetValue(GetOptionsDB().Get<int>("GameSetup.stars"));
    m_galaxy_shapes_list->Select(GetOptionsDB().Get<Shape>("GameSetup.galaxy-shape"));
    ShapeChanged(m_galaxy_shapes_list->CurrentItem());
    m_galaxy_ages_list->Select(GetOptionsDB().Get<GalaxySetupOption>("GameSetup.galaxy-age") - 1);
    m_starlane_freq_list->Select(GetOptionsDB().Get<GalaxySetupOption>("GameSetup.starlane-frequency") - (ALLOW_NO_STARLANES ? 0 : 1));
    m_planet_density_list->Select(GetOptionsDB().Get<GalaxySetupOption>("GameSetup.planet-density") - 1);
    m_specials_freq_list->Select(GetOptionsDB().Get<GalaxySetupOption>("GameSetup.specials-frequency"));
    m_monster_freq_list->Select(GetOptionsDB().Get<GalaxySetupOption>("GameSetup.monster-frequency"));
    m_native_freq_list->Select(GetOptionsDB().Get<GalaxySetupOption>("GameSetup.native-frequency"));
    m_ai_aggression_list->Select(GetOptionsDB().Get<Aggression>("GameSetup.ai-aggression"));

    SettingsChangedSignal();
}

void GalaxySetupPanel::SettingChanged_(int) {
    Sound::TempUISoundDisabler sound_disabler;
    SettingsChangedSignal();
}

void GalaxySetupPanel::SettingChanged(GG::DropDownList::iterator)
{ SettingChanged_(0); }

void GalaxySetupPanel::ShapeChanged(GG::DropDownList::iterator it)
{ ImageChangedSignal(m_textures[m_galaxy_shapes_list->IteratorToIndex(it)]); }


////////////////////////////////////////////////
// GalaxySetupWnd
////////////////////////////////////////////////
GalaxySetupWnd::GalaxySetupWnd() :
    CUIWnd(UserString("GSETUP_WINDOW_TITLE"), (HumanClientApp::GetApp()->AppWidth() - GAL_SETUP_WND_WD) / 2,
           (HumanClientApp::GetApp()->AppHeight() - GAL_SETUP_WND_HT) / 2, GAL_SETUP_WND_WD, GAL_SETUP_WND_HT,
           GG::INTERACTIVE | GG::MODAL),
    m_ended_with_ok(false),
    m_galaxy_setup_panel(0),
    m_player_name_label(0),
    m_player_name_edit(0),
    m_empire_name_label(0),
    m_empire_name_edit(0),
    m_empire_color_label(0),
    m_empire_color_selector(0),
    m_starting_secies_selector(0),
    m_starting_species_label(0),
    m_number_ais_label(0),
    m_number_ais_spin(0),
    m_preview_image(0),
    m_ok(0),
    m_cancel(0)
{
    Sound::TempUISoundDisabler sound_disabler;

    m_galaxy_setup_panel = new GalaxySetupPanel(GG::X0, GG::Y(4));

    boost::shared_ptr<GG::Font> font = ClientUI::GetFont();
    const GG::X LABELS_WIDTH = (GalaxySetupPanel::DEFAULT_WIDTH - 5) / 2;

    GG::Y ypos = m_galaxy_setup_panel->LowerRight().y;

    // player name
    m_player_name_label = new GG::TextControl(CONTROL_MARGIN, ypos, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_PLAYER_NAME"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    m_player_name_label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    m_player_name_label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.player-name")));
    m_player_name_edit = new CUIEdit(LABELS_WIDTH + 2 * CONTROL_MARGIN, ypos, LABELS_WIDTH, GetOptionsDB().Get<std::string>("GameSetup.player-name"));
    m_player_name_label->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_player_name_label->Height()) / 2));
    m_player_name_edit->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_player_name_edit->Height()) / 2));
    ypos += PANEL_CONTROL_SPACING;

    // empire name
    m_empire_name_label = new GG::TextControl(CONTROL_MARGIN, ypos, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_EMPIRE_NAME"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    m_empire_name_label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    m_empire_name_label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.empire-name")));
    m_empire_name_edit = new CUIEdit(LABELS_WIDTH + 2 * CONTROL_MARGIN, ypos, LABELS_WIDTH, GetOptionsDB().Get<std::string>("GameSetup.empire-name"));
    m_empire_name_label->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_empire_name_label->Height()) / 2));
    m_empire_name_edit->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_empire_name_edit->Height()) / 2));
    ypos += PANEL_CONTROL_SPACING;

    const GG::Y AUTO_CONTROL_HEIGHT = m_empire_name_edit->Height();

    // empire color
    m_empire_color_label = new GG::TextControl(CONTROL_MARGIN, ypos, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_EMPIRE_COLOR"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    m_empire_color_label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    m_empire_color_label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.empire-color")));
    m_empire_color_selector = new EmpireColorSelector(AUTO_CONTROL_HEIGHT);
    m_empire_color_selector->MoveTo(GG::Pt(LABELS_WIDTH + 2 * CONTROL_MARGIN, ypos + (PANEL_CONTROL_SPACING - m_empire_color_selector->Height()) / 2));
    m_empire_color_selector->Select(GetOptionsDB().Get<int>("GameSetup.empire-color"));
    ypos += PANEL_CONTROL_SPACING;

    // starting species
    m_starting_species_label = new GG::TextControl(CONTROL_MARGIN, ypos, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_SPECIES"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    m_starting_species_label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    m_starting_species_label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.starting-species")));
    m_starting_secies_selector = new SpeciesSelector(LABELS_WIDTH, AUTO_CONTROL_HEIGHT);
    m_starting_secies_selector->MoveTo(GG::Pt(LABELS_WIDTH + 2 * CONTROL_MARGIN, ypos + (PANEL_CONTROL_SPACING - m_starting_secies_selector->Height()) / 2));
    std::string default_starting_species = GetOptionsDB().Get<std::string>("GameSetup.starting-species");
    if (default_starting_species.empty() || default_starting_species == "1") {  // kludge / bug workaround for bug with options storage and retreival.  Empty-string options are stored, but read in as "true" boolean, and converted to string equal to "1"
        // if no previously-stored species selection, need to pick a default
        std::vector<std::string> selector_avail_species = m_starting_secies_selector->AvailableSpeciesNames();
        if (!selector_avail_species.empty()) {
            for (std::vector<std::string>::const_iterator it = selector_avail_species.begin();
                 it != selector_avail_species.end(); ++it)
            {
                // special case: see if humans are available.
                if ("SP_HUMAN" == *it) {
                    default_starting_species = "SP_HUMAN";
                    break;
                }
            }
            // if no humans, default to first listed species
            if (default_starting_species.empty())
                default_starting_species = *selector_avail_species.begin();
        }
    }
    m_starting_secies_selector->SelectSpecies(default_starting_species);
    ypos += PANEL_CONTROL_SPACING;

    // number of AIs
    m_number_ais_label = new GG::TextControl(CONTROL_MARGIN, ypos, LABELS_WIDTH, CONTROL_HEIGHT, UserString("GSETUP_NUMBER_AIS"), font, ClientUI::TextColor(), GG::FORMAT_RIGHT, GG::INTERACTIVE);
    m_number_ais_label->SetBrowseModeTime(GetOptionsDB().Get<int>("UI.tooltip-delay"));
    m_number_ais_label->SetBrowseText(UserString(GetOptionsDB().GetDescription("GameSetup.ai-players")));
    m_number_ais_spin = new CUISpin<int>(LABELS_WIDTH + 2 * CONTROL_MARGIN, ypos, GG::X(75), GetOptionsDB().Get<int>("GameSetup.ai-players"), 1, 0, MAX_AI_PLAYERS, true);
    m_number_ais_label->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_number_ais_label->Height()) / 2));
    m_number_ais_spin->OffsetMove(GG::Pt(GG::X0, (PANEL_CONTROL_SPACING - m_number_ais_spin->Height()) / 2));
    ypos += PANEL_CONTROL_SPACING;

    m_preview_ul = GG::Pt(ClientWidth() - PREVIEW_SZ.x - 7, GG::Y(7));

    // create a temporary texture and static graphic
    ypos += GG::Y(5);
    boost::shared_ptr<GG::Texture> temp_tex(new GG::Texture());
    m_preview_image =  new GG::StaticGraphic(m_preview_ul.x, m_preview_ul.y, PREVIEW_SZ.x, PREVIEW_SZ.y, temp_tex, GG::GRAPHIC_FITGRAPHIC); // create a blank graphic

    m_ok = new CUIButton(GG::X(10), ypos, GG::X(75), UserString("OK"));
    m_cancel = new CUIButton(10 + m_ok->Size().x + 15, ypos, GG::X(75), UserString("CANCEL"));

    Init();
}

const std::string& GalaxySetupWnd::EmpireName() const
{ return m_empire_name_edit->Text(); }

GG::Clr GalaxySetupWnd::EmpireColor() const
{ return m_empire_color_selector->CurrentColor(); }

const std::string& GalaxySetupWnd::StartingSpeciesName() const
{ return m_starting_secies_selector->CurrentSpeciesName(); }

int GalaxySetupWnd::NumberAIs() const
{ return m_number_ais_spin->Value(); }

void GalaxySetupWnd::Render() {
    CUIWnd::Render();
    GG::FlatRectangle(GG::Pt(ClientUpperLeft().x + m_preview_ul.x - 2,
                             ClientUpperLeft().y + m_preview_ul.y - 2),
                      GG::Pt(ClientUpperLeft().x + m_preview_ul.x + PREVIEW_SZ.x + 2,
                             ClientUpperLeft().y + m_preview_ul.y + PREVIEW_SZ.y + 2),
                      GG::CLR_BLACK, ClientUI::WndInnerBorderColor(), 1);
}

void GalaxySetupWnd::KeyPress (GG::Key key, boost::uint32_t key_code_point, GG::Flags<GG::ModKey> mod_keys) {
    if (!m_ok->Disabled() && (key == GG::GGK_RETURN || key == GG::GGK_KP_ENTER)) // Same behaviour as if "OK" was pressed
        OkClicked();
    else if (key == GG::GGK_ESCAPE) // Same behaviour as if "Cancel" was pressed
        CancelClicked();
}

void GalaxySetupWnd::Init() {
    AttachChild(m_galaxy_setup_panel);
    AttachChild(m_player_name_label);
    AttachChild(m_player_name_edit);
    AttachChild(m_empire_name_label);
    AttachChild(m_empire_name_edit);
    AttachChild(m_empire_color_label);
    AttachChild(m_empire_color_selector);
    AttachChild(m_starting_species_label);
    AttachChild(m_starting_secies_selector);
    AttachChild(m_number_ais_label);
    AttachChild(m_number_ais_spin);
    AttachChild(m_preview_image);
    AttachChild(m_ok);
    AttachChild(m_cancel);

    GG::Connect(m_galaxy_setup_panel->ImageChangedSignal,   &GalaxySetupWnd::PreviewImageChanged, this);
    GG::Connect(m_player_name_edit->EditedSignal,           &GalaxySetupWnd::PlayerNameChanged, this);
    GG::Connect(m_empire_name_edit->EditedSignal,           &GalaxySetupWnd::EmpireNameChanged, this);
    GG::Connect(m_ok->LeftClickedSignal,                    &GalaxySetupWnd::OkClicked, this);
    GG::Connect(m_cancel->LeftClickedSignal,                &GalaxySetupWnd::CancelClicked, this);

    PreviewImageChanged(m_galaxy_setup_panel->PreviewImage());
}

void GalaxySetupWnd::PreviewImageChanged(boost::shared_ptr<GG::Texture> new_image) {
    if (m_preview_image) {
        DeleteChild(m_preview_image);
        m_preview_image = 0;
    }
    m_preview_image = new GG::StaticGraphic(m_preview_ul.x, m_preview_ul.y, PREVIEW_SZ.x, PREVIEW_SZ.y, new_image, GG::GRAPHIC_FITGRAPHIC);
    AttachChild(m_preview_image);
}

void GalaxySetupWnd::EmpireNameChanged(const std::string& name)
{ m_ok->Disable(name.empty()); }

void GalaxySetupWnd::PlayerNameChanged(const std::string& name)
{ m_ok->Disable(name.empty()); }

void GalaxySetupWnd::OkClicked() {
    // record selected galaxy setup options as new defaults
    GetOptionsDB().Set("GameSetup.seed",                m_galaxy_setup_panel->GetSeed());
    GetOptionsDB().Set("GameSetup.stars",               m_galaxy_setup_panel->Systems());
    GetOptionsDB().Set("GameSetup.galaxy-shape",        m_galaxy_setup_panel->GetShape());
    GetOptionsDB().Set("GameSetup.galaxy-age",          m_galaxy_setup_panel->GetAge());
    GetOptionsDB().Set("GameSetup.starlane-frequency",  m_galaxy_setup_panel->GetStarlaneFrequency());
    GetOptionsDB().Set("GameSetup.planet-density",      m_galaxy_setup_panel->GetPlanetDensity());
    GetOptionsDB().Set("GameSetup.specials-frequency",  m_galaxy_setup_panel->GetSpecialsFrequency());
    GetOptionsDB().Set("GameSetup.monster-frequency",   m_galaxy_setup_panel->GetMonsterFrequency());
    GetOptionsDB().Set("GameSetup.native-frequency",    m_galaxy_setup_panel->GetNativeFrequency());
    GetOptionsDB().Set("GameSetup.ai-aggression",       m_galaxy_setup_panel->GetAIAggression());
    GetOptionsDB().Set("GameSetup.player-name",         m_player_name_edit->Text());
    GetOptionsDB().Set("GameSetup.empire-name",         EmpireName());
    GetOptionsDB().Set("GameSetup.empire-color",        static_cast<int>(m_empire_color_selector->CurrentItemIndex()));
    GetOptionsDB().Set("GameSetup.starting-species",    m_starting_secies_selector->CurrentSpeciesName());
    GetOptionsDB().Set("GameSetup.ai-players",          m_number_ais_spin->Value());

    // Save the changes:
    {
        boost::filesystem::ofstream ofs(GetConfigPath());
        if (ofs) {
            GetOptionsDB().GetXML().WriteDoc(ofs);
        } else {
            std::cerr << UserString("UNABLE_TO_WRITE_CONFIG_XML") << std::endl;
            std::cerr << GetConfigPath().string() << std::endl;
            Logger().errorStream() << UserString("UNABLE_TO_WRITE_CONFIG_XML");
            Logger().errorStream() << GetConfigPath().string();
        }
    }


    m_ended_with_ok = true;
    m_done = true;
}

void GalaxySetupWnd::CancelClicked() {
    m_ended_with_ok = false;
    m_done = true;
}
