#include <ScriptObject.h>
#include <Actor/Player.h>

class CmnDefWeatherForeCast :
  public EventScript
{
public:
  CmnDefWeatherForeCast() :
    EventScript( 721100 )
  {
  }

  void Scene00000( Entity::Player& player )
  {
    player.playScene( getId(), 0, HIDE_HOTBAR,
                      [ & ]( Entity::Player& player, const Event::SceneResult& result )
                      {
                      } );
  }

  void onTalk( uint32_t eventId, Entity::Player& player, uint64_t actorId ) override
  {
    Scene00000( player );
  }
};