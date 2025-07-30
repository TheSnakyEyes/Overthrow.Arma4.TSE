class TSE_TerminalContext : OVT_UIContext
{
    protected IEntity m_Laptop;
    protected ref TSE_TerminalMenuWidgets m_TerminalWidgets;
    
    override void PostInit()
    {
        if (SCR_Global.IsEditMode()) 
            return;
    }
    
    override void OnShow()
    {
        Print("[TSE_TerminalContext] OnShow called");
        
        // Initialize terminal widgets
        if (!m_TerminalWidgets)
        {
            Print("[TSE_TerminalContext] Creating new terminal widgets");
            m_TerminalWidgets = new TSE_TerminalMenuWidgets();
        }
        
        // Initialize widgets with root frame
        if (m_wRoot)
        {
            Print("[TSE_TerminalContext] Initializing widgets with root frame");
            m_TerminalWidgets.InitializeWidgets(m_wRoot);
        }
        else
        {
            Print("[TSE_TerminalContext] ERROR: No root frame found");
        }
        
        // Load terminal data (placeholder for future implementation)
        LoadTerminalData();
        
        // Show terminal
        if (m_TerminalWidgets)
        {
            Print("[TSE_TerminalContext] Showing terminal");
            m_TerminalWidgets.ShowTerminal(true);
        }
        
        Print("[TSE_TerminalContext] OnShow completed");
    }
    
    override void OnClose()
    {
        Print("[TSE_TerminalContext] OnClose called");
        
        // Hide terminal
        if (m_TerminalWidgets)
        {
            m_TerminalWidgets.ShowTerminal(false);
        }
    }
    
    // Set laptop entity for this context
    void SetLaptop(IEntity laptop)
    {
        Print("[TSE_TerminalContext] SetLaptop called with: " + laptop.GetName());
        m_Laptop = laptop;
    }
    
    // Get laptop entity
    IEntity GetLaptop()
    {
        return m_Laptop;
    }
    
    // Load terminal data (placeholder for future implementation)
    protected void LoadTerminalData()
    {
        Print("[TSE_TerminalContext] LoadTerminalData called");
        
        if (!m_TerminalWidgets)
        {
            Print("[TSE_TerminalContext] ERROR: No terminal widgets in LoadTerminalData");
            return;
        }
        
        // TODO: Load real data from game systems
        // For now, use default data from widget initialization
        
        // Example of how to update data when real systems are implemented:
        // m_TerminalWidgets.UpdateRegionStability(GetCurrentRegionStability());
        // m_TerminalWidgets.UpdateFactionProgress(GetOFProgress(), GetFIAProgress(), GetSFProgress(), GetSmuglersProgress());
        // m_TerminalWidgets.UpdateAvailableWeapons(GetAvailableWeapons());
        // m_TerminalWidgets.UpdateAvailableVehicles(GetAvailableVehicles());
    }
    
    // Public methods for updating terminal data (to be called from external systems)
    void UpdateRegionStability(string stability)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateRegionStability(stability);
    }
    
    void UpdateAffectedLocations(string locations)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateAffectedLocations(locations);
    }
    
    void UpdateOccupyingResources(string resources)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateOccupyingResources(resources);
    }
    
    void UpdateResistanceThreat(string threat)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateResistanceThreat(threat);
    }
    
    void UpdateFuturePlans(string plans)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateFuturePlans(plans);
    }
    
    void UpdateLastUpdated(string updated)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateLastUpdated(updated);
    }
    
    void UpdateNextDelivery(string delivery)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateNextDelivery(delivery);
    }
    
    void UpdateFactionProgress(float ofProgress, float fiaProgress, float sfProgress, float smuglersProgress)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateFactionProgress(ofProgress, fiaProgress, sfProgress, smuglersProgress);
    }
    
    void UpdateAvailableWeapons(string weapons)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateAvailableWeapons(weapons);
    }
    
    void UpdateAvailableVehicles(string vehicles)
    {
        if (m_TerminalWidgets)
            m_TerminalWidgets.UpdateAvailableVehicles(vehicles);
    }
    
    // Get current active tab
    int GetActiveTab()
    {
        if (m_TerminalWidgets)
            return m_TerminalWidgets.GetActiveTab();
        return 0;
    }
    
    // Refresh terminal data
    void RefreshTerminalData()
    {
        LoadTerminalData();
    }
}