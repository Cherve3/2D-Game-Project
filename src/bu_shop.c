
#include "simple_logger.h"

#include "bu_shop.h"
#include "bu_player.h"

static Bool shop_open;
static Bool bought_item;

Bool *get_shop_open()
{
	return shop_open;
}

void shop_init()
{
	shop_open = false;
	bought_item = false;
}

void open_shop()
{
	//slog("Shop open : %i", shop_open);
	Uint32 *keys = SDL_GetKeyboardState(NULL);
	if (shop_open == false){
		slog("want to buy item");
		shop_open = true;
	}
	else if (shop_open == true)
	{
		slog("close shop");
		shop_open = false;
	}
}

Bool buy_item(Item *item, Entity *player_ent)
{
	Player *player;
	if (!player_ent) return;

	player = (Player*)player_ent->data;

	if (item->cost < player->stats.money)
	{
		player->stats.money -= item->cost;
		slog("Buying item for $%i. You have this amount left: $%i", item->cost,player->stats.money);
		return true;
	}
	else
	{
		slog("Do not have enough money to purchase.");
	}

	return false;
}

/*eol@eof*/
