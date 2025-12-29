#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
//Patcher logic learned and upgraded from ppf project
// Helper to get file size
inline __int64 GetFileSize(int fd) {
    long long current = _lseeki64(fd, 0, SEEK_CUR);
    long long size = _lseeki64(fd, 0, SEEK_END);
    _lseeki64(fd, current, SEEK_SET);
    return size;
}

// The "Clipboard" struct shared between the thread and the UI
struct PatcherState {
    float progress = 0.0f;
    std::string status = "Idle";
    bool isFinished = false;
    bool wasSuccessful = false;
};

// The Function that runs in the background
inline void RunPatchLogic(std::string isoPath, std::string patchPath, bool createBackup, PatcherState* state) {
    state->progress = 0.0f;
    state->isFinished = false;

	// Backup Creation
    if (createBackup) {
        state->status = "Creating Backup (Please Wait)...";
        try {
            std::filesystem::path source(isoPath);
            std::filesystem::path backup = source;
            backup.replace_extension(".bak.iso");
            std::filesystem::copy_file(source, backup, std::filesystem::copy_options::overwrite_existing);
        }
        catch (...) {
            state->status = "Error: Failed to create backup!";
            state->isFinished = true;
            return;
        }
    }

    state->status = "Opening files...";

    // Open ISO for Reading and Writing (Binary)
    int bin = _open(isoPath.c_str(), _O_BINARY | _O_RDWR);
    if (bin == -1) {
        state->status = "Error: Could not open ISO file.";
        state->isFinished = true;
        return;
    }

    // Open Patch for Reading only
    int ppf = _open(patchPath.c_str(), _O_RDONLY | _O_BINARY);
    if (ppf == -1) {
        state->status = "Error: Could not open Patch file.";
        _close(bin);
        state->isFinished = true;
        return;
    }

    // Verify PPF Version 
    unsigned int magic;
    _read(ppf, &magic, 4);
	if (magic != '3FPP') { // little-endian 'PPF3'
        state->status = "Error: Not a valid PPF 3.0 Patch.";
        _close(bin);
        _close(ppf);
        state->isFinished = true;
        return;
    }

    // Determine Start Position
    unsigned char blockcheck;
    _lseeki64(ppf, 57, SEEK_SET);
    _read(ppf, &blockcheck, 1);

    int dataStart = (blockcheck) ? 1084 : 60;

    __int64 patchSize = GetFileSize(ppf);
    _lseeki64(ppf, dataStart, SEEK_SET);

    __int64 bytesProcessed = 0;
    __int64 totalBytesToRead = patchSize - dataStart;

    state->status = "Patching...";

    // Patch Loop
    __int64 offsetInISO;
    unsigned char dataLen;
    std::vector<unsigned char> buffer;

    // We loop until we reach the end of the patch file
    while (_tell(ppf) < patchSize) {
        // Read the target Offset (8 bytes)
        int r1 = _read(ppf, &offsetInISO, 8);
        if (r1 == 0) break; // EOF

        // Read how many bytes to write (1 byte)
        _read(ppf, &dataLen, 1);

        // Resize buffer and read the actual data
        buffer.resize(dataLen);
        _read(ppf, buffer.data(), dataLen);

        // WRITE to ISO
        _lseeki64(bin, offsetInISO, SEEK_SET);
        _write(bin, buffer.data(), dataLen);

        // Update Progress
        bytesProcessed += (9 + dataLen);
        state->progress = (float)bytesProcessed / (float)totalBytesToRead;
    }

    state->status = "Patching Complete!";
    state->wasSuccessful = true;
    state->isFinished = true;
    state->progress = 1.0f;

    _close(bin);
    _close(ppf);
}
