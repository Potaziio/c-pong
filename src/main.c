/*
 * PONG IN MY CUSTOM OPENGL WRAPPER FRAMEWORK 
 * This is a test game for my framework 
*/

#include "engine/core/engine.h"

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 900
#define TICK_TIME 30

#define PADDLE_SPEED 500
#define PADDLE_WIDTH 20.0f
#define PADDLE_HEIGHT 180.0f

#define BALL_START_SPEED 300

#define BALL_WIDTH 8.0f 
#define BALL_HEIGHT 8.0f

#define BALL_EDGE_HIT_SPEED_GAIN 16.0f
#define BALL_PADDLE_HIT_SPEED_GAIN 30.0

#define BALL_MAX_START_STEEPNESS 0.6f

#define BALL_HIT_KNOCK_FACTOR 0.2f

// TODO: Create initialization function for framework, for seeding random and initializing renderer pipeline 

struct paddle 
{
	struct rect sprite;
	int direction;
	struct vector2 normal;
	struct aabb collider;
};

struct ball
{
	struct rect sprite;
	struct vector2 direction;
	struct aabb collider;
	float speed;
};

int main(void)
{
	// Initialize window parameters
	struct window window = {
		.title = "Game",
		.color = (struct rgba_color){RGBA_BLACK},
		.width = WINDOW_WIDTH,
		.height = WINDOW_HEIGHT,
		.config = WINDOW_VSYNC_OFF,
	};

	int res;

	// Create window, exit if error
	if ((res = window_create(&window)) == 0)
	{
		logger_log_string(ERROR, "Window creation failed\n");
		return 0;
	}

	// Create window, exit if error
	if ((res = renderer_init()) == 0)
	{
		logger_log_string(ERROR, "Could not initialize renderer\n");
		return 0;
	}

	init_random(time(NULL));

	int ticks = 0;

	// Create our camera
	struct camera camera;
	camera.bounds = (struct vector2){window.width, window.height};
	camera.type = ORTHOGRAPHIC;

	float start_time = window_get_time();
	float delta_time = 0.0f;

	int game_sterted = 0;
		
	// Initializing data or objects
	
	struct paddle player1 = {.normal = {1.0f, 0.0f}};
	struct paddle player2 = {.normal = {-1.0f, 0.0f}};
	struct ball ball = {.speed = BALL_START_SPEED};


	// Player 1 on the left 
	player1.sprite = (struct rect){.color = {RGBA_WHITE}, 
		.position = {0.0f + PADDLE_WIDTH, window.height * 0.5f, 0.0f}, .scale = {PADDLE_WIDTH, PADDLE_HEIGHT, 0.0f}, 
		.config = RECT_ORIGIN_CENTER};

	// Player 2 on the right
	player2.sprite = (struct rect){.color = {RGBA_WHITE}, 
		.position = {window.width - PADDLE_WIDTH, window.height * 0.5f, 0.0f}, .scale = {PADDLE_WIDTH, PADDLE_HEIGHT, 0.0f}, 
		.config = RECT_ORIGIN_CENTER};

	// Ball at center
	ball.sprite = (struct rect){.color = {RGBA_WHITE}, 
		.position = {window.width * 0.5f, window.height * 0.5f, 0.0f}, .scale = {BALL_WIDTH, BALL_HEIGHT, 0.0f}, .rotation = {0.0f, 0.0f, 1.0f},
		.config = RECT_ORIGIN_CENTER};

	renderer_init_rectangle(&player1.sprite);
	renderer_init_rectangle(&player2.sprite);
	renderer_init_rectangle(&ball.sprite);

	// Get a starting random direction for the ball

	while (!window.should_close)
	{
		// Clear window and update camera
		window_clear(&window);
		camera_update(&camera);

		// Get input for direction
		
		player1.direction = input_get_key(GLFW_KEY_W) ? -1 : input_get_key(GLFW_KEY_S) ? 1 : 0;
		player2.direction = input_get_key(GLFW_KEY_UP) ? -1 : input_get_key(GLFW_KEY_DOWN) ? 1 : 0;

		// Draw
		renderer_draw_rectangle(&player1.sprite, &camera);
		renderer_draw_rectangle(&player2.sprite, &camera);
		renderer_draw_rectangle(&ball.sprite, &camera);

		if (game_sterted)
		{
			if (input_get_key(GLFW_KEY_SPACE))
				delta_time = delta_time * 0.5f;

			// Update players position

			float player1_move_y = player1.direction * PADDLE_SPEED * delta_time;
			float player2_move_y = player2.direction * PADDLE_SPEED * delta_time;
				
			if ((player1.sprite.position.y - player1.sprite.scale.y * 0.5f) + player1_move_y > 0.0f && 
					(player1.sprite.position.y + player1.sprite.scale.y * 0.5f) + player1_move_y < window.height)
				player1.sprite.position.y += player1_move_y;

			if ((player2.sprite.position.y - player2.sprite.scale.y * 0.5f)+ player2_move_y > 0.0f && 
					(player2.sprite.position.y + player2.sprite.scale.y * 0.5f) + player2_move_y < window.height)
				player2.sprite.position.y += player2_move_y;

			ball.sprite.position.x += ball.direction.x * ball.speed * delta_time;
			ball.sprite.position.y += ball.direction.y * ball.speed * delta_time;
			ball.sprite.rotation_angle += ball.direction.x * ball.speed * delta_time;

			physics_set_sprite_collider(&ball.collider, ball.sprite, ball.sprite.config);
			physics_set_sprite_collider(&player1.collider, player1.sprite, player1.sprite.config);
			physics_set_sprite_collider(&player2.collider, player2.sprite, player2.sprite.config);

			// Ball out of bounds behavior (reset)

			if (ball.sprite.position.x < 0.0f || ball.sprite.position.x + ball.sprite.scale.x * 0.5f > window.width)
			{
				// Reset position and speed of everything when the ball goes out of bounds
				ball.sprite.position = (struct vector3){window.width * 0.5f, window.height * 0.5f, 0.0f};
				player1.sprite.position = (struct vector3){0.0f + PADDLE_WIDTH, window.height * 0.5f, 0.0f};
				player2.sprite.position = (struct vector3){window.width - PADDLE_WIDTH, window.height * 0.5f, 0.0f};
				player1.direction = 0;
				player2.direction = 0;
				ball.direction = (struct vector2){0.0f, 0.0f};
				game_sterted = 0;
			}
		} 
		else 
		{
			if (input_get_key_down(GLFW_KEY_SPACE))
			{
				game_sterted = 1;

				// We get the starting direction of the ball, normalized y cannot be more than the start steepness or else
				// The ball goes into and up and down motion loop
				// Quick hack to fix this 
				struct vector2 ball_start_direction;
				ball_start_direction.x = rand_int(0, window.width) - ball.sprite.position.x;
				ball_start_direction.y = rand_int(0, window.height) - ball.sprite.position.y;
				vector2_normalize(&ball_start_direction);

				while (fabs(ball_start_direction.y) > BALL_MAX_START_STEEPNESS)
				{
					ball_start_direction.x = rand_int(0, window.width) - ball.sprite.position.x;
					ball_start_direction.y = rand_int(0, window.height) - ball.sprite.position.y;

					vector2_normalize(&ball_start_direction);
				}

				ball.direction = ball_start_direction;

			}
		}

		window_end_frame(&window);

		ticks++;

		if (ticks >= TICK_TIME) 
		{
			// Little hack for collision checking 
			// This runs every 30 frames, we do it this way to mitigate double collision bugs, our framerate is too high for a game like this
		
			// Resolve ball collision

			// Ball out of bounds (top bottom)

			// We add BALL_HIT_KNOCK_FACTOR to move the ball a little every time we hit something
			// This prevents our ball getting stuck on one motion continuously
				
			if (ball.sprite.position.y < 0.0f)
			{
				// Reflect ball from top
				ball.direction.x = (ball.direction.x - 2 * (vector2_dot(ball.direction, (struct vector2){0.0f, 1.0f})) * 0.0f) + BALL_HIT_KNOCK_FACTOR; 
				ball.direction.y = (ball.direction.y - 2 * (vector2_dot(ball.direction, (struct vector2){0.0f, 1.0f})) * 1.0f);
				ball.speed += BALL_EDGE_HIT_SPEED_GAIN;

			} 
			else if (ball.sprite.position.y + ball.sprite.scale.y > window.height)
			{
				ball.direction.x = (ball.direction.x - 2 * (vector2_dot(ball.direction, (struct vector2){0.0f, -1.0f})) * 0.0f) + BALL_HIT_KNOCK_FACTOR; 
				ball.direction.y = (ball.direction.y - 2 * (vector2_dot(ball.direction, (struct vector2){0.0f, -1.0f})) * -1.0f);
				ball.speed += BALL_EDGE_HIT_SPEED_GAIN;
			}

			// Ball hits players

			if (physics_aabb_in_aabb(player1.collider, ball.collider))
			{
				ball.direction.x = (ball.direction.x - 2 * (vector2_dot(ball.direction, player1.normal)) * player1.normal.x);
				ball.direction.y = (ball.direction.y - 2 * (vector2_dot(ball.direction, player1.normal)) * player1.normal.y) + BALL_HIT_KNOCK_FACTOR; 
				ball.speed += BALL_PADDLE_HIT_SPEED_GAIN;
			}

			else if (physics_aabb_in_aabb(player2.collider, ball.collider))
			{
				ball.direction.x = (ball.direction.x - 2 * (vector2_dot(ball.direction, player2.normal)) * player2.normal.x);
				ball.direction.y = (ball.direction.y - 2 * (vector2_dot(ball.direction, player2.normal)) * player2.normal.y) + BALL_HIT_KNOCK_FACTOR; 
				ball.speed += BALL_PADDLE_HIT_SPEED_GAIN;
			}


			ticks = 0;
		}

		delta_time = window_get_time() - start_time;
		start_time = window_get_time();
	}
	
	renderer_free_rectangle(&player1.sprite);
	renderer_free_rectangle(&player2.sprite);
	renderer_free_rectangle(&ball.sprite);

	renderer_free();
	window_free_memory(&window);	
		
	return 0;
}

