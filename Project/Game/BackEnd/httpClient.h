#pragma once
#include <string>
#include <future>

const static std::string serverURL = "http://localhost:3000/scores";

std::future<std::string> GetAllFacultiesAsync();
std::future<std::string> GetFacultyByIdAsync(int id);
std::future<std::string> PostFacultyAsync(int score);
std::future<std::string> PatchFacultyAsync(int id, int score);
std::future<std::string> DeleteFacultyAsync(int id);

std::future<std::string> GetWorkoutRankingAsync();
bool PostWorkout(int count);