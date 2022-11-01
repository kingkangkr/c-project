#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS

struct INT_POS
{
	int x, y;
};

struct VECTOR
{
	double v[2];
};

class PR
{
public:
	PR() {}
	~PR() {}

	void SetData(const VECTOR& r0, const VECTOR& v0, const double& dt);
	void Update(const VECTOR& force);
	void ReflectLeftRight();
	void ReflectUpDown();
	void ReflectCorner();

	void draw() const;

	const VECTOR& GetPosition() const { return m_r; }
	const INT_POS& GetIntPosition() const { return m_r_int; }

protected:
	VECTOR m_r, m_v;
	double m_h1, m_h2;
	INT_POS m_r_int, m_r_int_prev;
};

#define Nx	(70)
#define Ny	(25)

enum TILE_STATE { EMPTY = 0, BORDER_H, BORDER_V, BORDER_CORNER, TARGET, EARTH, MOON };

class Boundary
{
public:
	Boundary() {}
	~Boundary() {}

	TILE_STATE check_hit(const PR& r) const;
	void generate();
	void print() const;
	INT_POS getMoon();
	VECTOR getForce();

protected:
	TILE_STATE m_tile[Nx][Ny];
};

VECTOR Input_Initial_velocity();
