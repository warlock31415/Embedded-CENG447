#ifndef LM298_H
#define LM298_H
	#include <avr/io.h>
	#include <util/delay.h>
	#include "pin_map.h"

	class L298{
	public:
		void init(char clk);
		void turn_right(int speed);
		void forward(int speed);
		void back(int speed);
		void turn_left(int speed);

		void square_turn(int speed);
		void circ();

	private:
		char map(int d_cyc);
	};


#endif 