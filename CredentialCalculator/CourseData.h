#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
class CourseData
{
public:

	struct Course
	{
		float grade;
		int points;
		std::string name;
		std::string gradeName;
		int id;
	};

	
private:

	float CalculateCredential();
	std::vector<Course*> courses;
	std::string GradeToName(int i);

public:

	CourseData();
	~CourseData();

	bool DeleteCourse(int i);
	bool NewCourse(int _grade, int _points, std::string _name);
	int count = 0;
	Course* GetCourse(int i = -1);
	float cred;
	char* GetAsSaveData();
	bool FromSaveData(std::string dataString);
	int GetAmount();
};

