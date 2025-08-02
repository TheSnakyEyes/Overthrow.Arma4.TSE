// Terminal Menu Widgets Script
// Handles the terminal UI functionality including tab switching and data updates

class TSE_TerminalMenuWidgets
{
    protected Widget m_RootFrame;
    protected Widget m_IntelLayoutUp;
    protected Widget m_IntelLayoutDown;
    protected Widget m_FactionLayoutUp;
    protected Widget m_SmugglersLayoutUp;
    protected Widget m_SmugglersLayoutDown;
    
    protected ButtonWidget m_IntelTab;
    protected ButtonWidget m_FactionsTab;
    protected ButtonWidget m_SmuglersTab;
    
    protected ButtonWidget m_ButtonExit;
    protected TextWidget m_TabIndicatorText;
    
    protected int m_ActiveTab;
    
    // Image widgets for tab highlighting
    protected ImageWidget m_IntelTabImage;
    protected ImageWidget m_FactionsTabImage;
    protected ImageWidget m_SmuglersTabImage;
    
    // Colors for tab highlighting
    protected ref Color m_ActiveTabColor;
    protected ref Color m_InactiveTabColor;
    
    // Constructor
    void TSE_TerminalMenuWidgets()
    {
        m_ActiveTab = 0; // Intel tab is default
    }
    
    // Initialize widgets with root frame
    void InitializeWidgets(Widget rootFrame)
    {
        m_RootFrame = rootFrame;
        
        if (!rootFrame)
            return;
        
        // Initialize colors
        m_ActiveTabColor = new Color(0.1098, 0.302, 0.8745, 1.0); // Blue color for active tab
        m_InactiveTabColor = new Color(0.2627, 0.2627, 0.2627, 1.0); // Gray color for inactive tab
        
        // Find content layouts
        m_IntelLayoutUp = rootFrame.FindAnyWidget("Intel_Layout_Up");
        m_IntelLayoutDown = rootFrame.FindAnyWidget("Intel_Layout_Down");
        m_FactionLayoutUp = rootFrame.FindAnyWidget("Faction_Layout_Up");
        m_SmugglersLayoutUp = rootFrame.FindAnyWidget("Smugglers_Layout_Up");
        m_SmugglersLayoutDown = rootFrame.FindAnyWidget("Smugglers_Layout_Down");
        
        // Find tab buttons
        m_IntelTab = ButtonWidget.Cast(rootFrame.FindAnyWidget("Tab_Intel"));
        m_FactionsTab = ButtonWidget.Cast(rootFrame.FindAnyWidget("Tab_Factions"));
        m_SmuglersTab = ButtonWidget.Cast(rootFrame.FindAnyWidget("Tab_Smuglers"));
        
        // Find tab images (renamed Image0 widgets)
        m_IntelTabImage = ImageWidget.Cast(rootFrame.FindAnyWidget("Image_Intel"));
        m_FactionsTabImage = ImageWidget.Cast(rootFrame.FindAnyWidget("Image_Factions"));
        m_SmuglersTabImage = ImageWidget.Cast(rootFrame.FindAnyWidget("Image_Smuglers"));
        
        // Find other widgets
        m_ButtonExit = ButtonWidget.Cast(rootFrame.FindAnyWidget("ButtonExit"));
        m_TabIndicatorText = TextWidget.Cast(rootFrame.FindAnyWidget("Text0"));
        
        SetupEventHandlers();
        SwitchTab(0); // Initialize with Intel tab active
    }
    
    // Setup event handlers for buttons
    protected void SetupEventHandlers()
    {
        // Setup tab button events
        if (m_IntelTab)
        {
            SCR_InputButtonComponent inputComponent = SCR_InputButtonComponent.Cast(m_IntelTab.FindHandler(SCR_InputButtonComponent));
            if (inputComponent)
                inputComponent.m_OnActivated.Insert(OnIntelTabClicked);
        }
        
        if (m_FactionsTab)
        {
            SCR_InputButtonComponent inputComponent = SCR_InputButtonComponent.Cast(m_FactionsTab.FindHandler(SCR_InputButtonComponent));
            if (inputComponent)
                inputComponent.m_OnActivated.Insert(OnFactionsTabClicked);
        }
        
        if (m_SmuglersTab)
        {
            SCR_InputButtonComponent inputComponent = SCR_InputButtonComponent.Cast(m_SmuglersTab.FindHandler(SCR_InputButtonComponent));
            if (inputComponent)
                inputComponent.m_OnActivated.Insert(OnSmuglersTabClicked);
        }
        
        // Setup exit button event
        if (m_ButtonExit)
        {
            SCR_ButtonBaseComponent buttonComponent = SCR_ButtonBaseComponent.Cast(m_ButtonExit.FindHandler(SCR_ButtonBaseComponent));
            if (buttonComponent)
            {
                buttonComponent.m_OnClick.Insert(OnCloseButtonClicked);
            }
        }
    }
    
    // Tab button click handlers
    protected void OnIntelTabClicked()
    {
        SwitchTab(0);
    }
    
    protected void OnFactionsTabClicked()
    {
        SwitchTab(1);
    }
    
    protected void OnSmuglersTabClicked()
    {
        SwitchTab(2);
    }
    
    // Switch between tabs
    protected void SwitchTab(int tabIndex)
    {
        // Hide all content layouts
        if (m_IntelLayoutUp) m_IntelLayoutUp.SetVisible(false);
        if (m_IntelLayoutDown) m_IntelLayoutDown.SetVisible(false);
        if (m_FactionLayoutUp) m_FactionLayoutUp.SetVisible(false);
        if (m_SmugglersLayoutUp) m_SmugglersLayoutUp.SetVisible(false);
        if (m_SmugglersLayoutDown) m_SmugglersLayoutDown.SetVisible(false);
        
        // Show appropriate content based on tab
        switch (tabIndex)
        {
            case 0: // Intel
                if (m_IntelLayoutUp) m_IntelLayoutUp.SetVisible(true);
                if (m_IntelLayoutDown) m_IntelLayoutDown.SetVisible(true);
                if (m_TabIndicatorText) m_TabIndicatorText.SetText("#TSE-Terminal_Intel_Tab");
                break;
            case 1: // Factions
                if (m_FactionLayoutUp) m_FactionLayoutUp.SetVisible(true);
                if (m_TabIndicatorText) m_TabIndicatorText.SetText("#TSE-Terminal_Faction_Tab");
                break;
            case 2: // Smugglers
                if (m_SmugglersLayoutUp) m_SmugglersLayoutUp.SetVisible(true);
                if (m_SmugglersLayoutDown) m_SmugglersLayoutDown.SetVisible(true);
                if (m_TabIndicatorText) m_TabIndicatorText.SetText("#TSE-Terminal_Smugler_Tab");
                break;
        }
        
        m_ActiveTab = tabIndex;
        UpdateTabImageColors(); // Update image colors after switching tabs
    }
    
    protected void UpdateTabImageColors()
    {
        // Update Intel tab image color
        if (m_IntelTabImage)
        {
            if (m_ActiveTab == 0)
                m_IntelTabImage.SetColor(m_ActiveTabColor);
            else
                m_IntelTabImage.SetColor(m_InactiveTabColor);
        }
        
        // Update Factions tab image color
        if (m_FactionsTabImage)
        {
            if (m_ActiveTab == 1)
                m_FactionsTabImage.SetColor(m_ActiveTabColor);
            else
                m_FactionsTabImage.SetColor(m_InactiveTabColor);
        }
        
        // Update Smugglers tab image color
        if (m_SmuglersTabImage)
        {
            if (m_ActiveTab == 2)
                m_SmuglersTabImage.SetColor(m_ActiveTabColor);
            else
                m_SmuglersTabImage.SetColor(m_InactiveTabColor);
        }
    }
    
    // Close button handler
    protected void OnCloseButtonClicked()
    {
        
        // Get the UI context and close it using CloseLayout
        ChimeraCharacter player = ChimeraCharacter.Cast(GetGame().GetPlayerManager().GetPlayerControlledEntity(0));
        if (player)
        {
            OVT_UIManagerComponent uiManager = OVT_UIManagerComponent.Cast(player.FindComponent(OVT_UIManagerComponent));
            if (uiManager)
            {
                TSE_TerminalContext context = TSE_TerminalContext.Cast(uiManager.GetContextByString("OverthrowTerminalContext"));
                if (context)
                {
                    context.CloseLayout();
                }
            }
        }
    }
    
    // Show/hide terminal
    void ShowTerminal(bool show)
    {
        if (m_RootFrame)
        {
            m_RootFrame.SetVisible(show);
        }
    }
    
    // Get current active tab
    int GetActiveTab()
    {
        return m_ActiveTab;
    }
}