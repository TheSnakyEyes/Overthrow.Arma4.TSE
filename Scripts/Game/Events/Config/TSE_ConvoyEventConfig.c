[BaseContainerProps()]
class TSE_ConvoyEventConfig : TSE_BaseEventConfig
{
    [Attribute("", UIWidgets.Object, desc: "Convoy cargo configuration")]
    ref TSE_ConvoyCargoConfig m_CargoConfig;
    
    [Attribute(defvalue: "500", desc: "Reward for convoy reaching destination")]
    int m_iSuccessReward;
    
    [Attribute(defvalue: "50", desc: "Convoy destination reach tolerance (meters)")]
    float m_fDestinationTolerance;
    
    [Attribute(defvalue: "300", desc: "Convoy marker update interval (seconds)")]
    float m_fMarkerUpdateInterval;
    
    void TSE_ConvoyEventConfig()
    {
        m_sEventName = "Convoy Event";
        m_iSpawnChance = 60;
        m_iMinIntervalHours = 18;
        m_iMaxIntervalHours = 36;
        m_iDurationHours = 8;
        m_iPriority = 2;
        m_bAllowSimultaneous = false;
    }
    
    // Load configuration from file
    static TSE_ConvoyEventConfig LoadConfig()
    {
        Resource configResource = Resource.Load("{3F0D5071BC60E43C}Configs/Events/TSE_ConvoyEventConfig.conf");
        if (configResource && configResource.IsValid())
        {
            TSE_ConvoyEventConfig config = TSE_ConvoyEventConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(configResource.GetResource().ToBaseContainer()));
            if (config)
            {
                Print("[ConvoyEvent] Configuration loaded from file");
                return config;
            }
        }
        
        Print("[ConvoyEvent] Using default configuration");
        return new TSE_ConvoyEventConfig();
    }
} 