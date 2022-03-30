#include "item.hpp"
#include <fstream>

template <class T>
void log(const T& s) {
#ifdef DEBUG
	std::cerr << s;
#else
	(void)s;
#endif
}

#define HIT_CAP 400
#define EXP_CAP 400
template <class T>
void logn(const T& s) {
	log(s);
	log('\n');
}

bool read_db(vector<item>& equip)
{
	fstream db("database");
	if (db.fail())
		return false;

	string tmp;
	for (size_t n = 0; n < 6; n++)
		db >> tmp;

	logn("  type   | mastery | crit | haste | hit  | expertise |");
	while(!db.eof())
	{
		item tmp;
		db >> tmp.type;
		db >> tmp.stats[MAST];
		db >> tmp.stats[CRIT];
		db >> tmp.stats[HAST];
		db >> tmp.stats[HIT];
		db >> tmp.stats[EXP];
		equip.push_back(tmp);
		logn(tmp);
	}
	return true;
}

void save_db(const vector<item>& equip)
{
	ofstream save("database");
	save << "item	mastery crit haste hit expertise\n";
	size_t n = 0;
	for (; n < equip.size() - 1; n++)
	{
		save << equip[n].type << " ";
		save << equip[n].stats[MAST] << " ";
		save << equip[n].stats[CRIT] << " ";
		save << equip[n].stats[HAST] << " ";
		save << equip[n].stats[HIT] << " ";
		save << equip[n].stats[EXP] << "\n";
	}
	save << equip[n].type << " ";
	save << equip[n].stats[MAST] << " ";
	save << equip[n].stats[CRIT] << " ";
	save << equip[n].stats[HAST] << " ";
	save << equip[n].stats[HIT] << " ";
	save << equip[n].stats[EXP];
}

vector<float> get_stats(const item& a)
{
	vector<float> rval;
	if (a.stats[MAST] > 0)
		rval.push_back(MAST);
	if (a.stats[CRIT] > 0)
		rval.push_back(CRIT);
	if (a.stats[HAST] > 0)
		rval.push_back(HAST);
	if (a.stats[HIT] > 0)
		rval.push_back(HIT);
	if (a.stats[EXP] > 0)
		rval.push_back(EXP);
	return rval;
}

vector<float> get_reforgeable(const item& a)
{
	vector<float> rval;
	if (a.stats[MAST] == 0)
		rval.push_back(MAST);
	if (a.stats[CRIT] == 0)
		rval.push_back(CRIT);
	if (a.stats[HAST] == 0)
		rval.push_back(HAST);
	if (a.stats[HIT] == 0)
		rval.push_back(HIT);
	if (a.stats[EXP] == 0)
		rval.push_back(EXP);
	return rval;
}

void show_comp(vector<item>& current)
{
	cout << "==================================" << endl;
	cout << "item   mastery crit haste hit expertise" << endl;
	for (size_t n = 0; n < current.size(); n++)
	{
		cout << setw(7) << left << current[n].type << " ";
		cout << setw(5) << left << current[n].stats[MAST] <<  " ";
		cout << setw(5) << left << current[n].stats[CRIT] <<  " ";
		cout << setw(5) << left << current[n].stats[HAST] <<  " ";
		cout << setw(5) << left << current[n].stats[HIT] <<  " ";
		cout << setw(5) << left << current[n].stats[EXP] << endl;
	}
	cout << "=================================" << endl;
}

string get_enum(int index){
	switch (index)
	{
	case MAST:
		return "mastery";
	case CRIT:
		return "crit";
	case HAST:
		return "haste";
	case HIT:
		return "hit";
	case EXP:
		return "expertise";
	default:
		return "error";
	}
}

void show_vector(vector< vector<item> > equip_list)
{
	for (size_t n = 0; n < equip_list.size(); n++)
		show_comp(equip_list[n]);
}

void find_optimal_R(int index, const vector<item>& equip, vector< vector<item> >& equip_list, vector<item>& current_equip)
{
	if (index == 10)
	{
		float stats[5] = {0};
		size_t curequip_size = current_equip.size();
		for (size_t n = 0; n < curequip_size; n++)
		{
			for(size_t j = 0; j < 5; j++)
				stats[j] += current_equip[n].stats[j];
		}
		if (stats[HIT] >= HIT_CAP && stats[EXP] >= EXP_CAP)
			equip_list.push_back(current_equip);
		return;
	}
	const item& obj = equip[index];
	vector<float> stats_in	= get_stats(obj);
	vector<float> rfrgbl	= get_reforgeable(obj);
	size_t stats_size		= stats_in.size();
	size_t rfrgbl_size		= rfrgbl.size();

	item cpy(obj);
	find_optimal_R(index + 1, equip, equip_list, current_equip);
	for (size_t j = 0; j < stats_size; j++)
	{
		for (size_t k = 0; k < rfrgbl_size; k++)
		{
			cpy = obj;
			cpy.stats[rfrgbl[k]] = (cpy.stats[stats_in[j]] * 0.4);
			cpy.stats[stats_in[j]] *= 0.6;
			current_equip[index] = cpy;
			find_optimal_R(index + 1, equip, equip_list, current_equip);
		}
	}
}

vector< vector<item> > find_optimal2(const vector<item>& equip)
{
	vector< vector<item> > equip_list;
	vector<item> current_equip(equip);
	find_optimal_R(0, equip, equip_list, current_equip);
	return equip_list;
}

int main()
{
	vector<item> equip;
	if (!read_db(equip))
	{
		cout << "no database file, ensure a file named 'database' is present and uses correct formating" << endl;
		return 1;
	}
	vector< vector<item> > all_capped = find_optimal2(equip);
	show_vector(all_capped);
	save_db(equip);
	return 0;
}