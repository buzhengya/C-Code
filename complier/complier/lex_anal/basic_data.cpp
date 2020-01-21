#include "basic_data.h"
#include "log.h"

//#include <iomanip>

using namespace std;

void CCell::Print() const
{
	LOG_INFO << "start state : " << m_nStartState << " end state : " << m_nEndState;
	for (size_t i = 0; i < m_vecEdge.size(); ++i)
	{
		LOG_INFO << "edge : " << i + 1 << " start : " << m_vecEdge[i].GetStart()
			<< " end : " << m_vecEdge[i].GetEnd() << " input : " << m_vecEdge[i].GetInput();
	}
}
