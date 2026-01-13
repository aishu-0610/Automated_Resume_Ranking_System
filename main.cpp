#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <algorithm>
#include <dirent.h>

using namespace std;

struct Resume {
    string name;
    string filepath;
    double totalScore = 0;
    double keywordScore = 0;
    double skillScore = 0;
    double experienceScore = 0;
    vector<string> matchedKeywords;
    vector<string> matchedSkills;
};

unordered_map<string, int> loadWeightedList(const string &filename) {
    unordered_map<string, int> weights;
    ifstream file(filename);
    string word;
    int weight;

    while (file >> word >> weight) {
        weights[word] = weight;
    }
    return weights;
}

string readFileContent(const string &filepath) {
    ifstream file(filepath);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int extractExperience(const string &text) {
    regex expPattern("(\\d+)\\s*\\+?\\s*years?");
    smatch match;
    int maxYears = 0;
    string temp = text;

    while (regex_search(temp, match, expPattern)) {
        int years = stoi(match[1]);
        maxYears = max(maxYears, years);
        temp = match.suffix();
    }
    return maxYears;
}

void scoreResume(Resume &r,
                 const unordered_map<string, int> &keywords,
                 const unordered_map<string, int> &skills) {

    string content = readFileContent(r.filepath);
    transform(content.begin(), content.end(), content.begin(), ::tolower);

    // Keyword scoring
    for (auto it = keywords.begin(); it != keywords.end(); ++it) {
        if (content.find(it->first) != string::npos) {
            r.keywordScore += it->second;
            r.matchedKeywords.push_back(it->first);
        }
    }

    // Skill scoring
    for (auto it = skills.begin(); it != skills.end(); ++it) {
        if (content.find(it->first) != string::npos) {
            r.skillScore += it->second;
            r.matchedSkills.push_back(it->first);
        }
    }

    // Experience scoring
    r.experienceScore = extractExperience(content) * 2;

    r.totalScore = r.keywordScore + r.skillScore + r.experienceScore;
}

vector<Resume> getAllResumes(const string &folder) {
    vector<Resume> resumes;
    DIR *dir = opendir(folder.c_str());
    if (!dir) return resumes;

    dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;

        if (filename.size() > 4 &&
            filename.substr(filename.size() - 4) == ".txt") {

            Resume r;
            r.name = filename.substr(0, filename.size() - 4);
            r.filepath = folder + "/" + filename;
            resumes.push_back(r);
        }
    }

    closedir(dir);
    return resumes;
}

vector<Resume> getTop3Resumes(const vector<Resume> &resumes) {
    vector<Resume> top;
    int limit = min(3, (int)resumes.size());

    for (int i = 0; i < limit; ++i) {
        top.push_back(resumes[i]);
    }
    return top;
}

void saveResultsToCSV(const vector<Resume> &resumes, const string &filename) {
    ofstream file(filename);
    file << "Resume,TotalScore,KeywordScore,SkillScore,ExperienceScore\n";

    for (size_t i = 0; i < resumes.size(); ++i) {
        const Resume &r = resumes[i];
        file << r.name << ","
             << r.totalScore << ","
             << r.keywordScore << ","
             << r.skillScore << ","
             << r.experienceScore << "\n";
    }
}

int main() {
    cout << "Resume Ranking System\n";

    auto keywords = loadWeightedList("keywords.txt");
    auto skills = loadWeightedList("skills.txt");
    auto resumes = getAllResumes("resumes");

    if (resumes.empty()) {
        cerr << "No resumes found.\n";
        return 1;
    }

    // Score all resumes
    for (size_t i = 0; i < resumes.size(); ++i) {
        scoreResume(resumes[i], keywords, skills);
    }

    // Sort by total score
    sort(resumes.begin(), resumes.end(),
         [](const Resume &a, const Resume &b) {
             return a.totalScore > b.totalScore;
         });

    // Get top 3
    vector<Resume> top3 = getTop3Resumes(resumes);

    cout << "\nTop 3 Resumes:\n";
    for (int i = 0; i < top3.size(); ++i) {
        cout << i + 1 << ". "
             << top3[i].name
             << " (Score: " << top3[i].totalScore << ")\n";
    }

    saveResultsToCSV(resumes, "resume_scores.csv");
    cout << "\nResults saved to resume_scores.csv\n";

    return 0;
}
