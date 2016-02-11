
#pragma once

#include "Object.h"
#include "../Spacecrafts/FlareBomb.h"
#include "FlareAsteroid.h"
#include "../Economy/FlarePeople.h"
#include "FlareSectorInterface.h"
#include "FlareSimulatedSector.generated.h"

class UFlareSimulatedSpacecraft;
struct FFlareSpacecraftDescription;
class UFlareFleet;
class AFlareGame;
struct FFlarePlayerSave;
struct FFlareResourceDescription;




UCLASS()
class HELIUMRAIN_API UFlareSimulatedSector : public UFlareSectorInterface
{
    GENERATED_UCLASS_BODY()

public:
    /*----------------------------------------------------
        Save
    ----------------------------------------------------*/

	/** Load the sector from a save file */
	virtual void Load(const FFlareSectorDescription* Description, const FFlareSectorSave& Data, const FFlareSectorOrbitParameters& OrbitParameters);

	/** Load sector people */
	virtual UFlarePeople* LoadPeople(const FFlarePeopleSave& PeopleData);

	/** Save the sector to a save file */
    virtual FFlareSectorSave* Save();


    /*----------------------------------------------------
        Gameplay
    ----------------------------------------------------*/

    /** Create a station in the level  for a specific company */
	UFlareSimulatedSpacecraft* CreateStation(FName StationClass, UFlareCompany* Company, FVector TargetPosition, FRotator TargetRotation = FRotator::ZeroRotator, FName AttachPoint = NAME_None);

    /** Create a ship in the level  for a specific company */
	UFlareSimulatedSpacecraft* CreateShip(FName ShipClass, UFlareCompany* Company, FVector TargetPosition);

	/** Create a ship or station in the level  for a specific company. No null parameter accepted */
	UFlareSimulatedSpacecraft* CreateShip(FFlareSpacecraftDescription* ShipDescription, UFlareCompany* Company, FVector TargetLocation, FRotator TargetRotation = FRotator::ZeroRotator, FName AttachPoint = NAME_None);

	/** Create an asteroid */
	void CreateAsteroid(int32 ID, FName Name, FVector Location);

	/** Add the fleet and its ships from the sector */
	void AddFleet(UFlareFleet* Fleet);

	/** Remove the fleet but let its ships in the sector */
	void DisbandFleet(UFlareFleet* Fleet);

	/** Retire the fleet and its ships from the sector */
	void RetireFleet(UFlareFleet* Fleet);

	int RemoveSpacecraft(UFlareSimulatedSpacecraft* Spacecraft);

	void SetShipToFly(UFlareSimulatedSpacecraft* Ship);

	bool CanBuildStation(FFlareSpacecraftDescription* StationDescription, UFlareCompany* Company);

	bool BuildStation(FFlareSpacecraftDescription* StationDescription, UFlareCompany* Company);

	void SimulateTransport();

	uint32 SimulateTransport(UFlareCompany* Company, uint32 TransportCapacity);

	void SimulateTrade(TArray<uint32> CompanyRemainingTransportCapacity);

	void FillResourceConsumers(UFlareCompany* Company, uint32& TransportCapacity, bool AllowTrade);

	void AdaptativeTransportResources(UFlareCompany* Company, uint32& TransportCapacity, EFlareTransportLimitType::Type TransportLimitType, uint32 TransportLimit, bool ActiveOnly, bool AllowTrade = false);

	uint32 TakeUselessResources(UFlareCompany* Company, FFlareResourceDescription* Resource, uint32 QuantityToTake, bool AllowTrade = false);

	uint32 TakeResources(UFlareCompany* Company, FFlareResourceDescription* Resource, uint32 QuantityToTake);

	uint32 GiveResources(UFlareCompany* Company, FFlareResourceDescription* Resource, uint32 QuantityToGive);

	uint32 AdaptativeGiveResources(UFlareCompany* Company, FFlareResourceDescription* GivenResource, uint32 QuantityToGive, EFlareTransportLimitType::Type TransportLimitType, uint32 TransportLimit, bool ActiveOnly, bool StorageOnly);

protected:

    /*----------------------------------------------------
        Protected data
    ----------------------------------------------------*/

    // Gameplay data
	FFlareSectorSave                        SectorData;

    TArray<UFlareSimulatedSpacecraft*>      SectorStations;
	TArray<IFlareSpacecraftInterface*>      SectorStationInterfaces;
    TArray<UFlareSimulatedSpacecraft*>      SectorShips;
	TArray<IFlareSpacecraftInterface*>      SectorShipInterfaces;
	TArray<UFlareSimulatedSpacecraft*>      SectorSpacecrafts;

	TArray<UFlareFleet*>                    SectorFleets;

	UPROPERTY()
	UFlarePeople*							People;

	int32                                   PersistentStationIndex;

public:

    /*----------------------------------------------------
        Getters
    ----------------------------------------------------*/


    inline FName GetIdentifier() const
    {
        return SectorData.Identifier;
    }

	/** Get the description of this sector */
	FText GetSectorDescription() const;

    inline TArray<UFlareSimulatedSpacecraft*>& GetSectorStations()
    {
        return SectorStations;
    }

	inline TArray<IFlareSpacecraftInterface*>& GetSectorStationInterfaces() override
	{
		return SectorStationInterfaces;
	}

    inline TArray<UFlareSimulatedSpacecraft*>& GetSectorShips()
    {
        return SectorShips;
    }

	inline TArray<IFlareSpacecraftInterface*>& GetSectorShipInterfaces() override
	{
		return SectorShipInterfaces;
	}

	inline TArray<UFlareSimulatedSpacecraft*>& GetSectorSpacecrafts()
	{
		return SectorSpacecrafts;
	}

	inline TArray<UFlareFleet*>& GetSectorFleets()
	{
		return SectorFleets;
	}

	inline UFlarePeople* GetPeople()
	{
		return People;
	}

	UFlareSimulatedSector* GetSimulatedSector() override
	{
		return this;
	}

	uint32 GetTransportCapacity(UFlareCompany* Company);

	uint32 GetResourceCount(UFlareCompany* Company, FFlareResourceDescription* Resource);

	/** If positive, return the remaining tranport capacity after transport.
	 *  If negative, return the laking tranport capacity after transport
	 */
	int32 GetTransportCapacityBalance(UFlareCompany* Company, bool AllowTrade = false);

	int32 GetTransportCapacityNeeds(UFlareCompany* Company, bool AllowTrade = false);

};
