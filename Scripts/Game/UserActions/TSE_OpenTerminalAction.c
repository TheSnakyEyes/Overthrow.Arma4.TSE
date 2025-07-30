class TSE_OpenTerminalAction : ScriptedUserAction
{
    IEntity m_Laptop;
    
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        m_Laptop = pOwnerEntity;
        Print("[TSE_OpenTerminalAction] Init called for laptop: " + pOwnerEntity.GetName());
    }
    
    //---------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        Print("[TSE_OpenTerminalAction] PerformAction called");
        
        if (!CanBePerformedScript(pUserEntity))
        {
            Print("[TSE_OpenTerminalAction] CanBePerformedScript returned false");
            return;
        }
        
        Print("[TSE_OpenTerminalAction] CanBePerformedScript returned true, calling PerformActionInternal");
        PerformActionInternal(pOwnerEntity, pUserEntity);
    }
    
    protected void PerformActionInternal(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        Print("[TSE_OpenTerminalAction] PerformActionInternal started");
        
        // Get UI manager from player
        OVT_UIManagerComponent uiManager = OVT_UIManagerComponent.Cast(pUserEntity.FindComponent(OVT_UIManagerComponent));
        if (!uiManager) 
        {
            Print("[TSE_OpenTerminalAction] ERROR: No UI manager found on player");
            return;
        }
        
        Print("[TSE_OpenTerminalAction] UI manager found");
        
        // Get or create terminal context
        TSE_TerminalContext context = TSE_TerminalContext.Cast(uiManager.GetContext(TSE_TerminalContext));
        if (!context) 
        {
            Print("[TSE_OpenTerminalAction] ERROR: No terminal context found");
            return;
        }
        
        Print("[TSE_OpenTerminalAction] Terminal context found");
        
        // Set laptop entity for context
        context.SetLaptop(m_Laptop);
        
        Print("[TSE_OpenTerminalAction] About to show terminal UI");
        
        // Show terminal UI
        uiManager.ShowContext(TSE_TerminalContext);
        
        Print("[TSE_OpenTerminalAction] ShowContext called");
    }
    
    override bool GetActionNameScript(out string outName)
    {
        return false;
    }
    
    override bool CanBePerformedScript(IEntity user)
    {
        // Check if user has inventory manager (basic player validation)
        Managed inventoryManager = user.FindComponent(SCR_InventoryStorageManagerComponent);
        if (!inventoryManager)
            return false;
        
        // Any player can open the terminal - no owner check needed
        return true;
    }
    
    override bool HasLocalEffectOnlyScript() 
    { 
        return true; 
    }
}