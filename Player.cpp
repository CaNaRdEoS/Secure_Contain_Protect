#include <string>
#include "Player.h"
#include <raylib.h>

Player::Player(int x_p, int y_p, int w_p, int h_p, int v_p, std::string t_p)
	:x(x_p), y(y_p), w(w_p), h(h_p), v(v_p), t(t_p)
{}

int Player::getX()
{
	return x;
}

int Player::getY()
{
	return y;
}

int Player::getW()
{
	return w;
}

int Player::getH()
{
	return h;
}

int Player::getV()
{
	return v;
}

std::string Player::getT()
{
	return t;
}

void Player::moveX(int distance)
{
	x += distance;
}

void Player::moveY(int distance)
{
	y += distance;
}