#include "ShipDesign.h"

#include "../util/AppInterface.h"
#include "../util/MultiplayerCommon.h"
#include "../util/OptionsDB.h"
#include "../util/Directories.h"
#include "../Empire/Empire.h"
#include "../Empire/EmpireManager.h"
#include "ParserUtil.h"
#include "Condition.h"
#include "Effect.h"

#include <boost/filesystem/fstream.hpp>

std::string DumpIndent();

extern int g_indent;

namespace {
    const bool CHEAP_AND_FAST_SHIP_PRODUCTION = false;    // makes all ships cost 1 PP and take 1 turn to build
}

namespace {
    struct store_ship_design_impl {
        template <class T1, class T2>
        struct result {typedef void type;};
        template <class T>
        void operator()(std::map<std::string, ShipDesign*>& ship_designs, const T& ship_design) const {
            if (ship_designs.find(ship_design->Name(false)) != ship_designs.end()) {
                std::string error_str = "ERROR: More than one predefined ship design in predefined_ship_designs.txt has the name " + ship_design->Name(false);
                throw std::runtime_error(error_str.c_str());
            }
            ship_designs[ship_design->Name(false)] = ship_design;
        }
    };

    const phoenix::function<store_ship_design_impl> store_ship_design_;

    struct store_part_type_impl {
        template <class T1, class T2>
        struct result {typedef void type;};
        template <class T>
        void operator()(std::map<std::string, PartType*>& part_types, const T& part_type) const {
            if (part_types.find(part_type->Name()) != part_types.end()) {
                std::string error_str = "ERROR: More than one ship part in ship_parts.txt has the name " + part_type->Name();
                throw std::runtime_error(error_str.c_str());
            }
            part_types[part_type->Name()] = part_type;
        }
    };

    const phoenix::function<store_part_type_impl> store_part_type_;

    struct store_hull_type_impl {
        template <class T1, class T2>
        struct result {typedef void type;};
        template <class T>
        void operator()(std::map<std::string, HullType*>& hull_types, const T& hull_type) const {
            if (hull_types.find(hull_type->Name()) != hull_types.end()) {
                std::string error_str = "ERROR: More than one ship hull in ship_hulls.txt has the name " + hull_type->Name();
                throw std::runtime_error(error_str.c_str());
            }
            hull_types[hull_type->Name()] = hull_type;
        }
    };

    const phoenix::function<store_hull_type_impl> store_hull_type_;

    struct PartTypeStringVisitor :
        public boost::static_visitor<>
    {
        PartTypeStringVisitor(std::string& str) :
            m_str(str)
            {}
        void operator()(const double& d) const
            { m_str = "capacity stat"; }
        void operator()(const DirectFireStats& stats) const
            { m_str = "direct-fire weapon stats"; }
        void operator()(const LRStats& stats) const
            { m_str = "long-range weapon stats"; }
        void operator()(const FighterStats& stats) const
            { m_str = "fighter bay stats"; }
        std::string& m_str;
    };

    std::string PartTypeStatsString(const PartTypeStats& stats)
    {
        std::string retval;
        boost::apply_visitor(PartTypeStringVisitor(retval), stats);
        return retval;
    }

    boost::shared_ptr<const Effect::EffectsGroup>
    IncreaseMax(MeterType meter_type, const std::string& meter_name, double increase)
    {
        typedef boost::shared_ptr<const Effect::EffectsGroup> EffectsGroupPtr;
        typedef std::vector<Effect::EffectBase*> Effects;
        Condition::Self* scope = new Condition::Self;
        Condition::Self* activation = new Condition::Self;
        ValueRef::ValueRefBase<double>* vr =
            new ValueRef::Operation<double>(
                ValueRef::PLUS,
                new ValueRef::Variable<double>(false, meter_name),
                new ValueRef::Constant<double>(increase)
            );
        return EffectsGroupPtr(
            new Effect::EffectsGroup(
                scope, activation, Effects(1, new Effect::SetMeter(meter_type, vr, true))));
    }

    struct DescriptionVisitor : public boost::static_visitor<>
    {
        DescriptionVisitor(ShipPartClass part_class, std::string& description) :
            m_class(part_class),
            m_description(description)
        {}
        void operator()(const double& d) const
        {
            std::string desc_string =
                m_class == PC_FUEL || m_class == PC_COLONY ?
                "PART_DESC_CAPACITY" : "PART_DESC_STRENGTH";
            m_description +=
                str(FlexibleFormat(UserString(desc_string)) % d);
        }
        void operator()(const DirectFireStats& stats) const
        {
            m_description +=
                str(FlexibleFormat(UserString("PART_DESC_DIRECT_FIRE_STATS"))
                    % stats.m_damage
                    % stats.m_ROF
                    % stats.m_range);
        }
        void operator()(const LRStats& stats) const
        {
            m_description +=
                str(FlexibleFormat(UserString("PART_DESC_LR_STATS"))
                    % stats.m_damage
                    % stats.m_ROF
                    % stats.m_range
                    % stats.m_speed
                    % stats.m_health
                    % stats.m_stealth
                    % stats.m_capacity);
        }
        void operator()(const FighterStats& stats) const
        {
            m_description +=
                str(FlexibleFormat(UserString("PART_DESC_FIGHTER_STATS"))
                    % UserString(stats.m_type == BOMBER ? "BOMBER" : "INTERCEPTOR")
                    % stats.m_anti_ship_damage
                    % stats.m_anti_fighter_damage
                    % stats.m_launch_rate
                    % stats.m_speed
                    % stats.m_stealth
                    % stats.m_health
                    % stats.m_detection
                    % stats.m_capacity);
        }

        const ShipPartClass m_class;
        std::string& m_description;
    };

    bool DesignsTheSame(const ShipDesign& one, const ShipDesign& two) {
        return (
            one.Name()              == two.Name() &&
            one.Description()       == two.Description() &&
            one.DesignedByEmpire()  == two.DesignedByEmpire() &&
            one.DesignedOnTurn()    == two.DesignedOnTurn() &&
            one.Hull()              == two.Hull() &&
            one.Parts()             == two.Parts() &&
            one.Graphic()           == two.Graphic() &&
            one.Model()             == two.Model()
        );
        // not checking that IDs are the same, since the purpose of this is to
        // check if a design that might be added to the universe (which doesn't
        // have an ID yet) is the same as one that has already been added
        // (which does have an ID)
    }
}

////////////////////////////////////////////////
// Free Functions                             //
////////////////////////////////////////////////
const PartTypeManager& GetPartTypeManager() {
    return PartTypeManager::GetPartTypeManager();
}

const PartType* GetPartType(const std::string& name) {
    return GetPartTypeManager().GetPartType(name);
}

const HullTypeManager& GetHullTypeManager() {
    return HullTypeManager::GetHullTypeManager();
}

const HullType* GetHullType(const std::string& name) {
    return GetHullTypeManager().GetHullType(name);
}

const ShipDesign* GetShipDesign(int ship_design_id) {
    return GetUniverse().GetShipDesign(ship_design_id);
}


/////////////////////////////////////
// PartTypeManager                 //
/////////////////////////////////////
// static
PartTypeManager* PartTypeManager::s_instance = 0;

PartTypeManager::PartTypeManager() {
    if (s_instance)
        throw std::runtime_error("Attempted to create more than one PartTypeManager.");
    s_instance = this;

    std::string file_name = "ship_parts.txt";
    std::string input;

    boost::filesystem::ifstream ifs(GetResourceDir() / file_name);
    if (ifs) {
        std::getline(ifs, input, '\0');
        ifs.close();
    } else {
        Logger().errorStream() << "Unable to open data file " << file_name;
        return;
    }

    using namespace boost::spirit;
    using namespace phoenix;
    parse_info<const char*> result =
        parse(input.c_str(),
              as_lower_d[*part_p[store_part_type_(var(m_parts), arg1)]]
              >> end_p,
              skip_p);
    if (!result.full)
        ReportError(input.c_str(), result);
}

PartTypeManager::~PartTypeManager()
{
    for (std::map<std::string, PartType*>::iterator it = m_parts.begin(); it != m_parts.end(); ++it) {
        delete it->second;
    }
}

const PartType* PartTypeManager::GetPartType(const std::string& name) const {
    std::map<std::string, PartType*>::const_iterator it = m_parts.find(name);
    return it != m_parts.end() ? it->second : 0;
}

const PartTypeManager& PartTypeManager::GetPartTypeManager() {
    static PartTypeManager manager;
    return manager;
}

PartTypeManager::iterator PartTypeManager::begin() const {
    return m_parts.begin();
}

PartTypeManager::iterator PartTypeManager::end() const {
    return m_parts.end();
}


////////////////////////////////////////////////
// PartType stat variant types                //
////////////////////////////////////////////////
const double DirectFireStats::PD_SELF_DEFENSE_FACTOR = 2.0 / 3.0;

DirectFireStats::DirectFireStats() :
    m_damage(),
    m_ROF(),
    m_range()
{}

DirectFireStats::DirectFireStats(double damage,
                                 double ROF,
                                 double range) :
    m_damage(damage),
    m_ROF(ROF),
    m_range(range)
{}

LRStats::LRStats() :
    m_damage(),
    m_ROF(),
    m_range(),
    m_speed(),
    m_stealth(),
    m_health()
{}

LRStats::LRStats(double damage,
                 double ROF,
                 double range,
                 double speed,
                 double stealth,
                 double health,
                 int capacity) :
    m_damage(damage),
    m_ROF(ROF),
    m_range(range),
    m_speed(speed),
    m_stealth(stealth),
    m_health(health),
    m_capacity(capacity)
{
    if (m_capacity < 0)
        throw std::runtime_error("Attempted to create a LRStats with a "
                                 "nonpositive capacity.");
}

FighterStats::FighterStats() :
    m_type(),
    m_anti_ship_damage(),
    m_anti_fighter_damage(),
    m_launch_rate(),
    m_fighter_weapon_range(),
    m_speed(),
    m_stealth(),
    m_health(),
    m_detection(),
    m_capacity()
{}

FighterStats::FighterStats(CombatFighterType type,
                           double anti_ship_damage,
                           double anti_fighter_damage,
                           double launch_rate,
                           double fighter_weapon_range,
                           double speed,
                           double stealth,
                           double health,
                           double detection,
                           int capacity) :
    m_type(type),
    m_anti_ship_damage(anti_ship_damage),
    m_anti_fighter_damage(anti_fighter_damage),
    m_launch_rate(launch_rate),
    m_fighter_weapon_range(fighter_weapon_range),
    m_speed(speed),
    m_stealth(stealth),
    m_health(health),
    m_detection(detection),
    m_capacity(capacity)
{
    if (type == INTERCEPTOR && m_anti_fighter_damage < m_anti_ship_damage)
        Logger().debugStream() << "Creating an INTERCEPTOR FighterStats with weaker anti-fighter stat than anti-ship stat.";
    if (type == BOMBER && m_anti_ship_damage < m_anti_fighter_damage)
        Logger().debugStream() << "Creating a BOMBER FighterStats with weaker anti-ship stat than anti-fighter stat.";
    if (m_capacity < 0) {
        m_capacity = 0;
        Logger().debugStream() << "Creating a FighterStats with a nonpositive capacity.";
    }
}


////////////////////////////////////////////////
// PartType
////////////////////////////////////////////////
PartType::PartType() :
    m_name("invalid part type"),
    m_description("indescribable"),
    m_class(INVALID_SHIP_PART_CLASS),
    m_stats(1.0),
    m_cost(1.0),
    m_build_time(1),
    m_mountable_slot_types(),
    m_location(0),
    m_effects(),
    m_graphic("")
{}

PartType::PartType(
    const std::string& name, const std::string& description,
    ShipPartClass part_class, const PartTypeStats& stats, double cost, int build_time,
    std::vector<ShipSlotType> mountable_slot_types,
    const Condition::ConditionBase* location,
    const std::vector<boost::shared_ptr<const Effect::EffectsGroup> >& effects,
    const std::string& graphic) :
    m_name(name),
    m_description(description),
    m_class(part_class),
    m_stats(stats),
    m_cost(cost),
    m_build_time(build_time),
    m_mountable_slot_types(mountable_slot_types),
    m_location(location),
    m_effects(),
    m_graphic(graphic)
{
    switch (m_class) {
    case PC_SHORT_RANGE:
    case PC_POINT_DEFENSE:
        if (!boost::get<DirectFireStats>(&m_stats)) {
            std::string type_name = m_class == PC_SHORT_RANGE?
                "PC_SHORT_RANGE" : "PC_POINT_DEFENSE";
            throw std::runtime_error("PartType::PartType() : Wrong kind of stats specified "
                                     "for " + type_name + " part \"" + m_name + "\" -- "
                                     "was " + PartTypeStatsString(m_stats) + "; should have "
                                     "been " + PartTypeStatsString(DirectFireStats()));
        }
        break;
    case PC_MISSILES:
        if (!boost::get<LRStats>(&m_stats)) {
            throw std::runtime_error("PartType::PartType() : Wrong kind of stats specified "
                                     "for PC_MISSILES part \"" + m_name + "\" -- "
                                     "was " + PartTypeStatsString(m_stats) + "; should have "
                                     "been " + PartTypeStatsString(LRStats()));
        }
        break;
    case PC_FIGHTERS:
        if (!boost::get<FighterStats>(&m_stats)) {
            throw std::runtime_error("PartType::PartType() : Wrong kind of stats specified "
                                     "for PC_FIGHTERS part \"" + m_name + "\" -- "
                                     "was " + PartTypeStatsString(m_stats) + "; should have "
                                     "been " + PartTypeStatsString(FighterStats()));
        }
        break;
    default:
        if (!boost::get<double>(&m_stats)) {
            throw std::runtime_error("PartType::PartType() : Wrong kind of stats specified "
                                     "for generic part \"" + m_name + "\" -- "
                                     "was " + PartTypeStatsString(m_stats) + "; should have "
                                     "been " + PartTypeStatsString(double()));
        }
        break;
    }

    if (m_class == PC_SHIELD)
        m_effects.push_back(IncreaseMax(METER_SHIELD, "MaxShield", boost::get<double>(m_stats)));
    else if (m_class == PC_DETECTION)
        m_effects.push_back(IncreaseMax(METER_DETECTION, "MaxDetection", boost::get<double>(m_stats)));
    else if (m_class == PC_STEALTH)
        m_effects.push_back(IncreaseMax(METER_STEALTH, "MaxStealth", boost::get<double>(m_stats)));
    else if (m_class == PC_FUEL)
        m_effects.push_back(IncreaseMax(METER_FUEL, "MaxFuel", boost::get<double>(m_stats)));

    for (std::vector<boost::shared_ptr<const Effect::EffectsGroup> >::const_iterator it = effects.begin(); it != effects.end(); ++it)
        m_effects.push_back(*it);
}

PartType::~PartType()
{ delete m_location; }

const std::string& PartType::Name() const {
    return m_name;
}

const std::string& PartType::Description() const
{
    return m_description;
}

std::string PartType::StatDescription() const
{
    std::string retval;
    boost::apply_visitor(DescriptionVisitor(m_class, retval), m_stats);
    return retval;
}

ShipPartClass PartType::Class() const {
    return m_class;
}

const PartTypeStats& PartType::Stats() const {
    return m_stats;
}

bool PartType::CanMountInSlotType(ShipSlotType slot_type) const {
    if (INVALID_SHIP_SLOT_TYPE == slot_type)
        return false;
    for (std::vector<ShipSlotType>::const_iterator it = m_mountable_slot_types.begin(); it != m_mountable_slot_types.end(); ++it)
        if (*it == slot_type)
            return true;
    return false;
}

double PartType::Cost() const {
    return m_cost;
}

int PartType::BuildTime() const {
    return m_build_time;
}

const std::string& PartType::Graphic() const {
    return m_graphic;
}

const std::vector<boost::shared_ptr<const Effect::EffectsGroup> >& PartType::Effects() const {
    return m_effects;
}

const Condition::ConditionBase* PartType::Location() const {
    return m_location;
}


////////////////////////////////////////////////
// HullType stats                             //
////////////////////////////////////////////////
HullTypeStats::HullTypeStats() :
    m_fuel(0.0),
    m_battle_speed(0.0),
    m_starlane_speed(0.0),
    m_stealth(0.0),
    m_health(0.0)
{}

HullTypeStats::HullTypeStats(double fuel, double battle_speed, double starlane_speed,
                             double stealth, double health) :
    m_fuel(fuel),
    m_battle_speed(battle_speed),
    m_starlane_speed(starlane_speed),
    m_stealth(stealth),
    m_health(health)
{}


////////////////////////////////////////////////
// HullType
////////////////////////////////////////////////

// HullType::Slot
HullType::Slot::Slot() :
    type(INVALID_SHIP_SLOT_TYPE), x(0.5), y(0.5) {}
HullType::Slot::Slot(ShipSlotType slot_type, double x_, double y_) :
    type(slot_type), x(x_), y(y_) {}

// HullType
HullType::HullType() :
    m_name("generic hull type"),
    m_description("indescribable"),
    m_battle_speed(1.0),
    m_starlane_speed(1.0),
    m_fuel(0.0),
    m_stealth(0.0),
    m_health(0.0),
    m_cost(1.0),
    m_build_time(1),
    m_slots(),
    m_location(0),
    m_effects(),
    m_graphic("")
{}

HullType::HullType(const std::string& name, const std::string& description,
                   double fuel, double battle_speed, double starlane_speed,
                   double stealth, double health,
                   double cost, int build_time, const std::vector<Slot>& slots,
                   const Condition::ConditionBase* location,
                   const std::vector<boost::shared_ptr<const Effect::EffectsGroup> >& effects,
                   const std::string& graphic) :
    m_name(name),
    m_description(description),
    m_battle_speed(battle_speed),
    m_starlane_speed(starlane_speed),
    m_fuel(fuel),
    m_stealth(stealth),
    m_health(health),
    m_cost(cost),
    m_build_time(build_time),
    m_slots(slots),
    m_location(location),
    m_effects(),
    m_graphic(graphic)
{
    m_effects.push_back(IncreaseMax(METER_FUEL,     "MaxFuel",      m_fuel));
    m_effects.push_back(IncreaseMax(METER_STEALTH,  "MaxStealth",   m_stealth));
    m_effects.push_back(IncreaseMax(METER_HEALTH,   "MaxHealth",    m_health));
    for (std::vector<boost::shared_ptr<const Effect::EffectsGroup> >::const_iterator it = effects.begin(); it != effects.end(); ++it)
        m_effects.push_back(*it);
}



HullType::HullType(const std::string& name, const std::string& description,
                   const HullTypeStats& stats,
                   double cost, int build_time, const std::vector<Slot>& slots,
                   const Condition::ConditionBase* location,
                   const std::vector<boost::shared_ptr<const Effect::EffectsGroup> >& effects,
                   const std::string& graphic) :
    m_name(name),
    m_description(description),
    m_battle_speed(stats.m_battle_speed),
    m_starlane_speed(stats.m_starlane_speed),
    m_fuel(stats.m_fuel),
    m_stealth(stats.m_stealth),
    m_health(stats.m_health),
    m_cost(cost),
    m_build_time(build_time),
    m_slots(slots),
    m_location(location),
    m_effects(),
    m_graphic(graphic)
{
    m_effects.push_back(IncreaseMax(METER_FUEL,     "MaxFuel",      m_fuel));
    m_effects.push_back(IncreaseMax(METER_STEALTH,  "MaxStealth",   m_stealth));
    m_effects.push_back(IncreaseMax(METER_HEALTH,   "MaxHealth",    m_health));
    for (std::vector<boost::shared_ptr<const Effect::EffectsGroup> >::const_iterator it = effects.begin(); it != effects.end(); ++it)
        m_effects.push_back(*it);
}

HullType::~HullType()
{ delete m_location; }

const std::string& HullType::Name() const {
    return m_name;
}

const std::string& HullType::Description() const {
    return m_description;
}

std::string HullType::StatDescription() const {
    std::string retval = 
        str(FlexibleFormat(UserString("HULL_DESC"))
            % m_starlane_speed
            % m_fuel
            % m_battle_speed
            % m_health);
    return retval;
}

double HullType::BattleSpeed() const {
    return m_battle_speed;
}

double HullType::StarlaneSpeed() const {
    return m_starlane_speed;
}

double HullType::Fuel() const {
    return m_fuel;
}

double HullType::Health() const {
    return m_health;
}

double HullType::Cost() const {
    return m_cost;
}

int HullType::BuildTime() const {
    return m_build_time;
}

unsigned int HullType::NumSlots() const {
    return m_slots.size();
}

unsigned int HullType::NumSlots(ShipSlotType slot_type) const {
    unsigned int count = 0;
    for (std::vector<Slot>::const_iterator it = m_slots.begin(); it != m_slots.end(); ++it)
        if (it->type == slot_type)
            ++count;
    return count;
}

const std::vector<HullType::Slot>& HullType::Slots() const {
    return m_slots;
}

const Condition::ConditionBase* HullType::Location() const {
    return m_location;
}

const std::vector<boost::shared_ptr<const Effect::EffectsGroup> >& HullType::Effects() const {
    return m_effects;
}

const std::string& HullType::Graphic() const {
    return m_graphic;
}


/////////////////////////////////////
// HullTypeManager                 //
/////////////////////////////////////
// static
HullTypeManager* HullTypeManager::s_instance = 0;

HullTypeManager::HullTypeManager() {
    if (s_instance)
        throw std::runtime_error("Attempted to create more than one HullTypeManager.");
    s_instance = this;

    std::string file_name = "ship_hulls.txt";
    std::string input;

    boost::filesystem::ifstream ifs(GetResourceDir() / file_name);
    if (ifs) {
        std::getline(ifs, input, '\0');
        ifs.close();
    } else {
        Logger().errorStream() << "Unable to open data file " << file_name;
        return;
    }

    using namespace boost::spirit;
    using namespace phoenix;
    parse_info<const char*> result =
        parse(input.c_str(),
              as_lower_d[*hull_p[store_hull_type_(var(m_hulls), arg1)]]
              >> end_p,
              skip_p);
    if (!result.full)
        ReportError(input.c_str(), result);
}

HullTypeManager::~HullTypeManager()
{
    for (std::map<std::string, HullType*>::iterator it = m_hulls.begin(); it != m_hulls.end(); ++it) {
        delete it->second;
    }
}

const HullType* HullTypeManager::GetHullType(const std::string& name) const {
    std::map<std::string, HullType*>::const_iterator it = m_hulls.find(name);
    return it != m_hulls.end() ? it->second : 0;
}

const HullTypeManager& HullTypeManager::GetHullTypeManager()
{
    static HullTypeManager manager;
    return manager;
}

HullTypeManager::iterator HullTypeManager::begin() const {
    return m_hulls.begin();
}

HullTypeManager::iterator HullTypeManager::end() const {
    return m_hulls.end();
}


////////////////////////////////////////////////
// ShipDesign
////////////////////////////////////////////////
// static(s)
const int       ShipDesign::INVALID_DESIGN_ID = -1;
const int       ShipDesign::MAX_ID            = 2000000000;

ShipDesign::ShipDesign() :
    m_id(UniverseObject::INVALID_OBJECT_ID),
    m_name(""),
    m_designed_by_empire_id(ALL_EMPIRES),
    m_designed_on_turn(UniverseObject::INVALID_OBJECT_AGE),
    m_hull(""),
    m_parts(),
    m_graphic(""),
    m_3D_model(""),
    m_name_desc_in_stringtable(false),
    m_is_armed(false),
    m_can_colonize(false),
    m_build_cost(0.0),
    m_build_turns(0),
    m_min_SR_range(DBL_MAX),
    m_max_SR_range(0.0),
    m_min_LR_range(DBL_MAX),
    m_max_LR_range(0.0),
    m_min_PD_range(DBL_MAX),
    m_max_PD_range(0.0),
    m_min_weapon_range(DBL_MAX),
    m_max_weapon_range(0.0),
    m_min_non_PD_weapon_range(DBL_MAX),
    m_max_non_PD_weapon_range(0.0)
{}

ShipDesign::ShipDesign(const std::string& name, const std::string& description, int designed_by_empire_id,
                       int designed_on_turn, const std::string& hull, const std::vector<std::string>& parts,
                       const std::string& graphic, const std::string& model, bool name_desc_in_stringtable) :
    m_id(UniverseObject::INVALID_OBJECT_ID),
    m_name(name),
    m_description(description),
    m_designed_by_empire_id(designed_by_empire_id),
    m_designed_on_turn(designed_on_turn),
    m_hull(hull),
    m_parts(parts),
    m_graphic(graphic),
    m_3D_model(model),
    m_name_desc_in_stringtable(name_desc_in_stringtable),
    m_is_armed(false),
    m_can_colonize(false),
    m_build_cost(0.0),
    m_build_turns(0),
    m_min_SR_range(DBL_MAX),
    m_max_SR_range(0.0),
    m_min_LR_range(DBL_MAX),
    m_max_LR_range(0.0),
    m_min_PD_range(DBL_MAX),
    m_max_PD_range(0.0),
    m_min_weapon_range(DBL_MAX),
    m_max_weapon_range(0.0),
    m_min_non_PD_weapon_range(DBL_MAX),
    m_max_non_PD_weapon_range(0.0)
{
    // expand parts list to have empty values if fewer parts are given than hull has slots
    if (const HullType* hull_type = GetHullType(m_hull)) {
        if (m_parts.size() < hull_type->NumSlots())
            m_parts.resize(hull_type->NumSlots(), "");
    }

    if (!ValidDesign(m_hull, m_parts)) {
        Logger().errorStream() << "constructing an invalid ShipDesign!";
        Logger().errorStream() << Dump();
    }
    BuildStatCaches();
}

int ShipDesign::ID() const {
    return m_id;
}

const std::string& ShipDesign::Name(bool stringtable_lookup /* = true */) const
{
    if (m_name_desc_in_stringtable && stringtable_lookup)
        return UserString(m_name);
    else
        return m_name;
}

const std::string& ShipDesign::Description() const
{
    if (m_name_desc_in_stringtable)
        return UserString(m_description);
    else
        return m_description;
}

int ShipDesign::DesignedByEmpire() const
{
    return m_designed_by_empire_id;
}

void ShipDesign::SetID(int id)
{
    m_id = id;
}

void ShipDesign::Rename(const std::string& name)
{
    m_name = name;
}

const std::string& ShipDesign::Graphic() const {
    return m_graphic;
}

int ShipDesign::DesignedOnTurn() const {
    return m_designed_on_turn;
}

double ShipDesign::Cost() const {
    if (!CHEAP_AND_FAST_SHIP_PRODUCTION)
        return m_build_cost;
    else
        return 1.0;
}

int ShipDesign::BuildTime() const {
    if (!CHEAP_AND_FAST_SHIP_PRODUCTION)
        return m_build_turns;
    else
        return 1;
}

double ShipDesign::StarlaneSpeed() const {
    return GetHull()->StarlaneSpeed();
}

double ShipDesign::BattleSpeed() const {
    return GetHull()->BattleSpeed();
}

const std::multimap<double, const PartType*>& ShipDesign::SRWeapons() const
{ return m_SR_weapons; }

const std::multimap<double, const PartType*>& ShipDesign::LRWeapons() const
{ return m_LR_weapons; }

const std::multimap<double, const PartType*>& ShipDesign::PDWeapons() const
{ return m_PD_weapons; }

double ShipDesign::MinSRRange() const
{ return m_min_SR_range; }

double ShipDesign::MaxSRRange() const
{ return m_max_SR_range; }

double ShipDesign::MinLRRange() const
{ return m_min_LR_range; }

double ShipDesign::MaxLRRange() const
{ return m_max_LR_range; }

double ShipDesign::MinPDRange() const
{ return m_min_PD_range; }

double ShipDesign::MaxPDRange() const
{ return m_max_PD_range; }

double ShipDesign::MinWeaponRange() const
{ return m_min_weapon_range; }

double ShipDesign::MaxWeaponRange() const
{ return m_max_weapon_range; }

double ShipDesign::MinNonPDWeaponRange() const
{ return m_min_non_PD_weapon_range; }

double ShipDesign::MaxNonPDWeaponRange() const
{ return m_max_non_PD_weapon_range; }

//// TEMPORARY
double ShipDesign::Defense() const {
    // accumulate defense from defensive parts in design.
    double total_defense = 0.0;
    const PartTypeManager& part_manager = GetPartTypeManager();
    std::vector<std::string> all_parts = Parts();
    for (std::vector<std::string>::const_iterator it = all_parts.begin(); it != all_parts.end(); ++it) {
        const PartType* part = part_manager.GetPartType(*it);
        if (part && (part->Class() == PC_SHIELD || part->Class() == PC_ARMOUR))
            total_defense += boost::get<double>(part->Stats());
    }
    return total_defense;
}

double ShipDesign::Attack() const {
    // accumulate attack stat from all weapon parts in design
    const PartTypeManager& manager = GetPartTypeManager();

    double total_attack = 0.0;
    std::vector<std::string> all_parts = Parts();
    for (std::vector<std::string>::const_iterator it = all_parts.begin(); it != all_parts.end(); ++it) {
        const PartType* part = manager.GetPartType(*it);
        if (part) {
            if (part->Class() == PC_SHORT_RANGE || part->Class() == PC_POINT_DEFENSE)
                total_attack += boost::get<DirectFireStats>(part->Stats()).m_damage;
            else if (part->Class() == PC_MISSILES)
                total_attack += boost::get<LRStats>(part->Stats()).m_damage;
            else if (part->Class() == PC_FIGHTERS)
                total_attack += boost::get<FighterStats>(part->Stats()).m_anti_ship_damage;
        }
    }
    return total_attack;
}
//// END TEMPORARY

bool ShipDesign::CanColonize() const {
    return m_can_colonize;
}

bool ShipDesign::IsArmed() const {
    return m_is_armed;
}

const std::string& ShipDesign::Hull() const {
    return m_hull;
}

const HullType* ShipDesign::GetHull() const {
    return GetHullTypeManager().GetHullType(m_hull);
}

const std::vector<std::string>& ShipDesign::Parts() const {
    return m_parts;
}

std::vector<std::string> ShipDesign::Parts(ShipSlotType slot_type) const {
    std::vector<std::string> retval;

    const HullType* hull = GetHull();
    assert(hull);
    const std::vector<HullType::Slot>& slots = hull->Slots();

    unsigned int size = m_parts.size();
    assert(size == hull->NumSlots());

    // add to output vector each part that is in a slot of the indicated ShipSlotType 
    for (unsigned int i = 0; i < size; ++i)
        if (slots[i].type == slot_type)
            retval.push_back(m_parts[i]);

    return retval;
}

const std::string& ShipDesign::Model() const {
    return m_3D_model;
}

bool ShipDesign::ProductionLocation(int empire_id, int location_id) const {
    Condition::ObjectSet locations;
    Condition::ObjectSet non_locations;

    Universe& universe = GetUniverse();

    UniverseObject* loc = universe.Object(location_id);
    if (!loc) return false;

    Empire * empire = Empires().Lookup(empire_id);
    if (!empire) {
        Logger().debugStream() << "ShipDesign::ProductionLocation: Unable to get pointer to empire " << empire_id;
        return false;
    }

    UniverseObject * source = universe.Object(empire->CapitolID());
    if (!source) return false;

    locations.insert(loc);

    // apply hull location conditions to potential location
    const HullType* hull = GetHull();
    if (!hull) {
        Logger().errorStream() << "ShipDesign::ProductionLocation  ShipDesign couldn't get its own hull with name " << m_hull;
        return false;
    }
    hull->Location()->Eval(source, locations, non_locations, Condition::TARGETS);
    if (locations.empty())
        return false;

    // apply external and internal parts' location conditions to potential location
    for (std::vector<std::string>::const_iterator part_it = m_parts.begin(); part_it != m_parts.end(); ++part_it) {
        std::string part_name = *part_it;
        if (part_name.empty())
            continue;       // empty slots don't limit build location

        const PartType* part = GetPartType(part_name);
        if (!part) {
            Logger().errorStream() << "ShipDesign::ProductionLocation  ShipDesign couldn't get part with name " << part_name;
            return false;
            }
        part->Location()->Eval(source, locations, non_locations, Condition::TARGETS);
        if (locations.empty())
            return false;
    }
    // location matched all hull and part conditions, so is a valid build location
    return true;
}

bool ShipDesign::ValidDesign(const std::string& hull, const std::vector<std::string>& parts) {
    // ensure hull type exists and has exactly enough slots for passed parts
    const HullType* hull_type = GetHullTypeManager().GetHullType(hull);
    if (!hull_type) {
        Logger().debugStream() << "ShipDesign::ValidDesign: hull not found: " << hull;
        return false;
    }

    unsigned int size = parts.size();
    if (size > hull_type->NumSlots()) {
        Logger().debugStream() << "ShipDesign::ValidDesign: given " << size << " parts for hull with " << hull_type->NumSlots() << " slots";
        return false;
    }

    const std::vector<HullType::Slot>& slots = hull_type->Slots();

    // ensure all passed parts can be mounted in slots of type they were passed for
    const PartTypeManager& part_manager = GetPartTypeManager();
    for (unsigned int i = 0; i < size; ++i) {
        const std::string& part_name = parts[i];
        if (part_name.empty())
            continue;   // if part slot is empty, ignore - doesn't invalidate design

        const PartType* part = part_manager.GetPartType(part_name);
        if (!part) {
            Logger().debugStream() << "ShipDesign::ValidDesign: part not found: " << part_name;
            return false;
        }

        // verify part can mount in indicated slot
        ShipSlotType slot_type = slots[i].type;
        if (!(part->CanMountInSlotType(slot_type))) {
            Logger().debugStream() << "ShipDesign::ValidDesign: part " << part_name << " can't be mounted in " << boost::lexical_cast<std::string>(slot_type) << " slot";
            return false;
        }
    }

    return true;
}

bool ShipDesign::ValidDesign(const ShipDesign& design) {
    return ValidDesign(design.m_hull, design.m_parts);
}

void ShipDesign::BuildStatCaches()
{
    const HullType* hull = GetHullType(m_hull);
    if (!hull) {
        Logger().errorStream() << "ShipDesign::BuildStatCaches couldn't get hull with name " << m_hull;
        return;
    }

    m_build_turns = hull->BuildTime();
    m_build_cost = hull->Cost() * hull->BuildTime();

    for (std::vector<std::string>::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it) {
        if (it->empty())
            continue;

        const PartType* part = GetPartType(*it);
        if (!part) {
            Logger().errorStream() << "ShipDesign::BuildStatCaches couldn't get part with name " << *it;
            continue;
        }

        m_build_turns = std::max(m_build_turns, part->BuildTime()); // assume hull and parts are built in parallel
        m_build_cost += part->Cost();                               // add up costs of all parts

        switch (part->Class()) {
        case PC_SHORT_RANGE: {
            const DirectFireStats& stats = boost::get<DirectFireStats>(part->Stats());
            m_SR_weapons.insert(std::make_pair(stats.m_range, part));
            m_is_armed = true;
            m_min_SR_range = std::min(m_min_SR_range, stats.m_range);
            m_max_SR_range = std::max(m_max_SR_range, stats.m_range);
            m_min_weapon_range = std::min(m_min_weapon_range, stats.m_range);
            m_max_weapon_range = std::max(m_max_weapon_range, stats.m_range);
            m_min_non_PD_weapon_range = std::min(m_min_non_PD_weapon_range, stats.m_range);
            m_max_non_PD_weapon_range = std::max(m_max_non_PD_weapon_range, stats.m_range);
            break;
        }
        case PC_MISSILES: {
            const LRStats& stats = boost::get<LRStats>(part->Stats());
            m_LR_weapons.insert(std::make_pair(stats.m_range, part));
            m_is_armed = true;
            m_min_LR_range = std::min(m_min_LR_range, stats.m_range);
            m_max_LR_range = std::max(m_max_LR_range, stats.m_range);
            m_min_weapon_range = std::min(m_min_weapon_range, stats.m_range);
            m_max_weapon_range = std::max(m_max_weapon_range, stats.m_range);
            m_min_non_PD_weapon_range = std::min(m_min_non_PD_weapon_range, stats.m_range);
            m_max_non_PD_weapon_range = std::max(m_max_non_PD_weapon_range, stats.m_range);
            break;
        }
        case PC_FIGHTERS:
            m_is_armed = true;
            break;
        case PC_POINT_DEFENSE: {
            const DirectFireStats& stats = boost::get<DirectFireStats>(part->Stats());
            m_PD_weapons.insert(std::make_pair(stats.m_range, part));
            m_is_armed = true;
            m_min_PD_range = std::min(m_min_PD_range, stats.m_range);
            m_max_PD_range = std::max(m_max_PD_range, stats.m_range);
            m_min_weapon_range = std::min(m_min_weapon_range, stats.m_range);
            m_max_weapon_range = std::max(m_max_weapon_range, stats.m_range);
            break;
        }
        case PC_COLONY:
            m_can_colonize = true;
            break;
        default:
            break;
        }
    }

    if (m_SR_weapons.empty())
        m_min_SR_range = 0.0;
    if (m_LR_weapons.empty())
        m_min_LR_range = 0.0;
    if (m_PD_weapons.empty())
        m_min_PD_range = 0.0;
    if (!m_min_SR_range && !m_min_LR_range && !m_min_PD_range)
        m_min_weapon_range = 0.0;
    if (!m_min_LR_range && !m_min_PD_range)
        m_min_non_PD_weapon_range = 0.0;

    m_build_cost /= m_build_turns;
}

std::string ShipDesign::Dump() const
{
    std::string retval = DumpIndent() + "ShipDesign\n";
    ++g_indent;
    retval += DumpIndent() + "name = \"" + m_name + "\"\n";
    retval += DumpIndent() + "description = \"" + m_description + "\"\n";
    retval += DumpIndent() + "lookup_strings = " + (m_name_desc_in_stringtable ? "true" : "false") + "\n";
    retval += DumpIndent() + "hull = \"" + m_hull + "\"\n";
    retval += DumpIndent() + "parts = ";
    if (m_parts.empty()) {
        retval += "[]\n";
    } else if (m_parts.size() == 1) {
        retval += "\"" + *m_parts.begin() + "\"\n";
    } else {
        retval += "[\n";
        ++g_indent;
        for (std::vector<std::string>::const_iterator it = m_parts.begin(); it != m_parts.end(); ++it) {
            retval += DumpIndent() + "\"" + *it + "\"\n";
        }
        --g_indent;
        retval += DumpIndent() + "]\n";
    }
    retval += DumpIndent() + "graphic = \"" + m_graphic + "\"\n";
    retval += DumpIndent() + "model = \"" + m_3D_model + "\"\n";
    --g_indent;
    return retval; 
}

/////////////////////////////////////
// PredefinedShipDesignManager     //
/////////////////////////////////////
// static(s)
PredefinedShipDesignManager* PredefinedShipDesignManager::s_instance = 0;

PredefinedShipDesignManager::PredefinedShipDesignManager() {
    if (s_instance)
        throw std::runtime_error("Attempted to create more than one PredefinedShipDesignManager.");

    s_instance = this;

    Logger().debugStream() << "Initializing PredefinedShipDesignManager";

    std::string file_name = "premade_ship_designs.txt";
    std::string input;

    boost::filesystem::ifstream ifs(GetResourceDir() / file_name);
    if (ifs) {
        std::getline(ifs, input, '\0');
        ifs.close();
    } else {
        Logger().errorStream() << "Unable to open data file " << file_name;
        return;
    }

    using namespace boost::spirit;
    using namespace phoenix;
    parse_info<const char*> result =
        parse(input.c_str(),
              as_lower_d[*ship_design_p[store_ship_design_(var(m_ship_designs), arg1)]]
              >> end_p,
              skip_p);
    if (!result.full)
        ReportError(input.c_str(), result);

#ifdef OUTPUT_DESIGNS_LIST
    Logger().debugStream() << "Predefined Ship Designs:";
    for (iterator it = begin(); it != end(); ++it) {
        const ShipDesign* d = it->second;
        Logger().debugStream() << " ... " << d->Name();
    }
#endif
}

PredefinedShipDesignManager::~PredefinedShipDesignManager() {
    for (std::map<std::string, ShipDesign*>::iterator it = m_ship_designs.begin(); it != m_ship_designs.end(); ++it)
        delete it->second;
}

int PredefinedShipDesignManager::GenericUniverseDesignID(const std::string& name) const {
    std::map<std::string, int>::const_iterator it = m_design_generic_ids.find(name);
    if (it == m_design_generic_ids.end())
        return ShipDesign::INVALID_DESIGN_ID;
    else
        return it->second;
}

std::map<std::string, int> PredefinedShipDesignManager::AddShipDesignsToEmpire(Empire* empire) const {
    std::map<std::string, int> retval;

    if (!empire)
        return retval;

    for (iterator it = begin(); it != end(); ++it) {
        ShipDesign* d = it->second;

        if (it->first != d->Name(false)) {
            Logger().errorStream() << "Predefined ship design name in map (" << it->first << ") doesn't match name in ShipDesign::m_name (" << d->Name(false) << ")";
        }

        ShipDesign* copy = new ShipDesign(d->Name(false), d->Description(), empire->EmpireID(),
                                          d->DesignedOnTurn(), d->Hull(), d->Parts(),
                                          d->Graphic(), d->Model(), true);

        int design_id = empire->AddShipDesign(copy);    // also inserts design into Universe

        if (design_id == ShipDesign::INVALID_DESIGN_ID) {
            delete copy;
            Logger().errorStream() << "PredefinedShipDesignManager::AddShipDesignsToEmpire couldn't add a design to an empire";
        } else {
            retval[it->first] = design_id;
        }
    }

    return retval;
}

const std::map<std::string, int>& PredefinedShipDesignManager::AddShipDesignsToUniverse() const {
    m_design_generic_ids.clear();   // std::map<std::string, int>

    Universe& universe = GetUniverse();

    for (iterator it = begin(); it != end(); ++it) {
        ShipDesign* d = it->second;

        if (it->first != d->Name(false)) {
            Logger().errorStream() << "Predefined ship design name in map (" << it->first << ") doesn't match name in ShipDesign::m_name (" << d->Name(false) << ")";
        }

        ShipDesign* copy = new ShipDesign(d->Name(false), d->Description(), ALL_EMPIRES,
                                          d->DesignedOnTurn(), d->Hull(), d->Parts(),
                                          d->Graphic(), d->Model(), true);

        if (!copy) {
            Logger().errorStream() << "PredefinedShipDesignManager::AddShipDesignsToUniverse() couldn't duplicate the design with name " << d->Name();
            return m_design_generic_ids;
        }

        const ShipDesign& design_ref = *copy;

        bool already_added = false;

        /* check if there already exists this same design in the universe. */
        for (Universe::ship_design_iterator it = universe.beginShipDesigns(); it != universe.endShipDesigns(); ++it) {
            const ShipDesign* existing_design = it->second;
            if (!existing_design) {
                Logger().errorStream() << "PredefinedShipDesignManager::AddShipDesignsToUniverse found an invalid design in the Universe";
                continue;
            }

            const ShipDesign& existing_design_ref = *existing_design;

            if (DesignsTheSame(existing_design_ref, design_ref)) {
                Logger().debugStream() << "PredefinedShipDesignManager::AddShipDesignsToUniverse found there already is an exact duplicate of a design to be added, so is not re-adding it";
                m_design_generic_ids[design_ref.Name(false)] = existing_design_ref.ID();
                already_added = true;
                break;
            }
        }

        if (already_added) {
            delete copy;
            continue;
        }

        // design is apparently new, so add it to the universe
        int new_design_id = GetNewDesignID();   // generate new design id

        if (new_design_id == ShipDesign::INVALID_DESIGN_ID) {
            Logger().errorStream() << "PredefinedShipDesignManager::AddShipDesignsToUniverse Unable to get new design id";
            delete copy;
            return m_design_generic_ids;
        }

        bool success = universe.InsertShipDesignID(copy, new_design_id);

        if (!success) {
            Logger().errorStream() << "Empire::AddShipDesign Unable to add new design to universe";
            delete copy;
            return m_design_generic_ids;
        }

        m_design_generic_ids[design_ref.Name(false)] = new_design_id;
    }

    return m_design_generic_ids;
}

PredefinedShipDesignManager& PredefinedShipDesignManager::GetPredefinedShipDesignManager() {
    static PredefinedShipDesignManager manager;
    return manager;
}

PredefinedShipDesignManager::iterator PredefinedShipDesignManager::begin() const {
    return m_ship_designs.begin();
}

PredefinedShipDesignManager::iterator PredefinedShipDesignManager::end() const {
    return m_ship_designs.end();
}


///////////////////////////////////////////////////////////
// Free Functions                                        //
///////////////////////////////////////////////////////////
const PredefinedShipDesignManager& GetPredefinedShipDesignManager() {
    return PredefinedShipDesignManager::GetPredefinedShipDesignManager();
}

const ShipDesign* GetPredefinedShipDesign(const std::string& name) {
    int universe_generic_design_id = GetPredefinedShipDesignManager().GenericUniverseDesignID(name);    // gets id under which the premade ship design of the indicated name was stored generically in the universe.  that is, the id under which the design was stored with no empire creator
    return GetUniverse().GetShipDesign(universe_generic_design_id); // may return 0
}
