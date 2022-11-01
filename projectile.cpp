#include "projectile.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include "util.h"
#include <math.h>

VECTOR Input_Initial_velocity()
{
	double angle, magnitude;

	gotoxy(3, 30);
	printf("각도를 입력하시오: ");
	scanf("%lf", &angle);

	gotoxy(3, 31);
	printf("속도를 입력하시오: ");
	scanf("%lf", &magnitude);

	VECTOR ans = { magnitude * cos(RAD(angle)), magnitude * sin(RAD(angle)) };

	return ans;
}

//======================================================

void PR::SetData(const VECTOR& r0, const VECTOR& v0, const double& dt)
{
	m_r = r0;
	m_v = v0;
	m_h1 = dt;
	m_h2 = dt * dt;

	m_r_int.x = int(m_r.v[0]);
	m_r_int.y = int(m_r.v[1]);

	m_r_int_prev.x = m_r_int_prev.y = 0;
}

void PR::Update(const VECTOR& force)
{
	m_r_int_prev = m_r_int;

	int i;
	for (i = 0; i < 2; i++) {
		m_r.v[i] += m_h1 * m_v.v[i] + m_h2 * force.v[i];
		m_v.v[i] += m_h1 * force.v[i];
	}

	m_r_int.x = int(m_r.v[0]);
	m_r_int.y = int(m_r.v[1]);
}

void PR::ReflectLeftRight()
{
	m_v.v[0] = -m_v.v[0];
}

void PR::ReflectUpDown()
{
	m_v.v[1] = -m_v.v[1];
}

void PR::ReflectCorner()
{
	int i;
	for (i = 0; i < 2; i++) m_v.v[i] = -m_v.v[i];
}

void PR::draw() const
{
	bool changed = ((abs(m_r_int.x - m_r_int_prev.x) + abs(m_r_int.y - m_r_int_prev.y)) > 0) ? true : false;
	if (!changed) return;

	gotoxy(m_r_int_prev.x, Ny - 1 - m_r_int_prev.y);
	//	printf(" "); //이전 위치를 지웁니다.
	gotoxy(m_r_int.x, Ny - 1 - m_r_int.y);
	printf("*"); //새로운 위치에 문자를 출력해줍니다.

	Sleep(5);
}

//======================================================

TILE_STATE Boundary::check_hit(const PR& r) const
{
	INT_POS p = r.GetIntPosition();

	return m_tile[p.x][p.y];
}

void Boundary::generate()
{
	int i, j;
	for (i = 1; i < Nx - 1; i++) for (j = 1; j < Ny - 1; j++) m_tile[i][j] = EMPTY;

	for (i = 1; i < Nx - 1; i++) m_tile[i][0] = m_tile[i][Ny - 1] = BORDER_V;
	for (i = 1; i < Ny - 1; i++) m_tile[0][i] = m_tile[Nx - 1][i] = BORDER_H; \

		// 지구, 달 좌표 지정
		for (i = 1; i < 10; i++) for (j = 1; j < 10; j++) if (i + j < 15) m_tile[i][j] = EARTH;

	m_tile[61][21] = MOON;
	m_tile[60][21] = MOON;
	m_tile[59][21] = MOON;

	m_tile[62][20] = MOON;
	m_tile[61][20] = MOON;
	m_tile[60][20] = MOON;
	m_tile[59][20] = MOON;
	m_tile[58][20] = MOON;

	m_tile[62][19] = MOON;
	m_tile[61][19] = MOON;
	m_tile[60][19] = MOON;
	m_tile[59][19] = MOON;
	m_tile[58][19] = MOON;

	m_tile[61][18] = MOON;
	m_tile[60][18] = MOON;
	m_tile[59][18] = MOON;

	m_tile[0][0] = m_tile[Nx - 1][0] = m_tile[Nx - 1][0] = m_tile[Nx - 1][Ny - 1] = BORDER_CORNER;

	srand(time(NULL));//랜덤한 숫자

	i = (rand() % 30) + 30;
	j = (rand() % 20) + 2;

	// 타겟 좌표 지정
	m_tile[i][j] = TARGET;
}

void Boundary::print() const
{
	int i, j;
	for (i = 0; i < Nx; i++) for (j = 0; j < Ny; j++) {
		gotoxy(i, Ny - 1 - j);
		switch (m_tile[i][j]) {
		case EMPTY:
			printf(" ");
			break;

		case BORDER_H:
		case BORDER_V:
		case BORDER_CORNER:
			printf("#");
			break;

			// 달 그리기
		case MOON:
			printf("&");
			break;

		case TARGET:
			printf("$");
			break;

			// 지구 그리기
		case EARTH:
			printf("&");
			break;
		}
	}
}

// 달의 위치를 얻음
INT_POS Boundary::getMoon() {
	INT_POS pos;

	int i, j;
	for (i = 0; i < Nx; i++) for (j = 0; j < Ny; j++) {
		if (m_tile[i][j] == MOON) {
			pos.x = i;
			pos.y = j;

			return pos;
		}
	}
}

// 달과 지구 사이의 중력을 계산
VECTOR Boundary::getForce() {
	VECTOR force;
	double G = -9.8, M = 30000;

	INT_POS moon = getMoon();
	double dist = sqrt(pow((double)moon.x - 1, 2) + pow((double)moon.y - 1, 2));

	force = { G * M / pow(dist,3), G * M / pow(dist,3) };

	return force;
}
