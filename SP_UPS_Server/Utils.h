//
// Created by David Kuta on 27.11.2021.
//

#ifndef RPS_SERVER_UTILS_H
#define RPS_SERVER_UTILS_H


#include <cstdio>
#include <iostream>
#include <vector>
#include <regex>

using namespace std;
/**
 * Used for some utils
 */
class Utils {
public:
    /**
     * Splits string by its delimiter
     * @param str string
     * @param delim delimiter
     * @return vector of parts
     */
    static vector<string> split(const string str, const string delim);

    /**
     * cuts whitespaces from the string
     * @param text string
     * @return string without whitespaces
     */
    static string trim(string text);

    /**
     * cuts whitespaces from the string on the left side
     * @param text string
     * @return string without whitespaces
     */
    static string ltrim(string s);

    /**
     * cuts whitespaces from the right side
     * @param text string
     * @return string without whitespaces
     */
    static string rtrim(string s);

    static bool isAlpha(string s);
};


#endif //RPS_SERVER_UTILS_H
