#include "include/StreamVideoBuffer.h"


StreamVideoBuffer::StreamVideoBuffer() {
    sps = (char *)malloc(64 * sizeof(char));
    pps = (char *)malloc(64 * sizeof(char));
    preIFrame = (char *)malloc(16000 * sizeof(char));
    iFrame = (char *)malloc(256000 * sizeof(char));
    pFrame = (char **)malloc(64 * sizeof(char *));
    sizepFrame = (int *)malloc(64 * sizeof(int));
    for (int i = 0; i < 64; i++) {
        pFrame[i] = (char *)malloc(48000 * sizeof(char));
        sizepFrame[i] = 0;
        assert(pFrame[i]);
    }
    assert (sps && pps && iFrame && pFrame);
    numPFrames = 0;
    hasWriter = 0;
    sizeSPS = 0;
    sizePPS = 0;
    sizeiFrame = 0;
    sizepreIFrame = 0;
}

bool StreamVideoBuffer::clean() {
    numPFrames = 0;
    hasWriter = 1;
    sizeSPS = 0;
    sizePPS = 0;
    sizeiFrame = 0;
    return true;
}


bool StreamVideoBuffer::addSPS(std::vector<char> data) {
    sizeSPS = data.size();
    std::cout << data.size();
    for(int i = 0; i != data.size(); i++) {
        std::cout << i;
        if (i >= 64) {
            return false;
        }
        //printf("0x%02hhx ", data[i]);
        sps[i] = data[i];
    }
    return true;
}
bool StreamVideoBuffer::addPPS(std::vector<char> data) {
    sizePPS = data.size();
    for(int i = 0; i != data.size(); i++) {
        if (i >= 64) {
            return false;
        }
        //printf("0x%02hhx ", data[i]);
        pps[i] = data[i];
    }
    return true;
}
bool StreamVideoBuffer::addpreIFrame(std::vector<char> data) {
    sizepreIFrame = data.size();
    for(int i = 0; i != data.size(); i++) {
        if (i >= 32000) {
            return false;
        }
        //printf("0x%02hhx ", data[i]);
        preIFrame[i] = data[i];
    }
    return true;
}
bool StreamVideoBuffer::addIFrame(std::vector<char> data) {
    sizeiFrame = data.size();
    std::cout << "I Frame Size = " << data.size() << "\n";
    for(int i = 0; i != data.size(); i++) {
        if (i >= 256000) {
            std::cout << "iFrame too large for buffer! size = " << data.size() << "\n"; 
            return false;
        }
        iFrame[i] = data[i];
    }
    return true;
}
bool StreamVideoBuffer::addpFrame(std::vector<char> data) {
    if (numPFrames == 64) {
        return false;
    }
    assert(&data);
    assert(pFrame);
    assert(pFrame[numPFrames]);
    sizepFrame[numPFrames] = data.size();
    for(int i = 0; i != data.size(); i++) {
        if (i >= 48000) {
            return false;
        }
        assert(&data);
        pFrame[numPFrames][i] = data[i];
    }
    numPFrames++;
    return true;
}