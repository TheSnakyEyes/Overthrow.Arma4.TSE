// Terminal Menu Widgets Script
// Handles the terminal UI functionality including tab switching and data updates

class TSE_TerminalMenuWidgets
{
    // Widget references
    protected Widget m_RootFrame;
    protected Widget m_IntelContent;
    protected Widget m_FactionContent;
    protected Widget m_SmuglerContent;
    
    // Tab buttons
    protected ButtonWidget m_IntelTab;
    protected ButtonWidget m_FactionTab;
    protected ButtonWidget m_SmuglerTab;
    
    // Intel tab widgets
    protected TextWidget m_StabilityValue;
    protected TextWidget m_LocationsValue;
    protected TextWidget m_ResourcesValue;
    protected TextWidget m_ThreatValue;
    protected TextWidget m_PlansValue;
    protected TextWidget m_UpdatedValue;
    protected TextWidget m_DeliveryValue;
    
    // Faction tab widgets
    protected ProgressBarWidget m_OFBar;
    protected ProgressBarWidget m_FIABar;
    protected ProgressBarWidget m_SFBar;
    protected ProgressBarWidget m_SmuglersBar;
    
    // Smugler tab widgets
    protected TextWidget m_WeaponsValue;
    protected TextWidget m_VehiclesValue;
    protected ButtonWidget m_BarrageButton;
    protected ButtonWidget m_DistractionButton;
    
    // Close button
    protected ButtonWidget m_CloseButton;
    
    // Current active tab
    protected int m_ActiveTab;
    
    // Data variables (placeholder for future implementation)
    protected string m_RegionStability;
    protected string m_AffectedLocations;
    protected string m_OccupyingResources;
    protected string m_ResistanceThreat;
    protected string m_FuturePlans;
    protected string m_LastUpdated;
    protected string m_NextDelivery;
    
    protected float m_OFProgress;
    protected float m_FIAProgress;
    protected float m_SFProgress;
    protected float m_SmuglersProgress;
    
    protected string m_AvailableWeapons;
    protected string m_AvailableVehicles;
    
    // Constructor
    void TSE_TerminalMenuWidgets()
    {
        m_ActiveTab = 0; // Intel tab is default
        InitializeData();
    }
    
    // Initialize with default data
    protected void InitializeData()
    {
        m_RegionStability = "Unstable";
        m_AffectedLocations = "Our people suffer in City_1, City_2, City_3";
        m_OccupyingResources = "Strong";
        m_ResistanceThreat = "Extreme";
        m_FuturePlans = "Gain stability and-or Manage resources and-or Fight Resistance and-or Raid on Smuglers";
        m_LastUpdated = "N-time ago";
        m_NextDelivery = "t-minus hours";
        
        m_OFProgress = 0.6;
        m_FIAProgress = 0.7;
        m_SFProgress = 0.4;
        m_SmuglersProgress = 0.5;
        
        m_AvailableWeapons = "Tier of available weapons: Tier 1, Tier 2, Tier 3";
        m_AvailableVehicles = "Tier of available vehicles: Basic, Special, Armored";
    }
    
    // Initialize widgets with root frame
    void InitializeWidgets(Widget rootFrame)
    {
        m_RootFrame = rootFrame;
        
        // Find content panels
        m_IntelContent = rootFrame.FindAnyWidget("IntelContent");
        m_FactionContent = rootFrame.FindAnyWidget("FactionContent");
        m_SmuglerContent = rootFrame.FindAnyWidget("SmuglerContent");
        
        // Find tab buttons
        m_IntelTab = ButtonWidget.Cast(rootFrame.FindAnyWidget("IntelTab"));
        m_FactionTab = ButtonWidget.Cast(rootFrame.FindAnyWidget("FactionTab"));
        m_SmuglerTab = ButtonWidget.Cast(rootFrame.FindAnyWidget("SmuglerTab"));
        
        // Find Intel tab widgets
        m_StabilityValue = TextWidget.Cast(rootFrame.FindAnyWidget("StabilityValue"));
        m_LocationsValue = TextWidget.Cast(rootFrame.FindAnyWidget("LocationsValue"));
        m_ResourcesValue = TextWidget.Cast(rootFrame.FindAnyWidget("ResourcesValue"));
        m_ThreatValue = TextWidget.Cast(rootFrame.FindAnyWidget("ThreatValue"));
        m_PlansValue = TextWidget.Cast(rootFrame.FindAnyWidget("PlansValue"));
        m_UpdatedValue = TextWidget.Cast(rootFrame.FindAnyWidget("UpdatedValue"));
        m_DeliveryValue = TextWidget.Cast(rootFrame.FindAnyWidget("DeliveryValue"));
        
        // Find Faction tab widgets
        m_OFBar = ProgressBarWidget.Cast(rootFrame.FindAnyWidget("OFBar"));
        m_FIABar = ProgressBarWidget.Cast(rootFrame.FindAnyWidget("FIABar"));
        m_SFBar = ProgressBarWidget.Cast(rootFrame.FindAnyWidget("SFBar"));
        m_SmuglersBar = ProgressBarWidget.Cast(rootFrame.FindAnyWidget("SmuglersBar"));
        
        // Find Smugler tab widgets
        m_WeaponsValue = TextWidget.Cast(rootFrame.FindAnyWidget("WeaponsValue"));
        m_VehiclesValue = TextWidget.Cast(rootFrame.FindAnyWidget("VehiclesValue"));
        m_BarrageButton = ButtonWidget.Cast(rootFrame.FindAnyWidget("BarrageButton"));
        m_DistractionButton = ButtonWidget.Cast(rootFrame.FindAnyWidget("DistractionButton"));
        
        // Find close button
        m_CloseButton = ButtonWidget.Cast(rootFrame.FindAnyWidget("CloseButton"));
        
        // Setup event handlers for buttons
        SetupEventHandlers();
        
        // Initialize UI with default data
        UpdateUI();
    }
    
    // Tab click handlers
    protected void OnIntelTabClicked()
    {
        SwitchTab(0);
    }
    
    protected void OnFactionTabClicked()
    {
        SwitchTab(1);
    }
    
    protected void OnSmuglerTabClicked()
    {
        SwitchTab(2);
    }
    
    // Switch between tabs
    protected void SwitchTab(int tabIndex)
    {
        // Hide all content panels
        if (m_IntelContent)
            m_IntelContent.SetVisible(false);
        if (m_FactionContent)
            m_FactionContent.SetVisible(false);
        if (m_SmuglerContent)
            m_SmuglerContent.SetVisible(false);
        
        // Reset all tab button colors
        if (m_IntelTab)
        {
            m_IntelTab.SetColor(Color.FromSRGBA(204, 204, 204, 255));
            ImageWidget icon = ImageWidget.Cast(m_IntelTab.FindAnyWidget("IntelIcon"));
            if (icon)
                icon.SetColor(Color.FromSRGBA(204, 204, 204, 255));
        }
        if (m_FactionTab)
        {
            m_FactionTab.SetColor(Color.FromSRGBA(204, 204, 204, 255));
            ImageWidget icon = ImageWidget.Cast(m_FactionTab.FindAnyWidget("FactionIcon"));
            if (icon)
                icon.SetColor(Color.FromSRGBA(204, 204, 204, 255));
        }
        if (m_SmuglerTab)
        {
            m_SmuglerTab.SetColor(Color.FromSRGBA(204, 204, 204, 255));
            ImageWidget icon = ImageWidget.Cast(m_SmuglerTab.FindAnyWidget("SmuglerIcon"));
            if (icon)
                icon.SetColor(Color.FromSRGBA(204, 204, 204, 255));
        }
        
        // Show selected content and highlight tab
        switch (tabIndex)
        {
            case 0: // Intel
                if (m_IntelContent)
                    m_IntelContent.SetVisible(true);
                if (m_IntelTab)
                {
                    m_IntelTab.SetColor(Color.FromSRGBA(255, 153, 0, 255));
                    ImageWidget icon = ImageWidget.Cast(m_IntelTab.FindAnyWidget("IntelIcon"));
                    if (icon)
                        icon.SetColor(Color.FromSRGBA(255, 153, 0, 255));
                }
                break;
                
            case 1: // Faction Info
                if (m_FactionContent)
                    m_FactionContent.SetVisible(true);
                if (m_FactionTab)
                {
                    m_FactionTab.SetColor(Color.FromSRGBA(255, 153, 0, 255));
                    ImageWidget icon = ImageWidget.Cast(m_FactionTab.FindAnyWidget("FactionIcon"));
                    if (icon)
                        icon.SetColor(Color.FromSRGBA(255, 153, 0, 255));
                }
                break;
                
            case 2: // Smugler's Services
                if (m_SmuglerContent)
                    m_SmuglerContent.SetVisible(true);
                if (m_SmuglerTab)
                {
                    m_SmuglerTab.SetColor(Color.FromSRGBA(255, 153, 0, 255));
                    ImageWidget icon = ImageWidget.Cast(m_SmuglerTab.FindAnyWidget("SmuglerIcon"));
                    if (icon)
                        icon.SetColor(Color.FromSRGBA(255, 153, 0, 255));
                }
                break;
        }
        
        m_ActiveTab = tabIndex;
    }
    
    // Close button handler
    protected void OnCloseButtonClicked()
    {
        if (m_RootFrame)
            m_RootFrame.SetVisible(false);
    }
    
    // Service button handlers (placeholder for future implementation)
    protected void OnBarrageButtonClicked()
    {
        // TODO: Implement barrage service functionality
        Print("Barrage service button clicked - functionality to be implemented");
    }
    
    protected void OnDistractionButtonClicked()
    {
        // TODO: Implement distraction service functionality
        Print("Distraction service button clicked - functionality to be implemented");
    }
    
    // Update UI with current data
    void UpdateUI()
    {
        // Update Intel tab
        if (m_StabilityValue) m_StabilityValue.SetText(m_RegionStability);
        if (m_LocationsValue) m_LocationsValue.SetText(m_AffectedLocations);
        if (m_ResourcesValue) m_ResourcesValue.SetText(m_OccupyingResources);
        if (m_ThreatValue) m_ThreatValue.SetText(m_ResistanceThreat);
        if (m_PlansValue) m_PlansValue.SetText(m_FuturePlans);
        if (m_UpdatedValue) m_UpdatedValue.SetText(m_LastUpdated);
        if (m_DeliveryValue) m_DeliveryValue.SetText(m_NextDelivery);
        
        // Update Faction tab
        if (m_OFBar) m_OFBar.SetCurrent(m_OFProgress);
        if (m_FIABar) m_FIABar.SetCurrent(m_FIAProgress);
        if (m_SFBar) m_SFBar.SetCurrent(m_SFProgress);
        if (m_SmuglersBar) m_SmuglersBar.SetCurrent(m_SmuglersProgress);
        
        // Update Smugler tab
        if (m_WeaponsValue) m_WeaponsValue.SetText(m_AvailableWeapons);
        if (m_VehiclesValue) m_VehiclesValue.SetText(m_AvailableVehicles);
    }
    
    // Public methods for updating data (to be called from external systems)
    void UpdateRegionStability(string stability)
    {
        m_RegionStability = stability;
        if (m_StabilityValue) m_StabilityValue.SetText(stability);
    }
    
    void UpdateAffectedLocations(string locations)
    {
        m_AffectedLocations = locations;
        if (m_LocationsValue) m_LocationsValue.SetText(locations);
    }
    
    void UpdateOccupyingResources(string resources)
    {
        m_OccupyingResources = resources;
        if (m_ResourcesValue) m_ResourcesValue.SetText(resources);
    }
    
    void UpdateResistanceThreat(string threat)
    {
        m_ResistanceThreat = threat;
        if (m_ThreatValue) m_ThreatValue.SetText(threat);
    }
    
    void UpdateFuturePlans(string plans)
    {
        m_FuturePlans = plans;
        if (m_PlansValue) m_PlansValue.SetText(plans);
    }
    
    void UpdateLastUpdated(string updated)
    {
        m_LastUpdated = updated;
        if (m_UpdatedValue) m_UpdatedValue.SetText(updated);
    }
    
    void UpdateNextDelivery(string delivery)
    {
        m_NextDelivery = delivery;
        if (m_DeliveryValue) m_DeliveryValue.SetText(delivery);
    }
    
    void UpdateFactionProgress(float ofProgress, float fiaProgress, float sfProgress, float smuglersProgress)
    {
        m_OFProgress = ofProgress;
        m_FIAProgress = fiaProgress;
        m_SFProgress = sfProgress;
        m_SmuglersProgress = smuglersProgress;
        
        if (m_OFBar) m_OFBar.SetCurrent(ofProgress);
        if (m_FIABar) m_FIABar.SetCurrent(fiaProgress);
        if (m_SFBar) m_SFBar.SetCurrent(sfProgress);
        if (m_SmuglersBar) m_SmuglersBar.SetCurrent(smuglersProgress);
    }
    
    void UpdateAvailableWeapons(string weapons)
    {
        m_AvailableWeapons = weapons;
        if (m_WeaponsValue) m_WeaponsValue.SetText(weapons);
    }
    
    void UpdateAvailableVehicles(string vehicles)
    {
        m_AvailableVehicles = vehicles;
        if (m_VehiclesValue) m_VehiclesValue.SetText(vehicles);
    }
    
    // Show/hide terminal
    void ShowTerminal(bool show)
    {
        if (m_RootFrame)
            m_RootFrame.SetVisible(show);
    }
    
    // Get current active tab
    int GetActiveTab()
    {
        return m_ActiveTab;
    }

    // Setup event handlers for buttons
    protected void SetupEventHandlers()
    {
        // Tab buttons
        if (m_IntelTab)
        {
            SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(m_IntelTab.FindHandler(SCR_InputButtonComponent));
            if (action)
                action.m_OnActivated.Insert(OnIntelTabClicked);
        }
        
        if (m_FactionTab)
        {
            SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(m_FactionTab.FindHandler(SCR_InputButtonComponent));
            if (action)
                action.m_OnActivated.Insert(OnFactionTabClicked);
        }
        
        if (m_SmuglerTab)
        {
            SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(m_SmuglerTab.FindHandler(SCR_InputButtonComponent));
            if (action)
                action.m_OnActivated.Insert(OnSmuglerTabClicked);
        }
        
        // Service buttons
        if (m_BarrageButton)
        {
            SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(m_BarrageButton.FindHandler(SCR_InputButtonComponent));
            if (action)
                action.m_OnActivated.Insert(OnBarrageButtonClicked);
        }
        
        if (m_DistractionButton)
        {
            SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(m_DistractionButton.FindHandler(SCR_InputButtonComponent));
            if (action)
                action.m_OnActivated.Insert(OnDistractionButtonClicked);
        }
        
        // Close button
        if (m_CloseButton)
        {
            SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(m_CloseButton.FindHandler(SCR_InputButtonComponent));
            if (action)
                action.m_OnActivated.Insert(OnCloseButtonClicked);
        }
    }
}