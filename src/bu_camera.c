
#include "simple_logger.h"

#include "bu_level.h"
#include "bu_camera.h"
#include "bu_collision.h"
#include "bu_player.h"


static Camera camera = { 0 };

void camera_set_dimensions(Vector2D size)
{
	camera.view.w = size.x;
	camera.view.h = size.y;

	camera.playerBounds.w = size.x / 1.5;
	camera.playerBounds.h = size.y / 1.5;

	camera.playerBounds.x = size.x / 6;
	camera.playerBounds.y = size.y / 6;
}

Vector2D camera_get_dimensions()
{
	Vector2D size;
	size.x = camera.view.w;
	size.y = camera.view.h;
	return size;
}

Rect camera_get_player_bounds()
{
	Rect rect;
	rect.x = camera.playerBounds.x;
	rect.y = camera.playerBounds.y;
	rect.h = camera.playerBounds.h;
	rect.w = camera.playerBounds.w;
	return rect;
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
}

void camera_move(Vector2D move)
{
	//slog("MOVE x : %f , MOVE Y : %f", move.x, move.y);
	
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
		if (camera.view.x < get_level_dimension().x)
		{
			slog("Camera moving x");
			camera.view.x = camera.bounds.x - camera.view.w;
			camera.playerBounds.x -= move.x;
		}
	}
	if (camera.view.y > (camera.bounds.y - camera.view.h) )
	{
		if (camera.view.y < get_level_dimension().y)
		{
			slog("Camera moving y");
			camera.view.y = camera.bounds.y - camera.view.h;
			camera.playerBounds.y -= move.y;
		}
	}
	//slog("PlayerC x: %f, playerC y: %f", get_player_collider().x, get_player_collider().y);
	//slog("PlayerC w: %f, playerC h: %f", get_player_collider().w, get_player_collider().h);
	//slog("camera view x: %f, camera view y: %f", camera.view.x, camera.view.y);
	//slog("camera view w: %f, camera view h: %f", camera.view.w, camera.view.h);
	//slog("camera bounds x: %f, camera bounds y: %f", camera.bounds.x, camera.bounds.y);
	//slog("camera playerB x: %f, camera playerB y: %f", camera.playerBounds.x, camera.playerBounds.y);
	//slog("camera playerB w: %f, camera playerB h: %f\n", camera.playerBounds.w, camera.playerBounds.h);
	
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
	if (((rect.x + rect.w) < camera.view.x) || (rect.x > (camera.view.x + camera.view.w)) ||
		((rect.y + rect.h) < camera.view.y) || (rect.y > (camera.view.y + camera.view.h)))
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
