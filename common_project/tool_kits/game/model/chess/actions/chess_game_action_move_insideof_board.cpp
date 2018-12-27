#include "chess_game_action_move_insideof_board.h"
#include "chess_action_error.h"
#include "common/macros/assert.h"
#include "game/model/chess/chess_game_board.h"
#include "game/model/chess/chess_trait.h"
#include "game/model/chess/stockfish_converter.h"
#include "stockfish/adapter/position_adapter.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO(ChessGameActionType::MoveInsideOfBoard, ChessActionMoveInsideOfBoard)

GameActionResult ChessActionMoveInsideOfBoard::DoOnBoard(ChessGameBoard* game_board)
{
	GameActionResult result = game_board->IsRuleBased() ? DoOnBoardWithRule(game_board) : DoOnBoardWithoutRule(game_board);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
	}

	return result;
}

GameActionResult ChessActionMoveInsideOfBoard::DoOnBoardWithoutRule(ChessGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		auto ope_chess_id = this->ChessId();
		auto grid_from = this->GridFrom();
		auto grid_to = this->GridTo();

		ChessInfoAdapter chess_info_from(game_board->ChessAt(grid_from));
		if (!chess_info_from)
		{
			result.SetErrorInfo(chess_actoin_error::ParamErrorOnFromGrid);
			break;
		}

		if (chess_info_from.IsEmpty())
		{
			result.SetErrorInfo(chess_actoin_error::NotExistingChessOnFromGrid);
			break;
		}

		if (chess_info_from.GetId() != ope_chess_id)
		{
			result.SetErrorInfo(chess_actoin_error::ChessIdMismatchingOnFromGrid);
			break;
		}

		ChessInfoAdapter chess_info_to(game_board->ChessAt(grid_to));
		if (!chess_info_to)
		{
			result.SetErrorInfo(chess_actoin_error::ParamErrorOnToGrid);
			break;
		}

		if (chess_info_to.IsNotEmpty() && ChessTrait::IsSameColor(chess_info_from, chess_info_to))
		{
			result.SetErrorInfo(chess_actoin_error::CannotTakeChessOfOwnColorOnToGridWithoutRule);
			break;
		}

		{
			chess_info_from.SetToEmpty();
			chess_info_to.SetId(ope_chess_id);

			result.AddChessVariation({ grid_to, grid_from, chess_info_from, ChessVariation::Reason::MovedInsideOfBoardActively });
			result.AddChessVariation({ GridIndex::InvalidGrid, grid_to, chess_info_to, ChessVariation::Reason::CapturedPassively });
		}

		result.MakeValid();
	} while (false);

	return result;
}

GameActionResult ChessActionMoveInsideOfBoard::DoOnBoardWithRule(ChessGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		auto rule_board = game_board->CurrentRuleBoard();
		D_ASSERT(rule_board, L"rule_board should not be null");

		auto ope_chess_id = this->ChessId();
		auto grid_from = this->GridFrom();
		auto grid_to = this->GridTo();

		auto chess_info_from = game_board->ChessAt(grid_from);
		if (!chess_info_from)
		{
			result.SetErrorInfo(chess_actoin_error::ParamErrorOnFromGrid);
			break;
		}

		if (chess_info_from->IsEmpty())
		{
			result.SetErrorInfo(chess_actoin_error::NotExistingChessOnFromGrid);
			break;
		}

		auto chess_info_to = game_board->ChessAt(grid_to);
		if (!chess_info_to)
		{
			result.SetErrorInfo(chess_actoin_error::ParamErrorOnToGrid);
			break;
		}

		const auto square_from = stk_cvter::GridToSquare(grid_from);
		const auto square_to = stk_cvter::GridToSquare(grid_to);

		if (false == rule_board->IsMoveLegal(square_from, square_to))
		{
			result.SetErrorInfo(chess_actoin_error::IllegalMoveWithRule);
			break;
		}

		stk::MoveDetail move_detial;
		if (chess_info_from->GetId() != ope_chess_id 
			&& !rule_board->CanMoveToPromote(square_from, square_to, stk_cvter::ChessIdToPiece(ope_chess_id), &move_detial))
		{
			result.SetErrorInfo(chess_actoin_error::ChessIdMismatchingOnFromGridAndNotPromotionNeither);
			break;
		}

		{
			if (!move_detial.IsValid())
			{
				move_detial = rule_board->GetBestMove(square_from, square_to);
			}

			auto grid_captured = stk_cvter::SquareToGrid(move_detial.square_captured_);
			auto old_chess_id_from = game_board->ChessAt(grid_from)->GetId();
			auto old_chess_id_to = game_board->ChessAt(grid_to)->GetId();
			auto old_chess_id_captured = grid_captured ? game_board->ChessAt(grid_captured)->GetId() : ChessInfo::EmptyId;

			rule_board->DoMove(move_detial.move_);

			Piece pieces[SQUARE_NB];
			rule_board->CurrentPieces(pieces);
			for (auto square = SQ_A1; square <= SQ_H8; ++square)
			{
				auto grid = stk_cvter::SquareToGrid(square);
				auto chess_id = stk_cvter::PieceToChessId(pieces[square]);
				game_board->ChessAt(grid)->SetId(chess_id);
			}

			auto new_chess_id_from = game_board->ChessAt(grid_from)->GetId();
			auto new_chess_id_to = game_board->ChessAt(grid_to)->GetId();
			auto new_chess_id_captured = grid_captured ? game_board->ChessAt(grid_captured)->GetId() : ChessInfo::EmptyId;

			if (move_detial.is_short_castling_)
			{
				auto& grid_from_king = grid_from;
				auto grid_to_king = grid_from_king + GridIndex(2, 0);
				result.AddChessVariation({ grid_to_king, grid_from_king, old_chess_id_from, new_chess_id_from, ChessVariation::Reason::MovedInsideOfBoardActively });
				result.AddChessVariation({ GridIndex::InvalidGrid, grid_to_king, ChessInfo::EmptyId, old_chess_id_from, ChessVariation::Reason::CapturedPassively });

				auto& grid_from_rook = grid_to;
				auto grid_to_rook = grid_from_rook + GridIndex(-2, 0);
				result.AddChessVariation({ grid_to_rook, grid_from_rook, old_chess_id_to, new_chess_id_to, ChessVariation::Reason::MovedInsideOfBoardActively });
				result.AddChessVariation({ GridIndex::InvalidGrid, grid_to_rook, ChessInfo::EmptyId, old_chess_id_to, ChessVariation::Reason::CapturedPassively });
			}
			else if (move_detial.is_long_castling_)
			{
				auto& grid_from_king = grid_from;
				auto grid_to_king = grid_from_king + GridIndex(-2, 0);
				result.AddChessVariation({ grid_to_king, grid_from_king, old_chess_id_from, new_chess_id_from, ChessVariation::Reason::MovedInsideOfBoardActively });
				result.AddChessVariation({ GridIndex::InvalidGrid, grid_to_king, ChessInfo::EmptyId, old_chess_id_from, ChessVariation::Reason::CapturedPassively });

				auto& grid_from_rook = grid_to;
				auto grid_to_rook = grid_from_rook + GridIndex(3, 0);
				result.AddChessVariation({ grid_to_rook, grid_from_rook, old_chess_id_to, new_chess_id_to, ChessVariation::Reason::MovedInsideOfBoardActively });
				result.AddChessVariation({ GridIndex::InvalidGrid, grid_to_rook, ChessInfo::EmptyId, old_chess_id_to, ChessVariation::Reason::CapturedPassively });
			}
			else
			{
				result.AddChessVariation({ grid_to, grid_from, old_chess_id_from, new_chess_id_from, ChessVariation::Reason::MovedInsideOfBoardActively }); // including promotion
				result.AddChessVariation({ GridIndex::InvalidGrid, grid_to, old_chess_id_to, new_chess_id_to, ChessVariation::Reason::CapturedPassively });

				if (move_detial.is_captured_ && move_detial.square_captured_ != move_detial.square_to_) // ENPASSANT
				{
					result.AddChessVariation({ GridIndex::InvalidGrid, grid_captured, old_chess_id_captured, new_chess_id_captured, ChessVariation::Reason::CapturedPassively });
				}
			}
		}

		result.MakeValid();
	} while (false);

	return result;
}

void ChessActionMoveInsideOfBoard::UndoOnBoard(ChessGameBoard* game_board)
{
	if (game_board->IsRuleBased())
	{
		auto rule_board = game_board->CurrentRuleBoard();
		D_ASSERT(rule_board, L"rule_board should not be null");
		rule_board->UndoLastMove(nullptr);
	}

	game_board->UndoChessesViaActionResult(*result_ptr_);
}

void ChessActionMoveInsideOfBoard::RedoOnBoard(ChessGameBoard* game_board)
{
	if (game_board->IsRuleBased())
	{
		do 
		{
			auto rule_board = game_board->CurrentRuleBoard();
			D_ASSERT(rule_board, L"rule_board should not be null");

			auto ope_chess_id = this->ChessId();
			auto grid_from = this->GridFrom();
			auto grid_to = this->GridTo();

			auto chess_info_from = game_board->ChessAt(grid_from);
			if (!chess_info_from)
			{
				D_ASSERT(false, chess_actoin_error::ParamErrorOnFromGrid);
				break;
			}

			if (chess_info_from->IsEmpty())
			{
				D_ASSERT(false, chess_actoin_error::NotExistingChessOnFromGrid);
				break;
			}

			auto chess_info_to = game_board->ChessAt(grid_to);
			if (!chess_info_to)
			{
				D_ASSERT(false, chess_actoin_error::ParamErrorOnToGrid);
				break;
			}

			const auto square_from = stk_cvter::GridToSquare(grid_from);
			const auto square_to = stk_cvter::GridToSquare(grid_to);

			if (false == rule_board->IsMoveLegal(square_from, square_to))
			{
				D_ASSERT(false, chess_actoin_error::IllegalMoveWithRule);
				break;
			}

			stk::MoveDetail move_detial;
			if (chess_info_from->GetId() != ope_chess_id
				&& !rule_board->CanMoveToPromote(square_from, square_to, stk_cvter::ChessIdToPiece(ope_chess_id), &move_detial))
			{
				D_ASSERT(false, chess_actoin_error::ChessIdMismatchingOnFromGridAndNotPromotionNeither);
				break;
			}

			if (!move_detial.IsValid())
			{
				move_detial = rule_board->GetBestMove(square_from, square_to);
			}

			rule_board->DoMove(move_detial.move_);
		} while (false);
	}

	game_board->RedoChessesViaActionResult(*result_ptr_);
}

GAME_NAMESPACE_END