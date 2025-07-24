#include "position.h"

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <random>


namespace Engine {

constexpr std::string_view PieceToChar(" PNBRQK  pnbrqk");

namespace {
	const CastlingRights right_update[64] = {
		~WHITE_OOO, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ~WHITE_CASTLING, ANY_CASTLING, ANY_CASTLING, ~WHITE_OO,
		ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING,
		ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING,
		ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING,
		ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING,
		ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING,
		ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING,
		~BLACK_OOO, ANY_CASTLING, ANY_CASTLING, ANY_CASTLING, ~BLACK_CASTLING, ANY_CASTLING, ANY_CASTLING, ~BLACK_OO
	};

	uint64_t piece_zobrist[SQUARE_NB][PIECE_NB];
	uint64_t ep_zobrist[SQUARE_NB + 1]; 
	uint64_t castling_zobrist[CASTLE_RIGHT_NB];
	uint64_t side_zobrist;
} // namespace 
	
std::ostream& operator<<(std::ostream& os, const Position& pos) {
	os << "+---+---+---+---+---+---+---+---+\n";

	for (Rank r = RANK_8; r >= RANK_1; --r) {
		
		for (File f = FILE_A; f <= FILE_H; ++f) 
			os << "| " << PieceToChar[pos.piece_on(make_square(f, r))] << " ";

		os << "| " << std::to_string(r + 1) << "\n+---+---+---+---+---+---+---+---+\n";
	}

	os << "  a   b   c   d   e   f   g   h\n\n";

	os << "FEN String: " << pos.fen();
	os << "Hash Key: " << pos.hash() << "\n";

	return os << ((pos.to_play() == WHITE) ? "White" : "Black") << " to move\n";
}

// this static function is called at the start of main()
// it's purpose is to initialize the zobrist keys that
// will be used to hash the position for the transposition
// table
void Position::init() {
	std::mt19937_64 rng(0xae7f5489032548fe);
	
	for (Square sq = SQ_A1; sq <= SQ_H8; ++sq) {
		for (Piece pc : {W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
						 B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING}) {
		
			piece_zobrist[sq][pc] = rng();
		}
		ep_zobrist[sq] = rng();
	}

	ep_zobrist[SQ_NONE] = rng();

	for (size_t i = 0; i < CASTLE_RIGHT_NB; ++i)
		castling_zobrist[i] = rng();

	side_zobrist = rng();
}

void Position::set(std::string_view fen) {
	st = history;
	
	const std::unordered_map<char, Piece> piece_conversion = {
		{'p', B_PAWN},
		{'P', W_PAWN},
		{'n', B_KNIGHT},
		{'N', W_KNIGHT},
		{'b', B_BISHOP},
		{'B', W_BISHOP},
		{'r', B_ROOK},
		{'R', W_ROOK},
		{'q', B_QUEEN},
		{'Q', W_QUEEN},
		{'k', B_KING},
		{'K', W_KING}
	};
	
	int boardIdx = 56;
	size_t fenIdx = 0;
	while ( boardIdx >= 0 ) {
		char temp;
		switch (temp = fen[fenIdx++]) {
			case 'p':
				[[fallthrough]];
			case 'P':
				[[fallthrough]];
			case 'n':
				[[fallthrough]];
			case 'N':
				[[fallthrough]];
			case 'b':
				[[fallthrough]];
			case 'B':
				[[fallthrough]];
			case 'r':
				[[fallthrough]];
			case 'R':
				[[fallthrough]];
			case 'q':
				[[fallthrough]];
			case 'Q':
				[[fallthrough]];
			case 'k':
				[[fallthrough]];
			case 'K':
				board[boardIdx++] = piece_conversion.at(temp);
				break;
			case ' ':
			case '/':
				boardIdx -= 16;
				break;
			default:
				for (int i = 0; i < temp - '0'; ++i) {
					board[boardIdx++] = NO_PIECE;}
				break;
		}
	}
		
	key = 0;	
	for (Square sq = SQ_A1; sq <= SQ_H8; ++sq) {
		Piece pc = piece_on(sq);
		if (pc != NO_PIECE)
			key ^= piece_zobrist[sq][pc];
	}

	sideToMove = fen[fenIdx++] == 'w' ? WHITE : BLACK;	
	if (sideToMove == BLACK)
		key ^= side_zobrist;
	++fenIdx;

	char t;
	st->cr = CastlingRights(0);
	while ((t = fen[fenIdx++]) != ' ') {
		switch ( t ) {
			case 'K':
				st->cr |= WHITE_OO;
				break;
			case 'k':
				st->cr |= BLACK_OO;
				break;
			case 'Q':
				st->cr |= WHITE_OOO;
				break;
			case 'q':
				st->cr |= BLACK_OOO;
				break;
			default:
				break;
		}
	}

	key ^= castling_zobrist[st->cr];
	
	if ((t = fen[fenIdx++]) != '-') {
		char file = t;
		char rank = fen[fenIdx++];
		st->ep_sq = Square(((rank - '1') * 8) + (file - 'a'));
	} else { st->ep_sq = SQ_NONE; }
	++fenIdx;
	key ^= ep_zobrist[st->ep_sq];

	size_t start = fenIdx;
	while ((t = fen[fenIdx++]) != ' ') {}
	fiftyMoveCount = std::stoi(std::string(fen.substr(start, fenIdx)));

	std::string fm = std::string(fen.substr(fenIdx));
	int fullMove = std::stoi(fm);
	gamePly = (fullMove - 1) * 2 + (sideToMove == BLACK ? 1 : 0);

	byColor[WHITE] = byColor[BLACK] = byType[PAWN] = byType[KNIGHT] = byType[BISHOP] =
	byType[ROOK] = byType[QUEEN] = byType[KING] = byType[ALL_PIECES] = 0;

	for (Square sq = SQ_A1; sq <= SQ_H8; ++sq) {
		Piece curr = board[sq];
		switch (curr) {
			case W_PAWN:
				byColor[WHITE] |= square_bb(sq);
				byType[PAWN] |= square_bb(sq);
				break;
			case W_KNIGHT:
				byColor[WHITE] |= square_bb(sq);
				byType[KNIGHT] |= square_bb(sq);
				break;
			case W_BISHOP:
				byColor[WHITE] |= square_bb(sq);
				byType[BISHOP] |= square_bb(sq);
				break;
			case W_ROOK:
				byColor[WHITE] |= square_bb(sq);
				byType[ROOK] |= square_bb(sq);
				break;
			case W_QUEEN:
				byColor[WHITE] |= square_bb(sq);
				byType[QUEEN] |= square_bb(sq);
				break;
			case W_KING:
				byColor[WHITE] |= square_bb(sq);
				byType[KING] |= square_bb(sq);
				break;
			case B_PAWN:
				byColor[BLACK] |= square_bb(sq);
				byType[PAWN] |= square_bb(sq);
				break;
			case B_KNIGHT:
				byColor[BLACK] |= square_bb(sq);
				byType[KNIGHT] |= square_bb(sq);
				break;
			case B_BISHOP:
				byColor[BLACK] |= square_bb(sq);
				byType[BISHOP] |= square_bb(sq);
				break;
			case B_ROOK:
				byColor[BLACK] |= square_bb(sq);
				byType[ROOK] |= square_bb(sq);
				break;
			case B_QUEEN:
				byColor[BLACK] |= square_bb(sq);
				byType[QUEEN] |= square_bb(sq);
				break;
			case B_KING:
				byColor[BLACK] |= square_bb(sq);
				byType[KING] |= square_bb(sq);
				break;
			default:
				break;
		}
	}
	byType[ALL_PIECES] = byColor[WHITE] | byColor[BLACK];
	pos_is_ok();

	assert( pieces(WHITE, KING) && pieces(BLACK, KING));
	return;
}

std::string Position::fen() const {
	std::string s;

	for (Rank r = RANK_8; r >= RANK_1; --r) {
		int spaces = 0;
		for (File f = FILE_A; f <= FILE_H; ++f) {
			Piece p = piece_on(make_square(f, r));

			if (p == NO_PIECE) {
				++spaces;
			} else {
				if ( spaces ) {
					s      += std::to_string(spaces);
					spaces = 0;
				}
				s += PieceToChar[p];
			}
		}

		if (spaces)
			s += std::to_string(spaces);
		
		if (r != RANK_1)
			s += "/";
	}

	s += (sideToMove == WHITE) ? " w" : " b";
	s += " ";
	if (!st->cr) {
		s += "-";
	} else {
		if (st->cr & WHITE_OO)
			s += "K";
		if (st->cr & WHITE_OOO)
			s += "Q";
		if (st->cr & BLACK_OO)
			s += "k";
		if (st->cr & BLACK_OOO)
			s += "q";
	}
	s += " " + std::string(printSquare[st->ep_sq]);
	s += " " + std::to_string(fiftyMoveCount);
	s += " " + std::to_string((gamePly / 2) + 1);

	return s += "\n";
}

std::string Position::dress_move(Move m) const {
	return std::string(printSquare[m.from_sq()]) + std::string(printSquare[m.to_sq()]);
}

char Position::piece_str(Square s) const {
	return PieceToChar[piece_on(s)];
}

Bitboard Position::attacked_squares(Color us) const {
	Bitboard attacked(0);

	Bitboard pawns = pieces(us, PAWN);
	while (pawns) {
		attacked |= attacks_bb<PAWN>(pop_lsb(pawns), us);}
	
	Bitboard occ = pieces() & ~pieces(~us, KING);
	for (PieceType pt : { KNIGHT, BISHOP, ROOK, QUEEN, KING} ) {
		Bitboard stuffs = pieces(us, pt);
		while (stuffs) {
			attacked |= attacks_bb(pop_lsb(stuffs), occ, pt);}}

	return attacked;
}

bool Position::can_castle(CastlingRights cr) const {
	if (!(cr & st->cr)) 
		return false;

	constexpr Bitboard whiteOObb = 0x60;
	constexpr Bitboard whiteOOObb = 0xe;
	constexpr Bitboard blackOObb = whiteOObb << 56;
	constexpr Bitboard blackOOObb = whiteOOObb << 56;

	// invert occupancy because we need to make sure that the squares are empty
	Bitboard occ = ~pieces();
	Bitboard safe_squares = ~attacked_squares(~sideToMove);
	Bitboard heheha = Bitboard(0xc) << 56;

	switch (cr) {
		case WHITE_OO:
			return ((whiteOObb & occ) == whiteOObb) && ((whiteOObb & safe_squares) == whiteOObb);
		case WHITE_OOO:
			return ((whiteOOObb & occ) == whiteOOObb) && ((0xc & safe_squares) == 0xc);
		case BLACK_OO:
			return ((blackOObb & occ) == blackOObb) && ((blackOObb & safe_squares) == blackOObb);
		case BLACK_OOO:
			return ((blackOOObb & occ) == blackOOObb) && ((heheha & safe_squares) == heheha);
			break;
		default:
			assert(false);
			return false;
	}
}

Bitboard Position::pinned(Color us, PieceType pinnedTo) const {
	Bitboard pinned(0);
	Color them          = ~us;
	Bitboard pinnedToBB = pieces(us, pinnedTo);
	
	while (pinnedToBB) {
		// I call this king because it is the important square here, 
		// not necessarily because it is referring to the king, although
		// it can refer to the king
		Square king = pop_lsb(pinnedToBB);
		for (PieceType pt : { BISHOP, ROOK, QUEEN }) {
			Bitboard pinners = pieces(them, pt);
			while (pinners) {
				Square pinner      = pop_lsb(pinners);
				Bitboard line      = line_bb(pinner, king);

				if (line &= PseudoAttacks[pt][pinner]) {
					if (!more_than_one(line &= pieces()) && line) {
						pinned |= pop_lsb(line);}}
			}
		}
	}
	return pinned;
}

// returns all pieces currently checking a king.
// should ideally only be able to include pieces
// of the opposite color to who is moving, since
// that is how the rules of the game shape out
// however, until I test and confirm, we will
// act as though that is not the case
Bitboard Position::checkers() const {
	Bitboard checkers(0);
	Bitboard occ = pieces();

	for (Color color : { WHITE, BLACK }) {
		Square ksq = king_on(color);
		
		assert(is_ok(ksq));
		Color them = ~color;

		for (PieceType pt : { BISHOP, ROOK, QUEEN }) {
			Bitboard attacks = attacks_bb(ksq, occ, pt);
			// TODO there's a bug here involving two queens checking the king at the same time
			if (attacks &= pieces(them, pt)) {
				checkers |= attacks;}
		}

		Bitboard knights = pieces(them, KNIGHT);
		if (knights &= attacks_bb<KNIGHT>(ksq)) {
			checkers |= knights;}

		Bitboard pawns = pieces(them, PAWN);
		if (pawns &= attacks_bb<PAWN>(ksq, color)) {
			checkers |= pawns;}
	}
	
	return checkers;
}

void Position::put_piece(Piece pc, Square s) {
	assert(empty(s));

	board[s] = pc;

	byColor[color_of(pc)] ^= s;
	byType[type_of(pc)] ^= s;
	byType[ALL_PIECES] ^= s;

	key ^= piece_zobrist[s][pc];

	return;
}

void Position::move_piece(Square to, Square from) {
	assert(empty(to));

	Piece pc = board[from];
	Bitboard bb = square_bb(to, from);
	
	byColor[color_of(pc)] ^= bb;
	byType[type_of(pc)] ^= bb;
	byType[ALL_PIECES] ^= bb;

	board[from] = NO_PIECE;
	board[to] = pc;

	key ^= piece_zobrist[from][pc];
	key ^= piece_zobrist[to][pc];

	return;
}

Piece Position::remove_piece(Square s) {
	assert(!empty(s));

	Piece pc = board[s];
	board[s] = NO_PIECE;

	byColor[color_of(pc)] ^= s;
	byType[type_of(pc)] ^= s;
	byType[ALL_PIECES] ^= s;

	key ^= piece_zobrist[s][pc];
	
	return pc;
}

void Position::do_move(Move *m) {
	assert(m->is_ok());

	Square to = m->to_sq();
	Square from = m->from_sq();
	
	if (capture(m) || m->type() == ENPASSANT)
		st->capturedPiece = m->type() == ENPASSANT ? make_piece(PAWN, ~sideToMove) : remove_piece(to);
	else 
		st->capturedPiece = NO_PIECE;

	key ^= ep_zobrist[st->ep_sq];

	++st;
	st->cr = (st - 1)->cr;

	key ^= castling_zobrist[st->cr];
	st->cr &= right_update[to];
	st->cr &= right_update[from];
	key ^= castling_zobrist[st->cr];

	if (type_of(piece_on(from)) == PAWN && distance<Square>(to, from) == 2) 
		st->ep_sq = to - push_dir(sideToMove);
	else
		st->ep_sq = SQ_NONE;

	key ^= ep_zobrist[st->ep_sq];

	if (m->type() == PROMOTION) {
		remove_piece(from);
		put_piece(make_piece(m->promote_to(), sideToMove), to);}
	else 
		move_piece(to, from);

	if (m->type() == CASTLING) {
		if (to == SQ_C1) 
			move_piece(SQ_D1, SQ_A1);	
		else if (to == SQ_G1)
			move_piece(SQ_F1, SQ_H1);
		else if (to == SQ_C8)
			move_piece(SQ_D8, SQ_A8);
		else
			move_piece(SQ_F8, SQ_H8);
	} else if (m->type() == ENPASSANT) {
		Square capsq = to - push_dir(sideToMove);
		assert(type_of(piece_on(capsq)) == PAWN);
		remove_piece(capsq);
	}

	++gamePly;
	sideToMove = ~sideToMove;
	key ^= side_zobrist;
}

void Position::undo_move(Move *m) {
	key ^= ep_zobrist[st->ep_sq];
	key ^= castling_zobrist[st->cr];

	--st;
	--gamePly;
	sideToMove = ~sideToMove;

	key ^= ep_zobrist[st->ep_sq];
	key ^= castling_zobrist[st->cr];
	key ^= side_zobrist;

	// backwards bc we're undoing move 
	Square from = m->to_sq();
	Square to = m->from_sq();

	if (m->type() == PROMOTION) {
		remove_piece(from);
		put_piece(make_piece(PAWN, sideToMove), to);}
	else
		move_piece(to, from);
	
	Piece cap;
	if (m->type() == ENPASSANT) {
		put_piece(make_piece(PAWN, ~sideToMove), from - push_dir(sideToMove));} 
	else if ((cap = st->capturedPiece) != NO_PIECE) 
		put_piece(cap, from);

	if (m->type() == CASTLING) {
		if (from == SQ_G1) 
			move_piece(SQ_H1, SQ_F1);
		else if ( from == SQ_C1 ) 
			move_piece(SQ_A1, SQ_D1);
		else if ( from == SQ_G8 ) 
			move_piece(SQ_H8, SQ_F8);
		else
			move_piece(SQ_A8, SQ_D8);
	}
}

bool Position::legal_move(Move* m) const {
	Color us = sideToMove;
	Square from = m->from_sq();
	Square to = m->to_sq();
	Square ksq = king_on(us);

	if (from == ksq) 
		return !(to & attacked_squares(~us));

	// start with en pass
	if (m->type() == ENPASSANT) {
		Square capsq = to - push_dir(us);
		Bitboard occ = (pieces() ^ from ^ capsq) | to;

		return (!(attacks_bb<ROOK>(ksq, occ) & pieces(~us, ROOK, QUEEN)) && 
				!(attacks_bb<BISHOP>(ksq, occ) & pieces(~us, BISHOP, QUEEN)));
	}

	Bitboard pin = pinned(us);
	assert(pin & from);

	return !(pin & from) || (line_bb(to, ksq) & from) || (line_bb(from, ksq) & to); 
}

void Position::pos_is_ok() const {
	for (Square sq = SQ_A1; sq <= SQ_H8; ++sq) {
		Piece p = piece_on(sq);
		if (p == NO_PIECE)
			continue;

		PieceType pt = type_of(p);
		Color c = color_of(p);
		Bitboard sqbb = square_bb(sq);

		if (!(sqbb & pieces(c)))
			assert(false && "color bitboard");

		if (!(sqbb & pieces(pt)))
			assert(false && "piece bitboard");

	}
}
	
} // namespace Engine
