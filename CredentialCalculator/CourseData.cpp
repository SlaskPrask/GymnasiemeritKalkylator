#include "CourseData.h"

CourseData::CourseData()
{
}

bool CourseData::NewCourse(int _grade, int _points, std::string _name)
{
	Course *c = new Course();

	if (_grade == 6 || _grade == 11)
		c->grade = 0;
	else
	{
		float deductor = _grade < 6 ? (_grade - 1) * -2.5f : (_grade - 8) * -5; //(1-5)
		c->grade = 20 + deductor;
	}

	c->points = _points * 50;
	c->name = _name;
	c->id = 300 + count;
	c->gradeName = GradeToName(_grade);

	courses.push_back(c);
	count++;

	cred = CalculateCredential();
	return true;
}

float CourseData::CalculateCredential()
{
	float valueSum = 0;
	float pointSum = 0;

	for (auto it = courses.begin(); it != courses.end();)
	{
		valueSum += (*it)->grade * (*it)->points;
		pointSum += (*it)->points;
		++it;
	}

	return round((valueSum / pointSum) * 100) / 100;
}

CourseData::Course* CourseData::GetCourse(int i)
{
	if (courses.size() > i)
		return i > -1 ? courses.at(i) : courses.back();
	else
		return NULL;
}

CourseData::~CourseData()
{
	for (auto it = courses.begin(); it != courses.end();)
	{
		delete (*it);
		it = courses.erase(it);
		count--;
	}
}

char* CourseData::GetAsSaveData()
{
	std::string str = "";

	for (auto it = courses.begin(); it != courses.end();)
	{
		str += (*it)->name;
		str += ":";
		str += std::to_string((*it)->points);
		str += ":";
		str += (*it)->gradeName;
		str += ":";
		str += std::to_string((*it)->grade);
		str += ":";
		str += std::to_string((*it)->id);
		str += ";";

		++it;
	}

	char* sData = &str[0u];

	return sData;
}

bool CourseData::FromSaveData(std::string dataString)
{
	std::vector<std::string> cVec;

	int start = 0;

	for (int i = 0; i < dataString.size(); i++)
	{
		if (dataString[i] == ';')
		{
			cVec.push_back(dataString.substr(start, i - start));
			i++;
			start = i;		
		}
	}

	for (auto it = courses.begin(); it != courses.end();)
	{
		delete (*it);
		it = courses.erase(it);
		count--;
	}

	for (int i = 0; i < cVec.size(); i++)
	{
		Course *c = new Course();
		int start = 0;
		int index = 0;
		std::string temp = cVec.at(i);

		for (int j = 0; j < temp.size(); j++)
		{
			if (temp[j] == ':')
			{
				switch (index)
				{
					case 0:
						c->name = temp.substr(start, j - start);
						for (int k = 0; k < c->name.size(); k++)
						{
							if (c->name[k] == '\u03A3')
								c->name[k] = 'ä';
							if (c->name[k] == '\u03C3')
								c->name[k] = 'å';
							if (c->name[k] == '\u007F7')
								c->name[k] = 'ö';
						}

						break;
					case 1:
						c->points = std::atoi(temp.substr(start, j - start).c_str());
						break;
					case 2:
						c->gradeName = temp.substr(start, j - start);
						break;
					case 3:
						c->grade = std::atof(temp.substr(start, j - start).c_str());
						break;
					case 4:
						c->id = std::atoi(temp.substr(start, j - start).c_str());
						break;
					default:
						break;
				}

				j++;
				index++;
				start = j;
			}
		}

		courses.push_back(c);
		count = c->id + 1;
	}
	cred = CalculateCredential();

	//then set the list view

}

bool CourseData::DeleteCourse(int i)
{
	if (i == -1)
		return false;

	delete courses.at(i);
	courses.erase(courses.begin() + i);
	cred = CalculateCredential();
	std::cout << i << std::endl;
	return true;
}

std::string CourseData::GradeToName(int i)
{
	switch (i)
	{
		case 1:
			return "A";
		case 2:
			return "B";
		case 3:
			return "C";
		case 4:
			return "D";
		case 5:
			return "E";
		case 6:
			return "F";
		case 8:
			return "MVG";
		case 9:
			return "VG";
		case 10:
			return "G";
		case 11:
			return "IG";
		default:
			return "";
	}
}

int CourseData::GetAmount()
{
	return courses.size();
}
