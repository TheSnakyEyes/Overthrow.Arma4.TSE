[ComponentEditorProps(category: "GameScripted/Events", description: "Air Drop Spawn Point")]
class TSE_AirDropSpawnComponentClass : ScriptComponentClass {}

class TSE_AirDropSpawnComponent : ScriptComponent
{
    protected int m_iRegistrationAttempts = 0;
    protected static const int MAX_REGISTRATION_ATTEMPTS = 10;
    
    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        
        Print("[AirDropSpawn] OnPostInit called for spawn marker at: " + owner.GetOrigin());
        
        // Wait for game mode to be ready before registering
        GetGame().GetCallqueue().CallLater(RegisterWithEventManager, 8000, false);
    }
    
    void RegisterWithEventManager()
    {
        m_iRegistrationAttempts++;
        
        if (m_iRegistrationAttempts > MAX_REGISTRATION_ATTEMPTS)
        {
            Print("[AirDropSpawn] ERROR: Failed to register after " + MAX_REGISTRATION_ATTEMPTS + " attempts. Giving up.");
            return;
        }
        
        Print("[AirDropSpawn] Registration attempt " + m_iRegistrationAttempts + "/" + MAX_REGISTRATION_ATTEMPTS);
        
        OVT_OverthrowGameMode gameMode = OVT_OverthrowGameMode.Cast(GetGame().GetGameMode());
        if (!gameMode)
        {
            Print("[AirDropSpawn] Game mode not ready, retrying...");
            GetGame().GetCallqueue().CallLater(RegisterWithEventManager, 1000, false);
            return;
        }
        
        Print("[AirDropSpawn] Game mode found, looking for AirDrop Event Manager...");
        
        TSE_AirDropEventManagerComponent eventManager = TSE_AirDropEventManagerComponent.Cast(gameMode.FindComponent(TSE_AirDropEventManagerComponent));
        if (eventManager)
        {
            eventManager.RegisterAirDropSpawnMarker(GetOwner());
            Print("[AirDropSpawn] Successfully registered AirDrop spawn marker at: " + GetOwner().GetOrigin());
        }
        else
        {
            Print("[AirDropSpawn] ERROR: Could not find AirDropEventManagerComponent! Retrying in 3 seconds...");
            Print("[AirDropSpawn] DEBUG: Game mode type is: " + gameMode.Type().ToString());
            
            GetGame().GetCallqueue().CallLater(RegisterWithEventManager, 3000, false);
        }
    }
} 