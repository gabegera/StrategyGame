#include "Interfaces/StructureInterface.h"

bool IStructureInterface::TrySelect(ARTSCamera* SelectInstigator)
{
	return false;
}

bool IStructureInterface::TryRecycle(ARTSCamera* DestroyInstigator)
{
	return false;
}

bool IStructureInterface::DoesIncreaseStorage()
{
	return false;
}

bool IStructureInterface::DoesGenerateResources()
{
	return false;
}

bool IStructureInterface::DoesHarvestResources()
{
	return false;
}

bool IStructureInterface::DoesProvideHousing()
{
	return false;
}

bool IStructureInterface::DoesRequireWorkers()
{
	return false;
}
