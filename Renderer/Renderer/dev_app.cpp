#include "dev_app.h"
#include "math_types.h"

#define LAB_1 0		//particles and emitters
#define LAB_2 1		//matrix behavior
#define LAB_3 1		//frustrum culling

namespace end
{
	const float4 RED = float4(1, 0, 0, 1);
	const float4 GREEN = float4(0, 1, 0, 1);
	const float4 BLUE = float4(0, 0, 1, 1);

	const int MX = 0;
	const int MY = 1;
	const int MZ = 2;
	const int MW = 3;

	float4x4 player;

#pragma region Lab 1 Variables
#if LAB_1

	struct particle
	{
		float3 pos;
		float3 prev_pos;
		float3 init_velocity;
		float3 velocity;
		float4 color;
		float life_time;
		float time_stamp;
		bool alive;
	};

	struct emitter
	{
		float3 spawn_pos;
		float4 spawn_color;
		float spawn_time;
		float time_stamp;
		sorted_pool_t<int16_t, 265> indicies;
	};

	const int PARTICLE_COUNT = 1024;
	const int MAX_EMITTERS = 5;
	const int EMITTER_PARTICLE_COUNT = PARTICLE_COUNT / MAX_EMITTERS;
	const float3 GRAVITY = float3(0, -9.8f, 0);
	pool_t<particle, PARTICLE_COUNT> shared_pool;
	std::array<emitter*, MAX_EMITTERS> emitters;

#endif
#pragma endregion

#pragma region Lab 2 Variables
#if LAB_2

	const float4x4 MATRIX_I
	{
		float4(1, 0, 0, 0),
		float4(0, 1, 0, 0),
		float4(0, 0, 1, 0),
		float4(0, 0, 0, 1)
	};

	const float3 WORLDUP = float3(0, 1, 0);

	float4x4 look_at;
	float4x4 turn_to;

	float3 turnto_pos;
	float turnto_xangle;
	float turnto_yangle;

	float3 player_pos;
	float player_yangle;

	float3 cam_pos;
	float2 mouse_pos;
	float cam_xangle;
	float cam_yangle;
	float camturn_speed;

	float test;

#endif
#pragma endregion

#pragma region Lab 3 Variables
#if LAB_3

	const int aabbarr_size = 5;
	std::array<aabb_t, aabbarr_size> aabbs;
	std::array<bool, aabbarr_size> aabbsres;
	frustum_t f;

	const float fov = 1.0472;
	const float asp = 1.4;
	float nearclip = -1;
	float farclip = -8;

#endif
#pragma endregion

	double delta_time = 0.0;
	double grid_r, grid_g, grid_b;
	float run_time = 0.0f;
	bool frame_one = true;

	double dev_app_t::get_delta_time()const
	{
		return delta_time;
	}

	dev_app_t::dev_app_t()
	{

	}

	double calc_delta_time()
	{
		static std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = new_time - last_time;
		last_time = new_time;

		return std::min(1.0 / 15.0, elapsed_seconds.count());
	}
	
	//grid
	void dev_app_t::draw_grid(float4 color)
	{
		//vertical lines
		end::debug_renderer::add_line(float3(-9, 0, -10), float3(-9, 0, 10), color);
		end::debug_renderer::add_line(float3(-8, 0, -10), float3(-8, 0, 10), color);
		end::debug_renderer::add_line(float3(-7, 0, -10), float3(-7, 0, 10), color);
		end::debug_renderer::add_line(float3(-6, 0, -10), float3(-6, 0, 10), color);
		end::debug_renderer::add_line(float3(-5, 0, -10), float3(-5, 0, 10), color);
		end::debug_renderer::add_line(float3(-4, 0, -10), float3(-4, 0, 10), color);
		end::debug_renderer::add_line(float3(-3, 0, -10), float3(-3, 0, 10), color);
		end::debug_renderer::add_line(float3(-2, 0, -10), float3(-2, 0, 10), color);
		end::debug_renderer::add_line(float3(-1, 0, -10), float3(-1, 0, 10), color);
		end::debug_renderer::add_line(float3(0, 0, -10), float3(0, 0, 10), color);
		end::debug_renderer::add_line(float3(1, 0, -10), float3(1, 0, 10), color);
		end::debug_renderer::add_line(float3(2, 0, -10), float3(2, 0, 10), color);
		end::debug_renderer::add_line(float3(3, 0, -10), float3(3, 0, 10), color);
		end::debug_renderer::add_line(float3(4, 0, -10), float3(4, 0, 10), color);
		end::debug_renderer::add_line(float3(5, 0, -10), float3(5, 0, 10), color);
		end::debug_renderer::add_line(float3(6, 0, -10), float3(6, 0, 10), color);
		end::debug_renderer::add_line(float3(7, 0, -10), float3(7, 0, 10), color);
		end::debug_renderer::add_line(float3(8, 0, -10), float3(8, 0, 10), color);
		end::debug_renderer::add_line(float3(9, 0, -10), float3(9, 0, 10), color);

		//horizontal lines
		end::debug_renderer::add_line(float3(-10, 0, -9), float3(10, 0, -9), color);
		end::debug_renderer::add_line(float3(-10, 0, -8), float3(10, 0, -8), color);
		end::debug_renderer::add_line(float3(-10, 0, -7), float3(10, 0, -7), color);
		end::debug_renderer::add_line(float3(-10, 0, -6), float3(10, 0, -6), color);
		end::debug_renderer::add_line(float3(-10, 0, -5), float3(10, 0, -5), color);
		end::debug_renderer::add_line(float3(-10, 0, -4), float3(10, 0, -4), color);
		end::debug_renderer::add_line(float3(-10, 0, -3), float3(10, 0, -3), color);
		end::debug_renderer::add_line(float3(-10, 0, -2), float3(10, 0, -2), color);
		end::debug_renderer::add_line(float3(-10, 0, -1), float3(10, 0, -1), color);
		end::debug_renderer::add_line(float3(-10, 0, 0), float3(10, 0, 0), color);
		end::debug_renderer::add_line(float3(-10, 0, 1), float3(10, 0, 1), color);
		end::debug_renderer::add_line(float3(-10, 0, 2), float3(10, 0, 2), color);
		end::debug_renderer::add_line(float3(-10, 0, 3), float3(10, 0, 3), color);
		end::debug_renderer::add_line(float3(-10, 0, 4), float3(10, 0, 4), color);
		end::debug_renderer::add_line(float3(-10, 0, 5), float3(10, 0, 5), color);
		end::debug_renderer::add_line(float3(-10, 0, 6), float3(10, 0, 6), color);
		end::debug_renderer::add_line(float3(-10, 0, 7), float3(10, 0, 7), color);
		end::debug_renderer::add_line(float3(-10, 0, 8), float3(10, 0, 8), color);
		end::debug_renderer::add_line(float3(-10, 0, 9), float3(10, 0, 9), color);

		//corners
		end::debug_renderer::add_line(float3(10, 0, 10), float3(-10, 0, 10), color);
		end::debug_renderer::add_line(float3(-10, 0, 10), float3(-10, 0, -10), color);
		end::debug_renderer::add_line(float3(-10, 0, -10), float3(10, 0, -10), color);
		end::debug_renderer::add_line(float3(10, 0, -10), float3(10, 0, 10), color);
	}

	void dev_app_t::color_grid(float delta, float col_min, float col_max, float scalar = 1)
	{
		//increase red
		if (grid_r < col_max && grid_g == col_min && grid_b == col_min)
		{
			grid_r += delta * (scalar * (col_max - col_min));

			if (grid_r > col_max) grid_r = col_max;

			return;
		}

		//increase green
		if (grid_r == col_max && grid_g < col_max && grid_b == col_min)
		{
			grid_g += delta * (scalar * (col_max - col_min));

			if (grid_g > col_max) grid_g = col_max;

			return;
		}

		//increase blue
		if (grid_r == col_max && grid_g == col_max && grid_b < col_max)
		{
			grid_b += delta * (scalar * (col_max - col_min));

			if (grid_b > col_max) grid_b = col_max;

			return;
		}

		//decrease red
		if (grid_r > col_min && grid_g == col_max && grid_b == col_max)
		{
			grid_r -= delta * (scalar * (col_max - col_min));

			if (grid_r < col_min) grid_r = col_min;

			return;
		}

		//decrease green
		if (grid_r == col_min && grid_g > col_min && grid_b == col_max)
		{
			grid_g -= delta * (scalar * (col_max - col_min));

			if (grid_g < col_min) grid_g = col_min;

			return;
		}

		//decrease blue
		if (grid_r == col_min && grid_g == col_min && grid_b > col_min)
		{
			grid_b -= delta * (scalar * (col_max - col_min));

			if (grid_b < col_min) grid_b = col_min;

			return;
		}

		//in case
		grid_r = col_min;
		grid_g = col_min;
		grid_b = col_min;
	}

#pragma region Lab 1 FUNCTIONS
#if LAB_1

	void update_particle(particle* particle, float delta_time)
	{
		//test lifetime
		if (run_time >= particle->time_stamp + particle->life_time) {
			particle->alive = false;
			return;
		}

		//update parameters
		particle->prev_pos = particle->pos;
		particle->pos += particle->velocity * delta_time;
		particle->velocity += GRAVITY * delta_time;
	}

	void draw_particle(particle* particle) 
	{
		end::debug_renderer::add_line(particle->pos, particle->prev_pos, particle->color);
	}

#endif
#pragma endregion

#pragma region Lab 2 FUNCTIONS
#if LAB_2

	float vec_dot(float4 vec, float4 multi)
	{
		return vec.x * multi.x + vec.y * multi.y + vec.z * multi.z + vec.w * multi.w;
	}

	float4 matvec_dot(float4x4 matrix, float4 vec)
	{
		float4 product = float4();

		product.x = vec_dot(float4(matrix[MX].x, matrix[MY].x, matrix[MZ].x, matrix[MW].x), vec);
		product.y = vec_dot(float4(matrix[MX].y, matrix[MY].y, matrix[MZ].y, matrix[MW].y), vec);
		product.z = vec_dot(float4(matrix[MX].z, matrix[MY].z, matrix[MZ].z, matrix[MW].z), vec);
		product.w = vec_dot(float4(matrix[MX].w, matrix[MY].w, matrix[MZ].w, matrix[MW].w), vec);

		return product;
	}

	float4x4 matrix_multiply(float4x4 matrix, float4x4 multi)
	{
		float4x4 product = {
			float4(),
			float4(),
			float4(),
			float4()
		};

		product[MX].x = vec_dot(matrix[MX], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MX].y = vec_dot(matrix[MX], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MX].z = vec_dot(matrix[MX], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MX].w = vec_dot(matrix[MX], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		product[MY].x = vec_dot(matrix[MY], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MY].y = vec_dot(matrix[MY], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MY].z = vec_dot(matrix[MY], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MY].w = vec_dot(matrix[MY], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		product[MZ].x = vec_dot(matrix[MZ], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MZ].y = vec_dot(matrix[MZ], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MZ].z = vec_dot(matrix[MZ], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MZ].w = vec_dot(matrix[MZ], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		product[MW].x = vec_dot(matrix[MW], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MW].y = vec_dot(matrix[MW], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MW].z = vec_dot(matrix[MW], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MW].w = vec_dot(matrix[MW], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		return product;
	}

	float4x4_a matrix_multiply_a(float4x4_a matrix, float4x4_a multi)
	{
		float4x4_a product;

		product[MX].x = vec_dot(matrix[MX], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MX].y = vec_dot(matrix[MX], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MX].z = vec_dot(matrix[MX], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MX].w = vec_dot(matrix[MX], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		product[MY].x = vec_dot(matrix[MY], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MY].y = vec_dot(matrix[MY], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MY].z = vec_dot(matrix[MY], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MY].w = vec_dot(matrix[MY], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		product[MZ].x = vec_dot(matrix[MZ], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MZ].y = vec_dot(matrix[MZ], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MZ].z = vec_dot(matrix[MZ], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MZ].w = vec_dot(matrix[MZ], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		product[MW].x = vec_dot(matrix[MW], float4(multi[MX].x, multi[MY].x, multi[MZ].x, multi[MW].x));
		product[MW].y = vec_dot(matrix[MW], float4(multi[MX].y, multi[MY].y, multi[MZ].y, multi[MW].y));
		product[MW].z = vec_dot(matrix[MW], float4(multi[MX].z, multi[MY].z, multi[MZ].z, multi[MW].z));
		product[MW].w = vec_dot(matrix[MW], float4(multi[MX].w, multi[MY].w, multi[MZ].w, multi[MW].w));

		return product;
	}

	void draw_matrix(float4x4 matrix, float4 col_x, float4 col_y, float4 col_z)
	{
		float4 x = float4(1, 0, 0, 1);
		float4 y = float4(0, 1, 0, 1);
		float4 z = float4(0, 0, 1, 1);

		x = matvec_dot(matrix, x);
		y = matvec_dot(matrix, y);
		z = matvec_dot(matrix, z);

		float4 w = float4(0, 0, 0, 1);
		w = matvec_dot(matrix, w);

		//add lines
		end::debug_renderer::add_line(matrix[MW].xyz, x.xyz, col_x);
		end::debug_renderer::add_line(matrix[MW].xyz, y.xyz, col_y);
		end::debug_renderer::add_line(matrix[MW].xyz, z.xyz, col_z);
	}

	void matrix_position(float4x4& matrix, float3 position)
	{
		matrix[3].xyz = position;
	}

	void matrix_xrotation(float4x4& matrix, float angle)
	{
		float4x4 tmat = float4x4{
			float4(1, 0, 0, 0),
			float4(0, cos(angle), sin(angle), 0),
			float4(0, -sin(angle), cos(angle), 0),
			float4(0, 0, 0, 1)
		};

		matrix = matrix_multiply(matrix, tmat);
	}

	void matrix_yrotation(float4x4& matrix, float angle)
	{
		float4x4 tmat = float4x4{
			float4(cos(angle), 0, -sin(angle), 0),
			float4(0, 1, 0, 0),
			float4(sin(angle), 0, cos(angle), 0),
			float4(0, 0, 0, 1)
		};

		matrix = matrix_multiply(matrix, tmat);
	}

	void matrix_zrotation(float4x4& matrix, float angle)
	{
		float4x4 tmat = float4x4{
			float4(cos(angle), -sin(angle), 0, 0),
			float4(0, 1, 0, 0),
			float4(sin(angle), cos(angle), 1, 0),
			float4(0, 0, 0, 1)
		};

		matrix = matrix_multiply(matrix, tmat);
	}

	void matrix_position_a(float4x4_a& matrix, float3 position)
	{
		matrix[3].xyz = position;
	}

	void matrix_xrotation_a(float4x4_a& matrix, float angle)
	{
		float4x4_a tmat;

		tmat[0].x = 1;
		tmat[1].y = cos(angle);
		tmat[1].z = sin(angle);
		tmat[2].y = -sin(angle);
		tmat[2].z = cos(angle);
		tmat[3].w = 1;

		matrix = matrix_multiply_a(matrix, tmat);
	}

	void matrix_yrotation_a(float4x4_a& matrix, float angle)
	{
		float4x4_a tmat;

		tmat[0].x = cos(angle);
		tmat[0].z = -sin(angle);
		tmat[1].y = 1;
		tmat[2].x = sin(angle);
		tmat[2].z = cos(angle);
		tmat[3].w = 1;

		matrix = matrix_multiply_a(matrix, tmat);
	}

	void matrix_zrotation_a(float4x4_a& matrix, float angle)
	{
		float4x4_a tmat;

		tmat[0].x = cos(angle);
		tmat[0].y = sin(angle);
		tmat[1].x = -sin(angle);
		tmat[1].y = cos(angle);
		tmat[2].z = 1;
		tmat[3].w = 1;

		matrix = matrix_multiply_a(matrix, tmat);
	}

	void matrix_lookat(float4x4& matrix, float3 target)
	{
		float3 z = z.normalize(target - matrix[MW].xyz);
		float3 x = x.normalize(z.cross(WORLDUP, z));
		float3 y = y.normalize(x.cross(z, x));

		matrix[MX].xyz = x;
		matrix[MY].xyz = y;
		matrix[MZ].xyz = z;
	}

	void matrix_turnto(float3 target, float3 pos, float& x, float& y)
	{
		float4x4 temp = MATRIX_I;

		matrix_xrotation(temp, x);
		matrix_yrotation(temp, y);
		matrix_position(temp, pos);

		float3 v = target - temp[MW].xyz;
		float fx = v.dot(temp[MY].xyz, v);
		float fy = v.dot(temp[MX].xyz, v);

		x += fx < 0 ? 0.001 : 0;
		x += fx > 0 ? -0.001 : 0;
		y += fy > 0 ? 0.001 : 0;
		y += fy < 0 ? -0.001 : 0;
	}

	void dev_app_t::input_handler(float4x4_a& view, float delta)
	{
		if (in_up)
			player_pos += player[MZ].xyz * charspeed * delta;
		if (in_down)
			player_pos -= player[MZ].xyz * charspeed * delta;
		if (in_left)
			player_yangle -= charrotatespeed * delta;
		if (in_right)
			player_yangle += charrotatespeed * delta;

		if (in_mouse) {
			if (in_w)
				cam_pos += view[MZ].xyz * camspeed * delta;
			if (in_s)
				cam_pos -= view[MZ].xyz * camspeed * delta;
			if (in_d)
				cam_pos += view[MX].xyz * camspeed * delta;
			if (in_a)
				cam_pos -= view[MX].xyz * camspeed * delta;

			cam_xangle += (in_mousex - mouse_pos.x) * camturn_speed;
			cam_yangle += (in_mousey - mouse_pos.y) * camturn_speed;
		}

		mouse_pos.x = in_mousex;
		mouse_pos.y = in_mousey;
	}

#endif
#pragma endregion

#pragma region Lab 3 FUNCTIONS
#if LAB_3

	void draw_aabb(aabb_t aabb, float4 color)
	{
		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + -aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			color
		);

		debug_renderer::add_line(
			float3(aabb.center.x + -aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			float3(aabb.center.x + aabb.extents.x / 2, aabb.center.y + aabb.extents.y / 2, aabb.center.z + -aabb.extents.z / 2),
			color
		);
	}

	plane_t end::calculate_plane(float3 a, float3 b, float3 c)
	{
		plane_t result;

		result.normal = a.normalize(a.cross(a - b, c - b));
		result.offset - a.dot(a, result.normal);

		return result;
	}

	void calculate_frustum(frustum_t& frustum, const float4x4& view)
	{
		//calculate frustum
		float3 nearcenter = view[MW].xyz - view[MZ].xyz * nearclip;
		float3 farcenter = view[MW].xyz - view[MZ].xyz * farclip;

		float near_h = 2 * tan(fov / 2) * nearclip;
		float far_h = 2 * tan(fov / 2) * farclip;
		float near_w = near_h * asp;
		float far_w = far_h * asp;

		frustum.points[0] = farcenter + view[MY].xyz * (far_h * 0.5) - view[MX].xyz * (far_w * 0.5);
		frustum.points[1] = farcenter + view[MY].xyz * (far_h * 0.5) + view[MX].xyz * (far_w * 0.5);
		frustum.points[2] = farcenter - view[MY].xyz * (far_h * 0.5) - view[MX].xyz * (far_w * 0.5);
		frustum.points[3] = farcenter - view[MY].xyz * (far_h * 0.5) + view[MX].xyz * (far_w * 0.5);

		frustum.points[4] = nearcenter + view[MY].xyz * (near_h * 0.5) - view[MX].xyz * (near_w * 0.5);
		frustum.points[5] = nearcenter + view[MY].xyz * (near_h * 0.5) + view[MX].xyz * (near_w * 0.5);
		frustum.points[6] = nearcenter - view[MY].xyz * (near_h * 0.5) - view[MX].xyz * (near_w * 0.5);
		frustum.points[7] = nearcenter - view[MY].xyz * (near_h * 0.5) + view[MX].xyz * (near_w * 0.5);

		frustum.planes[0] = calculate_plane(frustum.points[6], frustum.points[2], frustum.points[0]);
		frustum.planes[1] = calculate_plane(frustum.points[4], frustum.points[0], frustum.points[1]);
		frustum.planes[2] = calculate_plane(frustum.points[5], frustum.points[1], frustum.points[3]);
		frustum.planes[3] = calculate_plane(frustum.points[7], frustum.points[3], frustum.points[2]);

		float3 point = view[MW].xyz + nearcenter.normalize(view[MZ].xyz) * nearclip;
		frustum.planes[4].offset = nearcenter.dot(point, nearcenter.normalize(view[MZ].xyz));
		frustum.planes[4].normal = view[MZ].xyz;

		point = view[MW].xyz + nearcenter.normalize(view[MZ].xyz) * nearclip;
		frustum.planes[5].offset = nearcenter.dot(point, nearcenter.normalize(view[MZ].xyz));
		frustum.planes[5].normal = float3(-view[MZ].x, -view[MZ].y, -view[MZ].z);

		//draw the frustum
		float4 c1 = float4(0.56, 0, 1, 1);
		float4 c2 = float4(0, 0.83, 1, 1);
		debug_renderer::add_line(frustum.points[0], frustum.points[1], c1);
		debug_renderer::add_line(frustum.points[0], frustum.points[2], c1);
		debug_renderer::add_line(frustum.points[3], frustum.points[1], c1);
		debug_renderer::add_line(frustum.points[2], frustum.points[3], c1);

		debug_renderer::add_line(frustum.points[4], frustum.points[5], c1);
		debug_renderer::add_line(frustum.points[4], frustum.points[6], c1);
		debug_renderer::add_line(frustum.points[7], frustum.points[5], c1);
		debug_renderer::add_line(frustum.points[6], frustum.points[7], c1);

		debug_renderer::add_line(frustum.points[4], frustum.points[0], c1);
		debug_renderer::add_line(frustum.points[5], frustum.points[1], c1);
		debug_renderer::add_line(frustum.points[6], frustum.points[2], c1);
		debug_renderer::add_line(frustum.points[7], frustum.points[3], c1);

		//draw normals
		point = (frustum.points[4] + frustum.points[6] + frustum.points[0] + frustum.points[2]) / 4;
		debug_renderer::add_line(point, point + frustum.planes[0].normal, c2);

		point = (frustum.points[4] + frustum.points[5] + frustum.points[0] + frustum.points[1]) / 4;
		debug_renderer::add_line(point, point + frustum.planes[1].normal, c2);

		point = (frustum.points[5] + frustum.points[7] + frustum.points[1] + frustum.points[3]) / 4;
		debug_renderer::add_line(point, point + frustum.planes[2].normal, c2);

		point = (frustum.points[5] + frustum.points[6] + frustum.points[3] + frustum.points[2]) / 4;
		debug_renderer::add_line(point, point + frustum.planes[3].normal, c2);

		point = (frustum.points[4] + frustum.points[5] + frustum.points[6] + frustum.points[7]) / 4;
		debug_renderer::add_line(point, point + frustum.planes[4].normal, c2);

		point = (frustum.points[0] + frustum.points[1] + frustum.points[2] + frustum.points[3]) / 4;
		debug_renderer::add_line(point, point + frustum.planes[5].normal, c2);
	}

	float get_r(const aabb_t& aabb, const frustum_t& frustum, int plane) {
		float3 math;
		float res = math.dot(
			float3(abs(frustum.planes[plane].normal.x), abs(frustum.planes[plane].normal.y), abs(frustum.planes[plane].normal.z)),
			aabb.extents);
		return res;
	}

	float get_res(const aabb_t& aabb, const frustum_t& frustum, int plane, int point) {
		float3 math;
		return math.dot((aabb.center - frustum.points[point]), frustum.planes[plane].normal);
	}

	bool end::aabb_to_frustum(const aabb_t& aabb, const frustum_t& frustum)
	{
		float3 math;
		int point = 0;
		int plane = 0;
		float result;
		float r = get_r(aabb, frustum, plane);
		result = get_res(aabb, frustum, plane, point);
		if (result < -r / 2) return false;


		point = 0;
		plane = 1;
		result = 0;
		r = get_r(aabb, frustum, plane);
		result = get_res(aabb, frustum, plane, point);
		if (result < -r / 2) return false;
		
		point = 1;
		plane = 2;
		result = 0;
		r = get_r(aabb, frustum, plane);
		result = get_res(aabb, frustum, plane, point);
		if (result < -r / 2) return false;
		
		point = 2;
		plane = 3;
		result = 0;
		r = get_r(aabb, frustum, plane);
		result = get_res(aabb, frustum, plane, point);
		if (result < -r / 2) return false;
		
		point = 4;
		plane = 4;
		result = 0;
		r = get_r(aabb, frustum, plane);
		result = get_res(aabb, frustum, plane, point);
		if (result < -r / 2) return false;
		
		point = 0;
		plane = 5;
		result = 0;
		r = get_r(aabb, frustum, plane);
		result = get_res(aabb, frustum, plane, point);
		if (result < -r / 2) return false;

		return true;
	}

#endif
#pragma endregion

	void dev_app_t::start()
	{
		MATRIX_IA[0].x = 1;
		MATRIX_IA[1].y = 1;
		MATRIX_IA[2].z = 1;
		MATRIX_IA[3].w = 1;

		grid_r = 0;
		grid_g = 0;
		grid_b = 0;

#pragma region Lab 1 START
#if LAB_1

		srand(time(NULL));

		//create the particle emitters and particles
		for (int i = 0; i < PARTICLE_COUNT; i++) {
			shared_pool[i].alive = false;
			shared_pool[i].life_time = 2;

			float val_x = (rand() % 5 + 1) - 2.5;
			float val_z = (rand() % 5 + 1) - 2.5;
			shared_pool[i].init_velocity = float3(val_x, 10, val_z);
		}

		for (int i = 0; i < MAX_EMITTERS; i++) {
			emitters[i] = new emitter();
		}

		emitters[0]->spawn_pos = float3(0, 0, 0);
		emitters[0]->spawn_color = float4(1, 1, 1, 1);
		emitters[0]->spawn_time = 0.01f;
		emitters[0]->time_stamp = 0;

		emitters[1]->spawn_pos = float3(7.5f, 0, 7.5f);
		emitters[1]->spawn_color = float4(1, 0.5, 0.5, 1);
		emitters[1]->spawn_time = 0.01f;
		emitters[1]->time_stamp = 0;

		emitters[2]->spawn_pos = float3(-7.5f, 0, 7.5f);
		emitters[2]->spawn_color = float4(0.5, 1, 0.5, 1);
		emitters[2]->spawn_time = 0.01f;
		emitters[2]->time_stamp = 0;

		emitters[3]->spawn_pos = float3(7.5f, 0, -7.5f);
		emitters[3]->spawn_color = float4(0.5, 0.5, 1, 1);
		emitters[3]->spawn_time = 0.01f;
		emitters[3]->time_stamp = 0;

		emitters[4]->spawn_pos = float3(-7.5f, 0, -7.5f);
		emitters[4]->spawn_color = float4(1, 0.5, 1, 1);
		emitters[4]->spawn_time = 0.01f;
		emitters[4]->time_stamp = 0;

#endif
#pragma endregion

#pragma region Lab 2 START
#if LAB_2

		//set inital parameters
		matrix_position(look_at, float3(-2, 1, -2));

		charspeed = 3;
		charrotatespeed = 3;
		camspeed = 3;

		player_pos = float3(0, 0, 0);
		player_yangle = 0;

		turnto_pos = float3(2, 1, 2);
		float turnto_yangle = 0;
		float turnto_xangle = 0;

		cam_pos = float3(0, 5, -10);
		mouse_pos = float2();
		cam_xangle = 0;
		cam_yangle = 0;
		camturn_speed = 0.005;

#endif
#pragma endregion

#pragma region Lab 3 START
#if LAB_3

		//create aabbs
		srand(time(NULL));
		for (int i = 0; i < aabbarr_size; i++) {
			aabbs[i].extents = float3(rand() % 3 + 1, rand() % 3 + 1, 1);
			aabbs[i].center = float3((rand() % 10) - 5 / 2, aabbs[i].extents.y / 2, (rand() % 10) - 5);
		}

#endif
#pragma endregion

		frame_one = false;
	}

	void dev_app_t::update(float4x4_a& view)
	{	
		//initial setup
		if (frame_one) {
			start();
		}

		delta_time = calc_delta_time();
		run_time += delta_time;

		//clear buffer
		debug_renderer::clear_lines();

		//draw the grid
		dev_app_t::color_grid(delta_time, 0.25, 0.5, 0.75);
		dev_app_t::draw_grid(float4(grid_r, grid_g, grid_b, 1));
		
#pragma region Lab 1 UPDATE
#if LAB_1
		
		//update emitters
		for (int i = 0; i < MAX_EMITTERS; i++) {
			if (run_time >= emitters[i]->time_stamp + emitters[i]->spawn_time) {
				int index = shared_pool.alloc();
				shared_pool[index].alive = true;
				shared_pool[index].color = emitters[i]->spawn_color;
				shared_pool[index].pos = emitters[i]->spawn_pos;
				shared_pool[index].velocity = shared_pool[index].init_velocity;
				shared_pool[index].time_stamp = run_time;
				emitters[i]->indicies[emitters[i]->indicies.alloc()] = index;
				emitters[i]->time_stamp = run_time;
			}
		}

		//update and draw particles
		for (int i = 0; i < MAX_EMITTERS; i++) {
			for (int j = 0; j < emitters[i]->indicies.size(); j++) {
				if (!shared_pool[emitters[i]->indicies[j]].alive) {
					shared_pool.free(emitters[i]->indicies[j]);
					emitters[i]->indicies.free(j);
				}

				update_particle(&shared_pool[emitters[i]->indicies[j]], delta_time);
				draw_particle(&shared_pool[emitters[i]->indicies[j]]);
			}
		}

#endif
#pragma endregion

#pragma region Lab 2 UPDATE
#if LAB_2

		//handle inputs
		input_handler(view, delta_time);

		//reset positional matrices
		player = MATRIX_I;
		turn_to = MATRIX_I;
		view = MATRIX_IA;

		//update player
		matrix_yrotation(player, player_yangle);
		matrix_position(player, player_pos);

		//update turn to
		matrix_turnto(player_pos, turnto_pos, turnto_xangle, turnto_yangle);
		matrix_xrotation(turn_to, turnto_xangle);
		matrix_yrotation(turn_to, turnto_yangle);
		matrix_position(turn_to, turnto_pos);

		//update look_at
		matrix_lookat(look_at, player_pos);

		//camera
		matrix_xrotation_a(view, cam_xangle);
		matrix_yrotation_a(view, cam_yangle);
		matrix_position_a(view, cam_pos);

		//draw matrices
		draw_matrix(player, RED, GREEN, BLUE);
		draw_matrix(look_at, RED, GREEN, BLUE);
		draw_matrix(turn_to, RED, GREEN, BLUE);

#endif
#pragma endregion

#pragma region Lab 3 UPDATE
#if LAB_3
		calculate_frustum(f, player);

		for (int i = 0; i < aabbarr_size; i++) {
			bool col_check = aabb_to_frustum(aabbs[i], f);

			if (col_check) draw_aabb(aabbs[i], GREEN);
			else draw_aabb(aabbs[i], RED);
		}
#endif
#pragma endregion
	}
}
