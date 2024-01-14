/*
* Name		: events.h
* Project	: A Debugging Tool for Vulkan API
* Director  : Ing. Ján Pečiva Ph.D.
* Author	: Jozef Bilko (xbilko03)
*/
#include <array>
#include <string>
#include <list>
#define ARRAYLEN 512

/* returns the number of how many vk api calls we are currently tracking */
int GetApiStatsLen();

/* returns the name of the vk api call */
std::string GetApiName(int index);

/* returns the value associated with a given ID of an vk api call */
int GetApiCount(int index);

/*
* whenever something important happened at the layer, call this function
* it also handles the information provided by the layer 'input'
*/
void newLayerEvent(const char* input);

/* returns the string value of a given vk api call according to its ID, this is definitely a subject to change */
std::string GetApiEventName(int ID);

/* returns the list containing the traced vk api calls */
std::list <int> GetApiCallHistory();

/* returns the nth value of the command buffer structure */
int GetCmdBuffVal(int ID);