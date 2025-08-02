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
        // Initialize terminal widgets
        if (!m_TerminalWidgets)
        {
            m_TerminalWidgets = new TSE_TerminalMenuWidgets();
        }
        
        // Initialize widgets with root frame
        if (m_wRoot)
        {
            m_TerminalWidgets.InitializeWidgets(m_wRoot);
        }
        
        // Load terminal data (placeholder for future implementation)
        LoadTerminalData();
        
        // Show terminal
        if (m_TerminalWidgets)
        {
            m_TerminalWidgets.ShowTerminal(true);
        }
    }
    
    override void OnActiveFrame(float timeSlice)
    {
        super.OnActiveFrame(timeSlice);
        
        // Handle tab switching via input actions
        if (m_InputManager.GetActionTriggered("MenuSelect"))
        {
            // This will be handled by the button components
        }
        
        // Handle close action
        if (m_InputManager.GetActionTriggered("MenuBack"))
        {
            CloseLayout();
        }
    }
    
    override void OnClose()
    {
        // Hide terminal
        if (m_TerminalWidgets)
        {
            m_TerminalWidgets.ShowTerminal(false);
        }
    }
    
    // Set laptop entity for this context
    void SetLaptop(IEntity laptop)
    {
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
        if (!m_TerminalWidgets)
            return;
        
        // TODO: Load real data from game systems
        // For now, use default data from widget initialization
        
        // Example of how to update data when real systems are implemented:
        // m_TerminalWidgets.UpdateRegionStability(GetCurrentRegionStability());
        // m_TerminalWidgets.UpdateFactionProgress(GetOFProgress(), GetFIAProgress(), GetSFProgress(), GetSmuglersProgress());
        // m_TerminalWidgets.UpdateAvailableWeapons(GetAvailableWeapons());
        // m_TerminalWidgets.UpdateAvailableVehicles(GetAvailableVehicles());
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