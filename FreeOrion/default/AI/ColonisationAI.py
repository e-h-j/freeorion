import freeOrionAIInterface as fo
import FreeOrionAI as foAI
import AIstate
import FleetUtilsAI
from EnumsAI import AIFleetMissionType, AIExplorableSystemType, AITargetType, AIFocusType
import AITarget
import PlanetUtilsAI
import TechsListsAI
import ProductionAI 
import AIDependencies

empireSpecies = {}
empireSpeciesSystems={}
empireColonizers = {}
activeGrowthSpecials={}
annexableSystemIDs=set([])
annexableRing1=set([])
annexableRing2=set([])
annexableRing3=set([])
annexablePlanetIDs=set([])
curBestMilShipRating = 20

# makes these mapped to string version of values in case any sizes become reals instead of int
planetSIzes=            {   str(fo.planetSize.tiny): 1,     str(fo.planetSize.small): 2,    str(fo.planetSize.medium): 3,   str(fo.planetSize.large): 4,    str(fo.planetSize.huge): 5,  str(fo.planetSize.asteroids): 3,  str(fo.planetSize.gasGiant): 3 }
environs =                  { str(fo.planetEnvironment.uninhabitable): 0,  str(fo.planetEnvironment.hostile): 1,  str(fo.planetEnvironment.poor): 2,  str(fo.planetEnvironment.adequate): 3,  str(fo.planetEnvironment.good):4 }
photoMap= { fo.starType.blue:3    , fo.starType.white:1.5  , fo.starType.red:-1 ,  fo.starType.neutron: -1 , fo.starType.blackHole: -10 , fo.starType.noStar: -10     }
#   mods per environ    uninhab   hostile    poor   adequate    good
popSizeModMap={
                            "env":               [ 0, -4, -2, 0,  3 ], 
                            "subHab":      [ 0,  1,  1,  1,  1 ], 
                            "symBio":       [ 0,  0,  1,  1,  1 ], 
                            "xenoGen":  [ 0,  1,  2,  2,  0 ], 
                            "xenoHyb":   [ 0,  2,  1,  0,  0 ], 
                            "cyborg":       [ 0,  2,  0,  0,  0 ], 
                            "ndim":            [ 0, 2,  2,  2,   2 ], 
                            "orbit":            [ 0,  1,  1,  1,  1 ], 
                            "gaia":             [ 0,  3,  3,  3,  3 ], 
                            }


def getColonyFleets():
    global empireSpecies,  empireColonizers,  empireSpeciesSystems,  annexableSystemIDs,  annexableRing1,  annexableRing2,  annexableRing3
    global  annexablePlanetIDs,  curBestMilShipRating
    
    curBestMilShipRating = ProductionAI.curBestMilShipRating()
    
    "get colony fleets"

    allColonyFleetIDs = FleetUtilsAI.getEmpireFleetIDsByRole(AIFleetMissionType.FLEET_MISSION_COLONISATION)
    AIstate.colonyFleetIDs[:] = FleetUtilsAI.extractFleetIDsWithoutMissionTypes(allColonyFleetIDs)

    # get suppliable systems and planets
    universe = fo.getUniverse()
    empire = fo.getEmpire()
    empireID = empire.empireID
    capitalID = PlanetUtilsAI.getCapital()
    #capitalID = empire.capitalID
    homeworld=None
    if capitalID:
        homeworld = universe.getPlanet(capitalID)
    if homeworld:
        speciesName = homeworld.speciesName
        homeworldName=homeworld.name
        homeSystemID = homeworld.systemID
    else:
        speciesName = ""
        homeworldName=" no remaining homeworld "
        homeSystemID = -1
    if not speciesName:
        speciesName = foAI.foAIstate.origSpeciesName
    species = fo.getSpecies(speciesName)
    if not species:
        print "**************************************************************************************"
        print "**************************************************************************************"
        print "Problem determining species for colonization planning: capitalID: %s,  homeworld %s  and species name %s"%(capitalID,  homeworldName,  speciesName)
    else:
        print "Plannning colonization for species name %s"%species.name

    fleetSupplyableSystemIDs = empire.fleetSupplyableSystemIDs
    fleetSupplyablePlanetIDs = PlanetUtilsAI.getPlanetsInSystemsIDs(fleetSupplyableSystemIDs)
    print ""
    print "    fleetSupplyableSystemIDs: " + str(list(fleetSupplyableSystemIDs))
    print "    fleetSupplyablePlanetIDs: " + str(fleetSupplyablePlanetIDs)
    print ""

    print "-------\nEmpire Obstructed Starlanes:"
    print  list(empire.obstructedStarlanes())


    annexableSystemIDs.clear()
    annexableRing1.clear()
    annexableRing2.clear()
    annexableRing3.clear()
    annexablePlanetIDs.clear()
    for sysID in empire.fleetSupplyableSystemIDs:
        annexableSystemIDs.add(sysID)
        for nID in  universe.getImmediateNeighbors(sysID,  empireID):
            annexableSystemIDs.add(nID)
            annexableRing1.add(nID)
    annexableRing1.difference_update(empire.fleetSupplyableSystemIDs)
    print "First Ring of annexable systems: ",  PlanetUtilsAI.sysNameIDs(annexableRing1)
    if empire.getTechStatus("CON_ORBITAL_CON") == fo.techStatus.complete:
        for sysID in list(annexableRing1):
            for nID in  universe.getImmediateNeighbors(sysID,  empireID):
                annexableRing2.add(nID)
        annexableRing2.difference_update(annexableSystemIDs)
        print "Second Ring of annexable systems: ",  PlanetUtilsAI.sysNameIDs(annexableRing2)
        annexableSystemIDs.update(annexableRing2)
        if foAI.foAIstate.aggression > fo.aggression.cautious:
            for sysID in list(annexableRing2):
                for nID in  universe.getImmediateNeighbors(sysID,  empireID):
                    annexableRing3.add(nID)
            annexableRing3.difference_update(annexableSystemIDs)
            print "Third Ring of annexable systems: ",  PlanetUtilsAI.sysNameIDs(annexableRing3)
            annexableSystemIDs.update(annexableRing3)
    annexablePlanetIDs.update( PlanetUtilsAI.getPlanetsInSystemsIDs(annexableSystemIDs))

    # get outpost and colonization planets
    
    exploredSystemIDs = foAI.foAIstate.getExplorableSystems(AIExplorableSystemType.EXPLORABLE_SYSTEM_EXPLORED)
    unExSysIDs = list(foAI.foAIstate.getExplorableSystems(AIExplorableSystemType.EXPLORABLE_SYSTEM_UNEXPLORED))
    unExSystems = map(universe.getSystem,  unExSysIDs)
    print "Unexplored Systems: %s " % [(sysID,  (sys and sys.name) or "name unknown") for sysID,  sys in zip( unExSysIDs,  unExSystems)]
    print "Explored SystemIDs: " + str(list(exploredSystemIDs))

    exploredPlanetIDs = PlanetUtilsAI.getPlanetsInSystemsIDs(exploredSystemIDs)
    print "Explored PlanetIDs: " + str(exploredPlanetIDs)
    print ""
    
    #visibleSystemIDs = foAI.foAIstate.visInteriorSystemIDs.keys() + foAI.foAIstate. visBorderSystemIDs.keys()
    #visiblePlanetIDs = PlanetUtilsAI.getPlanetsInSystemsIDs(visibleSystemIDs)
    #print "VisiblePlanets: %s "%[ (pid,  (universe.getPlanet(pid) and  universe.getPlanet(pid).name) or "unknown") for pid in  visiblePlanetIDs]
    #print ""
    
    #accessibleSystemIDs = [sysID for sysID in visibleSystemIDs if  universe.systemsConnected(sysID, homeSystemID, empireID) ]
    #acessiblePlanetIDs = PlanetUtilsAI.getPlanetsInSystemsIDs(accessibleSystemIDs)

    empireOwnedPlanetIDs = PlanetUtilsAI.getOwnedPlanetsByEmpire(universe.planetIDs, empireID)
    print "Empire Owned PlanetIDs:            " + str(empireOwnedPlanetIDs)
    
    #allOwnedPlanetIDs = PlanetUtilsAI.getAllOwnedPlanetIDs(exploredPlanetIDs) #working with Explored systems not all 'visible' because might not have a path to the latter
    allOwnedPlanetIDs = PlanetUtilsAI.getAllOwnedPlanetIDs(annexablePlanetIDs) #
    print "All annexable Owned or Populated PlanetIDs: " + str(set(allOwnedPlanetIDs)-set(empireOwnedPlanetIDs))

    #unOwnedPlanetIDs = list(set(exploredPlanetIDs) -set(allOwnedPlanetIDs))
    unOwnedPlanetIDs = list(set(annexablePlanetIDs) -set(allOwnedPlanetIDs))
    print "UnOwned annexable PlanetIDs:             " + str(PlanetUtilsAI.planetNameIDs(unOwnedPlanetIDs))
    
    empirePopCtrs = set( PlanetUtilsAI.getPopulatedPlanetIDs(  empireOwnedPlanetIDs) )
    empireOutpostIDs=set(empireOwnedPlanetIDs) - empirePopCtrs
    AIstate.popCtrIDs[:]=list(empirePopCtrs)
    AIstate.popCtrSystemIDs[:]=list(set(PlanetUtilsAI.getSystems(empirePopCtrs)))
    AIstate.outpostIDs[:]=list(empireOutpostIDs)
    AIstate.outpostSystemIDs[:]=list(set(PlanetUtilsAI.getSystems(empireOutpostIDs)))
    AIstate.colonizedSystems.clear()
    for pid in empireOwnedPlanetIDs:
        planet=universe.getPlanet(pid)
        if planet:
            AIstate.colonizedSystems.setdefault(planet.systemID,  []).append(pid)   # track these to plan Solar Generators and Singularity Generators
    AIstate.empireStars.clear()
    for sysID in AIstate.colonizedSystems:
        system = universe.getSystem(sysID)
        if system:
            AIstate.empireStars.setdefault(system.starType, []).append(sysID)
    
    
    oldPopCtrs=[]
    for specN in empireSpecies:
        oldPopCtrs.extend(empireSpecies[specN])
    oldEmpSpec = empireSpecies
    empireSpecies.clear()
    oldEmpCol=empireColonizers
    empireColonizers.clear()
    if empire.getTechStatus(TechsListsAI.exobotTechName) == fo.techStatus.complete:
        empireColonizers["SP_EXOBOT"]=[]# get it into colonizer list even if no colony yet
    empireSpeciesSystems.clear()
    
    for pID in empirePopCtrs:
        planet=universe.getPlanet(pID)
        if not planet:
            print "Error empire has apparently lost sight of former colony at planet %d but doesn't realize it"%pID
            continue
        pSpecName=planet.speciesName
        if pID not in oldPopCtrs:
            if  (AIFocusType.FOCUS_MINING in planet.availableFoci): 
                fo.issueChangeFocusOrder(pID, AIFocusType.FOCUS_MINING)
                print "Changing focus of newly settled planet ID %d : %s  to mining "%(pID,  planet.name )
        empireSpecies[pSpecName] = empireSpecies.get(pSpecName,  [])+[pID]
    print "\n"+"Empire species roster:"
    for specName in empireSpecies:
        thisSpec=fo.getSpecies(specName)
        if thisSpec:
            shipyards=[]
            for pID in empireSpecies[specName]:
                planet=universe.getPlanet(pID)
                if thisSpec.canColonize:
                    if "BLD_SHIPYARD_BASE" in [universe.getObject(bldg).buildingTypeName for bldg in planet.buildingIDs]:
                        shipyards.append(pID)
                empireSpeciesSystems.setdefault(planet.systemID,  {}).setdefault('pids', []).append(pID)
            if thisSpec.canColonize:
                empireColonizers[specName]=shipyards
            print "%s on planets %s; can%s colonize from %d shipyards; has tags %s"%(specName,  empireSpecies[specName],  ["not", ""][thisSpec.canColonize], len(shipyards),  list(thisSpec.tags))
        else:
            print "Unable to retrieve info for Species named %s"%specName
    print""
    if empireSpecies!=oldEmpSpec:
        print "Old empire species: %s  ; new empire species: %s"%(oldEmpSpec,  empireSpecies)
    if empireColonizers!=oldEmpCol:
        print "Old empire colonizers: %s  ; new empire colonizers: %s"%(oldEmpCol,  empireColonizers)
    
    print 

    # export colony targeted systems for other AI modules
    colonyTargetedPlanetIDs = getColonyTargetedPlanetIDs(universe.planetIDs, AIFleetMissionType.FLEET_MISSION_COLONISATION, empireID)
    allColonyTargetedSystemIDs = PlanetUtilsAI.getSystems(colonyTargetedPlanetIDs)
    AIstate.colonyTargetedSystemIDs = allColonyTargetedSystemIDs
    print ""
    print "Colony Targeted SystemIDs:         " + str(AIstate.colonyTargetedSystemIDs)
    print "Colony Targeted PlanetIDs:         " + str(colonyTargetedPlanetIDs)

    colonyFleetIDs = FleetUtilsAI.getEmpireFleetIDsByRole(AIFleetMissionType.FLEET_MISSION_COLONISATION)
    if not colonyFleetIDs:
        print "Available Colony Fleets:             0"
    else:
        print "Colony FleetIDs:                   " + str(FleetUtilsAI.getEmpireFleetIDsByRole(AIFleetMissionType.FLEET_MISSION_COLONISATION))

    numColonyFleets = len(FleetUtilsAI.extractFleetIDsWithoutMissionTypes(colonyFleetIDs))
    print "Colony Fleets Without Missions:      " + str(numColonyFleets)

    outpostTargetedPlanetIDs = getOutpostTargetedPlanetIDs(universe.planetIDs, AIFleetMissionType.FLEET_MISSION_OUTPOST, empireID)
    allOutpostTargetedSystemIDs = PlanetUtilsAI.getSystems(outpostTargetedPlanetIDs)

    # export outpost targeted systems for other AI modules
    AIstate.outpostTargetedSystemIDs = allOutpostTargetedSystemIDs
    print ""
    print "Outpost Targeted SystemIDs:        " + str(AIstate.outpostTargetedSystemIDs)
    print "Outpost Targeted PlanetIDs:        " + str(outpostTargetedPlanetIDs)

    outpostFleetIDs = FleetUtilsAI.getEmpireFleetIDsByRole(AIFleetMissionType.FLEET_MISSION_OUTPOST)
    if not outpostFleetIDs:
        print "Available Outpost Fleets:            0"
    else:
        print "Outpost FleetIDs:                  " + str(FleetUtilsAI.getEmpireFleetIDsByRole(AIFleetMissionType.FLEET_MISSION_OUTPOST))

    numOutpostFleets = len(FleetUtilsAI.extractFleetIDsWithoutMissionTypes(outpostFleetIDs))
    print "Outpost Fleets Without Missions:     " + str(numOutpostFleets)

    evaluatedColonyPlanetIDs = list(set(unOwnedPlanetIDs).union(empireOutpostIDs) - set(colonyTargetedPlanetIDs) )
    # print "Evaluated Colony PlanetIDs:        " + str(evaluatedColonyPlanetIDs)

    evaluatedOutpostPlanetIDs = list(set(unOwnedPlanetIDs) - set(outpostTargetedPlanetIDs)- set(colonyTargetedPlanetIDs))
    # print "Evaluated Outpost PlanetIDs:       " + str(evaluatedOutpostPlanetIDs)

    evaluatedColonyPlanets = assignColonisationValues(evaluatedColonyPlanetIDs, AIFleetMissionType.FLEET_MISSION_COLONISATION, fleetSupplyablePlanetIDs, species, empire)
    removeLowValuePlanets(evaluatedColonyPlanets)

    sortedPlanets = evaluatedColonyPlanets.items()
    sortedPlanets.sort(lambda x, y: cmp(x[1], y[1]), reverse=True)

    print ""
    print "Settleable Colony Planets (score,species) | ID | Name | Specials:"
    for ID, score in sortedPlanets:
        print "   %15s | %5s  | %s  | %s "%(score,  ID,  universe.getPlanet(ID).name ,  list(universe.getPlanet(ID).specials)) 
    print ""

    # export planets for other AI modules
    foAI.foAIstate.colonisablePlanetIDs = sortedPlanets#TODO: should include species designation corresponding to rating

    # get outpost fleets
    allOutpostFleetIDs = FleetUtilsAI.getEmpireFleetIDsByRole(AIFleetMissionType.FLEET_MISSION_OUTPOST)
    AIstate.outpostFleetIDs = FleetUtilsAI.extractFleetIDsWithoutMissionTypes(allOutpostFleetIDs)

    evaluatedOutpostPlanets = assignColonisationValues(evaluatedOutpostPlanetIDs, AIFleetMissionType.FLEET_MISSION_OUTPOST, fleetSupplyablePlanetIDs, species, empire)
    removeLowValuePlanets(evaluatedOutpostPlanets) #bad! lol, was preventing all mining outposts

    sortedOutposts = evaluatedOutpostPlanets.items()
    sortedOutposts.sort(lambda x, y: cmp(x[1], y[1]), reverse=True)

    print "Settleable Outpost PlanetIDs:"
    for ID, score in sortedOutposts:
        print "   %5s | %5s  | %s  | %s "%(score,  ID,  universe.getPlanet(ID).name ,  list(universe.getPlanet(ID).specials)) 
    print ""

    # export outposts for other AI modules
    foAI.foAIstate.colonisableOutpostIDs = sortedOutposts

def getColonyTargetedPlanetIDs(planetIDs, missionType, empireID):
    "return list being settled with colony planets"

    universe = fo.getUniverse()
    colonyAIFleetMissions = foAI.foAIstate.getAIFleetMissionsWithAnyMissionTypes([missionType])

    colonyTargetedPlanets = []

    for planetID in planetIDs:
        planet = universe.getPlanet(planetID)
        # add planets that are target of a mission
        for colonyAIFleetMission in colonyAIFleetMissions:
            aiTarget = AITarget.AITarget(AITargetType.TARGET_PLANET, planetID)
            if colonyAIFleetMission.hasTarget(missionType, aiTarget):
                colonyTargetedPlanets.append(planetID)

    return colonyTargetedPlanets

def getOutpostTargetedPlanetIDs(planetIDs, missionType, empireID):
    "return list being settled with outposts planets"

    universe = fo.getUniverse()
    outpostAIFleetMissions = foAI.foAIstate.getAIFleetMissionsWithAnyMissionTypes([missionType])

    outpostTargetedPlanets = []

    for planetID in planetIDs:
        planet = universe.getPlanet(planetID)
        # add planets that are target of a mission
        for outpostAIFleetMission in outpostAIFleetMissions:
            aiTarget = AITarget.AITarget(AITargetType.TARGET_PLANET, planetID)
            if outpostAIFleetMission.hasTarget(missionType, aiTarget):
                outpostTargetedPlanets.append(planetID)

    return outpostTargetedPlanets

def assignColonyFleetsToColonise():
    # assign fleet targets to colonisable planets
    sendColonyShips(AIstate.colonyFleetIDs, foAI.foAIstate.colonisablePlanetIDs, AIFleetMissionType.FLEET_MISSION_COLONISATION)

    # assign fleet targets to colonisable outposts
    sendColonyShips(AIstate.outpostFleetIDs, foAI.foAIstate.colonisableOutpostIDs, AIFleetMissionType.FLEET_MISSION_OUTPOST)

def assignColonisationValues(planetIDs, missionType, fleetSupplyablePlanetIDs, species, empire): #TODO: clean up supplyable versus annexable
    "creates a dictionary that takes planetIDs as key and their colonisation score as value"

    planetValues = {}
    for planetID in planetIDs:
        pv = []
        for specName in empireColonizers:
            thisSpecies=fo.getSpecies(specName)
            pv.append( (evaluatePlanet(planetID, missionType, fleetSupplyablePlanetIDs, thisSpecies, empire),  specName) )
        best = sorted(pv)[-1:]
        if best!=[]:
            if   (missionType == AIFleetMissionType.FLEET_MISSION_OUTPOST ):
                planetValues[planetID] = (best[0][0],  "")
            else:#TODO: check for system-local colonizer; also,  if a tie amongst top try to choose empire main species
                planetValues[planetID] = best[0]

    return planetValues

def evaluatePlanet(planetID, missionType, fleetSupplyablePlanetIDs, species, empire):
    "returns the colonisation value of a planet"
    # TODO: in planet evaluation consider specials and distance
    discountMultiplier = 20.0

    valMod = 0
    universe = fo.getUniverse()
    empireResearchList = [element.tech for element in empire.researchQueue]
    planet = universe.getPlanet(planetID)
    if (planet == None): return 0
    system = universe.getSystem(planet.systemID)
    tagList=[]
    starBonus=0
    if species:
        tagList = [tag for tag in species.tags]
    starPopMod=0
    if system:
        if "PHOTOTROPHIC" in tagList:
            starPopMod = photoMap.get( system.starType,  0 )
        if (empire.getTechStatus("PRO_SOL_ORB_GEN") == fo.techStatus.complete) or (  "PRO_SOL_ORB_GEN"  in empireResearchList[:8])  :    
            if system.starType in [fo.starType.blue, fo.starType.white]:
                if len (AIstate.empireStars.get(fo.starType.blue,  [])+AIstate.empireStars.get(fo.starType.white,  []))==0:
                    starBonus +=20* discountMultiplier
                elif   planet.systemID not in (AIstate.popCtrSystemIDs + AIstate.outpostSystemIDs) :
                    starBonus +=1+10*discountMultiplier #still has extra value as an alternate location for solar generators
            if system.starType in [fo.starType.yellow, fo.starType.orange]:
                if len (     AIstate.empireStars.get(fo.starType.blue,  [])+AIstate.empireStars.get(fo.starType.white,  [])+
                                    AIstate.empireStars.get(fo.starType.yellow,  [])+AIstate.empireStars.get(fo.starType.orange,  []))==0:
                    starBonus +=10
                else:
                    starBonus +=2 #still has extra value as an alternate location for solar generators
        if (empire.getTechStatus("PRO_SINGULAR_GEN") == fo.techStatus.complete) or (  "PRO_SINGULAR_GEN"  in empireResearchList[:8])  :    
            if system.starType in [fo.starType.blackHole] :
                if len (AIstate.empireStars.get(fo.starType.blackHole,  []))==0:
                    starBonus +=80*discountMultiplier #pretty rare planets, good for generator
                elif  planet.systemID not in (AIstate.popCtrSystemIDs + AIstate.outpostSystemIDs):
                    starBonus +=40*discountMultiplier #still has extra value as an alternate location for generators & for bnlocking enemies generators
            elif system.starType in [fo.starType.red] and ( len (AIstate.empireStars.get(fo.starType.blackHole,  [])) + len (AIstate.empireStars.get(fo.starType.red,  [])))==0:
                if  planet.systemID not in (AIstate.popCtrSystemIDs + AIstate.outpostSystemIDs):
                    starBonus +=40*discountMultiplier # can be used for artificial black hole
        if (empire.getTechStatus("PRO_NEUTRONIUM_EXTRACTION") == fo.techStatus.complete) or (  "PRO_NEUTRONIUM_EXTRACTION"  in empireResearchList[:8])  :    
            if system.starType in [fo.starType.neutron]:
                if len (AIstate.empireStars.get(fo.starType.neutron,  []))==0:
                    starBonus +=40*discountMultiplier #pretty rare planets, good for armor
                else:
                    starBonus +=10*discountMultiplier #still has extra value as an alternate location for generators & for bnlocking enemies generators
    retval = starBonus
    
    planetSpecials = list(planet.specials)
    if   (missionType == AIFleetMissionType.FLEET_MISSION_OUTPOST ):
        for special in planetSpecials:
            if "_NEST_" in special:
                retval+=5*discountMultiplier # get an outpost on the nest quick
        if  ( ( planet.size  ==  fo.planetSize.asteroids ) and  (empire.getTechStatus("PRO_MICROGRAV_MAN") == fo.techStatus.complete )): 
            if system:
                astVal=0
                for pid in system.planetIDs:
                    otherPlanet=universe.getPlanet(pid)
                    if otherPlanet.size == fo.planetSize.asteroids:
                        if pid==planetID:
                            continue
                        elif pid < planetID:
                            astVal=0
                            break
                    elif otherPlanet.size!= fo.planetSize.gasGiant and otherPlanet.owner==empire.empireID:
                        astVal+=20 * discountMultiplier
                retval += astVal
        if  ( ( planet.size  ==  fo.planetSize.gasGiant ) and  ( (empire.getTechStatus("PRO_ORBITAL_GEN") == fo.techStatus.complete ) or (  "PRO_ORBITAL_GEN"  in empireResearchList[:10]) )): 
            if system:
                orbGenVal=0
                for pid in system.planetIDs:
                    otherPlanet=universe.getPlanet(pid)
                    if otherPlanet.size == fo.planetSize.asteroids and otherPlanet.owner==empire.empireID:
                        if empire.getTechStatus("PRO_EXOBOTS") == fo.techStatus.complete:
                            orbGenVal+=10*discountMultiplier
                    elif otherPlanet.size!= fo.planetSize.gasGiant and otherPlanet.owner==empire.empireID:
                        orbGenVal+=10*discountMultiplier
                retval += orbGenVal
        if foAI.foAIstate.systemStatus.get(planet.systemID,  {}).get('fleetThreat', 0) > 2*curBestMilShipRating:
            retval = retval / 2.0
        return int(retval)
    else: #colonization mission
        asteroidBonus=0
        gasGiantBonus=0
        miningBonus=0
        planetSize = planet.size
        if system:
            for pid  in [id for id in system.planetIDs if id != planetID]:
                p2 = universe.getPlanet(pid)
                if p2:
                    if p2.size== fo.planetSize.asteroids :
                        asteroidBonus = 30
                    if p2.size== fo.planetSize.gasGiant :
                        gasGiantBonus += 50
        if   (planet.size==fo.planetSize.gasGiant):
            if not (species and species.name  ==  "SP_SUPER_TEST"): 
                return 0
            else:
                planetEnv = fo.planetEnvironment.good#I think
                planetSize=4 #I think
        elif ( planet.size  ==  fo.planetSize.asteroids ):
            planetSize=3 #I think
            if  not species or (species.name not in [  "SP_EXOBOT", "SP_SUPER_TEST"  ]):
                return 0
            elif species.name == "SP_EXOBOT":
                planetEnv =fo.planetEnvironment.poor
            elif species.name == "SP_SUPER_TEST":
                planetEnv = fo.planetEnvironment.good#I think
            else:
                return 0
        else:
            planetEnv  = environs[ str(species.getPlanetEnvironment(planet.type)) ]
        if planetEnv==0:
            return -9999
        popSizeMod=0
        popSizeMod += popSizeModMap["env"][planetEnv]
        if "SELF_SUSTAINING" in tagList:
            popSizeMod*=2
        popSizeMod += starPopMod
        if (empire.getTechStatus("GRO_SUBTER_HAB") == fo.techStatus.complete)  or "TUNNELS_SPECIAL" in planetSpecials:    
            if "TECTONIC_INSTABILITY_SPECIAL" not in planetSpecials:
                popSizeMod += popSizeModMap["subHab"][planetEnv]
        if empire.getTechStatus("GRO_SYMBIOTIC_BIO") == fo.techStatus.complete:
            popSizeMod += popSizeModMap["symBio"][planetEnv]
        if empire.getTechStatus("GRO_XENO_GENETICS") == fo.techStatus.complete:
            popSizeMod += popSizeModMap["xenoGen"][planetEnv]
        if empire.getTechStatus("GRO_XENO_HYBRID") == fo.techStatus.complete:
            popSizeMod += popSizeModMap["xenoHyb"][planetEnv]
        if empire.getTechStatus("GRO_CYBORG") == fo.techStatus.complete:
            popSizeMod += popSizeModMap["cyborg"][planetEnv]
        if empire.getTechStatus("CON_NDIM_STRUC") == fo.techStatus.complete:
            popSizeMod += popSizeModMap["ndim"][planetEnv]
        if empire.getTechStatus("CON_ORBITAL_HAB") == fo.techStatus.complete:
            popSizeMod += popSizeModMap["orbit"][planetEnv]

        if "GAIA_SPECIAL" in planet.specials:
            popSizeMod += 3

        for special in [ "SLOW_ROTATION_SPECIAL",  "SOLID_CORE_SPECIAL"] :
            if special in planetSpecials:
                popSizeMod -= 1

        """
        for special, tag in [ ("PROBIOTIC_SPECIAL",  "ORGANIC"),
                                                           ("FRUIT_SPECIAL",  "ORGANIC"),
                                                           ("SPICE_SPECIAL",  "ORGANIC"),
                                                           ("MONOPOLE_SPECIAL",  "ROBOTIC"),
                                                           ("SUPERCONDUCTOR_SPECIAL",  "ROBOTIC"),
                                                           ("POSITRONIUM_SPECIAL",  "ROBOTIC"),
                                                           ("MINERALS_SPECIAL",  "LITHIC"),
                                                           ("METALOIDS_SPECIAL",  "LITHIC"),
                                                     ]:
                                                     """
        for special in [ spec for spec in AIDependencies.metabolimBoosts  if spec in planetSpecials]:
            valMod += 10  # extra bonus due to potential applicability to other planets
        for thisTag in [ tag for tag in tagList if tag in  AIDependencies.metabolims]:
            popSizeMod +=  len( (set(planetSpecials).union([key for key in activeGrowthSpecials.keys() if  len(activeGrowthSpecials[key])>0 ] )).intersection(AIDependencies.metabolimBoostMap.get(thisTag,  []) ) )
            
        popSize = planetSize * popSizeMod

        if "DIM_RIFT_MASTER_SPECIAL" in planet.specials:
            popSize -= 4

        for special in [ "MINERALS_SPECIAL",  "CRYSTALS_SPECIAL",  "METALOIDS_SPECIAL"] : 
            if special in planetSpecials:
                miningBonus+=1
        
        proSingVal = [0, 4][(len( AIstate.empireStars.get(fo.starType.blackHole,  [])) > 0)]
        basePopInd=0.2
        indTechMap={    "GRO_ENERGY_META":  0.5, 
                                            "PRO_ROBOTIC_PROD":0.4, 
                                            "PRO_FUSION_GEN":       1.0, 
                                            "PRO_INDUSTRY_CENTER_I": 1, 
                                            "PRO_INDUSTRY_CENTER_II":1, 
                                            "PRO_INDUSTRY_CENTER_III":1, 
                                            "PRO_SOL_ORB_GEN":  2.0,   #assumes will build a gen at a blue/white star
                                            "PRO_SOL_ORB_GEN": 5,   # half the max value
                                            "PRO_SINGULAR_GEN": proSingVal, 
                                            }
                                            
        indVal = 0
        if (empire.getTechStatus("PRO_SENTIENT_AUTOMATION") == fo.techStatus.complete):
            indVal += discountMultiplier * 5
        indVal += discountMultiplier * basePopInd * popSize*miningBonus
        for tech in indTechMap:
            if (empire.getTechStatus(tech) == fo.techStatus.complete):
                indVal += discountMultiplier * basePopInd * popSize * indTechMap[tech]
        # used to give preference to closest worlds
        empireID = empire.empireID
        capitalID = PlanetUtilsAI.getCapital()
        homeworld = universe.getPlanet(capitalID)
        if homeworld:
            homeSystemID = homeworld.systemID
            evalSystemID = planet.systemID
            leastJumpsPath = len(universe.leastJumpsPath(homeSystemID, evalSystemID, empireID))
            distanceFactor = 1.001 / (leastJumpsPath + 1)
        else:
            distanceFactor = 0

        retval=0.0
        if popSize<0:
            if foAI.foAIstate.aggression > fo.aggression.typical and (fo.currentTurn() >= 10):
                retval  = starBonus+asteroidBonus+gasGiantBonus
        elif popSize==0:
            if foAI.foAIstate.aggression > fo.aggression.typical and (fo.currentTurn() >= 10):
                retval  = 0.5*( starBonus+asteroidBonus+gasGiantBonus) #actually is kind of a pain if the pop goes close to zero but not actually zero
        else:
            retval  = starBonus+indVal+asteroidBonus+gasGiantBonus + valMod
            if planet.systemID in annexableRing1:
                retval += 10
            elif planet.systemID in annexableRing2:
                retval += 20
            elif planet.systemID in annexableRing3:
                retval += 10
        
        thrtRatio = (foAI.foAIstate.systemStatus.get(planet.systemID,  {}).get('fleetThreat', 0)+0.2*foAI.foAIstate.systemStatus.get(planet.systemID,  {}).get('neighborThreat', 0)) / float(curBestMilShipRating)
        if thrtRatio > 4:
            retval = 0.3*retval 
        elif thrtRatio >= 2:
            retval = 0.7* retval
        elif thrtRatio > 0:
            retval = 0.85* retval

    return retval

def getPlanetHospitality(planetID, species):
    "returns a value depending on the planet type"

    universe = fo.getUniverse()

    planet = universe.getPlanet(planetID)
    if planet == None: return 0

    planetEnvironment = species.getPlanetEnvironment(planet.type)
    # print ":: planet:" + str(planetID) + " type:" + str(planet.type) + " size:" + str(planet.size) + " env:" + str(planetEnvironment)

    # reworked with races
    if planetEnvironment == fo.planetEnvironment.good: return 2.75
    if planetEnvironment == fo.planetEnvironment.adequate: return 1
    if planetEnvironment == fo.planetEnvironment.poor: return 0.5
    if planetEnvironment == fo.planetEnvironment.hostile: return 0.25
    if planetEnvironment == fo.planetEnvironment.uninhabitable: return 0.1

    return 0

def removeLowValuePlanets(evaluatedPlanets):
    "removes all planets with a colonisation value < minimalColoniseValue"

    removeIDs = []
    minVal = AIstate.minimalColoniseValue
    if foAI.foAIstate.aggression <fo.aggression.typical:
        minVal *= 3

    # print ":: min:" + str(AIstate.minimalColoniseValue)
    for planetID in evaluatedPlanets.iterkeys():
        #print ":: eval:" + str(planetID) + " val:" + str(evaluatedPlanets[planetID])
        if (evaluatedPlanets[planetID][0] < minVal):
            removeIDs.append(planetID)
    #print "removing ",  removeIDs
    for ID in removeIDs: del evaluatedPlanets[ID]

def sendColonyShips(colonyFleetIDs, evaluatedPlanets, missionType):
    "sends a list of colony ships to a list of planet_value_pairs"
    fleetPool = colonyFleetIDs[:]
    potentialTargets =   [  (pid, (score, specName)  )  for (pid,  (score, specName) ) in  evaluatedPlanets if score > 50 ]

    print "colony/outpost  ship matching -- fleets  %s to planets %s"%( fleetPool,  evaluatedPlanets)
    #for planetID_value_pair in evaluatedPlanets:
    fleetPool=set(fleetPool)
    universe=fo.getUniverse()
    while (len(fleetPool) > 0 ) and ( len(potentialTargets) >0):
        thisTarget = potentialTargets.pop(0)
        thisPlanetID=thisTarget[0]
        thisSysID = universe.getPlanet(thisPlanetID).systemID
        if (foAI.foAIstate.systemStatus.setdefault(thisSysID, {}).setdefault('monsterThreat', 0) > 2000) or (fo.currentTurn() <20  and foAI.foAIstate.systemStatus[thisSysID]['monsterThreat'] > 200):
            print "Skipping colonization of system %s due to Big Monster,  threat %d"%(PlanetUtilsAI.sysNameIDs([thisSysID]),  foAI.foAIstate.systemStatus[thisSysID]['monsterThreat'])
            continue
        thisSpec=thisTarget[1][1]
        foundFleets=[]
        thisFleetList = FleetUtilsAI.getFleetsForMission(nships=1,  targetStats={},  minStats={},  curStats={},  species=thisSpec,  systemsToCheck=[thisSysID],  systemsChecked=[], 
                                                     fleetPoolSet = fleetPool,   fleetList=foundFleets,  verbose=False)
        if thisFleetList==[]:
            fleetPool.update(foundFleets)#just to be safe
            continue #must have no compatible colony/outpost ships 
        fleetID = thisFleetList[0]

        aiTarget = AITarget.AITarget(AITargetType.TARGET_PLANET, thisPlanetID)
        aiFleetMission = foAI.foAIstate.getAIFleetMission(fleetID)
        aiFleetMission.addAITarget(missionType, aiTarget)
