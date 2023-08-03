#pragma once
#include <iostream>
#include <cstdint>
#include <chrono>
#include <vector>
#include "math_types.h"
#include "debug_renderer.h"
#include "pools.h"
#include "frustum_culling.h"

namespace end
{
	// Simple app class for development and testing purposes
	struct dev_app_t
	{
		bool in_w = false;
		bool in_a = false;
		bool in_s = false;
		bool in_d = false;
		bool in_up = false;
		bool in_down = false;
		bool in_left = false;
		bool in_right = false;
		bool in_mouse = false;

		float charspeed = 0;
		float charrotatespeed = 0;
		float camspeed = 0;

		float in_mousex, in_mousey;

		float4x4_a MATRIX_IA;

		void update(float4x4_a& view);

		dev_app_t();

		double get_delta_time()const;

		//added
		void start();
		void draw_grid(float4 color);
		void color_grid(float delta, float col_min, float col_max, float scalar);

		//LAB 2
		void input_handler(float4x4_a& view, float delta);
		void input_handler_l2(float delta);
	};
}