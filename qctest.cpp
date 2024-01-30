// ***************************************************************
// "Quality Control (QC) Testing"
// Written by Kevin Kwan @ Georgia Institute of Technology
// The Importance of Self-Study: To learn C++ and practice C++ programming
//
// UCLA CS31 Winter 2024 Project 3
// UCLA CS31 Winter 2024 Programming Assignment 3
// This program validates and analyzes string QC test results.
// It checks if a QC result string is valid, calculates the total number of tests, passes, defects, and batches, and handles errors appropriately.
//
// This program is written in C++ and compiled using g++.
// Comments have been added for understanding and viewing pleasure.
//
// I am not responsible for any mistakes or errors in this program.
// Use at your own risk.
//
// ***************************************************************

using namespace std;
#include <iostream>
#include <cassert>
#include <string>
#include <regex>

// This function checks if a QC result string is valid/well-formed based on the following conditions:
// - a batch of results must begin with the character Q (case sensitive)
// - a batch of results must report both pass and defect test results with p and d in either order
// - no leading zeros are allowed in any numeric value being reported
// - the total number of QC tests in a batch must equal the number of pass and defect test results.
// - the total number of QC tests in a batch must be greater than zero (0).
// - a single result string may include multiple batches of results
bool isValidQC(string results)
{
    // You can get a good understand of regex matching from a proper university class or online.
    regex pattern("Q([1-9][0-9]*)(p(0|[1-9][0-9]*)d(0|[1-9][0-9]*)|d(0|[1-9][0-9]*)p(0|[1-9][0-9]*))");
    smatch match;
    int totalTests = 0, passes = 0, defects = 0;
    // Understanding match:
    // match[0] is the entire match
    // match[1] is the number of tests (nums following Q)
    // match[2] is the first letter (p or d)
    // match[3] is the number of passes (nums following p)
    // match[4] is the number of defects (nums following d)
    // match[5] is the number of defects (nums following d)
    // match[6] is the number of passes (nums following p)
    while (regex_search(results, match, pattern))
    {
        totalTests += stoi(match[1]);
        if (match[2].str()[0] == 'p')
        {
            if (match[3].matched)
                passes += stoi(match[3]);
            if (match[4].matched)
                defects += stoi(match[4]);
        }
        else
        {
            if (match[5].matched)
                defects += stoi(match[5]);
            if (match[6].matched)
                passes += stoi(match[6]);
        }
        // Update results to be the suffix of the previous match so we can continue finding matches
        results = match.suffix().str();
    }
    return totalTests == passes + defects && totalTests > 0 && results.empty();
}

// This function returns the total number of pass test results from all the batches reported in the string.
// If the string is not valid, it returns -1.
int passQC(string results)
{
    if (!isValidQC(results))
        return -1;
    regex pattern("p(0|[1-9][0-9]*)");
    smatch match;
    int passes = 0;
    while (regex_search(results, match, pattern))
    {
        passes += stoi(match[1]);
        results = match.suffix().str();
    }
    return passes;
}

// This function returns the total number of defect test results from all the batches reported in the string.
// If the string is not valid, it returns -1.
int defectQC(string results)
{
    if (!isValidQC(results))
        return -1;
    regex pattern("d(0|[1-9][0-9]*)");
    smatch match;
    int defects = 0;
    while (regex_search(results, match, pattern))
    {
        defects += stoi(match[1]);
        results = match.suffix().str();
    }
    return defects;
}

// This function returns the total number of tests from all the batches reported in the string.
// If the string is not valid, it returns -1.
int totalQC(string results)
{
    if (!isValidQC(results))
        return -1;
    regex pattern("Q([1-9][0-9]*)");
    smatch match;
    int totalTests = 0;
    while (regex_search(results, match, pattern))
    {
        totalTests += stoi(match[1]);
        results = match.suffix().str();
    }
    return totalTests;
}

// This function returns the total number of batches reported in the string.
// If the string is not valid, it returns -1.
int batches(string results)
{
    if (!isValidQC(results))
        return -1;
    regex pattern("Q");
    smatch match;
    int batches = 0;
    while (regex_search(results, match, pattern))
    {
        batches++;
        results = match.suffix().str();
    }
    return batches;
}

// Main function for testing our functions.
int main()
{
    // Test cases taken from the .pdf
    assert(isValidQC("") == false);
    assert(isValidQC("  ") == false);
    assert(passQC("  ") == -1);
    assert(defectQC("   ") == -1);
    assert(totalQC("   ") == -1);
    assert(batches("   ") == -1);
    assert(isValidQC("Q2p1d1") == true);
    assert(passQC("Q2p1d1") == 1);
    assert(defectQC("Q2p1d1") == 1);
    assert(totalQC("Q2p1d1") == 2);
    assert(batches("Q2+1-1") == -1);

    // Here's some more test cases because why not
    assert(isValidQC("Q1p0d1Q1d0p1") == true);
    assert(isValidQC("Q5d2p3") == true);
    assert(isValidQC("Q5p3d2Q2p1d1") == true);
    assert(isValidQC("q1p0d1") == false);
    assert(isValidQC("Q1pd1") == false);
    assert(isValidQC("Q1p1d") == false);
    assert(isValidQC("Q1p0d1 asdfR") == false);
    assert(isValidQC("Q5p00003d0002") == false);
    assert(isValidQC("Q5p0d0") == false);
    assert(isValidQC("Q0p0d0") == false);
    assert(passQC("Q5p3d2Q2p1d1") == 4);
    assert(passQC("q1p0d1") == -1);
    assert(defectQC("Q5p3d2Q2p1d1") == 3);
    assert(defectQC("q1p0d1") == -1);
    assert(totalQC("Q5p3d2Q2p1d1") == 7);
    assert(totalQC("q1p0d1") == -1);
    assert(batches("Q2p1d1") == 1);
    assert(batches("Q5p3d2Q2p1d1") == 2);
    assert(batches("q1p0d1") == -1);

    cout << "All test cases succeeded." << endl;

    return 0;
}