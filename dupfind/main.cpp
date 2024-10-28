#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string.h>

struct {
	bool show = false;
	int maxskip = 2;
	float similarity_threshold = 0.7f;
	int report_len = 4;
} cfg;

struct linedata_t {
    std::string str;
    std::string tab;
    size_t group_ind;
};

struct groupdata_t {
    std::vector<size_t> members;
    const char *color;
};

std::vector<linedata_t> lines;
std::string file_name;
std::unordered_map<int, groupdata_t> groups;

inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

int str_distance(std::string &a, std::string &b)
{
    if (a.size() == 0)
        return b.size();

    if (b.size() == 0)
        return a.size();

    std::vector<std::vector<int>> dp(a.size(), std::vector<int>(b.size()));
    dp[0][0] = 0;
    for (size_t i = 1; i < a.size(); i++)
        dp[i][0] = i;
    for (size_t j = 1; j < b.size(); j++)
        dp[0][j] = j;

    for (size_t i = 1; i < a.size(); i++) {
        for (size_t j = 1; j < b.size(); j++) {
            dp[i][j] = std::min(std::min(dp[i][j - 1] + 1, dp[i - 1][j] + 1), dp[i - 1][j - 1] + (a[i] != b[j]));
        }
    }

    return dp.back().back();
}

void assign_line_to_group(int n, float threshold)
{
    bool ok = false;
    for (int i = 0; i < n; i++) {
        float similarity = 1.f - (float)str_distance(lines[n].str, lines[i].str) / (float)std::max(lines[n].str.size(), lines[i].str.size());

        if (similarity >= threshold) {
            groups[i].members.push_back(n);
            lines[n].group_ind = i;
            ok = true;
			break;
        }
    }

    if (!ok) {
        groups[n].members.push_back(n);
    }
}

void print_lines(int first, int last)
{
	for (int i = first; i <= last; i++) {
		std::cout << lines[i].tab << ' ' << lines[i].str << std::endl;
	}
}

void report_if_needed(int len, int first, int last)
{
	if (len >= cfg.report_len) {
		std::cout << file_name << ": [" << first+1 << ", " << last+1 << "] duplicates [" <<
			lines[first].group_ind+1 << ", " << lines[last].group_ind+1 << "]\n";

		if (cfg.show) {
			std::cout << "...\n";
			print_lines(lines[first].group_ind, lines[last].group_ind);
			std::cout << "...\n";
			print_lines(first, last);
			std::cout << "...\n\n";
		}
	}
}

void find_sequences()
{
	size_t seq_len = 1;
	size_t seq_first = 0;
	size_t seq_last = 0;
	
	for (size_t i = 1; i < lines.size(); i++) {
		if ((int)(i - seq_last) > cfg.maxskip + 1) {
			report_if_needed(seq_len, seq_first, seq_last);
			seq_len = 1;
			seq_last = i;
			seq_first = i;
		}

		int diff = lines[i].group_ind - lines[seq_last].group_ind;
		if (diff == 1) {
			seq_last = i;
			seq_len++;
		}
	}

	report_if_needed(seq_len, seq_first, seq_last);
}

bool is_line_regular(int n)
{
	if (lines[n].str.size() == 0)
		return false;

	if (lines[n].str == "}" || lines[n].str == "{")
		return false;
		
	if (lines[n].str.size() > 0 && lines[n].str[0] == '#')
		return false;

	if (lines[n].str.size() >= 2 && lines[n].str[0] == '/' && lines[n].str[1] == '/')
		return false;

	if (lines[n].str.size() >= 2 && lines[n].str[0] == '/' && lines[n].str[1] == '*')
		return false;

	if (lines[n].str.size() >= 6 && lines[n].str.substr(0, 6) == "break;")
		return false;

	return true;
}

void print_help()
{
	std::cout << "Usage: dupfind [options] <file1> <file2>...\n"
		     	 "options:\n"
				 "  --show           show code that was duplicated\n"
				 "  --maxskip X      maximum allowed number of lines to skip\n"
				 "  --similarity X   similarity threshold (in percent)\n"
				 "  --reportlen X    minimal length of sequence to report it\n";
}

char **parse_arg(char **argv)
{
	if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0) {
		print_help();
		exit(0);
	}

	if (argv[0][0] != '-' || argv[0][1] != '-')
		return argv;

	if (strcmp(argv[0], "--show") == 0) {
		cfg.show = true;
		return argv + 1;
	}

	if (argv[1] == NULL)
		return NULL;
	
	if (strcmp(argv[0], "--maxskip") == 0) {
		cfg.maxskip = atoi(argv[1]);
		return argv + 2;
	}
	if (strcmp(argv[0], "--similarity") == 0) {
		cfg.similarity_threshold = atof(argv[1]) / 100.f;
		return argv + 2;
	}
	if (strcmp(argv[0], "--reportlen") == 0) {
		cfg.report_len = atoi(argv[1]);
		return argv + 2;
	}

	return argv;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        print_help();
		exit(1);
    }

    argv++;
    while (*argv != NULL) {
		char **next = parse_arg(argv);
		if (next == NULL) {
			std::cout << "Input error\n";
			exit(1);
		}
		else if (next > argv) {
			argv = next;
			continue;
		}

        lines.clear();
        groups.clear();

		file_name = *argv;
        std::ifstream file(*argv);
        if (!file.is_open()) {
            std::cout << "Failed to open file\n";
            exit(1);
        }

        std::string line;
        while (std::getline(file, line)) {
        	rtrim(line);

			size_t i = 0;
			while (i < line.size() && isspace(line[i])) i++;
			std::string tab = line.substr(0, i);
			line            = line.substr(i, line.size() - i);
        	
            lines.push_back({ line, tab, 0 });
        }
        file.close();

        for (size_t j = 0; j < lines.size(); j++) {
			if (is_line_regular(j))
            	assign_line_to_group(j, cfg.similarity_threshold);
        }

        find_sequences();
        argv++;
    }
}
