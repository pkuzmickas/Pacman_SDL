#pragma once
class Ghost {
private:
	int id; // 1 - red
	bool scatterMode;
public:
	void findNextMove();
};