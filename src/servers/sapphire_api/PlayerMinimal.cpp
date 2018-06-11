#include "PlayerMinimal.h"

#include <Util/Util.h>
#include <Common.h>
#include <Exd/ExdDataGenerated.h>

#include <Database/DatabaseDef.h>

extern Core::Data::ExdDataGenerated g_exdDataGen;

namespace Core {

   using namespace Common;

   // player constructor
   PlayerMinimal::PlayerMinimal( void ) : m_id( 0 )
   {


   }

   // deconstructor
   PlayerMinimal::~PlayerMinimal( void )
   {

   }

   // load player from the db
   // TODO change void CPlayer::load to bool, we want to know if something went wrong
   void PlayerMinimal::load( uint32_t charId )
   {

      auto stmt = g_charaDb.getPreparedStatement( Db::CharaDbStatements::CHARA_SEL_MINIMAL );

      stmt->setUInt( 1, charId );
      auto res = g_charaDb.query( stmt );

      if( !res->next() )
         return;

      m_id = charId;

      memset( m_name, 0, 32 );

      strcpy( m_name, res->getString( "Name" ).c_str() );

      auto customize = res->getBlobVector( "Customize" );
      memcpy( (char*)m_look, customize.data(), customize.size() );
      for( int32_t i = 0; i < 26; i++ )
      {
         m_lookMap[i] = m_look[i];
      }

      auto modelEquip = res->getBlobVector( "ModelEquip" );
      memcpy( (char*)m_modelEquip, modelEquip.data(), modelEquip.size() );


      setBirthDay( res->getUInt8( "BirthDay" ), res->getUInt8( "BirthMonth" ) );
      m_guardianDeity = res->getUInt8( "GuardianDeity" );
      m_class = res->getUInt8( "Class" );
      m_contentId = res->getUInt64( "ContentId" );
      m_zoneId = res->getUInt8( "TerritoryId" );

      // SELECT ClassIdx, Exp, Lvl
      auto stmtClass = g_charaDb.getPreparedStatement( Db::CharaDbStatements::CHARA_SEL_MINIMAL );
      stmtClass->setInt( 1, m_id );

      auto resClass = g_charaDb.query( stmt );

      while( resClass->next() )
      {
         m_classMap[resClass->getUInt( 1 )] = resClass->getUInt( 3 );
      }
   }


   std::string PlayerMinimal::getLookString()
   {

      auto it = m_lookMap.begin();

      std::string lookString;

      for( ; it != m_lookMap.end(); ++it )
      {

         std::string s = std::to_string( it->second );

         lookString += "\"" + s + "\"";
         if( it != m_lookMap.end() )
         {
            lookString += ",";
         }
      }

      return lookString.substr( 0, lookString.size() - 1 );
   }

   std::string PlayerMinimal::getModelString()
   {
      std::string modelString = "\""
         + std::to_string( m_modelEquip[2] ) + "\",\""
         + std::to_string( m_modelEquip[3] ) + "\",\""
         + std::to_string( m_modelEquip[4] ) + "\",\""
         + std::to_string( m_modelEquip[6] ) + "\",\""
         + std::to_string( m_modelEquip[7] ) + "\",\"5\",\"6\",\"7\",\"8\",\"9\"";
      return modelString;
   }

   std::string PlayerMinimal::getInfoJson()
   {
      std::string charDetails = "{\"content\":[\"" + std::string( getName() ) + "\"," +
         //"[" + getClassString() + "]," +
         "[\"0\",\"0\",\"0\",\"0\",\"0\",\"1\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\"],"
         "\"0\",\"0\",\"0\",\"" +
                                std::to_string( getBirthMonth() ) +
                                "\",\"" + std::to_string( getBirthDay() ) +
                                "\",\"" + std::to_string( getGuardianDeity() ) +
                                "\",\"" + std::to_string( m_class ) +
                                "\",\"0\",\"" + std::to_string( getZoneId() ) +
                                "\",\"0\"," +

         "[" + getLookString() + "]," +
         "\"0\",\"0\"," +
         "[" + getModelString() + "]," +
         "\"1\",\"0\",\"0\",\"0\",\"0\",\"0\",\"\",\"0\",\"0\"]," +
         "\"classname\":\"ClientSelectData\",\"classid\":116}";
      return charDetails;
   }

   std::string PlayerMinimal::getClassString()
   {

      std::map<uint8_t, uint16_t>::iterator it;

      it = m_classMap.begin();

      std::string classString;

      for( ; it != m_classMap.end(); ++it )
      {

         std::string s = std::to_string( it->second );

         classString += "\"" + s + "\"";
         if( it != m_classMap.end() )
         {
            classString += ",";
         }
      }

      return classString.substr( 0, classString.size() - 1 );
   }

   void PlayerMinimal::saveAsNew()
   {

      std::vector< uint8_t > customize( 26 );
      std::vector< uint8_t > howTo( 33 );
      std::vector< uint8_t > aetherytes( 16 );
      std::vector< uint8_t > discovery( 421 );
      std::vector< uint8_t > questComplete( 396 );
      std::vector< uint8_t > unlocks( 64 );
      std::vector< uint8_t > mountGuide( 15 );
      std::vector< uint8_t > orchestrion( 40 );
      std::vector< uint8_t > modelEquip( 40 );
      std::vector< uint8_t > questTracking8( 10 );
      std::vector< int16_t > questTracking = { -1, -1, -1, -1, -1 };

      memset( questComplete.data(), 0, questComplete.size() );

      memcpy( questTracking8.data(), questTracking.data(), questTracking8.size() );

      for( uint32_t i = 0; i < m_lookMap.size(); i++ )
      {
         customize[i] = m_lookMap[i];
      }

      uint32_t equipModel[10];
      memset( equipModel, 0, 40 );
      memcpy( modelEquip.data(), equipModel, modelEquip.size() );

      uint32_t startZone;
      float x, y, z, o;
      int32_t startTown = 0;

      switch( static_cast< Core::Common::ClassJob >( m_class ) )
      {
      case Core::Common::ClassJob::Conjurer:
      case Core::Common::ClassJob::Lancer:
      case Core::Common::ClassJob::Archer:
         x = 127.0f;
         y = -13.0f;
         z = 147.0f;
         o = -2.1f;
         startZone = 183;
         startTown = 2;
         break;

      case Core::Common::ClassJob::Marauder:
      case Core::Common::ClassJob::Arcanist:
         x = -53.0f;
         y = 18.0f;
         z = 0.0f;
         o = 1.5f;
         startTown = 1;
         startZone = 181;
         break;

      case Core::Common::ClassJob::Thaumaturge:
      case Core::Common::ClassJob::Pugilist:
      case Core::Common::ClassJob::Gladiator:
         x = 42.0f;
         y = 4.0f;
         z = -157.6f;
         o = -0.3f;
         startTown = 3;
         startZone = 182;
         break;
      }

      //        "(AccountId, CharacterId, ContentId, Name, Hp, Mp, "
      //        "Customize, Voice, IsNewGame, TerritoryId, PosX, PosY, PosZ, PosR, ModelEquip, "
      //        "IsNewAdventurer, GuardianDeity, Birthday, BirthMonth, Class, Status, FirstClass, "
      //        "HomePoint, StartTown, Discovery, HowTo, QuestCompleteFlags, Unlocks, QuestTracking, "
      //        "Aetheryte, GMRank, UPDATE_DATE )

      // CharacterId, ClassIdx, Exp, Lvl
      auto stmtClass = g_charaDb.getPreparedStatement( Db::CharaDbStatements::CHARA_CLASS_INS );
      stmtClass->setInt( 1, m_id );
      stmtClass->setInt( 2, g_exdDataGen.get< Core::Data::ClassJob >( m_class )->expArrayIndex );
      stmtClass->setInt( 3, 0 );
      stmtClass->setInt( 4, 1 );
      g_charaDb.directExecute( stmtClass );

      auto stmt = g_charaDb.getPreparedStatement( Db::CharaDbStatements::CHARA_INS );
      stmt->setInt( 1, m_accountId );
      stmt->setInt( 2, m_id );
      stmt->setInt64( 3, m_contentId );
      stmt->setString( 4, std::string( m_name ) );
      stmt->setInt( 5, 100 );
      stmt->setInt( 6, 100 );
      stmt->setBinary( 7, customize );
      stmt->setInt( 8, m_voice );
      stmt->setInt( 9, 1 );
      stmt->setInt( 10, startZone );
      stmt->setDouble( 11, x );
      stmt->setDouble( 12, y );
      stmt->setDouble( 13, z );
      stmt->setDouble( 14, o );
      stmt->setBinary( 15, modelEquip );
      stmt->setInt( 16, 1 );
      stmt->setInt( 17, m_guardianDeity );
      stmt->setInt( 18, m_birthDay );
      stmt->setInt( 19, m_birthMonth );
      stmt->setInt( 20, m_class );
      stmt->setInt( 21, 1 );
      stmt->setInt( 22, m_class );
      stmt->setInt( 23, 2 );
      stmt->setInt( 24, startTown );
      stmt->setBinary( 25, discovery );
      stmt->setBinary( 26, howTo );
      stmt->setBinary( 27, questComplete );
      stmt->setBinary( 28, unlocks );
      stmt->setBinary( 29, questTracking8 );
      stmt->setBinary( 30, aetherytes );
      stmt->setInt( 31, m_gmRank );
      stmt->setBinary( 32, mountGuide );
      stmt->setBinary( 33, orchestrion );
      g_charaDb.directExecute( stmt );

      auto stmtSearchInfo = g_charaDb.getPreparedStatement( Db::CharaDbStatements::CHARA_SEARCHINFO_INS );
      stmtSearchInfo->setInt( 1, m_id );
      g_charaDb.directExecute( stmtSearchInfo );

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /// SET UP INVENTORIES
      createInvDbContainer( InventoryType::Bag0 );
      createInvDbContainer( InventoryType::Bag1 );
      createInvDbContainer( InventoryType::Bag2 );
      createInvDbContainer( InventoryType::Bag3 );

      createInvDbContainer( InventoryType::ArmoryOff );

      createInvDbContainer( InventoryType::ArmoryHead );
      createInvDbContainer( InventoryType::ArmoryBody );
      createInvDbContainer( InventoryType::ArmoryHand );
      createInvDbContainer( InventoryType::ArmoryWaist );
      createInvDbContainer( InventoryType::ArmoryLegs );
      createInvDbContainer( InventoryType::ArmoryFeet );

      createInvDbContainer( InventoryType::ArmotyNeck );
      createInvDbContainer( InventoryType::ArmoryEar );
      createInvDbContainer( InventoryType::ArmoryWrist );
      createInvDbContainer( InventoryType::ArmoryRing );
      createInvDbContainer( InventoryType::ArmoryMain );

      createInvDbContainer( InventoryType::Currency );
      createInvDbContainer( InventoryType::Crystal );

      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /// SETUP EQUIPMENT / STARTING GEAR
      auto classJobInfo = g_exdDataGen.get< Core::Data::ClassJob >( m_class );
      uint32_t weaponId = classJobInfo->itemStartingWeapon;
      uint64_t uniqueId = getNextUId64();

      uint8_t race = customize[CharaLook::Race];
      uint8_t gender = customize[CharaLook::Gender];

      auto raceInfo = g_exdDataGen.get< Core::Data::Race >( race );

      uint32_t body;
      uint32_t hands;
      uint32_t legs;
      uint32_t feet;
      uint64_t bodyUid = getNextUId64();
      uint64_t handsUid = getNextUId64();
      uint64_t legsUid = getNextUId64();
      uint64_t feetUid = getNextUId64();

      if( gender == 0 )
      {
         body = raceInfo->rSEMBody;
         hands = raceInfo->rSEMHands;
         legs = raceInfo->rSEMLegs;
         feet = raceInfo->rSEMFeet;
      }
      else
      {
         body = raceInfo->rSEFBody;
         hands = raceInfo->rSEFHands;
         legs = raceInfo->rSEFLegs;
         feet = raceInfo->rSEFFeet;
      }

      insertDbGlobalItem( weaponId, uniqueId );
      insertDbGlobalItem( body, bodyUid );
      insertDbGlobalItem( hands, handsUid );
      insertDbGlobalItem( legs, legsUid );
      insertDbGlobalItem( feet, feetUid );

      g_charaDb.execute( "INSERT INTO charaitemgearset (storageId, CharacterId, "
         "container_" + std::to_string( EquipSlot::MainHand ) + ", "
         "container_" + std::to_string( EquipSlot::Body ) + ", "
         "container_" + std::to_string( EquipSlot::Hands ) + ", "
         "container_" + std::to_string( EquipSlot::Legs ) + ", "
         "container_" + std::to_string( EquipSlot::Feet ) + ", "
         "UPDATE_DATE ) "
         "VALUES ( " + std::to_string( InventoryType::GearSet0 ) + ", " + std::to_string( m_id ) + ", " +
         std::to_string( uniqueId ) + ", " +
         std::to_string( bodyUid ) + ", " +
         std::to_string( handsUid ) + ", " +
         std::to_string( legsUid ) + ", " +
         std::to_string( feetUid ) + ", NOW());" );

   }

   void PlayerMinimal::insertDbGlobalItem( uint32_t weaponId, uint64_t uniqueId ) const
   {
      auto stmtItemGlobal = g_charaDb.getPreparedStatement( Db::CHARA_ITEMGLOBAL_INS );
      stmtItemGlobal->setInt( 1, m_id );
      stmtItemGlobal->setInt64( 2, uniqueId );
      stmtItemGlobal->setInt( 3, weaponId );
      g_charaDb.directExecute( stmtItemGlobal );
   }

   void PlayerMinimal::createInvDbContainer( uint16_t slot ) const
   {
      auto stmtCreateInv = g_charaDb.getPreparedStatement( Db::CHARA_ITEMINV_INS );
      stmtCreateInv->setInt( 1, m_id );
      stmtCreateInv->setInt( 2, slot );
      g_charaDb.directExecute( stmtCreateInv );
   }

   uint64_t PlayerMinimal::getNextUId64() const
   {
      g_charaDb.directExecute( std::string( "INSERT INTO uniqueiddata( IdName ) VALUES( 'NOT_SET' );" ) );
      auto res = g_charaDb.query( "SELECT LAST_INSERT_ID();" );

      if( !res->next() )
         return 0;

      return res->getUInt64( 1 );
   }
}
