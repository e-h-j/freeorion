// -*- C++ -*-
#ifndef _Building_h_
#define _Building_h_

#include "UniverseObject.h"
#include "ObjectMap.h"
#include "ValueRefFwd.h"
#include "../util/Export.h"

class BuildingType;
namespace Effect {
    class EffectsGroup;
}
namespace Condition {
    struct ConditionBase;
}

/** A Building UniverseObject type. */
class FO_COMMON_API Building : public UniverseObject {
public:
    /** \name Accessors */ //@{
    virtual std::set<std::string>
                                Tags() const;                                       ///< returns all tags this object has
    virtual bool                HasTag(const std::string& name) const;              ///< returns true iff this object has the tag with the indicated \a name

    virtual const std::string&  TypeName() const;   ///< returns user-readable string indicating the type of UniverseObject this is
    virtual UniverseObjectType  ObjectType() const;
    virtual std::string         Dump() const;

    const std::string&      BuildingTypeName() const    { return m_building_type; };        ///< returns the name of the BuildingType object for this building

    virtual int             ContainerObjectID() const   { return m_planet_id; }             ///< returns id of the object that directly contains this object, if any, or INVALID_OBJECT_ID if this object is not contained by any other
    virtual bool            ContainedBy(int object_id) const;                               ///< returns true if there is an object with id \a object_id that contains this UniverseObject
    int                     PlanetID() const            { return m_planet_id; }             ///< returns the ID number of the planet this building is on

    int                     ProducedByEmpireID() const  { return m_produced_by_empire_id; } ///< returns the empire ID of the empire that produced this building

    virtual TemporaryPtr<UniverseObject>
                            Accept(const UniverseObjectVisitor& visitor) const;

    bool                    OrderedScrapped() const     { return m_ordered_scrapped; }
    //@}

    /** \name Mutators */ //@{
    virtual void    Copy(TemporaryPtr<const UniverseObject> copied_object, int empire_id = ALL_EMPIRES);

    void            SetPlanetID(int planet_id);         ///< sets the planet on which the building is located

    void            Reset();                            ///< resets any building state, and removes owners
    void            SetOrderedScrapped(bool b = true);  ///< flags building for scrapping

    virtual void    ResetTargetMaxUnpairedMeters();
    //@}

protected:
    friend class Universe;
    /** \name Structors */ //@{
    Building() :
        UniverseObject(),
        m_building_type(),
        m_planet_id(INVALID_OBJECT_ID),
        m_ordered_scrapped(false),
        m_produced_by_empire_id(ALL_EMPIRES)
    {}
    Building(int empire_id, const std::string& building_type,
             int produced_by_empire_id = ALL_EMPIRES);                  ///< basic ctor
    
    template <class T> friend void boost::python::detail::value_destroyer<false>::execute(T const volatile* p);
    template <class T> friend void boost::checked_delete(T* x);
    ~Building() {}

    virtual Building*       Clone(int empire_id = ALL_EMPIRES) const;   ///< returns new copy of this Building
    //@}

private:
    std::string m_building_type;
    int         m_planet_id;
    bool        m_ordered_scrapped;
    int         m_produced_by_empire_id;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

/** A specification for a building of a certain type.  Each building type must
  * have a \a unique name string, by which it can be looked up using
  * GetBuildingType(...). */
class FO_COMMON_API BuildingType {
public:
    /** \name Structors */ //@{
    /** default ctor */
    BuildingType() :
        m_name(""),
        m_description(""),
        m_production_cost(0),
        m_production_time(0),
        m_capture_result(CR_DESTROY),
        m_tags(),
        m_location(0),
        m_effects(0),
        m_icon("")
    {};

    /** basic ctor */
    BuildingType(const std::string& name, const std::string& description,
                 const ValueRef::ValueRefBase<double>* production_cost,
                 const ValueRef::ValueRefBase<int>* production_time,
                 bool producible,
                 CaptureResult capture_result,
                 const std::set<std::string>& tags,
                 const Condition::ConditionBase* location,
                 const std::vector<boost::shared_ptr<const Effect::EffectsGroup> >& effects,
                 const std::string& icon) :
        m_name(name),
        m_description(description),
        m_production_cost(production_cost),
        m_production_time(production_time),
        m_producible(producible),
        m_capture_result(capture_result),
        m_tags(tags),
        m_location(location),
        m_effects(effects),
        m_icon(icon)
    {}

    ~BuildingType(); ///< dtor
    //@}

    /** \name Accessors */ //@{
    const std::string&              Name() const            { return m_name; }              ///< returns the unique name for this type of building
    const std::string&              Description() const     { return m_description; }       ///< returns a text description of this type of building
    std::string                     Dump() const;                                           ///< returns a data file format representation of this object

    bool                            ProductionCostTimeLocationInvariant() const;            ///< returns true if the production cost and time are invariant (does not depend on) the location
    float                           ProductionCost(int empire_id, int location_id) const;   ///< returns the number of production points required to build this building
    float                           PerTurnCost(int empire_id, int location_id) const;      ///< returns the maximum number of production points per turn that can be spend on this building
    int                             ProductionTime(int empire_id, int location_id) const;   ///< returns the number of turns required to build this building

    bool                            Producible() const      { return m_producible; }        ///< returns whether this building type is producible by players and appears on the production screen
    const std::set<std::string>& Tags() const               { return m_tags; }
    const Condition::ConditionBase* Location() const        { return m_location; }          ///< returns the condition that determines the locations where this building can be produced
    const std::vector<boost::shared_ptr<const Effect::EffectsGroup> >&
                                    Effects() const         { return m_effects; }           ///< returns the EffectsGroups that encapsulate the effects that buildings of this type have when operational
    const std::string&              Icon() const            { return m_icon; }              ///< returns the name of the grapic file for this building type
    bool ProductionLocation(int empire_id, int location_id) const;  ///< returns true iff the empire with ID empire_id can produce this building at the location with location_id

    /** returns CaptureResult for empire with ID \a to_empire_id capturing from
      * empire with IDs \a from_empire_id the planet (or other UniverseObject)
      * with id \a location_id on which this type of Building is located (if 
      * \a as_production_item is false) or which is the location of a Production
      * Queue BuildItem for a building of this type (otherwise) */
    CaptureResult                   GetCaptureResult(int from_empire_id, int to_empire_id,
                                                     int location_id, bool as_production_item) const
    { return m_capture_result; }
    //@}

private:
    std::string                                                 m_name;
    std::string                                                 m_description;
    const ValueRef::ValueRefBase<double>*                       m_production_cost;
    const ValueRef::ValueRefBase<int>*                          m_production_time;
    bool                                                        m_producible;
    CaptureResult                                               m_capture_result;
    std::set<std::string>                                       m_tags;
    const Condition::ConditionBase*                             m_location;
    std::vector<boost::shared_ptr<const Effect::EffectsGroup> > m_effects;
    std::string                                                 m_icon;
};

/** Holds all FreeOrion building types.  Types may be looked up by name. */
class BuildingTypeManager {
public:
    typedef std::map<std::string, BuildingType*>::const_iterator iterator;

    /** \name Accessors */ //@{
    /** returns the building type with the name \a name; you should use the
      * free function GetBuildingType(...) instead, mainly to save some typing. */
    const BuildingType*         GetBuildingType(const std::string& name) const;

    /** iterator to the first building type */
    iterator                    begin() const   { return m_building_types.begin(); }

    /** iterator to the last + 1th building type */
    iterator                    end() const     { return m_building_types.end(); }

    /** returns the instance of this singleton class; you should use the free
      * function GetBuildingTypeManager() instead */
    static BuildingTypeManager& GetBuildingTypeManager();
    //@}

private:
    BuildingTypeManager();
    ~BuildingTypeManager();

    std::map<std::string, BuildingType*> m_building_types;

    static BuildingTypeManager* s_instance;
};

/** returns the singleton building type manager */
FO_COMMON_API BuildingTypeManager& GetBuildingTypeManager();

/** Returns the BuildingType specification object for a building of
  * type \a name.  If no such BuildingType exists, 0 is returned instead. */
FO_COMMON_API const BuildingType* GetBuildingType(const std::string& name);

#endif // _Building_h_
