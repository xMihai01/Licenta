#pragma once

enum class QtKeyEnum {
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90
};

constexpr const char* QtEnumToString(QtKeyEnum key)
{
	switch (key)
	{
	case QtKeyEnum::A: return "A";
	case QtKeyEnum::B: return "B";
	case QtKeyEnum::C: return "C";
	case QtKeyEnum::D: return "D";
	case QtKeyEnum::E: return "E";
	case QtKeyEnum::F: return "F";
	case QtKeyEnum::G: return "G";
	case QtKeyEnum::H: return "H";
	case QtKeyEnum::I: return "I";
	case QtKeyEnum::J: return "J";
	case QtKeyEnum::K: return "K";
	case QtKeyEnum::L: return "L";
	case QtKeyEnum::M: return "M";
	case QtKeyEnum::N: return "N";
	case QtKeyEnum::O: return "O";
	case QtKeyEnum::P: return "P";
	case QtKeyEnum::Q: return "Q";
	case QtKeyEnum::R: return "R";
	case QtKeyEnum::S: return "S";
	case QtKeyEnum::T: return "T";
	case QtKeyEnum::U: return "U";
	case QtKeyEnum::V: return "V";
	case QtKeyEnum::W: return "W";
	case QtKeyEnum::X: return "X";
	case QtKeyEnum::Y: return "Y";
	case QtKeyEnum::Z: return "Z";
	}
	return "INVALID";
}