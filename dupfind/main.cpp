#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>

#define SEQ_SEARCH_DEPTH 3
#define SEQ_REPORT_THRESHOLD 4
#define SIMLARITY_THRESHOLD 0.7

struct LineData {
    std::string str;
    std::string tab;
    size_t group_ind;
};

struct GroupData {
    std::vector<size_t> members;
    const char *color;
};

std::vector<LineData> lines;
std::string file_name;
std::unordered_map<int, GroupData> groups;

struct {
	bool show = false;
} cfg;

// trim from end (in place)
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

int strDistance(std::string &a, std::string &b)
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

void assignLineToGroup(int n, float threshold)
{
    bool ok = false;
    for (int i = 0; i < n; i++) {
        float similarity = 1.f - (float)strDistance(lines[n].str, lines[i].str) / (float)std::max(lines[n].str.size(), lines[i].str.size());

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

void printLines(int first, int last)
{
	for (int i = first; i <= last; i++) {
		std::cout << lines[i].tab << ' ' << lines[i].str << std::endl;
	}
}

void detectSeq(int len, int first, int last)
{
	if (len >= SEQ_REPORT_THRESHOLD) {
		std::cout << file_name << ": [" << first+1 << ", " << last+1 << "] duplicates [" <<
			lines[first].group_ind+1 << ", " << lines[last].group_ind+1 << "]\n";

		if (cfg.show) {
			std::cout << "\n...\n";
			printLines(lines[first].group_ind, lines[last].group_ind);
			std::cout << "...\n";
			printLines(first, last);
			std::cout << "...\n";
		}
	}
}

void findDupSeqs()
{
	size_t seq_len = 1;
	size_t seq_first = 0;
	size_t seq_last = 0;
	for (size_t i = 1; i < lines.size(); i++) {
		if (i - seq_last > SEQ_SEARCH_DEPTH) {
			detectSeq(seq_len, seq_first, seq_last);
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

	detectSeq(seq_len, seq_first, seq_last);
}

bool shouldAssign(int n)
{
	if (lines[n].str == "}" || lines[n].str == "{")
		return false;
	if (lines[n].str.size() > 0 && lines[n].str[0] == '#')
		return false;
	return true;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "Usage: code-dup FILENAMES...\n";
        exit(1);
    }
    
    for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--show") == 0) {
			cfg.show = true;
			continue;
		}
    
        lines.clear();
        groups.clear();

		file_name = argv[i];
        std::ifstream file(argv[i]);
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
			if (shouldAssign(j))
            	assignLineToGroup(j, SIMLARITY_THRESHOLD);
        }

        findDupSeqs();
    }
}
