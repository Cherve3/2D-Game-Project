
#include "simple_logger.h"

#include "bu_camera.h"
#include "bu_player.h"

static Camera camera = { 0 };

void camera_set_dimensions(Vector2D size)
{
	camera.view.w = size.x;
	camera.view.h = size.y;

	camera.playerBounds.w = size.x / 1.5;
	camera.playerBounds.h = size.y / 1.5;
}

Vector2D camera_get_dimensions()
{
	Vector2D size;
	size.x = camera.view.w;
	size.y = camera.view.h;
	return size;
}

Vector2D camera_get_player_bounds()
{
	Vector2D size;
	size.x = camera.playerBounds.w;
	size.y = camera.playerBounds.h;
	return size;
}

void camera_set_bounds(Vector2D bounds)
{
	camera.bounds.x = bounds.x;
	camera.bounds.y = bounds.y;
}

Vector2D camera_get_bounds()
{
	return camera.bounds;
}

void camera_set_position(Vector2D position)
{
	camera.view.x = position.x;
	camera.view.y = position.y;

	camera.playerBounds.x = position.x / 6;
	camera.playerBounds.y = position.y / 6;
}

void camera_move(Vector2D move)
{
	camera.view.x += move.x;
	camera.view.y += move.y;

	camera.playerBounds.x += move.x;
	camera.playerBounds.y += move.y;

	if (camera.view.x < 0)
	{
		camera.view.x = 0;
		camera.playerBounds.x -= move.x;
	}
	if (camera.view.y < 0)
	{
		camera.view.y = 0;
		camera.playerBounds.y -= move.y;
	}
	if (camera.view.x > (camera.bounds.x - camera.view.w) )
	{
		camera.view.x = camera.bounds.x - camera.view.w;
		camera.playerBounds.x -= move.x;
	}
	if (camera.view.y > (camera.bounds.y - camera.view.h) )
	{
		camera.view.y = camera.bounds.y - camera.view.h;
		camera.playerBounds.y -= move.y;
	}
}

Bool camera_point_on_screen(Vector2D point)
{
	if ((!point.x < camera.view.x) || (point.x > camera.view.x + camera.view.w) ||
		(!point.y < camera.view.y) || (point.y > camera.view.y + camera.view.h))
	{
		return false;
	}
	return true;
}

Bool camera_rect_on_screen(SDL_Rect rect)
{
	if (((!rect.x + rect.w) < camera.view.x) || (rect.x > (camera.view.x + camera.view.w)) ||
		((!rect.y + rect.h) < camera.view.y) || (rect.y > (camera.view.y + camera.view.h)))
	{
		return false;
	}
	return true;
}

Rect camera_get_rect()
{
	return camera.view;
}

Vector2D camera_get_position()
{
	Vector2D position;
	position.x = camera.view.x;
	position.y = camera.view.y;

	return position;
}

Vector2D camera_get_offset()
{
	Vector2D position;
	position.x = -camera.view.x;
	position.y = -camera.view.y;
	return position;
}

/*eol@eof*/
