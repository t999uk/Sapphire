#include <Common.h>
#include <Logging/Logger.h>
#include <Network/CommonActorControl.h>

#include "Zone/Zone.h"

#include "Network/PacketWrappers/ActorControlPacket142.h"
#include "Network/PacketWrappers/ActorControlPacket143.h"
#include "Network/PacketWrappers/UpdateInventorySlotPacket.h"

#include "Inventory/Item.h"
#include "Inventory/ItemContainer.h"
#include "Inventory/ItemUtil.h"

#include "Player.h"
#include "Framework.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/clamp.hpp>

#include <Network/PacketDef/Zone/ServerZoneDef.h>

#include <Exd/ExdDataGenerated.h>
#include <Logging/Logger.h>
#include <Database/DatabaseDef.h>

#include "Actor/Player.h"

#include "Network/PacketWrappers/ServerNoticePacket.h"
#include "Network/PacketWrappers/ActorControlPacket143.h"


#include "Framework.h"
#include <Network/CommonActorControl.h>

extern Core::Framework g_fw;

using namespace Core::Common;
using namespace Core::Network::Packets;
using namespace Core::Network::Packets::Server;
using namespace Core::Network::ActorControl;


void Core::Entity::Player::initInventory()
{
  auto setupContainer = [ this ]( InventoryType type, uint8_t maxSize, const std::string& tableName,
                                  bool isMultiStorage, bool isPersistentStorage = true )
  { m_storageMap[ type ] = make_ItemContainer( type, maxSize, tableName, isMultiStorage, isPersistentStorage ); };

  // main bags
  setupContainer( Bag0, 34, "charaiteminventory", true );
  setupContainer( Bag1, 34, "charaiteminventory", true );
  setupContainer( Bag2, 34, "charaiteminventory", true );
  setupContainer( Bag3, 34, "charaiteminventory", true );

  // gear set
  setupContainer( GearSet0, 13, "charaitemgearset", true );

  // gil contianer
  setupContainer( Currency, 11, "charaiteminventory", true );

  // crystals??
  setupContainer( Crystal, 11, "charaiteminventory", true );

  // armory weapons - 0
  setupContainer( ArmoryMain, 34, "charaiteminventory", true );

  // armory offhand - 1
  setupContainer( ArmoryOff, 34, "charaiteminventory", true );

  //armory head - 2
  setupContainer( ArmoryHead, 34, "charaiteminventory", true );

  //armory body - 3
  setupContainer( ArmoryBody, 34, "charaiteminventory", true );

  //armory hand - 4
  setupContainer( ArmoryHand, 34, "charaiteminventory", true );

  //armory waist - 5
  setupContainer( ArmoryWaist, 34, "charaiteminventory", true );

  //armory legs - 6
  setupContainer( ArmoryLegs, 34, "charaiteminventory", true );

  //armory feet - 7
  setupContainer( ArmoryFeet, 34, "charaiteminventory", true );

  //neck
  setupContainer( ArmoryNeck, 34, "charaiteminventory", true );

  //earring
  setupContainer( ArmoryEar, 34, "charaiteminventory", true );

  //wrist
  setupContainer( ArmoryWrist, 34, "charaiteminventory", true );

  //armory rings - 11
  setupContainer( ArmoryRing, 34, "charaiteminventory", true );

  //soul crystals - 13
  setupContainer( ArmorySoulCrystal, 34, "charaiteminventory", true );

  // item hand in container
  // non-persistent container, will not save its contents
  setupContainer( HandIn, 10, "", true, false );

  loadInventory();

}

void Core::Entity::Player::sendItemLevel()
{
  queuePacket( makeActorControl142( getId(), SetItemLevel, getItemLevel(), 0 ) );
}

void Core::Entity::Player::equipWeapon( ItemPtr pItem )
{
  auto exdData = g_fw.get< Core::Data::ExdDataGenerated >();
  if( !exdData )
    return;

  auto itemInfo = exdData->get< Core::Data::Item >( pItem->getId() );
  auto itemClassJob = itemInfo->classJobUse;

  auto currentClass = getClass();
  auto newClassJob = static_cast< ClassJob >( itemClassJob );

  if( isClassJobUnlocked( newClassJob ) )
    return;

  // todo: check if soul crystal is equipped and use job instead

  setClassJob( newClassJob );
}

// equip an item
void Core::Entity::Player::equipItem( Common::GearSetSlot equipSlotId, ItemPtr pItem, bool sendUpdate )
{

  //g_framework.getLogger().debug( "Equipping into slot " + std::to_string( equipSlotId ) );

  updateModels( equipSlotId, pItem );

  if( sendUpdate )
  {
    this->sendModel();
    m_itemLevel = calculateEquippedGearItemLevel();
    sendItemLevel();
  }
}

void Core::Entity::Player::updateModels( GearSetSlot equipSlotId, const Core::ItemPtr& pItem )
{
  uint64_t model = pItem->getModelId1();
  uint64_t model2 = pItem->getModelId2();

  switch( equipSlotId )
  {
    case MainHand:
      m_modelMainWeapon = model;
      m_modelSubWeapon = model2;
      // TODO: add job change upon changing weapon if needed
      // equipWeapon( pItem );
      break;

    case OffHand:
      m_modelSubWeapon = model;
      break;

    case SoulCrystal:
      // TODO: add Job change on equipping crystal
      // change job
      break;

    case Waist:
      break;

    default: // any other slot
      auto modelSlot = equipSlotToModelSlot( equipSlotId );
      if( modelSlot == GearModelSlot::ModelInvalid )
        break;
      m_modelEquip[ static_cast< uint8_t >( modelSlot ) ] = static_cast< uint32_t >( model );
      break;

  }
}

Core::Common::GearModelSlot Core::Entity::Player::equipSlotToModelSlot( Common::GearSetSlot slot )
{
  switch( slot )
  {
    case MainHand:
    case OffHand:
    case Waist:
    case SoulCrystal:
    default:
      return GearModelSlot::ModelInvalid;
    case Head:
      return GearModelSlot::ModelHead;
    case Body:
      return GearModelSlot::ModelBody;
    case Hands:
      return GearModelSlot::ModelHands;
    case Legs:
      return GearModelSlot::ModelLegs;
    case Feet:
      return GearModelSlot::ModelFeet;
    case Neck:
      return GearModelSlot::ModelNeck;
    case Ear:
      return GearModelSlot::ModelEar;
    case Wrist:
      return GearModelSlot::ModelWrist;
    case Ring1:
      return GearModelSlot::ModelRing1;
    case Ring2:
      return GearModelSlot::ModelRing2;
  }
}

void Core::Entity::Player::unequipItem( Common::GearSetSlot equipSlotId, ItemPtr pItem )
{
  auto modelSlot = equipSlotToModelSlot( equipSlotId );
  if( modelSlot != GearModelSlot::ModelInvalid )
    m_modelEquip[ static_cast< uint8_t >( modelSlot ) ] = 0;
  sendModel();

  m_itemLevel = calculateEquippedGearItemLevel();
  sendItemLevel();
}

// TODO: these next functions are so similar that they could likely be simplified
void Core::Entity::Player::addCurrency( CurrencyType type, uint32_t amount )
{
  auto slot = static_cast< uint8_t >( static_cast< uint8_t >( type ) - 1 );
  auto currItem = m_storageMap[ Currency ]->getItem( slot );

  if( !currItem )
  {
    // TODO: map currency type to itemid
    currItem = createItem( 1 );
    m_storageMap[ Currency ]->setItem( slot, currItem );
  }

  uint32_t currentAmount = currItem->getStackSize();
  currItem->setStackSize( currentAmount + amount );
  writeItem( currItem );

  updateContainer( Currency, slot, currItem );

  auto invUpdate = boost::make_shared< UpdateInventorySlotPacket >( getId(),
                                                                    static_cast< uint8_t >( type ) - 1,
                                                                    Common::InventoryType::Currency,
                                                                    *currItem );
  queuePacket( invUpdate );
}

void Core::Entity::Player::removeCurrency( Common::CurrencyType type, uint32_t amount )
{

  auto currItem = m_storageMap[ Currency ]->getItem( static_cast< uint8_t >( type ) - 1 );

  if( !currItem )
    return;

  uint32_t currentAmount = currItem->getStackSize();
  if( amount > currentAmount )
    currItem->setStackSize( 0 );
  else
    currItem->setStackSize( currentAmount - amount );
  writeItem( currItem );

  auto invUpdate = boost::make_shared< UpdateInventorySlotPacket >( getId(),
                                                                    static_cast< uint8_t >( type ) - 1,
                                                                    Common::InventoryType::Currency,
                                                                    *currItem );
  queuePacket( invUpdate );
}


void Core::Entity::Player::addCrystal( Common::CrystalType type, uint32_t amount )
{
  auto currItem = m_storageMap[ Crystal ]->getItem( static_cast< uint8_t >( type ) - 1 );

  if( !currItem )
  {
    // TODO: map currency type to itemid
    currItem = createItem( static_cast< uint8_t >( type ) + 1 );
    m_storageMap[ Crystal ]->setItem( static_cast< uint8_t >( type ) - 1, currItem );
  }

  uint32_t currentAmount = currItem->getStackSize();

  currItem->setStackSize( currentAmount + amount );

  writeItem( currItem );

  writeInventory( Crystal );


  auto invUpdate = boost::make_shared< UpdateInventorySlotPacket >( getId(),
                                                                    static_cast< uint8_t >( type ) - 1,
                                                                    Common::InventoryType::Crystal,
                                                                    *currItem );
  queuePacket( invUpdate );
  queuePacket( makeActorControl143( getId(), ItemObtainIcon, static_cast< uint8_t >( type ) + 1, amount ) );
}

void Core::Entity::Player::removeCrystal( Common::CrystalType type, uint32_t amount )
{
  auto currItem = m_storageMap[ Crystal ]->getItem( static_cast< uint8_t >( type ) - 1 );

  if( !currItem )
    return;

  uint32_t currentAmount = currItem->getStackSize();
  if( amount > currentAmount )
    currItem->setStackSize( 0 );
  else
    currItem->setStackSize( currentAmount - amount );

  writeItem( currItem );

  auto invUpdate = boost::make_shared< UpdateInventorySlotPacket >( getId(),
                                                                    static_cast< uint8_t >( type ) - 1,
                                                                    Common::InventoryType::Crystal,
                                                                    *currItem );
  queuePacket( invUpdate );
}

void Core::Entity::Player::sendInventory()
{
  InventoryMap::iterator it;

  int32_t count = 0;
  for( it = m_storageMap.begin(); it != m_storageMap.end(); ++it, count++ )
  {

    auto pMap = it->second->getItemMap();
    auto itM = pMap.begin();

    for( ; itM != pMap.end(); ++itM )
    {
      if( !itM->second )
        return;

      if( it->second->getId() == InventoryType::Currency || it->second->getId() == InventoryType::Crystal )
      {
        auto currencyInfoPacket = makeZonePacket< FFXIVIpcCurrencyCrystalInfo >( getId() );
        currencyInfoPacket->data().sequence = count;
        currencyInfoPacket->data().catalogId = itM->second->getId();
        currencyInfoPacket->data().unknown = 1;
        currencyInfoPacket->data().quantity = itM->second->getStackSize();
        currencyInfoPacket->data().containerId = it->second->getId();
        currencyInfoPacket->data().slot = 0;
        queuePacket( currencyInfoPacket );
      }
      else
      {
        auto itemInfoPacket = makeZonePacket< FFXIVIpcItemInfo >( getId() );
        itemInfoPacket->data().sequence = count;
        itemInfoPacket->data().containerId = it->second->getId();
        itemInfoPacket->data().slot = itM->first;
        itemInfoPacket->data().quantity = itM->second->getStackSize();
        itemInfoPacket->data().catalogId = itM->second->getId();
        itemInfoPacket->data().condition = itM->second->getDurability();
        itemInfoPacket->data().spiritBond = 0;
        itemInfoPacket->data().hqFlag = itM->second->isHq() ? 1 : 0;
        itemInfoPacket->data().stain = itM->second->getStain();
        queuePacket( itemInfoPacket );
      }
    }

    auto containerInfoPacket = makeZonePacket< FFXIVIpcContainerInfo >( getId() );
    containerInfoPacket->data().sequence = count;
    containerInfoPacket->data().numItems = it->second->getEntryCount();
    containerInfoPacket->data().containerId = it->second->getId();
    queuePacket( containerInfoPacket );


  }

}

Core::Entity::Player::InvSlotPairVec Core::Entity::Player::getSlotsOfItemsInInventory( uint32_t catalogId )
{
  InvSlotPairVec outVec;
  for( auto i : { Bag0, Bag1, Bag2, Bag3 } )
  {
    auto inv = m_storageMap[ i ];
    for( auto item : inv->getItemMap() )
    {
      if( item.second && item.second->getId() == catalogId )
        outVec.push_back( std::make_pair( i, static_cast< int8_t >( item.first ) ) );
    }
  }
  return outVec;
}

Core::Entity::Player::InvSlotPair Core::Entity::Player::getFreeBagSlot()
{
  for( auto i : { Bag0, Bag1, Bag2, Bag3 } )
  {
    auto freeSlot = static_cast< int8_t >( m_storageMap[ i ]->getFreeSlot() );

    if( freeSlot != -1 )
      return std::make_pair( i, freeSlot );
  }
  // no room in inventory
  return std::make_pair( 0, -1 );
}

Core::ItemPtr Core::Entity::Player::getItemAt( uint16_t containerId, uint8_t slotId )
{
  return m_storageMap[ containerId ]->getItem( slotId );
}


uint32_t Core::Entity::Player::getCurrency( CurrencyType type )
{

  auto currItem = m_storageMap[ Currency ]->getItem( static_cast< uint8_t >( type ) - 1 );

  if( !currItem )
    return 0;

  return currItem->getStackSize();

}

uint32_t Core::Entity::Player::getCrystal( CrystalType type )
{

  auto currItem = m_storageMap[ Crystal ]->getItem( static_cast< uint8_t >( type ) - 1 );

  if( !currItem )
    return 0;

  return currItem->getStackSize();

}

void Core::Entity::Player::writeInventory( InventoryType type )
{
  auto pLog = g_fw.get< Logger >();
  auto pDb = g_fw.get< Db::DbWorkerPool< Db::ZoneDbConnection > >();

  auto storage = m_storageMap[ type ];

  if( !storage->isPersistentStorage() )
    return;

  std::string query = "UPDATE " + storage->getTableName() + " SET ";

  for( int32_t i = 0; i <= storage->getMaxSize(); i++ )
  {
    auto currItem = storage->getItem( i );

    if( i > 0 )
      query += ", ";

    query += "container_" + std::to_string( i ) + " = " + std::to_string( currItem ? currItem->getUId() : 0 );
  }

  query += " WHERE CharacterId = " + std::to_string( getId() );

  if( storage->isMultiStorage() )
    query += " AND storageId = " + std::to_string( static_cast< uint16_t >( type ) );

  pLog->debug( query );
  pDb->execute( query );
}

void Core::Entity::Player::writeItem( Core::ItemPtr pItem ) const
{
  auto pDb = g_fw.get< Db::DbWorkerPool< Db::ZoneDbConnection > >();
  auto stmt = pDb->getPreparedStatement( Db::CHARA_ITEMGLOBAL_UP );

  // todo: add more fields
  stmt->setInt( 1, pItem->getStackSize() );
  stmt->setInt( 2, pItem->getDurability() );
  stmt->setInt( 3, pItem->getStain() );

  stmt->setInt64( 4, pItem->getUId() );

  pDb->directExecute( stmt );
}

void Core::Entity::Player::deleteItemDb( Core::ItemPtr item ) const
{
  auto pDb = g_fw.get< Db::DbWorkerPool< Db::ZoneDbConnection > >();
  auto stmt = pDb->getPreparedStatement( Db::CHARA_ITEMGLOBAL_DELETE );

  stmt->setInt64( 1, item->getUId() );

  pDb->directExecute( stmt );
}


bool Core::Entity::Player::isObtainable( uint32_t catalogId, uint8_t quantity )
{

  return true;
}


Core::ItemPtr Core::Entity::Player::addItem( uint32_t catalogId, uint32_t quantity, bool isHq, bool silent )
{
  auto pDb = g_fw.get< Db::DbWorkerPool< Db::ZoneDbConnection > >();
  auto pExdData = g_fw.get< Data::ExdDataGenerated >();
  auto itemInfo = pExdData->get< Core::Data::Item >( catalogId );

  // if item data doesn't exist or it's a blank field
  if( !itemInfo || itemInfo->levelItem == 0 )
  {
    return nullptr;
  }

  quantity = std::min< uint32_t >( quantity, itemInfo->stackSize );

  // used for item obtain notification
  uint32_t originalQuantity = quantity;

  std::pair< uint16_t, uint8_t > freeBagSlot;
  bool foundFreeSlot = false;

  std::vector< uint16_t > bags = { Bag0, Bag1, Bag2, Bag3 };

  // add the related armoury bag to the applicable bags and try and fill a free slot there before falling back to regular inventory
  if( itemInfo->isEquippable && getEquipDisplayFlags() & StoreNewItemsInArmouryChest )
  {
    auto bag = Items::Util::getCharaEquipSlotCategoryToArmoryId( itemInfo->equipSlotCategory );

    bags.insert( bags.begin(), bag );
  }

  for( auto bag : bags )
  {
    auto storage = m_storageMap[ bag ];

    for( uint8_t slot = 0; slot < storage->getMaxSize(); slot++ )
    {
      auto item = storage->getItem( slot );

      // add any items that are stackable
      if( item && !itemInfo->isEquippable && item->getId() == catalogId )
      {
        uint32_t count = item->getStackSize();
        uint32_t maxStack = item->getMaxStackSize();

        // if slot is full, skip it
        if( count >= maxStack )
          continue;

        // check slot is same quality
        if( item->isHq() != isHq )
          continue;

        // update stack
        uint32_t newStackSize = count + quantity;
        if( newStackSize > maxStack )
        {
          quantity = newStackSize - maxStack;
          newStackSize = maxStack;
        }

        item->setStackSize( newStackSize );
        writeItem( item );

        auto slotUpdate = boost::make_shared< UpdateInventorySlotPacket >( getId(), slot, bag, *item );
        queuePacket( slotUpdate );

        // return existing stack if we have no overflow - items fit into a preexisting stack
        if( quantity == 0 )
        {
          queuePacket( makeActorControl143( getId(), ItemObtainIcon, catalogId, originalQuantity ) );

          return item;
        }

      }
      else if( !item && !foundFreeSlot )
      {
        freeBagSlot = { bag, slot };
        foundFreeSlot = true;
      }
    }
  }

  // couldn't find a free slot and we still have some quantity of items left, shits fucked
  if( !foundFreeSlot )
    return nullptr;

  auto item = createItem( catalogId, quantity );
  item->setHq( isHq );

  auto storage = m_storageMap[ freeBagSlot.first ];
  storage->setItem( freeBagSlot.second, item );

  writeInventory( static_cast< InventoryType >( freeBagSlot.first ) );

  if( !silent )
  {
    auto invUpdate = boost::make_shared< UpdateInventorySlotPacket >( getId(), freeBagSlot.second, freeBagSlot.first,
                                                                      *item );
    queuePacket( invUpdate );

    queuePacket( makeActorControl143( getId(), ItemObtainIcon, catalogId, originalQuantity ) );
  }

  return item;
}

void
Core::Entity::Player::moveItem( uint16_t fromInventoryId, uint8_t fromSlotId, uint16_t toInventoryId, uint8_t toSlot )
{

  auto tmpItem = m_storageMap[ fromInventoryId ]->getItem( fromSlotId );
  auto& itemMap = m_storageMap[ fromInventoryId ]->getItemMap();

  if( tmpItem == nullptr )
    return;

  itemMap[ fromSlotId ].reset();

  m_storageMap[ toInventoryId ]->setItem( toSlot, tmpItem );

  writeInventory( static_cast< InventoryType >( toInventoryId ) );

  if( fromInventoryId != toInventoryId )
    writeInventory( static_cast< InventoryType >( fromInventoryId ) );

  if( static_cast< InventoryType >( toInventoryId ) == GearSet0 )
    equipItem( static_cast< GearSetSlot >( toSlot ), tmpItem, true );

  if( static_cast< InventoryType >( fromInventoryId ) == GearSet0 )
    unequipItem( static_cast< GearSetSlot >( fromSlotId ), tmpItem );


}

bool Core::Entity::Player::updateContainer( uint16_t storageId, uint8_t slotId, ItemPtr pItem )
{
  auto containerType = Items::Util::getContainerType( storageId );

  m_storageMap[ storageId ]->setItem( slotId, pItem );

  switch( containerType )
  {
    case Armory:
    case Bag:
    case CurrencyCrystal:
    {
      writeInventory( static_cast< InventoryType >( storageId ) );
      break;
    }

    case GearSet:
    {
      if( pItem )
        equipItem( static_cast< GearSetSlot >( slotId ), pItem, true );
      else
        unequipItem( static_cast< GearSetSlot >( slotId ), pItem );

      writeInventory( static_cast< InventoryType >( storageId ) );
      break;
    }
    default:
      break;
  }

  return true;
}

void Core::Entity::Player::splitItem( uint16_t fromInventoryId, uint8_t fromSlotId,
                                      uint16_t toInventoryId, uint8_t toSlot, uint16_t itemCount )
{
  if( itemCount == 0 )
    return;

  auto fromItem = m_storageMap[ fromInventoryId ]->getItem( fromSlotId );
  if( !fromItem )
    return;

  // check we have enough items in the origin slot
  // nb: don't let the client 'split' a whole stack into another slot
  if( fromItem->getStackSize() < itemCount )
    // todo: correct the invalid item split? does retail do this or does it just ignore it?
    return;

  // make sure toInventoryId & toSlot are actually free so we don't orphan an item
  if( m_storageMap[ toInventoryId ]->getItem( toSlot ) )
    // todo: correct invalid move? again, not sure what retail does here
    return;

  auto newItem = addItem( fromItem->getId(), itemCount, fromItem->isHq(), true );
  if( !newItem )
    return;

  fromItem->setStackSize( fromItem->getStackSize() - itemCount );

  updateContainer( fromInventoryId, fromSlotId, fromItem );
  updateContainer( toInventoryId, toSlot, newItem );

  writeItem( fromItem );
}

void Core::Entity::Player::mergeItem( uint16_t fromInventoryId, uint8_t fromSlotId,
                                      uint16_t toInventoryId, uint8_t toSlot )
{
  auto fromItem = m_storageMap[ fromInventoryId ]->getItem( fromSlotId );
  auto toItem = m_storageMap[ toInventoryId ]->getItem( toSlot );

  if( !fromItem || !toItem )
    return;

  if( fromItem->getId() != toItem->getId() )
    return;

  uint32_t stackSize = fromItem->getStackSize() + toItem->getStackSize();
  uint32_t stackOverflow = stackSize - std::min< uint32_t >( fromItem->getMaxStackSize(), stackSize );

  // we can destroy the original stack if there's no overflow
  if( stackOverflow == 0 )
  {
    m_storageMap[ fromInventoryId ]->removeItem( fromSlotId );
    deleteItemDb( fromItem );
  }
  else
  {
    fromItem->setStackSize( stackOverflow );
    writeItem( fromItem );
  }


  toItem->setStackSize( stackSize );
  writeItem( toItem );

  updateContainer( fromInventoryId, fromSlotId, fromItem );
  updateContainer( toInventoryId, toSlot, toItem );
}

void Core::Entity::Player::swapItem( uint16_t fromInventoryId, uint8_t fromSlotId,
                                     uint16_t toInventoryId, uint8_t toSlot )
{
  auto fromItem = m_storageMap[ fromInventoryId ]->getItem( fromSlotId );
  auto toItem = m_storageMap[ toInventoryId ]->getItem( toSlot );
  auto& itemMap = m_storageMap[ fromInventoryId ]->getItemMap();

  if( fromItem == nullptr || toItem == nullptr )
    return;

  // An item is being moved from bag0-3 to equippment, meaning
  // the swapped out item will be placed in the matching armory.
  if( Items::Util::isEquipment( toInventoryId )
      && !Items::Util::isEquipment( fromInventoryId )
      && !Items::Util::isArmory( fromInventoryId ) )
  {
    updateContainer( fromInventoryId, fromSlotId, nullptr );
    fromInventoryId = Items::Util::getCharaEquipSlotCategoryToArmoryId( toSlot );
    fromSlotId = static_cast < uint8_t >( m_storageMap[ fromInventoryId ]->getFreeSlot() );
  }

  auto containerTypeFrom = Items::Util::getContainerType( fromInventoryId );
  auto containerTypeTo = Items::Util::getContainerType( toInventoryId );

  updateContainer( toInventoryId, toSlot, fromItem );
  updateContainer( fromInventoryId, fromSlotId, toItem );
}

void Core::Entity::Player::discardItem( uint16_t fromInventoryId, uint8_t fromSlotId )
{
  // i am not entirely sure how this should be generated or if it even is important for us...
  uint32_t transactionId = 1;

  auto fromItem = m_storageMap[ fromInventoryId ]->getItem( fromSlotId );

  deleteItemDb( fromItem );

  m_storageMap[ fromInventoryId ]->removeItem( fromSlotId );
  updateContainer( fromInventoryId, fromSlotId, nullptr );

  auto invTransPacket = makeZonePacket< FFXIVIpcInventoryTransaction >( getId() );
  invTransPacket->data().transactionId = transactionId;
  invTransPacket->data().ownerId = getId();
  invTransPacket->data().storageId = fromInventoryId;
  invTransPacket->data().catalogId = fromItem->getId();
  invTransPacket->data().stackSize = fromItem->getStackSize();
  invTransPacket->data().slotId = fromSlotId;
  invTransPacket->data().type = 7;
  queuePacket( invTransPacket );

  auto invTransFinPacket = makeZonePacket< FFXIVIpcInventoryTransactionFinish >( getId() );
  invTransFinPacket->data().transactionId = transactionId;
  invTransFinPacket->data().transactionId1 = transactionId;
  queuePacket( invTransFinPacket );
}

uint16_t Core::Entity::Player::calculateEquippedGearItemLevel()
{
  uint32_t iLvlResult = 0;

  auto gearSetMap = m_storageMap[ GearSet0 ]->getItemMap();

  auto it = gearSetMap.begin();

  while( it != gearSetMap.end() )
  {
    auto currItem = it->second;

    if( currItem )
    {
      iLvlResult += currItem->getItemLevel();

      // If item is weapon and isn't one-handed
      if( currItem->isWeapon() && !Items::Util::isOneHandedWeapon( currItem->getCategory() ) )
      {
        iLvlResult += currItem->getItemLevel();
      }
    }

    it++;
  }

  return boost::algorithm::clamp( iLvlResult / 13, 0, 9999 );
}


uint8_t Core::Entity::Player::getFreeSlotsInBags()
{
  uint8_t slots = 0;
  for( uint8_t container : { Bag0, Bag1, Bag2, Bag3 } )
  {
    const auto& storage = m_storageMap[ container ];
    slots += ( storage->getMaxSize() - storage->getEntryCount() );
  }
  return slots;
}

bool Core::Entity::Player::collectHandInItems( std::vector< uint32_t > itemIds )
{
  // todo: figure out how the game gets the required stack count
  const auto& container = m_storageMap[ HandIn ];

  std::vector< uint8_t > foundItems;

  auto itemMap = container->getItemMap();

  for( auto& item : itemMap )
  {
    for( auto needle : itemIds )
    {
      if( item.second->getId() == needle )
      {
        foundItems.push_back( item.first );
        break;
      }
    }
  }

  // couldn't find all the items required
  if( foundItems.size() != itemIds.size() )
    return false;

  // remove items
  for( auto item : foundItems )
  {
    container->removeItem( item );
  }

  return true;
}
