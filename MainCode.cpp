#include <iostream>
#include "IntervalTree.h"

using namespace std;


// Validation function for the input hours
bool validTime(float start_time, float end_time){
	if (start_time < 0 || end_time >= 24 || start_time>end_time){
		return false;
	}
	return true;
}

int main()
{

	bool isRush;
	IntervalTree mytree;

	float start_time;
	float end_time;
	float search_time;
	char task;
	char n = NULL;
	while (n != '|')
	{
		// Just some STDIN and STDOUT for the main function to add or search rush hours
		cout << "Enter A if you want to add rush hours or S if you want to search for hours\n";
		cin >> task;
		if (task == 'A'){
			cout << "Enter two floats:\n";
			cout << "Start Time: ";
			cin >> start_time;
			cout << "End Time: ";
			cin >> end_time;

			bool val = validTime(start_time, end_time);
			if (val){
				mytree.AddTimeSpan(start_time, end_time);
				cout << start_time << '\t' << end_time << " Rush hour is added";
			}
			else{
				cout << "Time span not valid!";
			}

			cout << endl;
		}
		else if (task == 'S'){
			cout << "Enter one float:\n";
			cout << "Search Time: ";
			cin >> search_time;

			bool val = validTime(search_time, search_time);
			if (val){
				isRush = mytree.IsRushHour(search_time);
				if (isRush){
					cout << search_time << " found!";
				}
				else{
					cout << search_time << " not found!";
				}
			}
			else{
				cout << "Time span not valid!";
			}

			cout << endl;
		}
		cout << "Enter | if you want to quit: ";
		cin >> n;
	}
}



