#include <ScriptObject.h>
#include <Zone/InstanceContent.h>

class TheFinalCoilofBahamutTurn4 : public InstanceContentScript
{
public:
   TheFinalCoilofBahamutTurn4() : InstanceContentScript( 30019 )
   { }

   void onInit( InstanceContentPtr instance ) override
   {
      instance->registerEObj( "unknown_0", 2004578, 0, 4, { 452.681213f, 0.000000f, 6.016883f }, 1.000000f, 0.000000f ); 
      instance->registerEObj( "unknown_1", 2004579, 0, 4, { 450.366699f, -0.000000f, 5.948895f }, 1.000000f, 0.000000f ); 
      instance->registerEObj( "unknown_2", 2004580, 0, 4, { 443.205688f, 0.000000f, 4.024448f }, 1.000000f, 0.000000f ); 
      instance->registerEObj( "Entrance", 2000182, 4895882, 5, { 450.000000f, 0.000000f, 21.250090f }, 1.000000f, 0.000000f ); 
      // States -> vf_lock_on vf_lock_of 
      instance->registerEObj( "unknown_3", 2004785, 5039563, 4, { 450.000000f, 0.000000f, 0.000000f }, 1.000000f, 0.000000f ); 
      instance->registerEObj( "unknown_4", 2004649, 5018241, 4, { 448.419403f, 0.000000f, 4.765350f }, 1.000000f, 0.000000f ); 
      instance->registerEObj( "Exit", 2004651, 0, 4, { 450.000000f, 0.000000f, 29.273590f }, 1.000000f, 0.000000f ); 

   }

   void onUpdate( InstanceContentPtr instance, uint32_t currTime ) override
   {

   }

   void onEnterTerritory( InstanceContentPtr instance, Entity::Player& player, uint32_t eventId, uint16_t param1,
                             uint16_t param2 ) override
   {

   }

};