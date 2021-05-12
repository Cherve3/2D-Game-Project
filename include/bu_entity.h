#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "chipmunk/chipmunk.h"

#include "gfc_types.h"

#include "gfc_audio.h"
#include "gf2d_sprite.h"

#include "bu_shapes.h"

#define EntitySoundMax 8

typedef struct Entity_s
{
	Bool		_inuse;
	TextWord   *name;

	/*physics*/
	cpVect		position;
	cpVect		spawn_pos;
	cpVect      velocity;
	cpShape	   *shape;
	cpBody     *body;
	Vector2D	acceleration;
	Rect        rect_collider;
	Sprite	   *sprite;
	float		frame;
	float		frameCount;
	float		frameRate;

	/*sound*/
	Sound* sound[EntitySoundMax];

	/*graphics*/
	Vector2D scale;                         /**<scale to draw sprite at*/
	Vector2D scaleCenter;                   /**<where to scale sprite from*/
	Vector3D rotation;                      /**<how to rotate the sprite*/
	Vector2D flip;                          /**<if to flip the sprite*/
	Vector2D facing;                        /**<direction the entity is facing*/


	void	  (*update)(struct Entity_s *self);
	void	  (*think)(struct Entity_s *self);
	void	  (*onTouch)(struct Entity_s *self, struct Entity_s *other);
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
* @brief All entities think
*/
void entity_manager_think_entities();

/**
 * @brief Update all entites
 */
void entity_manager_update_entities();

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

/**
 * @brief draws all entities that are in use to the current render frame
 */
void entity_draw_all();

void entity_touch(Entity *self, Entity *other);

void entity_collision_check(Entity *self);

Entity *get_entities_list();

Uint32 get_max_entities();

void print_entities();

#endif
