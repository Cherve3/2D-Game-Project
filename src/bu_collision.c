
#include "bu_collision.h"

Bool player_bounds_collision(Rect player, Rect bounds)
{
	//slog("")
	if (((player.x + player.w) < bounds.x) || (player.x > (bounds.x + bounds.w)) ||
		((player.y + player.h) < bounds.y) || (player.y > (bounds.y + bounds.h)))
	{
		return false;
	}
	return true;
}

Bool entity_collision(Rect ent, Rect other)
{
	if (((ent.x + ent.w) < other.x) || (ent.x > (other.x + other.w)) ||
		((ent.y + ent.h) < other.y) || (ent.y > (other.y + other.h)))
	{
		return false;
	}
	return true;
}


/*eol@eof*/
