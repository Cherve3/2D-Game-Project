#ifndef __BU_SHOP_H__
#define __BU_SHOP_H__

#include "gfc_types.h"

#include "bu_items.h"
#include "bu_entity.h"

Bool *get_shop_open();

void shop_init();

void open_shop();

Bool buy_item(Item *item, Entity *player);

#endif
