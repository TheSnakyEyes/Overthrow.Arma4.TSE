[BaseContainerProps(configRoot : true)]
class TSE_AirDropCargoConfig : ScriptAndConfig
{
    [Attribute("60", UIWidgets.EditBox, desc: "Weapon spawn chance (0-100)")]
    int m_iWeaponsChance;
    
    [Attribute("2", UIWidgets.EditBox, desc: "Minimum weapon count")]
    int m_iWeaponsMinCount;
    
    [Attribute("4", UIWidgets.EditBox, desc: "Maximum weapon count")]
    int m_iWeaponsMaxCount;
    
    [Attribute("80", UIWidgets.EditBox, desc: "Ammunition spawn chance (0-100)")]
    int m_iAmmunitionChance;
    
    [Attribute("4", UIWidgets.EditBox, desc: "Minimum ammunition count")]
    int m_iAmmunitionMinCount;
    
    [Attribute("8", UIWidgets.EditBox, desc: "Maximum ammunition count")]
    int m_iAmmunitionMaxCount;
    
    [Attribute("50", UIWidgets.EditBox, desc: "Medical supplies spawn chance (0-100)")]
    int m_iMedicalChance;
    
    [Attribute("2", UIWidgets.EditBox, desc: "Minimum medical supplies count")]
    int m_iMedicalMinCount;
    
    [Attribute("5", UIWidgets.EditBox, desc: "Maximum medical supplies count")]
    int m_iMedicalMaxCount;
    
    [Attribute("40", UIWidgets.EditBox, desc: "Attachments spawn chance (0-100)")]
    int m_iAttachmentsChance;
    
    [Attribute("1", UIWidgets.EditBox, desc: "Minimum attachments count")]
    int m_iAttachmentsMinCount;
    
    [Attribute("3", UIWidgets.EditBox, desc: "Maximum attachments count")]
    int m_iAttachmentsMaxCount;
    
    [Attribute("30", UIWidgets.EditBox, desc: "Throwables spawn chance (0-100)")]
    int m_iThrowableChance;
    
    [Attribute("1", UIWidgets.EditBox, desc: "Minimum throwables count")]
    int m_iThrowableMinCount;
    
    [Attribute("3", UIWidgets.EditBox, desc: "Maximum throwables count")]
    int m_iThrowableMaxCount;
    
    [Attribute("25", UIWidgets.EditBox, desc: "Explosives spawn chance (0-100)")]
    int m_iExplosivesChance;
    
    [Attribute("1", UIWidgets.EditBox, desc: "Minimum explosives count")]
    int m_iExplosivesMinCount;
    
    [Attribute("2", UIWidgets.EditBox, desc: "Maximum explosives count")]
    int m_iExplosivesMaxCount;
} 