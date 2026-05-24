#pragma once
#include <string>
#include <future>

std::future<std::string> GetAllFacultiesAsync();
std::future<std::string> GEtFacultyByIdAsync(int id);
std::future<std::string> PostFacultyAsync(const std::string& name);
std::future<std::string> PatchFacultyAsync(int id, const std::string& newName);
std::future<std::string> DeleteFacultyAsync(int id);
