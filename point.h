#ifndef POINT_H
#define POINT_H

class Point
{
public:
	Point(int _x,int _y,int _index)
	{
		this->x = _x;
		this->y = _y;
		this->index = _index;
	}
	Point(int _x,int _y)
	{
		this->x = _x;
		this->y = _y;
		this->index = 0;
	}
	void setx(int _x)
	{
		this->x = _x;
	}
	void sety(int _y)
	{
		this->y = _y;
	}
	void setindex(int _index)
	{
		this->index = _index;
	}
	int getx() const
	{
		return x;
	}
	int gety() const
	{
		return y;
	}
	int getindex() const
	{
		return index;
	}
	friend int operator==(const Point& a,const Point& b)
	{
		return (a.getx() == b.getx() && a.gety() == b.gety());
	}
private:
	int x,y;
	int index;
};

#endif