#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"

typedef struct Entity_s
{
	Bool		_inuse;
	Vector2D	position;
	Sprite	   *sprite;
	float		frame;
	float		frameCount;
	float		frameRate;
	void	  (*update)(struct Entity_s *self);
	void	  (*think)(struct Entity_s *self);
	void	  (*draw)(struct Entity_s *self);
	void	  (*free)(struct Entity_s *self);
	void	   *data;
}Entity;

/**
 * @brief Initialize internal entity management system 
 * @param max_entities 
 */
void entity_manager_init(Uint32 max_entities);

/**
 * @brief Update all entites
 */
void entity_manager_update_entites();

/**
* @brief free all entities in the system and destroy entity manager
*/
void entity_manager_free();

/**
* @brief allocate an entity, initialize it to zero and return a pointer to it
* @return NULL on error (see logs) or a pointer to an initialized entity.
*/
Entity *entity_new();

/**
* @brief frees provided entity
* @param ent the entity to free
*/
void entity_free(Entity *ent);

/**
* @brief draws an entity to the current render frame
* @param ent the entity to draw
*/
void entity_draw(Entity *ent);

#endif
