// Author: Anne Yang
// Date: 2025-04-10
// Description: This program simulates a resource contention scenario
// +-------------------------------+
// | Resource Contention Simulator |
// +-------------------------------+
// | 1. Initialization             |
// | 2. Simulation Loop (R rounds) |
// | 3. Thread-based Contention    |
// | 4. Result Logging & Analysis  |
// +-------------------------------+

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <unordered_set>

// Round = 5 (W) * 100 (N) / 20 (M) = 25 (R)
int N, M, R, W;

struct Participant {
    int id;
    int winCount;
    double speed;  // 0.0 ~ 1.0，數值越小速度越快
};

std::vector<Participant> participants;
std::vector<std::unique_ptr<std::mutex> > chairLocks;
std::mutex winCountMutex;

void compete(Participant& p, std::vector<std::unique_ptr<std::mutex> >& chairLocks) {
    // 模擬準備時間（速度越快越早搶）
    int delay = static_cast<int>((1.0 - p.speed) * 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    // 嘗試搶任一張椅子（非阻塞式）
    std::vector<int> idx(chairLocks.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(idx.begin(), idx.end(), gen);

    for (int i : idx) {
        if (chairLocks[i]->try_lock()) {
            {
                winCountMutex.lock();
                p.winCount++;
                // std::cout << "Participant " << p.id
                //         << " wins chair " << i
                //         << " (total wins: " << p.winCount << ")"
                //         << std::endl;
                winCountMutex.unlock();
            }
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " N M R W\n";
        return 1;
    }
    N = std::stoi(argv[1]);
    M = std::stoi(argv[2]);
    R = std::stoi(argv[3]);
    W = std::stoi(argv[4]);

    // 檢查是否有可能達成任務
    if (R * M < N * W) {
        std::cerr << "Impossible to complete: R * M < N * W\n";
        return 1;
    }

    participants.resize(N);
    chairLocks.reserve(M);
    for (int i = 0; i < M; ++i) {
        chairLocks.push_back(std::make_unique<std::mutex>());
    }

    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < N; ++i) {
        participants[i].id = i;
        participants[i].winCount = 0;
        participants[i].speed =
            static_cast<double>(rand()) / RAND_MAX;  // 0.0 ~ 1.0
    }

    // 依照參賽者速度快慢列印參賽者資訊
    std::sort(participants.begin(), participants.end(),
              [](const Participant& a, const Participant& b) {
                  return a.speed > b.speed;
              });
    for (const auto& p : participants) {
        std::cout << "ID: " << p.id
                  << ", Speed: " << p.speed
                  << std::endl;
    }

    std::cout << "--------------------------------" << std::endl;
    // 模擬回合
    std::cout << "Starting simulation..." << std::endl;

    for (int round = 0; round < R; ++round) {
        // 重置椅子狀態（unlock）
        for (auto& lock : chairLocks) {
                lock->unlock();  // 強制解鎖（保守處理）
        }

        // 混合策略：根據勝場加權選取 M 位參賽者（勝越多，機率越低）
        std::random_device rd;
        std::mt19937 gen(rd());
        std::vector<int> indices;
        std::vector<double> weights;
        int k = 2;  // 調整權重的參數，越大越不公平
        for (int i = 0; i < N; ++i) {
            indices.push_back(i);
            // weights.push_back(1.0 / (1 + participants[i].winCount));  // 勝場越多，機率越小
            weights.push_back(1.0 / std::pow(1 + participants[i].winCount, k));
        }

        std::discrete_distribution<> dist(weights.begin(), weights.end());

        std::unordered_set<int> selected;
        while (selected.size() < M) {
            int chosen = dist(gen);
            selected.insert(chosen);
        }

        std::vector<std::thread> threads;
        for (int idx : selected) {
            threads.emplace_back(compete, std::ref(participants[idx]), std::ref(chairLocks));
        }

        for (auto& t : threads) t.join();

        std::cout << "\rProgress: [" << std::string(round * 50 / R, '#') << std::string(50 - round * 50 / R, ' ')
        << "] " << (round + 1) << "/" << R << std::flush;
    }

    for (int i = 0; i < N; ++i) {
        std::cout << "Participant " << participants[i].id
                  << " wins: " << participants[i].winCount << std::endl;
    }
    
    // 檢查是否有參賽者達標
    bool allReached = true;
    for (const auto& p : participants) {
        if (p.winCount < W) {
            allReached = false;
            break;
        }
    }
    if (allReached) {
        std::cout << "All participants reached the target!" << std::endl;
    }

    // 勝率分布分析（可額外輸出到 CSV 檔）
    std::ofstream outFile("results.csv");
    outFile << "Participant ID, Wins\n";
    for (const auto& p : participants) {
        outFile << p.id << ", " << p.winCount << "\n";
    }
    outFile.close();

    // 分析公平性：勝場標準差與最大-最小差距
    double sum = 0.0, sumSq = 0.0;
    int minWin = participants[0].winCount;
    int maxWin = participants[0].winCount;

    for (const auto& p : participants) {
        sum += p.winCount;
        sumSq += p.winCount * p.winCount;
        minWin = std::min(minWin, p.winCount);
        maxWin = std::max(maxWin, p.winCount);
    }

    double mean = sum / N;
    double variance = sumSq / N - mean * mean;
    double stddev = std::sqrt(variance);
    std::cout << "Win Count Standard Deviation: " << stddev << std::endl;
    std::cout << "Mean Win Count: " << mean << std::endl;
    std::cout << "Max Win Count: " << maxWin << std::endl;
    std::cout << "Min Win Count: " << minWin << std::endl;
    std::cout << "Max-Min Win Gap: " << (maxWin - minWin) << std::endl;

    return 0;
}