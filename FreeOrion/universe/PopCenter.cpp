#include "PopCenter.h"
#include "../GG/XML/XMLDoc.h"

#include <stdexcept>

PopCenter::PopCenter() : 
   UniverseObject(),
   m_pop(0.0),
   m_max_pop(0.0),
   m_growth(0.0),
   m_race(-1)
{
}
   
PopCenter::PopCenter(double max_pop) : 
   UniverseObject(),
   m_pop(0.0),
   m_max_pop(max_pop),
   m_growth(0.0),
   m_race(-1)
{
}
   
PopCenter::PopCenter(double max_pop, int race) : 
   UniverseObject(),
   m_pop(0.0),
   m_max_pop(max_pop),
   m_growth(0.0),
   m_race(race)
{
}
   
PopCenter::PopCenter(const GG::XMLElement& elem) : 
   UniverseObject()
{
   if (elem.Tag() != "PopCenter")
      throw std::invalid_argument("Attempted to construct a PopCenter from an XMLElement that had a tag other than \"PopCenter\"");
   // TODO
}

PopCenter::~PopCenter()
{
}

double PopCenter::Inhabitants() const
{
   // TODO
}

PopCenter::DensityType PopCenter::PopDensity() const
{
   DensityType retval = OUTPOST;
   // TODO
   return retval;
}

GG::XMLElement PopCenter::XMLEncode() const
{
   // TODO
}

double PopCenter::AdjustPop(double pop)
{
   double retval = 0.0;
   m_pop += pop;
   if (m_pop < 0.0) {
      retval = m_pop;
      m_pop = 0.0;
   } else if (m_max_pop < pop) {
      retval = m_pop - m_max_pop;
      m_pop = m_max_pop;
   }
   return retval;
}

void PopCenter::MovementPhase(std::vector<SitRepEntry>& sit_reps)
{
   // TODO
}

void PopCenter::PopGrowthProductionResearchPhase(std::vector<SitRepEntry>& sit_reps)
{
   // TODO
}

void PopCenter::XMLMerge(const GG::XMLElement& elem)
{
   // TODO
}

