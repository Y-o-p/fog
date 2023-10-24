#include "Volume.h"

Volume::Volume(std::vector<Voxel>&& data, int width, int height, int depth) : 
    m_width(width), m_height(height), m_depth(depth) {
    m_data = data;
    calculate_gradient();
}

Volume::Volume(int width, int height, int depth) :
    m_width(width), m_height(height), m_depth(depth) {
    m_data = std::vector<Voxel>(width * height * depth);
}

Volume::Volume(const Volume &other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_depth = other.m_depth;
    m_data = other.m_data;
}

Volume::~Volume() {}

Voxel& Volume::get_voxel(int x, int y, int z) {
    return m_data[z * m_depth * m_height + (y * m_height + x)];
}

void Volume::calculate_gradient() {
    for (int z = 1; z < m_depth - 1; z++) {
        for (int y = 1; y < m_height - 1; y++) {
            for (int x = 1; x < m_width - 1; x++) {
                Voxel& v = get_voxel(x, y, z);
                
                // X-axis gradient calculation
                Voxel& x_before = get_voxel(x - 1, y, z);
                Voxel& x_after = get_voxel(x + 1, y, z);
                v.gradient.x = x_before.value - x_after.value;

                // Y-axis gradient calculation
                Voxel& y_before = get_voxel(x, y - 1, z);
                Voxel& y_after = get_voxel(x, y + 1, z);
                v.gradient.y = y_before.value - y_after.value;

                // Z-axis gradient calculation
                Voxel& z_before = get_voxel(x, y, z - 1);
                Voxel& z_after = get_voxel(x, y, z + 1);
                v.gradient.z = z_before.value - z_after.value;
            }
        }
    }
}

std::vector<float> Volume::get_raw_values() const {
    auto length = m_data.size();
    std::vector<float> raw = std::vector<float>(length);
    for (int i = 0; i < length; i++) {
        raw[i] = m_data[i].value;
    }
    return raw;
}
