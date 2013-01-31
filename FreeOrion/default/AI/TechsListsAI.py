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
    if index == 0:
        primaryMetaTechs = [
            "GRO_PLANET_ECOL",
            "GRO_SUBTER_HAB", 
            "SHP_DOMESTIC_MONSTER",
            "SHP_ORG_HULL", 
            "SHP_WEAPON_3",
            "LRN_ALGO_ELEGANCE", 
            "CON_ENV_ENCAPSUL", 
            "PRO_FUSION_GEN", 
            "SHP_WEAPON_5",
            "PRO_ROBOTIC_PROD", 
            "DEF_DEFENSE_NET_1", 
            "DEF_GARRISON_1", 
            "CON_ORBITAL_CON", 
            "SHP_WEAPON_6",
            "GRO_SYMBIOTIC_BIO", 
            "LRN_ARTIF_MINDS", 
            "LRN_FORCE_FIELD",
            "GRO_GENETIC_ENG", 
            "SHP_MULTICELL_CAST", 
            "PRO_ORBITAL_GEN",
            "SHP_WEAPON_7",
            "PRO_MICROGRAV_MAN",
            "SHP_WEAPON_8",
            "PRO_INDUSTRY_CENTER_I", 
            "DEF_GARRISON_2", 
            #"GRO_LIFECYCLE_MAN", # ship part  is too expensive for now
            "SHP_ENDOCRINE_SYSTEMS", 
            "LRN_GRAVITONICS",
            "CON_CONTGRAV_ARCH", 
            "DEF_GARRISON_3", 
            "SHP_WEAPON_8",
            "PRO_INDUSTRY_CENTER_II", 
            "GRO_XENO_GENETICS", 
            "SPY_DETECT_2",
            "SHP_CONT_SYMB",
            "DEF_DEFENSE_NET_2",
            "DEF_PLAN_BARRIER_SHLD_1",
            "CON_METRO_INFRA", 
            "CON_SUBTER_CONST", 
            "SHP_MONOCELL_EXP",
            "SHP_ENDOSYMB_HULL",
            "PRO_SOL_ORB_GEN",
            "DEF_DEFENSE_NET_REGEN_1",
            "DEF_PLAN_BARRIER_SHLD_2",
            "SHP_WEAPON_9",
            "CON_ORBITAL_HAB",
            "GRO_XENO_HYBRIDS", 
            "SHP_ASTEROID_HULLS",
            "GRO_TERRAFORM",
            "LRN_PHYS_BRAIN", 
            "LRN_TRANSLING_THT", 
            "LRN_XENOARCH",
            "GRO_GENETIC_MED",
            "SHP_LEAD_PLATE", 
            "SHP_BIOADAPTIVE_SPEC",
            "PRO_SENTIENT_AUTOMATION", 
            "PRO_EXOBOTS",
            "DEF_DEFENSE_NET_3",
            "DEF_SYST_DEF_MINE_1",
            "DEF_PLAN_BARRIER_SHLD_3",
            "SPY_DETECT_3",
            "LRN_QUANT_NET",
            "SHP_INTSTEL_LOG",
            "SHP_REINFORCED_HULL",
            "SHP_WEAPON_10", 
            "SHP_CONT_BIOADAPT",
            "SHP_SENT_HULL",
            "SHP_ZORTRIUM_PLATE",
            "DEF_SYST_DEF_MINE_2",
            "GRO_CYBORG", 
            #"CON_FRC_ENRG_STRC",
            #"CON_FRC_ENRG_CAMO",
            "GRO_GENOME_BANK",
            "SHP_BASIC_DAM_CONT",
            "CON_CONC_CAMP",
            "PRO_SINGULAR_GEN",
            "SHP_ADV_DAM_CONT",
            "PRO_INDUSTRY_CENTER_III", 
            "DEF_GARRISON_4",
            "SPY_CLOAK_1", 
            "DEF_DEFENSE_NET_REGEN_2",
            "GRO_GAIA_TRANS",
            "GRO_ENERGY_META",
            "LRN_TIME_MECH",
            "LRN_ART_BLACK_HOLE",
            "CON_ART_PLANET",
            "PRO_NEUTRONIUM_EXTRACTION",
            "LRN_STELLAR_TOMOGRAPHY",
            "LRN_ENCLAVE_VOID",
            "LRN_DISTRIB_THOUGHT", 
            "GRO_ENERGY_META", 
            "DEF_SYST_DEF_MINE_3",
            "DEF_PLAN_BARRIER_SHLD_4",
            "SHP_WEAPON_13",
            "SPY_DETECT_4",
            "SHP_WEAPON_11", 
            "SHP_WEAPON_12",
            "SHP_TITAN_HULL"
            "DEF_PLAN_BARRIER_SHLD_5",
            "SHP_SOLAR_CONT",
            "SHP_WEAPON_14",
            "SHP_WEAPON_15",
            "SHP_WEAPON_16",
            "SHP_WEAPON_17",
            ]
    elif index == 1:
        primaryMetaTechs = [
            "GRO_PLANET_ECOL",
            "GRO_SUBTER_HAB", 
            "SHP_DOMESTIC_MONSTER",
            "SHP_ORG_HULL", 
            "SHP_WEAPON_3",
            "LRN_ALGO_ELEGANCE", 
            "CON_ENV_ENCAPSUL", 
            "PRO_FUSION_GEN", 
            "SHP_WEAPON_5",
            "PRO_ROBOTIC_PROD", 
            "DEF_DEFENSE_NET_1", 
            "DEF_GARRISON_1", 
            "CON_ORBITAL_CON", 
            "SHP_WEAPON_6",
            "GRO_SYMBIOTIC_BIO", 
            "LRN_ARTIF_MINDS", 
            "LRN_FORCE_FIELD",
            "DEF_DEFENSE_NET_2",
            "DEF_PLAN_BARRIER_SHLD_1",
            "SHP_WEAPON_7",
            "CON_METRO_INFRA", 
            "CON_SUBTER_CONST", 
            "SHP_WEAPON_8",
            "PRO_INDUSTRY_CENTER_I", 
            "DEF_DEFENSE_NET_REGEN_1",
            "DEF_PLAN_BARRIER_SHLD_2",
            "GRO_GENETIC_ENG", 
            "SPY_DETECT_2",
            "PRO_SENTIENT_AUTOMATION", 
            "PRO_EXOBOTS",
            "DEF_DEFENSE_NET_3",
            "DEF_SYST_DEF_MINE_1",
            "DEF_PLAN_BARRIER_SHLD_3",
            "CON_ORBITAL_HAB",
            "GRO_XENO_HYBRIDS", 
            "SHP_MULTICELL_CAST", 
            "PRO_ORBITAL_GEN",
            "PRO_MICROGRAV_MAN",
            "DEF_GARRISON_2", 
            #"GRO_LIFECYCLE_MAN", # ship part  is too expensive for now
            "SHP_ENDOCRINE_SYSTEMS", 
            "LRN_GRAVITONICS",
            "CON_CONTGRAV_ARCH", 
            "DEF_GARRISON_3", 
            "SHP_WEAPON_8",
            "PRO_INDUSTRY_CENTER_II", 
            "GRO_XENO_GENETICS", 
            "SHP_CONT_SYMB",
            "SHP_MONOCELL_EXP",
            "SHP_ENDOSYMB_HULL",
            "PRO_SOL_ORB_GEN",
            "SHP_WEAPON_9",
            "SHP_ASTEROID_HULLS",
            "GRO_TERRAFORM",
            "LRN_PHYS_BRAIN", 
            "LRN_TRANSLING_THT", 
            "LRN_XENOARCH",
            "GRO_GENETIC_MED",
            "SHP_LEAD_PLATE", 
            "SHP_BIOADAPTIVE_SPEC",
            "SPY_DETECT_3",
            "LRN_QUANT_NET",
            "SHP_INTSTEL_LOG",
            "SHP_REINFORCED_HULL",
            "SHP_WEAPON_10", 
            "SHP_CONT_BIOADAPT",
            "SHP_SENT_HULL",
            "SHP_ZORTRIUM_PLATE",
            "DEF_SYST_DEF_MINE_2",
            "GRO_CYBORG", 
            #"CON_FRC_ENRG_STRC",
            #"CON_FRC_ENRG_CAMO",
            "GRO_GENOME_BANK",
            "SHP_BASIC_DAM_CONT",
            "CON_CONC_CAMP",
            "PRO_SINGULAR_GEN",
            "SHP_ADV_DAM_CONT",
            "PRO_INDUSTRY_CENTER_III", 
            "DEF_GARRISON_4",
            "SPY_CLOAK_1", 
            "DEF_DEFENSE_NET_REGEN_2",
            "GRO_GAIA_TRANS",
            "GRO_ENERGY_META",
            "LRN_TIME_MECH",
            "LRN_ART_BLACK_HOLE",
            "CON_ART_PLANET",
            "PRO_NEUTRONIUM_EXTRACTION",
            "LRN_STELLAR_TOMOGRAPHY",
            "LRN_ENCLAVE_VOID",
            "LRN_DISTRIB_THOUGHT", 
            "GRO_ENERGY_META", 
            "DEF_SYST_DEF_MINE_3",
            "DEF_PLAN_BARRIER_SHLD_4",
            "SHP_WEAPON_13",
            "SPY_DETECT_4",
            "SHP_WEAPON_11", 
            "SHP_WEAPON_12",
            "SHP_TITAN_HULL"
            "DEF_PLAN_BARRIER_SHLD_5",
            "SHP_SOLAR_CONT",
            "SHP_WEAPON_14",
            "SHP_WEAPON_15",
            "SHP_WEAPON_16",
            "SHP_WEAPON_17",
            ]
        
    return primaryMetaTechs
    
MasterTechList=[  #currently with typos, just for reference
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
        "DEF_DEFENSE_NET",
        "DEF_GARRISON_1",
        "DEF_GARRISON_2",
        "DEF_GARRISON_3",
        "DEF_GARRISON_4",
        "DEF_HI_ENRG_FOCUS",
        "DEF_PLANET_CLOAK",
        "DEF_PLAN_BARRIER_SHLD",
        "DEF_ROOT_DEFENSE",
        "DEF_SYST_DEF_MINE",
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
        "LRN_RUINS_MOD",
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
        "PRO_NEUTRONIUM",
        "PRO_ORBITAL_GEN",
        "PRO_ROBOTIC_PROD",
        "PRO_SINGULAR_GEN",
        "PRO_SOL_ORB_GEN",
        "SHP_ADV_DAM_CONT",
        "SHP_ASTEROID_",
        "SHP_ASTEROID_R",
        "SHP_BASIC_DAM_CONT",
        "SHP_BIONEUR_S",
        "SHP_BIOTERM",
        "SHP_CONTGRAV_MA",
        "SHP_CONT_BIOADAP",
        "SHP_CONT_SYMB",
        "SHP_DEUTERIU",
        "SHP_DOMESTIC_MON",
        "SHP_ENDOSYMB_H",
        "SHP_ENRG_BOUND_",
        "SHP_FLEET_REPAIR",
        "SHP_FRC_ENRG_C",
        "SHP_GAL_EXPLO",
        "SHP_IMPROVED_E",
        "SHP_INTSTEL_LOG",
        "SHP_LEAD_PLATE", 
        "SHP_MASSPROP_SPE",
        "SHP_MIDCOMB_LOG",
        "SHP_MINIAST_SWAR",
        "SHP_MONOCELL_EXP",
        "SHP_MULTICELL_",
        "SHP_MULTISPEC",
        "SHP_NANORO",
        "SHP_NOVA_BOMB",
        "SHP_ORG_HULL",
        "SHP_REINFORCED",
        "SHP_ROOT_AGGRES",
        "SHP_SCAT_AST_HU",
        "SHP_SENT_HULL",
        "SHP_SOLAR_CONT",
        "SHP_TRANSSPACE_D",
        "SHP_WEAPON_10",
        "SHP_WEAPON_12",
        "SHP_WEAPON_14",
        "SHP_WEAPON_16",
        "SHP_WEAPON_2",
        "SHP_WEAPON_4",
        "SHP_WEAPON_6",
        "SHP_WEAPON_8",
        "SHP_ZORTRIUM_PLATE",
        "SPY_DETECT_1",
        "SPY_DETECT_3",
        "SPY_DETECT_5",
        "SPY_LIGHTHOUSE",
        "SPY_ROOT_DECEPTI",
        "SPY_STEALTH_1",
        "SPY_STEALTH_3"
]

def aggressiveTechs():
    "primary techs for all categories"

    aggressiveTechsList = [
        "GRO_PLANET_ECOL",
        "GRO_SUBTER_HAB", 
        "SHP_DOMESTIC_MONSTER",
        "SHP_ORG_HULL", 
        "SHP_WEAPON_3",
        "LRN_ALGO_ELEGANCE", 
        "GRO_SYMBIOTIC_BIO", 
        "CON_ENV_ENCAPSUL", 
        "PRO_FUSION_GEN", 
        "SHP_WEAPON_5",
        "PRO_ORBITAL_GEN",
        "PRO_ROBOTIC_PROD", 
        "DEF_DEFENSE_NET_1", 
        "DEF_GARRISON_1", 
        "SHP_WEAPON_6",
        "LRN_ARTIF_MINDS", 
        "LRN_FORCE_FIELD",
        "GRO_GENETIC_ENG", 
        "SHP_MULTICELL_CAST", 
        "CON_ORBITAL_CON", 
        "PRO_MICROGRAV_MAN",
        "PRO_INDUSTRY_CENTER_I", 
        "DEF_GARRISON_2", 
        "GRO_LIFECYCLE_MAN",
        "SHP_ENDOCRINE_SYSTEMS", 
        "GRO_XENO_GENETICS", 
        "LRN_GRAVITONICS",
        "CON_CONTGRAV_ARCH", 
        "DEF_GARRISON_3", 
        "PRO_INDUSTRY_CENTER_II", 
        "SPY_DETECT_2",
        "GRO_XENO_HYBRIDS", 
        "SHP_WEAPON_7",
        "SHP_CONT_SYMB",
        "DEF_DEFENSE_NET_2",
        "DEF_PLAN_BARRIER_SHLD_1",
        "LRN_PHYS_BRAIN", 
        "LRN_TRANSLING_THT", 
        "SHP_ENDOSYMB_HULL",
        "SHP_ASTEROID_HULLS",
        "SHP_WEAPON_8",
        "GRO_TERRAFORM",
        "PRO_SOL_ORB_GEN",
        "CON_METRO_INFRA", 
        "CON_SUBTER_CONST", 
        "GRO_GENETIC_MED",
        "SHP_LEAD_PLATE", 
        "SHP_MONOCELL_EXP",
        "LRN_XENOARCH",
        "GRO_CYBORG", 
        "PRO_SENTIENT_AUTOMATION", 
        "PRO_EXOBOTS",
        "DEF_DEFENSE_NET_REGEN_1",
        "DEF_PLAN_BARRIER_SHLD_2",
        "DEF_DEFENSE_NET_3",
        "SHP_BIOADAPTIVE_SPEC",
        "SHP_WEAPON_9",
        "DEF_SYST_DEF_MINE_1",
        "DEF_PLAN_BARRIER_SHLD_3",
        "LRN_QUANT_NET",
        "SHP_INTSTEL_LOG",
        "SHP_REINFORCED_HULL",
        "SHP_WEAPON_10", 
        "CON_NDIM_STRC",
        "SHP_CONT_BIOADAPT",
        "SHP_SENT_HULL",
        "SHP_ZORTRIUM_PLATE",
        "DEF_SYST_DEF_MINE_2",
        "GRO_GENOME_BANK",
        "SHP_BASIC_DAM_CONT",
        "CON_ARCH_PSYCH",
        "CON_CONC_CAMP",
        "PRO_SINGULAR_GEN",
        "CON_FRC_ENRG_CAMO",
        "SHP_ADV_DAM_CONT",
        "PRO_INDUSTRY_CENTER_III", 
        "DEF_GARRISON_4",
        "SPY_CLOAK_1", 
        "SPY_DETECT_3",
        "DEF_DEFENSE_NET_REGEN_2",
        "GRO_GAIA_TRANS",
        "GRO_ENERGY_META",
        "CON_ART_PLANET",
        "CON_FRC_ENRG_STRC",
        "PRO_NEUTRONIUM_EXTRACTION",
        "LRN_STELLAR_TOMOGRAPHY",
        "LRN_ENCLAVE_VOID",
        "CON_ORBITAL_HAB",
        "LRN_PSIONICS",
        "LRN_DISTRIB_THOUGHT", 
        "GRO_ENERGY_META", 
        "DEF_SYST_DEF_MINE_3",
        "DEF_PLAN_BARRIER_SHLD_4",
        "SHP_WEAPON_13",
        "SPY_DETECT_4",
        "SHP_WEAPON_11", 
        "SHP_WEAPON_12",
        "SHP_TITAN_HULL"
        "DEF_PLAN_BARRIER_SHLD_5",
        "SHP_SOLAR_CONT",
        "SHP_WEAPON_14",
        "SHP_WEAPON_15",
        "SHP_WEAPON_16",
        "SHP_WEAPON_17",
        ]
    return aggressiveTechsList
