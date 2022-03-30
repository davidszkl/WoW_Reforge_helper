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

void find_optimal(const vector<item>& equip)
{
	size_t v_size =	equip.size();
	vector<item>	optimal(equip);
	vector<item>	current(equip);
	for (size_t n = 0; n < v_size; n++)
	{
		vector<float> stats_in	= get_stats(equip[n]);
		vector<float> rfrgbl	= get_reforgeable(equip[n]);
		size_t stats_size		= stats_in.size();
		size_t rfrgbl_size		= rfrgbl.size();

		for (size_t j = 0; j < stats_size; j++)
		{
			for (size_t k = 0; k < rfrgbl_size; k++)
			{
				current = equip;
				log("Reforging ");
				log(current[n].stats[stats_in[j]]);
				logn(" " + get_enum(stats_in[j]) + " into");
				log(current[n].stats[stats_in[j]] * 0.6);
				log(" " + get_enum(stats_in[j]) + " ");
				log(current[n].stats[stats_in[j]] * 0.4);
				logn(" " + get_enum(rfrgbl[k]));

				current[n].stats[rfrgbl[k]] = (current[n].stats[stats_in[j]] * 0.4);
				current[n].stats[stats_in[j]] *= 0.6;

				logn("Reforged into");
				log(current[n].stats[stats_in[j]]);
				log(" " + get_enum(stats_in[j]) + " ");
				log(current[n].stats[rfrgbl[k]]);
				logn(" " + get_enum(rfrgbl[k]));
				show_comp(current);
			}
		}
	}
}

void permute(string a, int l, int r)
{
    if (l == r)
        cout<<a<<endl;
    else
    {
        for (int i = l; i <= r; i++)
        {
            swap(a[l], a[i]);
            permute(a, l+1, r);
            swap(a[l], a[i]);
        }
    }
}

int main()
{
	vector<item> equip;
	if (!read_db(equip))
	{
		cout << "no database file, ensure a file named 'database' is present and uses correct formating" << endl;
		return 1;
	}
	find_optimal(equip);
	save_db(equip);
	return 0;
}