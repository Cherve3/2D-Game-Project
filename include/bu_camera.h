#ifndef __BU_CAMERA_H__
#define __BU_CAMERA_H__

#include "gfc_types.h"
#include "gfc_vector.h"

#include "bu_shapes.h"

typedef struct
{
	Rect view;
	Rect playerBounds;

	Vector2D bounds;
}Camera;

/**
 * @brief Set the camera size in width and height 
 * @param size the width and height of the camera
 */
void camera_set_dimensions(Vector2D size);

/**
* @brief Set the position of the camera
* @param position The position the camera will be set to
*/
void camera_set_position(Vector2D position);

/**
* @brief Set the upper bounds of the camera
* @param bounds The upper coordinate bounds 
*/
void camera_set_bounds(Vector2D bounds);

/**
 * @brief check if a point is on screen
 * @param point the point that visibility is checked
 * @return 1 if point is on screen otherwise NULL
 */
Bool camera_point_on_screen(Vector2D point);

/**
 * @brief Check if the rectangle is on screen
 * @param point the rect that visibility is checked
 * @return 1 if point is on screen otherwise NULL
 */
Bool camera_rect_on_screen(SDL_Rect rect);

/**
* @brief Move the camera to new position
* @param move A vector of the new x/y position to move to
*/
void camera_move(Vector2D move);

/**
* @brief Get the camera rectangle
* @return The rectangle
*/
Rect camera_get_rect();

/**
* @brief Get the current world position 
* @return The camera position in world coordinates
*/
Vector2D camera_get_position();

/**
* @brief return the offset for things to be drawn relative to the camera
* @return the inverse of the camara coordinates
*/
Vector2D camera_get_offset();

Rect camera_get_player_bounds();

#endif
