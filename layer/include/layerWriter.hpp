/*
* Name		    : layerWriter.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Header file of the layer writing into shared memory
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#pragma once

#include "../../transport/include/vkdbgFormat.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace vkdebug::layer {
    /* [xbilko03] create shared memory of this process */
    bool layer_WriterInit();

    /* [xbilko03] read agent's port */
    uint16_t layer_WriterGetAgentPort();

    /* [xbilko03] check if agent is alive */
    bool layer_WriterReaderAlive();

    /* [xbilko03] starts VkDebuggerAgent executable from the UI folder */
    bool layer_StartAgent(const std::string &uiPath);

    /* [xbilko03] set the number of current vulkan call of current thread */
    void layer_SetCurrentCall(uint64_t call);

    /* [xbilko03] start creating a MessageCallBegin message */
    void layer_CreateCallBegin(uint16_t function);

    /* [xbilko03] create a message of given type */
    void layer_CreateLayerInfoU64(uint8_t infoType, uint64_t value);

    /* [xbilko03] create a message of given type */
    void layer_CreateLayerInfoHandle(uint8_t infoType, uint64_t handle);

    /* [xbilko03] create a message of given type */
    void layer_CreateLayerInfoDouble(uint8_t infoType, double value);

    /* [xbilko03] create a message of given type */
    void layer_CreateLayerInfoBool(uint8_t infoType, bool value);

    /* [xbilko03] create a message of given type */
    void layer_CreateLayerInfoString(uint8_t infoType, const std::string &value);

    /* [xbilko03] create a message of given type, splits into parts when too large */
    void layer_CreateMemoryData(uint64_t handle, const void *bytes, uint64_t size);

    /* [xbilko03] object to write messages */
    class messageWriter {
    public:
        /* [xbilko03] starts creating message */
        void begin(uint8_t messageType, uint16_t function);

        /* [xbilko03] ends creating message */
        void end();

        void bytes(const void *data, size_t size);

        template<typename T>
        void value(T value) {
            bytes(&value, sizeof(T));
        }

        /* [xbilko03] bits of vulkan's parameters of type handle|pointer */
        template<typename T>
        void bits(T value) {
            size_t size = sizeof(T);
            if (size > 8)
                size = 8;
            bytes(&value, size);
        }

        bool pointer(bool notNull) {
            uint8_t byte = 0;
            if (notNull)
                byte = 1;
            bytes(&byte, 1);
            return notNull;
        }

        void string(const char *text) {
            size_t length = 0;
            if (text)
                length = strlen(text);
            if (length > 0xFFFF)
                length = 0xFFFF;
            uint16_t length16 = (uint16_t) (length);
            value(length16);
            bytes(text, length16);
        }

    private:
        bool active = false;
    };
}
