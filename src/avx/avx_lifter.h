#pragma once

#if IDA_SDK_VERSION >= 750

// Check if AVX lifter is available on this platform
bool isMicroAvx_avail();

// Check if AVX lifter is currently active
bool isMicroAvx_active();

// Initialize AVX lifter
void MicroAvx_init();

// Terminate AVX lifter
void MicroAvx_done();

#endif // IDA_SDK_VERSION >= 750
