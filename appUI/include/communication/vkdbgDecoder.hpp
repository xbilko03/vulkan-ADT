/*
* Name		    : vkdbgDecoder.hpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : [xbilko03] Header file for decoding logic of binary messages written by the layer
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Jan Pečiva Ph.D.
*/
/* AI-Assisted: Claude Opus 5.5: syntax, entire file */
#pragma once

#include "../../../transport/include/vkdbgFormat.hpp"
#include <vulkan/vulkan.h>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace vkdebug::ui {
    /* [xbilko03] decoded parameter name + value (as text) */
    struct decodedField {
        std::string name;
        std::string value;
    };

    /* [xbilko03] decoded parameters of one call */
    typedef std::vector<decodedField> decodedFields;

    /* [xbilko03] reads a message in same order as it was written by the writer, keeps track of offset and bytes of that message */
    class messageReader {
    public:
        messageReader(const uint8_t *data, size_t size) {
            messageData = data;
            messageSize = size;
        }

        /* [xbilko03] false = message ended sooner than expected (something is broken) */
        bool ok() { return valid; }

        /* [xbilko03] read bytes of type T within processed message, convert the value to readable string for that call's parameters and give it a label */
        template<typename T>
        void value(decodedFields &out, const char *label) {
            T value = {};
            if (read(&value, sizeof(T)))
                add(out, label, std::to_string(value));
        }

        /* [xbilko03] like value(), but also returns the number */
        template<typename T>
        T count(decodedFields &out, const char *label) {
            T value = {};
            if (read(&value, sizeof(T)))
                add(out, label, std::to_string(value));
            return value;
        }

        /* [xbilko03] like value(), but value is a hex formatted string (useful for pointers, handles, etc.) */
        template<typename T>
        void bits(decodedFields &out, const char *label) {
            uint64_t value = 0;
            size_t size = sizeof(T);
            if (size > 8)
                size = 8;
            if (read(&value, size)) {
                std::stringstream s;
                s << (void *) ((uintptr_t) (value));
                add(out, label, s.str());
            }
        }

        /* [xbilko03] attempt to read the pointer's validity flag, 0 means layer flagged it as VK_NULL_HANDLE, process it further depending on this flag */
        bool pointer(decodedFields &out, const char *label) {
            uint8_t byte = 0;
            if (!read(&byte, 1))
                return false;
            if (byte == 0)
                add(out, label, "VK_NULL_HANDLE");
            return byte != 0;
        }

        /* [xbilko03] same as value(), but value is a string */
        void string(decodedFields &out, const char *label) {
            uint16_t length = 0;
            /* [xbilko03] string is of variable length, the length in our format is encoded in first two bytes before string data */
            if (!read(&length, sizeof(length)))
                return;
            if (offset + length > messageSize) {
                valid = false;
                return;
            }
            add(out, label, std::string((const char *) (messageData + offset), length));
            offset += length;
        }

    private:
        /* [xbilko03] append one decoded parameter to decodedFields */
        void add(decodedFields &out, const char *label, const std::string &value) {
            decodedField field;
            field.name = label;
            field.value = value;
            out.push_back(field);
        }

        /* [xbilko03] take next size bytes from the message, and move forward */
        bool read(void *out, size_t size) {
            if (!valid || offset + size > messageSize) {
                valid = false;
                return false;
            }
            memcpy(out, messageData + offset, size);
            offset += size;
            return true;
        }

        const uint8_t *messageData;
        size_t messageSize;
        size_t offset = 0;
        bool valid = true;
    };

    /* [xbilko03] convert function number to Vulkan function name */
    const char *functionName(uint16_t function);

    /* [xbilko03] convert output parameters of a finished call into text */
    bool decodeCallEnd(uint16_t function, const uint8_t *payload, size_t size, decodedFields &out);

    /* [xbilko03] convert output parameters of a finished call into text (for functions within layerStatic.cpp) */
    bool decodeCallEndStatic(uint16_t function, const uint8_t *payload, size_t size, decodedFields &out);

    /* [xbilko03] convert input parameters of a call sent before that vulkan function's execution within vulkan driver */
    bool decodeCallInputs(uint16_t function, const uint8_t *payload, size_t size, decodedFields &out);

    /* [xbilko03] convert input parameters of a call sent before that vulkan function's execution within vulkan driver (for functions within layerStatic.cpp) */
    bool decodeCallInputsStatic(uint16_t function, const uint8_t *payload, size_t size, decodedFields &out);
}
