/*
ID: david.y3
LANG: C++
TASK: contact
*/

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

void setNthBit(long long &bits, bool isOne, int n)
{
    long long bit = 1 << n;

    if (bits & bit)
    {
        if (isOne)
            return;
        else
            bits ^= bit;
    }
    else
    {
        if (isOne)
            bits |= bit;
        else
            return;
    }
}

int main()
{
    std::ifstream fin("contact.in");

    int a, b, n;
    std::string emission = "";

    fin >> a >> b >> n >> emission;

    long long bitWindow = 0;
    int leadingZeros = 0;

    for (int i = 0; i < a; i++)
    {
        setNthBit(bitWindow, emission[i] - '0', i);
        if (emission[i] - '0' == 0)
        {
            leadingZeros++;
        }
        else
            leadingZeros = 0;
    }

    // Pattern, leadingZeros, digits, frequency
    std::map<std::pair<std::pair<long long, int>, int>, int> patternCount;

    for (int start = 0; start < emission.size(); start++)
    {
        int prevLeadingZeros = leadingZeros;
        
        for (int length = a; length <= b; length++)
        {
            if (start + length - 1 > emission.size())
            {
                break;
            }

            int digit = emission[start + length - 1] - '0';

            setNthBit(bitWindow, digit, length - 1);

            if (digit == 0)
            {
                leadingZeros++;
            }
            else
            {
                leadingZeros = 0;
            }

            patternCount[{{bitWindow, leadingZeros}, length}]++;
            
            std::cout << bitWindow << '\n';
            std::cout << start << ' ' << start + length - 1 << '\n';
            std::cout << "ZEROS: " << leadingZeros << '\n';
        }

        // Look at bit that will enter window
        // Determine how it will change the number of leading zeros
        if (start + a < emission.size() && emission[start + a] == 0)
        {
            prevLeadingZeros++;
        }
        else if (start + a < emission.size() && emission[start + a] == 1)
        {
            prevLeadingZeros = 0;
        }

        leadingZeros = prevLeadingZeros;
        
        // Leave only last a bits
        long long mask = (1 << (a)) - 1;

        bitWindow &= mask;
        bitWindow >>= 1;
    }

    // Frequency, {{pattern, leadingZeros}, length}
    // Sorted in decreasing order
    std::map<int, std::vector<std::pair<std::pair<long long, int>, int>>, std::greater<int>> getPatternsByFreq;

    // move frequencies and patterns into map with frequencies as key
    for (auto pattern : patternCount)
    {
        getPatternsByFreq[pattern.second].push_back(pattern.first);
    }

    // Sort by length and binary
    auto cmp = [](std::pair<std::pair<long long, int>, int> a, std::pair<std::pair<long long, int>, int> b) {
        if (a.second == b.second)
        {
            return (a.first.first << (a.first.second - 1)) < (b.first.first << (b.first.second - 1));
        }
        if (a.second < b.second) return true;
        
        return false;
    };

    for (auto &pByF : getPatternsByFreq)
    {
        std::sort(pByF.second.begin(), pByF.second.end(), cmp);
    }

    std::ofstream fout("contact.out");

    int freqPrinted = 0;

    for (auto pats : getPatternsByFreq)
    {
        if (freqPrinted == n)
            break;

        // Print frequency
        fout << pats.first << '\n';

        int patsPrinted = 0;

        for (std::pair<std::pair<long long, int>, int> p : pats.second)
        {
            // Print binary of p.first.first
            long long bitChecker = 1;
            
            while (bitChecker <= p.first.first)
            {
                if (bitChecker & p.first.first)
                {
                    fout << '1';
                }
                else
                    fout << '0';

                bitChecker <<= 1;
            }

            // Print leadingZeros (now they are trailing zeros)
            for (int i = 0; i < p.first.second; i++)
            {
                fout << '0';
            }
            
            patsPrinted++;

            // Dumb required formatting
            if (patsPrinted % 6 == 0 || patsPrinted == pats.second.size())
                fout << '\n';
            else
                fout << ' ';
        }

        freqPrinted++;
    }
}