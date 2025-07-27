[ComponentEditorProps(category: "GameScripted/Events", description: "Air Drop Event Manager")]
class TSE_AirDropEventManagerComponentClass : ScriptComponentClass {}

class TSE_AirDropEventManagerComponent : ScriptComponent
{
    ref array<IEntity> m_AirDropSpawnMarkers = new array<IEntity>();

    [Attribute(defvalue: "10", uiwidget: UIWidgets.EditBox, desc: "Crate check interval (minutes)")]
    int m_iCrateCheckIntervalMinutes;

    [Attribute(defvalue: "180", uiwidget: UIWidgets.EditBox, desc: "Time before special forces spawn (minutes)")]
    int m_iSpecialForcesDelayMinutes;

    [Attribute(defvalue: "300", uiwidget: UIWidgets.EditBox, desc: "Occupation success reward")]
    int m_iOccupationSuccessReward;

    float m_fEventStartTime;
    bool m_bEventActive;
    bool m_bSpecialForcesSpawned;
    bool m_bCrateEmpty;
    EntityID m_CrateID;
    vector m_vCratePosition;
    ref array<EntityID> m_SpawnedSpecialForces;
    float m_fLastCrateCheck;
    float m_fNextSpecialForcesSpawn;

    // Configuration override for Event Manager
    ref TSE_AirDropEventConfig m_ManagedConfig;
    bool m_bManagedByEventManager = false;

    // Static variables for crate marker tracking
    static vector s_vActiveCratePos = vector.Zero;
    static bool s_bCrateMarkerVisible = false;

    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        m_SpawnedSpecialForces = new array<EntityID>();
        
        Print("[AirDropEvent] OnPostInit: " + owner.GetName() + " | m_bEventActive=" + m_bEventActive);
        
        // Note: Event Manager will call StartEventFromManager instead of self-starting
        // Legacy behavior disabled to prevent conflicts with central EventManager
        Print("[AirDropEvent] Waiting for central EventManager to start events");
    }

    void WaitForGameModeInitialized()
    {
        OVT_OverthrowGameMode mode = OVT_OverthrowGameMode.Cast(GetGame().GetGameMode());
        if (!mode || !mode.IsInitialized())
        {
            GetGame().GetCallqueue().CallLater(WaitForGameModeInitialized, 1000, false);
            return;
        }
        
        Print("[AirDropEvent] OverthrowGameMode initialized, starting airdrop event logic");
        
        if (m_AirDropSpawnMarkers.IsEmpty())
        {
            Print("[AirDropEvent] No airdrop spawn markers registered!");
        }
        
        // Start event after initialization delay
        GetGame().GetCallqueue().CallLater(TryStartEvent, 600000, false); // 10 minutes
    }

    void TryStartEvent()
    {
        Print("[AirDropEvent] TryStartEvent | m_bEventActive=" + m_bEventActive);
        if (m_bEventActive)
            return;
        StartEvent();
    }

    void StartEventFromManager(TSE_AirDropEventConfig config)
    {
        if (m_bEventActive) {
            Print("[AirDropEvent] StartEventFromManager: already active, skipping");
            return;
        }
        
        Print("[AirDropEvent] Starting event from Event Manager");
        m_bManagedByEventManager = true;
        m_ManagedConfig = config;
        
        if (config)
        {
            m_iCrateCheckIntervalMinutes = config.m_iCrateCheckIntervalMinutes;
            m_iSpecialForcesDelayMinutes = config.m_iSpecialForcesDelayMinutes;
            m_iOccupationSuccessReward = config.m_iOccupationSuccessReward;
        }
        
        StartEvent();
    }

    bool IsEventActive()
    {
        return m_bEventActive;
    }

    void RegisterAirDropSpawnMarker(IEntity marker)
    {
        if (!marker) return;
        if (m_AirDropSpawnMarkers.Contains(marker)) return;
        m_AirDropSpawnMarkers.Insert(marker);
        Print("[AirDropEvent] Registered airdrop spawn marker: " + marker.GetOrigin());
    }

    void StartEvent()
    {
        if (m_bEventActive) {
            Print("[AirDropEvent] StartEvent: already active, skipping");
            return;
        }
        
        if (m_AirDropSpawnMarkers.IsEmpty()) {
            Print("[AirDropEvent] Not enough spawn markers to start event");
            if (!m_bManagedByEventManager)
                GetGame().GetCallqueue().CallLater(TryStartEvent, 3600000, false); // 1 hour
            return;
        }
        
        m_bEventActive = true;
        m_bSpecialForcesSpawned = false;
        m_bCrateEmpty = false;
        m_fEventStartTime = GetGame().GetWorld().GetWorldTime();
        m_fLastCrateCheck = 0;
        m_fNextSpecialForcesSpawn = m_fEventStartTime + (m_iSpecialForcesDelayMinutes * 60);
        
        Print("[AirDropEvent] Starting air drop event");
        
        SpawnAirDropCrate();
        SendInitialNotifications();
        
        // Start monitoring loop
        GetGame().GetCallqueue().CallLater(MonitorEvent, 30000, true); // Check every 30 seconds
    }

    void SpawnAirDropCrate()
    {
        // Get random spawn position
        int idx = Math.RandomInt(0, m_AirDropSpawnMarkers.Count());
        IEntity spawnMarker = m_AirDropSpawnMarkers[idx];
        m_vCratePosition = spawnMarker.GetOrigin();
        
        // Spawn crate
        ResourceName cratePrefab = "{B2658E47B2E284C7}Prefabs/Props/Military/MilitaryCrates/CrateStack_01/Dst/AirDrop_Crate.et";
        EntitySpawnParams spawnParams = EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        spawnParams.Transform[3] = m_vCratePosition;
        
        IEntity crate = GetGame().SpawnEntityPrefab(Resource.Load(cratePrefab), GetGame().GetWorld(), spawnParams);
        if (crate)
        {
            m_CrateID = crate.GetID();
            // Fill crate with supplies
            FillCrateWithSupplies(crate);
            Print("[AirDropEvent] Air drop crate spawned at: " + m_vCratePosition);
            
            // Update static marker position for map display
            s_vActiveCratePos = m_vCratePosition;
            s_bCrateMarkerVisible = true;
        }
        else
        {
            Print("[AirDropEvent] ERROR: Failed to spawn air drop crate!");
            EndEvent();
        }
    }

    void FillCrateWithSupplies(IEntity crate)
    {
        InventoryStorageManagerComponent invMgr = InventoryStorageManagerComponent.Cast(crate.FindComponent(InventoryStorageManagerComponent));
        if (!invMgr) {
            Print("[AirDropEvent] Crate has no InventoryStorageManagerComponent");
            return;
        }
        
        // Use cargo config if available from Event Manager, otherwise use defaults
        TSE_AirDropCargoConfig cargoConfig;
        if (m_ManagedConfig && m_ManagedConfig.m_CargoConfig)
        {
            cargoConfig = m_ManagedConfig.m_CargoConfig;
        }
        else
        {
            // Create default cargo config for legacy compatibility
            cargoConfig = new TSE_AirDropCargoConfig();
            cargoConfig.m_iWeaponsChance = 60;
            cargoConfig.m_iWeaponsMinCount = 2;
            cargoConfig.m_iWeaponsMaxCount = 4;
            cargoConfig.m_iAmmunitionChance = 80;
            cargoConfig.m_iAmmunitionMinCount = 4;
            cargoConfig.m_iAmmunitionMaxCount = 8;
            cargoConfig.m_iMedicalChance = 50;
            cargoConfig.m_iMedicalMinCount = 2;
            cargoConfig.m_iMedicalMaxCount = 5;
        }
        
        OVT_EconomyManagerComponent economy = OVT_Global.GetEconomy();
        if (!economy) {
            Print("[AirDropEvent] Could not get economy manager");
            return;
        }
        
        // Fill with weapons
        FillCrateCategory(invMgr, economy, SCR_EArsenalItemType.RIFLE, SCR_EArsenalItemMode.WEAPON, 
                         cargoConfig.m_iWeaponsChance, cargoConfig.m_iWeaponsMinCount, cargoConfig.m_iWeaponsMaxCount, "Weapons");
        
        // Fill with ammunition
        FillCrateCategory(invMgr, economy, SCR_EArsenalItemType.RIFLE, SCR_EArsenalItemMode.AMMUNITION, 
                         cargoConfig.m_iAmmunitionChance, cargoConfig.m_iAmmunitionMinCount, cargoConfig.m_iAmmunitionMaxCount, "Ammunition");
        
        // Fill with medical supplies
        FillCrateCategory(invMgr, economy, SCR_EArsenalItemType.HEAL, SCR_EArsenalItemMode.DEFAULT, 
                         cargoConfig.m_iMedicalChance, cargoConfig.m_iMedicalMinCount, cargoConfig.m_iMedicalMaxCount, "Medical");
        
        // Fill with attachments
        FillCrateCategory(invMgr, economy, SCR_EArsenalItemType.WEAPON_ATTACHMENT, SCR_EArsenalItemMode.DEFAULT, 
                         cargoConfig.m_iAttachmentsChance, cargoConfig.m_iAttachmentsMinCount, cargoConfig.m_iAttachmentsMaxCount, "Attachments");
        
        // Fill with throwables (grenades)
        FillCrateCategory(invMgr, economy, SCR_EArsenalItemType.LETHAL_THROWABLE, SCR_EArsenalItemMode.DEFAULT, 
                         cargoConfig.m_iThrowableChance, cargoConfig.m_iThrowableMinCount, cargoConfig.m_iThrowableMaxCount, "Throwables");
        
        // Fill with explosives
        FillCrateCategory(invMgr, economy, SCR_EArsenalItemType.EXPLOSIVES, SCR_EArsenalItemMode.DEFAULT, 
                         cargoConfig.m_iExplosivesChance, cargoConfig.m_iExplosivesMinCount, cargoConfig.m_iExplosivesMaxCount, "Explosives");
        
        Print("[AirDropEvent] Air drop crate filled with supplies using cargo configuration");
    }

    void FillCrateCategory(InventoryStorageManagerComponent invMgr, OVT_EconomyManagerComponent economy, 
                          SCR_EArsenalItemType itemType, SCR_EArsenalItemMode itemMode, 
                          int chance, int minCount, int maxCount, string categoryName)
    {
        // Check spawn chance
        if (Math.RandomInt(0, 100) >= chance) {
            Print("[AirDropEvent] Skipping " + categoryName + " category due to chance roll");
            return;
        }
        
        // Get faction-specific items
        array<SCR_EntityCatalogEntry> availableItems = new array<SCR_EntityCatalogEntry>();
        if (!GetSupportingFactionItems(economy, itemType, itemMode, availableItems)) {
            Print("[AirDropEvent] No items found for category: " + categoryName);
            return;
        }
        
        if (availableItems.IsEmpty()) {
            Print("[AirDropEvent] No available items for category: " + categoryName);
            return;
        }
        
        // Calculate random count
        int count = Math.RandomInt(minCount, maxCount + 1);
        
        // Spawn items
        for (int i = 0; i < count; i++) {
            // Pick random item from available
            SCR_EntityCatalogEntry randomItem = availableItems[Math.RandomInt(0, availableItems.Count())];
            ResourceName prefab = randomItem.GetPrefab();
            
            invMgr.TrySpawnPrefabToStorage(prefab, null, -1, EStoragePurpose.PURPOSE_ANY, null, 1);
        }
        
        Print("[AirDropEvent] Added " + count + " items to " + categoryName + " category");
    }

    bool GetSupportingFactionItems(OVT_EconomyManagerComponent economy, SCR_EArsenalItemType itemType, SCR_EArsenalItemMode itemMode, out array<SCR_EntityCatalogEntry> items)
    {
        // Get all items of the specified type
        array<SCR_EntityCatalogEntry> allItems = new array<SCR_EntityCatalogEntry>();
        if (!economy.FindInventoryItems(itemType, itemMode, "", allItems)) {
            return false;
        }
        
        // Filter to only supporting faction items
        int supportingFactionIndex = OVT_Global.GetConfig().GetSupportingFactionIndex();
        
        foreach (SCR_EntityCatalogEntry entry : allItems) {
            ResourceName prefab = entry.GetPrefab();
            int itemId = economy.GetInventoryId(prefab);
            
            // Check if item belongs to supporting faction
            if (economy.ItemIsFromFaction(itemId, supportingFactionIndex)) {
                items.Insert(entry);
            }
        }
        
        return !items.IsEmpty();
    }

    void SendInitialNotifications()
    {
        string nearestTownName = OVT_Global.GetTowns().GetTownName(m_vCratePosition);
        float distance = vector.Distance(m_vCratePosition, OVT_Global.GetTowns().GetNearestTown(m_vCratePosition).location);
        
        // Calculate direction from nearest town
        vector townPos = OVT_Global.GetTowns().GetNearestTown(m_vCratePosition).location;
        vector direction = m_vCratePosition - townPos;
        direction.Normalize();
        
        string directionText = GetDirectionText(direction);
        string distanceText = string.Format("%.0f", distance);
        
        // Check if nearest radio tower belongs to resistance
        OVT_RadioTowerData nearestTower = OVT_Global.GetOccupyingFaction().GetNearestRadioTower(m_vCratePosition);
        bool showExactLocation = false;
        
        if (nearestTower)
        {
            // If tower doesn't belong to occupying faction, it belongs to resistance
            showExactLocation = !nearestTower.IsOccupyingFaction();
        }
        
        if (showExactLocation)
        {
            // Resistance controls tower - send exact location notification
            OVT_Global.GetNotify().SendTextNotification("TSE_AirDropEventStartExact", -1, nearestTownName, directionText, distanceText);
            OVT_Global.GetNotify().SendExternalNotifications("TSE_AirDropEventStartExact", nearestTownName, directionText, distanceText);
            Print("[AirDropEvent] Resistance controls nearest tower - sent exact location notification");
        }
        else
        {
            // Occupying faction controls tower or no tower - send approximate location notification
            OVT_Global.GetNotify().SendTextNotification("TSE_AirDropEventStart", -1, nearestTownName, directionText, distanceText);
            OVT_Global.GetNotify().SendExternalNotifications("TSE_AirDropEventStart", nearestTownName, directionText, distanceText);
            Print("[AirDropEvent] Occupying faction controls tower - sent approximate location notification");
        }
    }

    string GetDirectionText(vector direction)
    {
        // Convert direction vector to compass direction
        float angle = Math.Atan2(direction[0], direction[2]) * Math.RAD2DEG;
        if (angle < 0) angle += 360;
        
        if (angle >= 337.5 || angle < 22.5) return "North";
        if (angle >= 22.5 && angle < 67.5) return "North-East";
        if (angle >= 67.5 && angle < 112.5) return "East";
        if (angle >= 112.5 && angle < 157.5) return "South-East";
        if (angle >= 157.5 && angle < 202.5) return "South";
        if (angle >= 202.5 && angle < 247.5) return "South-West";
        if (angle >= 247.5 && angle < 292.5) return "West";
        if (angle >= 292.5 && angle < 337.5) return "North-West";
        return "North";
    }

    void MonitorEvent()
    {
        if (!m_bEventActive) return;
        
        float currentTime = GetGame().GetWorld().GetWorldTime();
        
        // Check if crate still exists
        IEntity crate = GetGame().GetWorld().FindEntityByID(m_CrateID);
        if (!crate)
        {
            Print("[AirDropEvent] Crate was destroyed or removed - ending event");
            EndEvent();
            return;
        }
        
        // Check crate contents every interval
        if (currentTime - m_fLastCrateCheck >= (m_iCrateCheckIntervalMinutes * 60))
        {
            CheckCrateContents();
            m_fLastCrateCheck = currentTime;
        }
        
        // Spawn special forces after delay
        if (!m_bSpecialForcesSpawned && currentTime >= m_fNextSpecialForcesSpawn)
        {
            SpawnSpecialForces();
        }
        
        // Check event timeout (6 hours total)
        if (currentTime - m_fEventStartTime >= (6 * 3600))
        {
            EndEventTimeout();
        }
    }

    void CheckCrateContents()
    {
        IEntity crate = GetGame().GetWorld().FindEntityByID(m_CrateID);
        if (!crate) return;
        
        InventoryStorageManagerComponent invMgr = InventoryStorageManagerComponent.Cast(crate.FindComponent(InventoryStorageManagerComponent));
        if (!invMgr) return;
        
        // Check if crate is empty
        array<BaseInventoryStorageComponent> storages = new array<BaseInventoryStorageComponent>();
        invMgr.GetStorages(storages, EStoragePurpose.PURPOSE_ANY);
        
        bool isEmpty = true;
        foreach (BaseInventoryStorageComponent storage : storages)
        {
            array<IEntity> items = new array<IEntity>();
            storage.GetAll(items);
            if (!items.IsEmpty())
            {
                isEmpty = false;
                break;
            }
        }
        
        if (isEmpty)
        {
            m_bCrateEmpty = true;
            Print("[AirDropEvent] Crate is empty - resistance victory!");
            
            // Send victory notification
            OVT_Global.GetNotify().SendTextNotification("TSE_AirDropEventResistanceVictory", -1);
            OVT_Global.GetNotify().SendExternalNotifications("TSE_AirDropEventResistanceVictory");
            
            EndEvent();
        }
    }

    void SpawnSpecialForces()
    {
        m_bSpecialForcesSpawned = true;
        
        OVT_OverthrowConfigComponent config = OVT_Global.GetConfig();
        if (!config) return;
        
        OVT_Faction occupyingFaction = config.GetOccupyingFaction();
        if (!occupyingFaction) return;
        
        int numSquads = 2;
        if (m_ManagedConfig)
            numSquads = m_ManagedConfig.m_iSpecialForcesSquads;
        
        float spawnDistance = 800;
        if (m_ManagedConfig)
            spawnDistance = m_ManagedConfig.m_fSpecialForcesSpawnDistance;
        
        for (int i = 0; i < numSquads; i++)
        {
            // Calculate spawn position around crate
            float angle = (360.0 / numSquads) * i;
            vector spawnOffset = Vector(Math.Sin(angle * Math.DEG2RAD) * spawnDistance, 0, Math.Cos(angle * Math.DEG2RAD) * spawnDistance);
            vector spawnPos = m_vCratePosition + spawnOffset;
            
            // Adjust to ground level
            float surfaceY = GetGame().GetWorld().GetSurfaceY(spawnPos[0], spawnPos[2]);
            spawnPos[1] = surfaceY;
            
            // Get special forces group prefab (same as Job Assassinate Officer)
            ResourceName groupPrefab = occupyingFaction.GetGroupPrefabByName("special_forces");
            
            if (!groupPrefab.IsEmpty())
            {
                // Spawn group using OVT_Global helper (same as Job system)
                IEntity entity = OVT_Global.SpawnEntityPrefab(groupPrefab, spawnPos);
                
                if (entity)
                {
                    m_SpawnedSpecialForces.Insert(entity.GetID());
                    
                    SCR_AIGroup group = SCR_AIGroup.Cast(entity);
                    if (group)
                    {
                        // Give patrol waypoints to defend the crate position (same as Job system)
                        config.GivePatrolWaypoints(group, OVT_PatrolType.DEFEND, m_vCratePosition);
                        Print("[AirDropEvent] Special forces squad assigned to defend crate");
                    }
                    
                    Print("[AirDropEvent] Spawned special forces squad " + i + " at: " + spawnPos);
                }
                else
                {
                    Print("[AirDropEvent] ERROR: Failed to spawn special forces squad " + i);
                }
            }
            else
            {
                Print("[AirDropEvent] ERROR: Could not find special_forces group prefab");
            }
        }
        
        // Send notification about special forces
        OVT_RadioTowerData nearestTower = OVT_Global.GetOccupyingFaction().GetNearestRadioTower(m_vCratePosition);
        if (nearestTower && !nearestTower.IsOccupyingFaction())
        {
            // Resistance controls nearby tower - send warning about special forces
            OVT_Global.GetNotify().SendTextNotification("TSE_AirDropEventSpecialForces", -1);
            OVT_Global.GetNotify().SendExternalNotifications("TSE_AirDropEventSpecialForces");
            Print("[AirDropEvent] Resistance controls tower - sent special forces warning notification");
        }
        else
        {
            Print("[AirDropEvent] Occupying faction controls tower - no special forces warning sent to resistance");
        }
        
                Print("[AirDropEvent] Special forces squads spawned and moving to crate");
    }
    

    
    void EndEventTimeout()
    {
        Print("[AirDropEvent] Event timeout - checking final state");
        
        if (!m_bCrateEmpty)
        {
            // Crate not empty - occupation victory
            OVT_Global.GetOccupyingFaction().m_iResources += m_iOccupationSuccessReward;
            
            OVT_Global.GetNotify().SendTextNotification("TSE_AirDropEventOccupationVictory", -1, string.Format("%1", m_iOccupationSuccessReward));
            OVT_Global.GetNotify().SendExternalNotifications("TSE_AirDropEventOccupationVictory", string.Format("%1", m_iOccupationSuccessReward));
        }
        
        EndEvent();
    }

    void EndEvent()
    {
        if (!m_bEventActive) return;
        
        m_bEventActive = false;
        
        // Clear static marker
        s_bCrateMarkerVisible = false;
        s_vActiveCratePos = vector.Zero;
        
        // Clean up crate
        IEntity crate = GetGame().GetWorld().FindEntityByID(m_CrateID);
        if (crate)
        {
            SCR_EntityHelper.DeleteEntityAndChildren(crate);
        }
        
        // Clean up special forces
        foreach (EntityID id : m_SpawnedSpecialForces)
        {
            IEntity entity = GetGame().GetWorld().FindEntityByID(id);
            if (entity)
            {
                SCR_EntityHelper.DeleteEntityAndChildren(entity);
            }
        }
        m_SpawnedSpecialForces.Clear();
        
        // Stop monitoring
        GetGame().GetCallqueue().Remove(MonitorEvent);
        
        Print("[AirDropEvent] Air drop event ended");
        
        // Schedule next event if not managed by Event Manager
        if (!m_bManagedByEventManager)
        {
            int nextEventDelay = 4 * 3600 * 1000; // 4 hours in milliseconds
            GetGame().GetCallqueue().CallLater(TryStartEvent, nextEventDelay, false);
        }
    }

    // Static methods for map integration
    static vector GetActiveCratePosition()
    {
        return s_vActiveCratePos;
    }

    static bool IsCrateMarkerVisible()
    {
        return s_bCrateMarkerVisible;
    }
} 
