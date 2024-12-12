#pragma once
#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

namespace Engine {

class ExtMove : public Move {
	
	int value;

	ExtMove& operator=(Move m) { data = m.raw(); }

	operator float() const = delete;

}

}

#endif
