#include "LDRooms.h"

bool ALDRooms::SetGuest(ALDNPCats* NewGuest)
{
	if (IsValid(Guest))
	{
		return false;
	}

	Guest = NewGuest;
	return IsValid(NewGuest);
}

bool ALDRooms::RemoveGuest()
{
	if (IsValid(Guest))
	{
		Guest = nullptr;
		return true;
	}

	return false;
}
