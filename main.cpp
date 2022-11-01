#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "util.h"
#include "projectile.h"


int main()
{
	system("cls");
	system("mode con cols=92 lines=40");

	PR a;
	Boundary b;
	b.generate();

	double dt(0.05);
	VECTOR r0 = { 1., 1. }, v;

	int test(0);
	TILE_STATE s(EMPTY);

	do {
		system("cls");
		b.print();

		v = Input_Initial_velocity();

		a.SetData(r0, v, dt);

		const VECTOR force = b.getForce();

		int end(0);
		do {
			a.Update(force);

			s = b.check_hit(a);
			switch (s) {
			case TARGET:
				gotoxy(3, 33);
				printf("The target is destroyed!!!");
				end = 1;
				b.generate();
				break;

				// 경계에 닿으면 종료
			case BORDER_CORNER:
				end = 1;
				break;

				// 경계에 닿으면 종료
			case BORDER_V:
				end = 1;
				break;

				// 경계에 닿으면 종료
			case BORDER_H:
				end = 1;
				break;
			}

			if (s == EMPTY) a.draw();

		} while (!end);

		gotoxy(3, 34);
		printf("Quit? ('q' to quit)");
		test = _getch();

	} while (test != 'q');
}
