#include <array>
#include <string>
#include <list>
#define ARRAYLEN 512

int GetApiStatsLen();
std::string GetApiName(int index);
int GetApiCount(int index);
void layer_event(const char* input);
std::string GetApiEventName(int ID);
std::list <int> GetApiCallHistory();
int GetCmdBuffVal(int ID);