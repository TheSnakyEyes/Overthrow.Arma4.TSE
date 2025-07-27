[BaseContainerProps()]
class TSE_EventManagerConfig
{
    [Attribute(defvalue: "300", desc: "Event check interval (seconds) - how often manager checks for new events")]
    float m_fEventCheckInterval;
    
    [Attribute(defvalue: "2", desc: "Maximum simultaneous events")]
    int m_iMaxSimultaneousEvents;
    
    [Attribute(defvalue: "1", desc: "Minimum hours between any events")]
    int m_iMinGlobalCooldown;
    
    [Attribute(defvalue: "true", desc: "Enable event system")]
    bool m_bEventSystemEnabled;
    
    [Attribute(defvalue: "2", desc: "Initial delay before first event check (minutes)")]
    int m_iInitialDelayMinutes;
    
    [Attribute(defvalue: "true", desc: "Enable debug logging")]
    bool m_bDebugLogging;
    
    void TSE_EventManagerConfig()
    {
        // Set default testing values
        m_fEventCheckInterval = 300; // 5 minutes
        m_iMaxSimultaneousEvents = 2;
        m_iMinGlobalCooldown = 1; // 1 hour
        m_bEventSystemEnabled = true;
        m_iInitialDelayMinutes = 2; // 2 minutes
        m_bDebugLogging = true;
    }
    
    // Load configuration from file
    static TSE_EventManagerConfig LoadConfig()
    {
        Resource configResource = Resource.Load("{FA4CE661FAF3BB4B}Configs/Events/TSE_EventManagerConfig.conf");
        if (configResource && configResource.IsValid())
        {
            TSE_EventManagerConfig config = TSE_EventManagerConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(configResource.GetResource().ToBaseContainer()));
            if (config)
            {
                Print("[EventManager] Configuration loaded from file");
                return config;
            }
        }
        
        Print("[EventManager] Using default configuration");
        return new TSE_EventManagerConfig();
    }
} 