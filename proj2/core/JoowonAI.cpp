#include "JoowonAI.hpp"
#include <ics46/factory/DynamicFactory.hpp>
#include <iostream>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, joowon::JoowonAI, "JOOWON AI(REQUIRED)");

std::pair<int, int> joowon::JoowonAI::chooseMove(const OthelloGameState& state)
{
	int score;
	int best_score = -9999;
	std::string turn;
	if(state.isWhiteTurn())
		turn = "white";
	else
		turn = "black";
	std::unique_ptr<OthelloGameState> othello_copy = state.clone();
	std::vector<std::pair<int,int>> avail_moves = joowon::JoowonAI::generate_moves(othello_copy);
	std::pair<int,int> optimal_move;
	for (auto element: avail_moves)
	{
		std::unique_ptr<OthelloGameState> second_copy = state.clone();
		second_copy -> makeMove(element.first,element.second);
		score = search(second_copy, 2, turn);
		if(score >= best_score)
		{
			best_score = score;
			optimal_move.first = element.first;
			optimal_move.second = element.second;
		}
	}
	return optimal_move;

}
int joowon::JoowonAI::search(std::unique_ptr<OthelloGameState>& s, int depth, std::string turn)
{
	std::vector<std::pair<int,int>> potential_moves = joowon::JoowonAI::generate_moves(s);
	int my_score = -9999;
	int enemy_score = 9999;
	int score;
	if (depth == 0)
		return 1;
		//std::cout << "yes";
	else
	{
		if ((s->isWhiteTurn() && turn == "white") || (s->isBlackTurn() && turn == "black"))
		{
			for (auto element: potential_moves)
			{
			std::unique_ptr<OthelloGameState> copy = s->clone();
			copy -> makeMove(element.first, element.second);
			score = search(copy, depth - 1, turn);
			if (score >= my_score)
				my_score = score;
			}
		return my_score;
		}
		else
		{
			for (auto element: potential_moves)
			{
			std::unique_ptr<OthelloGameState> copy = s->clone();
			copy -> makeMove(element.first, element.second);
			score = search(copy, depth - 1, turn);
			if (score <= my_score)
				enemy_score = score;
			}
			return enemy_score;
		}
	}
}
int joowon::JoowonAI::evaluate_score(std::unique_ptr<OthelloGameState>& c)
{
	/*int width = c->board().width();
	int height = c->board().height();
	std::vector<std::pair<int,int>> moves = joowon::JoowonAI::generate_moves(c);*/
	if (c->isWhiteTurn())
		return (c->whiteScore() - c->blackScore());
	else
		return (c->blackScore() - c-> whiteScore());

}
std::vector<std::pair<int,int>> joowon::JoowonAI::generate_moves(std::unique_ptr<OthelloGameState>& s)
{
	std::vector<std::pair<int,int>> moves;
	for (int x = 0; x < s->board().width(); ++x)
		for (int y = 0; y < s->board().height(); ++y)
			if (s->isValidMove(x,y))
				moves.push_back(std::pair<int,int> (x,y));
	return moves;

}
