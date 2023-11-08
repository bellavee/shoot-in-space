#include "RenderItem.h"

void RenderItem::AddEntity(int entityid, std::shared_ptr<RenderItem> renderItem)
{
	_entityId = entityid;
	_entitiesRenderItem[_entityId] = renderItem;
}