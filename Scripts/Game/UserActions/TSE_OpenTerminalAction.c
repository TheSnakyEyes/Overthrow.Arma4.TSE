class TSE_OpenTerminalAction : ScriptedUserAction
{
    IEntity m_Laptop;
    
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        m_Laptop = pOwnerEntity;
    }
    
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        if (!CanBePerformedScript(pUserEntity))
            return;
            
        PerformActionInternal(pOwnerEntity, pUserEntity);
    }
    
    protected void PerformActionInternal(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        OVT_UIManagerComponent uiManager = OVT_UIManagerComponent.Cast(pUserEntity.FindComponent(OVT_UIManagerComponent));
        if (uiManager)
        {
            TSE_TerminalContext context = TSE_TerminalContext.Cast(uiManager.GetContextByString("OverthrowTerminalContext"));
            if (context)
            {
                context.SetLaptop(m_Laptop);
                uiManager.ShowContext(TSE_TerminalContext);
                return;
            }
            
            context = TSE_TerminalContext.Cast(uiManager.GetContext(TSE_TerminalContext));
            if (context)
            {
                context.SetLaptop(m_Laptop);
                uiManager.ShowContext(TSE_TerminalContext);
                return;
            }
        }
    }
    
    override bool CanBePerformedScript(IEntity user)
    {
        Managed inventoryManager = user.FindComponent(SCR_InventoryStorageManagerComponent);
        return (inventoryManager != null);
    }
    
    override bool HasLocalEffectOnlyScript()
    {
        return true;
    }
}
