
#include "simple_logger.h"

#include "bu_collision.h"
#include "bu_entity.h"
#include "bu_npc.h"

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

Bool attack_collision(Rect attack, int damage)
{
	int i;
	Bool collision = false;
	NPC *npc;

	for (i = 0; i < get_max_entities(); i++)
	{
		
		if (!get_entities_list()[i]._inuse) continue;
		if (!strstr(get_entities_list()[i].name, "NPC")) continue;
		//slog("Ent Name: %s", get_entities_list()[i].name);
		collision = entity_collision(attack, get_entities_list()[i].rect_collider);
		if (collision)
		{
			npc = (NPC*)get_entities_list()[i].data;
			npc->stats.life -= damage;
		}
	}
	//slog("Collision = %i", collision);
	return collision;
}


/*eol@eof*/
