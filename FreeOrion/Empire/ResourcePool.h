// -*- C++ -*-
#ifndef _ResourcePool_h_
#define _ResourcePool_h_

#ifndef BOOST_SIGNAL_HPP
#include <boost/signal.hpp>
#endif

#ifndef _UniverseObject_h_
#include "../universe/UniverseObject.h"
#endif

class Planet;
namespace GG {class XMLElement;}

template <class PoolT> 
class PlanetChangedFunctor
{
public:
    PlanetChangedFunctor(PoolT &pool, int planet_id);
    void operator()();

private:
    PoolT& m_pool;
    int    m_planet_id;
};

/**
* Base class for all resource pool.
*/
class ResourcePool
{
public:
    typedef bool (*SortFuncType)(const Planet*,const Planet*);///< type of function used to sort the planet vector

    ResourcePool();
    virtual ~ResourcePool();
    
    /** \name Signal Types */ //@{
    typedef boost::signal<void ()> ChangedSignalType;
    //@}

    ChangedSignalType& ChangedSignal() const {return m_changed_sig;} ///< returns the changed signal object for this ResourcePool

    virtual GG::XMLElement XMLEncode() const = 0;

    virtual double Stockpile() const;

    std::vector<Planet*>& Planets()       {return m_planets;} ///< returns the planet vector 
    const std::vector<Planet*>& Planets() const {return m_planets;} ///< returns the planet vector 

    void SetPlanets(const Universe::ObjectVec &planet_vec);///< sets the planet vector 

protected:
    virtual SortFuncType SortFunc() const; ///< used to order planet list
    virtual void PlanetChanged(int m_planet_id) = 0; ///< called when a planet of the planet vector has changed

private:
    void OnPlanetChanged(int m_planet_id);///< called through the PlanetChangedFunctor when a planet has changed

    std::vector<Planet*> m_planets; ///< list of planet which feed/consume the resource
    std::vector<boost::signals::connection > m_connections;///< connection list of planets

    friend class PlanetChangedFunctor<ResourcePool>;
    mutable ChangedSignalType m_changed_sig;
};

/**
* Resource pool for minerals.
*/
class MineralResourcePool : public ResourcePool
{
public:
    MineralResourcePool();
    MineralResourcePool(const GG::XMLElement& elem);
    
    double Available() const {return m_overall_pool;} ///< amount of mineral which is produced by the planets
    double Needed   () const {return m_needed_pool;}///< amount of mineral which is needed to support planet production

    virtual double Stockpile() const;

    virtual GG::XMLElement XMLEncode() const;

protected:
    virtual SortFuncType SortFunc() const; 
    virtual void PlanetChanged(int m_planet_id);

private:
    double m_overall_pool,m_available_pool,m_needed_pool,m_stockpile;
};

/**
* Resource pool for food.
*/
class FoodResourcePool : public ResourcePool
{
public:
    FoodResourcePool();
    FoodResourcePool(const GG::XMLElement& elem);

    double Available() const {return m_overall_pool;}
    double Needed   () const {return m_needed_pool;}

    virtual double Stockpile() const;

    virtual GG::XMLElement XMLEncode() const;

protected:
    virtual SortFuncType SortFunc() const; 
    virtual void PlanetChanged(int m_planet_id);

private:
    double m_overall_pool,m_available_pool,m_needed_pool,m_stockpile;
};

/**
* Resource pool for research.
*/
class ResearchResourcePool : public ResourcePool
{
public:
    ResearchResourcePool();
    ResearchResourcePool(const GG::XMLElement& elem);

    double Available() const {return m_overall_pool;}

    virtual GG::XMLElement XMLEncode() const;

protected:
    virtual void PlanetChanged(int m_planet_id);

private:
    double m_overall_pool;
};

/**
* Resource pool for population.
*/
class PopulationResourcePool : public ResourcePool
{
public:
    PopulationResourcePool();
    PopulationResourcePool(const GG::XMLElement& elem);

    double Available() const {return m_overall_pool;}
    double Growth   () const {return m_growth;}

    virtual GG::XMLElement XMLEncode() const;

protected:
    virtual void PlanetChanged(int m_planet_id);

private:
    double m_overall_pool,m_growth;
};

/**
* Resource pool for industry.
*/
class IndustryResourcePool : public ResourcePool
{
public:
    IndustryResourcePool();
    IndustryResourcePool(const GG::XMLElement& elem);

    double Available() const {return m_overall_pool;}

    virtual GG::XMLElement XMLEncode() const;

protected:
    virtual void PlanetChanged(int m_planet_id);

private:
    double m_overall_pool;
};

/**
* Resource pool for trade.
*/
class TradeResourcePool : public ResourcePool
{
public:
    TradeResourcePool();
    TradeResourcePool(const GG::XMLElement& elem);

    double Available() const {return m_overall_pool;}
    double Needed   () const {return m_needed_pool;}

    virtual double Stockpile() const;

    virtual GG::XMLElement XMLEncode() const;

protected:
    virtual SortFuncType SortFunc() const; 
    virtual void PlanetChanged(int m_planet_id);

private:
    double m_overall_pool,m_available_pool,m_needed_pool,m_stockpile;
};


// template implementations
template <class PoolT> 
PlanetChangedFunctor<PoolT>::PlanetChangedFunctor(PoolT& pool, int planet_id) :
    m_pool(pool),
    m_planet_id(planet_id)
{}

template <class PoolT> 
void PlanetChangedFunctor<PoolT>::operator()()
{
    m_pool.OnPlanetChanged(m_planet_id);
}

inline std::pair<std::string, std::string> ResourcePoolRevision()
{return std::pair<std::string, std::string>("$RCSfile$", "$Revision$");}

#endif // _ResourcePool_h_
