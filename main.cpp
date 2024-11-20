#include "Player.h"
#include <iostream>
#include <raylib.h>

void movingSprite(Player& player, Rectangle& sprite, int& acceleration)
{
	acceleration++;
	if (acceleration < 10)
	{
		sprite.x = 0;
	} else if (acceleration < 20)
	{
		sprite.x = player.getW();
	} else if (acceleration < 30)
	{
		sprite.x = player.getW()*2;
	} else if (acceleration < 40)
	{
		sprite.x = player.getW()*3;
	} else
	{
		acceleration = 0;
	}
}

int main()
{
	Player guard(250, 100, 41, 69, 3, "./assets/SCP_Guard_Walking.png");
	InitWindow(500, 200, "SCP : Secure Contain Protect");

	//Traitement de l'image du garde
	Image guard_img = LoadImage(guard.getT().c_str());
	Texture2D guard_texture = LoadTextureFromImage(guard_img);
	UnloadImage(guard_img);
	Rectangle guard_sprite = {0, 0, 40, 67};
	Rectangle guard_hitbox = {guard.getX()+4, guard.getY()+guard.getH()-5, guard.getW()-8, 7};
	int acceleration = 0;

	//Traitement de l'image du couloir 500 175
	Image corridor_img = LoadImage("./assets/SCP_Corridor.png");
	Texture2D corridor_texture = LoadTextureFromImage(corridor_img);
	Rectangle up_hitbox = {0, 114, 500, 10};
	Rectangle down_hitbox = {0, 182, 500, 10};
	Rectangle right_hitbox = {354, 120, 10, 62};
	Rectangle left_hitbox = {0, 120, 10, 62};
	UnloadImage(corridor_img);


	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_S) && !IsKeyDown(KEY_DOWN))
		{
			acceleration = 0;
		}

		if (IsKeyDown(KEY_D)|| IsKeyDown(KEY_RIGHT))
		{
			guard_hitbox.x += guard.getV();
			if (CheckCollisionRecs(guard_hitbox, right_hitbox))
			{
				guard_hitbox.x -= guard.getV();
			} else
			{
				guard.moveX(guard.getV());
				guard_sprite.y = 0;
				std::cout << guard_sprite.y << std::endl;
				movingSprite(guard, guard_sprite, acceleration);
			}
		}
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		{
			guard_hitbox.x -= guard.getV();
			if (CheckCollisionRecs(guard_hitbox, left_hitbox))
			{
				guard_hitbox.x += guard.getV();
			} else
			{
				guard.moveX(-guard.getV());
				guard_sprite.y = guard.getH();
				std::cout << guard_sprite.y << std::endl;
				movingSprite(guard, guard_sprite, acceleration);
			}
		}
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		{
			guard_hitbox.y -= guard.getV();
			if (CheckCollisionRecs(guard_hitbox, up_hitbox))
			{
				guard_hitbox.y += guard.getV();
			} else
			{
				guard.moveY(-guard.getV());
				guard_sprite.y = guard.getH()*2;
				std::cout << guard_sprite.y << std::endl;
				movingSprite(guard, guard_sprite, acceleration);
			}
		}
		if (IsKeyDown(KEY_S)|| IsKeyDown(KEY_DOWN))
		{
			guard_hitbox.y += guard.getV();
			if (CheckCollisionRecs(guard_hitbox, down_hitbox))
			{
				guard_hitbox.y -= guard.getV();
			} else
			{
				guard.moveY(guard.getV());
				guard_sprite.y = guard.getH()*3;
				std::cout << guard_sprite.y << std::endl;
				movingSprite(guard, guard_sprite, acceleration);
			}
		}

		//Draw
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexture(corridor_texture, 0, 10, WHITE);
		DrawTextureRec(guard_texture, guard_sprite, {guard.getX(), guard.getY()}, WHITE);

		//Hit boxes
		//DrawRectangle(guard_hitbox.x, guard_hitbox.y, guard_hitbox.width, guard_hitbox.height, RED);
		//DrawRectangle(up_hitbox.x, up_hitbox.y, up_hitbox.width, up_hitbox.height, WHITE);
		//DrawRectangle(down_hitbox.x, down_hitbox.y, down_hitbox.width, down_hitbox.height, WHITE);
		//DrawRectangle(right_hitbox.x, right_hitbox.y, right_hitbox.width, right_hitbox.height, WHITE);
		//DrawRectangle(left_hitbox.x, left_hitbox.y, left_hitbox.width, left_hitbox.height, WHITE);
		EndDrawing();
	}
	UnloadTexture(guard_texture);
	UnloadTexture(corridor_texture);
	CloseWindow();
	return 0;
}