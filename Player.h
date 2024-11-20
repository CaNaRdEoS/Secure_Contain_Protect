#include <string>
#include <raylib.h>

class Player
{
public:
	Player(int x, int y, int w, int h, int v, std::string t);
	int getX();
	int getY();
	int getW();
	int getH();
	int getV();
	std::string getT();

	void moveX(int distance);
	void moveY(int distance);


private:
	int x, y, w, h, v;
	std::string t;
};