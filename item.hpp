#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

using namespace std;

enum STATS {
	MAST,
	CRIT,
	HAST,
	HIT,
	EXP
};

class item
{
	public:
	item(): stats(5, 0){}
	string			type;
	vector<float>	stats;
};

ostream& operator<<(ostream& os, const item& a)
{
	os << setw(8) << left << a.type << " | ";
	os << setw(7) << left << a.stats[MAST] << " | ";
	os << setw(4) << left << a.stats[CRIT] << " | ";
	os << setw(5) << left << a.stats[HAST] << " | ";
	os << setw(4) << left << a.stats[HIT]<< " | ";
	os << setw(9) << left << a.stats[EXP]<< " | ";
    return os;
}