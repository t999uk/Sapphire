#include <ScriptObject.h>
#include <Zone/InstanceContent.h>

class TheHowlingEye :
  public InstanceContentScript
{
public:
  TheHowlingEye() :
    InstanceContentScript( 20003 )
  {
  }

  void onInit( InstanceContentPtr instance ) override
  {
    instance->registerEObj( "sgbg_r1fa_a_rockcol", 2000646, 4256721, 4, { -4.592406f, -1.261766f, 4.724565f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "sgbg_r1fa_a_rockcol_1", 2000647, 4256725, 4, { 7.587256f, -1.233237f, 6.169516f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "sgbg_r1fa_a_rockcol_2", 2000648, 4256726, 4, { -6.329784f, 3.332559f, -6.736721f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "sgbg_r1fa_a_rockcol_3", 2000649, 4256727, 4, { 5.895649f, 3.275768f, -6.217119f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "sgbg_r1fa_a_rockcol_4", 2000646, 4488538, 4, { -7.000000f, 0.000000f, 7.000000f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "sgbg_r1fa_a_rockcol_5", 2000647, 4488545, 4, { 7.000000f, 0.000000f, 7.000000f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "sgbg_r1fa_a_rockcol_6", 2000648, 4488546, 4, { -7.000000f, 0.000000f, -7.000000f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "sgbg_r1fa_a_rockcol_7", 2000649, 4488548, 4, { 7.000000f, 0.000000f, -7.000000f },
                            1.000000f, 0.000000f );
    instance->registerEObj( "Entrance", 2000274, 4246117, 5, { 0.506547f, -1.935848f, 14.110570f }, 1.000000f,
                            0.000000f );
    // States -> vf_lock_on vf_lock_of
    instance->registerEObj( "Exit", 2000275, 0, 4, { 1.078937f, -2.041544f, -18.363190f }, 1.000000f, 0.000000f );

  }

  void onUpdate( InstanceContentPtr instance, uint32_t currTime ) override
  {

  }

  void onEnterTerritory( InstanceContentPtr instance, Entity::Player& player, uint32_t eventId, uint16_t param1,
                         uint16_t param2 ) override
  {

  }

};