#ifndef MEMORYTYPE_H
#define MEMORYTYPE_H
enum class MemoryType {
    CPU,
    GPU,
    CPU_GPU
};

inline const char* to_string(MemoryType m) {
    switch (m) {
        case MemoryType::CPU:     return "CPU";
        case MemoryType::GPU:     return "GPU";
        case MemoryType::CPU_GPU: return "CPU+GPU";
        default:                  return "Unknown";
    }
}
#endif //MEMORYTYPE_H
