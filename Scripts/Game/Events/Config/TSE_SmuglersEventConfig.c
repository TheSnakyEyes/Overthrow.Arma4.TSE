[BaseContainerProps()]
class TSE_SmuglersEventConfig : TSE_BaseEventConfig
{
    [Attribute("", UIWidgets.Object, desc: "Smuggler crate content configuration")]
    ref TSE_SmuglerCrateContents m_CrateConfig;
    
    [Attribute(defvalue: "100", desc: "Vehicle cleanup radius around spawn markers (meters)")]
    float m_fCleanupRadius;
    
    [Attribute(defvalue: "900", desc: "Event monitoring interval (seconds)")]
    float m_fMonitoringInterval;
    
    void TSE_SmuglersEventConfig()
    {
        m_sEventName = "Smugglers Event";
        m_iSpawnChance = 40;
        m_iMinIntervalHours = 12;
        m_iMaxIntervalHours = 30;
        m_iDurationHours = 12;
        m_iPriority = 1;
        m_bAllowSimultaneous = true;
    }
    
    // Load configuration from file
    static TSE_SmuglersEventConfig LoadConfig()
    {
        Resource configResource = Resource.Load("{AA7A1AF2D2C1C5E1}Configs/Events/TSE_SmuglersEventConfig.conf");
        if (configResource && configResource.IsValid())
        {
            TSE_SmuglersEventConfig config = TSE_SmuglersEventConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(configResource.GetResource().ToBaseContainer()));
            if (config)
            {
                Print("[SmuglersEvent] Configuration loaded from file");
                return config;
            }
        }
        
        Print("[SmuglersEvent] Using default configuration");
        return new TSE_SmuglersEventConfig();
    }
} 