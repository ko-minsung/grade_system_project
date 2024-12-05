#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

struct Student {
    std::string id;  // 학번
    int score;       // 점수
    char grade;      // 등급
};

// CSV 파일 읽기
std::vector<Student> readCSV(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return students;
    }

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        Student student;
        std::string token;

        if (!std::getline(stream, student.id, ',')) {
            std::cerr << "Error: Invalid data format in file " << filename << std::endl;
            continue;
        }

        if (!std::getline(stream, token, ',')) {
            std::cerr << "Error: Invalid data format in file " << filename << std::endl;
            continue;
        }

        try {
            student.score = std::stoi(token);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid score value in file " << filename << std::endl;
            continue;
        }

        students.push_back(student);
    }

    return students;
}

// 점수 계산 및 등급 분류
void calculateGrades(std::vector<Student>& students, int aPercent, int bPercent, int cPercent) {
    // 높은 점수 순으로 정렬 (내림차순)
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.score > b.score;  // 높은 점수가 앞으로 오도록 정렬
    });

    int totalStudents = students.size();
    int aCount = totalStudents * aPercent / 100;
    int bCount = totalStudents * bPercent / 100;
    int cCount = totalStudents * cPercent / 100;

    for (int i = 0; i < totalStudents; ++i) {
        if (i < aCount) {
            students[i].grade = 'A';
        } else if (i < aCount + bCount) {
            students[i].grade = 'B';
        } else if (i < aCount + bCount + cCount) {
            students[i].grade = 'C';
        } else {
            students[i].grade = 'D';
        }
    }
}

int main() {
    const std::string inputFilename = "/home/kominsung/grade_system_project/students.csv";
    const std::string outputFilename = "/home/kominsung/grade_system_project/results.txt";

    // CSV 파일 읽기
    std::vector<Student> students = readCSV(inputFilename);
    if (students.empty()) {
        return 1;
    }

    // 사용자로부터 등급 비율 입력받기
    int aPercent, bPercent, cPercent;
    std::cout << "Enter the percentage for grade A: ";
    std::cin >> aPercent;
    std::cout << "Enter the percentage for grade B: ";
    std::cin >> bPercent;
    std::cout << "Enter the percentage for grade C: ";
    std::cin >> cPercent;

    // 입력값 검증
    if (aPercent + bPercent + cPercent > 100) {
        std::cerr << "Error: The total percentage exceeds 100%. Please try again." << std::endl;
        return 1;
    }

    // 등급 계산
    calculateGrades(students, aPercent, bPercent, cPercent);

    // 결과를 파일에 저장
    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << outputFilename << std::endl;
        return 1;
    }

    outputFile << "ID\tScore\tGrade\n";
    for (const auto& student : students) {
        outputFile << student.id << "\t" << student.score << "\t" << student.grade << "\n";
    }

    outputFile.close();

    std::cout << "Results have been saved to " << outputFilename << std::endl;
    return 0;
}
