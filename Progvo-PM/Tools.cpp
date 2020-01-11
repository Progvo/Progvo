// (c) 2019-2020 Pttn (https://Progvo.dev)

#include "Tools.hpp"

std::vector<std::byte> load(const Ecii::String &path) {
	std::ifstream file(path.str());
	std::vector<std::byte> data;
	if (file) {
		std::cout << "Malfermanta " << path.str() << "..." << std::endl;
		file.seekg(0, std::ios::end);
		std::streampos length(file.tellg());
		file.seekg(0, std::ios::beg);
		data = std::vector<std::byte>(length);
		file.read((char*) data.data(), length);
		std::cout << "Shargado de " << data.size() << " b sukcesis!" << std::endl;
	}
	else
		std::cerr << "Ne eblas malfermi la dosieron " << path.str() << " :| ..." << std::endl;
	return data;
}

void save(const std::vector<std::byte> &v8, const Ecii::String &path) {
	std::fstream file(path.str(), std::ios::out | std::ios::binary);
	if (file) {
		std::cout << "Konservanta de " << v8.size() << " b al " << path << "...";
		file.write((char*) v8.data(), v8.size());
		file.close();
		std::cout << " Sukcesis!" << std::endl;
	}
	else
		std::cerr << "Ne eblas malfermi la dosieron " << path.str() << " :| ..." << std::endl;
}

double timeSince(const std::chrono::time_point<std::chrono::system_clock> &t0) {
	const std::chrono::time_point<std::chrono::system_clock> t(std::chrono::system_clock::now());
	const std::chrono::duration<double> dt(t - t0);
	return dt.count();
}