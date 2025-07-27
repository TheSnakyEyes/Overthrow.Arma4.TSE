[BaseContainerProps()]
class TSE_AirDropEventConfig : TSE_BaseEventConfig
{
    [Attribute("", UIWidgets.Object, desc: "Air drop cargo configuration")]
    ref TSE_AirDropCargoConfig m_CargoConfig;
    
    [Attribute(defvalue: "10", desc: "Crate check interval (minutes)")]
    int m_iCrateCheckIntervalMinutes;
    
    [Attribute(defvalue: "180", desc: "Time before special forces spawn (minutes - 3 hours)")]
    int m_iSpecialForcesDelayMinutes;
    
    [Attribute(defvalue: "800", desc: "Special forces spawn distance from crate (meters)")]
    float m_fSpecialForcesSpawnDistance;
    
    [Attribute(defvalue: "300", desc: "Reward for occupying faction if crate not emptied")]
    int m_iOccupationSuccessReward;
    
    [Attribute(defvalue: "2", desc: "Number of special forces squads to spawn")]
    int m_iSpecialForcesSquads;
    
    void TSE_AirDropEventConfig()
    {
        m_sEventName = "Air Drop Event";
        m_iSpawnChance = 90; // High chance for testing
        m_iMinIntervalHours = 72;
        m_iMaxIntervalHours = 120;
        m_iDurationHours = 6;
        m_iPriority = 1; // Highest priority for testing
        m_bAllowSimultaneous = false;
        m_iMinGameTimeHours = 24;
    }
    
    // Load configuration from file
    static TSE_AirDropEventConfig LoadConfig()
    {
        Resource configResource = Resource.Load("{C7E2A5EB0A8B3D1F}Configs/Events/TSE_AirDropEventConfig.conf");
        if (configResource && configResource.IsValid())
        {
            TSE_AirDropEventConfig config = TSE_AirDropEventConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(configResource.GetResource().ToBaseContainer()));
            if (config)
            {
                Print("[AirDropEvent] Configuration loaded from file");
                return config;
            }
        }
        
        Print("[AirDropEvent] Using default configuration");
        return new TSE_AirDropEventConfig();
    }
} 