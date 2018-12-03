#ifndef JOOWON_AI
#define JOOWON_AI
#include "OthelloAI.hpp"
#include <vector>


namespace joowon
{
	class JoowonAI:public OthelloAI
	{
	public:
		virtual std::pair<int,int> chooseMove(const OthelloGameState& state);
		int search(std::unique_ptr<OthelloGameState>& s,int depth, std::string turn);
		int evaluate_score(std::unique_ptr<OthelloGameState>& c);
		std::vector<std::pair<int,int>> generate_moves(std::unique_ptr<OthelloGameState>& s);
	};

}








#endif //JOOWON_AI
