# AI can not currently add new building types to production queue
# AI can not currently design or build custom ships
# individual techs are to be removed from unusable list as AI programming progresses

outpostTechName =         "CON_ENV_ENCAPSUL"
exobotTechName =         "PRO_EXOBOTS"
gasOrbitalTechName = "PRO_ORBITAL_GEN"
solarOrbitalTechName = "PRO_SOL_ORB_GEN"

SupplyTechs=[ "CON_ORBITAL_CON" ,   "CON_CONTGRAV_ARCH",  "CON_GAL_INFRA",  "CON_ARCH_MONOFILS"]

cCamps=["CON_METRO_INFRA",   "CON_ARCH_PSYCH",  "CON_CONC_CAMP"]

def unusableTechsList():
    "techs currently unusable by the AI"

    unusableTechs = []
    ptechs = primaryMetaTechsList()
    for tech in [    

        'CON_ART_HEAVENLY',
        'CON_ART_PLANET',
        'CON_CONC_CAMP',
        'CON_FRC_ENRG_CAMO',
        'CON_INFRA_ECOL',
        'CON_ORGANIC_STRC',
        'CON_PLANET_DRIVE',
        'CON_SPACE_ELEVATOR',
        'CON_STARGATE',
        'CON_TRANS_ARCH',
        'SPY_LIGHTHOUSE',
        'GRO_BIOTERROR',
        'GRO_GENOME_BANK',
        'GRO_GAIA_TRANS',
        'GRO_INDUSTRY_CLONE',
        'GRO_INDUSTRY_FARM',
        'GRO_REMOTE_TERRAFORM',
        'GRO_SUSPENDED_ANIMATION',
        'GRO_TERRAFORM',
        'LRN_ART_BLACK_HOLE',
        'LRN_ENCLAVE_VOID',
        'LRN_GATEWAY_VOID',
        'LRN_MIND_VOID',
        'LRN_OBSERVATORY_I',
        'LRN_PSY_DOM',
        'LRN_SOLAR_MAN',
        'LRN_SPATIAL_DISTORT_GEN',
        'LRN_TIME_MECH',
        'LRN_TRANSCEND',
        'LRN_UNIF_CONC',
        'LRN_XENOARCH',
        'PRO_ENVIRO_MINING',
        'PRO_HEAVY_MINING_I',
        'PRO_INDUSTRY_CENTER_I',
        'PRO_INDUSTRY_CENTER_II',
        'PRO_INDUSTRY_CENTER_III',
        'PRO_NEUTRONIUM_EXTRACTION',
        'PRO_NDIM_ASSMB',
        'PRO_ORBITAL_GEN',
        'PRO_ORBITAL_MINE',
        'PRO_SINGULAR_GEN',
        'PRO_SOL_ORB_GEN',
        'PRO_ZERO_GEN',
        'SHP_ANTIMAT_TORP',
        'SHP_ANTIMATTER_TANK',
        'SHP_ASTEROID_HULLS',
        'SHP_ASTEROID_REFORM',
        'SHP_BIOADAPT_HULL',
        'SHP_BIOBOMBER',
        'SHP_BIOINTERCEPTOR',
        'SHP_BIONEUR_SPEC',
        'SHP_BOMBER',
        'SHP_CAMO_AST_HULL',
        'SHP_CONT_BIOADAPT',
        'SHP_CONT_SYMB',
        'SHP_CONTGRAV_MAINT',
        'SHP_DEATH_RAY',
        'SHP_DEUTERIUM_TANK',
        'SPY_DIST_MOD',
        'SHP_DOMESTIC_MONSTER',
        'SHP_ENDOMORPH_HULL',
        'SHP_ENDOSYMB_HULL',
        'SHP_ENRG_BOUND_MAN',
        'SHP_FRC_ENRG_COMP',
        'SHP_HAB_MEGAFAUN',
        'SHP_INTERCEPTOR',
        'SHP_LEAD_PLATE',
        'SHP_LOGISTICS_FAC',
        'SHP_MASS_DRIVER',
        'SHP_MASSPROP_SPEC',
        'SHP_MIDCOMB_LOG',
        'SHP_MIL_ROBO_CONT',
        'SHP_MINIAST_SWARM',
        'SHP_MONOCELL_EXP',
        'SHP_MONOMOLEC_LATTICE',
        'SHP_MULTICELL_CAST',
        'SHP_MULTISPEC_SHIELD',
        'SHP_NANOROBO_HULL',
        'SHP_NANOROBO_MAINT',
        'SPY_DETECT_3',
        'SHP_NEUTRONIUM_PLATE_NUC_MIS',
        'SHP_NEUTRONIUM_PLATE_SPEC_MIS',
        'SHP_NUCLEAR_MISSILE',
        'SHP_ORG_HULL',
        'SHP_PHASOR',
        'DEF_PLANET_CLOAK',
        'SHP_PLASMA_TORP',
        'SHP_PULSE_LASER',
        'SHP_QUANT_ENRG_MAG',
        'SPY_DETECT_2',
        'SHP_RECON_FIGHT',
        'SHP_SCAT_AST_HULL',
        'SPY_DETECT_4',
        'SHP_SENT_HULL',
        'SHP_SOLAR_CONT',
        'SHP_SPACE_FLUX_DRIVE',
        'SHP_SPECTRAL_MISSILE',
        'SHP_TRANSSPACE_DRIVE',
        'SHP_ZORTRIUM_PLATE',
        'SHP_WEAPON_11',
        'SHP_WEAPON_12',
        'SHP_WEAPON_13',
        'SHP_WEAPON_14',
        'SHP_WEAPON_15',
        'SHP_WEAPON_16',
        'SHP_WEAPON_17'
    ] :
        if tech not in ptechs:
            unusableTechs.append(tech)

    return unusableTechs

def primaryLearningTechsList():
    "primary learning techs"

    primaryLearningTechs = [

        'LRN_ALGO_ELEGANCE',
        'LRN_ARTIF_MINDS']

    return primaryLearningTechs

def primaryGroTechsList():
    "primary growth techs"

    primaryGroTechs = [

        'CON_ENV_ENCAPSUL']
 
    return primaryGroTechs

def primaryShipsTechsList():
    "primary ships techs"

    primaryShipsTechs = [
        'SHP_WEAPON_2',
        'SHP_WEAPON_3',
        'SHP_WEAPON_4',
        'SHP_WEAPON_5',
        'SHP_DEFLECTOR',
        'SHP_WEAPON_6',
        'SHP_WEAPON_7',
        'SHP_WEAPON_8',
        'SHP_WEAPON_9',
        'SHP_WEAPON_10'
        ]

    return primaryShipsTechs

def getPrimaryTechsList1():
    techsList1 = [
        "SHP_WEAPON_2",
        "GRO_PLANET_ECOL",
        "GRO_SUBTER_HAB", 
        "DEF_GARRISON_1", 
        "SHP_HAB_MEGAFAUN", 
        "LRN_ALGO_ELEGANCE", 
        "CON_ENV_ENCAPSUL", 
        "SHP_WEAPON_4",
        "GRO_GENETIC_ENG", 
        "DEF_DEFENSE_NET", 
        "GRO_GENETIC_ENG", 
        "SHP_MULTICELL_CAST", 
        "DEF_GARRISON_2", 
        "SHP_WEAPON_5",
        "DEF_GARRISON_3", 
        "SHP_WEAPON_6",
        "PRO_ROBOTIC_PROD", 
        "PRO_FUSION_GEN", 
        "PRO_INDUSTRY_CENTER_I", 
        "GRO_SYMBIOTIC_BIO", 
    ]
    return techsList1

def getPrimaryTechsList2():
    techsList2 = [
        "SHP_WEAPON_2",
        "GRO_PLANET_ECOL",
        "GRO_SYMBIOTIC_BIO", 
        "GRO_SUBTER_HAB", 
        "DEF_GARRISON_1", 
        "SHP_HAB_MEGAFAUN", 
        "LRN_ALGO_ELEGANCE", 
        "CON_ENV_ENCAPSUL", 
        "SHP_WEAPON_4",
        "GRO_GENETIC_ENG", 
        "DEF_DEFENSE_NET", 
        "GRO_GENETIC_ENG", 
        "SHP_MULTICELL_CAST", 
        "DEF_GARRISON_2", 
        "SHP_WEAPON_5",
        "DEF_GARRISON_3", 
        "SHP_WEAPON_6",
        "PRO_ROBOTIC_PROD", 
        "PRO_FUSION_GEN", 
        "PRO_INDUSTRY_CENTER_I", 
    ]
    return techsList2
    
def defenseTechs1():
    defTechs1=[
        "DEF_DEFENSE_NET_1", 
        "DEF_GARRISON_1", 
        ]
    return defTechs1

def defenseTechs2():
    defTechs2=[
        ]
    return defTechs2

def primaryMetaTechsList(index=0):
    "primary techs for all categories"
    #index=1-index
    index=0
    if index == 0:
        primaryMetaTechs = [
            "SHP_WEAPON_2",
            "GRO_PLANET_ECOL",
            "GRO_SUBTER_HAB", 
            "LRN_ALGO_ELEGANCE", 
            "SHP_DOMESTIC_MONSTER",
            "SHP_ORG_HULL", 
            "SHP_WEAPON_5",
            "LRN_ARTIF_MINDS", 
            "GRO_SYMBIOTIC_BIO", 
            "CON_ENV_ENCAPSUL", 
            "LRN_FORCE_FIELD",
            "DEF_DEFENSE_NET_1", 
            "SHP_WEAPON_6",
            "DEF_GARRISON_1", 
            "PRO_ROBOTIC_PROD", 
            "PRO_FUSION_GEN", 
            "PRO_ORBITAL_GEN",
            "CON_ORBITAL_CON", 
            "SPY_DETECT_2",
            "DEF_DEFENSE_NET_2",
            "LRN_GRAVITONICS",
            "SHP_WEAPON_7",
            "CON_METRO_INFRA", 
            "GRO_GENETIC_ENG", 
            "CON_SUBTER_CONST", 
            "SHP_MULTICELL_CAST", 
            "SHP_ENDOCRINE_SYSTEMS", 
            "SHP_WEAPON_8",
            "DEF_PLAN_BARRIER_SHLD_1",
            "PRO_INDUSTRY_CENTER_I", 
            "DEF_DEFENSE_NET_REGEN_1",
            "DEF_PLAN_BARRIER_SHLD_2",
            "PRO_SOL_ORB_GEN",
            "CON_CONTGRAV_ARCH", 
            "GRO_MEDICAL_PATH", 
            "LRN_PHYS_BRAIN", 
            "LRN_TRANSLING_THT", 
            "PRO_SENTIENT_AUTOMATION", 
            "PRO_EXOBOTS",
            "DEF_DEFENSE_NET_3",
            "DEF_SYST_DEF_MINE_1",
            "DEF_PLAN_BARRIER_SHLD_3",
            "CON_ORBITAL_HAB",
            "DEF_GARRISON_2", 
            "GRO_LIFECYCLE_MAN", 
            "DEF_GARRISON_3", 
            "SHP_WEAPON_8",
            "PRO_INDUSTRY_CENTER_II", 
            "SPY_DETECT_3",
            "SHP_CONT_SYMB",
            "SHP_MONOCELL_EXP",
            "SHP_ENDOSYMB_HULL",
            "GRO_XENO_GENETICS", 
            "PRO_SINGULAR_GEN",
            "LRN_XENOARCH",
            "SHP_WEAPON_9",
            "PRO_MICROGRAV_MAN",
            "GRO_XENO_HYBRIDS", 
            "LRN_QUANT_NET",
            "LRN_TIME_MECH",
            "LRN_STELLAR_TOMOGRAPHY",
            "PRO_NEUTRONIUM_EXTRACTION",
            "LRN_ENCLAVE_VOID",
            "LRN_PSY_DOM",
            "LRN_ART_BLACK_HOLE",
            "SHP_ASTEROID_HULLS",
            "GRO_TERRAFORM",
            "GRO_GENETIC_MED",
            "SHP_LEAD_PLATE", 
            "SHP_BIOADAPTIVE_SPEC",
            "SHP_INTSTEL_LOG",
            "SHP_REINFORCED_HULL",
            "SHP_WEAPON_10", 
            "SHP_CONT_BIOADAPT",
            "SHP_SENT_HULL",
            "SHP_ZORTRIUM_PLATE",
            "DEF_SYST_DEF_MINE_2",
            "GRO_CYBORG", 
            "GRO_GENOME_BANK",
            "SHP_BASIC_DAM_CONT",
            "CON_CONC_CAMP",
            "SHP_ADV_DAM_CONT",
            "PRO_INDUSTRY_CENTER_III", 
            "DEF_GARRISON_4",
            "SPY_STEALTH_1", 
            "DEF_DEFENSE_NET_REGEN_2",
            "SHP_ENRG_BOUND_MAN", 
            "GRO_GAIA_TRANS",
            "GRO_ENERGY_META",
            "CON_ART_PLANET",
            "LRN_DISTRIB_THOUGHT", 
            "GRO_ENERGY_META", 
            "DEF_SYST_DEF_MINE_3",
            "DEF_PLAN_BARRIER_SHLD_4",
            "SHP_WEAPON_13",
            "SPY_DETECT_4",
            "SHP_WEAPON_11", 
            "SHP_WEAPON_12",
            "DEF_PLAN_BARRIER_SHLD_5",
            "SHP_SOLAR_CONT",
            "SHP_WEAPON_14",
            "SHP_WEAPON_15",
            "SHP_WEAPON_16",
            "SHP_WEAPON_17",
            ]
    elif index == 1:
        primaryMetaTechs = [
            "SHP_WEAPON_2",
            "GRO_PLANET_ECOL",
            "GRO_SUBTER_HAB", 
            "SHP_DOMESTIC_MONSTER",
            "SHP_ORG_HULL", 
            "LRN_ALGO_ELEGANCE", 
            "CON_ENV_ENCAPSUL", 
            "SHP_WEAPON_5",
            "LRN_ARTIF_MINDS", 
            "DEF_DEFENSE_NET_1", 
            "DEF_GARRISON_1", 
            "GRO_SYMBIOTIC_BIO", 
            "PRO_FUSION_GEN", 
            "LRN_FORCE_FIELD",
            "PRO_ROBOTIC_PROD", 
            "SHP_WEAPON_6",
            "PRO_ORBITAL_GEN",
            "DEF_DEFENSE_NET_2",
            "DEF_PLAN_BARRIER_SHLD_1",
            "SPY_DETECT_2",
            "GRO_GENETIC_ENG", 
            "SHP_MULTICELL_CAST", 
            "CON_ORBITAL_CON", 
            "SHP_WEAPON_7",
            "CON_METRO_INFRA", 
            "CON_SUBTER_CONST", 
            "SHP_WEAPON_8",
            "SHP_ENDOCRINE_SYSTEMS", 
            "PRO_INDUSTRY_CENTER_I", 
            "DEF_DEFENSE_NET_REGEN_1",
            "DEF_PLAN_BARRIER_SHLD_2",
            "PRO_SOL_ORB_GEN",
            "LRN_GRAVITONICS",
            "CON_CONTGRAV_ARCH", 
            "LRN_PHYS_BRAIN", 
            "LRN_TRANSLING_THT", 
            "PRO_SENTIENT_AUTOMATION", 
            "PRO_EXOBOTS",
            "DEF_DEFENSE_NET_3",
            "DEF_SYST_DEF_MINE_1",
            "DEF_PLAN_BARRIER_SHLD_3",
            "CON_ORBITAL_HAB",
            "GRO_XENO_HYBRIDS", 
            "DEF_GARRISON_2", 
            "GRO_LIFECYCLE_MAN", 
            "DEF_GARRISON_3", 
            "SHP_WEAPON_8",
            "PRO_INDUSTRY_CENTER_II", 
            "GRO_XENO_GENETICS", 
            "SHP_CONT_SYMB",
            "SHP_MONOCELL_EXP",
            "SHP_ENDOSYMB_HULL",
            "PRO_SINGULAR_GEN",
            "SPY_DETECT_3",
            "LRN_XENOARCH",
            "SHP_WEAPON_9",
            "PRO_MICROGRAV_MAN",
            "LRN_QUANT_NET",
            "LRN_TIME_MECH",
            "LRN_STELLAR_TOMOGRAPHY",
            "PRO_NEUTRONIUM_EXTRACTION",
            "LRN_ENCLAVE_VOID",
            "LRN_PSY_DOM",
            "LRN_ART_BLACK_HOLE",
            "SHP_ASTEROID_HULLS",
            "GRO_TERRAFORM",
            "GRO_GENETIC_MED",
            "SHP_LEAD_PLATE", 
            "SHP_BIOADAPTIVE_SPEC",
            "SHP_INTSTEL_LOG",
            "SHP_REINFORCED_HULL",
            "SHP_WEAPON_10", 
            "SHP_CONT_BIOADAPT",
            "SHP_SENT_HULL",
            "SHP_ZORTRIUM_PLATE",
            "DEF_SYST_DEF_MINE_2",
            "GRO_CYBORG", 
            "GRO_GENOME_BANK",
            "SHP_BASIC_DAM_CONT",
            "CON_CONC_CAMP",
            "SHP_ADV_DAM_CONT",
            "PRO_INDUSTRY_CENTER_III", 
            "DEF_GARRISON_4",
            "SPY_STEALTH_1", 
            "DEF_DEFENSE_NET_REGEN_2",
            "SHP_ENRG_BOUND_MAN", 
            "GRO_GAIA_TRANS",
            "GRO_ENERGY_META",
            "CON_ART_PLANET",
            "LRN_DISTRIB_THOUGHT", 
            "GRO_ENERGY_META", 
            "DEF_SYST_DEF_MINE_3",
            "DEF_PLAN_BARRIER_SHLD_4",
            "SHP_WEAPON_13",
            "SPY_DETECT_4",
            "SHP_WEAPON_11", 
            "SHP_WEAPON_12",
            "DEF_PLAN_BARRIER_SHLD_5",
            "SHP_SOLAR_CONT",
            "SHP_WEAPON_14",
            "SHP_WEAPON_15",
            "SHP_WEAPON_16",
            "SHP_WEAPON_17",
            ]
            #"CON_FRC_ENRG_STRC",
            #"CON_FRC_ENRG_CAMO",
            #"SHP_TITAN_HULL"
        
    return primaryMetaTechs
    
MasterTechList=[  # just for reference
        "CON_ARCH_MONOFILS",
        "CON_ARCH_PSYCH",
        "CON_ART_HEAVENLY",
        "CON_ART_PLANET",
        "CON_CONC_CAMP",
        "CON_CONTGRAV_ARCH",
        "CON_ENV_ENCAPSUL",
        "CON_FRC_ENRG_CAMO",
        "CON_FRC_ENRG_STRC",
        "CON_GAL_INFRA",
        "CON_INFRA_ECOL",
        "CON_METRO_INFRA",
        "CON_NDIM_STRC",
        "CON_ORBITAL_CON",
        "CON_ORBITAL_HAB",
        "CON_ORGANIC_STRC",
        "CON_PLANET_DRIVE",
        "CON_STARGATE",
        "CON_SUBTER_CONST",
        "CON_TRANS_ARCH",
        "DEF_DEFENSE_NET_1",
        "DEF_DEFENSE_NET_2",
        "DEF_DEFENSE_NET_3",
        "DEF_DEFENSE_NET_REGEN_1",
        "DEF_DEFENSE_NET_REGEN_2",
        "DEF_GARRISON_1",
        "DEF_GARRISON_2",
        "DEF_GARRISON_3",
        "DEF_GARRISON_4",
        "DEF_PLANET_CLOAK",
        "DEF_PLAN_BARRIER_SHLD_1",
        "DEF_PLAN_BARRIER_SHLD_2",
        "DEF_PLAN_BARRIER_SHLD_3",
        "DEF_PLAN_BARRIER_SHLD_4",
        "DEF_PLAN_BARRIER_SHLD_5",
        "DEF_ROOT_DEFENSE",
        "DEF_SYST_DEF_MINE_1",
        "DEF_SYST_DEF_MINE_2",
        "DEF_SYST_DEF_MINE_3",
        "GRO_ADV_ECOMAN",
        "GRO_BIOTERROR",
        "GRO_CYBORG",
        "GRO_ECO_METAMORPH",
        "GRO_ENERGY_META",
        "GRO_GAIA_TRANS",
        "GRO_GENETIC_ENG",
        "GRO_GENETIC_MED",
        "GRO_GENOME_BANK",
        "GRO_LIFECYCLE_MAN",
        "GRO_MEDICAL_PATH",
        "GRO_NANOTECH_MED",
        "GRO_NANO_CYBERNET",
        "GRO_PLANET_ECOL",
        "GRO_SUBTER_HAB",
        "GRO_SYMBIOTIC_BIO",
        "GRO_TERRAFORM",
        "GRO_TRANSORG_SENT",
        "GRO_XENO_GENETICS",
        "GRO_XENO_HYBRIDS",
        "LRN_ALGO_ELEGANCE",
        "LRN_ARTIF_MINDS",
        "LRN_ART_BLACK_HOLE",
        "LRN_DISTRIB_THOUGHT",
        "LRN_ENCLAVE_VOID",
        "LRN_EVERYTHING",
        "LRN_FORCE_FIELD",
        "LRN_GATEWAY_VOID",
        "LRN_GRAVITONICS",
        "LRN_MIND_VOID",
        "LRN_NDIM_SUBSPACE",
        "LRN_OBSERVATORY_I",
        "LRN_PHYS_BRAIN",
        "LRN_PSIONICS",
        "LRN_PSY_DOM",
        "LRN_QUANT_NET",
        "LRN_SPATIAL_DISTORT_GEN",
        "LRN_STELLAR_TOMOGRAPHY",
        "LRN_TIME_MECH",
        "LRN_TRANSCEND",
        "LRN_TRANSLING_THT",
        "LRN_UNIF_CONC",
        "LRN_XENOARCH",
        "PRO_CORE_MINE",
        "PRO_EXOBOTS",
        "PRO_FUSION_GEN",
        "PRO_INDUSTRY_CENTER_I",
        "PRO_INDUSTRY_CENTER_II",
        "PRO_INDUSTRY_CENTER_III",
        "PRO_MICROGRAV_MAN",
        "PRO_NANOTECH_PROD",
        "PRO_NDIM_ASSMB",
        "PRO_NEUTRONIUM_EXTRACTION",
        "PRO_ORBITAL_GEN",
        "PRO_ROBOTIC_PROD",
        "PRO_SENTIENT_AUTOMATION",
        "PRO_SINGULAR_GEN",
        "PRO_SOL_ORB_GEN",
        "PRO_ZERO_GEN",
        "SHP_ADV_DAM_CONT",
        "SHP_ANTIMATTER_TANK",
        "SHP_ASTEROID_HULLS",
        "SHP_ASTEROID_REFORM",
        "SHP_BASIC_DAM_CONT",
        "SHP_BIOADAPTIVE_SPEC",
        "SHP_BIOTERM",
        "SHP_CAMO_AST_HULL",
        "SHP_CONTGRAV_MAINT",
        "SHP_CONT_BIOADAPT",
        "SHP_CONT_SYMB",
        "SHP_DEATH_SPORE",
        "SHP_DEUTERIUM_TANK",
        "SHP_DIAMOND_PLATE",
        "SHP_DOMESTIC_MONSTER",
        "SHP_ENDOCRINE_SYSTEMS",
        "SHP_ENDOSYMB_HULL",
        "SHP_ENRG_BOUND_MAN",
        "SHP_FLEET_REPAIR",
        "SHP_FRC_ENRG_COMP",
        "SHP_GAL_EXPLO",
        "SHP_IMPROVED_ENGINE_COUPLINGS",
        "SHP_INTSTEL_LOG",
        "SHP_LEAD_PLATE",
        "SHP_MASSPROP_SPEC",
        "SHP_MIDCOMB_LOG",
        "SHP_MIL_ROBO_CONT",
        "SHP_MINIAST_SWARM",
        "SHP_MONOCELL_EXP",
        "SHP_MONOMOLEC_LATTICE",
        "SHP_MULTICELL_CAST",
        "SHP_MULTISPEC_SHIELD",
        "SHP_NANOROBO_MAINT",
        "SHP_NOVA_BOMB",
        "SHP_N_DIMENSIONAL_ENGINE_MATRIX",
        "SHP_ORG_HULL",
        "SHP_QUANT_ENRG_MAG",
        "SHP_REINFORCED_HULL",
        "SHP_ROOT_AGGRESSION",
        "SHP_SCAT_AST_HULL",
        "SHP_SENT_HULL",
        "SHP_SINGULARITY_ENGINE_CORE",
        "SHP_SOLAR_CONT",
        "SHP_SPACE_FLUX_DRIVE",
        "SHP_TRANSSPACE_DRIVE",
        "SHP_WEAPON_10",
        "SHP_WEAPON_11",
        "SHP_WEAPON_12",
        "SHP_WEAPON_13",
        "SHP_WEAPON_14",
        "SHP_WEAPON_15",
        "SHP_WEAPON_16",
        "SHP_WEAPON_17",
        "SHP_WEAPON_2",
        "SHP_WEAPON_3",
        "SHP_WEAPON_4",
        "SHP_WEAPON_5",
        "SHP_WEAPON_6",
        "SHP_WEAPON_7",
        "SHP_WEAPON_8",
        "SHP_WEAPON_9",
        "SHP_ZORTRIUM_PLATE",
        "SPY_DETECT_1",
        "SPY_DETECT_2",
        "SPY_DETECT_3",
        "SPY_DETECT_4",
        "SPY_DETECT_5",
        "SPY_DIST_MOD",
        "SPY_LIGHTHOUSE",
        "SPY_PLANET_STEALTH_MOD",
        "SPY_ROOT_DECEPTION",
        "SPY_STEALTH_1",
        "SPY_STEALTH_2",
        "SPY_STEALTH_3",
        "SPY_STEALTH_4",
        ]
