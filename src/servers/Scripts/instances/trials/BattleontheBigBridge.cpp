#include <ScriptObject.h>
#include <Zone/InstanceContent.h>

class BattleontheBigBridge : public InstanceContentScript
{
public:
   BattleontheBigBridge() : InstanceContentScript( 20021 )
   { }

   void onInit( InstanceContentPtr instance ) override
   {
      instance->registerEObj( "unknown_0", 2002735, 0, 4, { 106.355499f, -5.000003f, -0.049812f }, 0.404395f, -1.570451f ); 
      instance->registerEObj( "sgvf_w_lvd_b0118", 2002872, 4630952, 4, { 109.916496f, -5.000003f, -0.035030f }, 1.000000f, 0.000000f ); 
      // States -> vf_bextwall_on vf_bextwall_of 
      instance->registerEObj( "unknown_1", 2002735, 0, 4, { -75.653900f, 1.999997f, 0.149403f }, 0.500000f, -1.570451f ); 
      instance->registerEObj( "sgvf_w_lvd_b0118_1", 2002872, 4635071, 4, { -72.579193f, 1.999997f, 0.830825f }, 1.000000f, 0.000000f ); 
      // States -> vf_bextwall_on vf_bextwall_of 
      instance->registerEObj( "sgvf_w_lvd_b0118_2", 2003902, 4635078, 4, { -37.106110f, 2.000000f, 1.552188f }, 1.000000f, 0.000000f ); 
      // States -> vf_bextwall_on vf_bextwall_of 
      instance->registerEObj( "Entrance", 2000182, 4630929, 5, { -141.564804f, 1.314597f, 3.459962f }, 1.000000f, 0.000000f ); 
      // States -> vf_lock_on vf_lock_of 
      instance->registerEObj( "Exit", 2000139, 0, 4, { 143.814301f, -5.000000f, -0.081344f }, 1.000000f, 0.000000f ); 
      instance->registerEObj( "Shortcut", 2000700, 0, 4, { -136.750107f, 1.329580f, 5.374416f }, 1.000000f, 0.000000f ); 

   }

   void onUpdate( InstanceContentPtr instance, uint32_t currTime ) override
   {

   }

   void onEnterTerritory( InstanceContentPtr instance, Entity::Player& player, uint32_t eventId, uint16_t param1,
                             uint16_t param2 ) override
   {

   }

};