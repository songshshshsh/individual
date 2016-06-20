#ifndef WAY_H
#define WAY_H

#include "Strategy.h"

//Strategy design pattern
class Way
{
public:
	Way(Strategy* strategy_): m_strategy(strategy_){}
	~Way()
	{
		if (m_strategy) delete m_strategy;
	}
	void setWay(Strategy* strategy_)
	{
		if (m_strategy) delete m_strategy;
		m_strategy = strategy_;
	}
	Strategy* getWay()
	{
		return m_strategy;
	}
private:
	Strategy*  m_strategy;
};

#endif