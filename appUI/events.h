#include <array>
#include <string>
#define ARRAYLEN 512

int GetApiStatsLen();
std::string GetApiName(int index);
int GetApiCount(int index);
void layer_event(const char* input);